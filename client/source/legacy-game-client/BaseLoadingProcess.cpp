#include "stdafx.h"
#include "GameLoadingProcess.h"
#include "UICommonMethod.h"
#include "ResBallFormat.h"
#include "ReadyRoomProcess.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "PutPacket.h"
#include "GamePlayProcess.h"
#include "QuestPlayProcess.h"
#include "ReplayProcess.h"
#include "UIFunction.h"

CBaseLoadingProcess::CBaseLoadingProcess() : CBaseProcess(), m_hLoadingThread(NULL), 
	m_bLoadingThreadDown(false), m_nThreadState(STATE_THREADSTART), m_nInputKeySendTime(0)
{
	m_nProcessCmd = PROCESS_GAMELOADING;
	m_bIsRender = false;
	m_pLeaveRoomPlayerPoint = NULL;
	m_bLoadingThreadDown = false;
	CPacketManager::GetPtr()->m_bIsLeaveRoom = false;
}

CBaseLoadingProcess::~CBaseLoadingProcess(void)
{
}

void CBaseLoadingProcess::SendMyProgress(DWORD MyProgress)
{
	PutGameLoad(MyProgress);
}

void CBaseLoadingProcess::Terminate()
{
	CloseHandle( (HANDLE)m_hLoadingThread );
}

void CBaseLoadingProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	UpdateProgress();

	//UICommonMethod::DisPlayNotice();
}

void CBaseLoadingProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	if(CPacketManager::GetPtr()->m_bIsLeaveRoom)
	{
		if(m_bLoadingThreadDown || !CPacketManager::GetPtr()->m_bIsRobotInfoDone)
			TerminateThread();
	}
	else if(CObjPlayerManager::GetPtr()->Check_ProgressFull(99) && m_bLoadingThreadDown)
		GotoGamePlay();
}

bool CBaseLoadingProcess::OnDefault(NiEventRef pEventRecord)
{
	CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	
	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CBaseLoadingProcess::TerminateThread()
{
	if(m_pLeaveRoomPlayerPoint != NULL)
		CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, m_pLeaveRoomPlayerPoint); //나간 캐릭터 리스트 삭제+인스턴스 삭제

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = false;
	CPacketManager::GetPtr()->m_bIsLeaveRoom = false;
	CPacketManager::GetPtr()->m_bIsAthleteInfoDone = true;

	CObjPlayerManager::GetPtr()->DeleteAlPlayerInList();

	CObjPlayerManager::GetPtr()->ErasePlayerModelPoint();

	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();

	printf("TerminateThread = 유져정보 다지웠거덩\n");

	CProcessManager::GetPtr()->SetChangeProcess(new CReadyRoomProcess);
}

//////////////////////////////////////////////////////////////////////////
bool CBaseLoadingProcess::UpdateThread()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(m_nThreadState)
	{
	case STATE_THREADSTART:
		{
			m_nThreadState = STATE_THREADINIT_LOAD;
		}
		break;
	case STATE_THREADINIT_LOAD:
		{
			FUNCTION_GameLoadingViewChange();

			m_nThreadState = STATE_THREADGROUND_LOAD;
		}
		break;
	case STATE_THREADGROUND_LOAD:
		{
			if(CPacketManager::GetPtr()->m_bIsRobotInfoDone)
			{
				InitUserProgress();

				LoadGround();
				m_nThreadState = STATE_THREADCHARACTER_LOAD;
				SendMyProgress(19);
			}
		}
		break;
	case STATE_THREADCHARACTER_LOAD:
		{
			LoadCharacter();

			CObjBallManager::GetPtr()->GetBallPointer()->SetCurrentPoint();

			m_nThreadState = STATE_THREADEND;
			SendMyProgress(99);
		}
		break;

	case STATE_THREADEND:
		{
			return true;
		}
		break;
	}

	return false;
}

void CBaseLoadingProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();

		CClientUIManager::GetPtr()->OnRender();	
	}
	else
	{
		CCameraController::GetPtr()->BeginCamera();
		{
			CClientUIManager::GetPtr()->OnRender();		
		}
		CCameraController::GetPtr()->EndCamera();
	}
}

void CBaseLoadingProcess::ProcessInput()
{
}

void CBaseLoadingProcess::LoadingThread(LPVOID pV)
{	
	CBaseLoadingProcess * pGameLoadingProcess = (CBaseLoadingProcess *)pV;

	while(1)
	{
		if(pGameLoadingProcess->UpdateThread()) break;
	}

	pGameLoadingProcess->m_bLoadingThreadDown = true;

	_endthread();
}

bool CBaseLoadingProcess::OnLostDevice()
{
	CObjectModelManager::GetPtr()->OnLostDevice();
	return true;
}

