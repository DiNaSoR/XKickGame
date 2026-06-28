// 공통 프로토콜 정의서 -
// 특정 환경에만 적용되는 코드를 삽입하지 말 것!

#ifndef __PACKET_FORM_H__
#define __PACKET_FORM_H__


// 클라이언트 환경 정의(CYG)
#ifndef USE_ECLIPSE // 이클립스 환경에서 제외되도록 설정 (이클립스 사용시 정의하고 사용할 것)(CYG)
	#ifdef _WIN32
		#define COMPILE_FOR_CLIENT
	#endif
#endif

// 환경별 공통 INCLUDE 정의 (CYG)
#ifdef	COMPILE_FOR_CLIENT		// CLIENT
		#include "ComInclude.h"
		#include "ComStructure.h"
#else							// SERVER
	#include "../Define.h"
	#include "../Struct.h"
#endif

#ifdef USE_ECLIPSE  // WIN32
	#include "../Define.h"
	#include "../Struct.h"
#endif

// 다중 패킷 전송시 패킷의 위치 정의 (CYG)
enum PACKET_POSITION
{
	PACKET_POSITION_START		= 0,	//패킷의 시작
	PACKET_POSITION_HALF,				//패킷의 가운데
	PACKET_POSITION_FINISH				//패킷의 종료
};

#ifdef	COMPILE_FOR_CLIENT
#pragma pack(push,1)
#else
#pragma pack(push,1)
#endif

// 프로토콜 생성자 매크로 (CYG)
#define PACKET_CONSTRUCT( CLASS_TYPE, COMMAND_TYPE )  CLASS_TYPE() : CHeadPacket(COMMAND_TYPE)	{  m_nBodySize = sizeof(CLASS_TYPE)-HEAD_SIZE; }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// COMMAND
//////////////////////////////////////////////////////////////////////////
#define CM_CERTIFY_LOGIN		1000
#define CM_INSTANT_LOGIN		1001
#define CM_CERTIFY_EXIT			1002
#define CM_MEMBER_INFO			1100
#define CM_CHARACTER_INFO		1200
#define CM_CHARACTER_END		1201
#define CM_CREATE_CHARACTER		1202
#define CM_DELETE_CHARACTER		1203
#define CM_CHOICE_CHARACTER		1204
#define CM_SERVER_LIST			1300
#define CM_EXECUTE_TUTORIAL	1400
#define CM_GAME_LOGIN			2000
#define CM_GAME_EXIT			2001
#define CM_UDP_CONFIRM			2002
#define CM_NOTICE_LIST			2003 // 공지사항 목록 (CYG)


#define CM_PLAYER_INFO			2100
#define CM_ITEM_INFO			2101
#define CM_SKILL_INFO			2102
#define CM_TRAINING_INFO		2103
#define CM_CEREMONY_INFO		2104
#define CM_QUEST_INFO			2105

#define CM_SALE_LIST			2110
#define CM_GAMEITEM_INFO		2111




#define CM_ROOM_INFO			2200
#define CM_ROOM_LIST			2201
#define CM_LOBBY_LIST			2202
#define CM_CREATE_ROOM			2203
#define CM_SET_ROOM			2204
#define CM_CHOICE_ROOM			2205
#define CM_QUICK_ROOM			2206
#define CM_LEAVE_ROOM			2300
#define CM_CHANGE_PARENT		2301
#define CM_CHANGE_JANG			2302
#define CM_ATHLETE_INFO		2303
#define CM_ATHLETE_END			2304
#define CM_ROBOT_INFO			2305
#define CM_ROBOT_END			2306
#define CM_CHANGE_GROUND		2307
#define CM_CHANGE_BALL			2308
#define CM_FORCE_OUT			2309
#define CM_INVITE_PLAYER		2310
#define CM_CARDBOT_INFO		2311
#define CM_CARDBOT_END			2312

#define CM_GAME_READY			2400
#define CM_GAME_START			2401
#define CM_GAME_COUNT			2402
#define CM_GAME_LOAD			2403
#define CM_GAME_PLAY			2404
#define CM_GAME_RESULT			2405
#define CM_GAME_END			2406
#define CM_LEVEL_UP			2407

#define CM_CHANGE_TEAM			2501
#define CM_CHANGE_POSITION		2502

#define CM_CHANGE_MENT			2601
#define CM_GROWUP_CHARACTER	2701
#define CM_QUEST_REWARD		2702 // 퀘스트에 대한 보상을한다 (CYG)
#define CM_EXECUTE_QUEST		2704 // 퀘스트 실행


#define CM_SHOPITEM_LIST		3100
#define CM_UPDATE_ITEM			3101
#define CM_EQUIP_ITEM			3102
#define CM_DIVEST_ITEM			3103
#define CM_BUY_ITEM			3104
#define CM_GIFT_ITEM			3105
#define CM_EXCHANGE_ITEM		3106


#define CM_BUY_RANDOMITEM		3110 // 랜덤샾의 아이템을 구입한다. (CYG)
#define CM_RANDOMSHOPITEM_LIST	3111 // 랜덤샵 목록을 요청한다. (CYG)
#define CM_ENCHANT_ITEM		3112 // 아이템을 강화한다. (CYG)
#define CM_REFRESH_SHOP		3113 // 랜덤샵을 새로고침한다. (CYG)




#define CM_SHOPSKILL_LIST		3200
#define CM_UPDATE_SKILL		3201
#define CM_EQUIP_SKILL			3202
#define CM_DIVEST_SKILL		3203
#define CM_BUY_SKILL			3204

#define CM_SHOPTRAINING_LIST	3300
#define CM_UPDATE_TRAINING		3301
#define CM_BUY_TRAINING		3304

#define CM_SHOPCEREMONY_LIST	3400
#define CM_UPDATE_CEREMONY		3401
#define CM_EQUIP_CEREMONY		3402
#define CM_DIVEST_CEREMONY		3403
#define CM_BUY_CEREMONY		3404

#define CM_QUEST_LIST			3500
#define CM_UPDATE_QUEST		3501
#define CM_CREATE_QUEST		3502

#define CM_TCP_PING			5000
#define CM_SEND_MESSAGE		5001
#define CM_RAISE_FACULTY		5002
#define CM_CHANGE_SETTING		5003

#define CM_UDP_PUNCHING		9000
#define CM_UDP_PING			9001
#define CM_OBJECT_ACTION		9002
#define CM_BALL_ACTION			9003
#define CM_OBJECT_LOCATION		9004
#define CM_GOAL_IN				9005
#define CM_INPUT_KEY			9006
#define CM_EMOTION_INFO		9007
#define CM_ANGER_INFO			9008

#define CM_CHARACTER_SEARCH	1205 // 캐릭터 찾기 (CYG)
#define CM_POST_ITEM			3107 // 아이템 전달 (소포시스템) (CYG)
#define CM_MISSION_REWARD		2703 // 미션에 대한 보상을 한다. (CYG)
#define CM_SCHEDULE_LIST		2004 // 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)
#define CM_PLAYERINFO_END		2109 // 플레이어 정보 끝 (CYG)
#define CM_AUTOPILOT_MODE		2408 // 오토파일럿 모드 설정 (CYG)
#define CM_CURRENT_WEATHER		2005 // 현재 날씨 얻어오기 (CYG)
#define CM_CONSUME_ITEM		2409 // 소모성 아이템 사용 통지 (CYG)
#define CM_UPDATE_ITEMFACULTY	3108 // 아이템 능력치 갱신 (CYG)
#define CM_DRAWFORCE_PLAYER	1003 // 접속중인 사용자 강제퇴장 (CYG)
#define CM_CURRENT_TIME		2006 // 현재 시간 얻어오기 (CYG)
#define CM_UPGRADE_SKILL		3205 // 스킬 강화 (CYG)
#define CM_SETTING_INFO		5004 // 옵션 정보 (CYG)
#define CM_SYNCH_PLAYER		2410 // 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
#define CM_CARDBOT_END			2312 // 카드로봇 정보 끝 (CYG)

#define CM_CARD_INFO			2106 // 카드템 정보 (CYG)

#define CM_EQUIP_CARD			3601 // 카드장착 (CYG)
#define CM_DIVEST_CARD			3602 // 카드해제 (CYG)
#define CM_GET_CARD			3603 // 카드획득 (CYG)
#define CM_BUY_CARDBOOSTER		3604 // 카드부스터 구입 (CYG)
#define CM_CARD_ENTRY			3605 // 카드 엔트리 세팅 (CYG)





#define CM_MIX_ITEM1			3701 // 아이템조합 타입 I (CYG)
#define CM_MIX_ITEM2			3702 // 아이템조합 타입 II (CYG)
#define CM_MIX_CARD1			3711 // 카드 조합 I (CYG)
#define CM_MIX_CARD2			3712 // 카드 조합 II (CYG)
#define CM_GOALIN_TCP			2411 // 골인 TCP 서버 처리 (CYG)
#define CM_SWITCH_VALUE		2412 // ON/OFF 값 공유 (CYG)
#define CM_PIT_IN				2413 // 방에서 갈아입으러 갔을 때 (CYG)

#define CM_BLACKLIST_INFO		2313 // 블랙리스트 정보 (CYG)
#define CM_ADD_BLACKLIST		2314 // 블랙리스트 추가 (CYG)
#define CM_DEL_BLACKLIST		2315 // 블랙리스트 삭제 (CYG)

#define CM_BUDDY_INFO			2316 // 친구 정보 (CYG)
#define CM_ADD_BUDDY			2317 // 친구 추가 (CYG)
#define CM_DEL_BUDDY			2318 // 친구 삭제 (CYG)

#define CM_WEEKLY_RECORD		2319 // 주간 기록 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
#define CM_WEEKLY_RANKING		2320 // 주간 랭킹 정보  (CYG)



//--SOURCE_AUTO_INSERT_SCRIPT


//////////////////////////////////////////////////////////////////////////
// Server To Server 프로토콜
//////////////////////////////////////////////////////////////////////////
#define STS_COMMAND_LIMIT 		200 // 서버간 통신은 200번대 이하로 제한한다.
#define CM_STS_LOGIN			101
#define CM_STS_DRAWFORCE		102 // 접속중인 사용자 강제퇴장 (CYG)
#define CM_UPDATE_WEATHER		103 // 전서버 날씨 갱신 (CYG)
#define CM_UPDATE_SCHEDULE		104 // 전서버 스케쥴 갱신 (CYG)
#define CM_UPDATE_NOTICE		105 // 전 서버 공지 갱신 (CYG)
#define CM_SEND_BROADCAST		106 // 전 서버 공지 채팅 전송 (CYG)

