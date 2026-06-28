//***********************************************************************************
//	File		:	SkillShopDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "StdAfx.h"
#include "SkillShopDialog.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "ResTableFormat.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

CSkillShopDialog::CSkillShopDialog(void)
{
}

CSkillShopDialog::~CSkillShopDialog(void)
{
}

void CSkillShopDialog::FUNCTION_ShopInit()
{
	m_nDialog = 0;

	m_nSkillIndex = 0;
}

void CSkillShopDialog::FUNCTION_ShopUpdate()
{
}

void CSkillShopDialog::FUNCTION_ShopTerminate()
{
	
}

void CSkillShopDialog::FUNCTION_InitShopDialog(char *sName, char *sIconFile, int nResourceType, int nCode, int nSell, int nSkillIndex)
{
	m_nDialog = nResourceType;
	m_nSkillIndex = nSkillIndex;

	_snprintf(m_sName, OBJECT_NAME_SIZE, "%s", sName );
	_snprintf(m_sIconFile, FILE_NAME_SIZE, "%s", sIconFile );

	m_nSkillSelectCode = nCode;
	m_nTableSell = nSell;

	FUNCTION_SkillType();

	FUNCTION_ShowBuyDialog(m_nDialog);

	FUNCTION_InitnSellUI(nSell);
}

void CSkillShopDialog::FUNCTION_SkillType()
{
	char text[STRLEN_64];
	for(int i = 0; i < 3; i++)
	{
		sprintf(text, "AskText_img%d", i);
		CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl(text);
		if(pName == NULL) return;
		sprintf(text, "SkillOk%d", i);
		CUIControl* pSkillOk = CClientUIManager::GetPtr()->FindEventControl(text);
		if(pSkillOk == NULL) return;

		if(i == m_nDialog) 
		{
			pName->SetVisible(N3UI_VISIBLE_TRUE);
			pSkillOk->SetVisible(N3UI_VISIBLE_TRUE);
		}
		else 
		{
			pName->SetVisible(N3UI_VISIBLE_FALSE);
			pSkillOk->SetVisible(N3UI_VISIBLE_FALSE);	
		}
	}

	UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_TRUE );

	switch(m_nDialog)
	{
	case 0:// 스킬 구매
		{
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900017 ,0));
			assert(pMTable);

			UIFunction::Set_ControlText( "SkillBuyDialogTitle", pMTable->m_sMessage );
			FUNCTION_BuySkill(GetSkillPoint(m_nSkillSelectCode), GetSkillCash(m_nSkillSelectCode));
		}
		break;
	case 1:// 트레이닝 구매
		{
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900018 ,0));
			assert(pMTable);

			UIFunction::Set_ControlText( "SkillBuyDialogTitle", pMTable->m_sMessage );
			FUNCTION_BuySkill(GetTrainingPoint(m_nSkillSelectCode), GetTrainingCash(m_nSkillSelectCode));
			UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_FALSE );
		}
		break;
	case 2:// 세레모니 구매
		{
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900019 ,0));
			assert(pMTable);

			UIFunction::Set_ControlText( "SkillBuyDialogTitle", pMTable->m_sMessage );
			FUNCTION_BuySkill(GetCeremonyPoint(m_nSkillSelectCode), GetCeremonyCash(m_nSkillSelectCode));
		}
		break;
	}
}

void CSkillShopDialog::FUNCTION_ShowBuyDialog(int nDialog)
{
	switch(nDialog)
	{
	case 0://아템구매
		{
			FUNCTION_BuySkill(GetSkillPoint(m_nSkillSelectCode), GetSkillCash(m_nSkillSelectCode));
		}
		break;
	case 1://선물하기
		{
			FUNCTION_BuySkill(GetTrainingPoint(m_nSkillSelectCode), GetTrainingCash(m_nSkillSelectCode));
		}
		break;
	case 2://교환하기
		{
			FUNCTION_BuySkill(GetCeremonyPoint(m_nSkillSelectCode), GetCeremonyCash(m_nSkillSelectCode));
		}
		break;
	}
}

