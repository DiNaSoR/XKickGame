#include "stdafx.h"
#include "TutorialPlay4Process.h"
#include "PutPacket.h"
#include "UIProgress.h"
#include "UIFunction.h"

CTutorialPlay4Process::CTutorialPlay4Process() : CTutorialPlayProcess()
{
	m_nTutorialType = TUTORIAL_4TYPE;
}

CTutorialPlay4Process::~CTutorialPlay4Process(void)
{
}

void CTutorialPlay4Process::FUNCTION_ExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_ExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent07"  );
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
void CTutorialPlay4Process::FUNCTION_KeyUIInit()
{
	CTutorialPlayProcess::FUNCTION_KeyUIInit();
}

void CTutorialPlay4Process::FUNCTION_GamePlayExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent06"  );
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

void CTutorialPlay4Process::FUNCTION_DemonstrationGameUIInit()
{
	FUNCTION_KeyUIInit();

	CTutorialPlayProcess::FUNCTION_DemonstrationGameUIInit();

	if( m_bAssistant )
		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
}

void CTutorialPlay4Process::FUNCTION_GamePlayUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayUIInit();

	UIFunction::Set_UIVisibility( "Key_Q", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_Q0", N3UI_VISIBLE_FALSE );
}

void CTutorialPlay4Process::FUNCTION_ResultUIInit(bool bSuccess)
{
	CTutorialPlayProcess::FUNCTION_ResultUIInit(bSuccess);
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	/*if( bSuccess )
	{
		CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 104, 0));
		assert(pTable);

		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
		UIFunction::Set_ControlText( "Conversation_Text", sText );
	}*/
}

bool CTutorialPlay4Process::TryProcess()
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

bool CTutorialPlay4Process::SuccessProcess()
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

void CTutorialPlay4Process::FailProcess()
{
	CObjPlayerInfo *pWaypointPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOWAYPOINTATTACK);
	assert(pWaypointPlayer);
	m_nTryTotalCount = pWaypointPlayer->GetWaypointCount() + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TACKLE];
}

void CTutorialPlay4Process::DemonstrationGameInit()
{
	CTutorialPlayProcess::DemonstrationGameInit();

	if(m_bAssistant)
	{
		m_pAssistantPlayer->SetKind(PLAYER_KIND_ROBOT);
		m_pAssistantPlayer->SetUpdateDutyMachine(true);
		m_pAssistantPlayer->ChangeAIMachine(DUTY_TACKLEMOVE);
		m_bTutorialTime = false;
	}
}

void CTutorialPlay4Process::DemonstrationPlay(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::DemonstrationPlay(fFrameTime, fAccumTime);

	CObjPlayerInfo *pWaypointPlayer = CObjPlayerManager::GetPtr()->GetDutyPlayer(PLAYER_TEAM_AWAY, DUTY_TUTOWAYPOINTATTACK);

	NiPoint3 temp = pWaypointPlayer->GetFactPoint() - m_pAssistantPlayer->GetFactPoint();

	if( 1.2f < temp.Length() && temp.Length() < 2.6f )
	{
		UIFunction::Set_UIVisibility( "Key_Q0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Key_Q1", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_TRUE );
	}
	else if( temp.Length() < 1.2f ||  2.6f < temp.Length() )
	{
		UIFunction::Set_UIVisibility( "Key_Q0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_Q1", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );
	}

	if(!m_bTutorialTime && pWaypointPlayer->m_nBallHaving != BALL_HAVING_MINE)
	{
		m_pAssistantPlayer->ChangeAIMachine(DUTY_STAND);
		m_nTutorialTime = timeGetTime();
		m_bTutorialTime = true;
	}
	if(m_bTutorialTime && timeGetTime() - m_nTutorialTime > 3000 )
	{
		FUNCTION_GamePlayExplainUIInit();
	}
}