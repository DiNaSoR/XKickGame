#include "stdafx.h"
#include "ReadyRoomProcess.h"
#include "UICommonMethod.h"
#include "UIIMEEditBox.h"
#include "PutPacket.h"
#include "UICommon.h"
#include "UIFunction.h"

#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "../InterfaceLibrary/UIIMEEDITBox.h"

void CReadyRoomProcess::RoomConfigInit(bool bView)
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	// 창 타이틀 설정
	CResMessageTableFormat* pTable = NULL;
		
	if(bView)
	{
		pTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900009 ,0));
		assert(pTable);
		UIFunction::Set_ControlText( "CreateRoom_Title", pTable->m_sMessage);
	}
	else
	{
		pTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900010 ,0));
		assert(pTable);
		UIFunction::Set_ControlText( "CreateRoom_Title", pTable->m_sMessage);
	}

	InitRoomNormalOption();

	if(bView)	// 확인만 활성
	{
		// 위, 아래 위장막
		UIFunction::Set_UIVisibility( "CR_Stc_Disable", N3UI_VISIBLE_TRUE );
		
		// 위 버튼들
		UIFunction::Set_UIVisibility( "CR_Btn_Create", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Cancel", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Ok", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_ViewOk", N3UI_VISIBLE_TRUE );
	}
	else		// 확인, 취소 활성
	{
		// 위, 아래 위장막
		UIFunction::Set_UIVisibility( "CR_Stc_Disable", N3UI_VISIBLE_FALSE );

		// 아래 버튼들
		UIFunction::Set_UIVisibility( "CR_Btn_Create", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CR_Btn_Cancel", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CR_Btn_Ok", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CR_Btn_ViewOk", N3UI_VISIBLE_FALSE );
	}
}

void CReadyRoomProcess::SetRoom()
{
	CObjRoomInfo* pRoom	= &CCreateRoomDialog::GetPtr()->m_pRoom;
	CCreateRoomDialog::GetPtr()->SetRoomNormalOption();

	if( pRoom->m_nMode == ROOM_MODE_BET && m_pObjPlayerInfo->m_cLevel.m_nLevel < 20 )
	{
		pRoom->m_nMode = ROOM_MODE_NORMAL;
		return;
	}

	CCSSetRoom			cPacket;

	cPacket.m_nRoomSeq			= pRoom->GetObjSeq();
	_snprintf(cPacket.m_sTitle, TITLE_NAME_SIZE, "%s",	pRoom->m_sTitle );
	cPacket.m_nState			= pRoom->m_nState;
	_snprintf(cPacket.m_sPass, 5, "%s",	pRoom->m_sPass );
	cPacket.m_nMode				= pRoom->m_nMode;
	cPacket.m_nAttackCode		= pRoom->m_nAttackCode;
	cPacket.m_nScaleCode		= pRoom->m_nScaleCode;
	cPacket.m_nAICode			= pRoom->m_nAICode;
	cPacket.m_nPointCode		= pRoom->m_nPointCode;
	cPacket.m_nStartLevel		= pRoom->m_nStartLevel;
	cPacket.m_nEndLevel			= pRoom->m_nEndLevel;
	cPacket.m_nCheckClub		= pRoom->m_nCheckClub;
	cPacket.m_nCheckTime		= pRoom->m_nCheckTime;
	cPacket.m_nCheckWeather		= pRoom->m_nCheckWeather;
	cPacket.m_nCheckViewChat	= pRoom->m_nCheckViewChat;
	cPacket.m_nCheckView		= pRoom->m_nCheckView;
	cPacket.m_nMaxCount			= pRoom->m_nScaleCode * 2;

	PutSetRoom(&cPacket);
}

void CReadyRoomProcess::Update_ReadyRoomTitle(char* sTitle)
{
	UIFunction::Set_ControlText( "RoomName", sTitle );
}

void CReadyRoomProcess::Update_LevelBounds(char* sLevelBounds)
{
	UIFunction::Set_ControlText( "RoomLevelBound", sLevelBounds );
}

void CReadyRoomProcess::Update_PersonLimit(char* sPersonLimit)
{
	UIFunction::Set_ControlText( "RoomPersonNumberBound", sPersonLimit );
}

/////////////////////////////////////////////////////////////////////////////////////
void CReadyRoomProcess::SetUIReadyRoomPositionSetting()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	if(pRoom == NULL) return;
	char sText[STRLEN_64]="";
	char sPosition[STRLEN_64]="";

	for(int i = 0; i < TEAM_SIZE; i++)
	{
		_snprintf( sText, STRLEN_64, "RPlayerPosition_H_%d", i );
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pControl);

		_snprintf( sText, STRLEN_64, "SetPositionBtn%d", i );
		_snprintf( sPosition, STRLEN_64, "RPositionBtn_%d", pRoom->m_cHomeSeat.m_nReservePosition[i] );
		UIFunction::ChangeChildTexture( pControl, sText, sPosition );
		//UICommonMethod::PositionIconDraw( pControl, "ReservePositionIcon", pRoom->m_cHomeSeat.m_nReservePosition[i], UICommonMethod::HOME_TEAM );

		_snprintf( sText, STRLEN_64, "RPlayerPosition_A_%d", i );
		pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pControl);

		_snprintf( sText, STRLEN_64, "SetPositionBtn%d", i+TEAM_SIZE );
		_snprintf( sPosition, STRLEN_64, "BPositionBtn_%d", pRoom->m_cAwaySeat.m_nReservePosition[i] );
		UIFunction::ChangeChildTexture( pControl, sText, sPosition );

		//UICommonMethod::PositionIconDraw( pControl, "ReservePositionIcon", pRoom->m_cAwaySeat.m_nReservePosition[i], UICommonMethod::HOME_TEAM );
	}
}

