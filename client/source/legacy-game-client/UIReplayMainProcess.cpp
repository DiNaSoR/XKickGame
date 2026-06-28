#include "stdafx.h"
#include "ClientUIManager.h"
#include "ReplayMainProcess.h"
#include "../InterfaceLibrary/UIButton.h"

IMPLEMENT_USERFUNCTION( REPLAY_TAB )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->m_nKind = index;

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "Channel0%d", i);
		CUIButton* pChannel = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChannel);

		if(i==index)
		{
			pChannel->SetPressed(true);
			if(1<=(int)pCurrentProcess->m_vFileList.size())
			{
				SN3UIUserFunctionParameter cParam;
				cParam.AddParam( "Index", "0" );
				CClientUIManager::GetPtr()->ProcUserFunction( "REPLAY_SELECT", &cParam );
			}
		}
		else
			pChannel->SetPressed(false);
	}

	pCurrentProcess->FUNCTION_UpdatePage(0);
}

IMPLEMENT_USERFUNCTION( REPLAY_SELECT )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->m_nSelectReplay = index;
}

IMPLEMENT_USERFUNCTION( REPLAY_CHOICE )
{
	int index = atol( pParam->GetValue( "Index" ) );;

	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->ReplayListChoice(index);
}

IMPLEMENT_USERFUNCTION( REPLAY_DELETE )
{
	char sText[MAX_PATH]="";

	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->ReplayListDelete();
}

IMPLEMENT_USERFUNCTION( REPLAY_PAGE_CHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_UpdatePage(index);
}

IMPLEMENT_USERFUNCTION( REFRESH_REPLAYLIST )
{
	CReplayMainProcess* pCurrentProcess = dynamic_cast<CReplayMainProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->m_nPage = 1;
	pCurrentProcess->FUNCTION_UpdatePage(0);
}