#include "stdafx.h"
#include "TutorialPlay2Process.h"
#include "PutPacket.h"
#include "UIProgress.h"
#include "UIFunction.h"

CTutorialPlay2Process::CTutorialPlay2Process() : CTutorialPlayProcess()
{
	m_nTutorialType = TUTORIAL_2TYPE;
	m_bChangeShootingZoon = false;
	m_pkShootingZoon = NULL;
	m_nTutorialPartKey2 = KEY_EMPTY;
}

CTutorialPlay2Process::~CTutorialPlay2Process(void)
{
}

void CTutorialPlay2Process::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);
	ChangeShootingZoon();
}

void CTutorialPlay2Process::InputKey()
{
	CBasePlayProcess::InputKey();

	if(m_pSelectPlayerInfo->m_cInputKey.m_nCommand != KEY_EMPTY)
	{
		m_nTutorialPartKey2 = m_pSelectPlayerInfo->m_cInputKey.m_nCommand;
	}
}

void CTutorialPlay2Process::FUNCTION_ExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_ExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent03"  );
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

void CTutorialPlay2Process::FUNCTION_KeyUIInit()
{
	CTutorialPlayProcess::FUNCTION_KeyUIInit();
}

void CTutorialPlay2Process::FUNCTION_DemonstrationGameUIInit()
{
	FUNCTION_KeyUIInit();

	CTutorialPlayProcess::FUNCTION_DemonstrationGameUIInit();

	if( m_bAssistant )
		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
}

void CTutorialPlay2Process::FUNCTION_GamePlayExplainUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayExplainUIInit();

	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);

	UIFunction::ChangeChildTexture( pBG, "Explain_Text", "TPMent02"  );
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

void CTutorialPlay2Process::FUNCTION_GamePlayUIInit()
{
	CTutorialPlayProcess::FUNCTION_GamePlayUIInit();
	int nTry = m_nTryTotalCount%2;

	UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_D", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Key_F", N3UI_VISIBLE_FALSE );

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
	}
}

void CTutorialPlay2Process::FUNCTION_ResultUIInit(bool bSuccess)
{
	CTutorialPlayProcess::FUNCTION_ResultUIInit(bSuccess);
	UIFunction::Set_UIVisibility( "Dialog_Gage01", N3UI_VISIBLE_FALSE );

	/*if( bSuccess )
	{
		CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 102, 0));
		assert(pTable);

		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "+ %d Points", pTable->m_cGift[0].m_nAmount);
		UIFunction::Set_ControlText( "Conversation_Text", sText );
	}*/
}

bool CTutorialPlay2Process::TryProcess()
{
	if(m_nTryTotalCount < m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS])
	{
		m_pSelectPlayerInfo->m_cInputKey.m_fAngle = -1;
		return true;
	}
	return false;
}

bool CTutorialPlay2Process::SuccessProcess()
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

void CTutorialPlay2Process::FailProcess()
{
	m_nTryTotalCount = m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYSHOOT] + m_pSelectPlayerInfo->m_cCount.m_nCount[ARRAY_COUNT_TRYPASS];
}

//튜토리얼 1에 관한 내용
void CTutorialPlay2Process::ChangeShootingZoon()
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
bool CTutorialPlay2Process::CheckShootingZoon()
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

void CTutorialPlay2Process::DemonstrationGameInit()
{
	CTutorialPlayProcess::DemonstrationGameInit();

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow"));
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

void CTutorialPlay2Process::DemonstrationPlay(float fFrameTime, float fAccumTime)
{
	CTutorialPlayProcess::DemonstrationPlay(fFrameTime, fAccumTime);

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Progress_Pow"));
	assert(pProgress);

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 500)
	{
		UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Key_F1", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_TRUE );
	}

	if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 1500)
	{
		UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Key_F1", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "Push00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Push01", N3UI_VISIBLE_FALSE );

		m_nTutorialTime = timeGetTime();
		m_bTutorialTime = true;
	}
	else if(!m_bTutorialTime && timeGetTime() - m_nTutorialTime > 500)
		pProgress->SetValue( ((timeGetTime() - m_nTutorialTime)-500)/10.f );

	if(m_bTutorialTime && timeGetTime() - m_nTutorialTime > 3000)
		FUNCTION_GamePlayExplainUIInit();
	else if(m_bTutorialTime)
	{
		m_pAssistantPlayer->m_cInputKey.m_nCommand = KEY_SHOOT2;
		m_pAssistantPlayer->m_cInputKey.m_nPower = 100;
	}
}