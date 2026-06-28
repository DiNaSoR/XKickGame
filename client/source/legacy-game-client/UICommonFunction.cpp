#include "stdafx.h"
#include "UIButton.h"
#include "UICommonMethod.h"
#include "ItemShopProcess.h"
#include "SkillShopProcess.h"
#include "SkillBagProcess.h"
#include "ItemBagProcess.h"
#include "ChoiceRoomProcess.h"
#include "ReadyRoomProcess.h"
#include "ChoiceServerProcess.h"
#include "ChoiceTrioProcess.h"
#include "ChoiceClassProcess.h"
#include "GuideTutorialProcess.h"
#include "GameOptionDialog.h"
#include "AudioManager.h"
#include "ChattingDialog.h"
#include "InvitationDialog.h"
#include "ReplayMainProcess.h"
#include "PutPacket.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "MovieManager.h"
#include "UIFunction.h"
#include "TutorialPlay1Process.h"
#include "TutorialMainProcess.h"

using namespace MovieManager;

IMPLEMENT_USERFUNCTION( MessageBox_Ok )
{
	CMessageBoxDialog::GetPtr()->OK();
}

IMPLEMENT_USERFUNCTION( MessageBox_Cancel )
{
	CMessageBoxDialog::GetPtr()->Cancel();
}

IMPLEMENT_USERFUNCTION( MessageBox )
{
	const char* windowstr = pParam->GetValue( "Window" ); 
	assert(windowstr);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(windowstr);
	assert(pControl);
	pControl->SetVisible(N3UI_VISIBLE_TRUE);
	pControl->Set_FocusedPage(pControl);
	CUIControl::RequestFocus(pControl);
}

IMPLEMENT_USERFUNCTION( ShowWindow )//새 다이얼로그를 보여줄 때 사용한다.
{
	const char* windowstr = pParam->GetValue( "Window" );
	assert(windowstr);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( windowstr );
	assert(pControl);

	pControl->SetVisible(N3UI_VISIBLE_TRUE);
	pControl->Set_FocusedPage(pControl);
	CUIControl::RequestFocus(pControl);
}

IMPLEMENT_USERFUNCTION( HideWindow )//새 다이얼로그를 감출때 사용한다.
{
	const char* windowstr = pParam->GetValue( "Window" );
	assert(windowstr);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( windowstr );
	assert(pControl);

	pControl->SetVisible(N3UI_VISIBLE_FALSE);
}

IMPLEMENT_USERFUNCTION( ShowUI )//UI를 보일때 쓴다.- 채팅창 처리를 하지 않음
{
	const char* windowstr = pParam->GetValue( "Window" );
	assert(windowstr);
	UIFunction::Set_UIVisibility(windowstr, N3UI_VISIBLE_TRUE);
}

IMPLEMENT_USERFUNCTION( HideUI )//UI를 감출 때 쓴다.- 채팅창 처리를 하지 않음
{
	const char* windowstr = pParam->GetValue( "Window" );
	assert(windowstr);
	UIFunction::Set_UIVisibility(windowstr, N3UI_VISIBLE_FALSE);
}
IMPLEMENT_USERFUNCTION( SetFocus )
{
	const char* windowstr = pParam->GetValue( "Window" );
	assert(windowstr);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( windowstr );
	assert(pControl);

	CUIControl::RequestFocus(pControl);
}

IMPLEMENT_USERFUNCTION( INFO_HIDENSHOW )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );

	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Character_Information");
	assert(pAni);

	if(index)
	{	
		pAni->SetInitUpdate(false);
		pAni->SetPlay(true, false, true, 6);

		UIFunction::Set_UIVisibility( "HideInfo", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "ShowInfo", N3UI_VISIBLE_FALSE );
	}
	else
	{
		pAni->SetInitUpdate(false);
		pAni->SetPlay(true);

		UIFunction::Set_UIVisibility( "HideInfo", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "ShowInfo", N3UI_VISIBLE_TRUE );
	}
}

//페이지 변경부분