void CReadyRoomProcess::SetUIReadyRoomAthleteSetting()
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "MenuTabButton06", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "MenuTabButton16", N3UI_VISIBLE_FALSE );

	for(int i = 0; i < TEAM_SIZE; i++)
	{
		_snprintf( sText, STRLEN_64, "Player_H_%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
		UIFunction::Set_ChildControlText( sText, "PlayerName", "" );
		UIFunction::Set_ChildControlText( sText, "PlayerPosition", "" );
		UIFunction::Set_ChildControlText( sText, "PlayerLevel", "" );
		UIFunction::SetChildToVisibility( sText ,"Static_Mark00", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( sText ,"Static_Mark01", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( sText ,"FittingListImg", N3UI_VISIBLE_FALSE );
		
		_snprintf( sText, STRLEN_64, "RPlayerPosition_H_%d", i );
		UIFunction::Set_ChildNumVisibility( sText, "PopUpBtn", i, N3UI_VISIBLE_FALSE );

		//UIFunction::Set_ChildNumVisibility( sText, "SetPositionBtn", i, N3UI_VISIBLE_FALSE );
		
		_snprintf( sText, STRLEN_64, "Player_A_%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
		UIFunction::Set_ChildControlText( sText, "PlayerName", "" );
		UIFunction::Set_ChildControlText( sText, "PlayerPosition", "" );
		UIFunction::Set_ChildControlText( sText, "PlayerLevel", "" );
		UIFunction::SetChildToVisibility( sText ,"Static_Mark00", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( sText ,"Static_Mark01", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( sText ,"FittingListImg", N3UI_VISIBLE_FALSE );

		_snprintf( sText, STRLEN_64, "RPlayerPosition_A_%d", i );
		UIFunction::Set_ChildNumVisibility( sText, "PopUpBtn", i+TEAM_SIZE, N3UI_VISIBLE_FALSE );

		//UIFunction::Set_ChildNumVisibility( sText, "SetPositionBtn", i+TEAM_SIZE, N3UI_VISIBLE_FALSE );

		if( 3 < i )	continue;
		_snprintf( sText, STRLEN_64, "ObserverPlayer0%d", i );
		CUIControl* pOsb = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pOsb);
		_snprintf( sText, STRLEN_64, "OView%d", i );
		CUIControl* pName = pOsb->FindEventControl( sText );
		assert(pName);
		UIFunction::Set_ChildControlText( pName, "PlayerName", "" );
		UIFunction::Set_ChildNumVisibility( sText, "PopUpBtn", i+ALLTEAM_SIZE, N3UI_VISIBLE_FALSE );
	}

	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	if(pRoom == NULL) return;

	SetUIReadyRoomPositionSetting();

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)	continue;
		
			bool	bTeamJang = false;
			bool	bBangJang = false;

			if((pAnyPlayer->GetObjSeq() == pRoom->m_nHomeJangSeq)||(pAnyPlayer->GetObjSeq() == pRoom->m_nAwayJangSeq))
				bTeamJang = true;

			if((pAnyPlayer->m_nTeam==pRoom->m_nRoomJangTeam) && (bTeamJang))
				bBangJang = true;

			SetUIReadyRoomAthleteInSetting(pAnyPlayer);

			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_HOME)
			{
				_snprintf( sText, STRLEN_64, "RPlayerPosition_H_%d", pAnyPlayer->m_nSeat );
				UIFunction::Set_ChildNumVisibility( sText, "PopUpBtn", pAnyPlayer->m_nSeat, N3UI_VISIBLE_TRUE );
			}
			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_AWAY)
			{
				_snprintf( sText, STRLEN_64, "RPlayerPosition_A_%d", pAnyPlayer->m_nSeat );
				UIFunction::Set_ChildNumVisibility( sText, "PopUpBtn", pAnyPlayer->m_nSeat+TEAM_SIZE, N3UI_VISIBLE_TRUE );
			}
			if(pAnyPlayer->m_nTeam==PLAYER_TEAM_VIEW)
			{
				_snprintf( sText, STRLEN_64, "ObserverPlayer0%d", pAnyPlayer->m_nSeat );
				CUIControl* pOsb = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pOsb);
				_snprintf( sText, STRLEN_64, "OView%d", pAnyPlayer->m_nSeat );
				CUIControl* pName = pOsb->FindEventControl( sText );
				assert(pName);
				UIFunction::Set_ChildNumVisibility( pName, "PopUpBtn", pAnyPlayer->m_nSeat+ALLTEAM_SIZE, N3UI_VISIBLE_TRUE );
			}

			if(m_pObjPlayerInfo->GetObjSeq() == pAnyPlayer->GetObjSeq())
			{
				EnableBanCommand(bBangJang , bTeamJang, pAnyPlayer->m_nTeam == PLAYER_TEAM_HOME);
				EnableTeamJangCommand(pAnyPlayer->m_nTeam , bTeamJang);
				EnableSetBallMapCommand(bBangJang);
				EnableStartRoomCommand(bBangJang);
				EnableSetRoomCommand(bBangJang);
				DisableSelfPopUpCommand(pAnyPlayer);
			}
			Set_MarkList(pAnyPlayer->m_nTeam, pAnyPlayer->m_nSeat, bBangJang, bTeamJang);
		}
	}

	SetGameModeSetting();
	DisableXPositionButton(pRoom);
}

