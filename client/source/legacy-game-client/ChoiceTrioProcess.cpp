#include "stdafx.h"
#include "ChoiceTrioProcess.h"
#include "InputManager.h"
#include "UICommonMethod.h"
#include "ChoiceServerProcess.h"
#include "PutPacket.h"
#include "PlayModeManager.h"
#include "GameOptionDialog.h"
#include "UtlLog.h"
#include "ResTableStack.h"
#include "CoreApi.h"
#include "UtlLog.h"
#include "MovieManager.h"
#include "UIFunction.h"

using namespace MovieManager;

CChoiceTrioProcess::CChoiceTrioProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_CHOICETRIO;	
	m_nSelectCharacterIndex = -1;
	m_pObjPlayerInfo = NULL;
	m_bRecvSettingInfo = false;

}

CChoiceTrioProcess::~CChoiceTrioProcess(void)
{
}

void CChoiceTrioProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

    CBaseProcess::Terminate();
}

void CChoiceTrioProcess::InitProcess()
{
    //CObjectManager::GetPtr()->DeleteAllObject(OBJECT_TRIOLIST);
	CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTCHARACTER, NULL);

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	CGameOptionDialog::GetPtr()->XmlSetting();
	CGameOptionDialog::GetPtr()->FUNCTION_SoundInit();
	CGameOptionDialog::GetPtr()->FUNCTION_VideoConfLoad();

	PutNoticeList(0);
	UICommonMethod::DisplayGameVersion();
	//UICommonMethod::DisplayMoney();

	FUNCTION_Init();

    FUNCTION_3DRender( "ChoiceTrioMain" );

	m_nState = STATE_START;
}

void CChoiceTrioProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

    m_cRotateCharacter.Update( fAccumTime );    
    if( m_cRotateCharacter.GetSelectIndex() == -1 ) 
    {
        CObjMemberInfo* pMember	= CObjMemberManager::GetPtr()->GetMemberPointer();
        assert( pMember );

        if( pMember != 0 )
        {
            CResBaseFormat * pLastChar = CObjectManager::GetPtr()->FindObject( OBJECT_TRIOLIST, pMember->m_nLastSeq );
            if( pLastChar == 0 )
            {
                if( CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST) > 0 )
				{
					pLastChar = CObjectManager::GetPtr()->GetSequenceField( OBJECT_TRIOLIST, 0 );
				}
            }

            if( pLastChar != 0 )
            {
                m_cRotateCharacter.SelectCharacterBySeq( pLastChar->GetObjSeq() );
				SelectCharacter(m_cRotateCharacter.GetSelectIndex());
                m_pObjPlayerInfo = (CObjPlayerInfo*) pLastChar;
				CGameOptionDialog::GetPtr()->m_pObjPlayerInfo = m_pObjPlayerInfo;
				
				if( !m_bRecvSettingInfo )
				{
					PutSettingInfo(m_pObjPlayerInfo->GetObjSeq());
					m_bRecvSettingInfo = true;
				}
            }            
        }
    }

	CUIControl* pMenu = CClientUIManager::GetPtr()->FindEventControl("MenuTabButton07");
	assert(pMenu);
	if( m_pObjPlayerInfo == NULL && pMenu->GetVisible() == N3UI_VISIBLE_TRUE )
	{
		UIFunction::Set_UIVisibility( "MenuTabButton07", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "MenuTabButton17", N3UI_VISIBLE_TRUE );
	}
	else if( m_pObjPlayerInfo != NULL && pMenu->GetVisible() == N3UI_VISIBLE_FALSE)
	{
		UIFunction::Set_UIVisibility( "MenuTabButton07", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "MenuTabButton17", N3UI_VISIBLE_FALSE );
	}

	{       
		CGameProject::s_pGameProject->BeginFrame();

        CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
		assert(pObjGroundInfo);    
		pObjGroundInfo->m_pModel->BeforeRender();

		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);

		m_cCharacterTexBoard.Render();

		CGameProject::s_pGameProject->EndFrame();

		CGameProject::s_pGameProject->DisplayFrame();
	}

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
}

void CChoiceTrioProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_INIT_LOAD;
		}
		break;
	case STATE_INIT_LOAD:
		{
			FUNCTION_DateUpdate();

			PutCharacterInfo();

			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{		
//			SetupMapLoading(m_pObjGroundInfo);
			LoadGround();

			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

void CChoiceTrioProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

    NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );
    pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

    pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
    pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
    pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CChoiceTrioProcess::LoadCharacter()
{
	for(int	i = 0; i < CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST); i++)
	{
		CObjPlayerInfo* pAnyPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_TRIOLIST, i));
		assert(pAnyPlayer);	
		if(pAnyPlayer == NULL) return;        
		CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nOrder);

		CProcessManager::GetPtr()->AttachItem(pAnyPlayer);

        NiControllerSequencePtr pkSeq = 0;
        
        //남자와 여자의 애니메이션을 서로 다르게 로드한다.
        switch( pAnyPlayer->m_cShape.m_nGender )
        {
        case 1: //남자
            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_M_back.kf", 0 );
            pkSeq->SetName( "100000000" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_M_front.kf", 0 );
            pkSeq->SetName( "100000001" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_L_back.kf", 0 );
            pkSeq->SetName( "100000002" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_L_front.kf", 0 );
            pkSeq->SetName( "100000003" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_R_back.kf", 0 );
            pkSeq->SetName( "100000004" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/UI_Select_R_front.kf", 0 );
            pkSeq->SetName( "100000005" );
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );
        break;
        
        case 2: //여자
            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_M_back.kf", 0 );
            pkSeq->SetName( "100000000" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_M_front.kf", 0 );
            pkSeq->SetName( "100000001" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_L_back.kf", 0 );
            pkSeq->SetName( "100000002" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_L_front.kf", 0 );
            pkSeq->SetName( "100000003" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_R_back.kf", 0 );
            pkSeq->SetName( "100000004" );        
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );

            pkSeq = NiControllerSequence::CreateSequenceFromFile( "./Animation/Select/Woman_UI_Select_R_front.kf", 0 );
            pkSeq->SetName( "100000005" );
            pAnyPlayer->m_pModel->m_pkControllerManager->AddSequence( pkSeq );
        break;

        default:
            assert(!"Gender Error!");
        break;
        }
    }
}

void CChoiceTrioProcess::Update_CharacterInfo()
{
	CObjMemberInfo* pMember	= CObjMemberManager::GetPtr()->GetMemberPointer();
	assert(pMember);

	//if(CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST) == 0 && pMember->m_nDeleteDate == 0)
	//	UIFunction::Set_UIVisibility("Character_Create_Button01", N3UI_VISIBLE_TRUE);

	this->InitCharacterSelectAnimator();

	m_bIsRender = true;

	char sStatic_CharacterInfo[STRLEN_64];

    CObjectManager * pObjectManager = CObjectManager::GetPtr();
    int nObjectSize = pObjectManager->GetObjectSize( OBJECT_TRIOLIST );

    for( int i=0; i<3; i++ )
    {
        CObjPlayerInfo * pPlayer = 0;

        bool bFind = false;
        for( int j=0; j<nObjectSize; j++ )
        {        
            pPlayer = dynamic_cast<CObjPlayerInfo*>(pObjectManager->GetSequenceField(OBJECT_TRIOLIST, j ));
			assert(pPlayer);   
			if( pPlayer == 0 ) continue;

            if( pPlayer->m_nOrder == i )
            {
                bFind = true;
                break;
            }
        }

        sprintf(sStatic_CharacterInfo,"Static_CharacterInfo%d", i );
        CUIControl* pStatic_CharacterInfo	= CClientUIManager::GetPtr()->FindEventControl(sStatic_CharacterInfo);
        if(pStatic_CharacterInfo == NULL) return;

        CUIControl* pStatic_Position			= pStatic_CharacterInfo->FindEventControl("Static_Position");
        if(pStatic_Position == NULL) return;

        CUIControl* pStatic_Level				= pStatic_CharacterInfo->FindEventControl("Static_Level");
        if(pStatic_Level == NULL) return;

        CUIControl* pStatic_Name				= pStatic_CharacterInfo->FindEventControl("Static_Name");
        if(pStatic_Name == NULL) return;
        
		CResMessageTableFormat* pMTable = NULL;

        if( bFind == true )
        {
            char text[STRLEN_64];

            sprintf(text, UICommonMethod::PositionNtoPositionStr(pPlayer->m_nPosition).c_str());
            pStatic_Position->SetText(text);

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
			assert(pMTable);

			sprintf(text, pMTable->m_sMessage, pPlayer->m_cLevel.m_nLevel);	
            pStatic_Level->SetText(text);

            sprintf(text,"%s",pPlayer->m_sName);	
            pStatic_Name->SetText(text);
        }

        if( bFind == false )
        {
            pStatic_Position->SetText("");
            pStatic_Level->SetText("");

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900039 ,0));
			assert(pMTable);

            pStatic_Name->SetText(pMTable->m_sMessage);
        }
    }

	SoundSetting();

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		if( i<CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST) )
		{
			_snprintf(sText, STRLEN_64, "SelectCharacter0%d", i);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
			_snprintf(sText, STRLEN_64, "CharacterSlot0%d", i);
			UIFunction::SetChildToVisibility( sText, "Silhouette", N3UI_VISIBLE_FALSE );
			UIFunction::SetChildToVisibility( sText, "CharaterCreateBtn", N3UI_VISIBLE_FALSE );	
		}
		else
		{			
			_snprintf(sText, STRLEN_64, "SelectCharacter0%d", i);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
			_snprintf(sText, STRLEN_64, "CharacterSlot0%d", i);
			UIFunction::SetChildToVisibility( sText, "Silhouette", N3UI_VISIBLE_TRUE );
			UIFunction::SetChildToVisibility( sText, "CharaterCreateBtn", N3UI_VISIBLE_TRUE );
		}
	}

    //if(CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST)>=3)
    //{
    //    UIFunction::Set_UIVisibility("Delete_Button",N3UI_VISIBLE_TRUE);
    //    UIFunction::Set_UIVisibility("Charater_Create_Button",N3UI_VISIBLE_FALSE);
    //    UIFunction::Set_UIVisibility("CHOICETRIO_NEXTPAGE_BUTTON",N3UI_VISIBLE_TRUE);
    //}
    //else if(CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST)<=0)
    //{	
    //    UIFunction::Set_UIVisibility("Delete_Button",N3UI_VISIBLE_FALSE);
    //    UIFunction::Set_UIVisibility("Charater_Create_Button",N3UI_VISIBLE_TRUE);
    //    UIFunction::Set_UIVisibility("CHOICETRIO_NEXTPAGE_BUTTON",N3UI_VISIBLE_FALSE);
    //}
    //else
    //{
    //    UIFunction::Set_UIVisibility("Delete_Button",N3UI_VISIBLE_TRUE);
    //    UIFunction::Set_UIVisibility("Charater_Create_Button",N3UI_VISIBLE_TRUE);
    //    UIFunction::Set_UIVisibility("CHOICETRIO_NEXTPAGE_BUTTON",N3UI_VISIBLE_TRUE);
    //}
}


void CChoiceTrioProcess::SelectCharacter( int nIndex )
{
	m_nSelectCharacterIndex = nIndex;

    if( CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST) == 0 )
		return;
	
    //선택된 캐릭터를 뛰어나오게 하고 나와있던 캐릭터를 들어가게 만든다.
	m_cRotateCharacter.SelectCharacter( nIndex );

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_TRIOLIST, m_cRotateCharacter.GetSelectSequence()));

	if( CObjectManager::GetPtr()->GetObjectSize(OBJECT_TRIOLIST) != 0 && m_pObjPlayerInfo == NULL )
		return;

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		_snprintf( sText, STRLEN_64, "Static_CharacterInfo%d", i );	
		
		UIFunction::SetChildToVisibility( sText, "Bg", N3UI_VISIBLE_TRUE);
		UIFunction::SetChildToVisibility( sText, "Bg01", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility( sText, "SelectBtn", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility( sText, "DelBtn", N3UI_VISIBLE_FALSE);
	}

	_snprintf( sText, STRLEN_64, "Static_CharacterInfo%d", nIndex );	
	
	UIFunction::SetChildToVisibility( sText, "Bg", N3UI_VISIBLE_FALSE);
	UIFunction::SetChildToVisibility( sText, "Bg01", N3UI_VISIBLE_TRUE);
	UIFunction::SetChildToVisibility( sText, "SelectBtn", N3UI_VISIBLE_TRUE);
	UIFunction::SetChildToVisibility( sText, "DelBtn", N3UI_VISIBLE_TRUE);

	CGameOptionDialog::GetPtr()->m_pObjPlayerInfo = m_pObjPlayerInfo;

	PutSettingInfo(m_pObjPlayerInfo->GetObjSeq());
}