class CPPGoalIn : public CHeadPacket
{ public:
	char				m_nGoalType;
	char				m_nGoalTeam;
	char				m_nShootKind;
	int					m_nCeremonyCode;
	int					m_nGoalSeq;
	int					m_nAssistSeq;
	char				m_nGoalerScore;
	char				m_nAssisterScore;
	float				m_fCurrentTime;
	char				m_nHomeScore;
	char				m_nAwayScore;
};

//////////////////////////////////////////////////////////////////////////
// STRUCTURE , DEFINE (필요하다면 추후에 Comdefine / Structure 로 이동 )
//////////////////////////////////////////////////////////////////////////
// 블랙리스트 정의
#define MAX_BLACK_LIST 10	// 페이지당 최대수
class CBlacklistInfo
{
public:
	int			m_nSeq;
	int			m_nPlayerSeq;
	short		m_nLevel;
	char		m_nPosition;
	char		m_sName[PLAYER_NAME_SIZE];
};


class CWeeklyRecord
{
public:
	int			m_nRecord[DAY_SUN];

public:
	CWeeklyRecord		operator+(const CWeeklyRecord& cRecord)
	{
		for(int i=0;i<DAY_SUN;++i) m_nRecord[i] += cRecord.m_nRecord[i];
		return *this;
	}
};

class CWeeklyRanking
{
public:
	int			m_nRanking[DAY_SUN];

public:
	CWeeklyRanking		operator+(const CWeeklyRanking& cRanking )
	{
		for(int i=0;i<DAY_SUN;++i) m_nRanking[i] += cRanking.m_nRanking[i];
		return *this;
	}
};



//////////////////////////////////////////////////////////////////////////
// CERTIFY PROTOCOL
//////////////////////////////////////////////////////////////////////////
class CCSCertifyLogin : public CHeadPacket
{ public:
	char			m_sID[ID_NAME_SIZE];
	char			m_sPass[LOGIN_PASS_SIZE];
	PACKET_CONSTRUCT( CCSCertifyLogin, CM_CERTIFY_LOGIN );
};

class CSCCertifyLogin : public CHeadPacket
{ public:
	char			m_nResponse;
	int				m_nMemberSeq;
	PACKET_CONSTRUCT( CSCCertifyLogin, CM_CERTIFY_LOGIN );
};
//////////////////////////////////////////////////////////////////////////
class CCSInstantLogin : public CHeadPacket
{ public:
	int				m_nMemberSeq;
	PACKET_CONSTRUCT( CCSInstantLogin, CM_INSTANT_LOGIN );
};

class CSCInstantLogin : public CHeadPacket
{ public:
	char			m_nResponse;
	PACKET_CONSTRUCT( CSCInstantLogin, CM_INSTANT_LOGIN );
};
//////////////////////////////////////////////////////////////////////////
class CCSCertifyExit : public CHeadPacket
{ public:
	char			m_nReason;
	PACKET_CONSTRUCT( CCSCertifyExit, CM_CERTIFY_EXIT );
};

class CSCCertifyExit : public CHeadPacket
{ public:
	char			m_nResponse;
	char			m_nReason;
	PACKET_CONSTRUCT( CSCCertifyExit, CM_CERTIFY_EXIT );
};
//////////////////////////////////////////////////////////////////////////
class CCSMemberInfo : public CHeadPacket
{ public:
	int				m_nMemberSeq;
	PACKET_CONSTRUCT( CCSMemberInfo, CM_MEMBER_INFO );
};

class CSCMemberInfo : public CHeadPacket
{ public:
	char			m_nResponse;
	int				m_nLastSeq;
	char			m_nCount;
	char			m_nTutorial;
	char			m_nQuest;
	CSetting		m_cSetting;
	CMoney			m_cMoney;
	int				m_nLoginDate;
	int				m_nDeleteDate;
	PACKET_CONSTRUCT( CSCMemberInfo, CM_MEMBER_INFO );
};

//////////////////////////////////////////////////////////////////////////
class CCharacterInfo
{ public:
	int				m_nPlayerSeq;
	short			m_nOrder;
	char			m_nPosition;
	char			m_nCondition;
	char			m_nAlias;
	char			m_sName[PLAYER_NAME_SIZE];
	CLevel			m_cLevel;
	CShape			m_cShape;
	int				m_nEquipWear[MAX_EQUIP];
	int				m_nHomeWear[4];
	int				m_nAwayWear[4];
};

class CCSCharacterInfo : public CHeadPacket
{ public:
	int				m_nMemberSeq;
	PACKET_CONSTRUCT( CCSCharacterInfo, CM_CHARACTER_INFO );
};

class CSCCharacterInfo : public CHeadPacket
{ public:
	char			m_nResponse;
	CCharacterInfo	m_cCharacterInfo;
	PACKET_CONSTRUCT( CSCCharacterInfo, CM_CHARACTER_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CCSCharacterEnd : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSCharacterEnd, CM_CHARACTER_END );
};

class CSCCharacterEnd : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCCharacterEnd, CM_CHARACTER_END );
};
//////////////////////////////////////////////////////////////////////////
class CCSCreateCharacter : public CHeadPacket
{ public:
	char				m_sName[PLAYER_NAME_SIZE];
	CShape				m_cShape;
	int					m_nEquip[5];
	PACKET_CONSTRUCT( CCSCreateCharacter, CM_CREATE_CHARACTER );
};

class CSCCreateCharacter : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCCreateCharacter, CM_CREATE_CHARACTER );
};
//////////////////////////////////////////////////////////////////////////
class CCSDeleteCharacter : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	char				m_sJumin[20];
	PACKET_CONSTRUCT( CCSDeleteCharacter, CM_DELETE_CHARACTER );
};

class CSCDeleteCharacter : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nPlayerSeq;
	int					m_nDeleteDate;
	PACKET_CONSTRUCT( CSCDeleteCharacter, CM_DELETE_CHARACTER );
};
//////////////////////////////////////////////////////////////////////////
class CCSChoiceCharacter : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSChoiceCharacter, CM_CHOICE_CHARACTER );
};

class CSCChoiceCharacter : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CSCChoiceCharacter, CM_CHOICE_CHARACTER );
};
//////////////////////////////////////////////////////////////////////////
class CServerData
{ public:
	char				m_nState;
	int					m_nServerCode;
	char				m_nServerMatch;
	char				m_sTitle[SERVER_NAME_SIZE];
	short				m_nMax;
	short				m_nCurrent;
	CAddress			m_cAddress;
};

class CCSServerList : public CHeadPacket
{ public:
	char				m_nChannel;
	PACKET_CONSTRUCT( CCSServerList, CM_SERVER_LIST );
};

class CSCServerList : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nChannel;
	CServerData			m_cServerData[LIST10_SIZE];
	PACKET_CONSTRUCT( CSCServerList, CM_SERVER_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSExecuteTutorial : public CHeadPacket
{ public:
	char				m_nTutorial;
	char				m_nOrder;
	PACKET_CONSTRUCT( CCSExecuteTutorial, CM_EXECUTE_TUTORIAL );
};

class CSCExecuteTutorial : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nTutorial;
	PACKET_CONSTRUCT( CSCExecuteTutorial, CM_EXECUTE_TUTORIAL );
};
//////////////////////////////////////////////////////////////////////////
class CCSExecuteQuest : public CHeadPacket
{ public:
	char				m_nQuest;
	char				m_nOrder;
	PACKET_CONSTRUCT( CCSExecuteQuest, CM_EXECUTE_QUEST );
};

class CSCExecuteQuest : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nQuest;
	CMoney				m_cMoney;
	PACKET_CONSTRUCT( CSCExecuteQuest, CM_EXECUTE_QUEST );
};
//////////////////////////////////////////////////////////////////////////
// GAME PROTOCOL
//////////////////////////////////////////////////////////////////////////
class CCSGameLogin : public CHeadPacket
{ public:
	int				m_nMemberSeq;
	int				m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSGameLogin, CM_GAME_LOGIN );
};

class CSCGameLogin : public CHeadPacket
{ public:
	char			m_nResponse;
	PACKET_CONSTRUCT( CSCGameLogin, CM_GAME_LOGIN );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameExit : public CHeadPacket
{ public:
	char			m_nReason;
	PACKET_CONSTRUCT( CCSGameExit, CM_GAME_EXIT );
};

class CSCGameExit : public CHeadPacket
{ public:
	char			m_nResponse;
	char                    m_nReason; // 종료 이유
	PACKET_CONSTRUCT( CSCGameExit, CM_GAME_EXIT );
};
//////////////////////////////////////////////////////////////////////////
class CCSUDPConfirm : public CHeadPacket
{ public:
	int				m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUDPConfirm, CM_UDP_CONFIRM );
};

class CSCUDPConfirm : public CHeadPacket
{ public:
	char			m_nResponse;
	PACKET_CONSTRUCT( CSCUDPConfirm, CM_UDP_CONFIRM );
};
//////////////////////////////////////////////////////////////////////////
// 공지사항 목록 (CYG)
class CNoticeData
{ public:
	int					m_nNoticeSeq;
	char				m_strText[TIP_SIZE];
};

class CCSNoticeList : public CHeadPacket
{ public:
	int m_nVersion; // 공지사항 버전
	PACKET_CONSTRUCT( CCSNoticeList, CM_NOTICE_LIST );
};

class CSCNoticeList : public CHeadPacket
{ public:
	char m_nResponse;
	int	 m_nVersion;
	int  m_nCount;
	CNoticeData m_cNoticeList[LIST5_SIZE];
	PACKET_CONSTRUCT( CSCNoticeList, CM_NOTICE_LIST );
};

//////////////////////////////////////////////////////////////////////////
#ifdef	COMPILE_FOR_CLIENT1

class CPlayerInfo
{ public:
	int					m_nPlayerSeq;
	char				m_sName[PLAYER_NAME_SIZE];
	char				m_sMent[PLAYER_MENT_SIZE];
	CLevel				m_cLevel;
	CFaculty			m_cBaseFaculty;
	CFaculty			m_cRaiseFaculty;
	CFaculty			m_cItemFaculty;
	CFaculty			m_cTrainingFaculty;
	CRecord				m_cTotalRecord;
	CRecord				m_cQuarterRecord;
	CRanking			m_cTotalRanking;
	CRanking			m_cQuarterRanking;
};
#else