void CReadyRoomProcess::SetUIReadyRoomAthleteInSetting(CObjPlayerInfo* pPlayer)
{
	char sText[STRLEN_64]="";
	
	if(pPlayer->m_nTeam == PLAYER_TEAM_HOME)
	{
		_snprintf( sText, STRLEN_64, "Player_H_%d", pPlayer->m_nSeat );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
	}
	else if( pPlayer->m_nTeam == PLAYER_TEAM_AWAY )
	{
		_snprintf( sText, STRLEN_64, "Player_A_%d", pPlayer->m_nSeat );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
	}
	else if( pPlayer->m_nTeam == PLAYER_TEAM_VIEW )
		_snprintf( sText, STRLEN_64, "ObserverPlayer0%d", pPlayer->m_nSeat);
		
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pControl);

	if( pPlayer->m_nTeam == PLAYER_TEAM_VIEW )
	{
		_snprintf( sText, STRLEN_64, "OView%d", pPlayer->m_nSeat );
		CUIControl* pName = pControl->FindEventControl( sText );
		assert(pName);
		UIFunction::Set_ChildControlText( pName, "PlayerName", pPlayer->m_sName );
		return;
	}
	else
	{
		// 선수 명
		UIFunction::Set_ChildControlText( pControl, "PlayerName", pPlayer->m_sName );
	}

	// 선수 포지션
	UIFunction::Set_ChildControlText( pControl, "PlayerPosition", UICommonMethod::PositionNtoPositionStr(pPlayer->m_nPosition).c_str() );
	
	// 선수 레벨
	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	_snprintf( sText, STRLEN_64, pMTable->m_sMessage, pPlayer->m_cLevel.m_nLevel );
	UIFunction::Set_ChildControlText( pControl, "PlayerLevel", sText );
}

void CReadyRoomProcess::Set_ReadyRoomUI()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);

	VectorPlayerList& pHomePlayerList = CObjPlayerManager::GetPtr()->m_vHomeList;
	VectorPlayerList& pAwayPlayerList = CObjPlayerManager::GetPtr()->m_vAwayList;
	VectorPlayerList& pViewPlayerList = CObjPlayerManager::GetPtr()->m_vViewList;

	Update_ReadyRoomTitle(pRoom->m_sTitle);

	FUNCTION_Pass();

	if(pRoom->m_nCheckClub)
		UIFunction::ChangeTexture( "ClubIcon", "ClubOn" );
	else
		UIFunction::ChangeTexture( "ClubIcon", "ClubOff" );

	switch(pRoom->m_nAICode)
	{
	case 0:
		UIFunction::ChangeTexture( "AiIcon", "AiOff" );
		break;
	case 1:
		UIFunction::ChangeTexture( "AiIcon", "AiAllOn" );
		break;
	case 2:
		UIFunction::ChangeTexture( "AiIcon", "AiKeepOn" );
		break;
	}
		
	if(pRoom->m_nScaleCode == ROOM_SCALE_5)
		UIFunction::ChangeTexture( "ScaleIcon", "RoomSize_5" );
	else if(pRoom->m_nScaleCode == ROOM_SCALE_6)
		UIFunction::ChangeTexture( "ScaleIcon", "RoomSize_6" );

	if(pRoom->m_nState == ROOM_STATE_SECRET)
		UIFunction::ChangeTexture( "SecretIcon", "SecretOn" );
	else
		UIFunction::ChangeTexture( "SecretIcon", "SecretOff" );
	
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "%d~%d",pRoom->m_nStartLevel,pRoom->m_nEndLevel);
	Update_LevelBounds(sText);
	
	_snprintf(sText, STRLEN_64, "%d/%d",pHomePlayerList.size()+pAwayPlayerList.size(),pRoom->m_nMaxCount);
	Update_PersonLimit(sText);

	_snprintf(sText, STRLEN_64, "%d/4",pViewPlayerList.size());
	UIFunction::Set_ControlText( "RoomObserverNumber", sText );
	
	// 맵, 볼 업뎃
	FUNCTION_UpdateMapChange();
	FUNCTION_UpdateBallChange();

	Set_RoomNumber( pRoom->GetObjSeq() );
}

void CReadyRoomProcess::FUNCTION_UpdateMapChange()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	// 그라운드 테이블 로드
	CResGroundTableFormat *pGroundTable = dynamic_cast<CResGroundTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_GROUNDTABLE, pRoom->m_nGroundCode, 0));
	assert(pGroundTable);

	// 맵 이름 출력
	UIFunction::Set_ControlText( "MapName", pGroundTable->m_sName );

	// 맵 출력
	UIFunction::ChangeTexture( "MapImg", pGroundTable->m_sIconFile );
}

void CReadyRoomProcess::FUNCTION_UpdateBallChange()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	// 볼 테이블 로드
	CResBallTableFormat *pBallTable = dynamic_cast<CResBallTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_BALLTABLE, pRoom->m_nBallCode, 0));
	assert(pBallTable);

	// 공 이름 출력
	UIFunction::Set_ControlText( "BallName", pBallTable->m_sName );

	// 공 출력
	char sText[MAX_PATH]="";
	_snprintf(sText, MAX_PATH, "Icon/Ball/%s", pBallTable->m_sIconFile);
	UIFunction::SetTextureChange( "BallImg", sText );
}

void CReadyRoomProcess::FUNCTION_MapChange(char idx)
{
	CObjRoomInfo * pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->m_nGroundCode += idx;

	if(pRoom->m_nGroundCode < 0) pRoom->m_nGroundCode = CResourceManager::GetPtr()->GetResourceSize(RESOURCE_GROUNDTABLE) - 1;
	if(pRoom->m_nGroundCode == CResourceManager::GetPtr()->GetResourceSize(RESOURCE_GROUNDTABLE)) pRoom->m_nGroundCode = 0;

	PutChangeGround(pRoom->m_nGroundCode);
}

void CReadyRoomProcess::FUNCTION_BallChange(char idx)
{
	CObjRoomInfo * pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->m_nBallCode += idx;

	if(pRoom->m_nBallCode < 0) pRoom->m_nBallCode = CResourceManager::GetPtr()->GetResourceSize(RESOURCE_BALLTABLE) - 1;
	if(pRoom->m_nBallCode == CResourceManager::GetPtr()->GetResourceSize(RESOURCE_BALLTABLE)) pRoom->m_nBallCode = 0;

	PutChangeBall(pRoom->m_nBallCode);
}

