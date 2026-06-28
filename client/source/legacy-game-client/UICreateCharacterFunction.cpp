#include "stdafx.h"
#include "CreateCharacterProcess.h"
#include "ChoiceTrioProcess.h"
#include "PutPacket.h"
#include "UIIMEEditBox.h"
#include "UIFunction.h"
#include "UIButton.h"

IMPLEMENT_USERFUNCTION( Create_Character_Confirm )
{
	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
	assert(pCurrentProcess);
	if(pCurrentProcess->InputCharacterName())
		pCurrentProcess->Create_Character();
}
IMPLEMENT_USERFUNCTION( Create_Character_Cancel )
{
	CProcessManager::GetPtr()->SetNextProcess(new CChoiceTrioProcess);
}
IMPLEMENT_USERFUNCTION( Create_Character_AbilityIncrease )
{
}
IMPLEMENT_USERFUNCTION( Create_Character_AbilityDecrease )
{
}
IMPLEMENT_USERFUNCTION( DUPLICATION_CHECK )
{
	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->DuplicationCheck();
}

IMPLEMENT_USERFUNCTION( CHANGE_STYLE )
{
	char nIndex=atoi(pParam->GetValue( "Index" ));

	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Change_Style(nIndex);
}

IMPLEMENT_USERFUNCTION( CHANGE_GENDER )
{
	int nIndex=atoi(pParam->GetValue( "Index" ));

	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Change_Gender(nIndex);
}

IMPLEMENT_USERFUNCTION( CHANGE_SKIN )
{
	int nIndex=atoi(pParam->GetValue( "Index" ));

	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->Change_Skin(nIndex);
}

IMPLEMENT_USERFUNCTION( CREATE_CHARACTER_RENDER ) //렌더링 영역 1
{
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;

	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_3DRender(windowstr);
}

IMPLEMENT_USERFUNCTION( INPUT_CHARACTERNAME ) //렌더링 영역 1
{
	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
	assert(pCurrentProcess);
	pCurrentProcess->InputCharacterName();
}

//////////////////////////////////////////////////////////////////////////
//캐릭 생성 튜토리얼 관련
IMPLEMENT_USERFUNCTION( CREATE_CONTINUE )
{
	UIFunction::Set_UIVisibility( "Choice_Taxt04", N3UI_VISIBLE_FALSE );			// 캐릭터 생성 설명 1
	UIFunction::Set_UIVisibility( "Choice_Taxt05", N3UI_VISIBLE_TRUE );				// 캐릭터 생성 설명 2
	UIFunction::Set_UIVisibility( "Create_Continue", N3UI_VISIBLE_FALSE );			// 계속 버튼
	UIFunction::Set_UIVisibility( "Create_Ok", N3UI_VISIBLE_TRUE );					// 확인 버튼
}

IMPLEMENT_USERFUNCTION( CREATE_OK )
{
	CUIIMEEditBox* pName=dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("CharacterName"));
	assert(pName);
	CUIControl::Set_FocusedPage(CUIControl::Get_FocusedPage());
	CUIControl::Set_FocusedControl(pName);
}

IMPLEMENT_USERFUNCTION( FACULTY_SHOW )
{
	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);	
	pCurrentProcess->FUNCTION_ShowFacultTuto();
}

