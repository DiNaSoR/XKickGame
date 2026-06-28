#include "stdafx.h"
#include "QuestPlay5Process.h"
#include "PutPacket.h"

CQuestPlay5Process::CQuestPlay5Process() : CQuestPlayProcess()
{
}

CQuestPlay5Process::~CQuestPlay5Process(void)
{
}

bool CQuestPlay5Process::TryProcess()
{
	CObjPlayerInfo *pWaypointPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOWAYPOINTATTACK);
	assert(pWaypointPlayer);
	if(m_nTryTotalCount < pWaypointPlayer->GetWaypointCount() + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_nCommand = 0;
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CQuestPlay5Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;

	if(!bSuccess && m_nTrySuccessCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE])
	{
		nTime = timeGetTime();
		bSuccess = true;
	}

	if(bSuccess && timeGetTime() - nTime > 1000)
	{
		CObjPlayerInfo *pWaypointPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOWAYPOINTATTACK);
		assert(pWaypointPlayer);
		m_nTrySuccessCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE];
		m_nTryTotalCount = pWaypointPlayer->GetWaypointCount() + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE];
		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay5Process::FailProcess()
{
	CObjPlayerInfo *pWaypointPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOWAYPOINTATTACK);
	assert(pWaypointPlayer);
	m_nTryTotalCount = pWaypointPlayer->GetWaypointCount() + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE];
}