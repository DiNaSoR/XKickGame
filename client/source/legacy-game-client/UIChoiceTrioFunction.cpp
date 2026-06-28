#include "stdafx.h"
#include "CreateCharacterProcess.h"
#include "ChoiceTrioProcess.h"
#include "GameLoginProcess.h"
#include "PutPacket.h"

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_USERFUNCTION( Select_Charater_Create ) //캐릭터 생성
{
	//테스트 UI
	CProcessManager::GetPtr()->SetNextProcess(new CCreateCharacterProcess);
}

IMPLEMENT_USERFUNCTION( Select_Charater_Choice ) //트리오 선택
{
	CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Select_Character();
}

IMPLEMENT_USERFUNCTION( Select_Charater_Delete ) //캐릭터 삭제
{
	CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_JUMIN, 0, CMessageBoxDialog::MSGBOXTYPE_IMECONFIRMCANCEL);
}

IMPLEMENT_USERFUNCTION( CHOICETRIO_CHARACTER_RENDER ) //렌더링 영역 1
{
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;
	CBaseProcess* pCurrentProcess = CProcessManager::GetPtr()->Get_CurrentProcess();
	pCurrentProcess->FUNCTION_3DRender(windowstr);
}

IMPLEMENT_USERFUNCTION( Select_Character )
{
    CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
    int nIndex = atoi(pParam->GetValue( "Index" ));
    pCurrentProcess->SelectCharacter( nIndex );
}

IMPLEMENT_USERFUNCTION( JUMIN_CONFIRM )
{
	CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Select_Delete();
}

IMPLEMENT_USERFUNCTION( LOGOUT )	// 임시 기능
{
	PutCertifyExit(PLAYER_EXIT_TRANSPORT);
	memset(CGameProject::s_pGameProject->m_strName, 0, MAX_PATH );
	memset(CGameProject::s_pGameProject->m_strPass, 0, MAX_PATH );
	CProcessManager::GetPtr()->SetChangeProcess(new CGameLoginProcess);
}