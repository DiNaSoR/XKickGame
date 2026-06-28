#include "stdafx.h"
#include "ReplayPacket.h"
#include "ObjBallManager.h"
#include "FblArea.h"
#include "FblFunction.h"
#include "GetPacket.h"

CReplay::CReplay()
{
	memset(&m_cRoomInfo, 0, sizeof(m_cRoomInfo));
}

void CReplay::InitReplayData()
{
	IteratorReplayList	iReplay;
	CReplayData*		pData;
	int					i;

	memset(m_sSaveFile, 0, MAX_PATH);

	//데이터 초기화//////////////////////////////////////////////////////////////

	for(i=0;i<(TEAM_SIZE*2);++i)
	{
		m_cAthleteInfo[i].m_nPlayerSeq = 0;
	}

	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		delete pData;
		//m_vReplayList.erase(iReplay++);
	}
	m_vReplayList.clear();

	m_nCurrent	= 0;
	m_nMark		= 0;

	CObjPlayerManager::GetPtr()->InitReplayReady(); //모든 선수들의 리플레이 여부 초기화

	char sCur_Time[STRLEN_32]="";

	GetCurDateTime(sCur_Time, "%Y%m%d_%H%M%S");
	
	_snprintf(m_sSaveFile, MAX_PATH, "./Replay/XKickReplay%s.xkr", sCur_Time);
}

void CReplay::SaveReplayHeader()
{
	//룸정보 기록
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	m_cRoomInfo.m_nVersion			= 1;
	m_cRoomInfo.m_nQuestCode		= pRoom->m_nQuestCode;
	m_cRoomInfo.m_nGroundCode		= pRoom->m_nGroundCode;
	m_cRoomInfo.m_nBallCode			= pRoom->m_nBallCode;
	m_cRoomInfo.m_nTimeCode			= pRoom->m_nTimeCode;
	m_cRoomInfo.m_nWeatherCode		= pRoom->m_nWeatherCode;
	m_cRoomInfo.m_nAttackCode		= pRoom->m_nAttackCode;
	m_cRoomInfo.m_nScaleCode		= pRoom->m_nScaleCode;
	m_cRoomInfo.m_nAICode			= pRoom->m_nAICode;
	m_cRoomInfo.m_nAttackTeam		= pRoom->m_nAttackTeam;

	m_cRoomInfo.m_cMvpPlayer		= pRoom->m_cMvpPlayer;
	m_cRoomInfo.m_cHomeSeat			= pRoom->m_cHomeSeat;
	m_cRoomInfo.m_cAwaySeat			= pRoom->m_cAwaySeat;

	//선수 정보 기록
	CObjPlayerInfo*		pAnyPlayer	= NULL;
	VectorAnyoneList*	pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_ATHLETE);

	memset(&m_cAthleteInfo, 0, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2));
	int i = 0;
	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			m_cAthleteInfo[i].m_nPlayerSeq				= pAnyPlayer->GetObjSeq();
			m_cAthleteInfo[i].m_nPosition				= pAnyPlayer->m_nPosition;
			m_cAthleteInfo[i].m_nTeam					= pAnyPlayer->m_nTeam;
			m_cAthleteInfo[i].m_nSeat					= pAnyPlayer->m_nSeat;
			m_cAthleteInfo[i].m_nForm					= pAnyPlayer->m_nForm;

			memcpy(&m_cAthleteInfo[i].m_cLevel,			&pAnyPlayer->m_cLevel,				sizeof(CLevel));
			memcpy(&m_cAthleteInfo[i].m_cShape,			&pAnyPlayer->m_cShape,				sizeof(CShape));
			memcpy(&m_cAthleteInfo[i].m_nEquipWear,		&pAnyPlayer->m_nEquipWear,			sizeof(int)*MAX_EQUIP);
			memcpy(&m_cAthleteInfo[i].m_nHomeWear,		&pAnyPlayer->m_nHomeWear,			sizeof(int)*4);
			memcpy(&m_cAthleteInfo[i].m_nAwayWear,		&pAnyPlayer->m_nAwayWear,			sizeof(int)*4);
			_snprintf(m_cAthleteInfo[i].m_sName, PLAYER_NAME_SIZE, pAnyPlayer->m_sName);

			++i;
		}
	}
}

void CReplay::InsertReplayData(float fTime, CHeadPacket* pPacket)
{
	CReplayData*	pData = new CReplayData;

	if(pData == NULL)
		return;

	pData->m_nIndex		= m_nCurrent;
	pData->m_fTimeStamp	= fTime;
	pData->m_nBodySize	= HEAD_SIZE+pPacket->m_nBodySize;
	memcpy(pData->m_sPacket, pPacket, HEAD_SIZE+pPacket->m_nBodySize);

	m_vReplayList.push_back(pData);

	++m_nCurrent;
}

