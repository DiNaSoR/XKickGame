//***********************************************************************************
//	File		:	PlayerInformationDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "StdAfx.h"
#include "PlayerInformationDialog.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "../InterfaceLibrary/UIButton.h"
#include "../InterfaceLibrary/UIIMEEDITBox.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "UICommonMethod.h"
#include "PutPacket.h"
#include "CoreApi.h"
#include "UICommon.h"
#include "UIFunction.h"
#include "ResTableStack.h"

CPlayerInformationDialog::CPlayerInformationDialog(void)
{
    m_nBigFractionIndex = 0;

    m_pCharacter = 0;
    m_pSelectPlayerInfo = 0;

    m_bIsMeInfo = false;

    m_nCurrentTradePage			= 0;
    m_nCurrentTradePeriod		= 0;
    m_nCurrentTradeTotalPage	= 0;
	
	m_nConsumPage				= 0;

	m_nAthletePage				= 0;
	m_nAthleteTotalPage			= 0;

	m_nSmallFractionStart	= 0;
	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호
}

CPlayerInformationDialog::~CPlayerInformationDialog(void)
{
    m_spRootNode = 0;
    m_spCamera = 0;

    SAFE_DELETE( m_pCharacter );
}

void CPlayerInformationDialog::SetPlayer( CObjPlayerInfo * pPlayerInfo )
{
    assert( pPlayerInfo != 0 );
    if( pPlayerInfo == 0 ) return;

    m_pSelectPlayerInfo = pPlayerInfo;

    CObjPlayerInfo * pMeInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pMeInfo);

	if( pMeInfo == m_pSelectPlayerInfo )
        m_bIsMeInfo = true;
    else
        m_bIsMeInfo = false;

    this->FUNCTION_PlayerInformationInit();
}

void CPlayerInformationDialog::FUNCTION_PlayerInformationInit()
{
    FUNCTION_InitLinePoint();

    if( m_pSelectPlayerInfo == 0 ) 
    {
        m_pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
		assert(m_pSelectPlayerInfo);     
		m_bIsMeInfo = true;
    }

    CUIControl* pControl		= NULL;
    CUIControl* pMentButton		= CClientUIManager::GetPtr()->FindEventControl( "Button_MentChange" );
    CUIControl* pMentEditBox	= CClientUIManager::GetPtr()->FindEventControl( "ment" );
    CUIControl* pIcon_Levelup	= CClientUIManager::GetPtr()->FindEventControl( "Icon_LevelUp" );

    assert( pIcon_Levelup && pMentButton && pMentEditBox );

    m_cSaleInfoArray.clear();

	CResMessageTableFormat* pMTable = NULL;

	char sText[MAX_PATH]="";

	// 자신의 정보를 볼때
    if( m_bIsMeInfo == true )
    {
		for(int i=1;i<4;i++)
		{
			_snprintf( sText, MAX_PATH, "Static_PlayerInfo_Tab%d", i );
			pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);
			pControl->SetEnabled(true);

			if( i==3 ) continue;
			_snprintf( sText, MAX_PATH, "PlayerInfo_SubTab0%d", i );
			pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);
			pControl->SetEnabled(true);			
		}

		pMentButton->SetEnabled( true );
        pMentEditBox->SetEnabled( true );        

		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900063 ,0));
		assert(pMTable);

		UIFunction::Set_ControlText( "PlayerInfoTitle", pMTable->m_sMessage );
        pMentButton->SetVisible( N3UI_VISIBLE_TRUE );
    }

	// 상대방 정보를 볼때
    if( m_bIsMeInfo == false )
    {
		for(int i=1;i<4;i++)
		{
			_snprintf( sText, MAX_PATH, "Static_PlayerInfo_Tab%d", i );
			pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);
			pControl->SetEnabled(false);

			if( i==3 ) continue;
			_snprintf( sText, MAX_PATH, "PlayerInfo_SubTab0%d", i );
			pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pControl);
			pControl->SetEnabled(false);	
		}

        pMentButton->SetEnabled( false );
        pMentEditBox->SetEnabled( false );

		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900064 ,0));
		assert(pMTable);

        UIFunction::Set_ControlText( "PlayerInfoTitle", pMTable->m_sMessage );
        pMentButton->SetVisible( N3UI_VISIBLE_FALSE );
        pIcon_Levelup->SetVisible( N3UI_VISIBLE_FALSE );
    }    

    m_nReceivePage		= 0;
    m_nMonthRankingPage = 0;
    m_nTotalRankingPage = 0;
	m_nAthletePage		= 0;
    
    m_nCurrentTradeTotalPage = 0;
    m_nCurrentTradePage = 0;
    m_nCurrentTradePeriod = 0;

    FUNCTION_PlayerBasicInformation();														// 기본정보
    FUNCTION_PlayerAbilityInformation();													// 능력치
    FUNCTION_PlayerConsumptionInformation();												// 소비아이템
    FUNCTION_PlayerReceiveInformation();													// 수상내역
	FUNCTION_TradePageView( 0 );															// 거래내역
    FUNCTION_PlayerMonthRankingInformation();
    FUNCTION_PlayerTotalRankingInformation();
	FUNCTION_AtheletListInformation( CLUB_LIST, 0 );

    Set_FACULTY();
    
    this->InitCharacter();

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(0);
}


void CPlayerInformationDialog::FUNCTION_InitFraction()
{
	CResFractionTableStack *temp = dynamic_cast<CResFractionTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_FRACTIONTABLE));	
	assert(temp);
	
	for(int i=0;i<LIST5_SIZE;i++)
		m_vFractionList[i].clear();
		
	for(int i=0;i<temp->GetResourceSize();i++)
	{
		// 분류 테이블 로딩
		CResFractionTableFormat *pTable = dynamic_cast<CResFractionTableFormat*>(temp->GetSequenceField(i));
		assert(pTable);

		if( pTable->GetObjCode()/1000 == PLAYERINFO_FRACTION)
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
	}
}

