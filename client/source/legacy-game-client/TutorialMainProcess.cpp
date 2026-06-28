#include "stdafx.h"
#include "TutorialMainProcess.h"
#include "UICommonMethod.h"
#include "../InterfaceLibrary/UIButton.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "UIFunction.h"
#include "PutPacket.h"

CTutorialMainProcess::CTutorialMainProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_TUTORIALSTEP1MAIN;
	m_bIsRender = true;
	m_nGetTime = 0;
}

CTutorialMainProcess::~CTutorialMainProcess(void)
{
}

void CTutorialMainProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CTutorialMainProcess::InitProcess()
{
	PutLeaveRoom();

	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, ".\\Script\\GameScript.xml", m_nProcessCmd*10000);

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	CCameraController::GetPtr()->ChangeCamera();

	CPacketManager::GetPtr()->m_bIsChoiceRoomDone = false;

	FUNCTION_BasicInfoDraw();

	m_nState = STATE_START;
}

void CTutorialMainProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	
	//UICommonMethod::BasicInfoStop();
	//UICommonMethod::DisplayAll(m_pObjPlayerInfo);
	UICommonMethod::DisPlayNotice();

	FUNCTION_CompleteUpdate();
	FUNCTION_ExpUpdate();
}

void CTutorialMainProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{		
			LoadGround();
			m_nState = STATE_PLAY;
		}
		break;
	case STATE_PLAY:
		{
			m_nState = STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

void CTutorialMainProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();
	}
}

bool CTutorialMainProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CTutorialMainProcess::GetSameProcess()
{
	return new CTutorialMainProcess;
}

void CTutorialMainProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		FUNCTION_MouseOverCheck();
		break;
	}
}

void CTutorialMainProcess::FUNCTION_MouseOverCheck()
{
	char sText[STRLEN_64]="";

	for(int i = 0; i < 3; i++)
	{
		_snprintf(sText, STRLEN_64, "TutorialPlayBG0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		RECT rt = pControl->GetWindowBox();

		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pControl->GetVisible() == N3UI_VISIBLE_TRUE)
			UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_TRUE );
		else
			UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_FALSE );
	}
}

void CTutorialMainProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CTutorialMainProcess::FUNCTION_ExpUpdate()
{
	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl( "LevelProgress" ));
	assert(pPrg);
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	static int g_nGauge = 0;

	pPrg->SetValue(g_nGauge*100);

	int nCnt = 0;

	for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++)
	{
		int nCode = pObjPlayerInfo->m_vQuestList.at(i)->m_nCode;

		if( 100 <= nCode && nCode < 108 )	
			nCnt++;
	}

	if( (nCnt - g_nGauge) == 0 ) return;

	int nAddCount = 20/(nCnt - g_nGauge);

	if( (timeGetTime() - m_nGetTime) < 2000 )
		pPrg->SetValue(pPrg->GetValue()+((timeGetTime() - m_nGetTime)/nAddCount));
	else
		g_nGauge = nCnt;
}

void CTutorialMainProcess::FUNCTION_CompleteUpdate()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	char sText[STRLEN_64]="";

	// √ ±‚»≠
	for(int i=0;i<LIST5_SIZE;i++)
	{
		_snprintf(sText, STRLEN_64, "BComp0%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
		//_snprintf(sText, STRLEN_64, "BasicTutorialBtn0%d", i );
		//
		//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		//assert(pControl);
		//pControl->SetEnabled(true);
	}	
	
	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "TutorialPlayBG0%d", i );
		UIFunction::SetChildToVisibility( sText, "PComp", N3UI_VISIBLE_FALSE );
		
		//_snprintf(sText, STRLEN_64, "StartBtn0%d", i);
		//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		//assert(pControl);
		//pControl->SetEnabled(true);
	}

	if( (int)!pObjPlayerInfo->m_vQuestList.size() )
		return;

	char nCnt=0;

	for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++)
	{
		int nCode = pObjPlayerInfo->m_vQuestList.at(i)->m_nCode;

		if( nCode < 105 )
		{
			_snprintf(sText, STRLEN_64, "BComp0%d", nCode%100 );
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
			//_snprintf(sText, STRLEN_64, "BasicTutorialBtn0%d", m_pObjPlayerInfo->m_vQuestList.at(i)->m_nCode%100 );
			//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			//assert(pControl);
			//pControl->SetEnabled(false);
			nCnt++;
		}	
		else if( 104 < nCode && nCode < 108 )
		{
			_snprintf(sText, STRLEN_64, "TutorialPlayBG0%d", nCode-105 );
			UIFunction::SetChildToVisibility( sText, "PComp", N3UI_VISIBLE_TRUE );
			//_snprintf(sText, STRLEN_64, "StartBtn0%d", nCode-105);
			//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			//assert(pControl);
			//pControl->SetEnabled(false);
			nCnt++;
		}
	}

	if( nCnt == LIST8_SIZE )
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_CHOICECLASS, 20900, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
}

void CTutorialMainProcess::FUNCTION_BasicInfoDraw()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	char nCnt=0;

	for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++)
	{
		int nCode = pObjPlayerInfo->m_vQuestList.at(i)->m_nCode;
		
		if( 104 < nCode && nCode < 108 )
			nCnt++;
	}

	static bool g_bInit = true;

	if( g_bInit && nCnt != 8 )
	{
		g_bInit = false;

		CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TMExplainBG" );
		assert(pBG);

		if( nCnt == 0 )
		{
			UIFunction::Set_UIVisibility( "NpcGuide", N3UI_VISIBLE_TRUE );
			UIFunction::ChangeChildTexture( pBG, "GT_Static", "CSMent02"  );
		}
		else
		{
			UIFunction::Set_UIVisibility( "NpcGuide", N3UI_VISIBLE_TRUE );
			UIFunction::ChangeChildTexture( pBG, "GT_Static", "CSMent03"  );
		}

		CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
		assert(pShadowLine);

		pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, false, 0);
	}

	m_nGetTime = timeGetTime();

	char sText[STRLEN_64]="";

	UICommonMethod::PositionIconDraw( "TraineePositionMark", pObjPlayerInfo->m_nPosition, UICommonMethod::HOME_TEAM );
	
	//CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	//assert(pMTable);
	//
	//_snprintf( sText, STRLEN_64, pMTable->m_sMessage, pObjPlayerInfo->m_cLevel.m_nLevel);
	//UIFunction::Set_ControlText( "TraineeLevel", sText );
	UIFunction::Set_ControlText( "TraineeName", pObjPlayerInfo->m_sName );
}