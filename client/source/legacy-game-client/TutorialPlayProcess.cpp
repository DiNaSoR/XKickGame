#include "stdafx.h"
#include "TutorialPlayProcess.h"
#include "FblArea.h"
#include "ObjPlayerManager.h"
#include "ObjRoomManager.h"
#include "UICommonMethod.h"
#include "ResBallFormat.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "FootballManager.h"
#include "PlayModeManager.h"
#include "DutyMachine_Base.h"
#include "PutPacket.h"
#include "UtlLog.h"
#include "EffectManager.h"
#include "ReadyRoomProcess.h"
#include "UIFunction.h"
#include "TutorialMainProcess.h"
#include "GuideTutorialProcess.h"

CTutorialPlayProcess::CTutorialPlayProcess() : CBasePlayProcess()
{
	m_nInputKeySendTime = 0;
	m_nTutorialType = TUTORIAL_1TYPE;

	m_bIsRender = false;
	m_nTutorialStep1MaxCount = 5;

	m_nTryTotalCount = 0;//총 시도 횟수

	m_nTrySuccessCount = 0;//시도 성공 횟수

	m_bTryProcess = false;

	m_bTutorialStep1End = false;

	m_pPannelManager = NULL;

    m_fAccumTime = 0.0f;

	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		m_ppKeyUIControl[i] = NULL;
	}

	m_bAssistant = true;

	m_bStart = false;
	m_bShadow = true;
}

CTutorialPlayProcess::~CTutorialPlayProcess(void)
{
}

void CTutorialPlayProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	char sText[STRLEN_64]="";
	sprintf(sText,".\\Script\\Tutorial%d.xml", m_nTutorialType);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, sText, m_nTutorialType);

	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_pRoom->SetParentSeq(pSelectPlayerInfo->GetObjSeq());

	m_pSelectPlayerInfo = pSelectPlayerInfo->Clone();

	CObjPlayerManager::GetPtr()->SetPlayerLists(m_pSelectPlayerInfo, m_pSelectPlayerInfo->m_nTeam);

	LoadGround();

	LoadCharacter();

	m_pPannelManager = new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode, pObjGroundInfo->m_pModel->m_spLightNode);

	m_nTime = timeGetTime();

	m_pAssistantPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_STAY);

	m_bAssistant = true;

	m_nState = STATE_START;
}

void CTutorialPlayProcess::FUNCTION_InitKeyUIControl()
{
	char temp[STRLEN_128];

	for(int i = 0; i < ARRAY_KEY_LEFTSHOOT; i++)
	{
		sprintf(temp,"Static_DirectionKey_%d", i);
		m_ppKeyUIControl[i] = CClientUIManager::GetPtr()->FindEventControl(temp);
	}

	for(int i = 0; i < 11; i++)
		UIFunction::Set_ChildNumVisibility("Dialog_TutoObject", "Static_TutoHotKey0", i+101, N3UI_VISIBLE_FALSE);

	CUIControl* pControl = UIFunction::Set_ChildNumVisibility("Dialog_TutoObject", "Static_TutoHotKey0", m_nTutorialType, N3UI_VISIBLE_TRUE);
	if(pControl == NULL) return;

	CUIControl* pTutoHotKeyBg = pControl->FindEventControl("Static_TutoHotKeyBg");
	if(pTutoHotKeyBg == NULL) return;

	for(int i = ARRAY_KEY_LEFTSHOOT; i < ARRAY_KEY_SIZE; i++)
	{
		sprintf(temp,"Static_TutoHotKey_%d", i);
		m_ppKeyUIControl[i] = pTutoHotKeyBg->FindEventControl(temp);
	}
}

void CTutorialPlayProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CEffectManager::GetPtr()->Update( fAccumTime );
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
}

void CTutorialPlayProcess::ProcessInput()
{
	//CBasePlayProcess::ProcessInput();

	//if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_M))
	//{
	//	m_bStart = true;
	//}
}

void CTutorialPlayProcess::Update(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->GetObjectNode()->Update(fAccumTime);
	CCharacterCollisionDetector::GetPtr()->Update(fFrameTime, fAccumTime);

	m_pPannelManager->InitPannelPoint();

	m_pPannelManager->Update(true);

	const DWORD& nRemainTime = m_pRoom->GetPlayTime();
}

void CTutorialPlayProcess::Terminate()
{
	CCameraController::GetPtr()->ChangeCamera();

	CBasePlayProcess::Terminate();

	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);
}

void CTutorialPlayProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	//m_pPannelManager=new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode,pObjGroundInfo->m_pModel->m_spLightNode);
}

void CTutorialPlayProcess::LoadCharacter()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);

			if(pAnyPlayer->m_nTeam == PLAYER_TEAM_VIEW)	continue;
			if(pAnyPlayer == NULL)						continue;
			if(pAnyPlayer->m_pModel)					continue;

			if(m_pSelectPlayerInfo->GetObjSeq() == m_pRoom->m_nParentSeq) CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, pAnyPlayer->GetKind());
			else CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, 0);

			if(pAnyPlayer->m_nPosition == POSITION_GK)
				pAnyPlayer->m_pModel->m_spCollisionNode = LoadCollision(pAnyPlayer->GetRootNode());

			CProcessManager::GetPtr()->AttachItem(pAnyPlayer); 	
		}
	}
	CObjectModelManager::GetPtr()->ShadowOnOff(true, true);
}

NiNodePtr CTutorialPlayProcess::LoadCollision(NiNode *pRootNode)
{
	NiStream kStream;
	char	FullName[STRLEN_128];

	sprintf(FullName, "Object\\Goal_Collision.nif");

	if( kStream.Load( FullName ) == false )
		return NULL;

	NiNodePtr spGroundObjectNode = (NiNode*)kStream.GetObjectAt(0);
	kStream.RemoveAllObjects();

	if(pRootNode)
	{
		pRootNode->AttachChild(spGroundObjectNode);//
		spGroundObjectNode->SetAppCulled(true);
		pRootNode->Update(0.0f);
		pRootNode->UpdateNodeBound();
		pRootNode->UpdateProperties();
		pRootNode->UpdateEffects();
	}

	return spGroundObjectNode;
}

//튜토리얼 공통적인 내용
void CTutorialPlayProcess::FUNCTION_UpdateScoreUI(bool bSuccess)
{
	char text[MAX_PATH];

	sprintf(text, "ONum_%d", m_nTryTotalCount);
	UIFunction::ChangeTexture("Static_CounterCurrent", text);

	sprintf(text, "ONum_%d", m_nTutorialStep1MaxCount);
	UIFunction::ChangeTexture("Static_CounterTotal", text);

	sprintf(text, "Static_Counter_IconBg%d", m_nTryTotalCount - 1);
	CUIControl* pTarget = CClientUIManager::GetPtr()->FindEventControl(text);
	assert(pTarget);

	if(bSuccess)	UIFunction::ChangeTexture(pTarget, "Static_Counter_Green");
	else 			UIFunction::ChangeTexture(pTarget, "Static_Counter_Red");
}

void CTutorialPlayProcess::FUNCTION_UpdateKeyUI()
{
	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		if(m_ppKeyUIControl[i])
		{
			if(CInputManager::GetPtr()->m_CInputAdapter.Get_ControlKeyState(i) != KEY_NONE)
				m_ppKeyUIControl[i]->SetVisible(N3UI_VISIBLE_TRUE);
			else
				m_ppKeyUIControl[i]->SetVisible(N3UI_VISIBLE_FALSE);
		}
	}
}

void CTutorialPlayProcess::FUNCTION_ExplainUIInit()
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "ExitBtn", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ReChallengeBtn", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "NextBtn", N3UI_VISIBLE_TRUE );
	
	UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_FALSE );

	_snprintf( sText, STRLEN_64, "TutorialTitle_0%d", m_nTutorialType%100 );
	UIFunction::ChangeTexture( "TutorialKindTitle", sText );

	if( m_bAssistant )
	{
		for(int i = 0; i < m_nTutorialStep1MaxCount; i++)
		{
			_snprintf(sText, STRLEN_64, "Static_Counter_IconBg%d", i);
			CUIControl* pTarget = CClientUIManager::GetPtr()->FindEventControl(sText);

			UIFunction::ChangeTexture(pTarget, "Static_Counter_Default");
		}
	}
}

void CTutorialPlayProcess::FUNCTION_KeyUIInit()
{
	UIFunction::Set_UIVisibility( "Key_Q0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_W0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_E0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_A0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_TRUE );

	UIFunction::Set_UIVisibility( "Key_Q", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_W", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_E", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_A", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_S", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_D", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_F", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Key_Q1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_W1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_E1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_A1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_S1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_D1", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_F1", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );
}

void CTutorialPlayProcess::FUNCTION_DemonstrationGameUIInit()
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_TRUE );

	if( m_bAssistant )
		UIFunction::Set_UIVisibility( "AssistPlay", N3UI_VISIBLE_TRUE );
}	