void CPlayerInformationDialog::FUNCTION_SmallFractionInit(char nBigFraction)
{
	char sText[MAX_PATH]="";

	if( !(int)m_vFractionList[nBigFraction].size() )
	{
		for(int i=0;i<3;i++)
		{
			_snprintf( sText, MAX_PATH, "PlayerInfo_SubTab0%d", i );
			UIFunction::Set_ControlText( sText, "" );
		}
		return;
	}

	if( m_nSmallFractionStart < 0 )																// 예외처리
		m_nSmallFractionStart = 0;
	else if( (int)m_vFractionList[nBigFraction].size() <= 3 )							// 5개 미만일 경우
		m_nSmallFractionStart = 0;
	else if( (int)m_vFractionList[nBigFraction].size() <= m_nSmallFractionStart+3 )	// 총개수 초과시
		m_nSmallFractionStart = m_vFractionList[nBigFraction].size() - 3;

	CClientUIManager::GetPtr()->FindEventControl("Icon_LevelUp")->SetAnimationStart(N3UI_STATE_NORMAL, ANIMATION_LOOP);

    if( m_pSelectPlayerInfo->m_cLevel.m_nFaculty != 0 && m_nSmallFractionStart == 0 && m_nBigFractionIndex == 0 )
        UIFunction::Set_UIVisibility( "Icon_LevelUp", N3UI_VISIBLE_TRUE );
	else
        UIFunction::Set_UIVisibility( "Icon_LevelUp", N3UI_VISIBLE_FALSE );

	FUNCTION_SmallFraction(0);

	for(int i=0;i<3;i++)
	{
		_snprintf( sText, MAX_PATH, "PlayerInfo_SubTab0%d", i );
		
		if( (int)m_vFractionList[nBigFraction].size() <= m_nSmallFractionStart+i )
			UIFunction::Set_ControlText( sText, "" );
		else
			UIFunction::Set_ControlText( sText, m_vFractionList[nBigFraction].at(m_nSmallFractionStart+i).c_str() );
	}
}

void CPlayerInformationDialog::FUNCTION_BigFraction(char nBigFraction)
{
	char sText[STRLEN_64]="";

	m_nBigFractionIndex = nBigFraction;

	FUNCTION_SmallFractionInit(nBigFraction);

	for(int i=0;i<4;i++)
	{
		_snprintf( sText, STRLEN_64, "Static_PlayerInfo_Tab%d", i );
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);
		if( nBigFraction == i )
			pBtn->SetPressed(true);
		else
			pBtn->SetPressed(false);
	}

	FUNCTION_SmallFraction(0);
}

void CPlayerInformationDialog::FUNCTION_SmallFraction(char nSmallFraction)
{
	m_nSmallFractionIndex = nSmallFraction;

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		_snprintf( sText, STRLEN_64, "PlayerInfo_SubTab0%d", i );
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);
		if( nSmallFraction == i )
			pBtn->SetPressed(true);
		else
			pBtn->SetPressed(false);
	}

	if( (int)m_vFractionList[m_nBigFractionIndex].size() <= m_nSmallFractionStart+nSmallFraction )
		return;

	SN3UIUserFunctionParameter cParam;

	_snprintf(sText, STRLEN_64, "%d", (m_vFractionList[m_nBigFractionIndex].size()*100)+nSmallFraction+m_nSmallFractionStart);

	UICommonMethod::OnClickTabInvisible( "500", "Static_PlayerInfoCharacter" );
	UICommonMethod::OnClickTabInvisible( "200", "Static_PlayerInfoRanking" );
	UICommonMethod::OnClickTabInvisible( "300", "Static_AthleteInfo" );
	UICommonMethod::OnClickTabInvisible( "200", "Static_ClubInfo" );

	switch(m_nBigFractionIndex)
	{
	case 0:		// 기본 정보
		{
			UICommonMethod::OnClickTab( sText, "Static_PlayerInfoCharacter" );
		}
		break;
	case 1:		// 랭킹 정보
		{
			UICommonMethod::OnClickTab( sText, "Static_PlayerInfoRanking" );
		}
		break;
	case 2:		// 선수 정보
		{
			UICommonMethod::OnClickTab( sText, "Static_AthleteInfo" );

			if(nSmallFraction == 1)
				PutBuddyInfo(0);
			else
				CPlayerInformationDialog::GetPtr()->FUNCTION_AtheletListInformation(nSmallFraction, 0);
		}
		break;
	case 3:		// 클럽 정보
		{
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20400, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			//UICommonMethod::OnClickTab( sText, "Static_ClubInfo" );
		}
		break;
	}
}

void CPlayerInformationDialog::InitCharacter()
{
    SAFE_DELETE( m_pCharacter );

    NiStream kStream;

    kStream.Load( "Ground\\UI_Result.nif" );

    m_spRootNode = (NiNode*) kStream.GetObjectAt(0);

    CObjPlayerInfo * pPlayerInfo = m_pSelectPlayerInfo;

    m_pCharacter = (CResCharacterFormat*) dynamic_cast<CResCharacterFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CHARATERMODEL, pPlayerInfo->m_cShape.m_nGender, pPlayerInfo->GetKind() ));
	assert(m_pCharacter);

    CObjPlayerInfo * pNewPlayer = pPlayerInfo->Clone();
    pNewPlayer->Init(m_pCharacter);

    CProcessManager::GetPtr()->AttachItem( pNewPlayer );

    NiNode * pCharacterParentNode = (NiNode*) m_pCharacter->m_spNode->GetParent();    

    CObjectModelManager::GetPtr()->DeleteModelObject( m_pCharacter );

    SAFE_DELETE( pNewPlayer );

    if( pCharacterParentNode != 0 )
    {    
        pCharacterParentNode->AttachChild( m_pCharacter->m_spNode );
    }

    NiNode * pCharacterPosNode = (NiNode*) m_spRootNode->GetObjectByName( "7001" );
    assert( pCharacterPosNode != 0 );

    m_pCharacter->SetNodePoint( NiPoint3::ZERO );
    pCharacterPosNode->AttachChild(m_pCharacter->GetRootNode() );

    m_spRootNode->Update( 0.0f );
    m_spRootNode->UpdateNodeBound();
    m_spRootNode->UpdateProperties();
    m_spRootNode->UpdateEffects();    

    m_pCharacter->SetTargetAnimation( 70011201 );

    m_spCamera = CoreApi_FindCamera( m_spRootNode );

    assert( m_spCamera != 0 );

    m_cCharacterTexBoard.Init( m_spCamera, m_spRootNode, CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
    m_cCharacterTexBoard.FUNCTION_3DRender( "Characterfaciel_Static" );
}