void CReadyRoomProcess::FUNCTION_Pass()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	CUIControl* pPass = CClientUIManager::GetPtr()->FindEventControl("PasswordBox");
	if(pRoom->m_nState == ROOM_STATE_SECRET)
	{
		pPass->SetVisible(N3UI_VISIBLE_TRUE);
		UIFunction::Set_ControlText( "Password", pRoom->m_sPass );
	}
	else
	{
		UIFunction::Set_ControlText( "Password", "" );
		pPass->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

void CReadyRoomProcess::Set_RoomNumber(const DWORD& nRoomNumber)
{
	char sRoomNumber[STRLEN_64];
	//100의 자리
	sprintf(sRoomNumber, "Num_%d", (nRoomNumber/100)%10);
	CUIControl* pHundredNumber = CClientUIManager::GetPtr()->FindEventControl("RoomNumber_000");
	assert(pHundredNumber);
	pHundredNumber->SetElement((CClientUIManager::GetPtr()->FindEventControl(sRoomNumber))->m_pElement->Clone());
	//10의 자리
	sprintf(sRoomNumber, "Num_%d", (nRoomNumber/10)%10);
	CUIControl* pTenNumber = CClientUIManager::GetPtr()->FindEventControl("RoomNumber_00");
	assert(pTenNumber);
	pTenNumber->SetElement((CClientUIManager::GetPtr()->FindEventControl(sRoomNumber))->m_pElement->Clone());
	//1의 자리
	sprintf(sRoomNumber, "Num_%d", (nRoomNumber/1)%10);
	CUIControl* pOneNumber = CClientUIManager::GetPtr()->FindEventControl("RoomNumber_0");
	assert(pOneNumber);
	pOneNumber->SetElement((CClientUIManager::GetPtr()->FindEventControl(sRoomNumber))->m_pElement->Clone());
}

void CReadyRoomProcess::Send_ReadyRoomOnClickWhisper(DWORD nIndex)
{
	char sText[MAX_PATH]="";

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	int nSeq = 0;

	if(11<nIndex)
		nSeq = pRoom->m_cViewSeat.m_nPlayerSeq[nIndex-ALLTEAM_SIZE];
	else if(5<nIndex)
		nSeq = pRoom->m_cAwaySeat.m_nPlayerSeq[nIndex-TEAM_SIZE];
	else
		nSeq = pRoom->m_cHomeSeat.m_nPlayerSeq[nIndex];

	VectorAnyoneList* pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);
	for( VectorAnyoneList::iterator iAnyone = pAnyoneList->begin(); iAnyone != pAnyoneList->end(); iAnyone++ )
	{
		for(VectorPlayerList::iterator iPlayer = (*iAnyone)->begin(); iPlayer != (*iAnyone)->end(); ++iPlayer)
		{
			CObjPlayerInfo * pPlayer = (*iPlayer);

			if( pPlayer->GetObjSeq() == nSeq )
			{
				sprintf(sText, "/%s ",pPlayer->m_sName);
				break;
			}
		}
	}

	UIFunction::Set_ControlText( "ChattingCommand", sText );
	CUIControl::RequestFocus(CClientUIManager::GetPtr()->FindEventControl( "ChattingCommand" ));
}

void CReadyRoomProcess::Send_ReadyRoomOnClickInfo(DWORD nIndex)
{
	CObjRoomInfo * pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	CReserveSeat * pSeat = 0;

	if( nIndex >= 0 && nIndex <= 5)
	{
		pSeat = &pRoom->m_cHomeSeat;
	}

	if( nIndex >= TEAM_SIZE && nIndex <= 11)
	{
		pSeat = &pRoom->m_cAwaySeat;
		nIndex -= TEAM_SIZE;
	}

	if( nIndex >= ALLTEAM_SIZE && nIndex <= 15)
	{
		pSeat = &pRoom->m_cViewSeat;
		nIndex -= ALLTEAM_SIZE;
	}

	assert( pSeat != 0 );
	if( pSeat == 0 ) return;

	int nSeq = pSeat->m_nPlayerSeq[nIndex];

	VectorAnyoneList* pAnyoneList = CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);
	for( VectorAnyoneList::iterator iAnyone = pAnyoneList->begin(); iAnyone != pAnyoneList->end(); iAnyone++ )
	{
		for(VectorPlayerList::iterator iPlayer = (*iAnyone)->begin(); iPlayer != (*iAnyone)->end(); ++iPlayer)
		{
			CObjPlayerInfo * pPlayer = (*iPlayer);

			if( pPlayer->GetObjSeq() == nSeq )
			{
				CPlayerInformationDialog::GetPtr()->SetPlayer( pPlayer );

				SN3UIUserFunctionParameter cParam;
				cParam.AddParam( "Window", "PlayerInformation" );

				CClientUIManager::GetPtr()->ProcUserFunction( "MessageBox", &cParam );
				return;
			}
		}
	}
}

void CReadyRoomProcess::Send_ReadyRoomOnClickAddFriend(DWORD nIndex)
{

}

void CReadyRoomProcess::Send_ReadyRoomOnClickAddBlock(DWORD nIndex)
{

}

void CReadyRoomProcess::Send_ReadyRoomOnClickBan(DWORD nIndex)
{
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	if(11<nIndex)
		PutForceOut(pRoom->m_cViewSeat.m_nPlayerSeq[nIndex-ALLTEAM_SIZE]);
	else if(5<nIndex)
		PutForceOut(pRoom->m_cAwaySeat.m_nPlayerSeq[nIndex-TEAM_SIZE]);
	else
		PutForceOut(pRoom->m_cHomeSeat.m_nPlayerSeq[nIndex]);
}

