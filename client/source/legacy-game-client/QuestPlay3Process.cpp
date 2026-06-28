#include "stdafx.h"
#include "QuestPlay3Process.h"
#include "PutPacket.h"
#include "UIFunction.h"

CQuestPlay3Process::CQuestPlay3Process() : CQuestPlayProcess()
{
	m_pkShootingZoon = NULL;
}

CQuestPlay3Process::~CQuestPlay3Process(void)
{
}

void CQuestPlay3Process::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CQuestPlayProcess::UpdateStateGameSetting(fFrameTime, fAccumTime);

	FindShootingZoon();
}

bool CQuestPlay3Process::TryProcess()
{
	static DWORD nTime = 0;
	static bool bTry = false;
	if(!bTry && CClientUIManager::GetPtr()->FindEventControl("Static_Start")->GetVisible() == N3UI_VISIBLE_FALSE )
	{
		UIFunction::Set_UIVisibility("Static_RNumber0", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//성공 연출씬 세팅
		bTry = true;
	}
	if(bTry && CClientUIManager::GetPtr()->FindEventControl("Static_RNumber0")->GetVisible() == N3UI_VISIBLE_FALSE)
	{
		CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_HOME, DUTY_TUTOCROSSATTACK);

		pCrossPlayer->m_cInputKey.m_nCommand = KEY_LONGPASS;
		pCrossPlayer->m_cInputKey.m_nPower = 70;
		nTime = timeGetTime() + 3000;
		bTry = false;
		return true;
	}
	return false;
}

bool CQuestPlay3Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;

	if(!bSuccess && m_nTrySuccessCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYDIRECTSHOOT])
	{
		nTime = timeGetTime();
		bSuccess = true;
	}

	if(bSuccess && timeGetTime() - nTime > 400)
	{
		CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_HOME, DUTY_TUTOCROSSATTACK);

		m_nTrySuccessCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYDIRECTSHOOT];

		m_nTryTotalCount = pCrossPlayer->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];

		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay3Process::FailProcess()
{
	CObjPlayerInfo *pCrossPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_HOME, DUTY_TUTOCROSSATTACK);
	if(pCrossPlayer == NULL) return;
	m_nTryTotalCount = pCrossPlayer->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}

void CQuestPlay3Process::FindShootingZoon()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pkShootingZoon = pObjGroundInfo->GetObjectByName("Tutorial_2_Object");
}