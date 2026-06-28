//***********************************************************************************
//	File		:	ShopDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "StdAfx.h"
#include "ShopDialog.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "ResTableFormat.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

CShopDialog::CShopDialog(void)
{
}

CShopDialog::~CShopDialog(void)
{
}

void CShopDialog::FUNCTION_ShopInit()
{
	m_nDialog = 0;
	m_nItemIndex = 0;
}

void CShopDialog::FUNCTION_InitShopDialog( CObjItemInfo* pItem, int nBigFraction, int nItemIndex, char nKind, int nPage )
{
	char sText[STRLEN_64]="";

	m_cItemInfo = pItem;
	memcpy( m_nOptionCode, pItem->m_nOptionCode, sizeof(int)*LIST5_SIZE );
	m_nPage			= nPage;
	m_nDialog		= nKind;
	m_nItemIndex	= nItemIndex;

	m_nBigFraction = nBigFraction;			

	FUNCTION_ItemType();						// 구입, 선물, 교환, 카트

	m_nCoinPrice = pItem->m_pTable->m_nCash;
	m_nPointPrice = pItem->m_pTable->m_nPoint;

	switch( nBigFraction )
	{
	case 0:
		m_nPointPrice = pItem->m_nPrice;
		FUNCTION_InitnSellUI(BUY_KIND_POINT);

		UIFunction::Set_UIVisibility( "Option_Num", N3UI_VISIBLE_TRUE );
		break;
	case 1:

		m_nCoinPrice = pItem->m_pTable->m_nCash*((float)pItem->m_pTable->m_nExchange/100);
		m_nPointPrice = pItem->m_pTable->m_nPoint*((float)pItem->m_pTable->m_nExchange/100);
		FUNCTION_InitnSellUI(pItem->m_pTable->m_nSell);

		UIFunction::Set_UIVisibility( "Option_Num", N3UI_VISIBLE_FALSE );
		break;
	case 2:
	case 3:
	case 4:
		
		FUNCTION_InitnSellUI(BUY_KIND_BOTH);

		for(int i=0;i<LIST5_SIZE;i++)
		{
			_snprintf(sText, STRLEN_64, "GradeBG0%d", i+1);
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);

			_snprintf( sText, MAX_PATH, "DEnchant_%d", i );
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

			_snprintf(sText, STRLEN_64, "Option_%d", i);
			UIFunction::Set_ControlText( sText, "" );

			UIFunction::Set_UIVisibility( "Option_Num", N3UI_VISIBLE_FALSE );
		}
		UIFunction::Set_UIVisibility( "ItemLimitLevel", N3UI_VISIBLE_TRUE );
	}
}

void CShopDialog::FUNCTION_InitShopDialog( CResAiCardTableFormat* pItem, int nBigFraction, int nItemIndex, char nKind, int nPage)
{
	char sText[STRLEN_64]="";

	m_cCardInfo = pItem;
	m_nPage			= nPage;
	m_nDialog		= nKind;
	m_nItemIndex	= nItemIndex;

	m_nBigFraction = nBigFraction;			

	FUNCTION_ItemType();						// 구입, 선물, 교환, 카트

	m_nCoinPrice = pItem->m_nCash;
	m_nPointPrice = pItem->m_nPoint;

	FUNCTION_InitnSellUI(BUY_KIND_BOTH);

	for(int i=0;i<LIST5_SIZE;i++)
	{
		_snprintf(sText, STRLEN_64, "GradeBG0%d", i+1);
		UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);

		_snprintf( sText, MAX_PATH, "DEnchant_%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

		_snprintf(sText, STRLEN_64, "Option_%d", i);
		UIFunction::Set_ControlText( sText, "" );
	}

	UIFunction::Set_UIVisibility( "Option_Num", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ItemLimitLevel", N3UI_VISIBLE_FALSE );
}

void CShopDialog::FUNCTION_ItemType()
{
	char sText[STRLEN_64]="";

	CResMessageTableFormat* pMTable = NULL;

	UIFunction::Set_UIVisibility( "DBuyItemBtn", N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility( "DGiftItemBtn", N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility( "DExchangeItemBtn", N3UI_VISIBLE_FALSE);
	
	switch(m_nDialog)
	{
	case BUYITEM_KIND://아템구매
		{
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900031 ,0));
			assert(pMTable);

			UIFunction::Set_ControlText( "ItemDialogTitle", pMTable->m_sMessage );
			UIFunction::Set_UIVisibility( "DBuyItemBtn", N3UI_VISIBLE_TRUE);
	
			if( m_nBigFraction == 2 )
				FUNCTION_BuyCard();
			else
				FUNCTION_BuyItem();
		}
		break;
	//case GIFTITEM_KIND://선물하기
	//	{
	//		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900032 ,0));
	//		assert(pMTable);

	//		UIFunction::Set_ControlText( "ItemDialogTitle", pMTable->m_sMessage );
	//		UIFunction::Set_UIVisibility( "DGiftItemBtn", N3UI_VISIBLE_TRUE);
	
	//	}
	//	break;
	case EXCHANGEITEM_KIND://교환하기
		{
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900033 ,0));
			assert(pMTable);

			UIFunction::Set_ControlText( "ItemDialogTitle", pMTable->m_sMessage );
			FUNCTION_ExchangeItem();
			UIFunction::Set_UIVisibility( "DExchangeItemBtn", N3UI_VISIBLE_TRUE);
		}
		break;
	default:
		return;
	}
}

