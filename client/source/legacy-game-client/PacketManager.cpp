#include "stdafx.h"
#include "PacketManager.h"
#include "GetPacket.h"
#include "ObjPlayerManager.h"
#include "ObjRoomManager.h"
#include "FootballManager.h"

CPacketManager::CPacketManager()
{
	m_bIsCertifyLoginDone	= false;
	m_bIsMemberInfoDone		= false;
	m_bIsPlayerInfoDone		= false;
	m_bIsChoiceRoomDone		= false;
	m_bIsAthleteInfoDone	= false;
	m_bIsRobotInfoDone		= false;
	m_bIsGameReadyDone		= false;
	m_bIsGameCountDone		= false;
	m_bIsGamePlayDone		= false;
	m_bIsGameEndDone		= false;

	m_bIsCertifyToGame		= false;
	m_bIsLeaveRoom			= false;
	m_bIsRaiseFacultyDone	= true;

	m_pTCPSocket			= NULL;

	m_pUDPSocket			= NULL;

	InitProtocol();
}

CPacketManager::~CPacketManager()
{}

void CPacketManager::InitProtocol()
{
	INSERT_PROTOCOL_FUNCTION(CM_CERTIFY_LOGIN,	GetCertifyLogin);
	INSERT_PROTOCOL_FUNCTION(CM_INSTANT_LOGIN,	GetInstantLogin);
	INSERT_PROTOCOL_FUNCTION(CM_CERTIFY_EXIT,	GetCertifyExit);
	INSERT_PROTOCOL_FUNCTION(CM_MEMBER_INFO,	GetMemberInfo);
	INSERT_PROTOCOL_FUNCTION(CM_CHARACTER_INFO,	GetCharacterInfo);
	INSERT_PROTOCOL_FUNCTION(CM_CHARACTER_END,	GetCharacterEnd);
	INSERT_PROTOCOL_FUNCTION(CM_CREATE_CHARACTER,	GetCreateCharacter);
	INSERT_PROTOCOL_FUNCTION(CM_DELETE_CHARACTER,	GetDeleteCharacter);
	INSERT_PROTOCOL_FUNCTION(CM_CHOICE_CHARACTER,	GetChoiceCharacter);
	INSERT_PROTOCOL_FUNCTION(CM_SERVER_LIST,	GetServerList);
	INSERT_PROTOCOL_FUNCTION(CM_EXECUTE_TUTORIAL,GetExecuteTutorial);
	INSERT_PROTOCOL_FUNCTION(CM_EXECUTE_QUEST,	GetExecuteQuest);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_LOGIN,		GetGameLogin);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_EXIT,		GetGameExit);
	INSERT_PROTOCOL_FUNCTION(CM_UDP_CONFIRM,	GetUDPConfirm);
	INSERT_PROTOCOL_FUNCTION(CM_NOTICE_LIST,	GetNoticeList); // 공지사항 목록 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_PLAYER_INFO,	GetPlayerInfo);
	INSERT_PROTOCOL_FUNCTION(CM_ITEM_INFO,		GetItemInfo);
	INSERT_PROTOCOL_FUNCTION(CM_SKILL_INFO,		GetSkillInfo);
	INSERT_PROTOCOL_FUNCTION(CM_TRAINING_INFO,	GetTrainingInfo);
	INSERT_PROTOCOL_FUNCTION(CM_CEREMONY_INFO,	GetCeremonyInfo);
	INSERT_PROTOCOL_FUNCTION(CM_QUEST_INFO,		GetQuestInfo);
	INSERT_PROTOCOL_FUNCTION(CM_SALE_LIST,		GetSaleList);
	INSERT_PROTOCOL_FUNCTION(CM_ROOM_INFO,		GetRoomInfo);
	INSERT_PROTOCOL_FUNCTION(CM_ROOM_LIST,		GetRoomList);
	INSERT_PROTOCOL_FUNCTION(CM_LOBBY_LIST,		GetLobbyList);
	INSERT_PROTOCOL_FUNCTION(CM_CREATE_ROOM,	GetCreateRoom);
	INSERT_PROTOCOL_FUNCTION(CM_SET_ROOM,		GetSetRoom);
	INSERT_PROTOCOL_FUNCTION(CM_CHOICE_ROOM,	GetChoiceRoom);
	INSERT_PROTOCOL_FUNCTION(CM_QUICK_ROOM,		GetQuickRoom);
	INSERT_PROTOCOL_FUNCTION(CM_LEAVE_ROOM,		GetLeaveRoom);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_PARENT,	GetChangeParent);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_JANG,	GetChangeJang);
	INSERT_PROTOCOL_FUNCTION(CM_ATHLETE_INFO,	GetAthleteInfo);
	INSERT_PROTOCOL_FUNCTION(CM_ATHLETE_END,	GetAthleteEnd);
	INSERT_PROTOCOL_FUNCTION(CM_ROBOT_INFO,		GetRobotInfo);
	INSERT_PROTOCOL_FUNCTION(CM_ROBOT_END,		GetRobotEnd);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_GROUND,	GetChangeGround);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_BALL,	GetChangeBall);
	INSERT_PROTOCOL_FUNCTION(CM_FORCE_OUT,		GetForceOut);
	INSERT_PROTOCOL_FUNCTION(CM_INVITE_PLAYER,	GetInvitePlayer);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_READY,		GetGameReady);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_START,		GetGameStart);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_COUNT,		GetGameCount);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_LOAD,		GetGameLoad);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_PLAY,		GetGamePlay);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_RESULT,	GetGameResult);
	INSERT_PROTOCOL_FUNCTION(CM_GAME_END,		GetGameEnd);
	INSERT_PROTOCOL_FUNCTION(CM_LEVEL_UP,		GetLevelUp);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_TEAM,	GetChangeTeam);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_POSITION,GetChangePosition);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_MENT,	GetChangeMent);
	INSERT_PROTOCOL_FUNCTION(CM_GROWUP_CHARACTER,GetGrowupCharacter);
	INSERT_PROTOCOL_FUNCTION(CM_QUEST_REWARD,	GetQuestReward); // 퀘스트에 대한 보상을한다 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_SHOPITEM_LIST,	GetShopItemList);
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_ITEM,	GetUpdateItem);
	INSERT_PROTOCOL_FUNCTION(CM_EQUIP_ITEM,		GetEquipItem);
	INSERT_PROTOCOL_FUNCTION(CM_DIVEST_ITEM,	GetDivestItem);
	INSERT_PROTOCOL_FUNCTION(CM_BUY_ITEM,		GetBuyItem);
	INSERT_PROTOCOL_FUNCTION(CM_EXCHANGE_ITEM,	GetExchangeItem);
	INSERT_PROTOCOL_FUNCTION(CM_SHOPSKILL_LIST,	GetShopSkillList);
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_SKILL,	GetUpdateSkill);
	INSERT_PROTOCOL_FUNCTION(CM_EQUIP_SKILL,	GetEquipSkill);
	INSERT_PROTOCOL_FUNCTION(CM_DIVEST_SKILL,	GetDivestSkill);
	INSERT_PROTOCOL_FUNCTION(CM_BUY_SKILL,		GetBuySkill);
	INSERT_PROTOCOL_FUNCTION(CM_SHOPTRAINING_LIST,GetShopTrainingList);
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_TRAINING,GetUpdateTraining);
	INSERT_PROTOCOL_FUNCTION(CM_BUY_TRAINING,	GetBuyTraining);
	INSERT_PROTOCOL_FUNCTION(CM_SHOPCEREMONY_LIST,	GetShopCeremonyList);
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_CEREMONY,GetUpdateCeremony);
	INSERT_PROTOCOL_FUNCTION(CM_EQUIP_CEREMONY,	GetEquipCeremony);
	INSERT_PROTOCOL_FUNCTION(CM_DIVEST_CEREMONY,GetDivestCeremony);
	INSERT_PROTOCOL_FUNCTION(CM_BUY_CEREMONY,	GetBuyCeremony);
	INSERT_PROTOCOL_FUNCTION(CM_QUEST_LIST,	GetQuestList);
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_QUEST,	GetUpdateQuest);
	INSERT_PROTOCOL_FUNCTION(CM_CREATE_QUEST,	GetCreateQuest);
	INSERT_PROTOCOL_FUNCTION(CM_SEND_MESSAGE,	GetSendMessage);
	INSERT_PROTOCOL_FUNCTION(CM_RAISE_FACULTY,	GetRaiseFaculty);
	INSERT_PROTOCOL_FUNCTION(CM_CHANGE_SETTING,	GetChangeSetting);
	INSERT_PROTOCOL_FUNCTION(CM_OBJECT_ACTION,	GetObjectAction);
	INSERT_PROTOCOL_FUNCTION(CM_BALL_ACTION,	GetBallAction);
	INSERT_PROTOCOL_FUNCTION(CM_GOAL_IN,		GetGoalIn);
	INSERT_PROTOCOL_FUNCTION(CM_INPUT_KEY,		GetInputKey);
	INSERT_PROTOCOL_FUNCTION(CM_UDP_PING,		GetUDPPing);
	INSERT_PROTOCOL_FUNCTION(CM_EMOTION_INFO,	GetEmotionInfo);
	INSERT_PROTOCOL_FUNCTION(CM_ANGER_INFO,		GetAngerInfo);
	INSERT_PROTOCOL_FUNCTION(CM_CHARACTER_SEARCH,GetCharacterSearch); // 캐릭터 찾기 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_POST_ITEM,		GetPostItem); // 아이템 전달 (소포시스템) (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_MISSION_REWARD,	GetMissionReward); // 미션에 대한 보상을 한다. (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_SCHEDULE_LIST,	GetScheduleList); // 골든,토너먼트	,클럽등의	스케쥴 타임 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_PLAYERINFO_END,	GetPlayerinfoEnd); // 플레이어 정보 끝 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_AUTOPILOT_MODE,	GetAutopilotMode); // 오토파일럿 모드 설정 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CURRENT_WEATHER,GetCurrentWeather); // 현재 날씨 얻어오기 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CONSUME_ITEM,	GetConsumeItem); // 소모성 아이템 사용 통지 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_UPDATE_OPTION,	GetUpdateOption); // 아이템 옵션 갱신 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CURRENT_TIME,	GetCurrentTimePacket); // 현재 시간 얻어오기 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_DRAWFORCE_PLAYER,GetDrawforcePlayer); // 접속중인 사용자 강제퇴장 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_TCP_PING,		GetTcpPing); // TCP Ping (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_UPGRADE_SKILL,	GetUpgradeSkill); // 스킬 강화 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_SETTING_INFO,	GetSettingInfo); // 옵션 정보 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_SYNCH_PLAYER,	GetSynchPlayer); // 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CARDBOT_INFO,	GetCardbotInfo); // 카드로봇 정보 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CARDBOT_END,	GetCardbotEnd); // 카드봇 정보 끝 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CARD_INFO,	GetCardInfo); // 카드템 정보 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_EQUIP_CARD,	GetEquipCard); // 카드장착 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_DIVEST_CARD,	GetDivestCard); // 카드해제 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_GET_CARD,	GetGetCard); // 카드획득 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_MIX_ITEM1,	GetMixItem1); // 아이템조합 타입 I (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_MIX_ITEM2,	GetMixItem2); // 아이템조합 타입 II (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_MIX_CARD1,	GetMixCard1); // 카드 조합 I (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_MIX_CARD2,	GetMixCard2); // 카드 조합 II (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_BUDDY_INFO,	GetBuddyInfo); // 친구 정보 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_GOALIN_TCP,	GetGoalinTcp); // 골인 TCP 서버 처리 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_SWITCH_VALUE,	GetSwitchValue); // ON/OFF 값 공유 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_ADD_BUDDY,	GetAddBuddy); // 친구추가 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_BLACKLIST_INFO,	GetBlacklistInfo); // 블랙리스트 정보 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_ADD_BLACKLIST,	GetAddBlacklist); // 블랙리스트 추가 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_WEEKLY_RECORD,	GetWeeklyRecord); // 주간 랭킹 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_DEL_BUDDY,	GetDelBuddy); // 친구삭제 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_DEL_BLACKLIST,	GetDelBlacklist); // 블랙리스트 삭제 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_WEEKLY_RANKING,	GetWeeklyRanking); // 주간 랭킹 정보  (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_GIFT_ITEM,	GetGiftItem); // 아이템 선물 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_RANDOMSHOPITEM_LIST, GetRandomshopitemList); // 랜덤샵의 목록을 요청한다 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_BUY_RANDOMITEM,	GetBuyRandomitem); // 랜덤샾의 아이템을 구입한다. (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_ENCHANT_ITEM,	GetEnchantItem); // 아이템을 강화한다. (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_REFRESH_SHOP,	GetRefreshShop); // 랜덤샵을 새로고침한다. (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_BUY_CARDBOOSTER,	GetBuyCardbooster); // 카드부스터 구입 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_CARD_ENTRY,	GetCardEntry); // 카드 엔트리 세팅 (CYG)
	INSERT_PROTOCOL_FUNCTION(CM_PIT_IN,	GetPitIn); // 피트인 (CYG)
	//--SOURCE_AUTO_INSERT_SCRIPT
}

