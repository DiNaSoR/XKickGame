#include "stdafx.h"
#include "ItemBagProcess.h"
#include "PutPacket.h"

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( ITEMBAG_BIG_FRACTION )
{
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	
	int index = atoi( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( ITEMBAG_SMALLFRACTION_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->m_nSmallFractionStart += index;
	pCurrentProcess->FUNCTION_SmallFractionInit(pCurrentProcess->m_nBigFractionIndex);
}

IMPLEMENT_USERFUNCTION( ITEMBAG_SMALL_FRACTION )
{
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int index = atoi( pParam->GetValue( "Index" ) );

	pCurrentProcess->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( ITEMBAG_CARD_DETAILVEIW )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_CardDetailView(index);
}

IMPLEMENT_USERFUNCTION( ITEMBAG_PAGE_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int page = pCurrentProcess->m_nCurrentPage + index;
	pCurrentProcess->FUNCTION_GoPage(page);
}

IMPLEMENT_USERFUNCTION( BAG_CharacterRefresh_Button )
{
}

IMPLEMENT_USERFUNCTION( ItemBag_Render ) //·»´õ¸µ ¿µ¿ª 1
{
	CBaseProcess* pCurrentProcess = CProcessManager::GetPtr()->Get_CurrentProcess();
	pCurrentProcess->FUNCTION_3DRender(pParam->GetValue( "Window" ));
}

IMPLEMENT_USERFUNCTION( ITEMB_SHAPE_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_ItemListExchange(index != 0);
}

IMPLEMENT_USERFUNCTION( EQUIP_BAGITEM )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_EquipBagItem(index);
}

IMPLEMENT_USERFUNCTION( DIVEST_BAGITEM )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_DivestBagItem(index);	
}

IMPLEMENT_USERFUNCTION( DIVEST_BAGITEM_S )
{
	int index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_DivestBagItemS(index);	
}

IMPLEMENT_USERFUNCTION( ENCHANT_BAGITEM )
{
	int index = atoi( pParam->GetValue( "Index" ) );
	
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTIOIN_Enchant(index);
}

IMPLEMENT_USERFUNCTION( ENCHANT_UP )
{
	int index = atoi( pParam->GetValue( "Index" ) );
	
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_EnchantOk(index);
}

IMPLEMENT_USERFUNCTION( ENCHANT_CHANGE )
{
	int index = atoi( pParam->GetValue( "Index" ) );
	
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_EnchantChange(index);
}

IMPLEMENT_USERFUNCTION( CARD_MIX )
{
	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_CardMix();
}

IMPLEMENT_USERFUNCTION( CARDINIT )
{
	//PutDivestCard(0);
}

IMPLEMENT_USERFUNCTION( CHANGE_ENTRY )
{
	char index = atoi( pParam->GetValue( "Index" ) );
	
	PutCardEntry(index);
}

IMPLEMENT_USERFUNCTION( CARD_TAB_CHANGE )
{
	char index = atoi( pParam->GetValue( "Index" ) );

	CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_CardTabChange(index);
}