class CPlayerInfo
{ public:
	int					m_nPlayerSeq;
	char				m_sName[PLAYER_NAME_SIZE];
	char				m_sMent[PLAYER_MENT_SIZE];
	CLevel				m_cLevel;
	CFaculty			m_cBaseFaculty;
	CFaculty			m_cRaiseFaculty;
	CFaculty			m_cTrainingFaculty;
	CRecord				m_cTotalRecord;
	CRecord				m_cQuarterRecord;
	CRanking			m_cTotalRanking;
	CRanking			m_cQuarterRanking;
	CItemFacultyInfo	m_cItemFaculty; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
};

#endif

class CCSPlayerInfo: public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSPlayerInfo, CM_PLAYER_INFO );
};

class CSCPlayerInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	CPlayerInfo			m_cPlayerInfo;  // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCPlayerInfo, CM_PLAYER_INFO );
};

//////////////////////////////////////////////////////////////////////////
class CCardInfo
{ public:
	int 				m_nPlayerSeq;
	int					m_nCardSeq;
	char				m_nEquipKind[3];
	char				m_nState;
	char				m_nLevel;
	char				m_nRank;
	char				m_nType;
	char				m_nPosition;
	char				m_nSkill;
	char				m_nTierd;
	int					m_nCostume;
	char				m_nEnchant;

};

class CCardBotInfo
{ public:
	char				m_nTeam;
	char				m_nSeat;
	CCardInfo 			m_cCard;
};
// 카드로봇 정보 (CYG)
class CCSCardbotInfo : public CHeadPacket
{ public:
	int m_nPlayerSeq; // 카드소유자 Seq
	PACKET_CONSTRUCT( CCSCardbotInfo,  CM_CARDBOT_INFO );
};

class CSCCardbotInfo : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	CCardBotInfo m_cCardBotInfo; // 카드봇 정보
	PACKET_CONSTRUCT( CSCCardbotInfo ,  CM_CARDBOT_INFO );
};
//////////////////////////////////////////////////////////////////////////
// 카드로봇 정보 끝 (CYG)
class CCSCardbotEnd : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSCardbotEnd,  CM_CARDBOT_END );
};

class CSCCardbotEnd : public CHeadPacket
{ public:
	char m_nResponse; // 결과값

	PACKET_CONSTRUCT( CSCCardbotEnd ,  CM_CARDBOT_END );
};
//////////////////////////////////////////////////////////////////////////
class CGameItem
{ public:
	int					m_nItemSeq;
	int					m_nCode;
	int					m_nClass;
	int					m_nPrice;
	char				m_nLevel;
	char				m_nGrade;
	char				m_nEquipKind;
	short				m_nAmount;
	int					m_nOptionCode[ITEM_OPTION_SIZE];
};

//////////////////////////////////////////////////////////////////////////


class CItemInfo
{ public:
	int					m_nItemSeq;
	int					m_nCode;
//	char				m_nLevel;
	char				m_nEquipKind;
	short				m_nAmount;
	int					m_nOptionCode[ITEM_OPTION_SIZE];
};


class CItemCountInfo
{ public:
	int					m_nItemSeq;
	short				m_nAmount;
};



class CCSItemInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSItemInfo, CM_ITEM_INFO );
};

class CSCItemInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	CGameItem			m_cItemInfo[MAX_ITEM_LIST];  // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCItemInfo, CM_ITEM_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CCSSkillInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSSkillInfo, CM_SKILL_INFO );
};

class CSCSkillInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	CSkillInfo			m_cSkillInfo[MAX_SKILL_LIST]; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCSkillInfo, CM_SKILL_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CCSTrainingInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSTrainingInfo, CM_TRAINING_INFO );
};

class CSCTrainingInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	CTrainingInfo		m_cTrainingInfo[MAX_TRAINING_LIST]; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCTrainingInfo, CM_TRAINING_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CCSCeremonyInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSCeremonyInfo, CM_CEREMONY_INFO );
};

class CSCCeremonyInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	CCeremonyInfo		m_cCeremonyInfo[MAX_CEREMONEY_LIST];// 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCCeremonyInfo, CM_CEREMONY_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CQuestInfo
{ public:
	int					m_nQuestSeq;
	int					m_nCode;
	short				m_nAmount;
	int					m_nPlayDate;
};

class CCSQuestInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSQuestInfo, CM_QUEST_INFO );
};

class CSCQuestInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	CQuestInfo			m_cQuestInfo[MAX_QUEST_LIST]; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCQuestInfo, CM_QUEST_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CSaleList
{ public:
	char				m_nObjectKind;
	int					m_nCode;
	char				m_nSaleKind;
	int					m_nPrice;
	short				m_nAmount;
	int					m_nSaleDate;
};

class CCSSaleList : public CHeadPacket
{ public:
	char				m_nPeriod;
	char				m_nPage;
	PACKET_CONSTRUCT( CCSSaleList, CM_SALE_LIST );
};

class CSCSaleList : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nPeriod;
	char				m_nPage;
	char				m_nTotalPage;
	CSaleList			m_cSaleList[LIST10_SIZE];
	PACKET_CONSTRUCT( CSCSaleList, CM_SALE_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CRoomInfo
{ public:
	char				m_nState;
	char				m_nMode;
	char				m_nCource;
	int					m_nRoomSeq;
	char				m_nRoomJangTeam;
	int					m_nHomeJangSeq;
	int					m_nAwayJangSeq;

	char				m_sTitle[TITLE_NAME_SIZE];
	char				m_sPass[5];
	int					m_nQuestCode;
	char 				m_nGroundCode;
	char 				m_nBallCode;
	char				m_nTimeCode;
	char				m_nWeatherCode;
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	short				m_nPointCode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char				m_nAttackTeam;
	char				m_nMaxCount;

	char 				m_nCheckClub;
	char 				m_nCheckTime;
	char 				m_nCheckWeather;
	char 				m_nCheckView;
	char 				m_nCheckViewChat;
	CReserveSeat		m_cHomeSeat;
	CReserveSeat		m_cAwaySeat;
	CReserveSeat		m_cViewSeat;
};

class CCSRoomInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	short				m_nRoomSeq;
	PACKET_CONSTRUCT( CCSRoomInfo, CM_ROOM_INFO );
};

class CSCRoomInfo: public CHeadPacket
{ public:
	char				m_nResponse;
	CRoomInfo			m_cRoomInfo;
	PACKET_CONSTRUCT( CSCRoomInfo, CM_ROOM_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CRoomData
{ public:
	char				m_nState;
	char				m_nMode;
	char				m_nCource;
	int					m_nRoomSeq;

	char				m_sTitle[TITLE_NAME_SIZE];
	char 				m_nScaleCode;
	char				m_nAICode;
	short				m_nPointCode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char 				m_nCheckClub;
	char 				m_nCheckView;
	char				m_nAthleteCount;
	char				m_nMaxCount;
	char				m_nViewCount;
	CReserveSeat		m_cHomeSeat;
	CReserveSeat		m_cAwaySeat;
};

class CCSRoomList : public CHeadPacket
{ public:
	char				m_nListKind;
	char				m_nPage;
	PACKET_CONSTRUCT( CCSRoomList, CM_ROOM_LIST );
};

class CSCRoomList: public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nPage;
	CRoomData			m_cRoomData[LIST5_SIZE];
	PACKET_CONSTRUCT( CSCRoomList, CM_ROOM_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CLobbyData
{ public:
	char				m_nState;
	int					m_nPlayerSeq;
	char				m_nPosition;
	char				m_nLevel;
	char				m_sName[PLAYER_NAME_SIZE];
	char				m_sMent[PLAYER_MENT_SIZE];
};

class CCSLobbyList : public CHeadPacket
{ public:
	char				m_nPage;
	PACKET_CONSTRUCT( CCSLobbyList, CM_LOBBY_LIST );
};

class CSCLobbyList : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nPage;
	CLobbyData			m_cLobbyData[LIST10_SIZE];
	PACKET_CONSTRUCT( CSCLobbyList, CM_LOBBY_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSCreateRoom : public CHeadPacket
{ public:
	char				m_nState;
	char				m_nMode;
	char				m_sTitle[TITLE_NAME_SIZE];
	char				m_sPass[5];
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	short				m_nPointCode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char 				m_nCheckClub;
	char 				m_nCheckTime;
	char 				m_nCheckWeather;
	char 				m_nCheckView;
	char 				m_nCheckViewChat;
	char				m_nMaxCount;
	char				m_nHomePosition[TEAM_SIZE];
	char				m_nAwayPosition[TEAM_SIZE];
	PACKET_CONSTRUCT( CCSCreateRoom, CM_CREATE_ROOM );
};

class CSCCreateRoom : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCCreateRoom, CM_CREATE_ROOM );
};
//////////////////////////////////////////////////////////////////////////
class CCSSetRoom : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	char				m_nState;
	char				m_nMode;
	char				m_sTitle[TITLE_NAME_SIZE];
	char				m_sPass[5];
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	short				m_nPointCode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char 				m_nCheckClub;
	char 				m_nCheckTime;
	char 				m_nCheckWeather;
	char 				m_nCheckView;
	char 				m_nCheckViewChat;
	char				m_nMaxCount;
	PACKET_CONSTRUCT( CCSSetRoom, CM_SET_ROOM );
};

class CSCSetRoom : public CHeadPacket
{ public:
	char				m_nResponse;
	/*
	char				m_nState;
	char				m_nMode;
	char				m_sTitle[TITLE_NAME_SIZE];
	char				m_sPass[5];
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	short				m_nPointCode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char 				m_nCheckClub;
	char 				m_nCheckTime;
	char 				m_nCheckWeather;
	char 				m_nCheckView;
	char 				m_nCheckViewChat;
	char				m_nMaxCount;
	*/
	PACKET_CONSTRUCT( CSCSetRoom, CM_SET_ROOM );
};
//////////////////////////////////////////////////////////////////////////
class CCSChoiceRoom : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	char				m_nType;
	char				m_sPass[5];
	PACKET_CONSTRUCT( CCSChoiceRoom, CM_CHOICE_ROOM );
};

class CSCChoiceRoom: public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nRoomSeq;
	char				m_nTeam;
	PACKET_CONSTRUCT( CSCChoiceRoom, CM_CHOICE_ROOM );
};
//////////////////////////////////////////////////////////////////////////
class CCSQuickRoom : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSQuickRoom, CM_QUICK_ROOM );
};

