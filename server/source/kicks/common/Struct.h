#ifndef _STRUCT_H_
#define _STRUCT_H_

#include "Define.h"
//캐릭터 정보 관련//////////////////////////////////////////////////////////////////
//캐릭터 형태 관련 클래스
class CShape
{
public:
	char		m_nGender;
	char		m_nSkin;
	char		m_nUniform;
};

//레벨 관련 클래스
class CLevel
{
public:
	short		m_nLevel;
	int			m_nExp;
	short		m_nFaculty;
	short		m_nSkill;
};

//돈 관련 클래스
class CMoney
{
public:
	int			m_nCash;
	int			m_nPoint;
	int			m_nCredit;
	int			m_nClubPoint;
};

//각종 크기 관련 클래스
class CSize
{
public:
	char		m_nCharacterSize;
	char		m_nInvenSize;
	char		m_nSkillSize;
};

//클럽 관련 클래스
class CClub
{
public:
	char		m_sClubNmae[CLUB_NAME_SIZE];
	int			m_nClubSeq;
	char		m_nClubGradeCode;
};

//주소 관련 클래스
class CAddress
{
public:
	char		m_sIP[IP_SIZE];
	int			m_nPort;
};

class CFaculty
{
public:
	char		m_nFaculty[ARRAY_FACULTY_SIZE];
};

class CItemFacultyInfo
{
public:
	CItemFacultyInfo() { m_nFacultyCnt = 0; }
	char				m_nFacultyCnt;
	unsigned int		m_nFaculty[MAX_ITEM_FACULTY_SIZE];
};

// 능력치 검사를 위한 Int버퍼 (CYG)
class CFacultyInt
{
public:
	int			m_nFaculty[ARRAY_FACULTY_SIZE];
};

// 친구정보
class CBuddyInfo
{
public:
	int			m_nSeq;
	int			m_nPlayerSeq;
	int			m_nState;
	int			m_nWhere;
	short		m_nLevel;
	char		m_nPosition;
	char		m_sName[PLAYER_NAME_SIZE];

};

class CResult
{
public:
	short		m_nResult[ARRAY_RESULT_SIZE];
};

class CRecord
{
public:
	int			m_nRecord[ARRAY_RECORD_SIZE]; // 레코드 사이즈 변경(CYG)

public:
	CRecord		operator+(const CRecord& cRecord)
	{
		for(int i=0;i<ARRAY_RECORD_SIZE;++i)
		{
			m_nRecord[i] += cRecord.m_nRecord[i];
		}
		return *this;
	}

	bool IsEmpty()
	{
		for(int i=0;i<ARRAY_RECORD_SIZE;++i)
		{
			if( m_nRecord[i] != 0 ) return false;
		}
		return true;
	}

};


class CRanking
{
public:
	short		m_nRanking[ARRAY_RANKING_SIZE];
};

#pragma pack(push,1)

class CSkillInfo
{
public:
	int					m_nSkillSeq;
	int					m_nCode;
	char				m_nEquipKind;
	char				m_nLevel;
};

class CTrainingInfo
{
public:
	int					m_nTrainingSeq;
	int					m_nCode;
	char				m_nLevel;
};

class CCeremonyInfo
{
public:
	int					m_nCeremonySeq;
	int					m_nCode;
	char				m_nEquipKind;
};

class CSetting
{
public:
	char		m_nCameraType;								//카메라 선택 enum CAMERA_TYPE
	char		m_nCameraTarget;							//카메라 대상체 enum CAMERA_TARGET
	char		m_nCameraTeam;
	char		m_nCameraZoom;								//카메라 줌인아웃
	char		m_nRadian;									//카메라 각도
	char		m_nShadow;									//그림자 유무
	char		m_nLabel;									//라벨 보기 선택(0:라벨안봄, 1:포지션만, 2:포지션+캐릭명)

	char		m_nSound;									//효과음 볼륨(0일 경우 볼륨 off)
	char		m_nMusic;									//배경음 볼륨(0일 경우 볼륨 off)

	char		m_nInvite;									//초대 응하기 유무
	char		m_nWhisper;									//귓속말 응하기 유무
	char		m_nFriend;									//친구추가 허용 유무

	short		m_nDefineKey[ARRAY_KEY_SIZE];				//키 선택

	int			m_nAttackSkillCode[ARRAY_SKILL_KEY_SIZE];	//공격 스킬 선택
	int			m_nDefenceSkillCode[ARRAY_SKILL_KEY_SIZE];	//방어 스킬 선택



	CSetting()
	{
		memset(this, 0, sizeof(CSetting));
	}
};



#pragma pack(push,4)

class CMission
{
public:
	int					m_nSeq;		// 미션코드
	int					m_nCount;	// 보상조건 카운트
	int					m_nReward;  // 보상
	char				m_nKind;	// OBJECT_KIND_POINT 또는 OBJECT_KIND_EXP
};