IMPLEMENT_USERFUNCTION( GoTo_GuideTutorial ) //퀘스트 페이지 가기
{
	CProcessManager::GetPtr()->SetNextProcess(new CGuideTutorialProcess(0));
}

IMPLEMENT_USERFUNCTION( GoTo_ItemShop ) 
{
	CProcessManager::GetPtr()->SetNextProcess(new CItemShopProcess);
}

//스킬상점 들어가기
IMPLEMENT_USERFUNCTION( GoTo_SkillShop ) 
{
	CProcessManager::GetPtr()->SetNextProcess(new CSkillShopProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ItemBag )
{
	CProcessManager::GetPtr()->SetNextProcess(new CItemBagProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_SkillBag )
{
	CProcessManager::GetPtr()->SetNextProcess(new CSkillBagProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ChoiceRoom )
{
	CProcessManager::GetPtr()->SetNextProcess(new CChoiceRoomProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ChoiceServer )
{
	CProcessManager::GetPtr()->SetNextProcess(new CChoiceServerProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ChoiceTrio ) 
{
	CProcessManager::GetPtr()->SetNextProcess(new CChoiceTrioProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_QuestMain ) 
{
//??	CProcessManager::GetPtr()->SetNextProcess(new CTutorialStep2MainProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ChoiceClass ) 
{
	CProcessManager::GetPtr()->SetNextProcess(new CChoiceClassProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_ReplayMain )
{
	CProcessManager::GetPtr()->SetNextProcess(new CReplayMainProcess);
}

IMPLEMENT_USERFUNCTION( GoTo_TutorialMain )
{
	CProcessManager::GetPtr()->SetNextProcess(new CTutorialMainProcess);
}

IMPLEMENT_USERFUNCTION( SHOW_PLAYERINFO_DIALOG ) 
{
    SN3UIUserFunctionParameter cParamA, cParamB, cParamC, cParamD;
	cParamA.AddParam( "Window", "PlayerInformation" );
	CClientUIManager::GetPtr()->ProcUserFunction( "MessageBox", &cParamA );

	CClientUIManager::GetPtr()->ProcUserFunction( "InfoD_PlayerInformationInit", &cParamB );

	cParamC.AddParam( "Window", "Static_PlayerInfo" );
	cParamC.AddParam( "Index", "400" );
	CClientUIManager::GetPtr()->ProcUserFunction( "OnClickTab", &cParamC );

	cParamD.AddParam( "Window", "Static_PlayerInfoCharacter" );
	cParamD.AddParam( "Index", "500" );
	CClientUIManager::GetPtr()->ProcUserFunction( "OnClickTab", &cParamD );
}

IMPLEMENT_USERFUNCTION( GoTo_GameEnd ) 
{
	TiXmlElement* pElement = CGameProject::s_pGameProject->GetTiXml("WindowPosition");
	char sText[STRLEN_8]="";
	
	RECT rc;
	GetWindowRect(CGameProject::s_pGameProject->GetWindowReference(), &rc);

	int x = rc.left;
	int y = rc.top;

	_snprintf(sText, STRLEN_8, "%d", x);
	pElement->SetAttribute( "X", sText );
	_snprintf(sText, STRLEN_8, "%d", y);
	pElement->SetAttribute( "Y", sText );

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");

    NiWindowRef hWnd = NiApplication::ms_pkApplication->GetAppWindow()->GetWindowReference();
    ::PostMessage( hWnd, WM_DESTROY, 0, 0 );
}

//////////////////////////////////////////////////////////////////////////
//사운드 관련부분
IMPLEMENT_USERFUNCTION( Sound )
{
	const char* soundstr = pParam->GetValue( "Sound" );
	CAudioManager::GetPtr()->Create2DSound(soundstr);
}

//////////////////////////////////////////////////////////////////////////
//채팅 관련부분
IMPLEMENT_USERFUNCTION( ChattingEnter )
{
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;

	CChattingDialog::GetPtr()->FUNCTION_ChattingEnter(windowstr);
	CChattingDialog::GetPtr()->m_nWhisperIdIndex = -1;
}

IMPLEMENT_USERFUNCTION( CHAT_SORT )
{
	CChattingDialog::GetPtr()->ChattingListClear();

	std::deque<CHAT_MESSAGE>::iterator iter;

	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort( index );
}	

IMPLEMENT_USERFUNCTION( OnClickWhisper )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	char sText[MAX_PATH]="";

	char sR[MAX_PATH]="";

	sprintf(sText, "Static_WhisperList%d", index); 
	CUIControl* pListID = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pListID);
	
	CUIControl* pCommand = CClientUIManager::GetPtr()->FindEventControl( "ChattingCommand" );
	assert(pCommand);

	WideCharToMultiByte(CP_ACP, 0, pListID->GetText(), MAX_PATH, sText, MAX_PATH, NULL, NULL);
	sprintf(sR, "/%s ", sText);
	
	// 리스트박스 없애기
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Static_WhisperPopUp" );
	assert(pControl);
	pControl->SetVisible(N3UI_VISIBLE_FALSE);

	pCommand->SetText(sR);
	pCommand->RequestFocus(pCommand);
}

// 플레이어 정보 관련

IMPLEMENT_USERFUNCTION( PLAYERINFO_BIGFRACTION )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CPlayerInformationDialog::GetPtr()->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( INFO_SUBTAB_MOVE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atoi( indexstr );
	
	CPlayerInformationDialog::GetPtr()->m_nSmallFractionStart += index;

	CPlayerInformationDialog::GetPtr()->FUNCTION_SmallFractionInit(CPlayerInformationDialog::GetPtr()->m_nBigFractionIndex);
}

IMPLEMENT_USERFUNCTION( PLAYERINFO_SUBTAB )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );

	CPlayerInformationDialog::GetPtr()->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( InfoD_PlayerInformationInit )
{
    CObjPlayerInfo * pMeInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pMeInfo);	
	CPlayerInformationDialog::GetPtr()->SetPlayer( pMeInfo );
}

IMPLEMENT_USERFUNCTION( Change_Ment )
{
    CPlayerInformationDialog::GetPtr()->FUNCTION_ChangeMent();
}

IMPLEMENT_USERFUNCTION( PlayerInfoClose )
{
    CPlayerInformationDialog::GetPtr()->FUNCTION_Close();
}

IMPLEMENT_USERFUNCTION( TradeInfo )
{
    int nIndex = atoi(pParam->GetValue( "Index" ));

    CPlayerInformationDialog::GetPtr()->FUNCTION_TradePageView( 0 );
}

IMPLEMENT_USERFUNCTION( InfoD_AbilityUpgrade_Button )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CPlayerInformationDialog::GetPtr()->OnClick_AbillityUp(index);
}

IMPLEMENT_USERFUNCTION( InfoD_AbilityDowngrade_Button )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CPlayerInformationDialog::GetPtr()->OnClick_AbillityDown(index);
}

IMPLEMENT_USERFUNCTION( InfoDConfirm_Ability )
{
	CPlayerInformationDialog::GetPtr()->InfoDConfirm_Ability();
}

IMPLEMENT_USERFUNCTION( InfoD_ReceivePagePrv )
{
}

IMPLEMENT_USERFUNCTION( InfoD_ReceivePageNext )
{
}

IMPLEMENT_USERFUNCTION( INFO_TRADE_PAGE_CHANGE )
{
    int index = atoi(pParam->GetValue( "Index" ));
    CPlayerInformationDialog::GetPtr()->FUNCTION_TradePageView( index );
}

IMPLEMENT_USERFUNCTION( InfoD_MonthRankingPagePrv )
{
}

IMPLEMENT_USERFUNCTION( InfoD_MonthRankingPageNext )
{
}

IMPLEMENT_USERFUNCTION( InfoD_TotalRankingPagePrv )
{
}

IMPLEMENT_USERFUNCTION( InfoD_TotalRankingPageNext )
{
}

IMPLEMENT_USERFUNCTION( INFO_CONSUME_PAGE_CHANGE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atoi( indexstr );

	CPlayerInformationDialog::GetPtr()->m_nConsumPage += index;
	CPlayerInformationDialog::GetPtr()->FUNCTION_PlayerConsumptionInformation();
}

IMPLEMENT_USERFUNCTION( EQUIP_CONSUMITEM )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	index += LIST10_SIZE*CPlayerInformationDialog::GetPtr()->m_nConsumPage;

	CObjPlayerInfo * pPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	if( CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(index)->m_nEquipKind == EQUIP_KIND_YES )
		PutDivestItem(CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(index)->m_nItemSeq);
	else
	{
		for(int i = 0;i < (int)CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.size(); i++)
		{
			if( (CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(index)->m_pTable->m_nType == CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(i)->m_pTable->m_nType) && 
				(CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(index)->m_pTable->m_nWear == CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(i)->m_pTable->m_nWear) )
				pPlayerInfo->m_vItemList.at(i)->m_nEquipKind = 0;
		}
		PutEquipItem(CPlayerInformationDialog::GetPtr()->m_vConsumptionItem.at(index)->m_nItemSeq);
	}
}

IMPLEMENT_USERFUNCTION( ATHLETE_PAGE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	int nPage = CPlayerInformationDialog::GetPtr()->m_nAthletePage;
	int nTotalPage = CPlayerInformationDialog::GetPtr()->m_nAthleteTotalPage;

	nPage += index;

    if(nPage<0)	
		nPage = 0;
	else if(nTotalPage<=nPage)	
		nPage = nTotalPage;
	
	switch( CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex )
	{
	case CLUB_LIST:
		break;
	case BUDDY_LIST:
		PutBuddyInfo(CPlayerInformationDialog::GetPtr()->m_nAthletePage);
		break;
	case BLOCK_LIST:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//옵션 창 부분
IMPLEMENT_USERFUNCTION( GameOptionInit )
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	
	if( pObjPlayerInfo != NULL )
		CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(pObjPlayerInfo);
	else
		CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(CGameOptionDialog::GetPtr()->m_pObjPlayerInfo);
}

IMPLEMENT_USERFUNCTION( GameOptionOK )
{
	CGameOptionDialog::GetPtr()->m_bOption = false;
	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionOK();
}

IMPLEMENT_USERFUNCTION( OP_CANCEL )
{
	CGameOptionDialog::GetPtr()->FUNCTION_SoundInit();
	CGameOptionDialog::GetPtr()->m_bKeyConfig = false;
}

/////////////////
//게임설정 부분//
/////////////////

IMPLEMENT_USERFUNCTION( OP_LABEL )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->GameOptionLabelChange(index);
}

IMPLEMENT_USERFUNCTION( OP_WHISPER )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->GameOptionWhisperChange(index);
}