void CChoiceTrioProcess::Select_Character()
{
    int nSelectSeq = m_cRotateCharacter.GetSelectSequence();
    if( nSelectSeq == -1 ) return;

	PutSettingInfo(m_pObjPlayerInfo->GetObjSeq());
	PutChoiceCharacter(m_pObjPlayerInfo->GetObjSeq());

	CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTCHARACTER, m_pObjPlayerInfo);
}

void CChoiceTrioProcess::SoundSetting()
{
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();				// 계정별로 저장되어있는 셋팅값
	assert(pMember);

	if( pMember->m_nLastSeq != 0 )
		m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_TRIOLIST, pMember->m_nLastSeq));

	CGameOptionDialog::GetPtr()->m_pObjPlayerInfo = m_pObjPlayerInfo;

	TiXmlElement*	pGameSoundXml	= CGameProject::s_pGameProject->GetTiXml("OptionSound");
	TiXmlNode*		pChild			= pGameSoundXml->FirstChild( "BGSound" );
	TiXmlElement*	pElement		= (TiXmlElement*)pChild;

	CAudioManager::GetPtr()->MusicVolume( (float)atof( pElement->Attribute( "Volume" ) ) );
	SetVolume( (float)atof( pElement->Attribute( "Volume" ) ) );

	// 효과음 설정
	pChild = pChild->NextSibling( "Sound" );
	pElement = (TiXmlElement*)pChild;

	CAudioManager::GetPtr()->SoundVolume( (float)atoi( pElement->Attribute( "Volume" ) ) / 10 );

	CGameOptionDialog::GetPtr()->XmlSetting();
	
	CGameOptionDialog::GetPtr()->FUNCTION_BGMScriptSettingInit();
	// 배경음 셋팅
	if(strlen(CGameOptionDialog::GetPtr()->LoadUserBGM(0)))
	{
		CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\Music\\");
		CAudioManager::GetPtr()->CreateMusic(CGameOptionDialog::GetPtr()->LoadUserBGM(0), true);//사운드 플레이
		CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\BGM\\");
	}
	else
	{
		CResOggTableFormat *temp = dynamic_cast<CResOggTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OGGTABLE, 0 ,0));
		assert(temp);
	
		CAudioManager::GetPtr()->CreateMusic(temp->m_sDataFile, true);//사운드 플레이
	}
}

