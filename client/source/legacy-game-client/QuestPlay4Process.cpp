#include "stdafx.h"
#include "QuestPlay4Process.h"
#include "PutPacket.h"

CQuestPlay4Process::CQuestPlay4Process() : CQuestPlayProcess()
{
}

CQuestPlay4Process::~CQuestPlay4Process(void)
{
}

bool CQuestPlay4Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CQuestPlay4Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;

	if(!bSuccess && m_nTrySuccessCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_LONGPASS] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_SHORTPASS])
	{
		nTime = timeGetTime();
		bSuccess = true;
	}

	if(bSuccess && timeGetTime() - nTime > 1000)
	{
		m_nTrySuccessCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_LONGPASS] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_SHORTPASS];
		m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay4Process::FailProcess()
{
	m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}