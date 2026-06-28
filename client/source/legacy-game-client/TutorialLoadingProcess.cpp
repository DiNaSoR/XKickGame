#include "stdafx.h"
#include "TutorialLoadingProcess.h"
#include "PutPacket.h"
#include "GamePlayProcess.h"
#include "UIFunction.h"
#include "TutorialPlay1Process.h"
#include "TutorialPlay2Process.h"
#include "TutorialPlay3Process.h"
#include "TutorialPlay4Process.h"

CTutorialLoadingProcess::CTutorialLoadingProcess() : CBaseLoadingProcess()
{
}

CTutorialLoadingProcess::CTutorialLoadingProcess(char *sText, int nCode) : CBaseLoadingProcess()
{
	sprintf(m_sLoadingPath, sText);

	m_nCode = nCode;
}

CTutorialLoadingProcess::~CTutorialLoadingProcess(void)
{
}

void CTutorialLoadingProcess::InitProcess()
{
	char sText[STRLEN_64]="";

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	CObjRoomInfo*	pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	pRoom->InitRoomInGame();
	CObjPlayerManager::GetPtr()->InitAthleteInGame();

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_sLoadingPath, m_nCode);

	ClearUserProgress();

	//if(pRoom->IsMeRoomJang())
	//	PutRobotInfo();

	if( m_hLoadingThread = _beginthread( &CTutorialLoadingProcess::LoadingThread,4096,this ) == -1 )
	{
		MessageBox(0,"쓰레드를 생성할 수 없어, 종료합니다.","오류",MB_OK);
	}

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = true;

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CTutorialLoadingProcess::GotoGamePlay()
{
	switch(m_nCode) 
	{
	case 105://일반 게임 하고 같음
	case 106:
	case 107:
		{
			CPlayModeManager::GetPtr()->SetRobotCount(0);
			CProcessManager::GetPtr()->SetNextProcess(new CGamePlayProcess());
		}
		break;
	}
}

void CTutorialLoadingProcess::FUNCTION_GameLoadingViewChange()
{
	char sText[STRLEN_64] = "";

	// 그라운드 테이블 로드
	CResGroundTableFormat *pGroundTable = dynamic_cast<CResGroundTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_GROUNDTABLE, 0, 0));
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

	// 볼 테이블 로드
	CResBallTableFormat *pBallTable = dynamic_cast<CResBallTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_BALLTABLE, 0, 0));
	assert(pBallTable);

	// 선택된 볼 표시
	CUIControl* pBall = CClientUIManager::GetPtr()->FindEventControl("BallImg");

	_snprintf(sText, STRLEN_64, "Icon/Ball/%s", pBallTable->m_sIconFile );

	CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pBall, sText);

	// 선택된 볼 명 표시
	for(int i=0;i<CResourceManager::GetPtr()->GetResourceSize(RESOURCE_BALLTABLE);i++)
	{
		sprintf(sText, "Choiced_BallName%02d", i);
		CUIControl* pBallName = CClientUIManager::GetPtr()->FindEventControl(sText);
		pBallName->SetVisible(N3UI_VISIBLE_FALSE);		
	}	
}