void CSkillShopDialog::FUNCTION_InitnSellUI(int nSell)
{
	char sText[STRLEN_64]="";

	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindCoinBtn"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindPointBtn"));
	assert(pChkBox1);

	CUIControl* pSkillPoint = CClientUIManager::GetPtr()->FindEventControl("SHOP_SkillPoint");
	assert(pSkillPoint);
	CUIControl* pSkillCoin = CClientUIManager::GetPtr()->FindEventControl("SHOP_SkillCoin");
	assert(pSkillCoin);

	switch(nSell)
	{
	case BUY_KIND_CASH:
		m_nBuyKind = BUY_KIND_CASH;
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);

		_snprintf(sText, STRLEN_64, "%d", m_nCoinPrice);
		pSkillCoin->SetText(sText);
		pSkillCoin->SetVisible(N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToVisibility( "SkillDetailBG", "PointIcon", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( "SkillDetailBG", "CoinIcon", N3UI_VISIBLE_TRUE );				

		pSkillPoint->SetVisible(N3UI_VISIBLE_FALSE);

		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_FALSE );
		break;
	case BUY_KIND_POINT:
		m_nBuyKind = BUY_KIND_POINT;
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);

		_snprintf(sText, STRLEN_64, "%d", m_nPointPrice);
		pSkillPoint->SetText(sText);
		pSkillPoint->SetVisible(N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToVisibility( "SkillDetailBG", "PointIcon", N3UI_VISIBLE_TRUE );
		UIFunction::SetChildToVisibility( "SkillDetailBG", "CoinIcon", N3UI_VISIBLE_FALSE );

		pSkillCoin->SetVisible(N3UI_VISIBLE_FALSE);

		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_TRUE );
		break;
	case BUY_KIND_BOTH:
		m_nBuyKind = BUY_KIND_CASH;
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);

		_snprintf(sText, STRLEN_64, "%d", m_nCoinPrice);
		pSkillCoin->SetText(sText);

		_snprintf(sText, STRLEN_64, "%d", m_nPointPrice);
		pSkillPoint->SetText(sText);

		pSkillCoin->SetVisible(N3UI_VISIBLE_TRUE);
		pSkillPoint->SetVisible(N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToVisibility( "SkillDetailBG", "PointIcon", N3UI_VISIBLE_TRUE );
		UIFunction::SetChildToVisibility( "SkillDetailBG", "CoinIcon", N3UI_VISIBLE_TRUE );

		UIFunction::Set_UIVisibility( "CoinBox", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "PointBox", N3UI_VISIBLE_TRUE );
		break;
	default:;
	}
}

void CSkillShopDialog::FUNCTION_BuyPointToCoin(int nSell)
{
	m_nBuyKind = nSell;

	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindCoinBtn"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("BuyKindPointBtn"));
	assert(pChkBox1);

	switch(nSell)
	{
	case BUY_KIND_CASH:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		break;
	case BUY_KIND_POINT:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		break;
	default:;
	}
}

void CSkillShopDialog::FUNCTION_BuySkill(int nPointPrice, int nCoinPrice)
{
	char text[STRLEN_64];

	if(m_nSkillSelectCode > 0)
	{
		CUIControl* pSkillName = CClientUIManager::GetPtr()->FindEventControl("SkillName_Text");//아이템 이름
		assert(pSkillName);
				
		if(strlen(m_sName)>20)
		{
			char* p;
			_snprintf(text, STRLEN_64, "%s", m_sName );
			p = &text[19];
			while(true)
			{
				if( *p == ' ' || *p == '(' )
				{
					*p='\0';
					strcat(text, "..");
					break;
				}
				else
					p--;
			}
			if(pSkillName) pSkillName->SetText(text);
		}
		else
			if(pSkillName) pSkillName->SetText(m_sName);

		CUIControl* pSkillImg = CClientUIManager::GetPtr()->FindEventControl("SKILLSHOP_BuyImg");
		if(pSkillImg == NULL) return;

		switch(m_nDialog)
		{
		case 0:
			{
				sprintf(text, "Icon/Skill/%s.png", m_sIconFile);
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pSkillImg, text);

				CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nSkillSelectCode, 0));
				assert(pSkillTable);
				_snprintf( text, STRLEN_64, "LEVEL %d", pSkillTable->m_nLimit );
				UIFunction::Set_UIVisibility( "SkillLimitLevel", N3UI_VISIBLE_TRUE );
				UIFunction::Set_ControlText( "SkillLimitLevel", text );
			}
			break;
		case 1:
			{
				sprintf(text, "Icon/Training/%s.png", m_sIconFile);
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pSkillImg, text);
				UIFunction::Set_UIVisibility( "SkillLimitLevel", N3UI_VISIBLE_FALSE );
			}
			break;
		case 2:
			{
				sprintf(text, "Icon/Ceremony/%s.png", m_sIconFile);
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pSkillImg, text);
				UIFunction::Set_UIVisibility( "SkillLimitLevel", N3UI_VISIBLE_FALSE );
			}
			break;
		}		

		m_nPointPrice = nPointPrice;
		m_nCoinPrice = nCoinPrice;
	}
}

