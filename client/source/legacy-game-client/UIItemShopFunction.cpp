#include "stdafx.h"
#include "ShopDialog.h"
#include "ItemShopProcess.h"

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_USERFUNCTION( ITEMSHOP_SMALLFRACTION_CHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->m_nSmallFractionStart += index;
	pCurrentProcess->FUNCTION_SmallFractionInit(pCurrentProcess->m_nBigFractionIndex);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_BIG_FRACTION )
{
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	pCurrentProcess->FUNCTION_BigFraction(index);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_SMALL_FRACTION )
{
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	pCurrentProcess->FUNCTION_SmallFraction(index);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_PAGE_CHANGE )
{
	int index = atol( pParam->GetValue( "Index" ) );

	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_GoPage(index);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_BUY_ITEM )
{
	int index = atol( pParam->GetValue( "Index" ) );
	
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_BuyItem(index);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_GIFT_ITEM )
{
	//int index = atol( pParam->GetValue( "Index" ) );

	//CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	//assert(pCurrentProcess);

	//CShopDialog::GetPtr()->FUNCTION_InitShopDialog(
	//	pCurrentProcess->m_CurrentItemList[index].sName, 
	//	pCurrentProcess->m_CurrentItemList[index].sIconFile, pCurrentProcess->m_CurrentItemList[index].nCode, 
	//	pCurrentProcess->m_CurrentItemList[index].nSell, index, GIFTITEM_KIND
	//);
}

IMPLEMENT_USERFUNCTION( ITEMSHOP_EXCHANGE_ITEM )
{
	int index = atol( pParam->GetValue( "Index" ) );
	
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->FUNCTION_ExchangeItem(index);
}

IMPLEMENT_USERFUNCTION( REFRESH_ITEM )
{
	CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_REFRESHITEM, 20210, CMessageBoxDialog::MSGBOXTYPE_CONFIRMCANCEL);
}

IMPLEMENT_USERFUNCTION( WearItemButton )
{

}

IMPLEMENT_USERFUNCTION( CharacterEquipInit )
{
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->CharacterEquipInit();
}

IMPLEMENT_USERFUNCTION( SHOP_CharacterRefresh_Button )
{
}

IMPLEMENT_USERFUNCTION( ItemShop_Render ) //렌더링 영역 1
{
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	const char* windowstr = pParam->GetValue( "Window" ); 
	if (windowstr == NULL) return;
	pCurrentProcess->FUNCTION_3DRender(windowstr);
}

/////////////////////////////////////////////////////////////////////////////////////
//ItemShopDialog


IMPLEMENT_USERFUNCTION( BUY_ITEM_POINT )
{
	CShopDialog::GetPtr()->FUNCTION_BuyPointToCoin(BUY_KIND_POINT);
}

IMPLEMENT_USERFUNCTION( BUY_ITEM_COIN )
{
	CShopDialog::GetPtr()->FUNCTION_BuyPointToCoin(BUY_KIND_CASH);
}

IMPLEMENT_USERFUNCTION( BUY_ITEM_OK )
{
	CShopDialog::GetPtr()->FUNCTION_BuyItemOk();
}

IMPLEMENT_USERFUNCTION( GIFT_ITEM_OK )
{
}

IMPLEMENT_USERFUNCTION( EXCHANGE_ITEM_OK )
{
	CShopDialog::GetPtr()->FUNCTION_ExchangeItemOk();
}

//IMPLEMENT_USERFUNCTION( GiftItemOk )
//{
//	CShopDialog::GetPtr()->FUNCTION_GiftItemOk();
//}
//
//IMPLEMENT_USERFUNCTION( ExchangeItemOk )
//{
//	CShopDialog::GetPtr()->FUNCTION_ExchangeItemOk();
//}

//IMPLEMENT_USERFUNCTION( BuyItemCombo0 )
//{
//	CShopDialog::GetPtr()->FUNCTION_BuyItemCombo(0);
//}
//
//IMPLEMENT_USERFUNCTION( BuyItemCombo1 )
//{
//	CShopDialog::GetPtr()->FUNCTION_BuyItemCombo(1);
//}
//
//IMPLEMENT_USERFUNCTION( BuyItemCombo2 )
//{
//	CShopDialog::GetPtr()->FUNCTION_BuyItemCombo(2);
//}

// 아이템 칼라 변경

IMPLEMENT_USERFUNCTION( COLOR_NEXT )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(temp);
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_ColorNext(index);
}

IMPLEMENT_USERFUNCTION( COLOR_PREV )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(temp);
   	assert(pCurrentProcess); 
	pCurrentProcess->FUNCTION_ColorPrev(index);
}

IMPLEMENT_USERFUNCTION( ITEM_DIVEST )
{
	const char* indexstr = pParam->GetValue( "Index" );
	if ( indexstr == 0 ) return;
	int index = atol( indexstr );

	CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();
	CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(temp);
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_DivestItem(index);
}