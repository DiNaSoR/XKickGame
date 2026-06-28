#include "stdafx.h"
#include "PutPacket.h"
#include "GameResultDialog.h"
#include "MessageBoxDialog.h"
#include "ReadyRoomProcess.h"
#include "ChoiceRoomProcess.h"
//////////////////////////////////////////////////////////////////////////
//대기룸 부분
IMPLEMENT_USERFUNCTION( WAITROOM_START_BUTTON ) 
{
	//테스트 UI
	PutGameReady(PLAYER_READY_QUESTION);
	printf("내가 보냈수 게임중이면 에러\n");
}
//대기룸 선택으로 되돌아가기
IMPLEMENT_USERFUNCTION( READYROOM_BACK_BUTTON ) 
{
	PutLeaveRoom();
}

// 방설정 창
IMPLEMENT_USERFUNCTION( ROOM_CONFIG )
{
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "RoomConfigView" );
	if(pControl->GetVisible() == N3UI_VISIBLE_TRUE)
		pCurrentProcess->RoomConfigInit(true);
	else
		pCurrentProcess->RoomConfigInit(false);
}

IMPLEMENT_USERFUNCTION( ROOM_CONFIG_OK )
{
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
    pCurrentProcess->SetRoom();
}

IMPLEMENT_USERFUNCTION( INVITE_ROOM )
{
	CInvitationDialog::GetPtr()->Init();
	
	PutLobbyList(0);
}

IMPLEMENT_USERFUNCTION( READYROOM_RENDER ) //렌더링 영역 1
{
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_3DRender(windowstr);
}

IMPLEMENT_USERFUNCTION( CHANGE_TEAM ) //홈팀으로 변경버튼
{
	char index = atoi(pParam->GetValue( "Index" ));

	switch( index )
	{
	case 0:
		PutChangeTeam(PLAYER_TEAM_HOME);
		break;
	case 1:
		PutChangeTeam(PLAYER_TEAM_AWAY);
		break;
	case 2:
		PutChangeTeam(PLAYER_TEAM_VIEW);
		break;
	}
}

IMPLEMENT_USERFUNCTION( GameResultOpenInit ) //관람석으로 변경버튼
{
	CGameResultDialog::GetPtr()->Init();
}

IMPLEMENT_USERFUNCTION( ReadyRoomOnClickInfo )
{
	const char* sIndex = pParam->GetValue( "Index" ); 
	if (sIndex == NULL) return;
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomOnClickInfo(atoi(sIndex));
}

IMPLEMENT_USERFUNCTION( ReadyRoomOnClickAddFriend )
{
	const char* sIndex = pParam->GetValue( "Index" ); 
	if (sIndex == NULL) return;
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomOnClickAddFriend(atoi(sIndex));
}

IMPLEMENT_USERFUNCTION( ReadyRoomOnClickAddBlock )
{
	const char* sIndex = pParam->GetValue( "Index" ); 
	if (sIndex == NULL) return;
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomOnClickAddBlock(atoi(sIndex));
}

IMPLEMENT_USERFUNCTION( ReadyRoomOnClickBan )
{
	const char* sIndex = pParam->GetValue( "Index" ); 
	if (sIndex == NULL) return;
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomOnClickBan(atoi(sIndex));
}

IMPLEMENT_USERFUNCTION( ReadyRoomChangePosition )
{
	const char* sIndex = pParam->GetValue( "Index" ); 
	if (sIndex == NULL) return;
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomChangePosition(atoi(sIndex));
}

IMPLEMENT_USERFUNCTION( ExitReadyRoom )
{
	CChattingDialog::GetPtr()->ChattingListClear();
	CChattingDialog::GetPtr()->ChattingListDelete();

	PutLeaveRoom();
}

IMPLEMENT_USERFUNCTION( RECORD_LASTGAME )
{
	if(CPacketManager::GetPtr()->m_cReplayQue.m_cRoomInfo.m_nVersion == 0)
	{
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 250302, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return;
	}
	else
	{
		CPacketManager::GetPtr()->m_cReplayQue.WriteReplayFile();
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 250300, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
	}
}

////////////////////////////////////////////////////////////////////////////
// 인벤 관련
IMPLEMENT_USERFUNCTION( INVEN_CONFIRM )
{
	CInvenDialog::GetPtr()->FUNCTION_InvenConfirm();

	CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	PutPitIn(pPlayer->GetObjSeq(), ROOM_PIT_OUT, false);
}

IMPLEMENT_USERFUNCTION( INVEN_CANCEL )
{
	CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	PutPitIn(pPlayer->GetObjSeq(), ROOM_PIT_OUT, false);
}

IMPLEMENT_USERFUNCTION( POPUP_INVEN )
{
	CInvenDialog::GetPtr()->InitInven();

	CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	PutPitIn(pPlayer->GetObjSeq(), ROOM_PIT_IN, false);
}

IMPLEMENT_USERFUNCTION( INVEN_FRACTION_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->SetSmallFractionStart(index);
}

IMPLEMENT_USERFUNCTION( INVEN_BIG_FRACTION )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->SetEntry(-1);
	CInvenDialog::GetPtr()->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( INVEN_SMALL_FRACTION )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( INVEN_ITEM_PAGE_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->FUNCTION_GoPage(index);
}

IMPLEMENT_USERFUNCTION( INVEN_SKILL_PAGE_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->FUNCTION_GoPage(index);
}

IMPLEMENT_USERFUNCTION( ITEM_EQUIP_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->FUNCTION_EquipNDivest(index);
}

IMPLEMENT_USERFUNCTION( SKILL_EQUIP_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CInvenDialog::GetPtr()->FUNCTION_EquipNDivest(index);
}

IMPLEMENT_USERFUNCTION( CHANGE_MAP )
{
	int index = atoi( pParam->GetValue( "Index" ) );

    CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_MapChange(index);
}

IMPLEMENT_USERFUNCTION( CHANGE_BALL )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(pCurrentProcess == NULL) return;
	pCurrentProcess->FUNCTION_BallChange(index);
}

IMPLEMENT_USERFUNCTION( READYROOM_WHISPER )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Send_ReadyRoomOnClickWhisper(index);
}

IMPLEMENT_USERFUNCTION( RR_IS_DETAIL )
{
	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(pCurrentProcess)
		CCreateRoomDialog::GetPtr()->FUNCTION_RR_Is_Detail();
}