IMPLEMENT_USERFUNCTION( OP_INVITE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->GameOptionInviteChange(index);
}

IMPLEMENT_USERFUNCTION( OP_SOUND )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionEffectSound(index);
}

IMPLEMENT_USERFUNCTION( OP_BGM )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionBgm(index);
}


////////////////////
//비디오 설정 부분//
////////////////////

IMPLEMENT_USERFUNCTION( GOP_PERFORMANCE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetPerformanceChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_CHARACTERLIGHT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetCharacterLightChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_LIGHTMAP )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetLightMapChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_WEATHER )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetWeatherChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_GROUND )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetGroundChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_EFFECT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetEffectChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_SHADOW )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetShadowChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_SOFT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetAntialiasingChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_SPECULAR )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->FUNCTION_SetSpecularChange(index);
}

IMPLEMENT_USERFUNCTION( GOP_SCREEN )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->SetVideoScreen(index);
}

IMPLEMENT_USERFUNCTION( GOP_MODE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	
	CGameOptionDialog::GetPtr()->SetWideScreen(index);
}

IMPLEMENT_USERFUNCTION( VIDEO_INIT )
{
	CGameOptionDialog::GetPtr()->FUNCTION_VideoLoading();
}
//////////////////////////////////////////////////////////////////////////
//키 세팅부