void CPlayerInformationDialog::Update( float fFrameTime, float fAccumTime )
{
    if( m_spRootNode == 0 ) return;

    m_spRootNode->Update( fAccumTime );

    CGameProject::s_pGameProject->BeginFrame();    

	m_cCharacterTexBoard.Render();

	CGameProject::s_pGameProject->EndFrame();

    CGameProject::s_pGameProject->DisplayFrame();
}

void CPlayerInformationDialog::Set_FACULTY()
{
    ZeroMemory(&m_cFaculty.m_nFaculty,sizeof(CFaculty));
    nUsedFaculty=0;

    Set_Remain_Faculty(m_pSelectPlayerInfo->m_cLevel.m_nFaculty);

}

void CPlayerInformationDialog::Set_Remain_Faculty(DWORD nFaculty)
{
    char sFaculty[STRLEN_4];

    _snprintf(sFaculty, STRLEN_4, "%d",nFaculty);
	UIFunction::Set_ControlText( "AbilityRemainPoint_Static" , sFaculty );
}

void CPlayerInformationDialog::FUNCTION_PlayerInformationUpdate()
{
}

void CPlayerInformationDialog::FUNCTION_PlayerInformationTerminate()
{

}

void CPlayerInformationDialog::FUNCTION_PlayerBasicInformation()
{
    char text[STRLEN_64]="";

    CUIControl* pPlayerInformation_Static = CClientUIManager::GetPtr()->FindEventControl("Static_PlayerInfoCharacter_DetailTab0");

    assert( pPlayerInformation_Static != 0 );
    if(pPlayerInformation_Static == NULL) return;

    std::string cPosition   = UICommonMethod::PositionNtoPositionStr( m_pSelectPlayerInfo->m_nPosition );
    std::string cCondition  = GetConditionString( (PLAYER_CONDITION) m_pSelectPlayerInfo->m_nCondition );
    std::string cAlias      = GetAliasString( (PLAYER_ALIAS) m_pSelectPlayerInfo->m_nAlias );

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900058 ,0));
	assert(pMTable);

	_snprintf(text, STRLEN_64, pMTable->m_sMessage, m_pSelectPlayerInfo->m_cLevel.m_nLevel);
	UIFunction::Set_ControlText( "myinfo_levelnum" , text );
	UIFunction::Set_ControlText( "myinfo_position", cPosition.c_str() );
	UIFunction::Set_ControlText( "myinfo_state", cCondition.c_str() );
	UIFunction::Set_ControlText( "myinfo_callname", cAlias.c_str() );
	UIFunction::Set_ControlText( "myinfo_name", m_pSelectPlayerInfo->m_sName );

	CUIIMEEditBox * pMent = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl( "ment" ));;
	pMent->ClearText();
    pMent->SetText( m_pSelectPlayerInfo->m_sMent );

    //pPlayerInformation_Static->SetVisible(N3UI_VISIBLE_FALSE);
}

void CPlayerInformationDialog::FUNCTION_PlayerAbilityInformation()
{
    if( m_bIsMeInfo == false ) return;

    char sText[STRLEN_64]="";

    for(int i = 0; i < LIST5_SIZE + LIST10_SIZE;i++)
    {
        _snprintf(sText, STRLEN_64, "Ability%d_Static", i);
        CUIControl* pAbility_Static = CClientUIManager::GetPtr()->FindEventControl(sText);
        assert(pAbility_Static);

        if(m_pSelectPlayerInfo->m_nPosition != POSITION_GK && (i == ARRAY_FACULTY_CATCH || i == ARRAY_FACULTY_PUNCH || i == ARRAY_FACULTY_GUARD))
            pAbility_Static->SetVisible(N3UI_VISIBLE_FALSE);

        CUIControl* pAutomatic_Img = pAbility_Static->FindEventControl("Automatic_Img");
        assert(pAutomatic_Img);

		CResPositionTableFormat *pPositioinTable = dynamic_cast<CResPositionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_POSITIONTABLE, m_pSelectPlayerInfo->m_nPosition , 0));
		assert(pPositioinTable);
		if( i == pPositioinTable->m_nAuto0 || i == pPositioinTable->m_nAuto1 || i == pPositioinTable->m_nAuto2 )
			pAutomatic_Img->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pAutomatic_Img->SetVisible(N3UI_VISIBLE_FALSE);

		_snprintf(sText, STRLEN_64, "%d", m_pSelectPlayerInfo->GetFaculty(i));
		UIFunction::Set_ChildControlText( pAbility_Static, "AbilityPoint_Static", sText );

        CUIControl* pGaugeBar_Static = pAbility_Static->FindEventControl("GaugeBar_Static");
        assert(pGaugeBar_Static);

        CUIProgress* pAbilityPoint0_GaugeBar = (CUIProgress*)pGaugeBar_Static->FindEventControl("AbilityPoint0_GaugeBar");
        assert(pAbilityPoint0_GaugeBar);
        pAbilityPoint0_GaugeBar->SetRange(0, 150);
        pAbilityPoint0_GaugeBar->SetValue(m_pSelectPlayerInfo->GetBaseFaculty(i) + m_pSelectPlayerInfo->GetItemFaculty(i) + m_pSelectPlayerInfo->GetTrainingFaculty(i));

        CUIProgress* pAbilityPoint1_GaugeBar = (CUIProgress*)pGaugeBar_Static->FindEventControl("AbilityPoint1_GaugeBar");
        assert(pAbilityPoint1_GaugeBar);
        pAbilityPoint1_GaugeBar->SetRange(0, 150);
        pAbilityPoint1_GaugeBar->SetValue(m_pSelectPlayerInfo->GetBaseFaculty(i) + m_pSelectPlayerInfo->GetItemFaculty(i));

        CUIProgress* pAbilityPoint2_GaugeBar = (CUIProgress*)pGaugeBar_Static->FindEventControl("AbilityPoint2_GaugeBar");
        assert(pAbilityPoint2_GaugeBar);
        pAbilityPoint2_GaugeBar->SetRange(0, 150);
        pAbilityPoint2_GaugeBar->SetValue(m_pSelectPlayerInfo->GetBaseFaculty(i));

        _snprintf(sText, STRLEN_64, "%d", m_pSelectPlayerInfo->GetBaseFaculty(i));
		UIFunction::Set_ChildControlText( pAbility_Static, "AbilityDefaultPoint_Static", sText );
       
        _snprintf(sText, STRLEN_64, "%d", m_pSelectPlayerInfo->GetItemFaculty(i));
		UIFunction::Set_ChildControlText( pAbility_Static, "AbilityUngradePoint_Static", sText );

        _snprintf(sText, STRLEN_64, "%d", m_pSelectPlayerInfo->GetTrainingFaculty(i));
		UIFunction::Set_ChildControlText( pAbility_Static, "AbilityNewPoint_Static", sText );

        _snprintf(sText, STRLEN_64, "%d", 0);
		UIFunction::Set_ChildControlText( pAbility_Static, "AbilityPulsPoint_Static", sText );
    }	

    _snprintf(sText, STRLEN_64, "%d", m_pSelectPlayerInfo->m_cLevel.m_nFaculty);
	UIFunction::Set_ControlText( "AbilityRemainPoint_Static", sText );

    CClientUIManager::GetPtr()->FindEventControl("Icon_LevelUp")->SetAnimationStart(N3UI_STATE_NORMAL, ANIMATION_LOOP);

    if( m_pSelectPlayerInfo->m_cLevel.m_nFaculty != 0 && m_nSmallFractionStart == 0 && m_nBigFractionIndex == 0 )
        UIFunction::Set_UIVisibility( "Icon_LevelUp", N3UI_VISIBLE_TRUE );
	else
        UIFunction::Set_UIVisibility( "Icon_LevelUp", N3UI_VISIBLE_FALSE );
}