/*
//능력치 관련 클래스
class CFaculty
{
public:
	char		m_nRun;			//달리기
	char		m_nDribble;		//드리블
	char		m_nQuick;		//순발력

	char		m_nStamina;		//근력
	char		m_nElasticity;	//탄력
	char		m_nBodycheck;	//몸싸움

	char		m_nCross;		//크로스
	char		m_nShortpass;	//숏패스
	char		m_nLongpass;	//롱패스

	char		m_nHeading;		//헤딩
	char		m_nShortshoot;	//단걸슛
	char		m_nLongshoot;	//중걸슛

	char		m_nKeeping;		//키핑
	char		m_nTackle;		//태클
	char		m_nSteal;		//스틸

	char		m_nCatch;		//캐치
	char		m_nPunch;		//펀치
	char		m_nGuard;		//수문

	char		m_nMove;		//이동능력
	char		m_nBody;		//신체능력
	char		m_nPass;		//패스능력
	char		m_nShoot;		//슈팅능력
	char		m_nDefense;		//방어능력
	char		m_nGoalkeep;	//골키핑능력

	char		m_nTalent;		//재능
};

//기록 관련 클래스
class CRecord
{
public:
	int			m_nMatch;
	int			m_nWin;
	int			m_nDraw;
	int			m_nMvp;
	int			m_nGoal;
	int			m_nAssist;
	int			m_nCut;
	int			m_nShoot;
	int			m_nSteal;
	int			m_nTackle;
	int			m_nPass;
	int			m_nTryShoot;
	int			m_nTrySteal;
	int			m_nTryTackle;
	int			m_nTryPass;
	int			m_nMark;
};

//랭킹 관련 클래스
class CRanking
{
public:
	short		m_nMatch;
	short		m_nWin;
	short		m_nWinPoint; //승점
	short		m_nMvp;
	short		m_nGoal;
	short		m_nAssist;
	short		m_nCut;
	short		m_nShoot;
	short		m_nSteal;
	short		m_nTackle;
	short		m_nPass;
	short		m_nGoalAverage;
	short		m_nAssistAverage;
	short		m_nCutAverage;
	short		m_nShootAverage;
	short		m_nStealAverage;
	short		m_nTackleAverage;
	short		m_nPassAverage;
	short		m_nMarkAverage;
	short		m_nWinRate;
	short		m_nShootRate;
	short		m_nStealRate;
	short		m_nTackleRate;
	short		m_nPassRate;
	short		m_nMark;
};

//결과 관련 클래스
class CResult
{
public:
	short		m_nWin;
	short		m_nGoal;
	short		m_nAssist;
	short		m_nCut;
	short		m_nShoot;
	short		m_nSteal;
	short		m_nTackle;
	short		m_nPass;
	short		m_nTryShoot;
	short		m_nTrySteal;
	short		m_nTryTackle;
	short		m_nTryPass;
	short		m_nMark;
	short		m_nService;
	short		m_nPossession;
};
*/

//개인 결과 관련 클래스
class CEachResult
{
public:
	int			m_nPlayerSeq;
	char		m_nTeam;
	short		m_nLevel;
	char		m_nPosition;
	char		m_sName[PLAYER_NAME_SIZE];
	int			m_nExp;
	int			m_nPoint;
	char		m_nIsMvp;
	char		m_nIsExpItem;
	char		m_nIsPointItem;
	char		m_nIsGoldenTime;
	char		m_nIsNumber;
	char		m_nIsEvent;
	char		m_nIsLevelUp;
	CResult		m_cPlayerResult;
};

//예약 포지션 관련 클래스
class CReserveSeat
{
public:
	char		m_nReservePosition[TEAM_SIZE];		//예약 포지션
	char		m_nUsingPosition[TEAM_SIZE];		//자리잡은 포지션
	int			m_nPlayerSeq[TEAM_SIZE];			//자리잡은 플레이어
};


//구매 로그 관련 클래스
class CSale
{
public:
	char		m_nObjectKind;
	char		m_nSaleKind;
	int			m_nObjectSeq;
	int			m_nObjectCode;
	int			m_nPrice;
	int			m_nAmount;
};

//보상 내역 관련 클래스
class CGift
{
public:
	char		m_nKind;	//enum OBJECT_KIND
	int			m_nCode;    //아이템코드(포인트만 주는경우는 0이다)
	int			m_nAmount;  //갯수
};

//아이템 정보 관련//////////////////////////////////////////////////////////////////
//아이템 관련 클래스

//리턴 정보 관련////////////////////////////////////////////////////////////////
class CTeamSeat
{
public:
	char		m_nTeam;
	char		m_nSeat;
};


//////////////////////////////////////////////////////////////////////////////////
// 기본 패킷
class CHeadPacket
{ public:
	const int		m_nCommand;
	int				m_nBodySize;
	int				m_nSequence;
	CHeadPacket() : m_nCommand(0) {}
	CHeadPacket( int nCommand ) : m_nCommand(nCommand)
	{
		m_nBodySize = 0;
		m_nSequence = 0;
	}
};
//////////////////////////////////////////////////////////////////////////////////
// 스케쥴 : 골든타임등
class CScheduleData
{ public:
	char m_nTimeType;
	char m_nDate;
	int  m_nServerCode;
	int  m_nTimeSeq;
	int  m_nStart;
	int  m_nEnd;
};

////////////////////////////////////////////////////////////////////////////////////
// 날씨 구조체
class CWeatherData
{ public:
	char m_nWeatherType;
	char m_nDate;
	int  m_nWeatherSeq;
	int  m_nStart;
	int  m_nEnd;
};

////////////////////////////////////////////////////////////////////////////////////
// 사용자 로그 구조체
class CPlayLog
{ public:
	int	 m_nExp;
	int  m_nMatch;
	int  m_nPoint;
	int  m_nCash;
	int  m_nDate;

	CPlayLog() { m_nExp = 0;m_nMatch = 0; m_nPoint = 0; m_nCash = 0;  m_nCash = 0; m_nDate = 0; }
};



// String Array (CYG)
typedef std::map<int, char*> CStringArray;
typedef std::map<int, CScheduleData*> CScheduleArray;
typedef std::map<int, CWeatherData*> CWeatherArray;
#endif