void CChoiceTrioProcess::Select_Delete()
{
	int nSelectSeq = m_cRotateCharacter.GetSelectSequence();
    if( nSelectSeq == -1 ) 
		return;
	
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "ImeText" );
	assert(pControl);

	CCSDeleteCharacter pPacket;
	pPacket.m_nPlayerSeq = nSelectSeq;
	_snprintf(pPacket.m_sJumin, 20, "%s", pControl->GetTextEx().c_str() );
	PutDeleteCharacter(&pPacket);
}

bool CChoiceTrioProcess::OnDefault(NiEventRef pEventRecord)
{
	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

bool CChoiceTrioProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CChoiceTrioProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

CBaseProcess* CChoiceTrioProcess::GetSameProcess()
{
	return new CChoiceTrioProcess;
}

void CChoiceTrioProcess::InitCharacterSelectAnimator()
{
    m_cRotateCharacter.Reset();

    CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

    CResGroundFormat *pGround = dynamic_cast<CResGroundFormat*>(pObjGroundInfo->m_pModel);
	assert(pGround); 
	if(pGround == NULL) return;

    CObjectManager * pObjectManager = CObjectManager::GetPtr();
    int nObjectSize = pObjectManager->GetObjectSize( OBJECT_TRIOLIST );

    for( int i=0; i<nObjectSize; i++ )
    {
        CObjPlayerInfo* pAnyPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->GetSequenceField( OBJECT_TRIOLIST, i ));
		assert(pAnyPlayer);     
		if( pAnyPlayer == 0 ) continue;

        CResCharacterFormat * pModel = pAnyPlayer->m_pModel;
        if( pModel == 0 ) continue;

        int nOrder = pAnyPlayer->m_nOrder;

        switch( pAnyPlayer->m_nOrder )
        {
        case 0:
            // nOrder : 슬롯번호, 100000001 : 뛰어나오는 애니메이션번호  100000000 : 뛰어들어가는 애니메이션번호, 70011100 : IDLE Animation 번호, 0.0f : 초기회전각도
            m_cRotateCharacter.SetCharacter( pModel, nOrder, 100000001, 100000000, 70011100, 0.0f ); 
        break;

        case 1:
            m_cRotateCharacter.SetCharacter( pModel, nOrder, 100000005, 100000004, 71011100, -10.0f );
        break;

        case 2:
            m_cRotateCharacter.SetCharacter( pModel, nOrder, 100000003, 100000002, 70011100, 10.0f );
        break;
        }
    }

    NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

    NiIntegerExtraData kShadowExtraData(1);
    kShadowExtraData.SetName( "Shadow" );

    CoreApi_RecursiveSetExtraDataToGeometry( pkCasterNode, &kShadowExtraData );    

    pGround->m_spNode->Update( 0.0f );
}

void CChoiceTrioProcess::FUNCTION_DateUpdate()
{
	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();

	// 최종 삭제일
	if(pMember->m_nDeleteDate == 0)
		UIFunction::Set_ControlText( "Character_Delete_Date", "0000-00-00 00:00:00" );
	else
		UIFunction::Set_ControlText( "Character_Delete_Date", TimestampToString(pMember->m_nDeleteDate).c_str() );

	// 최종 접속일
	UIFunction::Set_ControlText( "Character_Last_Date", TimestampToString(pMember->m_nLoginDate).c_str() );
}

void CChoiceTrioProcess::FUNCTION_Init()
{
	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "CharacterSlot0%d", i);
		UIFunction::SetChildToVisibility( sText, "Silhouette", N3UI_VISIBLE_TRUE );
		
		_snprintf( sText, STRLEN_64, "Static_CharacterInfo%d", i );	
		
		UIFunction::SetChildToVisibility( sText, "Bg", N3UI_VISIBLE_TRUE);
		UIFunction::SetChildToVisibility( sText, "Bg01", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility( sText, "SelectBtn", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility( sText, "DelBtn", N3UI_VISIBLE_FALSE);

		_snprintf(sText, STRLEN_64, "SelectCharacter0%d", i);
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	}
}