void CReadyRoomProcess::Send_ReadyRoomChangePosition(DWORD nIndex)
{
	CCSChangePosition cChangedPosition;

	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	if(pRoom == NULL) return;

	for(DWORD nIndex_H=0;nIndex_H<TEAM_SIZE;nIndex_H++)
		cChangedPosition.m_nHomePosition[nIndex_H]=pRoom->m_cHomeSeat.m_nReservePosition[nIndex_H];

	for(DWORD nIndex_A=0;nIndex_A<TEAM_SIZE;nIndex_A++)
		cChangedPosition.m_nAwayPosition[nIndex_A]=pRoom->m_cAwaySeat.m_nReservePosition[nIndex_A];

	char sText[STRLEN_64]="";

	if(nIndex/600==0)
	{
		_snprintf(sText, STRLEN_64, "RPlayerPosition_H_%d", nIndex/100);
		cChangedPosition.m_nHomePosition[nIndex/100]= static_cast<char>(nIndex%100);
	}
	else
	{
		_snprintf(sText, STRLEN_64, "RPlayerPosition_A_%d", nIndex/100-TEAM_SIZE);
		cChangedPosition.m_nAwayPosition[nIndex/100-TEAM_SIZE] = static_cast<char>(nIndex%100);
	}

	UIFunction::Set_ChildNumVisibility(sText,"Static_PositionListParent",nIndex/100,N3UI_VISIBLE_FALSE);

	if((nIndex/100==11) || (nIndex/100==5))
		return;

	PutChangePosition(&cChangedPosition);

}

void CReadyRoomProcess::Set_MarkList(char nTeam, DWORD nSeatNum, bool bBangJang, bool bTeamJang)
{
	char sText[STRLEN_64]="";

	DWORD nOnFlagNum=0;

	if( nTeam == PLAYER_TEAM_HOME )
		_snprintf(sText, STRLEN_64, "Player_H_%d", nSeatNum);
	else if( nTeam == PLAYER_TEAM_AWAY )
		_snprintf(sText, STRLEN_64, "Player_A_%d", nSeatNum);
	else
		return;

	if(bBangJang)
		UIFunction::SetChildToVisibility( sText, "Static_Mark00", N3UI_VISIBLE_TRUE );
	else
		UIFunction::SetChildToVisibility( sText, "Static_Mark00", N3UI_VISIBLE_FALSE );

	if(bTeamJang)
		UIFunction::SetChildToVisibility( sText, "Static_Mark01", N3UI_VISIBLE_TRUE );
	else
		UIFunction::SetChildToVisibility( sText, "Static_Mark01", N3UI_VISIBLE_FALSE );
}

void CReadyRoomProcess::DisableSelfPopUpCommand(CObjPlayerInfo* pPlayer)
{
	char sText[STRLEN_64]="";

	if(pPlayer->m_nTeam==PLAYER_TEAM_HOME)
	{
		_snprintf(sText, STRLEN_64, "PopUpBtn%d", pPlayer->m_nSeat);
		UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
	}
	if(pPlayer->m_nTeam==PLAYER_TEAM_AWAY)
	{
		_snprintf(sText, STRLEN_64, "PopUpBtn%d", pPlayer->m_nSeat+TEAM_SIZE);
		UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
	}
	if(pPlayer->m_nTeam==PLAYER_TEAM_VIEW)
	{
		_snprintf(sText, STRLEN_64, "PopUpBtn%d", pPlayer->m_nSeat+ALLTEAM_SIZE);
		UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
	}
}

void CReadyRoomProcess::EnableBanCommand(bool bBangJang, bool bTeamJang, bool bHomeTeam)
{
	char sText[STRLEN_64]="";

	for( int i=0;i<ALLATHELTE_SIZE;i++)
	{
		_snprintf(sText, STRLEN_64, "Static_PopUp%d", i);

		if(bBangJang)
			UIFunction::SetChildToVisibility( sText, "Button04", N3UI_VISIBLE_TRUE );
		else if(bTeamJang)
		{	
            if( bHomeTeam && i < TEAM_SIZE )
				UIFunction::SetChildToVisibility( sText, "Button04", N3UI_VISIBLE_TRUE );
			else if( !bHomeTeam && ( TEAM_SIZE <= i && i < ALLTEAM_SIZE ) )
				UIFunction::SetChildToVisibility( sText, "Button04", N3UI_VISIBLE_TRUE );
			else
				UIFunction::SetChildToVisibility( sText, "Button04", N3UI_VISIBLE_FALSE );
		}
		else
			UIFunction::SetChildToVisibility( sText, "Button04", N3UI_VISIBLE_FALSE );
	}
}
void CReadyRoomProcess::EnableTeamJangCommand(char nTeam ,bool bTeamJang)
{
	char sText[STRLEN_64]="";

	for( int i=0;i<ALLTEAM_SIZE;i++)
	{	
		_snprintf( sText, STRLEN_64, "SetPositionBtn%d", i );
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);

		if( bTeamJang && nTeam == PLAYER_TEAM_HOME && i < TEAM_SIZE )
			pBtn->SetEnabled(true);
		else if( bTeamJang && nTeam == PLAYER_TEAM_AWAY && ( TEAM_SIZE <= i && i < ALLTEAM_SIZE ) )
			pBtn->SetEnabled(true);
		else
			pBtn->SetEnabled(false);
	}
}

void CReadyRoomProcess::EnableSetBallMapCommand(bool bBangJang)
{
	UIFunction::Set_UIVisibility("Map_Disable", bBangJang?N3UI_VISIBLE_FALSE:N3UI_VISIBLE_TRUE);
}