int CPacketManager::ConnectTCPSocket(char *sIP, int nPort)
{
	SAFE_DELETE(m_pTCPSocket);
	m_pTCPSocket = new CNetTCPSocket;
	int check = m_pTCPSocket->TcpSocket(sIP, nPort);
	if(check == 0)
	{}
	else
		SAFE_DELETE(m_pTCPSocket);

	return check;
}

void CPacketManager::CloseTCPSocket()
{
	m_pTCPSocket->TcpClose();
	SAFE_DELETE(m_pTCPSocket);
}

int CPacketManager::ConnectUDPSocket(char *sIP, int nPort)
{
	SAFE_DELETE(m_pUDPSocket);
	m_pUDPSocket = new CNetUDPSocket;
	int check = m_pUDPSocket->UdpSocket(sIP, nPort);
	if(check < 0)
		SAFE_DELETE(m_pUDPSocket);

	return check;
}

void CPacketManager::CloseUDPSocket()
{
	m_pUDPSocket->UdpClose();
	SAFE_DELETE(m_pUDPSocket);
}

// 패킷 시퀀스를 생성하고 내용을 암호화한다
void CPacketManager::SecureLayer( CHeadPacket * pPacket )
{
	// 로그인시 서버측 시퀀스로 초기화
	if( pPacket->m_nCommand == CM_GAME_LOGIN )
		m_hTCPSecure.InitSeq( pPacket->m_nSequence );
	else if( pPacket->m_nCommand == CM_GAME_EXIT )
		m_hTCPSecure.m_bInit = false;
	else if( m_hTCPSecure.InitCheck() )
		m_hTCPSecure.MakeRecvPacket(pPacket);
}