void CSkillShopDialog::FUNCTION_BuyOk(int Index)
{
	switch(Index)
	{
	case 0:// 스킬 구매
		{
			FUNCTION_BuySkillOk();
		}
		break;
	case 1:// 트래이닝 구매
		{
			FUNCTION_BuyTrainingOk();
		}
		break;
	case 2:// 세레모니 구매
		{
			FUNCTION_BuyCeremonyOk();
		}
		break;
	}	
}

void CSkillShopDialog::FUNCTION_BuySkillOk()
{
	CCSBuySkill cPacket;

	cPacket.m_nBuyKind = m_nBuyKind;
	cPacket.m_nCode = m_nSkillSelectCode;
	if(m_nBuyKind == BUY_KIND_POINT)
		cPacket.m_nPrice = m_nPointPrice;
	else
		cPacket.m_nPrice = m_nCoinPrice;

	PutBuySkill(&cPacket);
}

void CSkillShopDialog::FUNCTION_BuyTrainingOk()
{
	CCSBuyTraining cPacket;

	cPacket.m_nBuyKind = m_nBuyKind;
	cPacket.m_nCode = m_nSkillSelectCode;
	if(m_nBuyKind == BUY_KIND_POINT)
		cPacket.m_nPrice = m_nPointPrice;
	else
		cPacket.m_nPrice = m_nCoinPrice;

	PutBuyTraining(&cPacket);
}

void CSkillShopDialog::FUNCTION_BuyCeremonyOk()
{
	CCSBuyCeremony cPacket;

	cPacket.m_nBuyKind = m_nBuyKind;
	cPacket.m_nCode = m_nSkillSelectCode;
	if(m_nBuyKind == BUY_KIND_POINT)
		cPacket.m_nPrice = m_nPointPrice;
	else
		cPacket.m_nPrice = m_nCoinPrice;

	PutBuyCeremony(&cPacket);
}