class CSCQuickRoom : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCQuickRoom, CM_QUICK_ROOM );
};
//////////////////////////////////////////////////////////////////////////
class CCSLeaveRoom : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	PACKET_CONSTRUCT( CCSLeaveRoom, CM_LEAVE_ROOM );
};

class CSCLeaveRoom : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nLeavePlayerSeq;
	char				m_nLeaveTeam;
	PACKET_CONSTRUCT( CSCLeaveRoom, CM_LEAVE_ROOM );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeParent : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSChangeParent, CM_CHANGE_PARENT );
};

class CSCChangeParent : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nParentSeq;
	CAddress			m_cParentAddress;
	PACKET_CONSTRUCT( CSCChangeParent, CM_CHANGE_PARENT );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeJang : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSChangeJang, CM_CHANGE_JANG );
};

class CSCChangeJang : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nRoomJangTeam;
	int					m_nHomeJangSeq;
	int					m_nAwayJangSeq;
	PACKET_CONSTRUCT( CSCChangeJang, CM_CHANGE_JANG );
};
//////////////////////////////////////////////////////////////////////////
class CAthleteInfo
{ public:
	int					m_nPlayerSeq;
	char				m_nPosition;
	char				m_nTeam;
	char				m_nSeat;
	char				m_sName[PLAYER_NAME_SIZE];
	char				m_sMent[PLAYER_MENT_SIZE]; //멘트 추가 (CYG)
	CLevel				m_cLevel;
	CShape				m_cShape;
	CAddress			m_cAddress;
	CFaculty			m_cBaseFaculty;
	CFaculty			m_cRaiseFaculty;
	CFaculty			m_cTrainingFaculty;
	int					m_nEquipWear[MAX_EQUIP];
	int					m_nHomeWear[4];
	int					m_nAwayWear[4];
	CSkillInfo			m_cSkillInfo[MAX_SKILL];
	CCeremonyInfo		m_cCeremonyInfo[MAX_CEREMONY];
	CItemFacultyInfo	m_cItemFaculty; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
};

class CCSAthleteInfo : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSAthleteInfo, CM_ATHLETE_INFO );
};

class CSCAthleteInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	CAthleteInfo		m_cAthleteInfo;  // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCAthleteInfo, CM_ATHLETE_INFO );

};
//////////////////////////////////////////////////////////////////////////
class CCSAthleteEnd : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSAthleteEnd, CM_ATHLETE_END );
};

class CSCAthleteEnd : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCAthleteEnd, CM_ATHLETE_END );
};
//////////////////////////////////////////////////////////////////////////
class CRobotInfo
{ public:
	int					m_nRobotSeq;
	char				m_nTeam;
	char				m_nSeat;
	char				m_nLevel;
	char				m_nPosition;
	int					m_nCostume;
};

class CCSRobotInfo : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	PACKET_CONSTRUCT( CCSRobotInfo, CM_ROBOT_INFO );
};

class CSCRobotInfo : public CHeadPacket
{ public:
	char				m_nResponse;
	CRobotInfo			m_cRobotInfo;
	PACKET_CONSTRUCT( CSCRobotInfo, CM_ROBOT_INFO );
};
//////////////////////////////////////////////////////////////////////////
class CCSRobotEnd : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSRobotEnd, CM_ROBOT_END );
};

class CSCRobotEnd : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCRobotEnd, CM_ROBOT_END );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeGround : public CHeadPacket
{ public:
	int					m_nGroundCode;
	PACKET_CONSTRUCT( CCSChangeGround, CM_CHANGE_GROUND );
};

class CSCChangeGround : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nGroundCode;
	PACKET_CONSTRUCT( CSCChangeGround, CM_CHANGE_GROUND );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeBall : public CHeadPacket
{ public:
	int					m_nBallCode;
	PACKET_CONSTRUCT( CCSChangeBall, CM_CHANGE_BALL );
};

class CSCChangeBall : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nBallCode;
	PACKET_CONSTRUCT( CSCChangeBall, CM_CHANGE_BALL );
};
//////////////////////////////////////////////////////////////////////////
class CCSForceOut : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSForceOut, CM_FORCE_OUT );
};

class CSCForceOut : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCForceOut, CM_FORCE_OUT );
};
//////////////////////////////////////////////////////////////////////////
class CCSInvitePlayer : public CHeadPacket
{ public:
	int  m_nPlayerSeq;
	char m_sMessage[MESSAGE_SIZE];
	PACKET_CONSTRUCT( CCSInvitePlayer, CM_INVITE_PLAYER );
};

class CSCInvitePlayer : public CHeadPacket
{ public:
	char m_nResponse;
	char m_sFromName[PLAYER_NAME_SIZE];
	char m_sMessage[MESSAGE_SIZE];
	char m_sPass[5];
	int m_nRoomSeq;
	PACKET_CONSTRUCT( CSCInvitePlayer, CM_INVITE_PLAYER );
};
//////////////////////////////////////////////////////////////////////////

class CCSGameReady : public CHeadPacket
{ public:
	char				m_nReady;
	PACKET_CONSTRUCT( CCSGameReady, CM_GAME_READY );
};

class CSCGameReady : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nReady;
	char				m_nCancelTeam;
	PACKET_CONSTRUCT( CSCGameReady, CM_GAME_READY );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameStart : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	PACKET_CONSTRUCT( CCSGameStart, CM_GAME_START );
};

class CSCGameStart : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nParentSeq;
	int					m_nWeather;
	int					m_nRoomTimeType;
	short				m_nRandom;
	CAddress			m_cParentAddress;
	CMission			m_cMission; // 미션코드(CYG)
	char				m_nAttackTeam;
	PACKET_CONSTRUCT( CSCGameStart, CM_GAME_START );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameCount : public CHeadPacket
{ public:
	char				m_nCount;
	PACKET_CONSTRUCT( CCSGameCount, CM_GAME_COUNT );
};

class CSCGameCount : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nCount;
	PACKET_CONSTRUCT( CSCGameCount, CM_GAME_COUNT );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameLoad : public CHeadPacket
{ public:
	char				m_nStep;
	PACKET_CONSTRUCT( CCSGameLoad, CM_GAME_LOAD );
};

class CSCGameLoad : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nPlayerSeq;
	char				m_nStep;
	PACKET_CONSTRUCT( CSCGameLoad, CM_GAME_LOAD );
};
//////////////////////////////////////////////////////////////////////////
class CCSGamePlay : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	PACKET_CONSTRUCT( CCSGamePlay, CM_GAME_PLAY );
};

class CSCGamePlay : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCGamePlay, CM_GAME_PLAY );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameResult : public CHeadPacket
{ public:
	int					m_nMvpSeq;
	char				m_nMvpLevel;
	char				m_nMvpPosition;
	char				m_sMvpName[PLAYER_NAME_SIZE];
	float				m_fCurrentTime;
	CResult				m_cHomeResult;
	CResult				m_cAwayResult;
	CEachResult			m_cEachResult[TEAM_SIZE*2];
	PACKET_CONSTRUCT( CCSGameResult, CM_GAME_RESULT );
};

class CSCGameResult : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nMvpSeq;
	char				m_nMvpLevel;
	char				m_nMvpPosition;
	char				m_sMvpName[PLAYER_NAME_SIZE];
	float				m_fCurrentTime;
	CResult				m_cHomeResult;
	CResult				m_cAwayResult;
	CEachResult			m_cEachResult[TEAM_SIZE*2];
	PACKET_CONSTRUCT( CSCGameResult, CM_GAME_RESULT );
};
//////////////////////////////////////////////////////////////////////////
class CCSGameEnd : public CHeadPacket
{ public:
	int					m_nRoomSeq;
	PACKET_CONSTRUCT( CCSGameEnd, CM_GAME_END );
};

class CSCGameEnd : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCGameEnd, CM_GAME_END );
};
//////////////////////////////////////////////////////////////////////////
class CCSLevelUp : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSLevelUp, CM_LEVEL_UP );
};

class CSCLevelUp : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nPlayerSeq;
	CMoney				m_cMoney;
	CLevel				m_cLevel;
	CFaculty			m_cBaseFaculty;
	PACKET_CONSTRUCT( CSCLevelUp, CM_LEVEL_UP );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeTeam : public CHeadPacket
{ public:
	char				m_nChangeTeam;
	PACKET_CONSTRUCT( CCSChangeTeam, CM_CHANGE_TEAM );
};

class CSCChangeTeam : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nPlayerSeq;
	char				m_nFromTeam;
	char				m_nToTeam;
	char				m_nSeat;
	CReserveSeat		m_cHomeSeat;
	CReserveSeat		m_cAwaySeat;
	CReserveSeat		m_cViewSeat;
	PACKET_CONSTRUCT( CSCChangeTeam, CM_CHANGE_TEAM );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangePosition : public CHeadPacket
{ public:
	char				m_nHomePosition[TEAM_SIZE];
	char				m_nAwayPosition[TEAM_SIZE];
	PACKET_CONSTRUCT( CCSChangePosition, CM_CHANGE_POSITION );
};

class CSCChangePosition : public CHeadPacket
{ public:
	char				m_nResponse;
	CReserveSeat		m_cHomeSeat;
	CReserveSeat		m_cAwaySeat;
	PACKET_CONSTRUCT( CSCChangePosition, CM_CHANGE_POSITION );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeMent : public CHeadPacket
{ public:
	char				m_sMent[PLAYER_MENT_SIZE];
	PACKET_CONSTRUCT( CCSChangeMent, CM_CHANGE_MENT );
};

class CSCChangeMent : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_sMent[PLAYER_MENT_SIZE];
	PACKET_CONSTRUCT( CSCChangeMent, CM_CHANGE_MENT );
};
//////////////////////////////////////////////////////////////////////////
class CCSGrowupCharacter : public CHeadPacket
{ public:
	char				m_nPosition;
	PACKET_CONSTRUCT( CCSGrowupCharacter, CM_GROWUP_CHARACTER );
};

class CSCGrowupCharacter : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nPosition;
	CPlayerInfo			m_cPlayerInfo;  // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCGrowupCharacter, CM_GROWUP_CHARACTER );
};
//////////////////////////////////////////////////////////////////////////
// 퀘스트에 대한 보상을한다 (CYG)
class CCSQuestReward : public CHeadPacket
{ public:
	char m_nQuest; // 퀘스트 코드
	PACKET_CONSTRUCT( CCSQuestReward, CM_QUEST_REWARD );
};