bool CPacketManager::SavePacketInList()
{
	CHeadPacket*	pPacket;
	CHeadPacket*	pQueBuffer;

	//TCP 패킷을 리스트에 등록
	if(m_pTCPSocket != NULL)
	{
		while(1)
		{
			pPacket = m_pTCPSocket->TcpRead();
			if(pPacket == NULL)				
			{
				int nError = WSAGetLastError();
				if( nError != WSAEWOULDBLOCK && nError  != ERROR_SUCCESS ) 
				{
//					if(nError == WSAECONNABORTED)
					{
						char temp[256];
						_snprintf(temp, 256, "서버 종료 에러 %d", nError);
						MessageBox(NULL, temp, "에러", MB_OK);
						return false;
					}
				}
				break;
			}

			pQueBuffer = (CHeadPacket*)m_cTCPQue.GetBuffer();
			if(pQueBuffer == NULL)
				break;

			memcpy(pQueBuffer, pPacket, HEAD_SIZE+(m_pTCPSocket->m_cGetHeadPacket.m_nBodySize));
			SecureLayer( pQueBuffer );
			m_vTCPPacketList.push_back(pQueBuffer);
		}
	}

	//UDP 패킷을 리스트에 등록
	if(m_pUDPSocket != NULL)
	{
		while(1)
		{
			pPacket = m_pUDPSocket->UdpRead();
			if(pPacket == NULL)
				break;

			pQueBuffer = (CHeadPacket*)m_cUDPQue.GetBuffer();
			if(pQueBuffer == NULL)
				break;

			memcpy(pQueBuffer, pPacket, HEAD_SIZE+(m_pUDPSocket->m_cGetHeadPacket.m_nBodySize));
			m_vUDPPacketList.push_back(pQueBuffer);
		}
	}
	return true;
}

