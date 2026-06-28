//***********************************************************************************
//	File		:	SkillShopDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef __SKILLSHOP_DIALOG_H__
#define __SKILLSHOP_DIALOG_H__

#include "Singleton.h"

#include "Control.h"

class CSkillShopDialog : public CSingleton<CSkillShopDialog>
{
public:

	char							m_sName[OBJECT_NAME_SIZE];

	char							m_sIconFile[FILE_NAME_SIZE];

	int								m_nSkillSelectCode;//선택된 아이템 코드

	int								m_nTableSell;

	int								m_nBuyKind;

	int								m_nDialog;//다이얼로그 타입

	int								m_nSkillIndex;//아이템 순서

	int								m_nPointPrice;

	int								m_nCoinPrice;

public:
	CSkillShopDialog(void);
	~CSkillShopDialog(void);

//	int								GetItemCode(int nItemIndex) { return m_cShopItemList.m_cShopItemData[nItemIndex].m_nCode; }

	void							FUNCTION_ShopInit();

	void							FUNCTION_ShopUpdate();

	void							FUNCTION_ShopTerminate();

	void							FUNCTION_InitShopDialog(char *sName, char *sIconFile, int nResourceType, int nCode, int nSell, int nItemIndex);

	void							FUNCTION_SkillType();

	void							FUNCTION_BuySkill(int nPointPrice, int nCoinPrice);

	void							FUNCTION_BuyOk(int Index);

	void							FUNCTION_BuySkillOk();

	void							FUNCTION_BuyTrainingOk();

	void							FUNCTION_BuyCeremonyOk();

	void							FUNCTION_InitnSellUI(int nSell);

	void							FUNCTION_BuyPointToCoin(int nSell);

	void							FUNCTION_ShowBuyDialog(int nDialog);

	//void							FUNCTION_CoinUpdate();

	//void							FUNCTION_PointUpdate();
};

#endif //__SKILLSHOP_DIALOG_H__