class CSCQuestReward : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	PACKET_CONSTRUCT( CSCQuestReward, CM_QUEST_REWARD );
};
//////////////////////////////////////////////////////////////////////////

// 3000번대
//////////////////////////////////////////////////////////////////////////
class CShopItemData
{
public:
	int					m_nCode;
	int					m_nRemain;
	char				m_nDiscount;
//	int					m_nGroupCount;
};




class CCSShopItemList : public CHeadPacket
{ public:
	int					m_nType;
	char				m_nBrand;
	short				m_nPage;
	PACKET_CONSTRUCT( CCSShopItemList, CM_SHOPITEM_LIST );
};

class CSCShopItemList : public CHeadPacket
{ public:
	char				m_nResponse;
	short				m_nCurrentPage;
	short				m_nTotalPage;
	CShopItemData		m_cShopItemData[LIST6_SIZE];
	PACKET_CONSTRUCT( CSCShopItemList, CM_SHOPITEM_LIST );
};
//////////////////////////////////////////////////////////////////////////
#define LIST_RANDOMSHOP_SIZE	32
// 랜덤샵의 목록을 요청한다 (CYG)
class CCSRandomshopitemList : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSRandomshopitemList,  CM_RANDOMSHOPITEM_LIST );
};



class CSCRandomshopitemList : public CHeadPacket
{ public:
	char m_nResponse;
	short m_nItemCnt;
	int m_nShopID;
	CGameItem m_cShopItemData[LIST_RANDOMSHOP_SIZE];

	PACKET_CONSTRUCT( CSCRandomshopitemList ,  CM_RANDOMSHOPITEM_LIST );
};

//////////////////////////////////////////////////////////////////////////
class CCSUpdateItem : public CHeadPacket
{ public:
	int                 m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUpdateItem, CM_UPDATE_ITEM );
};
class CSCUpdateItem : public CHeadPacket
{ public:
	char                m_nResponse;
	char				m_nUpdateKind;
	int					m_nItemSeq;
	int					m_nCode;
	int					m_nClass;
	int					m_nPrice;
	char				m_nLevel;
	char				m_nGrade;
	char				m_nEquipKind;
	short				m_nAmount;
	int					m_nOptionCode[ITEM_OPTION_SIZE];
	CItemFacultyInfo	m_cItemFaculty; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCUpdateItem, CM_UPDATE_ITEM );
};

//////////////////////////////////////////////////////////////////////////
class CCSEquipItem : public CHeadPacket
{ public:
	int					m_nItemSeq;
	PACKET_CONSTRUCT( CCSEquipItem, CM_EQUIP_ITEM );
};

class CSCEquipItem : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nItemSeq;
	char				m_nEquipKind;
	int					m_nEquipWear[MAX_EQUIP];
	CItemFacultyInfo	m_cItemFaculty;	// 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCEquipItem, CM_EQUIP_ITEM );
};
//////////////////////////////////////////////////////////////////////////
class CCSDivestItem : public CHeadPacket
{ public:
	int					m_nItemSeq;
	PACKET_CONSTRUCT( CCSDivestItem, CM_DIVEST_ITEM );
};

class CSCDivestItem : public CHeadPacket
{ public:
	char				m_nResponse;
	int					m_nItemSeq;
	char				m_nEquipKind;
	int					m_nEquipWear[MAX_EQUIP];
	CItemFacultyInfo	m_cItemFaculty; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCDivestItem, CM_DIVEST_ITEM );
};
//////////////////////////////////////////////////////////////////////////
class CCSBuyItem : public CHeadPacket
{ public:
	int					m_nCode;
	char				m_nBuyKind;
	int					m_nPrice;
	int					m_nOptionCode[ITEM_OPTION_SIZE];
	PACKET_CONSTRUCT( CCSBuyItem, CM_BUY_ITEM );
};

class CSCBuyItem : public CHeadPacket
{ public:
	char				m_nResponse;
	CMoney				m_cMoney;
	CShape				m_cShape;
	int					m_nEquipWear[MAX_EQUIP];
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCBuyItem, CM_BUY_ITEM );
};
//////////////////////////////////////////////////////////////////////////

// 랜덤샾의 아이템을 구입한다. (CYG)
class CCSBuyRandomitem : public CHeadPacket
{ public:
	char m_nIdx;
	char m_nBuyKind;
	int  m_nCode;
	int  m_nPrice;
	int  m_nOptionCode[ITEM_OPTION_SIZE];

	PACKET_CONSTRUCT( CCSBuyRandomitem,  CM_BUY_RANDOMITEM );
};



class CSCBuyRandomitem : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nIdx;
	CMoney m_cMoney;
	int m_nEquipWear[MAX_EQUIP];
	char m_nEquipKind;
	PACKET_CONSTRUCT( CSCBuyRandomitem ,  CM_BUY_RANDOMITEM );
};


//////////////////////////////////////////////////////////////////////////

// 아이템 선물 (CYG)
class CCSGiftItem : public CHeadPacket
{ public:
	int m_nToPlayerSeq;
	int m_nCode;
	char m_nBuyKind;
	int m_nPrice;
	int m_nOptionCode[ITEM_OPTION_SIZE];
	PACKET_CONSTRUCT( CCSGiftItem,  CM_GIFT_ITEM );
};

class CSCGiftItem : public CHeadPacket
{ public:
	char m_nResponse;
	CMoney m_cMoney;
	PACKET_CONSTRUCT( CSCGiftItem ,  CM_GIFT_ITEM );
};

//////////////////////////////////////////////////////////////////////////
class CCSExchangeItem : public CHeadPacket
{ public:
	int					m_nCode;
	char				m_nBuyKind;
	int					m_nPrice;
	PACKET_CONSTRUCT( CCSExchangeItem, CM_EXCHANGE_ITEM );
};

class CSCExchangeItem : public CHeadPacket
{ public:
	char				m_nResponse;
	CMoney				m_cMoney;
	CShape				m_cShape;
	int					m_nEquipWear[MAX_EQUIP];
	PACKET_CONSTRUCT( CSCExchangeItem, CM_EXCHANGE_ITEM );
};
//////////////////////////////////////////////////////////////////////////
class CShopSkillData
{ public:
	int                 m_nCode;
	char                m_nDiscount;
};

class CCSShopSkillList : public CHeadPacket
{ public:
	char				m_nType;
	char				m_nPosition;
	short				m_nPage;
	PACKET_CONSTRUCT( CCSShopSkillList, CM_SHOPSKILL_LIST );
};

class CSCShopSkillList : public CHeadPacket
{ public:
	char                m_nResponse;
	short				m_nCurrentPage;
	short				m_nTotalPage;
	char				m_nType;
	char				m_nPosition;
	CShopSkillData      m_cShopSkillData[LIST6_SIZE];
	PACKET_CONSTRUCT( CSCShopSkillList, CM_SHOPSKILL_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSUpdateSkill : public CHeadPacket
{ public:
	int                 m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUpdateSkill, CM_UPDATE_SKILL );
};

class CSCUpdateSkill : public CHeadPacket
{ public:
	char                m_nResponse;
	char				m_nUpdateKind;
	int					m_nSkillSeq;
	int					m_nCode;
	char				m_nEquipKind;
	char				m_nLevel;
	PACKET_CONSTRUCT( CSCUpdateSkill, CM_UPDATE_SKILL );
};
//////////////////////////////////////////////////////////////////////////
class CCSEquipSkill : public CHeadPacket
{ public:
	int                 m_nSkillSeq;
	PACKET_CONSTRUCT( CCSEquipSkill, CM_EQUIP_SKILL );
};

class CSCEquipSkill : public CHeadPacket
{ public:
	char                m_nResponse;
	int					m_nSkillSeq;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCEquipSkill, CM_EQUIP_SKILL );
};
//////////////////////////////////////////////////////////////////////////
class CCSDivestSkill : public CHeadPacket
{ public:
	int                 m_nSkillSeq;
	PACKET_CONSTRUCT( CCSDivestSkill, CM_DIVEST_SKILL );
};

class CSCDivestSkill : public CHeadPacket
{ public:
	char                m_nResponse;
	int					m_nSkillSeq;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCDivestSkill, CM_DIVEST_SKILL );
};
//////////////////////////////////////////////////////////////////////////
class CCSBuySkill : public CHeadPacket
{ public:
	int                 m_nCode;
	char                m_nBuyKind;
	int                 m_nPrice;
	PACKET_CONSTRUCT( CCSBuySkill, CM_BUY_SKILL );
};

class CSCBuySkill : public CHeadPacket
{ public:
	char                m_nResponse;
	CMoney              m_cMoney;
	char                m_nEquipKind;
	PACKET_CONSTRUCT( CSCBuySkill, CM_BUY_SKILL );
};
//////////////////////////////////////////////////////////////////////////
class CShopTrainingData
{ public:
	int                 m_nCode;
	char                m_nDiscount;
};

class CCSShopTrainingList : public CHeadPacket
{ public:
	short				m_nPage;
	PACKET_CONSTRUCT( CCSShopTrainingList, CM_SHOPTRAINING_LIST );
};