void CPacketManager::PacketProcess()
{
	CObjRoomInfo*	pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	CHeadPacket*	pPacket;
	Function_t		FUNCTION;

	//저장된 TCP 패킷을 적용
	if(m_pTCPSocket != NULL)
	{
		for(VectorHeadPacket::iterator itr=m_vTCPPacketList.begin(); itr!=m_vTCPPacketList.end(); ++itr)
		{
			pPacket = (CHeadPacket*)(*itr);

            printf( "Receive Packet Command : %d\n", pPacket->m_nCommand );

			FUNCTION = m_mFunctionHash[pPacket->m_nCommand];
			if(FUNCTION != NULL)
				FUNCTION(pPacket);

			switch(pPacket->m_nCommand)
			{
			case CM_GOALIN_TCP:
				if(m_bIsRecording)
					m_cReplayQue.InsertReplayData(pRoom->m_fStampTime, pPacket);
				break;
			}

			//m_pTCPSocket->m_cQue.ReleaseBuffer((char*)pPacket);
			m_cTCPQue.ReleaseBuffer((char*)pPacket);
		}
		m_vTCPPacketList.clear();
	}

	//저장된 UDP 패킷을 적용
	if(m_pUDPSocket != NULL)
	{
		for(VectorHeadPacket::iterator itr=m_vUDPPacketList.begin(); itr!=m_vUDPPacketList.end(); ++itr)
		{
			pPacket = (CHeadPacket*)(*itr);

			FUNCTION = m_mFunctionHash[pPacket->m_nCommand];
			if(FUNCTION != NULL)
				FUNCTION(pPacket);

			switch(pPacket->m_nCommand)
			{
			case CM_OBJECT_ACTION:
			case CM_BALL_ACTION:
			case CM_GOAL_IN:
				if(m_bIsRecording)
					m_cReplayQue.InsertReplayData(pRoom->m_fStampTime, pPacket);
				break;
			}

			//m_pUDPSocket->m_cQue.ReleaseBuffer((char*)pPacket);
			m_cUDPQue.ReleaseBuffer((char*)pPacket);
		}
		m_vUDPPacketList.clear();
	}
}

