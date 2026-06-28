#ifndef	__REPLAY_PACKET_H__
#define __REPLAY_PACKET_H__

#include <vector>
#include "ComDefine.h"
#include "PacketForm.h"

using std::vector;

class CReplayData;

typedef vector< CReplayData* >				VectorReplayList;
typedef vector< CReplayData* >::iterator	IteratorReplayList;

class CReplayData
{
public:
	int					m_nIndex;
	float				m_fTimeStamp;
	int					m_nBodySize;
	char				m_sPacket[PACKET_SIZE];
};

class CReplayPlayer
{
public:
	int					m_nPlayerSeq;
	char				m_nLevel;
	char				m_nPosition;
	char				m_nForm;
	short				m_nGoal;
	short				m_nAssist;
	short				m_nMark;
	char				m_sName[PLAYER_NAME_SIZE];
};

class CReplayReview
{
public:
	char				m_nVersion;
	short 				m_nGroundCode;
	short 				m_nBallCode;
	char 				m_nScaleCode;
	char				m_nAttackTeam;
	char				m_nHomeScore;
	char				m_nAwayScore;

	COnePlayer			m_cMvpPlayer;
	CReplayPlayer		m_cHomePlayer[TEAM_SIZE];
	CReplayPlayer		m_cAwayPlayer[TEAM_SIZE];
};

class CReplayRoomInfo
{
public:
	char				m_nVersion;
	int					m_nQuestCode;
	short 				m_nGroundCode;
	short 				m_nBallCode;
	char				m_nTimeCode;
	char				m_nWeatherCode;
	char				m_nAttackCode;
	char 				m_nScaleCode;
	char				m_nAICode;
	char				m_nAttackTeam;
	char				m_nHomeScore;
	char				m_nAwayScore;

	CReserveSeat		m_cHomeSeat;
	CReserveSeat		m_cAwaySeat;
	COnePlayer			m_cMvpPlayer;
};

class CReplayAthleteInfo
{
public:
	int					m_nPlayerSeq;
	char				m_nPosition;
	char				m_nTeam;
	char				m_nSeat;
	char				m_nForm;
	CLevel				m_cLevel;
	CShape				m_cShape;
	int					m_nEquipWear[MAX_EQUIP];
	int					m_nHomeWear[4];
	int					m_nAwayWear[4];
	char				m_sName[PLAYER_NAME_SIZE];
	short				m_nGoal;
	short				m_nAssist;
	short				m_nMark;
};

class CReplay
{
public:
	FILE*				m_pFileFd;
	char				m_sSaveFile[MAX_PATH];

	//리플레이 헤더
	CReplayRoomInfo		m_cRoomInfo;
	CReplayAthleteInfo	m_cAthleteInfo[TEAM_SIZE * 2];

	VectorReplayList	m_vReplayList;
	int					m_nCurrent;
	int					m_nMark;
	IteratorReplayList	m_iMark;

public:
	CReplay();
	~CReplay() {};

	void				InitReplayData();
	void				SaveReplayHeader();
	void				InsertReplayData(float fTime, CHeadPacket* pPacket);
	float				SetReplayIterator(float fStampTime);
	bool				RunReplayData(float fStampTime);
	void				RewindReplayIterator(float fOffsetTime);
	void				ForwardReplayIterator(float fOffsetTime);

	void				WriteReplayFile();
	void				ReadReplayFile(const char *FileName);
	CReplayReview		ReadReplayReview(const char *FileName);
};

#endif