#include "stdafx.h"
#include "PutPacket.h"
#include "ChoiceRoomProcess.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UIButton.h"
#include "ChoiceClassProcess.h"
#include "UICommonMethod.h"
#include "ObjServerInfo.h"
#include "UIFunction.h"

//////////////////////////////////////////////////////////////////////////
//룸 선택부분
IMPLEMENT_USERFUNCTION( ChoiceRoom_OK ) 
{
	
}
//서버 선택으로 되돌아가기
IMPLEMENT_USERFUNCTION( BackTo_ChoiceServer ) 
{
	PutGameExit(PLAYER_EXIT_TRANSPORT);
	//PutInstantLogin(); //인증 서버 접속 요청(간단 접속)
}
//내정보 선택으로 들어가기

IMPLEMENT_USERFUNCTION( RoomNumber ) //방을 선택하는 루틴
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;

	int				index = atol( indexstr );
    
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess->FUNCTION_ChoiceRoomIsSecret(index)) {
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Input_Password_Dlg" );
		if ( !pControl ) return;
		pControl->SetVisible(N3UI_VISIBLE_TRUE);

		pControl->Set_FocusedPage(pControl);
		CUIControl::RequestFocus(pControl);
		
		pControl = CClientUIManager::GetPtr()->FindEventControl( "Room_Password" );
		if ( !pControl ) return;
		pControl->ClearText();

		CUIControl::RequestFocus(pControl);
	}
	else
		pCurrentProcess->EnterRoom(index);
}

IMPLEMENT_USERFUNCTION( EnterRoom ) 
{
/*	if(CLobbyDialog::GetPtr()->CurrentSelectRoom < 0)
		return;

	//minkoni
	char szbuff[MAX_PATH];
	sprintf(szbuff, "Room%dButton", CLobbyDialog::GetPtr()->CurrentSelectRoom);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( szbuff );
	RECT rcTexture;
	rcTexture.left = 264; rcTexture.top = 968; rcTexture.right = 788; rcTexture.bottom = 1012;
	CRoomInformation::TextureChange(N3UI_STATE_NORMAL, pControl, "bg_lobby_01.tga", rcTexture);

	CRoomDialog::GetPtr()->EnterRoom();
*/
}

IMPLEMENT_USERFUNCTION( DEnterRoom ) //방을 들어가기 위한 루틴(더블 클릭)
{
//	if(CLobbyDialog::GetPtr()->CurrentSelectRoom < 0)
//		return;

	//minkoni
	char szbuff[MAX_PATH];
//	sprintf(szbuff, "Room%dButton", CLobbyDialog::GetPtr()->CurrentSelectRoom);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( szbuff );
	if(pControl == NULL) return;
	RECT rcTexture;
	rcTexture.left = 264; rcTexture.top = 968; rcTexture.right = 788; rcTexture.bottom = 1012;
//	CRoomInformation::TextureChange(N3UI_STATE_NORMAL, pControl, "bg_lobby_01.tga", rcTexture);

	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;

	int index = atol( indexstr );

//	CLobbyDialog::GetPtr()->CurrentSelectRoom = index;

//	CRoomDialog::GetPtr()->EnterRoom();
}

//////////////////////////////////////////////////////////////////////////
//룸 생성부분

IMPLEMENT_USERFUNCTION( CreateRoomInit )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomInit();
}

IMPLEMENT_USERFUNCTION( CreateRoom )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomInit();
}

IMPLEMENT_USERFUNCTION( CreateRoom_OK )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomOK();
}

IMPLEMENT_USERFUNCTION( CREATEROOM_PM_FIVE )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomLevelRange(5);
}

IMPLEMENT_USERFUNCTION( CREATEROOM_PM_TEN )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomLevelRange(10);
}

IMPLEMENT_USERFUNCTION( CREATEROOM_ALL_LV )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomLevelRange(0);
}

IMPLEMENT_USERFUNCTION( CREATEROOM_SECRET_CHECKBOX )
{
	CCreateRoomDialog::GetPtr()->FUNCTION_CreateRoomSecret();
}

IMPLEMENT_USERFUNCTION( RoomPageStart )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_RoomPageStart();
}

IMPLEMENT_USERFUNCTION( RoomPagePrv )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_RoomPagePrv();
}

IMPLEMENT_USERFUNCTION( RoomPageNext )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_RoomPageNext();
}

IMPLEMENT_USERFUNCTION( RoomPage3Next )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_RoomPage3Next();
}

IMPLEMENT_USERFUNCTION( LobbyPageStart )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_LobbyPageStart();
}

IMPLEMENT_USERFUNCTION( LobbyPagePrv )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_LobbyPagePrv();
}

IMPLEMENT_USERFUNCTION( LobbyPageNext )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_LobbyPageNext();
}

IMPLEMENT_USERFUNCTION( LobbyPage3Next )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_LobbyPage3Next();
}