IMPLEMENT_USERFUNCTION( CONTROL_CHANGE )
{
	CGameOptionDialog::GetPtr()->ControllerToggle();
}

IMPLEMENT_USERFUNCTION( CONTROL_CONF )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atoi( indexstr );

	if( index )
		CGameOptionDialog::GetPtr()->FUNCTION_ControlConfSet(true);
	else
		CGameOptionDialog::GetPtr()->FUNCTION_ControlConfSet(false);
}

IMPLEMENT_USERFUNCTION( KeySelect )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr != 0 )
	{
		int index = atol( indexstr );		
		CGameOptionDialog::GetPtr()->SetKeySelect(index);
	}
}

IMPLEMENT_USERFUNCTION( GO_DEFAULT )
{
	CCSChangeSetting pSetting;
	pSetting.m_nInitSetting = 1;
	if( CGameOptionDialog::GetPtr()->m_pObjPlayerInfo != NULL)
	{	
		PutChangeSetting(&pSetting, CGameOptionDialog::GetPtr()->m_pObjPlayerInfo->GetObjSeq());
	}
}

IMPLEMENT_USERFUNCTION( KEYINIT )
{
	CGameOptionDialog::GetPtr()->m_bKeyConfig = true;
	char szbuff[MAX_PATH];

	for(int i=0;i<12;i++)
	{
		sprintf(szbuff, "KeySelect%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szbuff);
		assert(pControl);

		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900047 ,0));
		assert(pMTable);

		if(!strcmp(pControl->GetTextEx().c_str(), pMTable->m_sMessage))
		{
			CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
			CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, pObjPlayerInfo->m_cSetting.m_nDefineKey[i], 0));
			assert(temp);
			pControl->SetText(temp->m_sName);
		}
	}
}