void CShopDialog::FUNCTION_BuyItem()
{
	char sText[STRLEN_64]="";
	CUIControl* pDialogBG = CClientUIManager::GetPtr()->FindEventControl("ItemDetailBG");
	assert(pDialogBG);

	if(m_cItemInfo->m_nCode > 0)
	{
		UIFunction::Set_ChildControlText( pDialogBG, "ItemName", m_cItemInfo->m_pTable->m_sName );

		switch(m_cItemInfo->m_nGrade)
		{
		case 1:	UIFunction::Set_Color(pDialogBG, "ItemName", 0xffcacaca);	break;
		case 2:	UIFunction::Set_Color(pDialogBG, "ItemName", 0xffb3fe00);	break;
		case 3:	UIFunction::Set_Color(pDialogBG, "ItemName", 0xff0496f4);	break;
		case 4:	UIFunction::Set_Color(pDialogBG, "ItemName", 0xffff56ed);	break;
		case 5:	UIFunction::Set_Color(pDialogBG, "ItemName", 0xffffb400);	break;
		}

		for(int i=1;i<=LIST5_SIZE;i++)
		{
			_snprintf(sText, STRLEN_64, "GradeBG0%d", i);
			if( i == m_cItemInfo->m_nGrade )
				UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_TRUE);
			else
				UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
		}

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		CUIControl* pItemImg = pDialogBG->FindEventControl("ListImg");
		assert(pItemImg);

		if( pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
			_snprintf(sText, MAX_PATH, "Icon/Item/%s.png", m_cItemInfo->m_pTable->m_sIconFile20);
		else
			_snprintf(sText, MAX_PATH, "Icon/Item/%s.png", m_cItemInfo->m_pTable->m_sIconFile);

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);

		_snprintf( sText, STRLEN_64, "LEVEL %d", m_cItemInfo->m_nLevel );
		UIFunction::Set_ControlText( "ItemLimitLevel", sText );

		if( pObjPlayerInfo->m_cLevel.m_nLevel < m_cItemInfo->m_nLevel )
			UIFunction::Set_Color("ItemLimitLevel", 0xffff0000);
		else
			UIFunction::Set_Color("ItemLimitLevel", 0xff000000);

		int nEnchant = 0;
		// 강화 횟수 측정
		for( int i=0;i<LIST5_SIZE;i++)
		{
			_snprintf( sText, MAX_PATH, "DEnchant_%d", i );
			CUIControl* pEOn = CClientUIManager::GetPtr()->FindEventControl( sText );

			if( i < m_cItemInfo->m_nGrade )
				pEOn->SetVisible(N3UI_VISIBLE_TRUE);
			else
				pEOn->SetVisible(N3UI_VISIBLE_FALSE);
			
			int nOption = m_cItemInfo->m_nOptionCode[i];

			if( nOption != 0 )
			{
				nEnchant++;
				UIFunction::ChangeTexture( pEOn, "EnchantOn" );

				int nSituationCode	= ITEM_TYPE_SITUATION(nOption);
				int nFacultyCode	= ITEM_TYPE_FACULTY(nOption);
				int nFaculty		= ITEM_TYPE_VALUE(nOption);

				char sOption[STRLEN_64]="";

				_snprintf(sOption, STRLEN_64, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);
				_snprintf(sText, STRLEN_64, "Option_%d", i);
				UIFunction::Set_ControlText( sText, sOption );
			}
			else
			{
				_snprintf(sText, STRLEN_64, "Option_%d", i);
				UIFunction::Set_ControlText( sText, "" );
				UIFunction::ChangeTexture( pEOn, "EnchantOff" );
				continue;
			}
		}

		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900101 ,0));
		assert(pMTable);
		_snprintf( sText, STRLEN_64, pMTable->m_sMessage, m_cItemInfo->m_nGrade - nEnchant );
		UIFunction::Set_ControlText( "Option_Num", sText );
	}
}