void CTutorialPlayProcess::FUNCTION_GamePlayExplainUIInit()
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );

	if( m_nTutorialType != 101 )
		UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	_snprintf(sText, STRLEN_64, "ONum_%d", m_nTryTotalCount);
	UIFunction::ChangeTexture("Static_CounterCurrent", sText);

	_snprintf(sText, STRLEN_64, "ONum_%d", m_nTutorialStep1MaxCount);
	UIFunction::ChangeTexture("Static_CounterTotal", sText);

	UIFunction::Set_UIVisibility( "AssistPlay", N3UI_VISIBLE_FALSE );
}

void CTutorialPlayProcess::FUNCTION_GamePlayUIInit()
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_TRUE );
}

void CTutorialPlayProcess::FUNCTION_ResultUIInit(bool bSuccess)
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ExitBtn", N3UI_VISIBLE_TRUE );
	
	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	if( bSuccess )
	{
		int nCnt = 0;

		if((int)pSelectPlayerInfo->m_vQuestList.size())
		{
			for(int i=0;i<(int)pSelectPlayerInfo->m_vQuestList.size();i++)
			{
				if( pSelectPlayerInfo->m_vQuestList.at(i)->m_nCode == m_nTutorialType )
					break;
				nCnt++;
			}
		}

		UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
		CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
		assert(pBG);

		UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent08"  );
		CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
		assert(pShadowLine);

		if( !pShadowLine->IsPlay() && m_bShadow )
		{
			pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
			pShadowLine->SetInitUpdate(false);
			pShadowLine->SetPlay(true, false, true, 0);

			m_bShadow = false;
		}

		if( nCnt == (int)pSelectPlayerInfo->m_vQuestList.size() )
		{
			UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_TRUE );
			//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_TRUE );
		}
		else
		{
			UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_FALSE );
			//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_FALSE );
		}

		UIFunction::Set_UIVisibility( "ReChallengeBtn", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "NextBtn", N3UI_VISIBLE_TRUE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
		CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
		assert(pBG);

		UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent09"  );
		CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
		assert(pShadowLine);

		if( !pShadowLine->IsPlay() && m_bShadow )
		{
			pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
			pShadowLine->SetInitUpdate(false);
			pShadowLine->SetPlay(true, false, true, 0);

			m_bShadow = false;
		}

		UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_FALSE );
		//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "ReChallengeBtn", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "NextBtn", N3UI_VISIBLE_FALSE );
	}
}

void CTutorialPlayProcess::FUNCTION_NextTutorial()
{
	int nCurrentTutorial = m_nTutorialType;

	nCurrentTutorial++;

	int nCnt = 0;
	
	if( 104 < nCurrentTutorial )
		nCurrentTutorial = 100;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	if((int)pObjPlayerInfo->m_vQuestList.size())
	{
        for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++ )
		{
			if( pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == nCurrentTutorial )
			{
				nCnt++;

				if( nCnt == 5)
					break;

				i=-1;
				nCurrentTutorial++;
				
				if( 104 < nCurrentTutorial )
					nCurrentTutorial = 100;
				
				continue;
			}
		}
	}

	if( nCnt == 5 )
		CProcessManager::GetPtr()->SetNextProcess(new CTutorialMainProcess);
	else
	{
		if( nCurrentTutorial == 100 )
			CProcessManager::GetPtr()->SetNextProcess(new CGuideTutorialProcess(14));
		else
			CProcessManager::GetPtr()->ChangeTutorialeMachine(nCurrentTutorial);
	}
}

void CTutorialPlayProcess::SetEndingAnimation()
{
	VectorAnyoneList* pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);

	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();	

	if(m_nTrySuccessCount > 0)
	{
		if(pMember->m_nTutorial > 62)
			m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1073, 0));//성공일때
		else
			m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1071 + Random(2), 0));//성공일때
	}
	else
		m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1074, 0));//실패일때

	assert(m_pCeremonyTable);

	int i = 1;
	int j = 0;

	CCameraController::GetPtr()->ChangeCameraType(CEREMONYFIX, m_pCeremonyTable->m_sCameraFile, m_pCeremonyTable->m_nNif);
	SetShadowCamera();

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			if((*iPlayer)->m_nTeam == PLAYER_TEAM_VIEW)
				continue;

			if((*iPlayer)->m_nTeam == PLAYER_TEAM_STAY)
				continue;

			CObjPlayerInfo* pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->GetObjSeq() == m_pSelectPlayerInfo->GetObjSeq()) 
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[0];
			}
			else if(pAnyPlayer->m_nTeam == m_pSelectPlayerInfo->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[i];
				i++;
			}
			else if(pAnyPlayer->m_nTeam != m_pSelectPlayerInfo->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nLoser[j];
				j++;
			}
		}
	}

	m_nTime = timeGetTime();
}

