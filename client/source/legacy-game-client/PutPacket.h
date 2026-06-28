#ifndef __PUT_PACKET_H__
#define __PUT_PACKET_H__

#include "ComInclude.h"
#include "ComStructure.h"
#include "PacketForm.h"

void							PutCertifyLogin(char sID[], char sPass[]);
void							PutInstantLogin();
void							PutCertifyExit(int nReason);
void							PutMemberInfo();
void							PutCharacterInfo();
void							PutCreateCharacter(CCSCreateCharacter* pPacket);
void							PutDeleteCharacter(CCSDeleteCharacter* pPacket);
void							PutChoiceCharacter(int nPlayerSeq);
void							PutServerList(int nChannel);
void							PutExecuteTutorial(int nTutorial, int nOrder);
void							PutExecuteQuest(int nQuest, int nOrder);

//패킷 관련 함수
void							PutGameLogin(CAddress* pAddress);
void							PutGameExit(int nReason);
void							PutUDPConfirm();
void							PutNoticeList(int nVersion ); // 공지사항 목록 (CYG)
void							PutPlayerInfo();
void							PutSaleList(int nPeriod, int nPage);
void							PutRoomInfo();
void							PutRoomList(int nListKind, int nPage);
void							PutLobbyList(int nPage);
void							PutCreateRoom(CCSCreateRoom* pPacket);
void							PutSetRoom(CCSSetRoom* pPacket);
void							PutChoiceRoom(CCSChoiceRoom* pPacket);
void							PutQuickRoom();
void							PutLeaveRoom();
void							PutChangeJang(int nPlayerSeq);
void							PutAthleteInfo();
void							PutRobotInfo();
void							PutChangeGround(int nGroundCode);
void							PutChangeBall(int nBallCode);
void							PutForceOut(int nPlayerSeq);
void							PutInvitePlayer(int nPlayerSeq, const char sMessage[]);

void							PutGameReady(int nReady);
void							PutGameStart();
void							PutGameCount(int nCount);
void							PutGameLoad(int nStep);
void							PutGamePlay();
void							PutGameResult();
void							PutGameEnd();
void							PutChangeTeam(int nChangeTeamKind);
void							PutChangePosition(CCSChangePosition* pPacket);
void							PutChangeMent(const char sMent[]);
void							PutGrowupCharacter( int nPosition );
void							PutQuestReward( short nQuest ); // 퀘스트에 대한 보상을한다 (CYG)

void							PutShopItemList(int nType, int nBrand, int nPage);
void							PutEquipItem(const int& nItemSeq);
void							PutDivestItem(int nItemSeq);
void							PutBuyItem(CCSBuyItem* pPacket);
void							PutGiftItem(CCSGiftItem* pPacket);
void							PutExchangeItem(CCSExchangeItem* pPacket);

void							PutShopSkillList(int nType, int nPosition, int nPage);
void							PutEquipSkill(int nSkillSeq);
void							PutDivestSkill(int nSkillSeq);
void							PutBuySkill(CCSBuySkill* pPacket);

void							PutShopTrainingList(int nPage);
void							PutEquipTraining(int nTrainingSeq);
void							PutDivestTraining(int nTrainingSeq);
void							PutBuyTraining(CCSBuyTraining* pPacket);

void							PutShopCeremonyList(int nPage);
void							PutEquipCeremony(int nCeremonySeq,int nEquipKind);
void							PutDivestCeremony(int nCeremonySeq);
void							PutBuyCeremony(CCSBuyCeremony* pPacket);

void							PutQuestList(int nPage);
void							PutCreateQuest(CCSCreateQuest* pPacket);

void							PutTCPPing();
void							GetTCPPing(CHeadPacket* pPacket);
void							PutSendMessage(CCSSendMessage* pPacket);
void							PutRaiseFaculty(CFaculty* pFaculty);
void							PutChangeSetting(CCSChangeSetting* pSetting, int nPlayerSeq );


