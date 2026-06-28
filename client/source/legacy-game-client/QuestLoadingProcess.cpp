#include "stdafx.h"
#include "QuestLoadingProcess.h"
#include "PutPacket.h"
#include "GamePlayProcess.h"
#include "QuestPlay1Process.h"
#include "QuestPlay2Process.h"
#include "QuestPlay3Process.h"
#include "QuestPlay4Process.h"
#include "QuestPlay5Process.h"
#include "QuestPlay6Process.h"

CQuestLoadingProcess::CQuestLoadingProcess() : CBaseLoadingProcess()
{
}

CQuestLoadingProcess::CQuestLoadingProcess(char *sText, int nCode) : CBaseLoadingProcess()
{
	sprintf(m_sLoadingPath, sText);

	m_nCode = nCode;
}

CQuestLoadingProcess::~CQuestLoadingProcess(void)
{
}

void CQuestLoadingProcess::InitProcess()
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

	if( m_hLoadingThread = _beginthread( &CQuestLoadingProcess::LoadingThread,4096,this ) == -1 )
	{
		MessageBox(0,"쓰레드를 생성할 수 없어, 종료합니다.","오류",MB_OK);
	}

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = true;

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CQuestLoadingProcess::GotoGamePlay()
{
	switch(m_nCode) 
	{
	case 10000:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay1Process());
		break;
	case 103:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay2Process());
		break;
	case 104:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay3Process());
		break;
	case 105://일반 게임 하고 같음
	case 109:
	case 110:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
		{
			CPlayModeManager::GetPtr()->SetRobotCount(0);
			CProcessManager::GetPtr()->SetNextProcess(new CGamePlayProcess());
		}
		break;
	case 106:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay4Process());
		break;
	case 107:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay5Process());
		break;
	case 108:
		CProcessManager::GetPtr()->SetNextProcess(new CQuestPlay6Process());
		break;
	}
}