void CReadyRoomProcess::EnableSetRoomCommand(bool bBangJang)
{
	UIFunction::Set_UIVisibility("SetRoomConfig", bBangJang ? N3UI_VISIBLE_TRUE : N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility("RoomConfigView", bBangJang ? N3UI_VISIBLE_FALSE : N3UI_VISIBLE_TRUE);
}

void CReadyRoomProcess::EnableStartRoomCommand(bool bBangJang)
{
	UIFunction::Set_UIVisibility("GameStart",bBangJang?N3UI_VISIBLE_TRUE:N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility("GameStartDisable",bBangJang?N3UI_VISIBLE_FALSE:N3UI_VISIBLE_TRUE);
}

void CReadyRoomProcess::DisableXPositionButton(CObjRoomInfo* pRoom)
{
	CObjPlayerInfo*		pObjPlayerInfo	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	bool	bTeamJang = false;

	if((pObjPlayerInfo->GetObjSeq() == pRoom->m_nHomeJangSeq)||(pObjPlayerInfo->GetObjSeq() == pRoom->m_nAwayJangSeq))
		bTeamJang = true;

	CUIButton* pBtn4 = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "SetPositionBtn4" ));
	assert(pBtn4);

	CUIButton* pBtn10 = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "SetPositionBtn10" ));
	assert(pBtn10);

	if(pRoom->m_nScaleCode == ROOM_SCALE_6 && bTeamJang)
	{
		if(pObjPlayerInfo->m_nTeam == PLAYER_TEAM_HOME)
			pBtn4->SetEnabled(true);
		else if(pObjPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY)
			pBtn10->SetEnabled(true);
	}
	else if(pRoom->m_nScaleCode == ROOM_SCALE_5)
	{
		pBtn4->SetEnabled(false);
		pBtn10->SetEnabled(false);
	}
}

void CReadyRoomProcess::ViewOverCheck()
{
	char sText[STRLEN_64]="";

	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);

	for(int i=0;i<4;i++)
	{
		_snprintf(sText, STRLEN_64, "ObserverPlayer0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		_snprintf(sText, STRLEN_64, "OView%d", i);
		CUIControl* pView = CClientUIManager::GetPtr()->FindEventControl(sText);

		RECT rtt;
		/*GetWindowRect(CGameProject::s_pGameProject->GetRenderWindowReference(), &rt)*/;

		pControl->GetWindowBox(rtt);

		//rtt.left += rt.left;
		//rtt.right += rt.left;
		//rtt.top += rt.top;
		//rtt.bottom += rt.top;

		if(UICommonMethod::IsRectInPoint(&rtt, &m_pDNDPoint) && pRoom->m_cViewSeat.m_nPlayerSeq[i])
			pControl->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}

	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			bool	bTeamJang = false;

			if((pAnyPlayer->GetObjSeq() == pRoom->m_nHomeJangSeq)||(pAnyPlayer->GetObjSeq() == pRoom->m_nAwayJangSeq))
				bTeamJang = true;

			bool	bBangJang = false;

			if((pAnyPlayer->m_nTeam==pRoom->m_nRoomJangTeam) && (bTeamJang))
				bBangJang = true;


			if(m_pObjPlayerInfo->GetObjSeq() == pAnyPlayer->GetObjSeq())
			{
				DWORD nIndex=0;
				const DWORD SlotNum=pRoom->m_nScaleCode*2;

				char sText[STRLEN_64];

				if( bBangJang || bTeamJang )
				{
					while(nIndex<SlotNum)
					{
						_snprintf(sText, STRLEN_64, "SetPositionBtn%d", nIndex);
						CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
						assert(pBtn);
						pBtn->SetEnabled(((pAnyPlayer->m_nTeam==PLAYER_TEAM_HOME)&(nIndex<6)&!(pRoom->m_nCource == ROOM_COURCE_COUNT)) | 
							((pAnyPlayer->m_nTeam==PLAYER_TEAM_AWAY)&(nIndex>5)&!(pRoom->m_nCource == ROOM_COURCE_COUNT))?true:false);
						nIndex++;
					}
				}

				DisableXPositionButton(pRoom);

				if(bBangJang && pRoom->m_nCource == ROOM_COURCE_COUNT)
					UIFunction::Set_UIVisibility( "Map_Disable", N3UI_VISIBLE_TRUE );
				else if( bBangJang && ( pRoom->m_nCource == ROOM_COURCE_NONE || pRoom->m_nCource == ROOM_COURCE_READY ) )
					UIFunction::Set_UIVisibility( "Map_Disable", N3UI_VISIBLE_FALSE );
				else
					UIFunction::Set_UIVisibility( "Map_Disable", N3UI_VISIBLE_TRUE );

				if(bTeamJang && pRoom->m_nCource == ROOM_COURCE_COUNT)
					UIFunction::Set_UIVisibility( "GameStartCancel", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( "GameStartCancel", N3UI_VISIBLE_FALSE );
			}
		}
	}
}