void CPlayerInformationDialog::FUNCTION_PlayerConsumptionInformation()
{
    if( m_bIsMeInfo == false ) return;

    char text[STRLEN_64];

	m_vConsumptionItem.clear();

    CUIControl* pConsumption_BG = CClientUIManager::GetPtr()->FindEventControl("Static_PlayerInfoCharacter_DetailTab2");
    assert(pConsumption_BG);

	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pSelectPlayerInfo);

    CBagItemList *pListDrink, *pListBar;

	pListDrink = new CBagItemList();
	pListBar = new CBagItemList();

	pSelectPlayerInfo->GetBagItemList( 501, LIST10_SIZE, 0, pListDrink );

	pSelectPlayerInfo->GetBagItemList( 502, LIST10_SIZE, 0, pListBar );

	for(int i=0;i<LIST10_SIZE;i++)
	{
		if(pListDrink->m_pItem[i] != NULL)
			m_vConsumptionItem.push_back(pListDrink->m_pItem[i]);

		if(pListBar->m_pItem[i] != NULL)
			m_vConsumptionItem.push_back(pListBar->m_pItem[i]);

		sprintf(text, "ConsumptionAbility%d_Static", i);
        CUIControl* pConsumptionAbility_Static = pConsumption_BG->FindEventControl(text);
        assert(pConsumptionAbility_Static);

		pConsumptionAbility_Static->SetVisible(N3UI_VISIBLE_FALSE);
	}

	int nCount = 0;
	int nConsumTotalPage = 0;
	int nStartPage = 0;

	if( (int)m_vConsumptionItem.size() <= LIST10_SIZE )
	{
		m_nConsumPage=0;
		nCount = (int)m_vConsumptionItem.size();
	}
	else
	{
		nCount = (int)m_vConsumptionItem.size();
		nConsumTotalPage = nCount/LIST10_SIZE;

		if(nCount%LIST10_SIZE)
			nConsumTotalPage++;

		if( nConsumTotalPage <= m_nConsumPage )
			nStartPage = m_nConsumPage = 0;
		else if(0 < m_nConsumPage && m_nConsumPage < nConsumTotalPage )
			nStartPage = m_nConsumPage;
		else if( m_nConsumPage < 0 )
			nStartPage = m_nConsumPage = nConsumTotalPage-1;

		if( nStartPage == 0 )
			nCount = LIST10_SIZE;
		else
            nCount -= LIST10_SIZE*nStartPage;
	}	
		
    for(int i = 0; i < nCount;i++)
    {
        sprintf(text, "ConsumptionAbility%d_Static", i);
        CUIControl* pConsumptionAbility_Static = pConsumption_BG->FindEventControl(text);
        assert(pConsumptionAbility_Static);

		pConsumptionAbility_Static->SetVisible(N3UI_VISIBLE_TRUE);

		CUICheckBox* pConsumptionUse = dynamic_cast<CUICheckBox*>(pConsumptionAbility_Static->FindEventControl("ConsumptionUse_ChkBox"));
        assert(pConsumptionUse);
		pConsumptionUse->SetCheckValue(m_vConsumptionItem.at(i+(nStartPage*LIST10_SIZE))->m_nEquipKind == EQUIP_KIND_YES);

        CUIControl* pConsumptionName_Static = pConsumptionAbility_Static->FindEventControl("ConsumptionName_Static");
        assert(pConsumptionName_Static);
		pConsumptionName_Static->SetText(m_vConsumptionItem.at(i+(nStartPage*LIST10_SIZE))->m_pTable->m_sName);

        CUIControl* pConsumptionEffect_Static = pConsumptionAbility_Static->FindEventControl("ConsumptionEffect_Static");
        assert(pConsumptionEffect_Static);

		char sOption[STRLEN_32]="";

		for(int k=0;k<3;k++)
		{
			if(m_vConsumptionItem.at(i+(nStartPage*LIST10_SIZE))->m_nOptionCode[k] != 0)
			{
				if( k != 0 )
					strcat(sOption, ", ");

				CResOptionTableFormat* pOptionTable = dynamic_cast<CResOptionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OPTIONTABLE, m_vConsumptionItem.at(i+(nStartPage*LIST10_SIZE))->m_nOptionCode[k], 0));
				if(pOptionTable == NULL) continue;

				strcat(sOption, pOptionTable->m_sTitle);
			}
		}

        pConsumptionEffect_Static->SetText(sOption);	

        CUIControl* pConsumptionNumber_Static = pConsumptionAbility_Static->FindEventControl("ConsumptionNumber_Static");
        assert(pConsumptionNumber_Static);
		sprintf(text, "%d", m_vConsumptionItem.at(i+(nStartPage*LIST10_SIZE))->m_nAmount);
        pConsumptionNumber_Static->SetText(text);
    }

	CUIControl* pPage = CClientUIManager::GetPtr()->FindEventControl( "ConsumPageName" );
	assert(pPage);

	_snprintf(text, STRLEN_64, "%d", m_nConsumPage+1);
	pPage->SetText(text);

	delete pListDrink;
	delete pListBar;
}