void CShopDialog::FUNCTION_BuyCard()
{
	char sText[STRLEN_64]="";
	CUIControl* pDialogBG = CClientUIManager::GetPtr()->FindEventControl("ItemDetailBG");
	assert(pDialogBG);

	if(m_cCardInfo->GetObjCode() != 0)
	{
		UIFunction::Set_ChildControlText( pDialogBG, "ItemName", m_cCardInfo->m_sName );

		UIFunction::Set_Color( pDialogBG, "ItemName", 0xffffffff );

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		CUIControl* pItemImg = pDialogBG->FindEventControl("ListImg");
		assert(pItemImg);

		switch( m_cCardInfo->m_nType )
		{
		case 10:
			_snprintf(sText, MAX_PATH, "Icon/Item/Card_BoosterPack_01.png");
			break;
		case 20:
			_snprintf(sText, MAX_PATH, "Icon/Item/Card_BoosterPack_02.png");
			break;
		case 30:
			_snprintf(sText, MAX_PATH, "Icon/Item/Card_BoosterPack_03.png");
			break;
		}

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);
	}
}

void CShopDialog::FUNCTION_ExchangeItem()
{
	char sText[STRLEN_64]="";

	CUIControl* pDialogBG = CClientUIManager::GetPtr()->FindEventControl("ItemDetailBG");
	assert(pDialogBG);

	if(m_cItemInfo->m_nCode > 0)
	{
		UIFunction::Set_ChildControlText( pDialogBG, "ItemName", m_cItemInfo->m_pTable->m_sName );
		
		UIFunction::Set_Color(pDialogBG, "ItemName", 0xffffffff);

		for(int i=0;i<LIST5_SIZE;i++)
		{
			_snprintf(sText, STRLEN_64, "GradeBG0%d", i+1);
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);

			_snprintf( sText, MAX_PATH, "DEnchant_%d", i );
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

			_snprintf(sText, STRLEN_64, "Option_%d", i);
			UIFunction::Set_ControlText( sText, "" );
		}

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		CUIControl* pItemImg = pDialogBG->FindEventControl("ListImg");
		assert(pItemImg);

		if( pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
			_snprintf(sText, MAX_PATH, "Icon/Item/%s.png", m_cItemInfo->m_pTable->m_sIconFile20);
		else
			_snprintf(sText, MAX_PATH, "Icon/Item/%s.png", m_cItemInfo->m_pTable->m_sIconFile);

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);

		_snprintf( sText, STRLEN_64, "LEVEL %d", m_cItemInfo->m_nLevel );
		UIFunction::Set_ControlText( "ItemLimitLevel", sText );

		if( pObjPlayerInfo->m_cLevel.m_nLevel < m_cItemInfo->m_nLevel )
			UIFunction::Set_Color("ItemLimitLevel", 0xffff0000);
		else
			UIFunction::Set_Color("ItemLimitLevel", 0xff000000);
		
		UIFunction::Set_UIVisibility( "Option_Num", N3UI_VISIBLE_FALSE );
	}
}