void							PutCharacterSearch(const char* sName ); // 캐릭터 찾기 (CYG)
void							PutPostItem(int nItemSeq ); // 아이템 전달 (소포시스템) (CYG)
void							PutMissionReward(CMission * pMission ); // 미션에 대한 보상을 한다. (CYG)
void							PutScheduleList( int nServerCode );// 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)
void							PutPlayerinfoEnd(); // 플레이어 정보 끝 (CYG)
void							PutAutopilotMode(char bEnable ); // 오토파일럿 모드 설정 (CYG)
void							PutCurrentWeather(); // 현재 날씨 얻어오기 (CYG)
void							PutUpdateOption(); // 아이템 옵션 갱신 (CYG)
void							PutCurrentTime(); // 현재 시간 얻어오기 (CYG)
void							PutDrawforcePlayer(char sID[] ,char sPass[]  ); // 접속중인 사용자 강제퇴장 (CYG)
void							PutTcpPing(); // TCP Ping (CYG)
void							PutUpgradeSkill(int nSkillSeq); // 스킬 강화 (CYG)
void							PutUpgradeSkill_Minus(int nSkillSeq);
void							PutSettingInfo(int    nPlayerSeq ); // 옵션 정보 (CYG)
void							PutSynchPlayer(); // 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
void							PutCardbotInfo(int nPlayerSeq ); // 카드로봇 정보 (CYG)
void							PutCardInfo(int nPlayerSeq); // 카드템 정보 (CYG)
void							PutEquipCard(int    nCardSeq,char   nEquipKind, char	nEntry); // 카드장착 (CYG)
void							PutDivestCard(int    nCardSeq, char		nEquipEntry); // 카드해제 (CYG)
void							PutMixItem1( int  nMixSeq,int  * pItemSeq,char * pItemCnt); // 아이템조합 타입 I (CYG)
void							PutMixItem2( int nItemSeq );
void							PutMixCard1(char nCardMixType,int  * nCardSeq); // 카드 조합 I (CYG)
void							PutMixCard2(int  nCardSeq,char nEmblemType,int  nEmblemCnt ); // 카드 조합 II (CYG)
void							PutBuddyInfo(int nPage); // 친구 정보 (CYG)
void							PutGoalIn();
void							PutGoalinTcp(); // 골인 TCP 서버 처리 (CYG)
void							PutSwitchValue(char nType, int nValue); // ON/OFF 값 공유 (CYG)
void							PutAddBuddy( int nPlayerSeq); // 친구추가 (CYG)
void							PutBlacklistInfo( int nPlayerSeq); // 블랙리스트 정보 (CYG)
void							PutAddBlacklist(int nPlayerSeq); // 블랙리스트 추가 (CYG)
void							PutWeeklyRecord(int nPlayerSeq ); // 주간 랭킹 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
void							PutDelBuddy(int nPlayerSeq  ); // 친구삭제 (CYG)
void							PutDelBlacklist(int nPlayerSeq  ); // 블랙리스트 삭제 (CYG)
void							PutWeeklyRanking(int nPlayerSeq ); // 주간 랭킹 정보  (CYG)
void							PutRandomshopitemList(); // 랜덤샵의 목록을 요청한다 (CYG)
void							PutBuyRandomitem(char nIdx,char nBuyKind,int  nCode,int  nPrice,int  nOptionCode[ITEM_OPTION_SIZE]); // 랜덤샾의 아이템을 구입한다. (CYG)
void							PutEnchantItem(int  nItemSeq,int  nPrice,char nEnchantType, char nBuyKind ); // 아이템을 강화한다. (CYG)
void							PutRefreshShop(char nBuyKind); // 랜덤샵을 새로고침한다. (CYG)
void							PutBuyCardbooster(int nCode,char nBuyKind,int  nPrice); // 카드부스터 구입 (CYG)
void							PutCardEntry(char nEntry ); // 카드 엔트리 세팅 (CYG)
void							PutPitIn(int  nPlayerSeq,char bInOut, char bAdminReq ); // 피트인 (CYG)
//--SOURCE_AUTO_INSERT_SCRIPT
 
//P2P 패킷 관련 함수
void							PutUDPPunching();
void							PutObjectAction();
void							PutBallAction();
void							PutObjectLocation();
void							PutGoalIn_UDP();
void							PutInputKey(int nPlayerSeq, CInputKey cInputKey);
void							PutEmotionInfo(int nEmotionCode);
void							PutAngerInfo();

#endif
