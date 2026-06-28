#include "stdafx.h"
#include "QuestPlay6Process.h"
#include "PutPacket.h"
#include "UIFunction.h"

CQuestPlay6Process::CQuestPlay6Process() : CQuestPlayProcess()
{
}

CQuestPlay6Process::~CQuestPlay6Process(void)
{
}

bool CQuestPlay6Process::TryProcess()
{
	static DWORD nTime = timeGetTime();
	static bool bTry = false;
	if(!bTry && CClientUIManager::GetPtr()->FindEventControl("Static_Start")->GetVisible() == N3UI_VISIBLE_FALSE )
	{
		UIFunction::Set_UIVisibility("Static_RNumber0", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);
		bTry = true;
	}
	if(bTry && CClientUIManager::GetPtr()->FindEventControl("Static_RNumber0")->GetVisible() == N3UI_VISIBLE_FALSE)
	{
		CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOCROSSATTACK);

		pCrossPlayer->m_cInputKey.m_nCommand = KEY_LONGPASS;
		pCrossPlayer->m_cInputKey.m_nPower = 70;
		nTime = timeGetTime() + 3000;
		bTry = false;
		return true;
	}
	return false;
}

bool CQuestPlay6Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;

	if(!bSuccess && m_nTrySuccessCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_CLEAR])
	{
		nTime = timeGetTime();
		m_nTime = timeGetTime();//성공이므로 실패조건 피해가기 위해서
		bSuccess = true;
	}

	if(bSuccess && timeGetTime() - nTime > 400)
	{
		CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOCROSSATTACK);

		m_nTrySuccessCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_CLEAR];
		m_nTryTotalCount = pCrossPlayer->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay6Process::FailProcess()
{
	CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOCROSSATTACK);
	if(pCrossPlayer == NULL) return;

	m_nTryTotalCount = pCrossPlayer->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}