void CTutorialPlayProcess::UpdateStateStart(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	CBasePlayProcess::UpdateStateStart(fFrameTime, fAccumTime);

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_LOADING;
}

void CTutorialPlayProcess::UpdateStateLoading(float fFrameTime, float fAccumTime)
{
	CObjBallManager::GetPtr()->GetBallPointer()->SetCurrentPoint();
	m_nState = STATE_GAME_SETTING;
}
void CTutorialPlayProcess::UpdateStateRestart(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_SETTING;
}
void CTutorialPlayProcess::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameSetting(fFrameTime, fAccumTime);

	CObjPlayerManager::GetPtr()->CreateAIMachine(1);//게임 초기화 부분

	CFblEngine::GetPtr()->SetMeParent(true);

	//FUNCTION_InitKeyUIControl();

	//FUNCTION_UpdateScoreUI(true);	

	//FUNCTION_UIInit();

	m_nState = STATE_GAME_INIT;
}
void CTutorialPlayProcess::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);

//#ifdef __AITYPE__
	CAIManager::GetPtr()->Init(CFblArea::GetPtr()->GetGroundX(), CFblArea::GetPtr()->GetGroundY(), CFblArea::GetPtr()->GetPenaltyRadius(), false);
//#else
//#endif

	CCameraController::GetPtr()->ChangeCameraType(D4WAYTILTMOVE);
	CCameraController::GetPtr()->ChangeCameraTarget(m_nCameraTarget);
	SetShadowCamera();

	DemonstrationGameInit();

	m_pPannelManager->Update(false);

	m_bTryProcess = false;
}

void CTutorialPlayProcess::DemonstrationGameInit()
{
	if(m_bAssistant)
	{
		m_pAssistantPlayer->m_pModel->m_spNode->SetAppCulled(false);

		CObjPlayerManager::GetPtr()->ChangeList(m_pSelectPlayerInfo, PLAYER_TEAM_STAY);//내꺼 스테이에 넣기
		CObjPlayerManager::GetPtr()->ChangeList(m_pAssistantPlayer, PLAYER_TEAM_HOME);//조교 팀에 넣기

		m_pAssistantPlayer->m_nTeam = PLAYER_TEAM_HOME;
		m_pAssistantPlayer->m_nForm = m_pSelectPlayerInfo->m_nForm;
		//m_pAssistantPlayer->SetNodePoint(m_pSelectPlayerInfo->GetNodePoint());
		m_pAssistantPlayer->CreateAIMachine(1);
		CCameraController::GetPtr()->SetCameraForm(m_pAssistantPlayer->m_nForm);
		CCameraController::GetPtr()->CameraSetting();

		m_pSelectPlayerInfo->m_nTeam = PLAYER_TEAM_STAY;
		m_pSelectPlayerInfo->m_pModel->m_spNode->SetAppCulled(true);
		//m_pSelectPlayerInfo->SetNodePoint(NiPoint3(100,0,0));

		CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + m_pAssistantPlayer->m_nPosition*100 + 50, 0));
		assert(pTempAbility);

		CFaculty cFaculty;
		pTempAbility->SetValue(m_pAssistantPlayer->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, m_pAssistantPlayer->m_cTrainingFaculty.m_nFaculty, 0);

		//조교 케릭터 홈팀으로 변경후 위치 해당위치로 변경
		//나 스테이로 변경후 위치 Z갑 -10000으로 변경
		//카메라 타겟은 조교로 한다
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_STAY)
	{
		m_pSelectPlayerInfo->m_pModel->m_spNode->SetAppCulled(false);

		CObjPlayerManager::GetPtr()->ChangeList(m_pSelectPlayerInfo, PLAYER_TEAM_HOME);//조교 스테이에 넣기
		CObjPlayerManager::GetPtr()->ChangeList(m_pAssistantPlayer, PLAYER_TEAM_STAY);//내꺼 팀에 넣기

		m_pSelectPlayerInfo->m_nTeam = PLAYER_TEAM_HOME;
		m_pSelectPlayerInfo->m_nForm = m_pAssistantPlayer->m_nForm;
		//m_pSelectPlayerInfo->SetNodePoint(m_pAssistantPlayer->GetNodePoint());
		CCameraController::GetPtr()->SetCameraForm(m_pSelectPlayerInfo->m_nForm);
		CCameraController::GetPtr()->CameraSetting();

		m_pAssistantPlayer->m_nTeam = PLAYER_TEAM_STAY;
		m_pAssistantPlayer->m_pModel->m_spNode->SetAppCulled(true);
		//m_pAssistantPlayer->SetNodePoint(NiPoint3(100,0,0));

		CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + m_pAssistantPlayer->m_nPosition*100 + 50, 0));
		assert(pTempAbility);

		CFaculty cFaculty;
		pTempAbility->SetValue(m_pSelectPlayerInfo->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, m_pSelectPlayerInfo->m_cTrainingFaculty.m_nFaculty, 0);

		//나 케릭터 홈팀으로 변경후 위치 해당위치로 변경
		//조교 스테이로 변경후 위치 Z갑 -10000으로 변경
		//카메라 타겟은 나로 한다
	}
	else m_pAssistantPlayer->SetNodePoint(NiPoint3(100,0,0));
}