IMPLEMENT_USERFUNCTION( CHANGE_SKILL )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;

	char sText[STRLEN_64]="";
	
	_snprintf( sText, STRLEN_64, "Combo_Skill_%s" , indexstr );
	CUIComboBox* pComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
	assert(pComboBox);

	int index = atol( indexstr );

	if( (index%LIST10_SIZE) == 0 )
		CGameOptionDialog::GetPtr()->m_nAttackSkillCode[index/LIST10_SIZE] = (int)pComboBox->GetSelectedData();
	else
		CGameOptionDialog::GetPtr()->m_nDefenceSkillCode[index/LIST10_SIZE] = (int)pComboBox->GetSelectedData();
}
///////////////////
// 배경음악 설정 //
///////////////////
IMPLEMENT_USERFUNCTION( BGM_USER )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );
	
	if( index )
		CGameOptionDialog::GetPtr()->BGM_Choice(true);
	else
		CGameOptionDialog::GetPtr()->BGM_Choice(false);
}

IMPLEMENT_USERFUNCTION( USER_MUSIC )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );
    
	CGameOptionDialog::GetPtr()->FUNCTION_UserMusicConf(index);
}

IMPLEMENT_USERFUNCTION( BGM_PAGE_CHANGE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );

	CGameOptionDialog::GetPtr()->m_nBGMPage+=index;
	if( CGameOptionDialog::GetPtr()->m_bUserBGM )
		CGameOptionDialog::GetPtr()->FUNCTION_UserMusicInit();
	else
		CGameOptionDialog::GetPtr()->FUNCTION_DefaultMusicInit();
}

/////////////////
// 퀵채팅 부분 //
/////////////////
IMPLEMENT_USERFUNCTION( QUICKC_USER )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	if( index )
		CGameOptionDialog::GetPtr()->FUNCTION_QuickChoice(true);
	else
		CGameOptionDialog::GetPtr()->FUNCTION_QuickChoice(false);
}

///////////////////
// 언어선택 부분 //
///////////////////
IMPLEMENT_USERFUNCTION( LG_LANG_CHANGE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	char index = atoi( indexstr );
	CGameOptionDialog::GetPtr()->FUNCTION_LanguageSelect(index);
}