bool CBaseLoadingProcess::OnResetDevice()
{
    CObjectModelManager::GetPtr()->SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	CObjectModelManager::GetPtr()->OnResetDevice();
	return true;
}

void CBaseLoadingProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

    CPlayModeManager::GetPtr()->GroundOptionApply( pObjGroundInfo->m_pModel );
}

bool CBaseLoadingProcess::LoadCharacter()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	CObjRoomInfo* pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();

	int nPlayerCount = CObjPlayerManager::GetPtr()->GetPlayerCount();

	int i = 0;

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);

			if(pAnyPlayer == NULL)						continue;
			if(pAnyPlayer->m_nTeam == PLAYER_TEAM_VIEW)	continue;
			if(pAnyPlayer->m_pModel)					continue;

			if(pRoom->IsMeParent()) 
			{
				CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, pAnyPlayer->GetKind());
			}
			else 
			{
				CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, 0);
			}

			if(pRoom->m_nMode != ROOM_MODE_REPLAY && m_pSelectPlayerInfo->GetObjSeq() == pAnyPlayer->GetObjSeq())//나냐?
			{
				pAnyPlayer->ObjectPushBack(CObjectModelManager::GetPtr()->ObjectLoadStickCharacter(RESOURCE_PASSDIRECTIONMODEL, 10009, pAnyPlayer->m_pModel, "Bip01"));
			}

			if(pAnyPlayer->m_nPosition == POSITION_GK)
				pAnyPlayer->m_pModel->m_spCollisionNode = LoadCollision(pAnyPlayer->GetRootNode());

			CProcessManager::GetPtr()->AttachItem(pAnyPlayer); 	
			SendMyProgress(19 + (79*(++i)/nPlayerCount));
		}
	}
	CObjectModelManager::GetPtr()->ShadowOnOff(true, true);

	return false;
}

NiNodePtr CBaseLoadingProcess::LoadCollision(NiNode *pRootNode)
{
	NiStream kStream;
	char	FullName[STRLEN_128];

	sprintf(FullName, "Object\\Goal_Collision.nif");

	if( kStream.Load( FullName ) == false )
		return NULL;

	NiNodePtr spGroundObjectNode = (NiNode*)kStream.GetObjectAt(0);
	kStream.RemoveAllObjects();

	if(pRootNode)
	{
		pRootNode->AttachChild(spGroundObjectNode);//
		spGroundObjectNode->SetAppCulled(true);
		pRootNode->Update(0.0f);
		pRootNode->UpdateNodeBound();
		pRootNode->UpdateProperties();
		pRootNode->UpdateEffects();
	}

	return spGroundObjectNode;
}
/////////////////////////////////////////////////////////////////////////////////////

void CBaseLoadingProcess::FUNCTION_GameLoadingViewChange()
{
	CObjRoomInfo* pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();
	char sText[STRLEN_64] = "";

	// 그라운드 테이블 로드
	CResGroundTableFormat *pGroundTable = dynamic_cast<CResGroundTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_GROUNDTABLE, pRoom->m_nGroundCode, 0));
	assert(pGroundTable);

	_snprintf( sText, STRLEN_64, "GroundName0%d", pGroundTable->GetObjCode() );
	UIFunction::ChangeTexture( "GroundName", sText );

	// 로딩 배경(그라운드) 변경
	CUIControl* pGameLoadingBG = CClientUIManager::GetPtr()->FindEventControl("GameLoading_BG");

	if( CGameProject::s_pGameProject->GetWideScreen() )
	{	
		_snprintf(sText, STRLEN_64, "Map/Xkick_W_map%02d.dds", pGroundTable->GetObjCode());
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pGameLoadingBG, sText);
	}
	else
	{
		_snprintf(sText, STRLEN_64, "Map/%s.dds", pGroundTable->m_sIconFile );
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pGameLoadingBG, sText);
	}

	//// 보유 볼 표시
	//CUIControl* pHomeBall = CClientUIManager::GetPtr()->FindEventControl("Home_Ball");
	//CUIControl* pAwayBall = CClientUIManager::GetPtr()->FindEventControl("Away_Ball");

	//if(pRoom->m_nAttackTeam == PLAYER_TEAM_HOME)
	//{
	//	pHomeBall->SetVisible(N3UI_VISIBLE_TRUE);
	//	pAwayBall->SetVisible(N3UI_VISIBLE_FALSE);
	//}
	//else
	//{
	//	pHomeBall->SetVisible(N3UI_VISIBLE_FALSE);
	//	pAwayBall->SetVisible(N3UI_VISIBLE_TRUE);
	//}

	// 볼 테이블 로드
	CResBallTableFormat *pBallTable = dynamic_cast<CResBallTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_BALLTABLE, pRoom->m_nBallCode, 0));
	assert(pBallTable);
	
	// 선택된 볼 표시
	CUIControl* pBall = CClientUIManager::GetPtr()->FindEventControl("BallImg");
	
	_snprintf(sText, STRLEN_64, "Icon/Ball/%s", pBallTable->m_sIconFile );

	CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pBall, sText);

	// 선택된 볼 명 표시
	for(int i=0;i<CResourceManager::GetPtr()->GetResourceSize(RESOURCE_BALLTABLE);i++)
	{
		sprintf(sText, "Choiced_BallName0%d", i);
		CUIControl* pBallName = CClientUIManager::GetPtr()->FindEventControl(sText);
		if(!pBallName) continue;
		if(i == pRoom->m_nBallCode)
			pBallName->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pBallName->SetVisible(N3UI_VISIBLE_FALSE);		
	}	
}