void CPlayerInformationDialog::FUNCTION_PlayerReceiveInformation()
{
	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20400 ,0));
	assert(pMTable);

	UIFunction::Set_ControlText( "ReceiveList0", pMTable->m_sMessage );
	return;		// 예외처리

    char text[STRLEN_64];

    CUIControl* pPlayerInformation_Static = CClientUIManager::GetPtr()->FindEventControl("Static_PlayerInfoCharacter_DetailTab3");
    if(pPlayerInformation_Static == NULL) return;

    CUIControl* pReceive_BG = CClientUIManager::GetPtr()->FindEventControl("Receive_BG");
    if(pReceive_BG == NULL) return;

    for(int i = 0; i < 1;i++)
    {
        sprintf(text, "List%d_Static", i);
        CUIControl* pList_Static = pReceive_BG->FindEventControl(text);

        if(pList_Static == NULL) return;

        CUIControl* pTradeTime_Static = pList_Static->FindEventControl("Time_Static");
        if(pTradeTime_Static == NULL) return;
        pTradeTime_Static->SetText("오늘 어제 어져께");	

        CUIControl* pTradeContent_Static = pList_Static->FindEventControl("Content_Static");
        if(pTradeContent_Static == NULL) return;
        pTradeContent_Static->SetText("킥스2 오픈배타 기념 토너먼트에서 우승했습니다.짝짝짝");
    }

    pPlayerInformation_Static->SetVisible(N3UI_VISIBLE_FALSE);
}

void CPlayerInformationDialog::AddSaleList( int nPeriod, CSaleList & cSaleInfo )
{
    if( cSaleInfo.m_nObjectKind == 0  ) return;
    
    int nSize = m_cSaleInfoArray.size();

    m_cSaleInfoArray.push_back( cSaleInfo );
    this->ShowTrade( nPeriod, nSize, cSaleInfo );
}

void CPlayerInformationDialog::SetTradePage( int nPeriod, int nTotalTradePage, int nCurrentPage )
{
    m_nCurrentTradePeriod       = nPeriod;
    m_nCurrentTradeTotalPage    = nTotalTradePage;
    m_nCurrentTradePage         = nCurrentPage;

    char sText[STRLEN_64]="";

	for( int i=0; i<LIST10_SIZE; i++ )
    {
        _snprintf( sText, STRLEN_64, "List%d_TradeStatic", i);
		CUIControl * pList = CClientUIManager::GetPtr()->FindEventControl( sText );        
        assert( pList );
        
        pList->SetVisible( N3UI_VISIBLE_FALSE );
    }   

    _snprintf( sText, STRLEN_64, "%d/%d", m_nCurrentTradePage + 1, m_nCurrentTradeTotalPage );
	UIFunction::Set_ControlText( "TradePageName", sText );
}

void CPlayerInformationDialog::FUNCTION_TradePageView( int nPage )
{
    m_cSaleInfoArray.clear();

    m_nCurrentTradePage += nPage;
    
    if( m_nCurrentTradePage >= m_nCurrentTradeTotalPage) m_nCurrentTradePage = 0;
    if( m_nCurrentTradePage < 0 ) m_nCurrentTradePage = m_nCurrentTradeTotalPage - 1;

    PutSaleList( 1, m_nCurrentTradePage );	//nKind = 1 현재 분기
}

void CPlayerInformationDialog::ShowTrade( int nPeriod, int nIndex, CSaleList & cSaleInfo )
{
    char sText[STRLEN_64]="";

    _snprintf( sText, STRLEN_64, "List%d_TradeStatic", nIndex );
	CUIControl *pListStatic = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pListStatic);

    pListStatic->SetVisible( N3UI_VISIBLE_TRUE );

    std::string cIconName;
    switch( cSaleInfo.m_nObjectKind )
    {    
    case OBJECT_KIND_ITEM:
        cIconName = "IconItem";
	    break;
    case OBJECT_KIND_SKILL:
        cIconName = "IconSkill";
		break;
    case OBJECT_KIND_CEREMONY:
        cIconName = "IconCeremony";
		break;
    case OBJECT_KIND_TRAINING:
        cIconName = "IconTraining";
	    break;
    }

    CUIControl *pTypeIcon = pListStatic->FindEventControl( "Type_Icon" );
	assert(pTypeIcon);
	if(cIconName.length())
		UIFunction::ChangeTexture( pTypeIcon, (char*) cIconName.c_str() );

    _snprintf( sText, STRLEN_64, "%d", cSaleInfo.m_nPrice );
	UIFunction::Set_ChildControlText( pListStatic, "Price", sText );

    std::string cSaleDate = TimestampToString( cSaleInfo.m_nSaleDate );
	std::string cDate = cSaleDate.substr(0, cSaleDate.find(" "));
	UIFunction::Set_ChildControlText( pListStatic, "Time_Static", cDate.c_str() );

	if( cSaleInfo.m_nCode != 0 )
	{
		std::string cObjectName = this->GetObjectName( cSaleInfo.m_nObjectKind, cSaleInfo.m_nCode);
		UIFunction::Set_ChildControlText( pListStatic, "Content_Static", cObjectName.c_str() );
	}
}