void CShopDialog::FUNCTION_InitnSellUI(int nSell)
{
	char sText[STRLEN_64]="";

	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindCoinBtn"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindPointBtn"));
	assert(pChkBox1);

	CUIControl* pCoinPoint = CClientUIManager::GetPtr()->FindEventControl("CoinPointNum");
	assert(pCoinPoint);

	switch(nSell)
	{
	case BUY_KIND_CASH:
		m_nBuyKind = BUY_KIND_CASH; 
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);

		_snprintf(sText, STRLEN_64, "%d", m_nCoinPrice);
		pCoinPoint->SetText(sText);

		UIFunction::Set_UIVisibility( "PointIcon", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CoinIcon", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_TRUE );
		break;
	case BUY_KIND_POINT:
		m_nBuyKind = BUY_KIND_POINT; 
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);

		_snprintf(sText, STRLEN_64, "%d", m_nPointPrice);
		pCoinPoint->SetText(sText);
		
		UIFunction::Set_UIVisibility( "PointIcon", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CoinIcon", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_FALSE );
		break;
	case BUY_KIND_BOTH:
		m_nBuyKind = BUY_KIND_CASH; 
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);

		_snprintf(sText, STRLEN_64, "%d", m_nCoinPrice);
		pCoinPoint->SetText(sText);

		UIFunction::Set_UIVisibility( "PointIcon", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CoinIcon", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_TRUE );
		break;
	default:;
	}
}

void CShopDialog::FUNCTION_BuyPointToCoin(int nSell)
{
	m_nBuyKind = nSell;
	
	char sText[STRLEN_64]="";

	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindCoinBtn"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindPointBtn"));
	assert(pChkBox1);

	CUIControl* pCoinPoint = CClientUIManager::GetPtr()->FindEventControl("CoinPointNum");
	assert(pCoinPoint);

	switch(nSell)
	{
	case BUY_KIND_CASH:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);

		_snprintf(sText, STRLEN_64, "%d", m_nCoinPrice);
		pCoinPoint->SetText(sText);

		UIFunction::Set_UIVisibility( "PointIcon", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CoinIcon", N3UI_VISIBLE_TRUE );				
		break;
	case BUY_KIND_POINT:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);

		_snprintf(sText, STRLEN_64, "%d", m_nPointPrice);
		pCoinPoint->SetText(sText);
		
		UIFunction::Set_UIVisibility( "PointIcon", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CoinIcon", N3UI_VISIBLE_FALSE );				
		break;
	default:;
	}
}

void CShopDialog::FUNCTION_BuyItemOk()
{
	switch( m_nBigFraction )
	{
	case 0:
		PutBuyRandomitem( m_cItemInfo->m_nItemSeq, m_nBuyKind, m_cItemInfo->m_nCode, m_cItemInfo->m_nPrice, m_nOptionCode );
		break;
	case 1:
		break;
	case 2:
		{
			switch(m_nBuyKind)
			{
			case BUY_KIND_CASH:
				PutBuyCardbooster(m_cCardInfo->GetObjCode(), m_nBuyKind, m_cCardInfo->m_nCash);
				break;
			case BUY_KIND_POINT:
				PutBuyCardbooster(m_cCardInfo->GetObjCode(), m_nBuyKind, m_cCardInfo->m_nPoint);
				break;
			}
		}
		break;
	case 3:
	case 4:
		break;
	}
}

//void CShopDialog::FUNCTION_GiftItemOk()
//{
//	CCSGiftItem cPacket;
//
//	cPacket.m_nBuyKind = 0;
//	cPacket.m_nCode = 0;
//	cPacket.m_nOptionCode[0] = 0;
//	cPacket.m_nOptionCode[1] = 0;
//	cPacket.m_nOptionCode[2] = 0;
//	cPacket.m_nOptionCode[3] = 0;
//	cPacket.m_nPrice = 0;
//	cPacket.m_nToPlayerSeq = 0;
//
//	PutGiftItem(&cPacket);
//}
//
void CShopDialog::FUNCTION_ExchangeItemOk()
{
	CCSExchangeItem cPacket;

	cPacket.m_nBuyKind = m_nBuyKind;
	cPacket.m_nCode = m_cItemInfo->m_nCode;
	
	switch(m_nBuyKind)
	{
	case BUY_KIND_POINT:
		cPacket.m_nPrice = m_nPointPrice;
		break;
	case BUY_KIND_CASH:
		cPacket.m_nPrice = m_nCoinPrice;
		break;
	}

	PutExchangeItem(&cPacket);
}