IMPLEMENT_USERFUNCTION( LG_ADJUST )
{
	CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_LANGEXIT, 20700, CMessageBoxDialog::MSGBOXTYPE_CONFIRMCANCEL);
}

//////////////////////////////////////////////////////////////////////////
//게임 텝관련

IMPLEMENT_USERFUNCTION( OnClickTab )
{
	string sIndexstr = pParam->GetValue( "Index" );
	if ( sIndexstr.size() == 0 ) return;

	string windowstr = pParam->GetValue( "Window" );

	const char* indexstr = pParam->GetValue( "Index" );
	int index = atol( indexstr );

	// 초기화
	if( !windowstr.compare( "Static_PlayerInfo" ) )
		CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex = 0;

	// 통계 그래프 관련
	if( !windowstr.compare( "Static_PlayerInfoCharacter" ) )
		CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex = index;

	// 선수 리스트 관련
	if( !windowstr.compare( "Static_AthleteInfo" ) )
	{
		if( index == 301 )		// 친구 리스트일 경우
			PutBuddyInfo(0);
		else
			CPlayerInformationDialog::GetPtr()->FUNCTION_AtheletListInformation(index%100, 0);

		CPlayerInformationDialog::GetPtr()->m_nAthletePage = 0;
        CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex = index%LIST10_SIZE;
	}

	if( !windowstr.compare( "Static_ClubInfo" ) && index == 201 )
		CPlayerInformationDialog::GetPtr()->FUNCTION_ClubMemberList(0);					// 임시 초기화

	// 게임 옵션 관련
	if( !windowstr.compare("Static_GameOption") )
	{
		CGameOptionDialog::GetPtr()->m_nOptionTabIndex = index;
		if(CGameOptionDialog::GetPtr()->m_nOptionTabIndex != 602)
			CGameOptionDialog::GetPtr()->m_bKeyConfig = false;

		if(CGameOptionDialog::GetPtr()->m_nOptionTabIndex != 605)
			CGameOptionDialog::GetPtr()->FUNCTION_LanguageLoading();

		if(CGameOptionDialog::GetPtr()->m_nOptionTabIndex == 601 && CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM))
		{
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20301, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			return;
		}

		if(CGameOptionDialog::GetPtr()->m_nOptionTabIndex == 602 && CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_CHOICETRIO))
		{
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20300, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			return;
		}
	}

	string sWindowstr = pParam->GetValue( "Window" );
	if ( sWindowstr.size() == 0 ) return;

	UICommonMethod::OnClickTab(sIndexstr.c_str(),(sWindowstr+"_Tab").c_str(),(sWindowstr+"_DetailTab").c_str());
}

IMPLEMENT_USERFUNCTION( OP_CAMERA )
{
	string windowstr = pParam->GetValue( "Window" );
	const char* indexstr = pParam->GetValue( "Index" );
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionCameraChange(index);
}

IMPLEMENT_USERFUNCTION( OP_ZOOM )
{
	string windowstr = pParam->GetValue( "Window" );
	const char* indexstr = pParam->GetValue( "Index" );
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionZoomChange(index);
}

IMPLEMENT_USERFUNCTION( OP_CAMERATARGET )
{
	string windowstr = pParam->GetValue( "Window" );
	const char* indexstr = pParam->GetValue( "Index" );
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionCameraTarget(index);
}

IMPLEMENT_USERFUNCTION( OP_CAMERATEAM )
{
	string windowstr = pParam->GetValue( "Window" );
	const char* indexstr = pParam->GetValue( "Index" );
	int index = atol( indexstr );

	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionCameraTeam(index);
}

IMPLEMENT_USERFUNCTION( OP_RAD )
{
	CGameOptionDialog::GetPtr()->FUNCTION_GameOptionRadianChange();
}