void CBaseLoadingProcess::InitUserProgress()
{
	//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("Static_LoadingMassage");
	//assert(pControl);
	//pControl->SetVisible(N3UI_VISIBLE_FALSE);

	// HOME TEAM Init
	const VectorPlayerList& m_vPlayerList_H = CObjPlayerManager::GetPtr()->m_vHomeList;

	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_H.size(); ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_H_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToValue(sProgressBarName, "Static_Level", m_vPlayerList_H[nBarNum]->m_cLevel.m_nLevel);
		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Name", m_vPlayerList_H[nBarNum]->m_sName);

		UICommonMethod::PositionIconDraw( sProgressBarName, "Static_Position", m_vPlayerList_H[nBarNum]->m_nPosition, UICommonMethod::HOME_TEAM );
		
		N3UI_VISIBLE_TYPE eVisible = (m_vPlayerList_H[nBarNum]->GetKind() != PLAYER_KIND_ROBOT) ? N3UI_VISIBLE_FALSE : N3UI_VISIBLE_TRUE;
		UIFunction::SetChildToVisibility(sProgressBarName, "Static_Position_AI", eVisible);
	}

	// AWAY TEAM Init
	const VectorPlayerList& m_vPlayerList_A = CObjPlayerManager::GetPtr()->m_vAwayList;

	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_A.size(); ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_A_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToValue(sProgressBarName, "Static_Level", m_vPlayerList_A[nBarNum]->m_cLevel.m_nLevel);
		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Name", m_vPlayerList_A[nBarNum]->m_sName);

		UICommonMethod::PositionIconDraw( sProgressBarName, "Static_Position", m_vPlayerList_A[nBarNum]->m_nPosition, UICommonMethod::AWAY_TEAM );

		N3UI_VISIBLE_TYPE eVisible = (m_vPlayerList_A[nBarNum]->GetKind() != PLAYER_KIND_ROBOT) ? N3UI_VISIBLE_FALSE : N3UI_VISIBLE_TRUE;
		UIFunction::SetChildToVisibility(sProgressBarName, "Static_Position_AI", eVisible);
	}
	
	// Viewers Init
	const VectorPlayerList& m_vPlayerList_V = CObjPlayerManager::GetPtr()->m_vViewList;

	if( (int)m_vPlayerList_V.size() )
		UIFunction::Set_UIVisibility( "ObserverT", N3UI_VISIBLE_TRUE );
	else
		UIFunction::Set_UIVisibility( "ObserverT", N3UI_VISIBLE_FALSE );

	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_V.size(); ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_V_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_TRUE);
	}
}