float CReplay::SetReplayIterator(float fStampTime)
{
	CHeadPacket*		pPacket;
	IteratorReplayList	iReplay;
	CReplayData*		pData;
	float				fTime;
	bool				bBallAction = false;

	fTime = fStampTime - 8.0f;//8.0f; //리플레이 저장시간
	if(fTime < 0.0f)
		fTime = 0.0f;

	//이전 마지막 리플레이 위치와 시간 조건이 맞는 곳 검색(개략적 검색)
	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;

		//프로토콜은 미리 업데이트 해논다
		if(pPacket->m_nCommand == CM_OBJECT_ACTION)
		{
			GetObjectAction_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_BALL_ACTION)
		{
			GetBallAction((CHeadPacket*)pData->m_sPacket);
			bBallAction = true;
		}

		if(pData->m_nIndex < m_nMark)
			continue;

		if(pData->m_fTimeStamp > fTime)
		{
			CObjPlayerManager::GetPtr()->SetAthletePosition(); //모든 선수들의 위치를 재세팅
			m_nMark	= pData->m_nIndex;
			m_iMark = iReplay;
			return pData->m_fTimeStamp;
		}
	}
	return fStampTime;
}
/*
float CReplay::SetReplayIterator(float fStampTime)
{
	CHeadPacket*		pPacket;
	IteratorReplayList	iReplay;
	CReplayData*		pData;
	float				fTime;
	bool				bBallAction = false;

	fTime = fStampTime - 10.0f;
	if(fTime < 0.0f)
		fTime = 0.0f;

	CObjPlayerManager::GetPtr()->InitReplayReady(); //모든 선수들의 리플레이 여부 초기화

	//이전 마지막 리플레이 위치와 시간 조건이 맞는 곳 검색(개략적 검색)
	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		if(pData->m_nIndex < m_nMark)
			continue;

		if(pData->m_fTimeStamp > fTime)
		{
			m_iMark = iReplay;
			break;
		}
	}

	//개략적 검색으로 부터 모든 프로토콜을 읽을 수 있는 곳 검색(세부적 검색)
	for(iReplay=m_iMark;iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;

		printf("replay index(%d), timestamp(%f)\n", pData->m_nIndex, pData->m_fTimeStamp);
		//프로토콜은 미리 업데이트 해논다
		if(pPacket->m_nCommand == CM_OBJECT_ACTION)
		{
			GetObjectAction_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_BALL_ACTION)
		{
			GetBallAction((CHeadPacket*)pData->m_sPacket);
			bBallAction = true;
		}

		if((CObjPlayerManager::GetPtr()->IsReplayReady()) && (bBallAction)) //모든 프로토콜을 한번씩 읽었다면 거기부터 시작
		{
			CObjPlayerManager::GetPtr()->SetAthletePosition(); //모든 선수들의 위치를 재세팅

			m_nMark	= pData->m_nIndex;
			m_iMark = iReplay;
			return pData->m_fTimeStamp;
		}
	}

	return fStampTime;
}
*/

bool CReplay::RunReplayData(float fStampTime)
{
	CHeadPacket*		pPacket;
	CReplayData*		pData;
	IteratorReplayList	iReplay;
	IteratorReplayList	iTemp;
	CObjBallInfo*		pBall	= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	iTemp = m_iMark;
	for(iReplay=iTemp;iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		break;

		if(pData->m_fTimeStamp > fStampTime)
		{
			m_iMark = iReplay;
			m_nMark = pData->m_nIndex;
			return true;
		}
		else
		{
			pPacket = (CHeadPacket*)pData->m_sPacket;
			if(pPacket->m_nCommand == CM_OBJECT_ACTION)
				GetObjectAction((CHeadPacket*)pData->m_sPacket);
			else if(pPacket->m_nCommand == CM_BALL_ACTION)
			{
				//공이 경기장 밖(골대안)에 있을 경우는 시간 보정하지 않는다(그물효과에 문제 발생)
				if((pBall->m_cCurrentPoint.x > CFblArea::GetPtr()->m_fGroundX) || (pBall->m_cCurrentPoint.x < -CFblArea::GetPtr()->m_fGroundX))
					GetBallAction((CHeadPacket*)pData->m_sPacket);
				else
					GetBallAction_Replay((CHeadPacket*)pData->m_sPacket, pData->m_fTimeStamp-fStampTime);
			}
			else if((pPacket->m_nCommand == CM_GOAL_IN) && (pRoom->m_nMode == ROOM_MODE_REPLAY))
				GetGoalIn((CHeadPacket*)pData->m_sPacket);
			else if((pPacket->m_nCommand == CM_GOALIN_TCP) && (pRoom->m_nMode == ROOM_MODE_REPLAY))
				GetGoalinTcp((CHeadPacket*)pData->m_sPacket);
		}
	}
	++m_nMark; //마지막으로 플레이한 프로토콜의 다음 번호로 지정하므로써 다음 플레이시 거기부터 플레이되는 것을 피한다
	return false;
}