IMPLEMENT_USERFUNCTION( PopUp )
{
	const char* sWindowstr = pParam->GetValue( "Window" );
	if ( !sWindowstr ) return;
	const char* sIndexstr = pParam->GetValue( "Index" );
	if ( !sIndexstr ) return;

	if(strlen(sWindowstr))
	{
		CProcessManager::Add_PopUp(sWindowstr);
		CProcessManager::Add_PopUpHoverArea(sWindowstr,sWindowstr);
		if(strlen(sIndexstr))
			CProcessManager::Add_PopUpHoverArea(sWindowstr,sIndexstr);
	}
}

IMPLEMENT_USERFUNCTION( PopUpAddArea )
{
	const char* sWindowstr = pParam->GetValue( "Window" );
	if ( !sWindowstr ) return;
	const char* sIndexstr = pParam->GetValue( "Index" );
	if ( !sIndexstr ) return;

	if(strlen(sWindowstr))
	{
		CProcessManager::Add_PopUpHoverArea(sWindowstr,sIndexstr);
	}
}

IMPLEMENT_USERFUNCTION( WHISPER_POPUP )
{
	CChattingDialog::GetPtr()->CChattingDialog::FUNCTION_ChattingWhisperListPopup();
}

//////////////////////////////////////////////////////////////////////////
// 초대 창 관련
IMPLEMENT_USERFUNCTION( INVITEBTN )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	CInvitationDialog::GetPtr()->ListCheck(index);
}

IMPLEMENT_USERFUNCTION( INVITE_HIDE )
{
	CInvitationDialog::GetPtr()->InvitationCancel();
}

IMPLEMENT_USERFUNCTION( INVITE_OK )
{
	CInvitationDialog::GetPtr()->InvitationOk();
}

IMPLEMENT_USERFUNCTION( INVITE_TAB )
{

}

IMPLEMENT_USERFUNCTION( GO_CHOICESERVER )
{
	PutGameExit(PLAYER_EXIT_TRANSPORT);
}

IMPLEMENT_USERFUNCTION( CR_TAB_CHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CCreateRoomDialog::GetPtr()->FUNCTION_TabChange(index);
}

IMPLEMENT_USERFUNCTION( CR_TIME )
{
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Time"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		PutCurrentTime();
	else
		UICommonMethod::TimeChange(ROOM_TIME_AUTO);
}

IMPLEMENT_USERFUNCTION( CR_WEATHER )
{
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Weather"));
	assert(pChkBox);
	if( pChkBox->GetChecked() )
		PutCurrentWeather();
	else
		UICommonMethod::WeatherChange(ROOM_WEATHER_AUTO);
}

IMPLEMENT_USERFUNCTION( CR_GAME_CHOICE )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	UICommonMethod::GameChange(index);
}

IMPLEMENT_USERFUNCTION( CR_MANCOUNT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	UICommonMethod::ManCountChange(index);
}

IMPLEMENT_USERFUNCTION( CR_AI )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	UICommonMethod::AiChange(index);
}

IMPLEMENT_USERFUNCTION( CR_POINT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );
	UICommonMethod::PointChange(index);
}

///////////////////////////////////////////////////////////////////////////
// 선수 리스트 관련
IMPLEMENT_USERFUNCTION( INVITE_CLUB_MEMBER )
{

}

IMPLEMENT_USERFUNCTION( FOLLOW_CLUB_MEMBER )
{

}

IMPLEMENT_USERFUNCTION( INVITE_FRIEND_MEMBER )
{

}

IMPLEMENT_USERFUNCTION( FOLLOW_FRIEND_MEMBER )
{

}

IMPLEMENT_USERFUNCTION( READY )
{
	CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20400, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
}

IMPLEMENT_USERFUNCTION( POPUP_QUICK )
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "QuickMenu" );
	assert(pControl);

	if( pControl->GetVisible() == N3UI_VISIBLE_TRUE )
		UIFunction::Set_UIVisibility( "QuickMenu", N3UI_VISIBLE_FALSE );
	else
		UIFunction::Set_UIVisibility( "QuickMenu", N3UI_VISIBLE_TRUE );
}