void CPlayerInformationDialog::FUNCTION_PlayerMonthRankingInformation()
{
	return;			// 예외처리

    char text[STRLEN_64];

    CUIControl* pPlayerInformation_Static = CClientUIManager::GetPtr()->FindEventControl("Static_PlayerInfoRanking_DetailTab0");
    if(pPlayerInformation_Static == NULL) return;

    CUIControl* pMonthRanking_BG = CClientUIManager::GetPtr()->FindEventControl("MonthRanking_BG");
    if(pMonthRanking_BG == NULL) return;

    for(int i = 0; i < LIST10_SIZE + 3;i++)
    {
        sprintf(text, "MonthRanking%d_Static", i);
        CUIControl* pMonthRanking_Static = pMonthRanking_BG->FindEventControl(text);
        if(pMonthRanking_Static == NULL) return;

        CUIControl* pMonthRankingName_Static = pMonthRanking_Static->FindEventControl("MonthRankingName_Static");
        if(pMonthRankingName_Static == NULL) return;
        pMonthRankingName_Static->SetText("출전");	

        CUIControl* pMonthRankingNumber_Static = pMonthRanking_Static->FindEventControl("MonthRankingNumber_Static");
        if(pMonthRankingNumber_Static == NULL) return;
        sprintf(text, "%d", i);
        pMonthRankingNumber_Static->SetText(text);

        CUIControl* pMonthRankingRanking_Static = pMonthRanking_Static->FindEventControl("MonthRankingRanking_Static");
        if(pMonthRankingRanking_Static == NULL) return;
        sprintf(text, "%d", i);
        pMonthRankingRanking_Static->SetText(text);
    }

    pPlayerInformation_Static->SetVisible(N3UI_VISIBLE_FALSE);
}

void CPlayerInformationDialog::FUNCTION_PlayerTotalRankingInformation()
{
	return;				// 예외처리
    char text[STRLEN_64];

    CUIControl* pPlayerInformation_Static = CClientUIManager::GetPtr()->FindEventControl("Static_PlayerInfoRanking_DetailTab1");
    if(pPlayerInformation_Static == NULL) return;

    CUIControl* pTotalRanking_BG = CClientUIManager::GetPtr()->FindEventControl("TotalRanking_BG");
    if(pTotalRanking_BG == NULL) return;

    for(int i = 0; i < LIST10_SIZE + 3;i++)
    {
        sprintf(text, "TotalRanking%d_Static", i);
        CUIControl* pTotalRanking_Static = pTotalRanking_BG->FindEventControl(text);
        if(pTotalRanking_Static == NULL) return;

        CUIControl* pTotalRankingName_Static = pTotalRanking_Static->FindEventControl("TotalRankingName_Static");
        if(pTotalRankingName_Static == NULL) return;
        pTotalRankingName_Static->SetText("출전");	

        CUIControl* pTotalRankingNumber_Static = pTotalRanking_Static->FindEventControl("TotalRankingNumber_Static");
        if(pTotalRankingNumber_Static == NULL) return;
        sprintf(text, "%d", i);
        pTotalRankingNumber_Static->SetText(text);

        CUIControl* pTotalRankingRanking_Static = pTotalRanking_Static->FindEventControl("TotalRankingRanking_Static");
        if(pTotalRankingRanking_Static == NULL) return;
        sprintf(text, "%d", i);
        pTotalRankingRanking_Static->SetText(text);
    }

    pPlayerInformation_Static->SetVisible(N3UI_VISIBLE_FALSE);
}

void CPlayerInformationDialog::OnClick_AbillityUp(DWORD nIndex)
{
    char text[STRLEN_64];

    if((static_cast<int>( nUsedFaculty ) < static_cast<int>( m_pSelectPlayerInfo->m_cLevel.m_nFaculty )) && (m_pSelectPlayerInfo->GetBaseFaculty(nIndex) + static_cast<int>( m_cFaculty.m_nFaculty[nIndex] ) < 100))
    {
        ++m_cFaculty.m_nFaculty[nIndex];
        ++nUsedFaculty;

        sprintf(text, "Ability%d_Static", nIndex);
        CUIControl* pAbility_Static = CClientUIManager::GetPtr()->FindEventControl(text);
        assert(pAbility_Static);
        Set_Remain_Faculty(m_pSelectPlayerInfo->m_cLevel.m_nFaculty - nUsedFaculty);
        CUIControl* pAbilityPlusPoint_Static = pAbility_Static->FindEventControl("AbilityPulsPoint_Static");
        assert(pAbilityPlusPoint_Static);
        sprintf(text, "%d", m_cFaculty.m_nFaculty[nIndex]);
        pAbilityPlusPoint_Static->SetText(text);
		CUIControl* pAbilityAmountPoint = pAbility_Static->FindEventControl("AbilityDefaultPoint_Static");
		assert(pAbilityAmountPoint);
		int n = _wtoi(pAbilityAmountPoint->GetText());
		sprintf(text, "%d", ++n);
		pAbilityAmountPoint->SetText(text);

		CUIControl* pAbilityUngradePoint_Static = pAbility_Static->FindEventControl("AbilityUngradePoint_Static");
		assert(pAbilityUngradePoint_Static);

        CUIControl* pAbilityNewPoint_Static = pAbility_Static->FindEventControl("AbilityNewPoint_Static");
		assert(pAbilityNewPoint_Static);

		CUIControl* pAbilityPoint_Static = pAbility_Static->FindEventControl("AbilityPoint_Static");
		assert(pAbilityPoint_Static);

		sprintf(text, "%d", _wtoi(pAbilityAmountPoint->GetText()) + _wtoi(pAbilityUngradePoint_Static->GetText()) + _wtoi(pAbilityNewPoint_Static->GetText()) );
		pAbilityPoint_Static->SetText(text);

		CUIControl* pGaugeBar_Static = pAbility_Static->FindEventControl("GaugeBar_Static");
        assert(pGaugeBar_Static);

		CUIProgress* pAbilityPoint0_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint0_GaugeBar"));
		assert(pAbilityPoint0_GaugeBar);
		CUIProgress* pAbilityPoint1_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint1_GaugeBar"));
		assert(pAbilityPoint1_GaugeBar);
		CUIProgress* pAbilityPoint2_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint2_GaugeBar"));
		assert(pAbilityPoint2_GaugeBar);		
		if(pAbilityPoint2_GaugeBar->GetValue()!=pAbilityPoint0_GaugeBar->GetValue())
			pAbilityPoint0_GaugeBar->SetValue(pAbilityPoint0_GaugeBar->GetValue() + 1);
		if(pAbilityPoint2_GaugeBar->GetValue()!=pAbilityPoint1_GaugeBar->GetValue())
			pAbilityPoint1_GaugeBar->SetValue(pAbilityPoint1_GaugeBar->GetValue() + 1);

		pAbilityPoint2_GaugeBar->SetValue(pAbilityPoint2_GaugeBar->GetValue() + 1);		
    }
}