void CReplay::RewindReplayIterator(float fOffsetTime)
{
	CHeadPacket*		pPacket;
	IteratorReplayList	iReplay;
	CReplayData*		pData;
	float				fTime;
	bool				bBallAction = false;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	fTime = pRoom->m_fStampTime - fOffsetTime;
	if(fTime < 0.0f)
		fTime = 0.0f;

	pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]	= 0;
	pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]	= 0;
	//이전 마지막 리플레이 위치와 시간 조건이 맞는 곳 검색(개략적 검색)
	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;
		if(pPacket->m_nCommand == CM_GOAL_IN) //점수 추적용
		{
			GetGoalIn_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_GOALIN_TCP) //점수 추적용
		{
			GetGoalinTcp_Replay((CHeadPacket*)pData->m_sPacket);
		}

		if(pData->m_fTimeStamp > fTime)
		{
			m_iMark = iReplay;
			break;
		}
	}

	//개략적 검색으로 부터 모든 프로토콜을 읽을 수 있는 곳 검색(세부적 검색)
	for(iReplay=m_iMark;iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;

		//프로토콜은 미리 업데이트 해논다
		if(pPacket->m_nCommand == CM_OBJECT_ACTION)
		{
			GetObjectAction_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_BALL_ACTION)
		{
			GetBallAction((CHeadPacket*)pData->m_sPacket);
			bBallAction = true;
		}

		if((CObjPlayerManager::GetPtr()->IsReplayReady()) && (bBallAction)) //모든 프로토콜을 한번씩 읽었다면 거기부터 시작
		{
			CObjPlayerManager::GetPtr()->InitReplayReady(); //모든 선수들의 리플레이 여부 초기화
			CObjPlayerManager::GetPtr()->SetAthletePosition(); //모든 선수들의 위치를 재세팅

			m_nMark	= pData->m_nIndex;
			m_iMark = iReplay;
			pRoom->m_fStampTime = pData->m_fTimeStamp;
			return;
		}
	}
}

void CReplay::ForwardReplayIterator(float fOffsetTime)
{
	CHeadPacket*		pPacket;
	IteratorReplayList	iReplay;
	CReplayData*		pData;
	float				fTime;
	bool				bBallAction = false;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	fTime = pRoom->m_fStampTime + fOffsetTime;
	if(fTime < 0.0f)
		fTime = 0.0f;

	//이전 마지막 리플레이 위치와 시간 조건이 맞는 곳 검색(개략적 검색)
	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;
		if(pPacket->m_nCommand == CM_GOAL_IN) //점수 추적용
		{
			GetGoalIn_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_GOALIN_TCP) //점수 추적용
		{
			GetGoalinTcp_Replay((CHeadPacket*)pData->m_sPacket);
		}


		if(pData->m_fTimeStamp > fTime)
		{
			m_iMark = iReplay;
			break;
		}
	}

	//개략적 검색으로 부터 모든 프로토콜을 읽을 수 있는 곳 검색(세부적 검색)
	for(iReplay=m_iMark;iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;

		//프로토콜은 미리 업데이트 해논다
		if(pPacket->m_nCommand == CM_OBJECT_ACTION)
		{
			GetObjectAction_Replay((CHeadPacket*)pData->m_sPacket);
		}
		else if(pPacket->m_nCommand == CM_BALL_ACTION)
		{
			GetBallAction((CHeadPacket*)pData->m_sPacket);
			bBallAction = true;
		}

		if((CObjPlayerManager::GetPtr()->IsReplayReady()) && (bBallAction)) //모든 프로토콜을 한번씩 읽었다면 거기부터 시작
		{
			CObjPlayerManager::GetPtr()->InitReplayReady(); //모든 선수들의 리플레이 여부 초기화
			CObjPlayerManager::GetPtr()->SetAthletePosition(); //모든 선수들의 위치를 재세팅

			m_nMark	= pData->m_nIndex;
			m_iMark = iReplay;
			pRoom->m_fStampTime = pData->m_fTimeStamp;
			return;
		}
	}
	iReplay = m_vReplayList.end();
}

