#include "stdafx.h"
#include "ShopDialog.h"
#include "SkillShopDialog.h"
#include "SkillShopProcess.h"
#include "SkillBagProcess.h"
#include "../InterfaceLibrary/UIComboBox.h"
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( SSHOP_BIG_FRACTION )
{
	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( SSHOP_SMALL_FRACTION )
{
	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( SKILLSHOP_PAGE_CHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->m_nCurrentPage += index;
	pCurrentProcess->FUNCTION_GoPage(pCurrentProcess->m_nCurrentPage);
}

IMPLEMENT_USERFUNCTION( SkillBag_OK )
{
	CProcessManager::GetPtr()->SetNextProcess(new CSkillBagProcess);
}

IMPLEMENT_USERFUNCTION( SHOP_SkillPosition_Combo )
{
	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->UpdatePositionArrange();
}

/////////////////////////////////////////////////////////////////////////////////////
//SkillShopDialog

IMPLEMENT_USERFUNCTION( SKILL_BUY_DIALOG_SHOW )
{
	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atol( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_Buy(index);
}

IMPLEMENT_USERFUNCTION( SkillS_SHOP_Replay )
{

}

IMPLEMENT_USERFUNCTION( BUY_SKILL_POINT )
{
	CSkillShopDialog::GetPtr()->FUNCTION_BuyPointToCoin(BUY_KIND_POINT);
}

IMPLEMENT_USERFUNCTION( BUY_SKILL_COIN )
{
	CSkillShopDialog::GetPtr()->FUNCTION_BuyPointToCoin(BUY_KIND_CASH);
}

IMPLEMENT_USERFUNCTION( BuySkillOk )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillShopDialog::GetPtr()->FUNCTION_BuyOk(index);
}

IMPLEMENT_USERFUNCTION( SkillShop_Render ) //·»´õ¸µ ¿µ¿ª 1
{
    CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

    const char* windowstr = pParam->GetValue( "Window" ); 
    if (windowstr == NULL) return;
    pCurrentProcess->FUNCTION_3DRender(windowstr);
}

IMPLEMENT_USERFUNCTION( SKILL_VIEW )
{
    CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

    pCurrentProcess->FUNCTION_View();
}

IMPLEMENT_USERFUNCTION( SKILLVIEW_REPLAY )
{
    CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
    pCurrentProcess->FUNCTION_Replay();
}

IMPLEMENT_USERFUNCTION( POSITION_ARRANGE )
{
	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
	assert(pCurrentProcess);;

	pCurrentProcess->UpdatePositionArrange();
}

IMPLEMENT_USERFUNCTION( SHOP_DETAIL_TAB )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_DetailTabChange(index);
}