void CPlayerInformationDialog::OnClick_AbillityDown(DWORD nIndex)
{
    char text[STRLEN_64];

    if(m_cFaculty.m_nFaculty[nIndex] > 0)
    {
        --m_cFaculty.m_nFaculty[nIndex];
        --nUsedFaculty;
        Set_Remain_Faculty(m_pSelectPlayerInfo->m_cLevel.m_nFaculty - nUsedFaculty);
        sprintf(text, "Ability%d_Static",nIndex);
        CUIControl* pAbility_Static = CClientUIManager::GetPtr()->FindEventControl(text);
        assert(pAbility_Static);
        CUIControl* pAbilityPlusPoint_Static = pAbility_Static->FindEventControl("AbilityPulsPoint_Static");
        assert(pAbilityPlusPoint_Static);
        sprintf(text, "%d",m_cFaculty.m_nFaculty[nIndex]);
        pAbilityPlusPoint_Static->SetText(text);
		CUIControl* pAbilityAmountPoint = pAbility_Static->FindEventControl("AbilityDefaultPoint_Static");
		assert(pAbilityAmountPoint);
		int n = _wtoi(pAbilityAmountPoint->GetText());
		sprintf(text, "%d", --n);
		pAbilityAmountPoint->SetText(text);

		CUIControl* pAbilityUngradePoint_Static = pAbility_Static->FindEventControl("AbilityUngradePoint_Static");
		assert(pAbilityUngradePoint_Static);

        CUIControl* pAbilityNewPoint_Static = pAbility_Static->FindEventControl("AbilityNewPoint_Static");
		assert(pAbilityNewPoint_Static);

		CUIControl* pAbilityPoint_Static = pAbility_Static->FindEventControl("AbilityPoint_Static");
		assert(pAbilityPoint_Static);

		sprintf(text, "%d", _wtoi(pAbilityAmountPoint->GetText()) + _wtoi(pAbilityUngradePoint_Static->GetText()) + _wtoi(pAbilityNewPoint_Static->GetText()) );
		pAbilityPoint_Static->SetText(text);

		CUIControl* pGaugeBar_Static = pAbility_Static->FindEventControl("GaugeBar_Static");
        assert(pGaugeBar_Static);

		CUIProgress* pAbilityPoint0_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint0_GaugeBar"));
		assert(pAbilityPoint0_GaugeBar);
		CUIProgress* pAbilityPoint1_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint1_GaugeBar"));
		assert(pAbilityPoint1_GaugeBar);
		CUIProgress* pAbilityPoint2_GaugeBar = dynamic_cast<CUIProgress*>(pGaugeBar_Static->FindEventControl("AbilityPoint2_GaugeBar"));
		assert(pAbilityPoint2_GaugeBar);
		if(pAbilityPoint2_GaugeBar->GetValue()!=pAbilityPoint0_GaugeBar->GetValue())
			pAbilityPoint0_GaugeBar->SetValue(pAbilityPoint0_GaugeBar->GetValue() - 1);
		if(pAbilityPoint2_GaugeBar->GetValue()!=pAbilityPoint1_GaugeBar->GetValue())
			pAbilityPoint1_GaugeBar->SetValue(pAbilityPoint1_GaugeBar->GetValue() - 1);

		pAbilityPoint2_GaugeBar->SetValue(pAbilityPoint2_GaugeBar->GetValue() - 1);
    }
}

void CPlayerInformationDialog::InfoDConfirm_Ability()
{
    if(nUsedFaculty==0)
        return;

    PutRaiseFaculty(&m_cFaculty);
}

std::string CPlayerInformationDialog::GetObjectName( int nObjectKind, int nCode )
{
    std::string cResultText;

    switch( (OBJECT_KIND) nObjectKind )
    {
    case OBJECT_KIND_EXCHANGE:
    case OBJECT_KIND_ITEM:
        {        
            CResItemTableFormat *pItemTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, nCode, 0));
            if( pItemTable != 0 ) cResultText = pItemTable->m_sName;
        }
        break;

    case OBJECT_KIND_SKILL:
        {        
            CResSkillTableFormat * pSkillTable = (CResSkillTableFormat*) CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, nCode, 0 );
            if( pSkillTable != 0 ) cResultText = pSkillTable->m_sName;
        }
        break;

    case OBJECT_KIND_CEREMONY:
        {
            CResCeremonyTableFormat * pCeremonyTable = (CResCeremonyTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_CEREMONYTABLE, nCode, 0 );
            if( pCeremonyTable != 0 ) cResultText = pCeremonyTable->m_sName;
        }
        break;

    case OBJECT_KIND_TRAINING:
        {
            CResTrainingTableFormat * pTrainingTable = (CResTrainingTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_TRAININGTABLE, nCode, 0 );
            if( pTrainingTable != 0 ) cResultText = pTrainingTable->m_sName;
        }
        break;

    default:
        cResultText = "Error";
        break;
    }

    return cResultText;
}

void CPlayerInformationDialog::FUNCTION_ChangeMent()
{
    CUIControl * pMent = CClientUIManager::GetPtr()->FindEventControl( "ment" );
    assert( pMent != 0 );

    std::string cInputMent;
    cInputMent = pMent->GetTextEx();    

    if( cInputMent == m_pSelectPlayerInfo->m_sMent ) return;

    PutChangeMent( cInputMent.c_str() );
}

void CPlayerInformationDialog::FUNCTION_Close()
{
	m_bIsMeInfo = true;

    SN3UIUserFunctionParameter cParam;
    cParam.AddParam( "Window", "PlayerInformation" );

    CClientUIManager::GetPtr()->ProcUserFunction( "HideWindow",  &cParam );
}

