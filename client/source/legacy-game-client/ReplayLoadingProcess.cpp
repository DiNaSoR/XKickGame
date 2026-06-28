#include "stdafx.h"
#include "ReplayLoadingProcess.h"
#include "PutPacket.h"
#include "ReplayProcess.h"

CReplayLoadingProcess::CReplayLoadingProcess(const char *FileName) : CBaseLoadingProcess()
{
	_snprintf(m_sFileName, MAX_PATH, FileName);	
}

CReplayLoadingProcess::~CReplayLoadingProcess(void)
{
}

void CReplayLoadingProcess::InitProcess()
{
//테스트
	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_pSelectPlayerInfo = pObjPlayerInfo->Clone();

	m_pSelectPlayerInfo->m_nTeam = PLAYER_TEAM_VIEW;

	CObjPlayerManager::GetPtr()->SetPlayerLists(m_pSelectPlayerInfo, m_pSelectPlayerInfo->m_nTeam);
//테스트 끝

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPacketManager::GetPtr()->m_cReplayQue.InitReplayData();
	CPacketManager::GetPtr()->m_cReplayQue.ReadReplayFile(m_sFileName);

	CObjRoomInfo*	pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	pRoom->InitRoomInGame();
	CObjPlayerManager::GetPtr()->InitAthleteInGame();

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);

	ClearUserProgress();

	if( m_hLoadingThread = _beginthread( &CReplayLoadingProcess::LoadingThread,4096,this ) == -1 )
	{
		MessageBox(0,"쓰레드를 생성할 수 없어, 종료합니다.","오류",MB_OK);
	}

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = true;

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CReplayLoadingProcess::SendMyProgress(DWORD MyProgress)
{
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			CObjPlayerInfo* pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL) continue;

			pAnyPlayer->m_nLoadingStep = (char)MyProgress;
		}
	}

	UpdateProgress();
}

void CReplayLoadingProcess::GotoGamePlay()
{
	CProcessManager::GetPtr()->SetNextProcess(new CReplayProcess);
}