class CSCShopTrainingList : public CHeadPacket
{ public:
	char                m_nResponse;
	short				m_nCurrentPage;
	short				m_nTotalPage;
	CShopTrainingData	m_cShopTrainingData[LIST6_SIZE];
	PACKET_CONSTRUCT( CSCShopTrainingList, CM_SHOPTRAINING_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSUpdateTraining : public CHeadPacket
{ public:
	int                 m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUpdateTraining, CM_UPDATE_TRAINING );
};

class CSCUpdateTraining : public CHeadPacket
{ public:
	char                m_nResponse;
	char				m_nUpdateKind;
	int					m_nTrainingSeq;
	int					m_nCode;
	char				m_nLevel;
	PACKET_CONSTRUCT( CSCUpdateTraining, CM_UPDATE_TRAINING );
};
//////////////////////////////////////////////////////////////////////////
class CCSBuyTraining : public CHeadPacket
{ public:
	int                 m_nCode;
	char                m_nBuyKind;
	int                 m_nPrice;
	PACKET_CONSTRUCT( CCSBuyTraining, CM_BUY_TRAINING );
};

class CSCBuyTraining : public CHeadPacket
{ public:
	char                m_nResponse;
	CMoney              m_cMoney;
	char                m_nEquipKind;
	CFaculty			m_cTrainingFaculty;
	PACKET_CONSTRUCT( CSCBuyTraining, CM_BUY_TRAINING );
};
//////////////////////////////////////////////////////////////////////////
class CShopCeremonyData
{ public:
	int                 m_nCode;
	char                m_nDiscount;
};

class CCSShopCeremonyList : public CHeadPacket
{ public:
	short				m_nPage;
	PACKET_CONSTRUCT( CCSShopCeremonyList, CM_SHOPCEREMONY_LIST );
};

class CSCShopCeremonyList : public CHeadPacket
{ public:
	char                m_nResponse;
	short				m_nCurrentPage;
	short				m_nTotalPage;
	CShopCeremonyData	m_cShopCeremonyData[LIST6_SIZE];
	PACKET_CONSTRUCT( CSCShopCeremonyList, CM_SHOPCEREMONY_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSUpdateCeremony : public CHeadPacket
{ public:
	int                 m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUpdateCeremony, CM_UPDATE_CEREMONY );
};

class CSCUpdateCeremony : public CHeadPacket
{ public:
	char                m_nResponse;
	char				m_nUpdateKind;
	int					m_nCeremonySeq;
	int					m_nCode;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCUpdateCeremony, CM_UPDATE_CEREMONY );
};
//////////////////////////////////////////////////////////////////////////
class CCSEquipCeremony : public CHeadPacket
{ public:
	int                 m_nCeremonySeq;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CCSEquipCeremony, CM_EQUIP_CEREMONY );
};

class CSCEquipCeremony : public CHeadPacket
{ public:
	char                m_nResponse;
	int					m_nCeremonySeq;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCEquipCeremony, CM_EQUIP_CEREMONY );
};
//////////////////////////////////////////////////////////////////////////
class CCSDivestCeremony : public CHeadPacket
{ public:
	int                 m_nCeremonySeq;
	PACKET_CONSTRUCT( CCSDivestCeremony, CM_DIVEST_CEREMONY );
};

class CSCDivestCeremony : public CHeadPacket
{ public:
	char                m_nResponse;
	int					m_nCeremonySeq;
	char				m_nEquipKind;
	PACKET_CONSTRUCT( CSCDivestCeremony, CM_DIVEST_CEREMONY );
};
//////////////////////////////////////////////////////////////////////////
class CCSBuyCeremony : public CHeadPacket
{ public:
	int                 m_nCode;
	char                m_nBuyKind;
	int                 m_nPrice;
	PACKET_CONSTRUCT( CCSBuyCeremony, CM_BUY_CEREMONY );
};

class CSCBuyCeremony : public CHeadPacket
{ public:
	char                m_nResponse;
	CMoney              m_cMoney;
	char                m_nEquipKind;
	PACKET_CONSTRUCT( CSCBuyCeremony, CM_BUY_CEREMONY );
};
//////////////////////////////////////////////////////////////////////////
class CQuestData
{ public:
	int                 m_nCode;
};

class CCSQuestList : public CHeadPacket
{ public:
	short				m_nPage;
	PACKET_CONSTRUCT( CCSQuestList, CM_QUEST_LIST );
};

class CSCQuestList : public CHeadPacket
{ public:
	char                m_nResponse;
	short				m_nCurrentPage;
	short				m_nTotalPage;
	CQuestData			m_cQuestData[LIST6_SIZE];
	PACKET_CONSTRUCT( CSCQuestList, CM_QUEST_LIST );
};
//////////////////////////////////////////////////////////////////////////
class CCSUpdateQuest : public CHeadPacket
{ public:
	int                 m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUpdateQuest, CM_UPDATE_QUEST );
};

class CSCUpdateQuest : public CHeadPacket
{ public:
	char                m_nResponse;
	char				m_nUpdateKind;
	int					m_nQuestSeq;
	int					m_nCode;
	short				m_nAmount;
	int					m_nPlayDate;
	PACKET_CONSTRUCT( CSCUpdateQuest, CM_UPDATE_QUEST );
};
//////////////////////////////////////////////////////////////////////////
class CCSCreateQuest : public CHeadPacket
{ public:
	char				m_nState;
	char				m_nMode;
	char				m_sTitle[TITLE_NAME_SIZE];
	char				m_sPass[5];
	int					m_nQuestCode;
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	char 				m_nStartLevel;
	char 				m_nEndLevel;
	char 				m_nCheckClub;
	char 				m_nCheckTime;
	char 				m_nCheckWeather;
	char 				m_nCheckView;
	char 				m_nCheckViewChat;
	char				m_nMaxCount;
	char				m_nHomePosition[TEAM_SIZE];
	char				m_nAwayPosition[TEAM_SIZE];
	PACKET_CONSTRUCT( CCSCreateQuest, CM_CREATE_QUEST );
};

class CSCCreateQuest : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCCreateQuest, CM_CREATE_QUEST );
};
//////////////////////////////////////////////////////////////////////////

// 5000번대
//////////////////////////////////////////////////////////////////////////
class CCSTCPPing : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSTCPPing, CM_TCP_PING );
};

class CSCTCPPing : public CHeadPacket
{ public:
	char				m_nResponse;
	PACKET_CONSTRUCT( CSCTCPPing, CM_TCP_PING );
};
//////////////////////////////////////////////////////////////////////////
class CCSSendMessage : public CHeadPacket
{ public:
	char				m_nChatKind;
	char				m_sToName[PLAYER_NAME_SIZE];
	char				m_sMessage[MESSAGE_SIZE];
	PACKET_CONSTRUCT( CCSSendMessage, CM_SEND_MESSAGE );
};

class CSCSendMessage : public CHeadPacket
{ public:
	int					m_nPlayerSeq;
	char				m_nResponse;
	char				m_nChatKind;
	char				m_sFromName[PLAYER_NAME_SIZE];
	char				m_sMessage[MESSAGE_SIZE];
	PACKET_CONSTRUCT( CSCSendMessage, CM_SEND_MESSAGE );
};
//////////////////////////////////////////////////////////////////////////
class CCSRaiseFaculty : public CHeadPacket
{ public:
	CFaculty			m_cChangeFaculty;
	PACKET_CONSTRUCT( CCSRaiseFaculty, CM_RAISE_FACULTY );
};

class CSCRaiseFaculty : public CHeadPacket
{ public:
	char				m_nResponse;
	CLevel				m_cLevel;
	CFaculty			m_cRaiseFaculty;
	PACKET_CONSTRUCT( CSCRaiseFaculty, CM_RAISE_FACULTY );
};
//////////////////////////////////////////////////////////////////////////
class CCSChangeSetting : public CHeadPacket
{ public:
	char				m_nInitSetting;
	int					m_nPlayerSeq;
	CSetting			m_cChangeSetting;
	PACKET_CONSTRUCT( CCSChangeSetting, CM_CHANGE_SETTING );
};

class CSCChangeSetting : public CHeadPacket
{ public:
	char				m_nResponse;
	char				m_nInitSetting;
	CSetting			m_cSetting;
	PACKET_CONSTRUCT( CSCChangeSetting, CM_CHANGE_SETTING );
};
//////////////////////////////////////////////////////////////////////////
// 캐릭터 찾기 (CYG)
class CCSCharacterSearch : public CHeadPacket
{ public:
	char m_sName[PLAYER_NAME_SIZE]; // 캐릭터명
	PACKET_CONSTRUCT( CCSCharacterSearch, CM_CHARACTER_SEARCH );
};

class CSCCharacterSearch : public CHeadPacket
{ public:
	char m_nResponse; //결과
	int  m_nPlayerSeq;//캐릭터 시퀀스
	PACKET_CONSTRUCT( CSCCharacterSearch, CM_CHARACTER_SEARCH );
};
//////////////////////////////////////////////////////////////////////////
// 아이템 전달 (소포시스템) (CYG)
class CCSPostItem : public CHeadPacket
{ public:
	int m_nItemSeq; // 아이템 시퀀스
	int	m_nPlayerSeq; // 사용자 시퀀스
	PACKET_CONSTRUCT( CCSPostItem, CM_POST_ITEM );
};

class CSCPostItem : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	int m_nItemSeq;   // 아이템시퀀스
	int m_nEquipWear[MAX_EQUIP]; // 장착상태 명세서
	CItemFacultyInfo	m_cItemFaculty;  // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCPostItem, CM_POST_ITEM );
};
//////////////////////////////////////////////////////////////////////////
// 미션에 대한 보상을 한다. (CYG)
class CCSMissionReward : public CHeadPacket
{ public:
	CMission m_cMission; // 보상할 미션내역
	PACKET_CONSTRUCT( CCSMissionReward, CM_MISSION_REWARD );
};

class CSCMissionReward : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	PACKET_CONSTRUCT( CSCMissionReward, CM_MISSION_REWARD );
};
//////////////////////////////////////////////////////////////////////////
// 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)

class CCSScheduleList : public CHeadPacket
{ public:
	int  m_nServerCode;
	PACKET_CONSTRUCT( CCSScheduleList, CM_SCHEDULE_LIST );
};

class CSCScheduleList : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nCount;
	char m_nPacketPosition;
	long m_nCurrentTime;
	CScheduleData m_cScheduleList[MAX_SCHEDULE_LIST];
	PACKET_CONSTRUCT( CSCScheduleList, CM_SCHEDULE_LIST );
};

// 플레이어 정보 끝 (CYG)
class CCSPlayerinfoEnd : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSPlayerinfoEnd, CM_PLAYERINFO_END );
};

class CSCPlayerinfoEnd : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCPlayerinfoEnd, CM_PLAYERINFO_END );
};

// 오토파일럿 모드 설정 (CYG)
class CCSAutopilotMode : public CHeadPacket
{ public:
	char m_bEnable; // 모드 설정
	PACKET_CONSTRUCT( CCSAutopilotMode, CM_AUTOPILOT_MODE );
};

class CSCAutopilotMode : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	char m_bEnable;   // 모드값 리턴
	int	 m_nPlayerSeq;// 사용자 시퀀스
	PACKET_CONSTRUCT( CSCAutopilotMode, CM_AUTOPILOT_MODE );
};




// 현재 날씨 얻어오기 (CYG)
class CCSCurrentWeather : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSCurrentWeather, CM_CURRENT_WEATHER );
};