void CPlayerInformationDialog::DrawGraph()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("graph");
	assert(pControl);

	D3DCOLOR color = D3DCOLOR_ARGB( 255, 255, 128, 0 );
	POINT pt = { 55.0f, -27.0f };
	
	CClientUIManager::GetPtr()->DrawPolyLine( pControl, m_pPoint, 7, color, pt );
}

void CPlayerInformationDialog::FUNCTION_InitLinePoint()
{
	srand(time(NULL));

	int nScale[LIST8_SIZE];

	for(int i=0;i<LIST8_SIZE;i++)
		nScale[i] = rand()%11*(-18.f);

	m_pPoint[0].x = 0.0f;
	m_pPoint[0].y = nScale[0];

	for(int i=1;i<LIST8_SIZE;i++)
	{
		m_pPoint[i].x = m_pPoint[i-1].x + 60.0f;
		m_pPoint[i].y = nScale[i];
	}
}

void CPlayerInformationDialog::FUNCTION_AtheletListInformation(char nKind, char nPage)
{
	char sText[MAX_PATH]="";

	CObjPlayerInfo*	pMyPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CUIControl* pList	= NULL;

	bool bVisible = false;

	CBuddyInfo cBuddyInfo;

	for(int i=0;i<LIST10_SIZE;i++)
	{
		switch( nKind )
		{
		case CLUB_LIST:
			{
				_snprintf( sText, MAX_PATH, "List%d_ClubStatic", i );
				pList = CClientUIManager::GetPtr()->FindEventControl( sText );

				UIFunction::Set_UIVisibility( pList, "ClubName", N3UI_VISIBLE_TRUE );

				bVisible = false;

				/*if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) && m_cBuddyInfo.m_nState == BUDDY_STATE_LOBBY )
					UIFunction::Set_UIVisibility( pList, "Btn_invite", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( pList, "Btn_invite", N3UI_VISIBLE_FALSE );

				if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_CHOICEROOM) && m_cBuddyInfo.m_nState == BUDDY_STATE_ROOM )
					UIFunction::Set_UIVisibility( pList, "Btn_follow", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( pList, "Btn_follow", N3UI_VISIBLE_FALSE );	*/
			}
			break;
		case BUDDY_LIST:
			{
				_snprintf( sText, MAX_PATH, "List%d_BuddyStatic", i );
				pList = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pList);

				UIFunction::Set_UIVisibility( pList, "ClubName", N3UI_VISIBLE_FALSE );

				if( pMyPlayer->m_vBuddyList.size() <= i+(LIST10_SIZE*m_nAthletePage) )	
					bVisible = false;
				else
				{
					cBuddyInfo = (*pMyPlayer->m_vBuddyList.at(i+(LIST10_SIZE*m_nAthletePage)));
					
/*					if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) && cBuddyInfo.m_nState == BUDDY_STATE_LOBBY )
						UIFunction::Set_UIVisibility( pList, "Btn_invite", N3UI_VISIBLE_TRUE );
					else
						UIFunction::Set_UIVisibility( pList, "Btn_invite", N3UI_VISIBLE_FALSE );

					if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_CHOICEROOM) && cBuddyInfo.m_nState == BUDDY_STATE_ROOM )
						UIFunction::Set_UIVisibility( pList, "Btn_follow", N3UI_VISIBLE_TRUE );
					else
						UIFunction::Set_UIVisibility( pList, "Btn_follow", N3UI_VISIBLE_FALSE );			*/	

					bVisible = true;
				}
			}
			break;
		case BLOCK_LIST:
			{
				_snprintf( sText, MAX_PATH, "List%d_BlockStatic", i );
				pList = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pList);

				UIFunction::Set_UIVisibility( pList, "ClubName", N3UI_VISIBLE_FALSE );

				bVisible = false;
			}
			break;
		}

		if( bVisible )
		{
			// 캐릭터 명 표시
			UIFunction::Set_ChildControlText( pList, "Name", cBuddyInfo.m_sName );
			
			// 레벨 표시
			_snprintf( sText, MAX_PATH, "%d", cBuddyInfo.m_nLevel );
			UIFunction::Set_ChildControlText( pList, "Level", sText );
			
			// 포지션 마크 표시
			_snprintf( sText, MAX_PATH, "Position_H_%d", cBuddyInfo.m_nPosition );
			CUIControl* pCopy = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pCopy);
			CUIControl* pPosition = pList->FindEventControl( "Position_Icon" );
			assert(pPosition);
			pPosition->SetElement(pCopy->m_pElement->Clone());

			// 온/오프라인 표시
			if( cBuddyInfo.m_nState )
			{
				UIFunction::Set_UIVisibility( pList, "Off_Icon", N3UI_VISIBLE_FALSE );
				UIFunction::Set_UIVisibility( pList, "On_Icon", N3UI_VISIBLE_TRUE );
			}
			else
			{
				UIFunction::Set_UIVisibility( pList, "Off_Icon", N3UI_VISIBLE_TRUE );
				UIFunction::Set_UIVisibility( pList, "On_Icon", N3UI_VISIBLE_FALSE );
			}

			pList->SetVisible(N3UI_VISIBLE_TRUE);
		}
		else
			pList->SetVisible(N3UI_VISIBLE_FALSE);

	}

	//ClubAthleteListPageName	CLUBATHELETE_PAGE_CHANGE
	//BuddyAthleteListPageName	BUDDYATHELETE_PAGE_CHANGE
	//BlockAthleteListPageName	BLOCKATHELETE_PAGE_CHANGE

	_snprintf( sText, MAX_PATH, "%d", m_nAthletePage + 1 );
	//UIFunction::Set_ControlText( "AthleteListPageName", sText );	// 페이지
}

void CPlayerInformationDialog::FUNCTION_ClubMemberList(char nPage)
{
	char sText[MAX_PATH]="";

	CObjPlayerInfo*	pMyPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "ClubMember_BG" );
	assert(pBG);

	for(int i=0;i<LIST10_SIZE;i++)
	{
		_snprintf( sText, MAX_PATH, "List%d_ClubMemberStatic", i );
		CUIControl* pList = pBG->FindEventControl( sText );
		//ClubList_BG

		pList->SetVisible(N3UI_VISIBLE_FALSE);
	}	
}