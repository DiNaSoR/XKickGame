#include "stdafx.h"
#include "TutorialPlay3Process.h"
#include "PutPacket.h"
#include "UIProgress.h"
#include "UIFunction.h"

CTutorialPlay3Process::CTutorialPlay3Process() : CTutorialPlayProcess()
{
	m_nTutorialType = TUTORIAL_3TYPE;
}

CTutorialPlay3Process::~CTutorialPlay3Process(void)
{
}

void CTutorialPlay3Process::FUNCTION_ExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_ExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent04"  );
	CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
	assert(pShadowLine);

	if( !pShadowLine->IsPlay() && m_bShadow )
	{
		pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, false, 0);
		
		m_bShadow = false;
	}
}
void CTutorialPlay3Process::FUNCTION_KeyUIInit()
{
	CTutorialPlayProcess::FUNCTION_KeyUIInit();
}

void CTutorialPlay3Process::FUNCTION_GamePlayExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent05"  );
	CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
	assert(pShadowLine);

	if( !pShadowLine->IsPlay() && m_bShadow )
	{
		pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, false, 0);
		
		m_bShadow = false;
	}
}

void CTutorialPlay3Process::FUNCTION_DemonstrationGameUIInit()
{
	FUNCTION_KeyUIInit();

	CTutorialPlayProcess::FUNCTION_DemonstrationGameUIInit();

	if( m_bAssistant )
		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
}

void CTutorialPlay3Process::FUNCTION_GamePlayUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayUIInit();
	int nTry = m_nTryTotalCount%2;

	UIFunction::Set_UIVisibility( "Key_S", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_TRUE );
	
	UIFunction::Set_UIVisibility( "Key_S", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_FALSE );
}

void CTutorialPlay3Process::FUNCTION_ResultUIInit(bool bSuccess)
{
	CTutorialPlayProcess::FUNCTION_ResultUIInit(bSuccess);
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	/*if( bSuccess )
	{
		CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 103, 0));
		assert(pTable);

		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
		UIFunction::Set_ControlText( "Conversation_Text", sText );
	}*/
}

bool CTutorialPlay3Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CTutorialPlay3Process::SuccessProcess()
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

void CTutorialPlay3Process::FailProcess()
{
	m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}

void CTutorialPlay3Process::DemonstrationGameInit()
{
	CTutorialPlayProcess::DemonstrationGameInit();

	if(m_bAssistant)
	{
		//공소유 변경
		CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();
		pBall->m_nBallForm = m_pAssistantPlayer->m_nForm;
		m_bTutorialTime = false;
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_STAY)
	{
		//공소유 변경
		CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();
		pBall->m_nBallForm = m_pSelectPlayerInfo->m_nForm;
	}

	CObjBallManager::GetPtr()->InitBallInPlay();
}

void CTutorialPlay3Process::DemonstrationPlay(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::DemonstrationPlay(fFrameTime, fAccumTime);

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 500)
	{
		UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Key_S1", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_TRUE );
	}

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 1000)
	{
		UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_S1", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );

		m_nTutorialTime = timeGetTime();
		m_bTutorialTime = true;
	}

	if(m_bTutorialTime && timeGetTime() - m_nTutorialTime > 3000)
		FUNCTION_GamePlayExplainUIInit();
	else if(m_bTutorialTime)
	{
		m_pAssistantPlayer->m_cInputKey.m_nCommand = KEY_SHORTPASS;
		m_pAssistantPlayer->m_cInputKey.m_nPower = 30;
	}
}