void CReplay::WriteReplayFile()
{
	m_pFileFd = fopen(m_sSaveFile, "wb+");
	if(m_pFileFd == NULL)	return;

	//룸 정보 쓰기
	fwrite((char *)&m_cRoomInfo, sizeof(CReplayRoomInfo), 1, m_pFileFd);

	//선수 정보 쓰기
	fwrite((char *)&m_cAthleteInfo, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2), 1, m_pFileFd);

	//프로토콜 정보 쓰	기
	CHeadPacket*		pPacket;
	CReplayData*		pData;
	IteratorReplayList	iReplay;

	for(iReplay=m_vReplayList.begin();iReplay!=m_vReplayList.end();++iReplay)
	{
		pData = (*iReplay);
		if(pData == NULL)		continue;

		pPacket = (CHeadPacket*)pData->m_sPacket;
		fwrite((char *)pData, pData->m_nBodySize+12, 1, m_pFileFd);
	}

	fflush(m_pFileFd);
	fclose(m_pFileFd);
}

void CReplay::ReadReplayFile(const char *FileName)
{
	CHeadPacket*		pPacket;
	CReplayData*		pData;
	char				sHeader[100];
	char				sBuffer[PACKET_SIZE];
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pPlayer;

	_snprintf(m_sSaveFile, MAX_PATH, FileName);

	m_pFileFd = fopen(m_sSaveFile, "rb");
	if(m_pFileFd == NULL)	return;

	//룸 정보 읽기
	fread(sBuffer, sizeof(CReplayRoomInfo), 1, m_pFileFd);
	memcpy(&m_cRoomInfo, sBuffer, sizeof(CReplayRoomInfo));

	pRoom->m_nQuestCode		= m_cRoomInfo.m_nQuestCode;
	pRoom->m_nGroundCode	= m_cRoomInfo.m_nGroundCode;
	pRoom->m_nBallCode		= m_cRoomInfo.m_nBallCode;
	pRoom->m_nTimeCode		= m_cRoomInfo.m_nTimeCode;
	pRoom->m_nWeatherCode	= m_cRoomInfo.m_nWeatherCode;
	pRoom->m_nAttackCode	= m_cRoomInfo.m_nAttackCode;
	pRoom->m_nScaleCode		= m_cRoomInfo.m_nScaleCode;
	pRoom->m_nAICode		= m_cRoomInfo.m_nAICode;
	pRoom->m_nAttackTeam	= m_cRoomInfo.m_nAttackTeam;	

	memcpy(&pRoom->m_cHomeSeat, &m_cRoomInfo.m_cHomeSeat, sizeof(CReserveSeat));
	memcpy(&pRoom->m_cAwaySeat, &m_cRoomInfo.m_cAwaySeat, sizeof(CReserveSeat));

	//선수 정보 읽기
	fread(sBuffer, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2), 1, m_pFileFd);
	memcpy(&m_cAthleteInfo, sBuffer, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2));

	for(int i=0;i<(TEAM_SIZE*2);++i)
	{
		if(m_cAthleteInfo[i].m_nPlayerSeq != 0)
		{
			pPlayer = new CObjPlayerInfo;
			if(pPlayer == NULL)		return;

			pPlayer->InitPlayerInRoom(PLAYER_KIND_USER);
			pPlayer->SetObjSeq( m_cAthleteInfo[i].m_nPlayerSeq);

			pPlayer->m_nPosition			= m_cAthleteInfo[i].m_nPosition;
			pPlayer->m_nTeam				= m_cAthleteInfo[i].m_nTeam;
			pPlayer->m_nSeat				= m_cAthleteInfo[i].m_nSeat;
			pPlayer->m_cLevel				= m_cAthleteInfo[i].m_cLevel;
			pPlayer->m_cShape				= m_cAthleteInfo[i].m_cShape;

			memcpy(&pPlayer->m_nEquipWear,	&m_cAthleteInfo[i].m_nEquipWear,	sizeof(int)*MAX_EQUIP);
			memcpy(&pPlayer->m_nHomeWear,	&m_cAthleteInfo[i].m_nHomeWear,		sizeof(int)*4);
			memcpy(&pPlayer->m_nAwayWear,	&m_cAthleteInfo[i].m_nAwayWear,		sizeof(int)*4);
			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, m_cAthleteInfo[i].m_sName);

			CObjPlayerManager::GetPtr()->SetPlayerLists(pPlayer, pPlayer->m_nTeam);
		}
	}

	//프로토콜 정보 읽기 (큐에 저장)
	m_nCurrent	= 0;
	m_nMark		= 0;
	while(1)
	{
		memset(sHeader, 0, sizeof(sHeader));
		if(fread(sHeader, 12, 1, m_pFileFd) == 0) //파일의 끝
			break;
		pData = (CReplayData*)sHeader;

		memset(sBuffer, 0, sizeof(sBuffer));
		fread(sBuffer, pData->m_nBodySize, 1, m_pFileFd);
		pPacket = (CHeadPacket*)sBuffer;

		InsertReplayData(pData->m_fTimeStamp, pPacket);
	}
	m_iMark = m_vReplayList.begin();

	fflush(m_pFileFd);
	fclose(m_pFileFd);
}

