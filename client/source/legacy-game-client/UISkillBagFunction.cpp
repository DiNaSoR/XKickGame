#include "stdafx.h"
#include "SkillShopProcess.h"
#include "SkillBagProcess.h"
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( SBAG_BIG_FRACTION )
{
	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( SBAG_SMALL_FRACTION )
{
	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( SBAG_PAGECHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int page = pCurrentProcess->m_nCurrentPage + index;
	pCurrentProcess->FUNCTION_GoPage(page);
}

IMPLEMENT_USERFUNCTION( BAG_WearSkillButton )
{
}

IMPLEMENT_USERFUNCTION( SKILLB_SHAPE_CHANGE )
{
	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_ItemListExchange(index != 0);
}

IMPLEMENT_USERFUNCTION( DETAIL_PAGECHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

    CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
    pCurrentProcess->FUNCTION_MySkillPageChange(index);
}

IMPLEMENT_USERFUNCTION( DIVEST_ALL )
{
    CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
    pCurrentProcess->FUNCTION_AllDivest();
}

IMPLEMENT_USERFUNCTION( EQUIP_SBAGITEM )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_SkillEquip(index);
}

IMPLEMENT_USERFUNCTION( DIVEST_SBAGITEM )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_SkillDivest(index);
}

IMPLEMENT_USERFUNCTION( SKILL_UP )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_SkillUpgrade(index);
}

IMPLEMENT_USERFUNCTION( DETAIL_TAB )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillBagProcess* pCurrentProcess = dynamic_cast<CSkillBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_DetailTabChange(index);
}

