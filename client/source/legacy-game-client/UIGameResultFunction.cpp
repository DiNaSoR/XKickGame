#include "stdafx.h"
#include "GameResultProcess.h"
#include "ReadyRoomProcess.h"
#include "ChoiceClassProcess.h"

IMPLEMENT_USERFUNCTION( GameResult_OK ) 
{
	//테스트 UI
	CProcessManager::GetPtr()->SetNextProcess(new CReadyRoomProcess);
}

IMPLEMENT_USERFUNCTION( GAMERESULT_STATESFACIAL_RANDER ) //렌더링 영역 1
{
	CGameResultProcess* pCurrentProcess = dynamic_cast<CGameResultProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;

	pCurrentProcess->FUNCTION_3DRender(windowstr);
}