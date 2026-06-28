#include "stdafx.h"
#include "PutPacket.h"
#include "PacketManager.h"
#include "ObjectManager.h"
#include "ObjMemberManager.h"
#include "ObjPlayerManager.h"
#include "ObjBallManager.h"
#include "ObjRoomManager.h"
#include "FblFunction.h"

/////////////////////////////////////////////////////////////////////////////////////
// 서버로 보내는 패킷 함수
/////////////////////////////////////////////////////////////////////////////////////
void PutCertifyLogin(char sID[], char sPass[])
{
	int nCheck = 0;
	int nTry   = 0;
	for( nTry = 0; nTry < 3; nTry++ )
	{
		#ifdef _EXTERNAL_SERVER_ // 외부서버 (CYG 임시)
			nCheck = CPacketManager::GetPtr()->ConnectTCPSocket("115.68.53.5 ", 2001);
		#else
			nCheck = CPacketManager::GetPtr()->ConnectTCPSocket("192.168.10.101 ", 2001);
		#endif

		if( nCheck == 0 ) break;
	}
	
	switch( nCheck )
	{
	case 0:
		break;
	case -1:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 10002, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return;
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 10003, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 10004, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		return;
	}

//	Sleep(2000);

	CCSCertifyLogin		cPacket;

	cPacket.m_nCommand		= CM_CERTIFY_LOGIN;
	cPacket.m_nBodySize		= sizeof(CCSCertifyLogin) - HEAD_SIZE;

	_snprintf(cPacket.m_sID, ID_NAME_SIZE, sID);
	_snprintf(cPacket.m_sPass, LOGIN_PASS_SIZE, sPass); 

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
	printf("putcertifylogin...\n");
}
/////////////////////////////////////////////////////////////////////////////////////
void PutInstantLogin()
{
	#ifdef _EXTERNAL_SERVER_ // 외부서버 (CYG 임시)
		int check = CPacketManager::GetPtr()->ConnectTCPSocket("115.68.53.5 ", 2001);
	#else
		int check = CPacketManager::GetPtr()->ConnectTCPSocket("192.168.10.101 ", 2001);
	#endif

	if(check == 0)
	{
	}
	else if(check == -1)
	{
		//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::STATE_SERVER_NOT_OPEN);
		return;
	}
	else
	{
		//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::STATE_DISCONNECT);
		return;
	}
//	Sleep(2000);

	CCSInstantLogin		cPacket;

	cPacket.m_nCommand		= CM_INSTANT_LOGIN;
	cPacket.m_nBodySize		= sizeof(CCSInstantLogin) - HEAD_SIZE;

	cPacket.m_nMemberSeq	= CObjMemberManager::GetPtr()->GetMemberSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutCertifyExit(int nReason)
{
	CCSCertifyExit		cPacket;

	cPacket.m_nCommand		= CM_CERTIFY_EXIT;
	cPacket.m_nBodySize 	= sizeof(CCSCertifyExit) - HEAD_SIZE;

	cPacket.m_nReason		= nReason;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutMemberInfo()
{
	CCSMemberInfo		cPacket;

	cPacket.m_nCommand		= CM_MEMBER_INFO;
	cPacket.m_nBodySize		= sizeof(CCSMemberInfo) - HEAD_SIZE;

	cPacket.m_nMemberSeq	= CObjMemberManager::GetPtr()->GetMemberSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutCharacterInfo()
{
	CCSCharacterInfo			cPacket;

	cPacket.m_nCommand		= CM_CHARACTER_INFO;
	cPacket.m_nBodySize		= sizeof(CCSCharacterInfo) - HEAD_SIZE;

	cPacket.m_nMemberSeq	= CObjMemberManager::GetPtr()->GetMemberSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutCreateCharacter(CCSCreateCharacter* pPacket)
{
	pPacket->m_nCommand		= CM_CREATE_CHARACTER;
	pPacket->m_nBodySize	= sizeof(CCSCreateCharacter) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutDeleteCharacter(CCSDeleteCharacter* pPacket)
{
	pPacket->m_nCommand		= CM_DELETE_CHARACTER;
	pPacket->m_nBodySize	= sizeof(CCSDeleteCharacter) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////]
void PutChoiceCharacter(int nPlayerSeq)
{
	CCSChoiceCharacter			cPacket;

	cPacket.m_nCommand		= CM_CHOICE_CHARACTER;
	cPacket.m_nBodySize		= sizeof(CCSChoiceCharacter) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutServerList(int nChannel)
{
	CCSServerList			cPacket;

	cPacket.m_nCommand		= CM_SERVER_LIST;
	cPacket.m_nBodySize		= sizeof(CCSServerList) - HEAD_SIZE;

	cPacket.m_nChannel		= nChannel;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutExecuteTutorial(int nTutorial, int nOrder)
{
	CCSExecuteTutorial		cPacket;

	cPacket.m_nCommand		= CM_EXECUTE_TUTORIAL;
	cPacket.m_nBodySize		= sizeof(CCSExecuteTutorial) - HEAD_SIZE;

	cPacket.m_nTutorial		= nTutorial;
	cPacket.m_nOrder		= nOrder;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutExecuteQuest(int nQuest, int nOrder)
{
	CCSExecuteQuest		cPacket;

	cPacket.m_nCommand		= CM_EXECUTE_QUEST;
	cPacket.m_nBodySize		= sizeof(CCSExecuteQuest) - HEAD_SIZE;

	cPacket.m_nQuest		= nQuest;
	cPacket.m_nOrder		= nOrder;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameLogin(CAddress* pAddress)
{
	int					check;
	CObjPlayerInfo*		pChoicePlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pChoicePlayer);
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->m_cServerAddress = *(pAddress);

	check = CPacketManager::GetPtr()->ConnectTCPSocket(pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort);
	if(check == 0) //정상
	{}
	else if(check == -1)
	{
		//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::STATE_SERVER_NOT_OPEN);
		return;
	}
	else
	{
		//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::STATE_DISCONNECT);
		return;
	}
	Sleep(2000);

	check = CPacketManager::GetPtr()->ConnectUDPSocket(pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort-1000);
	if(check < 0)
	{
		//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::STATE_DISCONNECT);
		exit(1);
	}

	CCSGameLogin		cPacket;

	cPacket.m_nCommand		= CM_GAME_LOGIN;
	cPacket.m_nBodySize		= sizeof(CCSGameLogin) - HEAD_SIZE;

	cPacket.m_nMemberSeq	= CObjMemberManager::GetPtr()->GetMemberSeq();
	cPacket.m_nPlayerSeq	= pChoicePlayer->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameExit(int nReason)
{
	CCSGameExit		cPacket;

	cPacket.m_nCommand		= CM_GAME_EXIT;
	cPacket.m_nBodySize		= sizeof(CCSGameExit) - HEAD_SIZE;

	cPacket.m_nReason		= nReason;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutUDPConfirm()
{
	CCSUDPConfirm		cPacket;

	cPacket.m_nCommand		= CM_UDP_CONFIRM;
	cPacket.m_nBodySize		= sizeof(CCSUDPConfirm) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 공지사항 목록 (CYG)
void PutNoticeList( int nVersion  )
{
	CCSNoticeList		cPacket;

	cPacket.m_nCommand		= CM_NOTICE_LIST;
	cPacket.m_nBodySize		= sizeof(CCSNoticeList) - HEAD_SIZE;
	cPacket.m_nVersion = nVersion;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutPlayerInfo()
{
	CCSPlayerInfo		cPacket;
	CObjPlayerInfo*		pChoicePlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pChoicePlayer);
	cPacket.m_nCommand		= CM_PLAYER_INFO;
	cPacket.m_nBodySize		= sizeof(CCSPlayerInfo) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= pChoicePlayer->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutSaleList(int nPeriod, int nPage)
{
	CCSSaleList				cPacket;

	cPacket.m_nCommand		= CM_SALE_LIST;
	cPacket.m_nBodySize		= sizeof(CCSSaleList) - HEAD_SIZE;

	cPacket.m_nPeriod		= nPeriod;
	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutRoomInfo()
{
	CCSRoomInfo			cPacket;
	CObjPlayerInfo*		pChoicePlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pChoicePlayer);

	cPacket.m_nCommand		= CM_ROOM_INFO;
	cPacket.m_nBodySize		= sizeof(CCSRoomInfo) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pChoicePlayer->m_nRoomSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutRoomList(int nListKind, int nPage)
{
	CCSRoomList			cPacket;

	cPacket.m_nCommand		= CM_ROOM_LIST;
	cPacket.m_nBodySize		= sizeof(CCSRoomList) - HEAD_SIZE;

	cPacket.m_nListKind		= nListKind;
	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutLobbyList(int nPage)
{
	CCSLobbyList			cPacket;

	cPacket.m_nCommand		= CM_LOBBY_LIST;
	cPacket.m_nBodySize		= sizeof(CCSLobbyList) - HEAD_SIZE;

	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutCreateRoom(CCSCreateRoom* pPacket)
{
	pPacket->m_nCommand		= CM_CREATE_ROOM;
	pPacket->m_nBodySize	= sizeof(CCSCreateRoom) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutSetRoom(CCSSetRoom* pPacket)
{
	pPacket->m_nCommand		= CM_SET_ROOM;
	pPacket->m_nBodySize	= sizeof(CCSSetRoom) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChoiceRoom(CCSChoiceRoom* pPacket)
{
	pPacket->m_nCommand		= CM_CHOICE_ROOM;
	pPacket->m_nBodySize	= sizeof(CCSChoiceRoom) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutQuickRoom()
{
	CCSQuickRoom		cPacket;

	cPacket.m_nCommand		= CM_QUICK_ROOM;
	cPacket.m_nBodySize		= sizeof(CCSQuickRoom) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= 0;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutLeaveRoom()
{
	CCSLeaveRoom		cPacket;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nCommand		= CM_LEAVE_ROOM;
	cPacket.m_nBodySize		= sizeof(CCSLeaveRoom) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pRoom->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeJang(int nPlayerSeq)
{
	CCSChangeJang		cPacket;

	cPacket.m_nCommand		= CM_CHANGE_JANG;
	cPacket.m_nBodySize		= sizeof(CCSChangeJang) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutAthleteInfo()
{
	CCSAthleteInfo		cPacket;
	CObjPlayerInfo*		pChoicePlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pChoicePlayer);

	cPacket.m_nCommand		= CM_ATHLETE_INFO;
	cPacket.m_nBodySize		= sizeof(CCSAthleteInfo) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= pChoicePlayer->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutRobotInfo()
{
	CCSRobotInfo		cPacket;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nCommand		= CM_ROBOT_INFO;
	cPacket.m_nBodySize		= sizeof(CCSRobotInfo) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pRoom->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeGround(int nGroundCode)
{
	CCSChangeGround		cPacket;

	cPacket.m_nCommand		= CM_CHANGE_GROUND;
	cPacket.m_nBodySize		= sizeof(CCSChangeGround) - HEAD_SIZE;

	cPacket.m_nGroundCode	= nGroundCode;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeBall(int nBallCode)
{
	CCSChangeBall		cPacket;

	cPacket.m_nCommand		= CM_CHANGE_BALL;
	cPacket.m_nBodySize		= sizeof(CCSChangeBall) - HEAD_SIZE;

	cPacket.m_nBallCode	= nBallCode;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutForceOut(int nPlayerSeq)
{
	CCSForceOut		cPacket;

	cPacket.m_nCommand		= CM_FORCE_OUT;
	cPacket.m_nBodySize		= sizeof(CCSForceOut) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutInvitePlayer(int nPlayerSeq, const char sMessage[])
{
	CCSInvitePlayer		cPacket;

	cPacket.m_nCommand		= CM_INVITE_PLAYER;
	cPacket.m_nBodySize		= sizeof(CCSInvitePlayer) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= nPlayerSeq;
	_snprintf(cPacket.m_sMessage, MESSAGE_SIZE, sMessage);

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameReady(int nReady)
{
	CCSGameReady		cPacket;

	cPacket.m_nCommand		= CM_GAME_READY;
	cPacket.m_nBodySize		= sizeof(CCSGameReady) - HEAD_SIZE;

	cPacket.m_nReady		= nReady;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameStart()
{
	CCSGameStart		cPacket;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nCommand		= CM_GAME_START;
	cPacket.m_nBodySize		= sizeof(CCSGameStart) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pRoom->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameCount(int nCount)
{
	CCSGameCount		cPacket;

	cPacket.m_nCommand		= CM_GAME_COUNT;
	cPacket.m_nBodySize		= sizeof(CCSGameCount) - HEAD_SIZE;

	cPacket.m_nCount		= nCount;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameLoad(int nStep)
{
	CCSGameLoad			cPacket;

	cPacket.m_nCommand		= CM_GAME_LOAD;
	cPacket.m_nBodySize		= sizeof(CCSGameLoad) - HEAD_SIZE;

	cPacket.m_nStep			= nStep;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGamePlay()
{
	CCSGamePlay			cPacket;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nCommand		= CM_GAME_PLAY;
	cPacket.m_nBodySize		= sizeof(CCSGamePlay) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pRoom->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameResult()
{
	CCSGameResult		cPacket;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pAnyPlayer		= NULL;
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vAthleteList);

	cPacket.m_nCommand = CM_GAME_RESULT;
	cPacket.m_nBodySize = sizeof(CCSGameResult) - HEAD_SIZE;

	pRoom->SetGameResult();

	cPacket.m_nMvpSeq		= pRoom->m_cMvpPlayer.m_nPlayerSeq;
	cPacket.m_nMvpLevel		= pRoom->m_cMvpPlayer.m_nLevel;
	cPacket.m_nMvpPosition	= pRoom->m_cMvpPlayer.m_nPosition;
	_snprintf(cPacket.m_sMvpName, PLAYER_NAME_SIZE, pRoom->m_cMvpPlayer.m_sName);
	cPacket.m_fCurrentTime	= pRoom->m_fCurrentTime;
	cPacket.m_cHomeResult	= pRoom->m_cHomeResult;
	cPacket.m_cAwayResult	= pRoom->m_cAwayResult;
	memcpy(&cPacket.m_cEachResult, &pRoom->m_cEachResult, sizeof(CEachResult)*TEAM_SIZE*2);

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGameEnd()
{
	CCSGameEnd			cPacket;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nCommand		= CM_GAME_END;
	cPacket.m_nBodySize		= sizeof(CCSGameEnd) - HEAD_SIZE;

	cPacket.m_nRoomSeq		= pRoom->GetObjSeq();

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeTeam(int nChangeTeamKind)
{
	CCSChangeTeam		cPacket;

	cPacket.m_nCommand		= CM_CHANGE_TEAM;
	cPacket.m_nBodySize		= sizeof(CCSChangeTeam) - HEAD_SIZE;

	cPacket.m_nChangeTeam	= nChangeTeamKind;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangePosition(CCSChangePosition* pPacket)
{
	pPacket->m_nCommand		= CM_CHANGE_POSITION;
	pPacket->m_nBodySize	= sizeof(CCSChangePosition) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeMent(const char sMent[])
{
	CCSChangeMent		cPacket;

	cPacket.m_nCommand		= CM_CHANGE_MENT;
	cPacket.m_nBodySize	= sizeof(CCSChangeMent) - HEAD_SIZE;

	_snprintf(cPacket.m_sMent, PLAYER_MENT_SIZE, sMent);

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

/////////////////////////////////////////////////////////////////////////////////////
void PutGrowupCharacter( int nPosition )
{
	CCSGrowupCharacter		cPacket;

	cPacket.m_nCommand		= CM_GROWUP_CHARACTER;
	cPacket.m_nBodySize		= sizeof(CCSGrowupCharacter) - HEAD_SIZE;
	cPacket.m_nPosition		= nPosition;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 퀘스트에 대한 보상을한다 (CYG)
void PutQuestReward( short nQuest  )
{
	CCSQuestReward		cPacket;

	cPacket.m_nCommand		= CM_QUEST_REWARD;
	cPacket.m_nBodySize		= sizeof(CCSQuestReward) - HEAD_SIZE;

	cPacket.m_nQuest		= (char)nQuest;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PutShopItemList(int nType, int nBrand, int nPage)
{
	CCSShopItemList		cPacket;

	cPacket.m_nCommand		= CM_SHOPITEM_LIST;
	cPacket.m_nBodySize		= sizeof(CCSShopItemList) - HEAD_SIZE;

	cPacket.m_nType			= nType;
	cPacket.m_nBrand		= nBrand;
	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutEquipItem(const int& nItemSeq)
{
	CCSEquipItem		cPacket;

	cPacket.m_nCommand		= CM_EQUIP_ITEM;
	cPacket.m_nBodySize		= sizeof(CCSEquipItem) - HEAD_SIZE;

	cPacket.m_nItemSeq		= nItemSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutDivestItem(int nItemSeq)
{
	CCSDivestItem		cPacket;

	cPacket.m_nCommand		= CM_DIVEST_ITEM;
	cPacket.m_nBodySize		= sizeof(CCSDivestItem) - HEAD_SIZE;

	cPacket.m_nItemSeq		= nItemSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutBuyItem(CCSBuyItem* pPacket)
{
	pPacket->m_nCommand		= CM_BUY_ITEM;
	pPacket->m_nBodySize	= sizeof(CCSBuyItem) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGiftItem(CCSGiftItem* pPacket)
{
	pPacket->m_nCommand		= CM_GIFT_ITEM;
	pPacket->m_nBodySize	= sizeof(CCSGiftItem) - HEAD_SIZE;
	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutExchangeItem(CCSExchangeItem* pPacket)
{
	pPacket->m_nCommand		= CM_EXCHANGE_ITEM;
	pPacket->m_nBodySize	= sizeof(CCSExchangeItem) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutShopSkillList(int nType, int nPosition, int nPage)
{
	CCSShopSkillList		cPacket;

	cPacket.m_nCommand		= CM_SHOPSKILL_LIST;
	cPacket.m_nBodySize		= sizeof(CCSShopSkillList) - HEAD_SIZE;

	cPacket.m_nType			= nType;
	cPacket.m_nPosition		= nPosition;
	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutEquipSkill(int nSkillSeq)
{
	CCSEquipSkill		cPacket;

	cPacket.m_nCommand		= CM_EQUIP_SKILL;
	cPacket.m_nBodySize		= sizeof(CCSEquipSkill) - HEAD_SIZE;

	cPacket.m_nSkillSeq		= nSkillSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutDivestSkill(int nSkillSeq)
{
	CCSDivestSkill		cPacket;

	cPacket.m_nCommand		= CM_DIVEST_SKILL;
	cPacket.m_nBodySize		= sizeof(CCSDivestSkill) - HEAD_SIZE;

	cPacket.m_nSkillSeq		= nSkillSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutBuySkill(CCSBuySkill* pPacket)
{
	pPacket->m_nCommand		= CM_BUY_SKILL;
	pPacket->m_nBodySize	= sizeof(CCSBuySkill) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutShopTrainingList(int nPage)
{
	CCSShopTrainingList		cPacket;

	cPacket.m_nCommand		= CM_SHOPTRAINING_LIST;
	cPacket.m_nBodySize		= sizeof(CCSShopTrainingList) - HEAD_SIZE;

	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutBuyTraining(CCSBuyTraining* pPacket)
{
	pPacket->m_nCommand		= CM_BUY_TRAINING;
	pPacket->m_nBodySize	= sizeof(CCSBuyTraining) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutShopCeremonyList(int nPage)
{
	CCSShopCeremonyList		cPacket;

	cPacket.m_nCommand		= CM_SHOPCEREMONY_LIST;
	cPacket.m_nBodySize		= sizeof(CCSShopCeremonyList) - HEAD_SIZE;

	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutEquipCeremony(int nCeremonySeq, int nEquipKind)
{
	CCSEquipCeremony		cPacket;

	cPacket.m_nCommand		= CM_EQUIP_CEREMONY;
	cPacket.m_nBodySize		= sizeof(CCSEquipCeremony) - HEAD_SIZE;

	cPacket.m_nCeremonySeq		= nCeremonySeq;
	cPacket.m_nEquipKind		= nEquipKind;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutDivestCeremony(int nCeremonySeq)
{
	CCSDivestCeremony		cPacket;

	cPacket.m_nCommand		= CM_DIVEST_CEREMONY;
	cPacket.m_nBodySize		= sizeof(CCSDivestCeremony) - HEAD_SIZE;

	cPacket.m_nCeremonySeq		= nCeremonySeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutBuyCeremony(CCSBuyCeremony* pPacket)
{
	pPacket->m_nCommand		= CM_BUY_CEREMONY;
	pPacket->m_nBodySize	= sizeof(CCSBuyCeremony) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutQuestList(int nPage)
{
	CCSQuestList		cPacket;

	cPacket.m_nCommand		= CM_QUEST_LIST;
	cPacket.m_nBodySize		= sizeof(CCSQuestList) - HEAD_SIZE;

	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutCreateQuest(CCSCreateQuest* pPacket)
{
	pPacket->m_nCommand		= CM_CREATE_QUEST;
	pPacket->m_nBodySize	= sizeof(CCSCreateQuest) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutTCPPing()
{
	CCSTCPPing			cPacket;

	cPacket.m_nCommand		= CM_TCP_PING;
	cPacket.m_nBodySize		= sizeof(CCSTCPPing) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void GetTCPPing(CHeadPacket* pPacket)
{
	CSCTCPPing*		pBody = (CSCTCPPing*)pPacket;

	switch(pBody->m_nResponse)
	{
	case 0:
		PutTCPPing();
		break;
	case -1:
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void PutSendMessage(CCSSendMessage* pPacket)
{
	pPacket->m_nCommand		= CM_SEND_MESSAGE;
	pPacket->m_nBodySize	= sizeof(CCSSendMessage) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)pPacket, pPacket->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutRaiseFaculty(CFaculty* pFaculty)
{
	CCSRaiseFaculty			cPacket;

	cPacket.m_nCommand		= CM_RAISE_FACULTY;
	cPacket.m_nBodySize		= sizeof(CCSRaiseFaculty) - HEAD_SIZE;

	cPacket.m_cChangeFaculty = (*pFaculty);

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutChangeSetting(CCSChangeSetting* pSetting, int nPlayerSeq )
{
	CCSChangeSetting			cPacket;

	cPacket = (*pSetting);

	cPacket.m_nCommand		= CM_CHANGE_SETTING;
	cPacket.m_nPlayerSeq	= nPlayerSeq;
	cPacket.m_nBodySize		= sizeof(CCSChangeSetting) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

void PutGoalIn()
{
	PutGoalinTcp();
	//CPacketManager::GetPtr()->SendChildUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// P2P 패킷 함수
/////////////////////////////////////////////////////////////////////////////////////
void PutUDPPunching()
{
	CCSUDPPunching		cPacket;
	CObjPlayerInfo*		pChoicePlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pChoicePlayer);
	cPacket.m_nCommand		= CM_UDP_PUNCHING;
	cPacket.m_nBodySize		= sizeof(CCSUDPPunching) - HEAD_SIZE;

	cPacket.m_nPlayerSeq	= pChoicePlayer->GetObjSeq();

	CPacketManager::GetPtr()->SendServerUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
// 각 child에게 액션 상태를 보내주는 함수
// UDP가 되는 사람에게 보낼 시 유실을 고려하여 3회 정도 보낸다
// Relay로 보내야 될 시 TCP로 보내므로 1회만 보낸다
// 그러므로 이 두 상황에 맞게 각각 패킷을 만들어 보낸다
void PutObjectAction()
{
	char				sPacket[PACKET_SIZE];
	CPPObjectAction*	pObjectAction	= (CPPObjectAction*)sPacket;
	CObjPlayerInfo*		pAnyPlayer;
	CObjBallInfo*		pBall			= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vAthleteList);
	int					nCount;

	// UDP용 패킷 만들기 /////////////////////////////////////////////////////////////////////////////////////
	nCount = 0;

	//공 프로토콜 세팅
	pObjectAction->m_cBallAction.m_nState			= pBall->m_nState;
	pObjectAction->m_cBallAction.m_nBallerSeq		= pBall->m_nBallerSeq;
	pObjectAction->m_cBallAction.m_nNoMarkSeq		= pBall->m_nNoMarkSeq;
	pObjectAction->m_cBallAction.m_fOriginTime		= pBall->m_fOriginTime;
	pObjectAction->m_cBallAction.m_cOriginLocation	= ConvertPointToLocation(&pBall->m_cOriginPoint);
	pObjectAction->m_cBallAction.m_cCurrentLocation	= ConvertPointToLocation(&pBall->m_cCurrentPoint);
	pObjectAction->m_cBallAction.m_cOriginVector	= pBall->m_cOriginVector;
	pObjectAction->m_cBallAction.m_nSkillType		= pBall->m_nSkillType;
	pObjectAction->m_cBallAction.m_nPlayTime		= (unsigned short)(pRoom->m_fPlayTime * 100);
	pObjectAction->m_cBallAction.m_fCurrentTime		= pRoom->m_fCurrentTime;

	//선수 프로토콜 세팅
	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			//얼마 이상 이미 보낸 프로토콜은 더이상 보내지 않음
			if(pAnyPlayer->m_cCurrentAction.m_nUDPCounter >= MAX_UDPSEND)
				continue;

			pObjectAction->m_cPlayerAction[nCount].m_nActionSeq			= pAnyPlayer->m_cCurrentAction.m_nActionSeq;
			pObjectAction->m_cPlayerAction[nCount].m_nPlayerSeq			= pAnyPlayer->GetObjSeq();
			pObjectAction->m_cPlayerAction[nCount].m_nAniCode			= pAnyPlayer->m_cCurrentAction.m_nAniCode;
			pObjectAction->m_cPlayerAction[nCount].m_fStartAngle		= pAnyPlayer->m_cCurrentAction.m_fStartAngle;
			pObjectAction->m_cPlayerAction[nCount].m_fEndAngle			= pAnyPlayer->m_cCurrentAction.m_fEndAngle;
			pObjectAction->m_cPlayerAction[nCount].m_nAniSpeed			= pAnyPlayer->m_cCurrentAction.m_nAniSpeed;
			pObjectAction->m_cPlayerAction[nCount].m_nMoveSpeed			= pAnyPlayer->m_cCurrentAction.m_nMoveSpeed;
			pObjectAction->m_cPlayerAction[nCount].m_nSkillType			= pAnyPlayer->m_cCurrentAction.m_nSkillType;
			pObjectAction->m_cPlayerAction[nCount].m_nHeadCode			= pAnyPlayer->m_cCurrentAction.m_nHeadCode;

			pObjectAction->m_cPlayerAction[nCount].m_cNodeLocation		= ConvertPointToLocation(&pAnyPlayer->m_cNodePoint);
			pObjectAction->m_cPlayerAction[nCount].m_cTargetLocation	= ConvertPointToLocation(&pAnyPlayer->m_cTargetPoint);

			++nCount;
			++pAnyPlayer->m_cCurrentAction.m_nUDPCounter;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(nCount != 0) //보낼 사람이 있다
	{
		pObjectAction->m_nCommand	= CM_OBJECT_ACTION;
		pObjectAction->m_nBodySize	= sizeof(CPPObjectAction) + (sizeof(CPlayerAction) * nCount) - HEAD_SIZE;
		pObjectAction->m_nAmount	= nCount;
		CPacketManager::GetPtr()->SendChildUDP((char*)pObjectAction, pObjectAction->m_nBodySize+HEAD_SIZE);
	}

	// TCP용 패킷 만들기 /////////////////////////////////////////////////////////////////////////////////////
	/*
	if(!CPacketManager::GetPtr()->IsSendRelay()) //릴레이로 보낼 사람 없으면 리턴
		return;

	nCount = 0;

	//공 프로토콜 세팅
	pObjectAction->m_cBallAction.m_nState			= pBall->m_nState;
	pObjectAction->m_cBallAction.m_nBallerSeq		= pBall->m_nBallerSeq;
	pObjectAction->m_cBallAction.m_nNoMarkSeq		= pBall->m_nNoMarkSeq;
	pObjectAction->m_cBallAction.m_fOriginTime		= pBall->m_fOriginTime;
	pObjectAction->m_cBallAction.m_cOriginLocation	= ConvertPointToLocation(&pBall->m_cOriginPoint);
	pObjectAction->m_cBallAction.m_cCurrentLocation	= ConvertPointToLocation(&pBall->m_cCurrentPoint);
	pObjectAction->m_cBallAction.m_cOriginVector	= pBall->m_cOriginVector;
	pObjectAction->m_cBallAction.m_nSkillType		= pBall->m_nSkillType;
	pObjectAction->m_cBallAction.m_nPlayTime		= (unsigned short)(pRoom->m_fPlayTime * 100);
	pObjectAction->m_cBallAction.m_fCurrentTime		= pRoom->m_fCurrentTime;

	//선수 프로토콜 세팅
	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			//1회 이상 이미 보낸 프로토콜은 더이상 보내지 않음
			if(pAnyPlayer->m_cCurrentAction.m_nTCPCounter >= 1)
				continue;

			pObjectAction->m_cPlayerAction[nCount].m_nActionSeq			= pAnyPlayer->m_cCurrentAction.m_nActionSeq;
			pObjectAction->m_cPlayerAction[nCount].m_nPlayerSeq			= pAnyPlayer->GetObjSeq();
			pObjectAction->m_cPlayerAction[nCount].m_nAniCode			= pAnyPlayer->m_cCurrentAction.m_nAniCode;
			pObjectAction->m_cPlayerAction[nCount].m_nStartAngle		= (short)pAnyPlayer->m_cCurrentAction.m_fStartAngle * 100.0f;
			pObjectAction->m_cPlayerAction[nCount].m_nEndAngle			= (short)pAnyPlayer->m_cCurrentAction.m_fEndAngle * 100.0f;
			pObjectAction->m_cPlayerAction[nCount].m_nAniSpeed			= pAnyPlayer->m_cCurrentAction.m_nAniSpeed;
			pObjectAction->m_cPlayerAction[nCount].m_nMoveSpeed			= pAnyPlayer->m_cCurrentAction.m_nMoveSpeed;
			pObjectAction->m_cPlayerAction[nCount].m_nSkillType			= pAnyPlayer->m_cCurrentAction.m_nSkillType;
			pObjectAction->m_cPlayerAction[nCount].m_nHeadCode			= pAnyPlayer->m_cCurrentAction.m_nHeadCode;

			pObjectAction->m_cPlayerAction[nCount].m_cNodeLocation		= ConvertPointToLocation(&pAnyPlayer->m_cNodePoint);
			pObjectAction->m_cPlayerAction[nCount].m_cTargetLocation	= ConvertPointToLocation(&pAnyPlayer->m_cTargetPoint);

			++nCount;
			++pAnyPlayer->m_cCurrentAction.m_nTCPCounter;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(nCount != 0) //보낼 사람이 있다
	{
		return;

		pObjectAction->m_nCommand	= CM_OBJECT_ACTION;
		pObjectAction->m_nBodySize	= sizeof(CPPObjectAction) + (sizeof(CPlayerAction) * nCount) - HEAD_SIZE;
		pObjectAction->m_nAmount	= nCount;
		CPacketManager::GetPtr()->SendChildTCP((char*)pObjectAction, pObjectAction->m_nBodySize+HEAD_SIZE);
	}
	*/
}
//////////////////////////////////////////////////////////////////////////
void PutBallAction()
{
	CPPBallAction		cPacket;
	CObjBallInfo*		pBall			= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	/*
	//본인이 부모라면 리턴한다
	if(pChoicePlayer->GetObjSeq() == pRoom->m_nParentSeq)
	return;

	//공이 물리 상태일 때만 보낸다
	if(pBall->m_nState != BALL_STATE_MOTION)
	return;
	*/

	cPacket.m_nState			= pBall->m_nState;
	cPacket.m_nBallerSeq		= pBall->m_nBallerSeq;
	cPacket.m_fOriginTime		= pBall->m_fOriginTime;
	cPacket.m_cOriginLocation	= ConvertPointToLocation(&pBall->m_cOriginPoint);
	cPacket.m_cCurrentLocation	= ConvertPointToLocation(&pBall->m_cCurrentPoint);
	cPacket.m_cOriginVector		= pBall->m_cOriginVector;
	cPacket.m_nSkillType		= pBall->m_nSkillType;
	cPacket.m_nBallSound		= pBall->m_nBallSound;
	cPacket.m_nBallEffect		= pBall->m_nBallEffect;

	cPacket.m_nCommand	= CM_BALL_ACTION;
	cPacket.m_nBodySize	= sizeof(CPPBallAction);

	CPacketManager::GetPtr()->SendChildUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);

    pBall->m_nBallEffect = 0;
}
//////////////////////////////////////////////////////////////////////////
void PutObjectLocation()
{
	char				sPacket[1024];
	CPPObjectLocation*	pObjectLocation	= (CPPObjectLocation*)sPacket;
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vAthleteList);
	int					nCount			= 0;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			pObjectLocation->m_cPlayerLocation[nCount].m_nPlayerSeq	= pAnyPlayer->GetObjSeq();
//			pObjectLocation->m_cPlayerLocation[nCount].m_nX			= (short)(pAnyPlayer->m_cCurrentPoint.x * 100);
//			pObjectLocation->m_cPlayerLocation[nCount].m_nY			= (short)(pAnyPlayer->m_cCurrentPoint.y * 100);

			pObjectLocation->m_cPlayerLocation[nCount].m_nX			= (short)(pAnyPlayer->m_cNodePoint.x * 100);
			pObjectLocation->m_cPlayerLocation[nCount].m_nY			= (short)(pAnyPlayer->m_cNodePoint.y * 100);

			++nCount;
		}
	}

	pObjectLocation->m_nCommand		= CM_OBJECT_LOCATION;
	pObjectLocation->m_nBodySize	= sizeof(CPPObjectLocation) + (sizeof(CPlayerLocation) * nCount) - HEAD_SIZE;
	pObjectLocation->m_nAmount		= nCount;
	CPacketManager::GetPtr()->SendChildUDP((char*)pObjectLocation, pObjectLocation->m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutGoalIn_UDP()
{
	CPPGoalIn			cPacket;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjBallInfo*		pBall			= CObjBallManager::GetPtr()->GetBallPointer();
	CObjPlayerInfo*		pKicker			= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nKickSeq);
	CObjPlayerInfo*		pAssister		= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nAssistSeq);

	cPacket.m_nGoalType			= pBall->m_nGoalType;
	cPacket.m_nGoalTeam			= pBall->m_nGoalTeam;
	cPacket.m_nShootKind		= pBall->m_nShootKind;
	cPacket.m_nCeremonyCode		= pRoom->m_nCeremonyCode;
	cPacket.m_nGoalSeq			= pBall->m_nKickSeq;
	cPacket.m_nAssistSeq		= pBall->m_nAssistSeq;

	if(pKicker == NULL)		cPacket.m_nGoalerScore		= 0;
	else					cPacket.m_nGoalerScore		= pKicker->m_cCount.m_nCount[ARRAY_COUNT_GOAL];

	if(pAssister == NULL)	cPacket.m_nAssisterScore	= 0;
	else					cPacket.m_nAssisterScore	= pAssister->m_cCount.m_nCount[ARRAY_COUNT_ASSIST];

	cPacket.m_fCurrentTime		= pRoom->m_fCurrentTime;
	cPacket.m_nHomeScore		= static_cast<char>(pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]);
	cPacket.m_nAwayScore		= static_cast<char>(pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]);

	pRoom->m_nGoalerScore		= cPacket.m_nGoalerScore;
	pRoom->m_nAssisterScore		= cPacket.m_nAssisterScore;

	cPacket.m_nCommand	= CM_GOAL_IN;
	cPacket.m_nBodySize	= sizeof(CPPGoalIn);
	CPacketManager::GetPtr()->SendChildUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
void PutInputKey(int nPlayerSeq, CInputKey cInputKey)
{
	CPPInputKey			cPacket;

	cPacket.m_nPlayerSeq	= nPlayerSeq;
	cPacket.m_cInputKey		= cInputKey;

	cPacket.m_nCommand	= CM_INPUT_KEY;
	cPacket.m_nBodySize	= sizeof(CPPInputKey);
	CPacketManager::GetPtr()->SendParentUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE); //Parent 에게
}

/////////////////////////////////////////////////////////////////////////////////////
void PutEmotionInfo(int nEmotionCode)
{
	CPPEmotionInfo		cPacket;
	CObjPlayerInfo*		pPlayer			= CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	if( pPlayer == NULL ) return;

	cPacket.m_nPlayerSeq	= pPlayer->GetObjSeq();
	cPacket.m_nEmotionCode	= nEmotionCode;

	cPacket.m_nCommand	= CM_EMOTION_INFO;
	cPacket.m_nBodySize	= sizeof(CPPEmotionInfo);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;

			cPacket.m_bCheckUDP		= pAnyPlayer->m_bGetUDP;

			CPacketManager::GetPtr()->SendUDP(&pAnyPlayer->m_cUDPAddress, (char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void PutAngerInfo()
{
	CPPAngerInfo		cPacket;
	CObjPlayerInfo*		pPlayer			= CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_ATHLETE);//본인위치
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	cPacket.m_nHomeAngerTime	= pRoom->m_nHomeAngerTime;
	cPacket.m_nAwayAngerTime	= pRoom->m_nAwayAngerTime;

	cPacket.m_nCommand	= CM_ANGER_INFO;
	cPacket.m_nBodySize	= sizeof(CPPAngerInfo);
	CPacketManager::GetPtr()->SendAngerUDP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

/////////////////////////////////////////////////////////////////////////////////////
// 캐릭터 찾기 (CYG)
void PutCharacterSearch( const char * sName )
{
	CCSCharacterSearch		cPacket;

	cPacket.m_nCommand		= CM_CHARACTER_SEARCH;
	cPacket.m_nBodySize		= sizeof(CCSCharacterSearch) - HEAD_SIZE;

	_snprintf(cPacket.m_sName, ID_NAME_SIZE, sName);


	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 아이템 전달 (소포시스템) (CYG)
void PutPostItem( int nItemSeq, int nPlayerSeq )
{
	CCSPostItem		cPacket;

	cPacket.m_nCommand		= CM_POST_ITEM;
	cPacket.m_nBodySize		= sizeof(CCSPostItem) - HEAD_SIZE;

	cPacket.m_nItemSeq = nItemSeq;
	cPacket.m_nPlayerSeq = nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 미션에 대한 보상을 한다. (CYG)
void PutMissionReward( CMission * pMission  )
{
	CCSMissionReward		cPacket;

	cPacket.m_nCommand		= CM_MISSION_REWARD;
	cPacket.m_nBodySize		= sizeof(CCSMissionReward) - HEAD_SIZE;
	memcpy( &cPacket.m_cMission, pMission, sizeof(CMission) );

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)
// nServerCode == 0 이면 모든 서버 데이터 전송
void PutScheduleList( int nServerCode )
{
	CCSScheduleList		cPacket;

	cPacket.m_nCommand		= CM_SCHEDULE_LIST;
	cPacket.m_nBodySize		= sizeof(CCSScheduleList) - HEAD_SIZE;
	cPacket.m_nServerCode   = nServerCode;
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 플레이어 정보 끝 (CYG)
void PutPlayerinfoEnd()
{
	CCSPlayerinfoEnd		cPacket;
	cPacket.m_nCommand		= CM_PLAYERINFO_END;
	cPacket.m_nBodySize		= sizeof(CCSPlayerinfoEnd) - HEAD_SIZE;
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 오토파일럿 모드 설정 (CYG)
void PutAutopilotMode( char bEnable )
{
	CCSAutopilotMode		cPacket;

	cPacket.m_nCommand		= CM_AUTOPILOT_MODE;
	cPacket.m_nBodySize		= sizeof(CCSAutopilotMode) - HEAD_SIZE;
	cPacket.m_bEnable		= bEnable;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 현재 날씨 얻어오기 (CYG)
void PutCurrentWeather()
{
	CCSCurrentWeather		cPacket;
	cPacket.m_nCommand		= CM_CURRENT_WEATHER;
	cPacket.m_nBodySize		= sizeof(CCSCurrentWeather) - HEAD_SIZE;
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 아이템 옵션 갱신 (CYG)
void PutUpdateOption()
{
	CCSUpdateOption			cPacket;

	cPacket.m_nCommand		= CM_UPDATE_OPTION;
	cPacket.m_nBodySize		= sizeof(CCSUpdateOption) - HEAD_SIZE;
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 현재 시간 얻어오기 (CYG)
void PutCurrentTime()
{
	CCSCurrentTime		cPacket;
	memset(&cPacket, 0, sizeof(cPacket));
	cPacket.m_nCommand		= CM_CURRENT_TIME;
	cPacket.m_nBodySize		= sizeof(CCSCurrentTime) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 접속중인 사용자 강제퇴장 (CYG)
void PutDrawforcePlayer( char sID[] ,char sPass[]   )
{
	#ifdef _EXTERNAL_SERVER_ // 외부서버 (CYG 임시)
		int check = CPacketManager::GetPtr()->ConnectTCPSocket("115.68.53.5 ", 2001);
	#else
		int check = CPacketManager::GetPtr()->ConnectTCPSocket("192.168.10.101 ", 2001);
	#endif

	if(check == -1)
		return;

	Sleep(200);
	CCSDrawforcePlayer		cPacket;

	cPacket.m_nCommand		= CM_DRAWFORCE_PLAYER;
	cPacket.m_nBodySize		= sizeof(CCSDrawforcePlayer) - HEAD_SIZE;

	_snprintf(cPacket.m_sID, ID_NAME_SIZE, sID);
	_snprintf(cPacket.m_sPass, LOGIN_PASS_SIZE, sPass); 

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// TCP Ping (CYG)
void PutTcpPing()
{
	CCSTCPPing		cPacket;

	cPacket.m_nCommand		= CM_TCP_PING;
	cPacket.m_nBodySize		= sizeof(CCSTCPPing) - HEAD_SIZE;
	printf( "TCP PING\n" );
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

// 스킬 강화 (CYG)
void PutUpgradeSkill( int nSkillSeq )
{
	CCSUpgradeSkill		cPacket;

	cPacket.m_nCommand		= CM_UPGRADE_SKILL;
	cPacket.m_nBodySize		= sizeof(CCSUpgradeSkill) - HEAD_SIZE;
	cPacket.m_nSkillSeq		= nSkillSeq;
	cPacket.m_nSequence		= 0;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

// 나중에 삭제.. 임시코드
void PutUpgradeSkill_Minus( int nSkillSeq )
{
	CCSUpgradeSkill		cPacket;

	cPacket.m_nCommand		= CM_UPGRADE_SKILL;
	cPacket.m_nBodySize		= sizeof(CCSUpgradeSkill) - HEAD_SIZE;
	cPacket.m_nSkillSeq		= nSkillSeq;
	cPacket.m_nSequence		= -1;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

// 옵션 정보 (CYG)
// 인증/게임서버에서 동일하게 사용할 수 있으며 nPlayerSeq는 인증서버에서만 유효하다.
void PutSettingInfo( int    nPlayerSeq  )
{
	CCSSettingInfo		cPacket;

	cPacket.m_nCommand		= CM_SETTING_INFO;
	cPacket.m_nBodySize		= sizeof(CCSSettingInfo) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
void PutSynchPlayer()
{
	CCSSynchPlayer		cPacket;

	cPacket.m_nCommand		= CM_SYNCH_PLAYER;
	cPacket.m_nBodySize		= sizeof(CCSSynchPlayer) - HEAD_SIZE;
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

/////////////////////////////////////////////////////////////////////////////////////
// 카드로봇 정보 (CYG)
void PutCardbotInfo( int nPlayerSeq  )
{
	CCSCardbotInfo		cPacket;

	cPacket.m_nCommand		= CM_CARDBOT_INFO;
	cPacket.m_nBodySize		= sizeof(CCSCardbotInfo) - HEAD_SIZE;
	cPacket.m_nPlayerSeq    = nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드템 정보 (CYG)
void PutCardInfo( int nPlayerSeq )
{
	CCSCardInfo		cPacket;

	cPacket.m_nCommand		= CM_CARD_INFO;
	cPacket.m_nBodySize		= sizeof(CCSCardInfo) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드장착 (CYG)
void PutEquipCard( int    nCardSeq,char   nEquipKind, char		nEntry )
{
	CCSEquipCard		cPacket;

	cPacket.m_nCommand		= CM_EQUIP_CARD;
	cPacket.m_nBodySize		= sizeof(CCSEquipCard) - HEAD_SIZE;
	cPacket.m_nCardSeq		= nCardSeq;
	cPacket.m_nEquipEntry	= nEntry;
	cPacket.m_nEquipKind	= nEquipKind;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드해제 (CYG)
void PutDivestCard( int    nCardSeq, char		nEquipEntry )
{
	CCSDivestCard		cPacket;

	cPacket.m_nCommand		= CM_DIVEST_CARD;
	cPacket.m_nBodySize		= sizeof(CCSDivestCard) - HEAD_SIZE;
	cPacket.m_nCardSeq		= nCardSeq;
	cPacket.m_nEquipEntry	= nEquipEntry;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 아이템조합 타입 I (CYG)
void PutMixItem1( int  nMixSeq,int  * pItemSeq,char * pItemCnt )
{
	CCSMixItem1		cPacket;

	cPacket.m_nCommand		= CM_MIX_ITEM1;
	cPacket.m_nBodySize		= sizeof(CCSMixItem1) - HEAD_SIZE;
	cPacket.m_nMixSeq		= nMixSeq;
	memcpy(cPacket.m_nItemSeq, pItemSeq, sizeof(int)*ITEM1_MIX_SIZE);
	memcpy(cPacket.m_nItemCnt, pItemCnt, sizeof(char)*ITEM1_MIX_SIZE);
	
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 아이템조합 타입 II (CYG)
void PutMixItem2( int nItemSeq )
{
	CCSMixItem2		cPacket;

	cPacket.m_nCommand		= CM_MIX_ITEM2;
	cPacket.m_nBodySize		= sizeof(CCSMixItem2) - HEAD_SIZE;
	cPacket.m_nItemSeq		= nItemSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드 조합 I (CYG)
void PutMixCard1( char nCardMixType,int  * nCardSeq)
{
	CCSMixCard1		cPacket;

	cPacket.m_nCommand		= CM_MIX_CARD1;
	cPacket.m_nBodySize		= sizeof(CCSMixCard1) - HEAD_SIZE;
	cPacket.m_nCardMixType	= nCardMixType;
	memcpy(cPacket.m_nCardSeq, nCardSeq, sizeof(int)*CARD_MIX_SIZE );

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드 조합 II (CYG)
void PutMixCard2( int  nCardSeq,char nEmblemType,int  nEmblemCnt)
{
	CCSMixCard2		cPacket;

	cPacket.m_nCommand		= CM_MIX_CARD2;
	cPacket.m_nBodySize		= sizeof(CCSMixCard2) - HEAD_SIZE;
	cPacket.m_nCardSeq		= nCardSeq;
	cPacket.m_nEmblemCnt	= nEmblemCnt;
	cPacket.m_nEmblemType	= nEmblemType;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 친구 정보 (CYG)
void PutBuddyInfo( int nPage )
{
	CCSBuddyInfo		cPacket;

	cPacket.m_nCommand		= CM_BUDDY_INFO;
	cPacket.m_nBodySize		= sizeof(CCSBuddyInfo) - HEAD_SIZE;
	cPacket.m_nPage			= nPage;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 골인 TCP 서버 처리 (CYG)
void PutGoalinTcp()
{
	CCSGoalinTcp		cTCPPacket;

	cTCPPacket.m_nCommand		= CM_GOALIN_TCP;
	cTCPPacket.m_nBodySize		= sizeof(CCSGoalinTcp) - HEAD_SIZE;

	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjBallInfo*		pBall			= CObjBallManager::GetPtr()->GetBallPointer();
	CObjPlayerInfo*		pKicker			= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nKickSeq);
	CObjPlayerInfo*		pAssister		= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBall->m_nAssistSeq);

	cTCPPacket.m_cGoalPacket.m_nGoalType			= pBall->m_nGoalType;
	cTCPPacket.m_cGoalPacket.m_nGoalTeam			= pBall->m_nGoalTeam;
	cTCPPacket.m_cGoalPacket.m_nShootKind		= pBall->m_nShootKind;
	cTCPPacket.m_cGoalPacket.m_nCeremonyCode		= pRoom->m_nCeremonyCode;
	cTCPPacket.m_cGoalPacket.m_nGoalSeq			= pBall->m_nKickSeq;
	cTCPPacket.m_cGoalPacket.m_nAssistSeq		= pBall->m_nAssistSeq;

	if(pKicker == NULL)		cTCPPacket.m_cGoalPacket.m_nGoalerScore		= 0;
	else					cTCPPacket.m_cGoalPacket.m_nGoalerScore		= pKicker->m_cCount.m_nCount[ARRAY_COUNT_GOAL];

	if(pAssister == NULL)	cTCPPacket.m_cGoalPacket.m_nAssisterScore	= 0;
	else					cTCPPacket.m_cGoalPacket.m_nAssisterScore	= pAssister->m_cCount.m_nCount[ARRAY_COUNT_ASSIST];

	cTCPPacket.m_cGoalPacket.m_fCurrentTime		= pRoom->m_fCurrentTime;
	cTCPPacket.m_cGoalPacket.m_nHomeScore		= static_cast<char>(pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]);
	cTCPPacket.m_cGoalPacket.m_nAwayScore		= static_cast<char>(pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]);

	pRoom->m_nGoalerScore		= cTCPPacket.m_cGoalPacket.m_nGoalerScore;
	pRoom->m_nAssisterScore		= cTCPPacket.m_cGoalPacket.m_nAssisterScore;

	cTCPPacket.m_cGoalPacket.m_nCommand	= CM_GOAL_IN;
	cTCPPacket.m_cGoalPacket.m_nBodySize	= sizeof(CPPGoalIn);


	CPacketManager::GetPtr()->SendTCP((char*)&cTCPPacket, cTCPPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// ON/OFF 값 공유 (CYG)
void PutSwitchValue( char nType, int nValue)
{
	CCSSwitchValue		cPacket;

	cPacket.m_nCommand		= CM_SWITCH_VALUE;
	cPacket.m_nBodySize		= sizeof(CCSSwitchValue) - HEAD_SIZE;
	cPacket.m_nType			= nType;
	cPacket.m_nValue		= nValue;
	

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 친구추가 (CYG)
void PutAddBuddy(  int nPlayerSeq )
{
	CCSAddBuddy		cPacket;

	cPacket.m_nCommand		= CM_ADD_BUDDY;
	cPacket.m_nBodySize		= sizeof(CCSAddBuddy) - HEAD_SIZE;
	cPacket.m_nPlayerSeq    = nPlayerSeq;
	

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 블랙리스트 정보 (CYG)
void PutBlacklistInfo( int nPage )
{
	CCSBlacklistInfo		cPacket;

	cPacket.m_nCommand		= CM_BLACKLIST_INFO;
	cPacket.m_nBodySize		= sizeof(CCSBlacklistInfo) - HEAD_SIZE;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 블랙리스트 추가 (CYG)
void PutAddBlacklist( int nPlayerSeq )
{
	CCSAddBlacklist		cPacket;

	cPacket.m_nCommand		= CM_ADD_BLACKLIST;
	cPacket.m_nBodySize		= sizeof(CCSAddBlacklist) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 주간 랭킹 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
void PutWeeklyRecord( int nPlayerSeq  )
{
	CCSWeeklyRecord		cPacket;

	cPacket.m_nCommand		= CM_WEEKLY_RECORD;
	cPacket.m_nBodySize		= sizeof(CCSWeeklyRecord) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 친구삭제 (CYG)
void PutDelBuddy( int nPlayerSeq   )
{
	CCSDelBuddy		cPacket;

	cPacket.m_nCommand		= CM_DEL_BUDDY; 
	cPacket.m_nBodySize		= sizeof(CCSDelBuddy) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;	

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 블랙리스트 삭제 (CYG)
void PutDelBlacklist( int nPlayerSeq   )
{
	CCSDelBlacklist		cPacket;

	cPacket.m_nCommand		= CM_DEL_BLACKLIST;
	cPacket.m_nBodySize		= sizeof(CCSDelBlacklist) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

// 주간 랭킹 정보  (CYG)
void PutWeeklyRanking( int nPlayerSeq  )
{
	CCSWeeklyRanking		cPacket;

	cPacket.m_nCommand		= CM_WEEKLY_RANKING;
	cPacket.m_nBodySize		= sizeof(CCSWeeklyRanking) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 랜덤샵의 목록을 요청한다 (CYG)
void PutRandomshopitemList()
{
	CCSRandomshopitemList		cPacket;

	cPacket.m_nCommand		= CM_RANDOMSHOPITEM_LIST;
	cPacket.m_nBodySize		= sizeof(CCSRandomshopitemList) - HEAD_SIZE;
	
	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 랜덤샾의 아이템을 구입한다. (CYG)
void PutBuyRandomitem( char nIdx,char nBuyKind,int  nCode,int  nPrice,int nOptionCode[ITEM_OPTION_SIZE])
{
	CCSBuyRandomitem		cPacket;

	cPacket.m_nCommand		= CM_BUY_RANDOMITEM;
	cPacket.m_nBodySize		= sizeof(CCSBuyRandomitem) - HEAD_SIZE;
	
	cPacket.m_nIdx		= nIdx;
	cPacket.m_nBuyKind	= nBuyKind;
	cPacket.m_nCode		= nCode;
	cPacket.m_nPrice	= nPrice;
	memcpy( cPacket.m_nOptionCode, nOptionCode, sizeof(int)*ITEM_OPTION_SIZE );


	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}

void PutEnchantItem( int  nItemSeq,int  nPrice,char nEnchantType, char nBuyKind )
{
	CCSEnchantItem		cPacket;

	cPacket.m_nCommand		= CM_ENCHANT_ITEM;
	cPacket.m_nBodySize		= sizeof(CCSEnchantItem) - HEAD_SIZE;
	cPacket.m_nItemSeq		= nItemSeq;
	cPacket.m_nPrice		= nPrice;
	cPacket.m_nEnchantType	= nEnchantType;
	cPacket.m_nBuyKind		= nBuyKind;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 랜덤샵을 새로고침한다. (CYG)
void PutRefreshShop( char nBuyKind )
{
	CCSRefreshShop		cPacket;

	cPacket.m_nCommand		= CM_REFRESH_SHOP;
	cPacket.m_nBodySize		= sizeof(CCSRefreshShop) - HEAD_SIZE;
	cPacket.m_nBuyKind		= nBuyKind;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드부스터 구입 (CYG)
void PutBuyCardbooster( int nCode,char nBuyKind,int  nPrice)
{
	CCSBuyCardbooster		cPacket;

	cPacket.m_nCommand		= CM_BUY_CARDBOOSTER;
	cPacket.m_nBodySize		= sizeof(CCSBuyCardbooster) - HEAD_SIZE;
	cPacket.m_nCode			= nCode;
	cPacket.m_nBuyKind		= nBuyKind;
	cPacket.m_nPrice		= nPrice;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 카드 엔트리 세팅 (CYG)
void PutCardEntry( char nEntry )
{
	CCSCardEntry		cPacket;
	cPacket.m_nCommand		= CM_CARD_ENTRY;
	cPacket.m_nBodySize		= sizeof(CCSCardEntry) - HEAD_SIZE;
	cPacket.m_nEntry		= nEntry;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
// 피트인 (CYG)
void PutPitIn( int  nPlayerSeq,char bInOut, char bAdminReq  )
{
	CCSPitIn		cPacket;

	cPacket.m_nCommand		= CM_PIT_IN;
	cPacket.m_nBodySize		= sizeof(CCSPitIn) - HEAD_SIZE;
	cPacket.m_nPlayerSeq	= nPlayerSeq;
	cPacket.m_bInOut		= bInOut;
	cPacket.m_bAdminReq		= bAdminReq;

	CPacketManager::GetPtr()->SendTCP((char*)&cPacket, cPacket.m_nBodySize+HEAD_SIZE);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//--SOURCE_AUTO_INSERT_SCRIPT