void CPacketManager::SendTCP(char* sPacket, int nPacketSize)
{
	
	char	SECURITY_BUFFER[PACKET_SIZE];
	CHeadPacket * pPacket = (CHeadPacket*)sPacket;
	if( m_hTCPSecure.InitCheck() && // 현재 로그인되어 있는 상태인지
		pPacket->m_nCommand > CM_GAME_EXIT ) // 로그인과 종료값, 인증서버 패킷은 인코딩하지 않음
	{
		// 안전한 보안 구현을 위하여 버퍼 복사
		memcpy( SECURITY_BUFFER, pPacket, pPacket->m_nBodySize+HEAD_SIZE );
		// 암호화
		m_hTCPSecure.MakeSendPacket((CHeadPacket *)SECURITY_BUFFER);
		m_pTCPSocket->SendServer(SECURITY_BUFFER, nPacketSize);
		return;
	}
	printf( "일반전송 : %d\n",pPacket->m_nCommand );
	m_pTCPSocket->SendServer((char*)pPacket, nPacketSize);
}

void CPacketManager::SendUDP(CAddress* pTargetAddr, char* sPacket, int nBodySize)
{
	m_pUDPSocket->UdpWrite(pTargetAddr, sPacket, nBodySize);
}

void CPacketManager::SendReplay(char* sPacket)
{
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	if(m_bIsRecording)
		m_cReplayQue.InsertReplayData(pRoom->m_fStampTime, (CHeadPacket*)sPacket);
}

void CPacketManager::SendAllUDP(char* sPacket, int nBodySize)
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;

			SendUDP(&pAnyPlayer->m_cUDPAddress, sPacket, nBodySize);
		}
	}
}

void CPacketManager::SendServerUDP(char* sPacket, int nBodySize)
{
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();

	m_pUDPSocket->UdpWrite(pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort-1000, sPacket, nBodySize);
}

