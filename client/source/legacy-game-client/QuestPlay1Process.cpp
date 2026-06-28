#include "stdafx.h"
#include "QuestPlay1Process.h"
#include "UICommonMethod.h"
#include "UIProgress.h"
#include "PutPacket.h"
#include "UIFunction.h"

CQuestPlay1Process::CQuestPlay1Process() : CQuestPlayProcess()
{
	m_nQuestType = 10000;
}

CQuestPlay1Process::~CQuestPlay1Process(void)
{
}

void CQuestPlay1Process::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CQuestPlayProcess::UpdateProcess(fFrameTime, fAccumTime);

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow"));
	assert(pProgress);
	CUIProgress* pProgressO = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow_Big"));
	assert(pProgressO);
	pProgressO->SetValue(pProgress->GetValue());
}

void CQuestPlay1Process::FUNCTION_ExplainUIInit()
{
	CQuestPlayProcess::FUNCTION_ExplainUIInit();

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900103, 0));
	assert(pMTable);

	UIFunction::Set_ControlText( "Explain_Text", pMTable->m_sMessage );
}
void CQuestPlay1Process::FUNCTION_KeyUIInit()
{
	CQuestPlayProcess::FUNCTION_KeyUIInit();
}

void CQuestPlay1Process::FUNCTION_DemonstrationGameUIInit()
{
	CQuestPlayProcess::FUNCTION_DemonstrationGameUIInit();

	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_TRUE );

	UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_D1", N3UI_VISIBLE_TRUE );
}

void CQuestPlay1Process::FUNCTION_GamePlayExplainUIInit()
{
	CQuestPlayProcess::FUNCTION_GamePlayExplainUIInit();

	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900104, 0));
	assert(pMTable);

	UIFunction::Set_ControlText( "Explain_Text", pMTable->m_sMessage );
}

void CQuestPlay1Process::FUNCTION_GamePlayUIInit()
{
	CQuestPlayProcess::FUNCTION_GamePlayUIInit();
	FUNCTION_KeyUIInit();
}

void CQuestPlay1Process::FUNCTION_ResultUIInit(bool bSuccess)
{
	CQuestPlayProcess::FUNCTION_ResultUIInit(bSuccess);
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	//if( bSuccess )
	//{
	//	CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, m_nQuestType, 0));
	//	assert(pTable);

	//	char sText[STRLEN_64]="";

	//	_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
	//	UIFunction::Set_ControlText( "Conversation_Text", sText );
	//}
}

bool CQuestPlay1Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_nCommand = 0;
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CQuestPlay1Process::SuccessProcess()
{
	static DWORD nTime = 0;
	static bool	 bSuccess = false;
	if(!bSuccess && 70 <= m_cBackUpInputKey.m_nPower && m_cBackUpInputKey.m_nPower <= 90)
	{
		nTime = timeGetTime();
		m_cBackUpInputKey.m_nPower = 0;
		bSuccess = true;
	}
	if(bSuccess && timeGetTime() - nTime > 1000)
	{
		m_nTrySuccessCount++;
		m_nTryTotalCount++;
		bSuccess = false;
		return true;
	}
	return false;
}

void CQuestPlay1Process::FailProcess()
{
	m_nTryTotalCount++;	
}