// 룸 패스워드 확인 부분
IMPLEMENT_USERFUNCTION( PASS_CONFIRM )  
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int	index = atol( indexstr );

	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	if(pCurrentProcess == NULL) return;

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Input_Password_Dlg" );
	if ( !pControl ) return;
	pControl->SetVisible(N3UI_VISIBLE_FALSE);
	
	pCurrentProcess->FUNCTION_ChoiceRoomConfirmPass(index);
}

IMPLEMENT_USERFUNCTION( PASS_HIDE )  
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Input_Password_Dlg" );
	if ( !pControl ) return;
	pControl->SetVisible(N3UI_VISIBLE_FALSE);
}

IMPLEMENT_USERFUNCTION( CHOICEROOM_QUICKSEARCH )
{
	PutQuickRoom();
}

IMPLEMENT_USERFUNCTION( LADDER_JOIN )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->EnterLadderRoom();
}

IMPLEMENT_USERFUNCTION( ROOM_ARRANGE )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	CUIComboBox* pRoomArrangeComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("RoomArrangeComboBox"));
	assert(pCurrentProcess);
	pCurrentProcess->m_nRoomArrange = pRoomArrangeComboBox->GetSelectedItemIndex();
	PutRoomList(pCurrentProcess->m_nRoomArrange, pCurrentProcess->m_nRoomPage);
}

IMPLEMENT_USERFUNCTION( ROOM_ARRANGE2 )
{
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int	index = atol( indexstr );
	
	PutRoomList(index, pCurrentProcess->m_nRoomPage);
}

// 로비리스트 부분(추후 구현)

IMPLEMENT_USERFUNCTION( CHOICEROOM_SELECT_TAB )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;

	int	index = atol( indexstr );

	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_ChangeTab(index);
}

IMPLEMENT_USERFUNCTION( POPUP_LOBBYINFO )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int	index = atol( indexstr );

	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(temp);
	assert(pCurrentProcess);
	pCurrentProcess->m_nPopupIndex = index;

	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "CHOICEROOM_USERLIST_BUTTON%d", index+1);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "LobbyList_PopUp" );
	assert(pControl);
	POINT pt;
	GetCursorPos(&pt);

	RECT rt;
	GetWindowRect(CGameProject::s_pGameProject->GetRenderWindowReference(), &rt);

	pControl->SetVisible(N3UI_VISIBLE_TRUE);
	pControl->SetLocation(pt.x-rt.left-50, pt.y-10-rt.top, true);
}

IMPLEMENT_USERFUNCTION( LOBBYLIST_WHISPER )
{
	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(temp);
	assert(pCurrentProcess);
	int idx = pCurrentProcess->m_nPopupIndex;

	char sText[STRLEN_64]="";
	_snprintf(sText, STRLEN_64, "/%s ",	pCurrentProcess->m_cLobbyList.m_cLobbyData[idx].m_sName);
	UIFunction::Set_ControlText( "ChattingCommand", sText );
	CUIControl::RequestFocus( CClientUIManager::GetPtr()->FindEventControl( "ChattingCommand" ) );
}

IMPLEMENT_USERFUNCTION( LOBBYLIST_ADDFRIEND )
{
	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(temp);
	assert(pCurrentProcess);
	int idx = pCurrentProcess->m_nPopupIndex;
	PutAddBuddy(pCurrentProcess->m_cLobbyList.m_cLobbyData[idx].m_nPlayerSeq);
}

IMPLEMENT_USERFUNCTION( LOBBYLIST_BLOCK )
{

}

//// 퀘스트 관련
//IMPLEMENT_USERFUNCTION( CHOICE_QUEST )
//{
//	const char* indexstr = pParam->GetValue( "Index" );
//	if ( indexstr == 0 ) return;
//	int	index = atol( indexstr );
//
//	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
//	assert(pCurrentProcess);
//
//	pCurrentProcess->m_nQuestCode = pCurrentProcess->m_vQuestCodeList.at(index + (LIST5_SIZE*pCurrentProcess->m_nLobbyPage));
//
//	pCurrentProcess->FUNCTION_CreateQuestInfo();
//}
//
IMPLEMENT_USERFUNCTION( CREATEQUEST_OK )
{
    CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_CreateQuest();
}
//
//IMPLEMENT_USERFUNCTION( SELECT_QUEST )
//{
//	const char* indexstr = pParam->GetValue( "Index" );
//	if ( indexstr == 0 ) return;
//	int	index = atol( indexstr );
//
//	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
//	assert(pCurrentProcess);
//
//	pCurrentProcess->m_nSelectQuestCode = pCurrentProcess->m_vQuestCodeList.at(index + (LIST5_SIZE*pCurrentProcess->m_nLobbyPage));
//}
//
//IMPLEMENT_USERFUNCTION( CREATE_QUEST )
//{
//	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
//	assert(pCurrentProcess);
//
//	pCurrentProcess->m_nQuestCode = pCurrentProcess->m_nSelectQuestCode;
//
//	pCurrentProcess->FUNCTION_CreateQuestInfo();
//}