void CTutorialPlayProcess::UpdateStateGameReady(float fFrameTime, float fAccumTime)
{
	//초기 튜토리얼 입장 모습
	CBasePlayProcess::UpdateStateGameReady(fFrameTime, fAccumTime);
    ClothSimulationManager::GetPtr()->Update( fAccumTime );

	FUNCTION_ExplainUIInit();

	if(m_bStart)
	{
		FUNCTION_DemonstrationGameUIInit();

		CInputManager::GetPtr()->InitInputKey();

		UIFunction::Set_UIVisibility("Static_Start", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP); //START 연출씬 세팅
		m_nTime = timeGetTime();

		NormalSound("game_start1.wav"); //Sound 세팅

		m_nState = STATE_GAME_START;
	}
}
void CTutorialPlayProcess::UpdateStateGameStart(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	Update(fFrameTime, fAccumTime);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsPlayStartEnd() && m_bStart)
	{
		CObjPlayerManager::GetPtr()->SetUpdateAIMachine(true);

		m_nState = STATE_GAME_PLAY;
//		FUNCTION_ChangeTutorialPlayUI();

		m_nTutorialTime = timeGetTime();

		if( !m_bAssistant )
			FUNCTION_GamePlayUIInit();
	}
}
void CTutorialPlayProcess::UpdateStateGamePlay(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	CObjBallInfo*	pBall = CObjBallManager::GetPtr()->GetBallPointer();

	Update(fFrameTime, fAccumTime);

	if(!m_bTryProcess || (m_nTutorialType == 104 || m_nTutorialType == 108))//다이렉트랑 걷어내기
		InputKey();

	CObjPlayerManager::GetPtr()->Update(fFrameTime, fAccumTime);

//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
//#else
//#endif

	CObjBallManager::GetPtr()->Update();

	if(CInputManager::GetPtr()->GetSoundPlay())
	{
		CAudioManager::GetPtr()->Create2DSound("gui_ok.wav");
	}

	FUNCTION_UpdateKeyUI();

	if(m_bAssistant) DemonstrationPlay(fFrameTime, fAccumTime);//시범 플레이
	else GamePlay(fFrameTime, fAccumTime);//시범 플레이	

	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	CInputManager::GetPtr()->Update();
}

void CTutorialPlayProcess::DemonstrationPlay(float fFrameTime, float fAccumTime)
{
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
}