void CReadyRoomProcess::SetGameModeSetting()
{
	CObjRoomInfo * pRoomInfo = CObjRoomManager::GetPtr()->GetRoomPointer();

	switch( pRoomInfo->m_nMode )
	{
	case ROOM_MODE_QUEST:
		{
			UIFunction::Set_UIVisibility( "SetRoomConfigDisable", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "SetRoomConfig", N3UI_VISIBLE_FALSE );

			UIFunction::Set_UIVisibility( "LastGameResultDisable", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "LastGameResult", N3UI_VISIBLE_TRUE );

			UIFunction::Set_UIVisibility( "TeamChangeBtn", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "TeamChangeBtnDisable", N3UI_VISIBLE_TRUE );

			UIFunction::Set_UIVisibility( "Map_Disable", N3UI_VISIBLE_TRUE );

			UIFunction::Set_UIVisibility( "InviteBtn", N3UI_VISIBLE_TRUE );

			//UIFunction::Set_UIVisibility( "QuestInformation", N3UI_VISIBLE_TRUE );

			//UIFunction::Set_UIVisibility( "RoomConfigViewBtn", N3UI_VISIBLE_FALSE );

			//CObjRoomInfo * pRoomInfo = CObjRoomManager::GetPtr()->GetRoomPointer();            
			//ViewQuestDetail( pRoomInfo->m_nQuestCode, false );
		}
		break;

	case ROOM_MODE_LADDER:
			UIFunction::Set_UIVisibility( "SetRoomConfigDiable", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "SetRoomConfig", N3UI_VISIBLE_FALSE );

			UIFunction::Set_UIVisibility( "InviteBtn", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "WaitRoom_disable", N3UI_VISIBLE_TRUE );

			//UIFunction::Set_UIVisibility( "QuestInformation", N3UI_VISIBLE_FALSE );

			//UIFunction::Set_UIVisibility( "RoomConfigViewBtn", N3UI_VISIBLE_FALSE );
		break;
	case ROOM_MODE_BET:
	case ROOM_MODE_NORMAL:
	case ROOM_MODE_SINGLE:
		{
			UIFunction::Set_UIVisibility( "SetRoomConfigDisable", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "SetRoomConfig", N3UI_VISIBLE_TRUE );

			UIFunction::Set_UIVisibility( "LastGameResultDisable", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "LastGameResult", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "InviteBtn", N3UI_VISIBLE_TRUE );

			UIFunction::Set_UIVisibility( "TeamChangeBtn", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "TeamChangeBtnDisable", N3UI_VISIBLE_FALSE );

			//UIFunction::Set_UIVisibility( "QuestInformation", N3UI_VISIBLE_FALSE );
		}
		break;
	}
}

void CReadyRoomProcess::InitRoomNormalOption()
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	//// 게임선택
	//switch(pRoom->m_nMode)
	//{
	//case ROOM_MODE_NORMAL:
	//	UICommonMethod::GameChange(0);
	//	break;
	//case ROOM_MODE_BET:
	//	UICommonMethod::GameChange(1);
	//	break;
	//}

	// 방제 설정
	CUIIMEEditBox* pTitleEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Ime_Title"));
	assert(pTitleEdit);
	pTitleEdit->SetText(pRoom->m_sTitle);

	// 비번방 여부 확인 및 설정
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Pass"));
	assert(pChkBox);
	CUIEditBox* pPassEdit = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Stc_Pass"));
	assert(pPassEdit);
	if( pRoom->m_nState == ROOM_STATE_SECRET )
	{
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_TRUE );
		pChkBox->SetCheckValue(true);
		pPassEdit->SetText(pRoom->m_sPass);
	}
	else 
	{
		pPassEdit->ClearText();
		UIFunction::Set_UIVisibility( "PassBox", N3UI_VISIBLE_FALSE );
	}

	// 인원설정
	switch(pRoom->m_nScaleCode)
	{
	case ROOM_SCALE_5:
		UICommonMethod::ManCountChange(0);
		break;
	case ROOM_SCALE_6:
		UICommonMethod::ManCountChange(1);
		break;
	}

	// 인공지능설정
	switch(pRoom->m_nAICode)
	{
	case ROOM_AI_KEEPER:
		UICommonMethod::AiChange(0);
		break;
	case ROOM_AI_ALL:
		UICommonMethod::AiChange(1);
		break;
	case ROOM_AI_NONE:
		UICommonMethod::AiChange(2);
		break;
	}

	// 포인트 설정
	switch(pRoom->m_nPointCode)
	{
	case ROOM_POINT_300:
		UICommonMethod::PointChange(0);
		break;
	case ROOM_POINT_500:
		UICommonMethod::PointChange(1);
		break;
	case ROOM_POINT_1000:
		UICommonMethod::PointChange(2);
		break;
	}

	char sText[STRLEN_4]="";

	// 레벨 범위 설정
	_snprintf(sText, STRLEN_4, "%d", pRoom->m_nStartLevel);
	UIFunction::Set_ControlText( "CR_Min_Lv", sText );
	_snprintf(sText, STRLEN_4, "%d", pRoom->m_nEndLevel);
	UIFunction::Set_ControlText( "CR_Max_Lv", sText );

	InitRoomDetailOption();
}

void CReadyRoomProcess::InitRoomDetailOption()
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	// 포지션 선택 콤보박스 비활성 표시
	UIFunction::Set_UIVisibility( "Position_Disable", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Static_Disable6", N3UI_VISIBLE_FALSE );

	CUIComboBox* pComboBox = NULL;

	char sText[STRLEN_64]="";

	// 포지션 설정
	for(int i=0;i<ROOM_SCALE_6;i++) {
		_snprintf(sText, STRLEN_64, "GRATEROOM_POSITONSETTING%d_LISTBOX", i+1);
		pComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pComboBox);
		pComboBox->SetSelectedByIndex(PositionToConvertIndex(i, pRoom->m_cHomeSeat.m_nReservePosition[i]));
	}

	// 선공 설정
	pComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Cb_Attack"));
	assert(pComboBox);
	pComboBox->SetSelectedByIndex(pRoom->m_nAttackCode);

	// 시간변화 허용
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Time"));
	assert(pChkBox);
	if( pRoom->m_nCheckTime )
	{
		pChkBox->SetChecked(true);
		UICommonMethod::TimeChange(pRoom->m_nCheckTime);
	}
	else
	{
		pChkBox->SetChecked(false);
		UICommonMethod::TimeChange(ROOM_TIME_AUTO);
	}

	// 날씨변화 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Weather"));
	assert(pChkBox);
	if( pRoom->m_nCheckWeather )
	{
		pChkBox->SetChecked(true);
		UICommonMethod::WeatherChange(pRoom->m_nCheckWeather);
	}
	else
	{
		pChkBox->SetChecked(false);
		UICommonMethod::WeatherChange(ROOM_WEATHER_AUTO);
	}

	// 클럽전용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_Club"));
	assert(pChkBox);
	if( pRoom->m_nCheckClub )
		pChkBox->SetChecked(true);
	else
		pChkBox->SetChecked(false);

	// 관전 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_View"));
	assert(pChkBox);
	if( pRoom->m_nCheckView )
		pChkBox->SetChecked(true);
	else
		pChkBox->SetChecked(false);

	// 관전자채팅 허용
	pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CR_Chk_ViewChat"));
	assert(pChkBox);
	if( pRoom->m_nCheckViewChat )
		pChkBox->SetChecked(true);
	else
		pChkBox->SetChecked(false);
}

