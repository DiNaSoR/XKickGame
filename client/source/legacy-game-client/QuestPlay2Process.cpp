#include "stdafx.h"
#include "QuestPlay2Process.h"
#include "PutPacket.h"

CQuestPlay2Process::CQuestPlay2Process() : CQuestPlayProcess()
{
	m_bChangeShootingZoon = false;
	m_pkShootingZoon = NULL;
	m_nTutorialPartKey2 = KEY_EMPTY;
}

CQuestPlay2Process::~CQuestPlay2Process(void)
{
}

void CQuestPlay2Process::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
	CQuestPlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);
	ChangeShootingZoon();
}

void CQuestPlay2Process::InputKey()
{
	CBasePlayProcess::InputKey();

	if(m_pSelectPlayerInfo->m_cInputKey.m_nCommand != KEY_EMPTY)
	{
		m_nTutorialPartKey2 = m_pSelectPlayerInfo->m_cInputKey.m_nCommand;
	}
}

bool CQuestPlay2Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CQuestPlay2Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;

	if(!bSuccess && CheckShootingZoon())
	{
		if( (m_bChangeShootingZoon && m_nTutorialPartKey2 == KEY_SHOOT2) || (!m_bChangeShootingZoon && m_nTutorialPartKey2 == KEY_SHOOT1))
		{
			m_nTutorialPartKey2 = KEY_EMPTY;
			nTime = timeGetTime();
			bSuccess = true;
		}
	}
	if(bSuccess && timeGetTime() - nTime > 1000)
	{
		m_nTrySuccessCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT];
		m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay2Process::FailProcess()
{
	m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}

//튜토리얼 1에 관한 내용
void CQuestPlay2Process::ChangeShootingZoon()
{
	m_bChangeShootingZoon = !m_bChangeShootingZoon;

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	if(!m_pkShootingZoon)
	{
		m_pkShootingZoon = pObjGroundInfo->GetObjectByName("F");
		m_pkShootingZoon->SetAppCulled(true);	
		m_pkShootingZoon = pObjGroundInfo->GetObjectByName("D");
		m_pkShootingZoon->SetAppCulled(false);	
	}

	if(m_bChangeShootingZoon)
	{
		m_pkShootingZoon->SetAppCulled(true);
		m_pkShootingZoon = pObjGroundInfo->GetObjectByName("F");
		m_pkShootingZoon->SetAppCulled(false);	
	}
	else
	{
		m_pkShootingZoon->SetAppCulled(true);
		m_pkShootingZoon = pObjGroundInfo->GetObjectByName("D");
		m_pkShootingZoon->SetAppCulled(false);
	}
}
//튜토리얼 1에 관한 내용
bool CQuestPlay2Process::CheckShootingZoon()
{
	if(m_pkShootingZoon) 
	{
		NiAVObject* pkGroundArea = m_pkShootingZoon->GetObjectByName("GroundArea");

		NiPoint3 cPoint = m_pSelectPlayerInfo->GetFactPoint();

		if(CCollisionDetector::PickObject(pkGroundArea, cPoint - NiPoint3(0,0,1), cPoint, NULL, NULL, NULL))
		{
			return true;
		}
	}
	return false;
}