CReplayReview CReplay::ReadReplayReview(const char *FileName)
{
	CReplayReview		cReview;

	memset(&cReview, 0, sizeof(cReview));

	CReplayRoomInfo		cRoomInfo;
	CReplayAthleteInfo	cAthleteInfo[TEAM_SIZE * 2];
	char				sBuffer[PACKET_SIZE];

	cReview.m_nVersion		= 0;
	m_pFileFd = fopen(FileName, "rb");
	if(m_pFileFd == NULL)	return cReview;

	//룸 정보 읽기
	fread(sBuffer, sizeof(CReplayRoomInfo), 1, m_pFileFd);
	memcpy(&cRoomInfo, sBuffer, sizeof(CReplayRoomInfo));

	cReview.m_nVersion		= cRoomInfo.m_nVersion;
	cReview.m_nGroundCode	= cRoomInfo.m_nGroundCode;
	cReview.m_nBallCode		= cRoomInfo.m_nBallCode;
	cReview.m_nScaleCode	= cRoomInfo.m_nScaleCode;
	cReview.m_nAttackTeam	= cRoomInfo.m_nAttackTeam;
	cReview.m_nHomeScore	= cRoomInfo.m_nHomeScore;
	cReview.m_nAwayScore	= cRoomInfo.m_nAwayScore;
	cReview.m_cMvpPlayer	= cRoomInfo.m_cMvpPlayer;
	
	//선수 정보 읽기
	fread(sBuffer, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2), 1, m_pFileFd);
	memcpy(&cAthleteInfo, sBuffer, sizeof(CReplayAthleteInfo) * (TEAM_SIZE*2));

	int h = 0;
	int a = 0;
	for(int i=0;i<(TEAM_SIZE*2);++i)
	{
		if(cAthleteInfo[i].m_nPlayerSeq != 0)
		{
			if(cAthleteInfo[i].m_nTeam == PLAYER_TEAM_HOME)
			{
				cReview.m_cHomePlayer[h].m_nPlayerSeq	= cAthleteInfo[i].m_nPlayerSeq;
				cReview.m_cHomePlayer[h].m_nLevel		= cAthleteInfo[i].m_cLevel.m_nLevel;
				cReview.m_cHomePlayer[h].m_nPosition	= cAthleteInfo[i].m_nPosition;
				cReview.m_cHomePlayer[h].m_nForm		= cAthleteInfo[i].m_nForm;
				cReview.m_cHomePlayer[h].m_nGoal		= cAthleteInfo[i].m_nGoal;
				cReview.m_cHomePlayer[h].m_nAssist		= cAthleteInfo[i].m_nAssist;
				cReview.m_cHomePlayer[h].m_nMark		= cAthleteInfo[i].m_nMark;
				_snprintf(cReview.m_cHomePlayer[h].m_sName, PLAYER_NAME_SIZE, cAthleteInfo[i].m_sName);
				++h;
			}
			else
			{
				cReview.m_cAwayPlayer[a].m_nPlayerSeq	= cAthleteInfo[i].m_nPlayerSeq;
				cReview.m_cAwayPlayer[a].m_nLevel		= cAthleteInfo[i].m_cLevel.m_nLevel;
				cReview.m_cAwayPlayer[a].m_nPosition	= cAthleteInfo[i].m_nPosition;
				cReview.m_cAwayPlayer[a].m_nForm		= cAthleteInfo[i].m_nForm;
				cReview.m_cAwayPlayer[a].m_nGoal		= cAthleteInfo[i].m_nGoal;
				cReview.m_cAwayPlayer[a].m_nAssist		= cAthleteInfo[i].m_nAssist;
				cReview.m_cAwayPlayer[a].m_nMark		= cAthleteInfo[i].m_nMark;
				_snprintf(cReview.m_cAwayPlayer[a].m_sName, PLAYER_NAME_SIZE, cAthleteInfo[i].m_sName);
				++a;
			}
		}
	}

	fflush(m_pFileFd);
	fclose(m_pFileFd);

	return cReview;
}