void CTutorialPlayProcess::GamePlay(float fFrameTime, float fAccumTime)
{
	if(!m_bTryProcess && TryProcess())
	{
		m_nTime = timeGetTime();
		m_bTryProcess = true;
	}

	if(m_bTryProcess && SuccessProcess())//성공은 즉시 처리
	{
//		CFblEngine::GetPtr()->m_bIsGoalState = true;

		FUNCTION_UpdateScoreUI(true);

		//성공
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Success" );
		assert(pControl);
		
		pControl->SetVisible(N3UI_VISIBLE_TRUE);
		pControl->SetPlay( true, false, false, 0 );
		//UIFunction::Set_UIVisibility("Static_Success", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//성공 연출씬 세팅

		//CObjPlayerManager::GetPtr()->SetPlayerAnimation(PLAYER_TEAM_STAY, ANIMATION_SUCCESS);

		CInputManager::GetPtr()->InitInputKey();
		m_pSelectPlayerInfo->m_cInputKey.InitCommand();
		m_pSelectPlayerInfo->InitDirectKey();

		m_nTime = timeGetTime();

		m_nState = STATE_GOAL_PLAY;	
	}
	else if(m_bTryProcess && timeGetTime() - m_nTime > 3500)
	{
		FailProcess();

		FUNCTION_UpdateScoreUI(false);

		//실패
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Failure" );
		assert(pControl);

		pControl->SetVisible(N3UI_VISIBLE_TRUE);
		pControl->SetPlay( true, false, false, 0 );
		//UIFunction::Set_UIVisibility("Static_Retry", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//실패 연출씬 세팅

		//CObjPlayerManager::GetPtr()->SetPlayerAnimation(PLAYER_TEAM_STAY, ANIMATION_FAILURE);

		CInputManager::GetPtr()->InitInputKey();
		m_pSelectPlayerInfo->m_cInputKey.InitCommand();
		m_pSelectPlayerInfo->InitDirectKey();

		CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);

		m_nTime = timeGetTime();

		m_nState = STATE_GOAL_PLAY;
	}
	else 
	{
		CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	}
}

void CTutorialPlayProcess::UpdateStateGoalPlay(float fFrameTime, float fAccumTime)
{
	Update(fFrameTime, fAccumTime);
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
//#else
//#endif

	CObjBallManager::GetPtr()->Update();
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(timeGetTime() - m_nTime > 2000)
	{
		if(m_nTutorialStep1MaxCount <= m_nTryTotalCount)
		{
			//결과 패킷 보내기
			if( ( m_nTutorialType != 104 && m_nTrySuccessCount > 2 ) || ( m_nTutorialType == 104 && m_nTrySuccessCount > 0 ) )
			{
				FUNCTION_ResultUIInit(true);

				CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
				if(pMember == NULL) return;

				CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

				if((int)pSelectPlayerInfo->m_vQuestList.size())
				{
					int nCnt = 0;
					for(int i=0;i<(int)pSelectPlayerInfo->m_vQuestList.size();i++)
					{
						if( pSelectPlayerInfo->m_vQuestList.at(i)->m_nCode == m_nTutorialType )
							break;
						nCnt++;
						if( nCnt == (int)pSelectPlayerInfo->m_vQuestList.size() )
						{
							CObjQuestInfo* pQuestList = new CObjQuestInfo();
							pQuestList->m_nAmount	+= 1;
							pQuestList->m_nCode		= m_nTutorialType;
							
							pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
							PutQuestReward((short)m_nTutorialType);
							break;
						}
					}
					m_nState = STATE_GAME_END_READY;
					return;
				}
				else
				{
					CObjQuestInfo* pQuestList = new CObjQuestInfo();
					pQuestList->m_nAmount	+= 1;
					pQuestList->m_nCode		= m_nTutorialType;
					
					pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
					PutQuestReward((short)m_nTutorialType);
					m_nState = STATE_GAME_END_READY;
					return;
				}
			}
			else
				FUNCTION_ResultUIInit(false);

			m_nTime = timeGetTime();
		}
		else 
		{
			CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);
			m_nState = STATE_GAME_INIT;
		}
	}
}
void CTutorialPlayProcess::UpdateStateReplayReady(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_REPLAY_PLAY;
}
void CTutorialPlayProcess::UpdateStateReplayPlay(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_CEREMONY_PLAY;
}
void CTutorialPlayProcess::UpdateStateCeremonyPlay(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_END_READY;
}
void CTutorialPlayProcess::UpdateStateGameEndReady(float fFrameTime, float fAccumTime)
{
	//m_nState = STATE_GAME_END;
}
void CTutorialPlayProcess::UpdateStateGameEnd(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	NormalSound("game_end1.wav"); //Sound 세팅
	SetEndingAnimation();
    ClothSimulationManager::GetPtr()->Reset();

	m_nState = STATE_END;
}
void CTutorialPlayProcess::UpdateStateEnd(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	Update(fFrameTime, fAccumTime);
    ClothSimulationManager::GetPtr()->Reset();

	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(!m_bTutorialStep1End && timeGetTime() - m_nTime > 2000)
	{
		m_bTutorialStep1End = true;
		PutGameResult();
	}
}