class CSCCurrentWeather : public CHeadPacket
{ public:
	char m_nResponse;       // 결과값
	char m_nCurrentWeather; // 현재 날씨값
	int  m_nEndTime;      // 현재날씨 종료시간
	PACKET_CONSTRUCT( CSCCurrentWeather, CM_CURRENT_WEATHER );
};

// 소모성 아이템 사용 통지 (CYG)
class CCSConsumeItem : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSConsumeItem, CM_CONSUME_ITEM );
};

class CSCConsumeItem : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	short m_nCount; // 전송된 건수
	CItemCountInfo m_cItemCnt[MAX_ITEM_LIST]; // 소모된 아이템 정보


	PACKET_CONSTRUCT( CSCConsumeItem, CM_CONSUME_ITEM );
};



// 아이템 능력치 갱신 (CYG)
class CCSUpdateItemfaculty : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSUpdateItemfaculty, CM_UPDATE_ITEMFACULTY );
};

class CSCUpdateItemfaculty : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	int m_nEquipWear[MAX_EQUIP]; // 장착상태
	CItemFacultyInfo	m_cItemFaculty; // 가변패킷 이 변수 다음에 다른 변수를 추가하지 말 것 (CYG)
	PACKET_CONSTRUCT( CSCUpdateItemfaculty, CM_UPDATE_ITEMFACULTY );
};

// 접속중인 사용자 강제퇴장 (CYG)
class CCSDrawforcePlayer : public CHeadPacket
{ public:
	char m_sID[ID_NAME_SIZE]; // 아이디
	char m_sPass[LOGIN_PASS_SIZE]; // 패스워드
	PACKET_CONSTRUCT( CCSDrawforcePlayer, CM_DRAWFORCE_PLAYER );
};

class CSCDrawforcePlayer : public CHeadPacket
{ public:
	char m_nResponse; // 결과
	PACKET_CONSTRUCT( CSCDrawforcePlayer, CM_DRAWFORCE_PLAYER );
};


// 현재 시간 얻어오기 (CYG)
class CCSCurrentTime : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSCurrentTime, CM_CURRENT_TIME );
};

class CSCCurrentTime : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	char m_nRoomTimeType;// 시간 구분
	int  m_nCurrentTime; // 현재 시간값
	PACKET_CONSTRUCT( CSCCurrentTime, CM_CURRENT_TIME );
};


// 스킬 강화 (CYG)
class CCSUpgradeSkill : public CHeadPacket
{ public:
	int m_nSkillSeq;
	PACKET_CONSTRUCT( CCSUpgradeSkill,  CM_UPGRADE_SKILL );
};



class CSCUpgradeSkill : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nLevel;
	int  m_nSkillSeq;
	int  m_nSkillPoint;
	PACKET_CONSTRUCT( CSCUpgradeSkill ,  CM_UPGRADE_SKILL );
};

// 옵션 정보 (CYG)
class CCSSettingInfo : public CHeadPacket
{ public:
	int    m_nPlayerSeq; // 플레이어 시퀀스
	PACKET_CONSTRUCT( CCSSettingInfo,  CM_SETTING_INFO );
};



class CSCSettingInfo : public CHeadPacket
{ public:
	char m_nResponse;    // 결과값
	int    m_nPlayerSeq; // 플레이어 시퀀스
	CSetting m_cSetting; // 세팅정보
	PACKET_CONSTRUCT( CSCSettingInfo ,  CM_SETTING_INFO );
};

// 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)
class CCSSynchPlayer : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSSynchPlayer,  CM_SYNCH_PLAYER );
};

class CSCSynchPlayer : public CHeadPacket
{ public:
	char m_nResponse; // 결과
	PACKET_CONSTRUCT( CSCSynchPlayer ,  CM_SYNCH_PLAYER );
};

// 카드템 정보 (CYG)
class CCSCardInfo : public CHeadPacket
{ public:
	int m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSCardInfo,  CM_CARD_INFO );
};

class CSCCardInfo : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nCount;
	CCardInfo m_cCardInfo[MAX_CARD_LIST];
	PACKET_CONSTRUCT( CSCCardInfo ,  CM_CARD_INFO );
};

// 카드장착 (CYG)
class CCSEquipCard : public CHeadPacket
{ public:
	int    m_nCardSeq;
	char   m_nEquipEntry;
	char   m_nEquipKind;
	PACKET_CONSTRUCT( CCSEquipCard,  CM_EQUIP_CARD );
};



class CSCEquipCard : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nCardSeq;
	char m_nEquipEntry;
	char m_nEquipKind;
	PACKET_CONSTRUCT( CSCEquipCard ,  CM_EQUIP_CARD );
};


// 카드해제 (CYG)
class CCSDivestCard : public CHeadPacket
{ public:
	int    m_nCardSeq;
	char   m_nEquipEntry;
	PACKET_CONSTRUCT( CCSDivestCard,  CM_DIVEST_CARD );
};



class CSCDivestCard : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nCardSeq;
	char m_nEquipEntry;
	char m_nEquipKind;
	PACKET_CONSTRUCT( CSCDivestCard ,  CM_DIVEST_CARD );
};


// 카드획득 (CYG)
class CCSGetCard : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSGetCard,  CM_GET_CARD );
};

class CSCGetCard : public CHeadPacket
{ public:
	char m_nResponse;
	CCardInfo m_cCard;
	PACKET_CONSTRUCT( CSCGetCard ,  CM_GET_CARD );
};


// 아이템조합 타입 I (CYG)
class CCSMixItem1 : public CHeadPacket
{ public:
	int  m_nMixSeq;
	int  m_nItemSeq[ITEM1_MIX_SIZE];
	char m_nItemCnt[ITEM1_MIX_SIZE];
	PACKET_CONSTRUCT( CCSMixItem1,  CM_MIX_ITEM1 );
};



class CSCMixItem1 : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nMixSeq;
	int  m_nItemSeq;
	PACKET_CONSTRUCT( CSCMixItem1 ,  CM_MIX_ITEM1 );
};

// 아이템조합 타입 II (CYG)
class CCSMixItem2 : public CHeadPacket
{ public:
	int	 m_nEmblemType;
	int  m_nItemSeq;
	PACKET_CONSTRUCT( CCSMixItem2,  CM_MIX_ITEM2 );
};



class CSCMixItem2 : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nItemSeq;
	PACKET_CONSTRUCT( CSCMixItem2 ,  CM_MIX_ITEM2 );
};

// 카드 조합 I (CYG)
class CCSMixCard1 : public CHeadPacket
{ public:
	char m_nCardMixType;
	int  m_nCardSeq[CARD_MIX_SIZE];

	PACKET_CONSTRUCT( CCSMixCard1,  CM_MIX_CARD1 );
};



class CSCMixCard1 : public CHeadPacket
{ public:
	char m_nResponse;
	CCardInfo m_cCard;
	PACKET_CONSTRUCT( CSCMixCard1 ,  CM_MIX_CARD1 );
};

// 카드 조합 II (CYG)
class CCSMixCard2 : public CHeadPacket
{ public:
	int  m_nCardSeq;
	char m_nEmblemType;
	int  m_nEmblemCnt;

	PACKET_CONSTRUCT( CCSMixCard2,  CM_MIX_CARD2 );
};



class CSCMixCard2 : public CHeadPacket
{ public:
	char  m_nResponse;
	int   m_nItemSeq;
	short m_nItemCnt;
	PACKET_CONSTRUCT( CSCMixCard2 ,  CM_MIX_CARD2 );
};

// 친구 정보 (CYG)
class CCSBuddyInfo : public CHeadPacket
{ public:
	int m_nPage;
	PACKET_CONSTRUCT( CCSBuddyInfo,  CM_BUDDY_INFO );
};

class CSCBuddyInfo : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nPage;
	char m_nCount;
	int	 m_nTotalPage;
	CBuddyInfo m_cBuddyInfo[MAX_BUDDY_LIST];
	PACKET_CONSTRUCT( CSCBuddyInfo ,  CM_BUDDY_INFO );
};


// 골인 TCP 서버 처리 (CYG)
class CCSGoalinTcp : public CHeadPacket
{ public:
	CPPGoalIn m_cGoalPacket;
	PACKET_CONSTRUCT( CCSGoalinTcp,  CM_GOALIN_TCP );
};


class CSCGoalinTcp : public CHeadPacket
{ public:
	char m_nResponse;
	CPPGoalIn m_cGoalPacket;
	PACKET_CONSTRUCT( CSCGoalinTcp ,  CM_GOALIN_TCP );
};


// ON/OFF 값 공유 (CYG)
class CCSSwitchValue : public CHeadPacket
{ public:
	char m_nType;
	int	 m_nValue;
	PACKET_CONSTRUCT( CCSSwitchValue,  CM_SWITCH_VALUE );
};



class CSCSwitchValue : public CHeadPacket
{ public:
	char m_nResponse; // 결과값
	char m_nType;     // 타입
	int  m_nValue;   // 모드값 리턴
	int  m_nPlayerSeq;// 사용자 시퀀스
	PACKET_CONSTRUCT( CSCSwitchValue ,  CM_SWITCH_VALUE );
};


// 친구추가 (CYG)
class CCSAddBuddy : public CHeadPacket
{ public:
	 int m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSAddBuddy,  CM_ADD_BUDDY );
};



class CSCAddBuddy : public CHeadPacket
{ public:
	 char m_nResponse;
	PACKET_CONSTRUCT( CSCAddBuddy ,  CM_ADD_BUDDY );
};

// 블랙리스트 정보 (CYG)
class CCSBlacklistInfo : public CHeadPacket
{ public:
	PACKET_CONSTRUCT( CCSBlacklistInfo,  CM_BLACKLIST_INFO );
};

// 친구정보
class CSCBlacklistInfo : public CHeadPacket
{ public:
	char m_nResponse;
	char m_nCount;
	CBlacklistInfo m_cBlacklistInfo[MAX_BLACK_LIST];
	PACKET_CONSTRUCT( CSCBlacklistInfo ,  CM_BLACKLIST_INFO );
};


// 블랙리스트 추가 (CYG)
class CCSAddBlacklist : public CHeadPacket
{ public:
	int m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSAddBlacklist,  CM_ADD_BLACKLIST );
};



class CSCAddBlacklist : public CHeadPacket
{ public:
	 char m_nResponse;
	PACKET_CONSTRUCT( CSCAddBlacklist ,  CM_ADD_BLACKLIST );
};


