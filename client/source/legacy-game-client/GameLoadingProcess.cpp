#include "stdafx.h"
#include "GameLoadingProcess.h"
#include "PutPacket.h"
#include "GamePlayProcess.h"

CGameLoadingProcess::CGameLoadingProcess() : CBaseLoadingProcess()
{
}

CGameLoadingProcess::~CGameLoadingProcess(void)
{
}

void CGameLoadingProcess::InitProcess()
{
	m_pLeaveRoomPlayerPoint = NULL;

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	pRoom->InitRoomInGame();
	CObjPlayerManager::GetPtr()->InitAthleteInGame();

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);

	ClearUserProgress();
	
	if(pRoom->IsMeRoomJang())
		PutRobotInfo();

	if( m_hLoadingThread = _beginthread( &CGameLoadingProcess::LoadingThread,4096,this ) == -1 )
	{
		MessageBox(0,"쓰레드를 생성할 수 없어, 종료합니다.","오류",MB_OK);
	}

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	CObjRoomManager::GetPtr()->UserCount();

	CCameraController::GetPtr()->ChangeCamera();

	CChattingDialog::GetPtr()->ChattingListClear();
	CChattingDialog::GetPtr()->ChattingListDelete();

	//CGameProject::s_pGameProject->RecreateInputSystem();

	m_nState = STATE_START;
}

void CGameLoadingProcess::GotoGamePlay()
{
	CGameProject::s_pGameProject->HidePointer();

	CProcessManager::GetPtr()->SetNextProcess(new CGamePlayProcess);
}