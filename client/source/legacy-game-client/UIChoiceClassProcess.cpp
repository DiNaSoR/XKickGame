#include "stdafx.h"
#include "ChoiceClassProcess.h"
#include "PutPacket.h"
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( CLASS_NEXT_EXPLAIN )
{
	CChoiceClassProcess* pCurrentProcess = dynamic_cast<CChoiceClassProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);	
	pCurrentProcess->FUNCTION_ClassNextExplain();
}

IMPLEMENT_USERFUNCTION( CHANGE_POSITION )
{
	char index=atoi(pParam->GetValue( "Index" ));

	CChoiceClassProcess* pCurrentProcess = dynamic_cast<CChoiceClassProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);	
	pCurrentProcess->FUNCTION_PositionExplain(index);
}

IMPLEMENT_USERFUNCTION( CHOICE_CLASS_CONFIRM )
{
	CChoiceClassProcess* pCurrentProcess = dynamic_cast<CChoiceClassProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);	

	PutGrowupCharacter(pCurrentProcess->m_nPosition*LIST10_SIZE);// 0 : POSITION_FW(10), 1 : POSITION_MF(20), 2 : POSITION_DF(30)
}

///////////////////////////////////////////////////////////////////////
////전직 관련
//IMPLEMENT_USERFUNCTION( CHOICECLASS_CONTINUE )
//{
//	char sText[STRLEN_64]="";
//
//	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
//	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
//	assert(pCurrentProcess);
//	sprintf(sText, "Choice_Taxt0%d", pCurrentProcess->m_nClassIndex+1);
//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );		
//	UIFunction::Set_UIVisibility( "Choice_Title", N3UI_VISIBLE_FALSE );				// 전직 타이틀
//	UIFunction::Set_UIVisibility( "ChoiceClass_Continue", N3UI_VISIBLE_FALSE );		// 계속 버튼
//
//	UIFunction::Set_UIVisibility( "Choice_BTaxt", N3UI_VISIBLE_TRUE );				// 2차 전직 설명
//	sprintf(sText, "Static_Position0%d", pCurrentProcess->m_nClassIndex);
//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );						// 포지션 설명
//	UIFunction::Set_UIVisibility( "ChoiceClass_Ok", N3UI_VISIBLE_TRUE );			// 완료 버튼
//}
//
//IMPLEMENT_USERFUNCTION( CHOICECLASS_OK )
//{
//	char sText[MAX_PATH]="";
//
//	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
//	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
//	assert(pCurrentProcess);
//	CObjPlayerInfo* pPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
//	assert(pCurrentProcess);
//	sprintf(sText, "Class_Tree0%d", pCurrentProcess->m_nClassIndex);
//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );						// 클래스 별 트리 표시
//	
//	CUIControl* pText = CClientUIManager::GetPtr()->FindEventControl("Choice_BTaxt");	// 캐릭터 명
//	assert(pText);
//	
//	_snprintf( sText, MAX_PATH, pText->GetTextEx().c_str(), pPlayerInfo->m_sName );
//	pText->SetText(sText);
//}
//
//IMPLEMENT_USERFUNCTION( CHOICE_CLASS_CONFIRM )
//{
//	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
//	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
//	assert(pCurrentProcess);
//	if( !pCurrentProcess->m_nClassIndex )
//		PutGrowupCharacter(pCurrentProcess->m_nPosition*LIST10_SIZE);// 0 : POSITION_FW(10), 1 : POSITION_MF(20), 2 : POSITION_DF(30)
//	else
//		PutGrowupCharacter(pCurrentProcess->m_nPosition);
//	pCurrentProcess->m_nClassIndex = -1;
//}
//
//IMPLEMENT_USERFUNCTION( NEXT_EXPLAIN )
//{
//	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
//	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
//	assert(pCurrentProcess);
//	pCurrentProcess->FUNCTION_Explain();
//}
//IMPLEMENT_USERFUNCTION( VIEW_POSITION )
//{
//	const char* sIndexstr = pParam->GetValue( "Index" ); 
//	char nIndex=atoi(sIndexstr);
//
//	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
//	CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(temp);
//	assert(pCurrentProcess);
//	pCurrentProcess->FUNCTION_SelectPositionVeiw(nIndex);
//}