//void CSkillShopDialog::FUNCTION_PointUpdate()
//{
//	char text[STRLEN_64]="";
//
//	CUIControl* pBuyItemBG = CClientUIManager::GetPtr()->FindEventControl("Skillbuyitem_bg");
//	assert(pBuyItemBG);
//	
//	CObjMemberInfo* pMember	= CObjMemberManager::GetPtr()->GetMemberPointer();
//	
//	int nPointPrice = 0;
//
//	switch(m_nDialog)
//	{
//	case 0:
//		nPointPrice = GetSkillPoint(m_nSkillSelectCode);
//		break;
//	case 1:
//		nPointPrice = GetTrainingPoint(m_nSkillSelectCode);
//		break;
//	case 2:
//		nPointPrice = GetCeremonyPoint(m_nSkillSelectCode);
//		break;
//	}
//	
//	// 보유 포인트 표시
//	CUIControl* pRemainPointT = pBuyItemBG->FindEventControl("leftpricetext_static_S");
//	assert(pRemainPointT);
//
//	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900020 ,0));
//	assert(pMTable);
//
//	pRemainPointT->SetText(pMTable->m_sMessage);
//
//	// 구입 포인트 표시
//	CUIControl* pBuyPointT = pBuyItemBG->FindEventControl("buytext_Static_S");
//	assert(pBuyPointT);
//
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900021 ,0));
//	assert(pMTable);
//
//	pBuyPointT->SetText(pMTable->m_sMessage);
//
//	// 구매후 남은 포인트 표시
//	CUIControl* pTotalPointT = pBuyItemBG->FindEventControl("totallefttext_static_S");
//	assert(pTotalPointT);
//
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900022 ,0));
//	assert(pMTable);
//
//	pTotalPointT->SetText(pMTable->m_sMessage);
//
//	// 보유 포인트 출력
//	CUIControl* pRemainPoint = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemKeep");
//	assert(pRemainPoint);
//	sprintf(text, "%d", pMember->m_cMoney.m_nPoint);
//	pRemainPoint->SetText(text);
//
//	// 구입 포인트 출력
//	CUIControl* pBuyPoint = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemBuy");
//	assert(pBuyPoint);
//	sprintf(text, "%d", nPointPrice);
//	pBuyPoint->SetText(text);
//
//	// 구매후 남은 포인트
//	CUIControl* pTotalPoint = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemTotal");
//	assert(pTotalPoint);
//	sprintf(text, "%d", pMember->m_cMoney.m_nPoint-nPointPrice);
//	pTotalPoint->SetText(text);
//
//	// 단위 출력
//	CUIControl* pText = pBuyItemBG->FindEventControl("value_point_Static_S");
//	assert(pText);
//
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900023 ,0));
//	assert(pMTable);
//
//	pText->SetText(pMTable->m_sMessage);
//	pText = pBuyItemBG->FindEventControl("valuetext_Static1_S");
//	pText->SetText(pMTable->m_sMessage);
//	pText = pBuyItemBG->FindEventControl("valuetext_Static2_S");
//	pText->SetText(pMTable->m_sMessage);
//}
//
//void CSkillShopDialog::FUNCTION_CoinUpdate()
//{
//	char text[STRLEN_64]="";
//
//	CUIControl* pBuyItemBG = CClientUIManager::GetPtr()->FindEventControl("Skillbuyitem_bg");
//	assert(pBuyItemBG);
//	
//	CObjMemberInfo* pMember	= CObjMemberManager::GetPtr()->GetMemberPointer();
//	
//	int nCashPrice = 0;
//
//	switch(m_nDialog)
//	{
//	case 0:
//		nCashPrice = GetSkillCash(m_nSkillSelectCode);
//		break;
//	case 1:
//		nCashPrice = GetTrainingCash(m_nSkillSelectCode);
//		break;
//	case 2:
//		nCashPrice = GetCeremonyCash(m_nSkillSelectCode);
//		break;
//	}
//	
//    // 보유 코인 표시
//	CUIControl* pRemainCoinT = pBuyItemBG->FindEventControl("leftpricetext_static_S");
//	assert(pRemainCoinT);
//
//	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900024 ,0));
//	assert(pMTable);
//
//	pRemainCoinT->SetText(pMTable->m_sMessage);
//
//	// 구입 코인 표시
//	CUIControl* pBuyCoinT = pBuyItemBG->FindEventControl("buytext_Static_S");
//	assert(pBuyCoinT);
//
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900025 ,0));
//	assert(pMTable);
//
//	pBuyCoinT->SetText(pMTable->m_sMessage);
//
//	// 구매후 남은 코인 표시
//	CUIControl* pTotalCoinT = pBuyItemBG->FindEventControl("totallefttext_static_S");
//	assert(pTotalCoinT);
//
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900026 ,0));
//	assert(pMTable);
//
//	pTotalCoinT->SetText(pMTable->m_sMessage);
//
//	// 보유 코인 출력
//	CUIControl* pRemainCoin = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemKeep");
//	assert(pRemainCoin);
//	sprintf(text, "%d", pMember->m_cMoney.m_nCash);
//	pRemainCoin->SetText(text);
//
//	// 구입 코인 출력
//	CUIControl* pBuyCoin = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemBuy");
//	assert(pBuyCoin);
//	sprintf(text, "%d", nCashPrice);
//	pBuyCoin->SetText(text);
//
//	// 구매후 남은 코인
//	CUIControl* pTotalCoin = pBuyItemBG->FindEventControl("SKILLSHOP_BuyItemTotal");
//	assert(pTotalCoin);
//	sprintf(text, "%d", pMember->m_cMoney.m_nCash-nCashPrice);
//	pTotalCoin->SetText(text);
//
//	// 단위 출력
//	CUIControl* pText = pBuyItemBG->FindEventControl("value_point_Static_S");
//	assert(pText);
//	
//	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900027 ,0));
//	assert(pMTable);
//
//	pText->SetText(pMTable->m_sMessage);
//	pText = pBuyItemBG->FindEventControl("valuetext_Static1_S");
//	pText->SetText(pMTable->m_sMessage);
//	pText = pBuyItemBG->FindEventControl("valuetext_Static2_S");
//	pText->SetText(pMTable->m_sMessage);
//}