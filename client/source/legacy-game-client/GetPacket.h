#ifndef __GET_PACKET_H__
#define __GET_PACKET_H__

#include "PacketForm.h"

void GetCertifyLogin(CHeadPacket* pPacket);
void GetInstantLogin(CHeadPacket* pPacket);
void GetCertifyExit(CHeadPacket* pPacket);
void GetMemberInfo(CHeadPacket* pPacket);
void GetCharacterInfo(CHeadPacket* pPacket);
void GetCharacterEnd(CHeadPacket* pPacket);
void GetCreateCharacter(CHeadPacket* pPacket);
void GetDeleteCharacter(CHeadPacket* pPacket);
void GetChoiceCharacter(CHeadPacket* pPacket);
void GetServerList(CHeadPacket* pPacket);
void GetExecuteTutorial(CHeadPacket* pPacket);
void GetExecuteQuest(CHeadPacket* pPacket); //

void GetGameLogin(CHeadPacket* pPacket);
void GetGameExit(CHeadPacket* pPacket);
void GetUDPConfirm(CHeadPacket* pPacket);
void GetNoticeList(CHeadPacket* pPacket); // 공지사항 목록 (CYG)
void GetPlayerInfo(CHeadPacket* pPacket);
void GetSaleList(CHeadPacket* pPacket);
void GetItemInfo(CHeadPacket* pPacket);
void GetSkillInfo(CHeadPacket* pPacket);
void GetTrainingInfo(CHeadPacket* pPacket);
void GetCeremonyInfo(CHeadPacket* pPacket);
void GetQuestInfo(CHeadPacket* pPacket);
void GetRoomInfo(CHeadPacket* pPacket);
void GetRoomList(CHeadPacket* pPacket);
void GetLobbyList(CHeadPacket* pPacket);
void GetCreateRoom(CHeadPacket* pPacket);
void GetSetRoom(CHeadPacket* pPacket);
void GetChoiceRoom(CHeadPacket* pPacket);
void GetQuickRoom(CHeadPacket* pPacket);
void GetLeaveRoom(CHeadPacket* pPacket);
void GetChangeParent(CHeadPacket* pPacket);
void GetChangeJang(CHeadPacket* pPacket);
void GetAthleteInfo(CHeadPacket* pPacket);
void GetAthleteEnd(CHeadPacket* pPacket);
void GetRobotInfo(CHeadPacket* pPacket);
void GetRobotEnd(CHeadPacket* pPacket);
void GetChangeGround(CHeadPacket* pPacket);
void GetChangeBall(CHeadPacket* pPacket);
void GetForceOut(CHeadPacket* pPacket);
void GetInvitePlayer(CHeadPacket* pPacket);

void GetGameReady(CHeadPacket* pPacket);
void GetGameStart(CHeadPacket* pPacket);
void GetGameCount(CHeadPacket* pPacket);
void GetGameLoad(CHeadPacket* pPacket);
void GetGamePlay(CHeadPacket* pPacket);
void GetGameResult(CHeadPacket* pPacket);
void GetGameEnd(CHeadPacket* pPacket);
void GetLevelUp(CHeadPacket* pPacket);
void GetChangeTeam(CHeadPacket* pPacket);
void GetChangePosition(CHeadPacket* pPacket);
void GetChangeMent(CHeadPacket* pPacket);
void GetGrowupCharacter(CHeadPacket* pPacket);
void GetQuestReward(CHeadPacket* pPacket); // 퀘스트에 대한 보상을한다 (CYG)

void GetShopItemList(CHeadPacket* pPacket);
void GetUpdateItem(CHeadPacket* pPacket);
void GetEquipItem(CHeadPacket* pPacket);
void GetDivestItem(CHeadPacket* pPacket);
void GetBuyItem(CHeadPacket* pPacket);
void GetExchangeItem(CHeadPacket* pPacket);
void GetShopSkillList(CHeadPacket* pPacket);
void GetUpdateSkill(CHeadPacket* pPacket);
void GetEquipSkill(CHeadPacket* pPacket);
void GetDivestSkill(CHeadPacket* pPacket);
void GetBuySkill(CHeadPacket* pPacket);
void GetShopTrainingList(CHeadPacket* pPacket);
void GetUpdateTraining(CHeadPacket* pPacket);
void GetEquipTraining(CHeadPacket* pPacket);
void GetDivestTraining(CHeadPacket* pPacket);
void GetBuyTraining(CHeadPacket* pPacket);
void GetShopCeremonyList(CHeadPacket* pPacket);
void GetUpdateCeremony(CHeadPacket* pPacket);
void GetEquipCeremony(CHeadPacket* pPacket);
void GetDivestCeremony(CHeadPacket* pPacket);
void GetBuyCeremony(CHeadPacket* pPacket);
void GetQuestList(CHeadPacket* pPacket);
void GetCreateQuest(CHeadPacket* pPacket);
void GetUpdateQuest(CHeadPacket* pPacket);
void GetSendMessage(CHeadPacket* pPacket);
void GetRaiseFaculty(CHeadPacket* pPacket);
void GetChangeSetting(CHeadPacket* pPacket);