void CBaseLoadingProcess::UpdateUserProgress()
{
	// HOME TEAM Update
    const VectorPlayerList& m_vPlayerList_H = CObjPlayerManager::GetPtr()->m_vHomeList;
	
	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_H.size(); ++nBarNum)
	{
		char sName[STRLEN_64];
		sprintf(sName,"%s",m_vPlayerList_H[nBarNum]->m_sName);

		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_H_User0%d", nBarNum+1);
		CUIControl* pCUIControls = CClientUIManager::GetPtr()->FindEventControl(sProgressBarName);
		if(pCUIControls == NULL) return;
		pCUIControls->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pProgressBg = pCUIControls->FindEventControl("GaugeBar_Static");
		assert(pProgressBg);
		CUIProgress* pProgress = dynamic_cast<CUIProgress*>(pProgressBg->FindEventControl("static_Progress"));
		assert(pProgress);
		pProgress->SetValue(m_vPlayerList_H[nBarNum]->m_nLoadingStep);		

		if(m_vPlayerList_H[nBarNum]->m_nLoadingStep >= pProgress->GetMaxValue())
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_FALSE);
	}

	// AWAY TEAM Update
	const VectorPlayerList& m_vPlayerList_A = CObjPlayerManager::GetPtr()->m_vAwayList;
	
	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_A.size(); ++nBarNum)
	{
		char sName[STRLEN_64];
		sprintf(sName,"%s",m_vPlayerList_A[nBarNum]->m_sName);

		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_A_User0%d", nBarNum+1);
		CUIControl* pCUIControls = CClientUIManager::GetPtr()->FindEventControl(sProgressBarName);
		if(pCUIControls == NULL) return;
		pCUIControls->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pProgressBg = pCUIControls->FindEventControl("GaugeBar_Static");
		assert(pProgressBg);
		CUIProgress* pProgress = dynamic_cast<CUIProgress*>(pProgressBg->FindEventControl("static_Progress"));
		assert(pProgress);
		pProgress->SetValue(m_vPlayerList_A[nBarNum]->m_nLoadingStep);		

		if(m_vPlayerList_A[nBarNum]->m_nLoadingStep >= pProgress->GetMaxValue())
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_FALSE);
	}

	// Viewers Update
	const VectorPlayerList& m_vPlayerList_V = CObjPlayerManager::GetPtr()->m_vViewList;
	
	for(DWORD nBarNum = 0; nBarNum < m_vPlayerList_V.size(); ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_V_User0%d", nBarNum+1);
		CUIControl* pCUIControls = CClientUIManager::GetPtr()->FindEventControl(sProgressBarName);
		if(pCUIControls == NULL) return;
		pCUIControls->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pProgressBg = pCUIControls->FindEventControl("GaugeBar_Static");
		assert(pProgressBg);
		CUIProgress* pProgress = dynamic_cast<CUIProgress*>(pProgressBg->FindEventControl("static_Progress"));
		assert(pProgress);
		pProgress->SetValue(m_vPlayerList_V[nBarNum]->m_nLoadingStep);		

		if(m_vPlayerList_V[nBarNum]->m_nLoadingStep >= pProgress->GetMaxValue())
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_TRUE);
		else
			pProgressBg->FindEventControl("static_Progress_Comp")->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

void CBaseLoadingProcess::ClearUserProgress()
{
	//CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("Static_LoadingMassage");
	//assert(pControl);
	//pControl->SetVisible(N3UI_VISIBLE_TRUE);

	// HOME TEAM Clear
	const VectorPlayerList& m_vPlayerList_H = CObjPlayerManager::GetPtr()->m_vHomeList;

    for(DWORD nBarNum = 0; nBarNum < TEAM_SIZE; ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_H_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_FALSE);

		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Level", "");
		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Name", "");

		UICommonMethod::PositionIconDraw( sProgressBarName, "Static_Position", 0, UICommonMethod::HOME_TEAM );
		UIFunction::SetProgressValue(sProgressBarName, "GaugeBar_Static", "static_Progress", 0);
	
		UIFunction::SetChildToVisibility(sProgressBarName, "GaugeBar_Static", "static_Progress_Comp", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(sProgressBarName, "Static_Position_AI", N3UI_VISIBLE_FALSE);
	}

	// AWAY TEAM Clear
	const VectorPlayerList& m_vPlayerList_A = CObjPlayerManager::GetPtr()->m_vAwayList;

	for(DWORD nBarNum = 0; nBarNum < TEAM_SIZE; ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_A_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_FALSE);

		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Level", "");
		UIFunction::Set_ChildControlText(sProgressBarName, "Static_Name", "");

		UICommonMethod::PositionIconDraw( sProgressBarName, "Static_Position", 0, UICommonMethod::AWAY_TEAM );
		UIFunction::SetProgressValue(sProgressBarName, "GaugeBar_Static", "static_Progress", 0);

		UIFunction::SetChildToVisibility(sProgressBarName, "GaugeBar_Static", "static_Progress_Comp", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(sProgressBarName, "Static_Position_AI", N3UI_VISIBLE_FALSE);
	}

	// Viewers Clear
	const VectorPlayerList& m_vPlayerList_V = CObjPlayerManager::GetPtr()->m_vViewList;

	for(DWORD nBarNum = 0; nBarNum < TEAM_SIZE-2; ++nBarNum)
	{
		char sProgressBarName[STRLEN_32];
		sprintf(sProgressBarName, "Static_V_User0%d", nBarNum+1);
		UIFunction::Set_UIVisibility(sProgressBarName, N3UI_VISIBLE_FALSE);

		UIFunction::SetProgressValue(sProgressBarName, "GaugeBar_Static", "static_Progress", 0);
		UIFunction::SetChildToVisibility(sProgressBarName, "GaugeBar_Static", "static_Progress_Comp", N3UI_VISIBLE_FALSE);
	}
}

void CBaseLoadingProcess::UpdateProgress()
{
	if(CPacketManager::GetPtr()->m_bIsRobotInfoDone)
		UpdateUserProgress();
}
