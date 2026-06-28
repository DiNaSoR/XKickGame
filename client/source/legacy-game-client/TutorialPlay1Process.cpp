#include "stdafx.h"
#include "TutorialPlay1Process.h"
#include "UICommonMethod.h"
#include "UIProgress.h"
#include "PutPacket.h"
#include "UIFunction.h"

CTutorialPlay1Process::CTutorialPlay1Process() : CTutorialPlayProcess()
{
	m_nTutorialType = TUTORIAL_1TYPE;
}

CTutorialPlay1Process::~CTutorialPlay1Process(void)
{
}

void CTutorialPlay1Process::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::UpdateProcess(fFrameTime, fAccumTime);

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow"));
	assert(pProgress);
	CUIProgress* pProgressO = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow_Big"));
	assert(pProgressO);
	pProgressO->SetValue(pProgress->GetValue());
}

void CTutorialPlay1Process::FUNCTION_ExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_ExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent00"  );
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
void CTutorialPlay1Process::FUNCTION_KeyUIInit()
{
	CTutorialPlayProcess::FUNCTION_KeyUIInit();
}

void CTutorialPlay1Process::FUNCTION_DemonstrationGameUIInit()
{
	FUNCTION_KeyUIInit();

	CTutorialPlayProcess::FUNCTION_DemonstrationGameUIInit();

	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_TRUE );

	if( m_bAssistant )
		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
}

void CTutorialPlay1Process::FUNCTION_GamePlayExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayExplainUIInit();

	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );
	
	UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent01"  );
	CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
	assert(pShadowLine);

	if( !pShadowLine->IsPlay() && m_bShadow )
	{
		pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, true, 0);

		m_bShadow = false;
	}
}

void CTutorialPlay1Process::FUNCTION_GamePlayUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayUIInit();

	UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_A0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_D", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_F", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_A", N3UI_VISIBLE_FALSE );
	
	int nTry = m_nTryTotalCount%3;

	switch(nTry)
	{
	case 0:
		UIFunction::Set_UIVisibility( "Key_D", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_FALSE );
		break;
	case 1:
		UIFunction::Set_UIVisibility( "Key_F", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_FALSE );
		break;
	case 2:
		UIFunction::Set_UIVisibility( "Key_A", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_A0", N3UI_VISIBLE_FALSE );
		break;
	}
}

void CTutorialPlay1Process::FUNCTION_ResultUIInit(bool bSuccess)
{
	CTutorialPlayProcess::FUNCTION_ResultUIInit(bSuccess);
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	/*if( bSuccess )
	{
		CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 101, 0));
		assert(pTable);

		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
		UIFunction::Set_ControlText( "Conversation_Text", sText );
	}*/
}

bool CTutorialPlay1Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_nCommand = 0;
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CTutorialPlay1Process::SuccessProcess()
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

void CTutorialPlay1Process::FailProcess()
{
	m_nTryTotalCount++;	
}

void CTutorialPlay1Process::DemonstrationGameInit()
{
	CTutorialPlayProcess::DemonstrationGameInit();

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow_Big"));
	assert(pProgress);

	if(m_bAssistant)
	{
		//공소유 변경
		CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();
		pBall->m_nBallForm = m_pAssistantPlayer->m_nForm;
		pProgress->SetValue( 0.f );
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

void CTutorialPlay1Process::DemonstrationPlay(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::DemonstrationPlay(fFrameTime, fAccumTime);

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow_Big"));
	assert(pProgress);

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 500)
	{
		UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Key_D1", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_TRUE );
	}

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 4500)
	{
		UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_D1", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );

		m_nTutorialTime = timeGetTime();
		m_bTutorialTime = true;
	}
	else if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 500)
		pProgress->SetValue( ((timeGetTime() - m_nTutorialTime)-500)/50.f );

	if(m_bTutorialTime && timeGetTime() - m_nTutorialTime > 3000)
		FUNCTION_GamePlayExplainUIInit();
	else if(m_bTutorialTime)
	{
		m_pAssistantPlayer->m_cInputKey.m_nCommand = KEY_SHOOT1;
		m_pAssistantPlayer->m_cInputKey.m_nPower = 80;
	}
}