void GetCharacterSearch(CHeadPacket* pPacket); // 캐릭터 찾기 (CYG)
void GetPostItem(CHeadPacket* pPacket); // 아이템 전달 (소포시스템) (CYG)
void GetMissionReward(CHeadPacket* pPacket); // 미션에 대한 보상을 한다. (CYG)
void GetScheduleList(CHeadPacket* pPacket); // 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)
void GetPlayerinfoEnd(CHeadPacket* pPacket); // 플레이어 정보 끝 (CYG)
void GetAutopilotMode(CHeadPacket* pPacket); // 오토파일럿 모드 설정 (CYG)
void GetCurrentWeather(CHeadPacket* pPacket); // 현재 날씨 얻어오기 (CYG)
void GetConsumeItem(CHeadPacket* pPacket); // 소모성 아이템 사용 통지 (CYG)
void GetUpdateOption(CHeadPacket* pPacket); // 아이템 옵션 갱신 (CYG)
void GetCurrentTimePacket(CHeadPacket* pPacket); // 현재 시간 얻어오기 (CYG)
void GetDrawforcePlayer(CHeadPacket* pPacket); // 접속중인 사용자 강제퇴장 (CYG)
void GetTcpPing(CHeadPacket* pPacket); // TCP Ping (CYG)
void GetUpgradeSkill(CHeadPacket* pPacket); // 스킬 강화 (CYG)
void GetSettingInfo(CHeadPacket* pPacket); // 옵션 정보 (CYG)
void GetUpdateItemfaculty(CHeadPacket* pPacket); // 아이템 능력치 갱신 (CYG)
void GetSynchPlayer(CHeadPacket* pPacket); // 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
void GetCardbotInfo(CHeadPacket* pPacket); // 카드로봇 정보 (CYG)
void GetCardbotEnd(CHeadPacket* pPacket); // 카드로봇 정보 끝 (CYG)
void GetCardInfo(CHeadPacket* pPacket); // 카드템 정보 (CYG)
void GetEquipCard(CHeadPacket* pPacket); // 카드장착 (CYG)
void GetDivestCard(CHeadPacket* pPacket); // 카드해제 (CYG)
void GetGetCard(CHeadPacket* pPacket); // 카드획득 (CYG)
void GetMixItem1(CHeadPacket* pPacket); // 아이템조합 타입 I (CYG)
void GetMixItem2(CHeadPacket* pPacket); // 아이템조합 타입 II (CYG)
void GetMixCard1(CHeadPacket* pPacket); // 카드 조합 I (CYG)
void GetMixCard2(CHeadPacket* pPacket); // 카드 조합 II (CYG)
void GetBuddyInfo(CHeadPacket* pPacket); // 친구 정보 (CYG)
void GetGoalinTcp(CHeadPacket* pPacket); // 골인 TCP 서버 처리 (CYG)
void GetGoalinTcp_Replay(CHeadPacket* pPacket);
void GetSwitchValue(CHeadPacket* pPacket); // ON/OFF 값 공유 (CYG)
void GetAddBuddy(CHeadPacket* pPacket); // 친구추가 (CYG)
void GetBlacklistInfo(CHeadPacket* pPacket); // 블랙리스트 정보 (CYG)
void GetAddBlacklist(CHeadPacket* pPacket); // 블랙리스트 추가 (CYG)
void GetWeeklyRecord(CHeadPacket* pPacket); // 주간 랭킹 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
void GetDelBuddy(CHeadPacket* pPacket); // 친구삭제 (CYG)
void GetDelBlacklist(CHeadPacket* pPacket); // 블랙리스트 삭제 (CYG)
void GetWeeklyRanking(CHeadPacket* pPacket); // 주간 랭킹 정보  (CYG)
void GetGiftItem(CHeadPacket* pPacket); // 아이템 선물 (CYG)
void GetRandomshopitemList(CHeadPacket* pPacket); // 랜덤샵의 목록을 요청한다 (CYG)
void GetBuyRandomitem(CHeadPacket* pPacket); // 랜덤샾의 아이템을 구입한다. (CYG)
void GetEnchantItem(CHeadPacket* pPacket); // 아이템을 강화한다. (CYG)
void GetRefreshShop(CHeadPacket* pPacket); // 랜덤샵을 새로고침한다. (CYG)
void GetBuyCardbooster(CHeadPacket* pPacket); // 카드부스터 구입 (CYG)
void GetCardEntry(CHeadPacket* pPacket); // 카드 엔트리 세팅 (CYG)
void GetPitIn(CHeadPacket* pPacket); // 피트인 (CYG)
//--SOURCE_AUTO_INSERT_SCRIPT

//P2P 패킷 관련 함수
void							GetObjectAction(CHeadPacket* pPacket);
void							GetObjectAction_Replay(CHeadPacket* pPacket);
void							GetBallAction(CHeadPacket* pPacket);
void							GetBallAction_Replay(CHeadPacket* pPacket, float fTime);
void							GetObjectLocation(CHeadPacket* pPacket);
void							GetGoalIn(CHeadPacket* pPacket);
void							GetGoalIn_Replay(CHeadPacket* pPacket);
void							GetInputKey(CHeadPacket* pPacket);
void							GetUDPPing(CHeadPacket * pPacket);
void							GetEmotionInfo(CHeadPacket* pPacket);
void							GetAngerInfo(CHeadPacket* pPacket);

#endif