void CPacketManager::SendParentUDP(char* sPacket, int nBodySize)
{
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pParentPlayer	= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pRoom->m_nParentSeq);
	CHeadPacket*		pHead;

	if(pParentPlayer == NULL)	return;
	if(pRoom->IsMeParent())		return;

	if(pParentPlayer->m_bPutUDP)
		SendUDP(&pParentPlayer->m_cUDPAddress, sPacket, nBodySize);
	else //릴레이 서버 이용자
	{
		pHead = (CHeadPacket*)sPacket;
		pHead->m_nSequence = pParentPlayer->GetObjSeq();
		memcpy(sPacket, pHead, HEAD_SIZE);

		//SendServerUDP(sPacket, nBodySize);
		m_pUDPSocket->UdpWrite(pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort-1000, sPacket, nBodySize);
	}
}

void CPacketManager::SendChildUDP(char* sPacket, int nBodySize)
{
	CObjPlayerInfo*		pAnyPlayer;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);
	CHeadPacket*		pHead;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)		continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;

			if(pAnyPlayer->GetObjSeq() == pRoom->m_nParentSeq)
				SendReplay(sPacket);
			else
			{
				if(pAnyPlayer->m_bPutUDP)
					SendUDP(&pAnyPlayer->m_cUDPAddress, sPacket, nBodySize);
				else //릴레이 서버 이용자
				{
					pHead = (CHeadPacket*)sPacket;
					pHead->m_nSequence = pAnyPlayer->GetObjSeq();
					//printf( "UDP Relay Test : %d / %d \t%s\t%d\n", pHead->m_nSequence, pHead->m_nCommand, pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort );
					memcpy(sPacket, pHead, HEAD_SIZE);
					
					SendServerUDP(sPacket, nBodySize);
				}
			}
		}
	}
}

void CPacketManager::SendAngerUDP(char* sPacket, int nBodySize)
{
	CObjPlayerInfo*		pAnyPlayer;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);
	CHeadPacket*		pHead;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)		continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;

			if(pAnyPlayer->m_nTeam == PLAYER_TEAM_VIEW) //관전자에게는 분노정보를 무조건 보낸다
			{}
			else if(pAnyPlayer->IsPossibleSkillType(SKILL_ANGER)) //선수에게는 분노 스킬이 있는 사람에게만 보낸다
			{}
			else
				continue;

			if(pAnyPlayer->GetObjSeq() == pRoom->m_nParentSeq)
				SendReplay(sPacket);
			else
			{
				if(pAnyPlayer->m_bPutUDP)
					SendUDP(&pAnyPlayer->m_cUDPAddress, sPacket, nBodySize);
				else //릴레이 서버 이용자
				{
					pHead = (CHeadPacket*)sPacket;
					pHead->m_nSequence = pAnyPlayer->GetObjSeq();
					//printf( "UDP Relay Test : %d / %d \t%s\t%d\n", pHead->m_nSequence, pHead->m_nCommand, pRoom->m_cServerAddress.m_sIP, pRoom->m_cServerAddress.m_nPort );
					memcpy(sPacket, pHead, HEAD_SIZE);

					SendServerUDP(sPacket, nBodySize);
				}
			}
		}
	}
}

void CPacketManager::SendChildTCP(char* sPacket, int nBodySize)
{
	CObjPlayerInfo*		pAnyPlayer;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;
			if(pAnyPlayer->GetObjSeq() == pRoom->m_nParentSeq)
				SendReplay(sPacket);
			else
			{
				if(pAnyPlayer->m_bPutUDP)
					continue;
				else //릴레이 서버 이용자
					SendTCP(sPacket, nBodySize);
			}
		}
	}
}

bool CPacketManager::IsSendRelay()
{
	CObjPlayerInfo*		pAnyPlayer;
	CObjRoomInfo*		pRoom			= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);


	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)									continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_ROBOT)			continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)		continue;
			if(pAnyPlayer->GetKind() == PLAYER_KIND_TUTORIALROBOT)	continue;
			if(pAnyPlayer->GetObjSeq() == pRoom->m_nParentSeq)		continue;

			if(pAnyPlayer->m_bPutUDP)
				continue;
			else //릴레이 서버 이용자
				return true;
		}
	}
	return false;
}
