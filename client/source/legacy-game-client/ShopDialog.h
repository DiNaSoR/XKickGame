//***********************************************************************************
//	File		:	ShopDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef __SHOP_DIALOG_H__
#define __SHOP_DIALOG_H__

#include "Singleton.h"

#include "Control.h"

class CShopDialog : public CSingleton<CShopDialog>
{
public:
	CObjItemInfo*					m_cItemInfo;
	CResAiCardTableFormat*			m_cCardInfo;

	char							m_sName[OBJECT_NAME_SIZE];
	char							m_sIconFile[FILE_NAME_SIZE];

	int								m_nBigFraction;

	int								m_nPage;

	int								m_nOptionCode[ITEM_OPTION_SIZE];

	int								m_nBuyKind;
	int								m_nDialog;//다이얼로그 타입
	int								m_nItemIndex;//아이템 순서

	int								m_nPointPrice;
	int								m_nCoinPrice;

	char							m_nGrade;

public:
	CShopDialog(void);
	~CShopDialog(void);
	
	void							FUNCTION_ItemType();
	void							FUNCTION_ShopInit();
	void							FUNCTION_InitnSellUI(int nSell);
	void							FUNCTION_InitShopDialog( CObjItemInfo* pItem, int nBigFraction, int nItemIndex, char nKind, int nPage);
	void							FUNCTION_InitShopDialog( CResAiCardTableFormat* pItem, int nBigFraction, int nItemIndex, char nKind, int nPage);
	void							FUNCTION_BuyPointToCoin(int nSell);
	
	void							FUNCTION_BuyItem();
	void							FUNCTION_BuyCard();
	void							FUNCTION_ExchangeItem();
	//void							FUNCTION_GiftItem();
	
	void							FUNCTION_BuyItemOk();
	void							FUNCTION_ExchangeItemOk();
	//void							FUNCTION_GiftItemOk();
	
};

#endif //__SHOP_DIALOG_H__