int CReadyRoomProcess::PositionToConvertIndex(int idx, int nPosition)
{
	switch(idx)
	{
	case 0:
		{
			switch(nPosition)
			{
			case POSITION_FW:	return 0;	break;
			case POSITION_ST:	return 1;	break;
			case POSITION_CF:	return 2;	break;
			case POSITION_WF:	return 3;	break;
			case POSITION_SS:	return 4;	break;
			case POSITION_ALL:	return 5;	break;
			}
		}
		break;
	case 1:
		{
			switch(nPosition)
			{
			case POSITION_MF:	return 0;	break;
			case POSITION_AM:	return 1;	break;
			case POSITION_CM:	return 2;	break;
			case POSITION_SM:	return 3;	break;
			case POSITION_DM:	return 4;	break;
			case POSITION_ALL:	return 5;	break;
			}
		}
		break;
	case 2:
		{
			switch(nPosition)
			{
			case POSITION_DF:	return 0;	break;
			case POSITION_SW:	return 1;	break;
			case POSITION_CB:	return 2;	break;
			case POSITION_SP:	return 3;	break;
			case POSITION_ALL:	return 4;	break;
			}
		}
		break;
	case 3:
	case 4:
		{
			switch(nPosition)
			{
			case POSITION_ALL:	return 0;	break;
			case POSITION_FW:	return 1;	break;
			case POSITION_ST:	return 2;	break;
			case POSITION_CF:	return 3;	break;
			case POSITION_WF:	return 4;	break;
			case POSITION_SS:	return 5;	break;
			case POSITION_MF:	return 6;	break;
			case POSITION_AM:	return 7;	break;
			case POSITION_CM:	return 8;	break;
			case POSITION_SM:	return 9;	break;
			case POSITION_DM:	return 10;	break;
			case POSITION_DF:	return 11;	break;
			case POSITION_SW:	return 12;	break;
			case POSITION_CB:	return 13;	break;
			case POSITION_SP:	return 14;	break;
			}
		}
		break;
	case 5:
		{
			switch(nPosition)
			{
			case POSITION_GK:	return 0;	break;
			}
		}
	default:			return 0;	break;
	}
	return 0;
}

void CReadyRoomProcess::FUNCTION_TeamChangeVisible(N3UI_VISIBLE_TYPE eVISIBLE_TYPE)
{
	if(eVISIBLE_TYPE == N3UI_VISIBLE_TRUE)
	{
		UIFunction::Set_UIVisibility( "TeamChangeBtn", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "TeamChangeBtnDisable", N3UI_VISIBLE_FALSE );
	}
	else 
	{
		UIFunction::Set_UIVisibility( "TeamChangeBtn", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "TeamChangeBtnDisable", N3UI_VISIBLE_TRUE );
	}
}

void CReadyRoomProcess::FUNCTION_MapNameToolTipUpdate()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "BallImg" );
	assert(pControl);

	RECT rt = pControl->GetWindowBox();

	if( UICommonMethod::IsRectInPoint( &rt, &m_pDNDPoint ) )
		UIFunction::Set_UIVisibility( "ToolTip", N3UI_VISIBLE_TRUE );
	else
		UIFunction::Set_UIVisibility( "ToolTip", N3UI_VISIBLE_FALSE );
}

void CReadyRoomProcess::FUNCTION_PitInUpdate()
{
	char sText[STRLEN_64]="";

	VectorAnyoneList* pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);
	CObjRoomInfo* pRoom				= CObjRoomManager::GetPtr()->GetRoomPointer();

	for(int i=0;i<TEAM_SIZE;i++)
	{
		int nHomeSeq = pRoom->m_cHomeSeat.m_nPlayerSeq[i];
		int nAwaySeq = pRoom->m_cAwaySeat.m_nPlayerSeq[i];

		VectorAnyoneList::iterator it;
		VectorPlayerList::iterator itp;
		for(it = pAnyoneList->begin();it != pAnyoneList->end();it++)
		{
			for(itp = (*it)->begin();itp != (*it)->end();itp++)
			{
				if( (*itp)->GetObjSeq() == nHomeSeq )
				{
					_snprintf(sText, STRLEN_64, "Player_H_%d", i);

					if( (*itp)->m_bPit == ROOM_PIT_IN )
						UIFunction::SetChildToVisibility( sText, "FittingListImg", N3UI_VISIBLE_TRUE );
					else
						UIFunction::SetChildToVisibility( sText, "FittingListImg", N3UI_VISIBLE_FALSE );					
					break;
				}

				if( (*itp)->GetObjSeq() == nAwaySeq )
				{
					_snprintf(sText, STRLEN_64, "Player_A_%d", i);
					if( (*itp)->m_bPit == ROOM_PIT_IN )
						UIFunction::SetChildToVisibility( sText, "FittingListImg", N3UI_VISIBLE_TRUE );
					else
						UIFunction::SetChildToVisibility( sText, "FittingListImg", N3UI_VISIBLE_FALSE );

					break;
				}
			}
		}
	}
}