// 주간 기록 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
class CCSWeeklyRecord : public CHeadPacket
{ public:
	int m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSWeeklyRecord,
 );
};



class CSCWeeklyRecord : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nPlayerSeq;
	CWeeklyRecord m_cWeeklyMatchRecord;
	CWeeklyRecord m_cWeeklyMarkRecord;
	PACKET_CONSTRUCT( CSCWeeklyRecord ,  CM_WEEKLY_RECORD );
};

// 친구삭제 (CYG)
class CCSDelBuddy : public CHeadPacket
{ public:
	int m_nPlayerSeq;  // 삭제할 친구
	PACKET_CONSTRUCT( CCSDelBuddy,  CM_DEL_BUDDY );
};



class CSCDelBuddy : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCDelBuddy ,  CM_DEL_BUDDY );
};

// 블랙리스트 삭제 (CYG)
class CCSDelBlacklist : public CHeadPacket
{ public:
	int m_nPlayerSeq;  // 삭제할 블랙리스트
	PACKET_CONSTRUCT( CCSDelBlacklist,  CM_DEL_BLACKLIST );
};



class CSCDelBlacklist : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCDelBlacklist ,  CM_DEL_BLACKLIST );
};


// 주간 랭킹 정보  (CYG)
class CCSWeeklyRanking : public CHeadPacket
{ public:
	int m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSWeeklyRanking,  CM_WEEKLY_RANKING );
};



class CSCWeeklyRanking : public CHeadPacket
{ public:
	char m_nResponse;
	int  m_nPlayerSeq;
	CWeeklyRanking m_cWeeklyRanking;
	CWeeklyRanking m_cWeeklyMarkRanking;
	PACKET_CONSTRUCT( CSCWeeklyRanking ,  CM_WEEKLY_RANKING );
};

// 아이템을 강화한다. (CYG)
class CCSEnchantItem : public CHeadPacket
{ public:
	int  m_nItemSeq;
	int  m_nPrice;
	char m_nEnchantType;
	char m_nBuyKind;

	PACKET_CONSTRUCT( CCSEnchantItem,  CM_ENCHANT_ITEM );
};



class CSCEnchantItem : public CHeadPacket
{ public:
	char   m_nResponse;
	int    m_nItemSeq;
	char   m_nEnchantType;
	CMoney m_cMoney;

	PACKET_CONSTRUCT( CSCEnchantItem ,  CM_ENCHANT_ITEM );
};

// 랜덤샵을 새로고침한다. (CYG)
class CCSRefreshShop : public CHeadPacket
{ public:
	char m_nBuyKind;

	PACKET_CONSTRUCT( CCSRefreshShop,  CM_REFRESH_SHOP );
};



class CSCRefreshShop : public CHeadPacket
{ public:
	char   m_nResponse;
	CMoney m_cMoney;

	PACKET_CONSTRUCT( CSCRefreshShop ,  CM_REFRESH_SHOP );
};


// 카드부스터 구입 (CYG)
class CCSBuyCardbooster : public CHeadPacket
{ public:
	int	 m_nCode;
	char m_nBuyKind;
	int  m_nPrice;

	PACKET_CONSTRUCT( CCSBuyCardbooster,  CM_BUY_CARDBOOSTER );
};



class CSCBuyCardbooster : public CHeadPacket
{ public:
	char   m_nResponse;
	CCardInfo m_cCardInfo;
	char   m_nCode;
	CMoney m_cMoney;
	PACKET_CONSTRUCT( CSCBuyCardbooster ,  CM_BUY_CARDBOOSTER );
};

// 카드 엔트리 세팅 (CYG)
class CCSCardEntry : public CHeadPacket
{ public:
	char m_nEntry;

	PACKET_CONSTRUCT( CCSCardEntry,  CM_CARD_ENTRY );
};



class CSCCardEntry : public CHeadPacket
{ public:
	char   m_nResponse;
	char   m_nEntry;
	PACKET_CONSTRUCT( CSCCardEntry ,  CM_CARD_ENTRY );
};

// 피트인 (CYG)
class CCSPitIn : public CHeadPacket
{ public:
	int	 m_nPlayerSeq;
	char m_bInOut;
	char m_bAdminReq;
	PACKET_CONSTRUCT( CCSPitIn,  CM_PIT_IN );
};



class CSCPitIn : public CHeadPacket
{ public:
	char m_nResponse;
	int	 m_nPlayerSeq;
	char m_bInOut;
	char m_bAdminReq;
	PACKET_CONSTRUCT( CSCPitIn ,  CM_PIT_IN );
};

//--SOURCE_AUTO_INSERT_SCRIPT

//////////////////////////////////////////////////////////////////////////
// STS 서버 로그인 (CYG)
class CCSStsLogin : public CHeadPacket
{ public:
	short m_nServerCode;
	PACKET_CONSTRUCT( CCSStsLogin, CM_STS_LOGIN );
};

class CSCStsLogin : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCStsLogin, CM_STS_LOGIN );
};

// 접속중인 사용자 강제퇴장 (CYG)
class CCSStsDrawforce : public CHeadPacket
{ public:
	int m_nPlayerSeq; // 사용자 시퀀스
	int m_nMemberSeq; // 사용자 시퀀스
	PACKET_CONSTRUCT( CCSStsDrawforce, CM_STS_DRAWFORCE );
};

class CSCStsDrawforce : public CHeadPacket
{ public:
	char m_nResponse; // 결과
	PACKET_CONSTRUCT( CSCStsDrawforce, CM_STS_DRAWFORCE );
};


// 전서버 날씨 갱신 (CYG)
class CCSUpdateWeather : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSUpdateWeather, CM_UPDATE_WEATHER );
};

class CSCUpdateWeather : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCUpdateWeather, CM_UPDATE_WEATHER );
};

// 전서버 스케쥴 갱신 (CYG)
class CCSUpdateSchedule : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSUpdateSchedule, CM_UPDATE_SCHEDULE );
};

class CSCUpdateSchedule : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCUpdateSchedule, CM_UPDATE_SCHEDULE );
};

// 전 서버 공지 갱신 (CYG)
class CCSUpdateNotice : public CHeadPacket
{ public:

	PACKET_CONSTRUCT( CCSUpdateNotice, CM_UPDATE_NOTICE );
};

class CSCUpdateNotice : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCUpdateNotice, CM_UPDATE_NOTICE );
};

// 전 서버 공지 채팅 전송 (CYG)
class CCSSendBroadcast : public CHeadPacket
{ public:
	char m_nChatKind;
	char m_sMessage[MESSAGE_SIZE];
	PACKET_CONSTRUCT( CCSSendBroadcast, CM_SEND_BROADCAST );
};

class CSCSendBroadcast : public CHeadPacket
{ public:
	char m_nResponse;
	PACKET_CONSTRUCT( CSCSendBroadcast, CM_SEND_BROADCAST );
};

//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// P2P PROTOCOL
	//////////////////////////////////////////////////////////////////////////
	class CCSUDPPunching : public CHeadPacket
	{ public:
		int					m_nPlayerSeq;
	PACKET_CONSTRUCT( CCSUDPPunching, CM_UDP_PUNCHING );
	};

	class CSCUDPPunching : public CHeadPacket
	{ public:
		char				m_nResponse;
	PACKET_CONSTRUCT( CSCUDPPunching, CM_UDP_PUNCHING );
	};
	//////////////////////////////////////////////////////////////////////////
	class CCSUDPPing : public CHeadPacket
	{ public:
		char			       m_nResponse;
	PACKET_CONSTRUCT( CCSUDPPing, CM_UDP_PING );
	};

	class CSCUDPPing : public CHeadPacket
	{ public:
	PACKET_CONSTRUCT( CSCUDPPing, CM_UDP_PING );
	};


	//////////////////////////////////////////////////////////////////////////

// 클라이언트에서만 사용하는 UDP 패킷
	// ok
#ifdef	COMPILE_FOR_CLIENT

	class CBallAction
	{ public:
		char				m_nState;
		int					m_nBallerSeq;
		int					m_nNoMarkSeq;
		float				m_fOriginTime;
		CLocation			m_cOriginLocation;
		CLocation			m_cCurrentLocation;
		CVector				m_cOriginVector;
		short				m_nSkillType;
		unsigned short		m_nPlayTime;
		float				m_fCurrentTime;
	};

	class CPlayerAction
	{ public:
		int					m_nPlayerSeq;
		char				m_nActionSeq;
		int					m_nAniCode;
		float				m_fStartAngle;
		float				m_fEndAngle;
		char				m_nAniSpeed;
		short				m_nMoveSpeed;
		short				m_nSkillType;
		short				m_nHeadCode;
		CLocation			m_cNodeLocation;
		CLocation			m_cTargetLocation;
	};

	class CPPObjectAction : public CHeadPacket
	{ public:
		CBallAction			m_cBallAction;
		char				m_nAmount;
		CPlayerAction		m_cPlayerAction[];
	};


	//////////////////////////////////////////////////////////////////////////
	class CPlayerLocation
	{ public:
		int					m_nPlayerSeq;
		short				m_nX;
		short				m_nY;
	};

	class CPPObjectLocation: public CHeadPacket
	{ public:
		char				m_nAmount;
		CPlayerLocation		m_cPlayerLocation[];
	};
	//////////////////////////////////////////////////////////////////////////
	class CPPBallAction : public CHeadPacket
	{ public:
		char				m_nState;
		int					m_nBallerSeq;
		float				m_fOriginTime;
		CLocation			m_cOriginLocation;
		CLocation			m_cCurrentLocation;
		CVector				m_cOriginVector;
		short				m_nSkillType;
		char				m_nBallSound;
		char				m_nBallEffect;
	};
	//////////////////////////////////////////////////////////////////////////
	class CPPInputKey : public CHeadPacket
	{ public:
		int					m_nPlayerSeq;
		CInputKey			m_cInputKey;
	};
	//////////////////////////////////////////////////////////////////////////
	class CPPEmotionInfo : public CHeadPacket
	{ public:
		int					m_nPlayerSeq;
		char				m_nEmotionCode;
		bool				m_bCheckUDP;
	};
	//////////////////////////////////////////////////////////////////////////
	class CPPAngerInfo : public CHeadPacket
	{ public:
	int					m_nHomeAngerTime;
	int					m_nAwayAngerTime;
	};
#endif

#ifdef	COMPILE_FOR_CLIENT
#pragma pack(push,4)
#else
#pragma pack(push,4)
#endif

#endif




