#include "stdafx.h"
#include "InvenDialog.h"
#include "ResTableStack.h"
#include "UIFunction.h"
#include "UICommonMethod.h"
#include "UIButton.h"
#include "PutPacket.h"
#include "../InterfaceLibrary/UIProgress.h"

CInvenDialog::CInvenDialog(void)
{
	m_nSmallFractionStart	= 0;
	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호

	m_nCurrentPage			= 1;
	m_nTotalPage			= 0;

	m_nEntry				= -1;
}

CInvenDialog::~CInvenDialog(void)
{

}

void CInvenDialog::InitInven()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	memset(m_arrSkill, 0, sizeof(CObjSkillInfo)*50);
	pObjPlayerInfo->GetBagSkillAllList(m_arrSkill);

	memcpy(m_nInvenEquipWear, pObjPlayerInfo->m_nEquipWear, sizeof(m_nInvenEquipWear));

	m_vEDList.clear();

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(0);

	UIFunction::Set_UIVisibility( "TimeDelayStc", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "TimeDelayPrg", N3UI_VISIBLE_FALSE );
}

void CInvenDialog::FUNCTION_InitFraction()
{
	CResFractionTableStack *temp = dynamic_cast<CResFractionTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_FRACTIONTABLE));	
	assert(temp);
	
	for(int i=0;i<LIST5_SIZE;i++)
	{
		m_vFractionList[i].clear();
		m_vSmallFractionTypeList[i].clear();
	}

	for(int i=0;i<temp->GetResourceSize();i++)
	{
		// 아이템 분류 테이블 로딩
		CResFractionTableFormat *pTable = dynamic_cast<CResFractionTableFormat*>(temp->GetSequenceField(i));
		assert(pTable);

		if( pTable->GetObjCode()/1000 == INVEN_FRACTION)
		{
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
			m_vSmallFractionTypeList[pTable->m_nBigFractionIndex].push_back(pTable->m_nTypeCode);
		}
	}
}

void CInvenDialog::FUNCTION_BigFraction(int nBigFraction)
{
	m_nBigFractionIndex		= nBigFraction;
	m_nSmallFractionStart	= 0;
	//m_nCurrentPage			= 1;

	char sText[STRLEN_64]="";

	for(int i = 0; i < 3; i++)
	{
		_snprintf( sText, MAX_PATH, "TOP_TabButton0%d", i );
		CUIButton* pBigFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));	
		assert(pBigFraction);

		if( nBigFraction == i )
			pBigFraction->SetPressed(true);
		else
			pBigFraction->SetPressed(false);
	}

	FUNCTION_SmallFraction(0);
	FUNCTION_SmallFractionInit(nBigFraction);

	UIFunction::Set_UIVisibility( "Inven_00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Inven_01", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Inven_02", N3UI_VISIBLE_FALSE );

	switch( nBigFraction )
	{
	case 0:
		FUNCTION_InitItemInven();
		UIFunction::Set_UIVisibility( "Inven_00", N3UI_VISIBLE_TRUE );
		break;
	case 1:
		PutCardEntry(CARD_ENTRY_GET);
		UIFunction::Set_UIVisibility( "Inven_01", N3UI_VISIBLE_TRUE );
		break;
	case 2:
		FUNCTION_InitSkillInven();
		UIFunction::Set_UIVisibility( "Inven_02", N3UI_VISIBLE_TRUE );
		break;
	}	
}

void CInvenDialog::FUNCTION_SmallFractionInit(int nBigFraction)
{
	char sText[MAX_PATH]="";

	if( !(int)m_vFractionList[nBigFraction].size() )
	{
		for(int i=0;i<LIST5_SIZE;i++)
		{
			_snprintf( sText, MAX_PATH, "BOTTOM_TabButton0%d", i );
			UIFunction::Set_ControlText( sText, "" );
		}
		return;
	}

	if( m_nSmallFractionStart < 0 )																// 예외처리
		m_nSmallFractionStart = 0;
	else if( (int)m_vFractionList[nBigFraction].size() <= LIST5_SIZE )							// 5개 미만일 경우
		m_nSmallFractionStart = 0;
	else if( (int)m_vFractionList[nBigFraction].size() <= m_nSmallFractionStart+LIST5_SIZE )	// 총개수 초과시
		m_nSmallFractionStart = m_vFractionList[nBigFraction].size() - LIST5_SIZE;

	FUNCTION_SmallFraction(0);

	for(int i=0;i<LIST5_SIZE;i++)
	{
		_snprintf( sText, MAX_PATH, "BOTTOM_TabButton0%d", i );
		
		if( (int)m_vFractionList[nBigFraction].size() <= m_nSmallFractionStart+i )
			UIFunction::Set_ControlText( sText, "" );
		else
			UIFunction::Set_ControlText( sText, m_vFractionList[nBigFraction].at(m_nSmallFractionStart+i).c_str() );
	}
}

void CInvenDialog::FUNCTION_SmallFraction(int nSmallFractionIndex)
{
	m_nSmallFractionIndex = nSmallFractionIndex;

	m_nCurrentPage = 1;

	char sText[STRLEN_64]="";

	for(int i = 0; i < 3; i++)
	{
		_snprintf(sText, STRLEN_64, "BOTTOM_TabButton0%d", i);
		CUIButton* pSmallFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pSmallFraction);

		if(m_nSmallFractionIndex == i)
			pSmallFraction->SetPressed(true);
		else 
			pSmallFraction->SetPressed(false);
	}

	switch(m_nBigFractionIndex)
	{
	case 0:
		FUNCTION_GetItemBagList();
		break;
	case 1:
		if( m_nEntry != -1 )
			FUNCTION_GetEntryList();
		break;
	case 2:
		FUNCTION_GetSkillBagList();
		break;
	}
}

void CInvenDialog::FUNCTION_GetItemBagList()
{
	int nListCount = 12;

	memset(m_arrItem, 0, sizeof(m_arrItem));

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	// 빈 슬록 예외처리
	if( (int)m_vSmallFractionTypeList[m_nBigFractionIndex].size() <= (m_nSmallFractionStart+m_nSmallFractionIndex) ) return;

	int nType = m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+m_nSmallFractionIndex);
	int nCount = pObjPlayerInfo->GetBagItemAllList( nType, m_arrItem );
	
	m_nTotalPage = nCount/nListCount;

	if( nCount%nListCount )	m_nTotalPage++;

	int nPageNum = 12*(m_nCurrentPage-1);

	for(int i = 0; i < nListCount; i++)
	{
		if(m_arrItem[i + nPageNum].m_nCode == 0)
		{
			m_nItemCode[i] = 0;
			FUNCTION_ImageRender(i, 0, 0, NULL);
		}
		else
		{
			m_nItemCode[i] = m_arrItem[i + nPageNum].m_nCode;
			m_nItemSeq[i] = m_arrItem[i + nPageNum].m_nItemSeq;

			int nEquip = m_arrItem[i + nPageNum].m_nEquipKind;

			ED_VECTORLIST::iterator it = find(m_vEDList.begin(), m_vEDList.end(), m_arrItem[i + nPageNum].m_nItemSeq);
			if( it != m_vEDList.end() )
			{
				if( m_arrItem[i + nPageNum].m_nEquipKind == EQUIP_KIND_YES )
					nEquip = EQUIP_KIND_NO;
				else
					nEquip = EQUIP_KIND_YES;
			}

			CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_nItemCode[i], 0));			
			assert(pTable);
			if( pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
				FUNCTION_ImageRender(i, m_nItemCode[i], nEquip, pTable->m_sIconFile20);
			else
				FUNCTION_ImageRender(i, m_nItemCode[i], nEquip, pTable->m_sIconFile);
		}
	}

	FUNCTION_UpdatePage();
}

void CInvenDialog::FUNCTION_GetEntryList()
{
	PutCardEntry(m_nSmallFractionIndex);
}

void CInvenDialog::FUNCTION_GetSkillBagList()
{
	int nListCount = 24;
	
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	int nCount = pObjPlayerInfo->m_vSkillList.size();

	m_nTotalPage = nCount/nListCount;

	if( nCount%nListCount )	m_nTotalPage++;

	int nPageNum = 24*(m_nCurrentPage-1);

	for(int i = 0; i < nListCount; i++)
	{
		if(m_arrSkill[i + nPageNum].m_nCode == 0) 
		{
			m_nSkillCode[i] = 0;
			FUNCTION_ImageRender( i, 0, 0, NULL );
		}
		else
		{
			m_nSkillCode[i] = m_arrSkill[i + nPageNum].m_nCode;

			int nEquip = m_arrSkill[i + nPageNum].m_nEquipKind;

			CResSkillTableFormat *pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nSkillCode[i], 0));			
			assert(pTable);

            if( pTable != 0 )
				FUNCTION_ImageRender( i, m_nSkillCode[i], m_arrSkill[i + nPageNum].m_nEquipKind, pTable->m_sIconFile, m_arrSkill[i].m_nLevel );
		}
	}

	FUNCTION_UpdatePage();
}

void CInvenDialog::FUNCTION_ImageRender( int ItemIndex, int nCode, int nEquip, char *sIconFile, char nSkillLevel )
{
	char sText[STRLEN_64]="";
	
	switch(m_nBigFractionIndex)
	{
	case 0:
		_snprintf(sText, STRLEN_64, "ItemSlot");
		break;
	case 2:
		_snprintf(sText, STRLEN_64, "SkillSlot");
		break;
	}
	
	CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pListBG);

	_snprintf(sText, STRLEN_64, "ListBG%d", ItemIndex);
	CUIControl* pControl = pListBG->FindEventControl(sText);

	if(nCode > 0)
	{
		pControl->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pImg = pControl->FindEventControl("ListImg");//리스트 이미지
		assert(pImg);

		switch(m_nBigFractionIndex)
		{
		case 0:
			_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", sIconFile);
			CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
			break;
		case 2:
			_snprintf(sText, STRLEN_64, "Icon/Skill/%s.png", sIconFile);
			CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
			if( nSkillLevel )
			{
				_snprintf( sText, STRLEN_64, "Level0%d", nSkillLevel );
				UIFunction::ChangeChildTexture( pControl, "SLv", sText );
			}
			break;
		}

		if( nEquip == EQUIP_KIND_YES )								// 착용 및 사용 여부 표시
			UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_TRUE );
		else
			UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );
	}
	else pControl->SetVisible(N3UI_VISIBLE_FALSE);
}

void CInvenDialog::FUNCTION_GoPage(int nPage)
{
	m_nCurrentPage += nPage;

	if( m_nTotalPage < m_nCurrentPage )
		m_nCurrentPage = 1;
	if( m_nCurrentPage <= 0 )
		m_nCurrentPage = m_nTotalPage;
	
	switch(m_nBigFractionIndex)
	{
	case 0:
		FUNCTION_GetItemBagList();
		break;
	case 2:
		FUNCTION_GetSkillBagList();
		break;
	};	
}

void CInvenDialog::FUNCTION_UpdatePage()
{
	char sText[STRLEN_32]="";

	_snprintf(sText, STRLEN_64, "%d/%d", m_nCurrentPage, m_nTotalPage );
	switch( m_nBigFractionIndex )
	{
	case 0:
		UIFunction::Set_ControlText( "InvenItemPageName", sText );
		break;
	case 2:
		UIFunction::Set_ControlText( "InvenSkillPageName", sText );
		break;
	}	
}

void CInvenDialog::FUNCTION_InitItemInven()
{
	char sText[STRLEN_64]="";

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	for(int i = 0; i < MAX_EQUIP; i++)
	{
		if(m_nInvenEquipWear[i] == 0)
			continue;

		if(m_nInvenEquipWear[i] == -1)
		{
			_snprintf(sText, STRLEN_64, "Slot%02d", i);
			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pControl);

			UIFunction::Set_UIVisibility( pControl, "List_Img", N3UI_VISIBLE_FALSE );
			continue;
		}

		CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_nInvenEquipWear[i], 0));
		assert(pTable);
		
		_snprintf(sText, STRLEN_64, "Slot%02d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		CUIControl* pImg = pControl->FindEventControl( "List_Img" );
		assert(pImg);

		if(pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN)
		{
			if(strcmp(pTable->m_sIconFile20, "-") && pTable->m_nOrder >= 100)
			{
				pImg->SetVisible( N3UI_VISIBLE_TRUE );
				_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", pTable->m_sIconFile20);
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
			}
			else
				UIFunction::Set_UIVisibility( pControl, "List_Img", N3UI_VISIBLE_FALSE );
		}
		else
		{
			if(strcmp(pTable->m_sIconFile, "-") && pTable->m_nOrder >= 100)
			{
				pImg->SetVisible( N3UI_VISIBLE_TRUE );
				_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", pTable->m_sIconFile);
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText);
			}
			else
				UIFunction::Set_UIVisibility( pControl, "List_Img", N3UI_VISIBLE_FALSE );
		}
	}
}

void CInvenDialog::SetEntry(char nEntry)
{
	m_nEntry = nEntry;
}

void CInvenDialog::FUNCTION_InitEntryInven(char nEntry)
{
	m_nEntry = nEntry;

	char sText[STRLEN_64]="";

	for(int i = 0; i < 3; i++)
	{
		_snprintf(sText, STRLEN_64, "BOTTOM_TabButton0%d", i);
		CUIButton* pSmallFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pSmallFraction);

		if(nEntry == i)
			pSmallFraction->SetPressed(true);
		else 
			pSmallFraction->SetPressed(false);
	}

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	char nUserPositionKind = pObjPlayerInfo->m_nPosition/LIST10_SIZE;
	
	if( 3<nUserPositionKind )
		nUserPositionKind = 6;

	for(int i = 0; i < 5; i++)
	{
		_snprintf(sText, STRLEN_64, "CardBG%d", i);
		CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pEntry);

		if( i==4 )	i++;

		CUIControl* pImg = pEntry->FindEventControl( "ListImg" );
		assert(pImg);
		
		pImg->SetVisible(N3UI_VISIBLE_FALSE);

		if( nUserPositionKind  == i+1 )
			UIFunction::SetChildToVisibility( pEntry, "User_Mark", N3UI_VISIBLE_TRUE );
		else
			UIFunction::SetChildToVisibility( pEntry, "User_Mark", N3UI_VISIBLE_FALSE );

		for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
		{
			char nEquip = pObjPlayerInfo->m_vCardbotList.at(j)->m_nEquipKind[nEntry];
			char nPosition = pObjPlayerInfo->m_vCardbotList.at(j)->m_nPosition;
			char nRank = pObjPlayerInfo->m_vCardbotList.at(j)->m_nRank;
			char nLv = pObjPlayerInfo->m_vCardbotList.at(j)->m_nLevel;
			int nCostume = pObjPlayerInfo->m_vCardbotList.at(j)->m_nCostume;

			if( nEquip == i+1 )
			{
				UICommonMethod::FUNCTION_CardInfoDraw(pImg, nPosition, nRank, nLv, nCostume);

				pImg->SetVisible(N3UI_VISIBLE_TRUE);
				break;
			}
		}
	}
}

void CInvenDialog::FUNCTION_InitSkillInven()
{
	
}

void CInvenDialog::UpdateInven()
{
	switch( m_nBigFractionIndex )
	{
	case 0:
		{
			int nListCount = 12;
			
			for(int i = 0; i < nListCount; i++)
			{
				char sText[STRLEN_64]="";

				_snprintf(sText, STRLEN_64, "ItemSlot");
				
				CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
				assert(pListBG);

				_snprintf(sText, STRLEN_64, "ListBG%d", i);
				CUIControl* pControl = pListBG->FindEventControl(sText);
				assert(pControl);

				if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )		// 마우스 오버 이미지 처리
					UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_FALSE );
			}
		}
		break;
	case 2:
		{
			int nListCount = 24;

			for(int i = 0; i < nListCount; i++)
			{
				char sText[STRLEN_64]="";

				_snprintf(sText, STRLEN_64, "SkillSlot");
				
				CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
				assert(pListBG);

				_snprintf(sText, STRLEN_64, "ListBG%d", i);
				CUIControl* pControl = pListBG->FindEventControl(sText);
				assert(pControl);

				if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )		// 마우스 오버 이미지 처리
					UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_FALSE );		
			}
		}
		break;
	}
}

void CInvenDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		UpdateInven();

		if( m_nBigFractionIndex == 0 )
		{
			FUNCTION_EquipWearToolTipViewUpdate();
			FUNCTION_ItemBagToolTipViewUpdate();
		}
		break;
	}
}

void CInvenDialog::SetSmallFractionStart(int nStart)
{
	m_nSmallFractionStart += nStart;
	CInvenDialog::GetPtr()->FUNCTION_SmallFractionInit(m_nBigFractionIndex);
}

void CInvenDialog::FUNCTION_EquipNDivest(int idx)
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch( m_nBigFractionIndex )
	{
	case 0:
		{
			int nPageNum = 12*(m_nCurrentPage-1);							// 페이지 계산
			int nWear = m_arrItem[idx + nPageNum].m_pTable->m_nWear;		// 클릭 웨어 타입

			// 클릭한 아이템의 착/탈 변화가 있었는지 검색
			ED_VECTORLIST::iterator it = find( m_vEDList.begin(), m_vEDList.end(), m_arrItem[idx + nPageNum].m_nItemSeq );

			// 이전에 변화가 없었다면 토글 할 수 있도록 아이템 번호를 벡터에 넣어주고 있다면 원래대로 돌리도록 아이템 번호를 삭제한다.
			if( it == m_vEDList.end() )
				m_vEDList.push_back( m_arrItem[idx + nPageNum].m_nItemSeq );
			else
				m_vEDList.erase(it);

			// 토글 적용후 해당 아이템이 토글 변화가 있는지 검사
			it = find( m_vEDList.begin(), m_vEDList.end(), m_arrItem[idx + nPageNum].m_nItemSeq );

			bool bEquip = false;

			// 현재 액션이 입는 액션 인지 검사
			if( m_arrItem[idx + nPageNum].m_nEquipKind == EQUIP_KIND_NO && it != m_vEDList.end() )
				bEquip = true;                

			if( m_arrItem[idx + nPageNum].m_nEquipKind == EQUIP_KIND_YES && it == m_vEDList.end() )
				bEquip = true;

			// 입지 않는 액션이라면 벗고 끝냄
			if( !bEquip )
			{
				m_nInvenEquipWear[nWear] = -1;
				CInvenDialog::GetPtr()->FUNCTION_InitItemInven();
				FUNCTION_GetItemBagList();
				break;
			}
			else	// 입는 액션이라면 인벤에 캐릭 EquipWear에 해당 코드 삽입
				m_nInvenEquipWear[nWear] = m_arrItem[idx + nPageNum].m_nCode;

			for( int i=0;i<MAX_INVEN;i++ )
			{
				if( m_arrItem[i].m_nCode == 0 )	break;												// 아이템 정보가 없다면 끝냄
				if( m_arrItem[i].m_nItemSeq == m_arrItem[idx + nPageNum].m_nItemSeq )	continue;	// 현재 클릭한 아이템이면 패스

				it = find( m_vEDList.begin(), m_vEDList.end(), m_arrItem[i].m_nItemSeq );			// 벡터에 토글변화가 있는지 검사

				// 입고 있었다면 토글을 통해 해제
				if( m_arrItem[i].m_pTable->m_nType == m_arrItem[idx + nPageNum].m_pTable->m_nType )
				{
					if( m_arrItem[i].m_nEquipKind == EQUIP_KIND_YES && it == m_vEDList.end() )
						m_vEDList.push_back( m_arrItem[i].m_nItemSeq );

					if( m_arrItem[i].m_nEquipKind == EQUIP_KIND_NO && it != m_vEDList.end() )
						m_vEDList.erase( it );
				}
			}

			CInvenDialog::GetPtr()->FUNCTION_InitItemInven();
			FUNCTION_GetItemBagList();
		}
		break;
	case 2:
		{
			int nPageNum = 24*(m_nCurrentPage-1);

			if( m_arrSkill[idx + nPageNum].m_nEquipKind == EQUIP_KIND_YES )
				m_arrSkill[idx + nPageNum].m_nEquipKind = EQUIP_KIND_NO;
			else
				m_arrSkill[idx + nPageNum].m_nEquipKind = EQUIP_KIND_YES;

			FUNCTION_GetSkillBagList();
		}
		break;
	}
}

void CInvenDialog::FUNCTION_InvenConfirm()
{
	int nCount=0;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	char nSkin = pObjPlayerInfo->m_cShape.m_nSkin;

	pObjPlayerInfo->GetBagItemAllList( ITEM_TYPE_ALL, m_arrItem );
	
	for(int i=0;i<MAX_INVEN;i++)
	{
		if( m_arrItem[i].m_nCode == 0 )	break;

		ED_VECTORLIST::iterator it = find(m_vEDList.begin(), m_vEDList.end(), m_arrItem[i].m_nItemSeq);
		if( it != m_vEDList.end() )
		{
			if( m_arrItem[i].m_nEquipKind == EQUIP_KIND_YES )
				PutDivestItem(m_arrItem[i].m_nItemSeq);
			else
				PutEquipItem(m_arrItem[i].m_nItemSeq);

			//if( m_arrItem[i].m_pTable->m_nType == ITEM_TYPE_FACE || m_arrItem[i].m_pTable->m_nType == ITEM_TYPE_GLOVE )
			//{
			//	nSkin = m_arrItem[i].m_pTable->m_nColor;
			//
			//	for( int j=0; j<MAX_EQUIP; j++ )
			//    {
			//		int nCode = pObjPlayerInfo->m_nEquipWear[j];
			//		if( nCode > 0 )
			//		{
			//			CObjectModelManager::GetPtr()->DeleteModelObject( pObjPlayerInfo->m_pEquipWearModelID[j] );
			//			pObjPlayerInfo->m_pEquipWearModelID[j] = 0;
			//		}
			//	}
			//	CProcessManager::GetPtr()->AttachItem(pObjPlayerInfo);
			//}

			//pObjPlayerInfo->m_nEquipWear[m_arrItem[i].m_pTable->m_nWear] = m_arrItem[i].GetObjCode();
			//pObjPlayerInfo->m_pEquipWearModelID[m_arrItem[i].m_pTable->m_nWear] = 
			//CObjectModelManager::GetPtr()->ExchangeItem(pObjPlayerInfo->m_nEquipWear[m_arrItem[i].m_pTable->m_nWear], nSkin,
			//pObjPlayerInfo->m_pEquipWearModelID[m_arrItem[i].m_pTable->m_nWear], pObjPlayerInfo->m_pModel);
		}

		nCount++;
	}

	for(int i=0;i<MAX_SKILL;i++)
	{
		if( m_arrSkill[i].m_nCode == 0 )	break;

		if( pObjPlayerInfo->m_vSkillList.at(i)->m_nEquipKind == m_arrSkill[i].m_nEquipKind )
			continue;
		else
		{
			if( m_arrSkill[i].m_nEquipKind == EQUIP_KIND_YES )
				PutEquipSkill(m_arrSkill[i].m_nSkillSeq);
			else
				PutDivestSkill(m_arrSkill[i].m_nSkillSeq);

			nCount++;
		}
	}

	if( nCount )
		PutAthleteInfo();
}

void CInvenDialog::FUNCTION_ItemBagToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	char sAllOption[MAX_PATH]="";

	RECT rt;
	
	int nCount = 12;

	CUIControl* pBackPannel = CClientUIManager::GetPtr()->FindEventControl("ItemSlot");
	assert(pBackPannel);

	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("ToolTipI");
	assert(pToolTip);
	pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	
	int i=0;
	for(;i<nCount;i++)
	{
		_snprintf(sText, MAX_PATH, "ListBG%d", i);
		CUIControl* pOverItem = pBackPannel->FindEventControl(sText);
		assert(pOverItem);

		rt = pOverItem->GetWindowBox();
		
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pOverItem->GetVisible() == N3UI_VISIBLE_TRUE)
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			break;
		}
		else
			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	}

	if(nCount == i) return;

	int nPageNum = 12*(m_nCurrentPage-1);

	if( m_arrItem[i + nPageNum].m_nLevel == 0 )
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  CResourceManager::GetPtr()->m_pStaticMap[210020].c_str() );
	else
	{
		_snprintf( sText, MAX_PATH, "LEVEL %d", m_arrItem[i + nPageNum].m_nLevel );
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  sText );
	}

	UIFunction::Set_ChildControlText( pToolTip, "ItemName", m_arrItem[i + nPageNum].m_pTable->m_sName );

	// 등급에 따른 네임색 변경
	switch(m_arrItem[i + nPageNum].m_nGrade)
	{
	case 1:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffcacaca);	break;
	case 2:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffb3fe00);	break;
	case 3:	UIFunction::Set_Color(pToolTip, "ItemName", 0xff0496f4);	break;
	case 4:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffff56ed);	break;
	case 5:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffffb400);	break;
	}

	CUIControl* pEnchant = pToolTip->FindEventControl( "Enchant" );
	assert(pEnchant);

	int nLine		= 0;
	int nLen		= strlen(m_arrItem[i + nPageNum].m_pTable->m_sName);

	for( int j=0;j<LIST5_SIZE;j++)
	{
		_snprintf( sText, MAX_PATH, "Enchant_%d", j );
		CUIControl* pEOn = pEnchant->FindEventControl( sText );
		
		if( j < m_arrItem[i + nPageNum].m_nGrade )
			pEOn->SetVisible(N3UI_VISIBLE_TRUE);
		else
		{
			pEOn->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}
		
		if( m_arrItem[i + nPageNum].m_nOptionCode[j] != 0 )
			UIFunction::ChangeTexture( pEOn, "EnchantOn" );
		else
		{
			UIFunction::ChangeTexture( pEOn, "EnchantOff" );
			continue;
		}

		if( 0 < j )	strcat(sAllOption, "\n");

		int nSituationCode	= ITEM_TYPE_SITUATION(m_arrItem[i + nPageNum].m_nOptionCode[j]);
		int nFacultyCode	= ITEM_TYPE_FACULTY(m_arrItem[i + nPageNum].m_nOptionCode[j]);
		int nFaculty		= ITEM_TYPE_VALUE(m_arrItem[i + nPageNum].m_nOptionCode[j]);

		char sOption[STRLEN_64]="";

		_snprintf(sOption, STRLEN_64, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);

		strcat(sAllOption, sOption);
		nLine++;
		
		if(nLen<(int)strlen(sOption))
			nLen = strlen(sOption);
	}

	if( 0 < m_arrItem[i + nPageNum].m_nGrade )
		nLine+=2;

	if( 90+(nLen*5) < 150 )
		pToolTip->SetSize(150, 70+(16*nLine), true);
	else
		pToolTip->SetSize(90+(nLen*5), 70+(16*nLine), true);

	pToolTip->SetLocation(m_pDNDPoint.x-(pToolTip->GetWidth()/2), m_pDNDPoint.y+5, true);

	UIFunction::Set_ChildControlText( pToolTip, "description", sAllOption );
}

void CInvenDialog::FUNCTION_EquipWearToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	char sAllOption[MAX_PATH]="";

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	short arrEquipWear[14] = { 
		ITEM_EQUIP_HAIR, 
		ITEM_EQUIP_FACE, 
		ITEM_EQUIP_SHIRTS, 
		ITEM_EQUIP_PANTS, 
		ITEM_EQUIP_GLOVE, 
		ITEM_EQUIP_SOCKS, 
		ITEM_EQUIP_SHOES, 
		ITEM_EQUIP_EYE, 
		ITEM_EQUIP_EAR, 
		ITEM_EQUIP_NECK, 
		ITEM_EQUIP_TATTOO, 
		ITEM_EQUIP_WRIST, 
		ITEM_EQUIP_ARM, 
		ITEM_EQUIP_KNEE 
	};

	RECT rt;
	
	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("ToolTipE");
	assert(pToolTip);
	pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	
	int i=0;
	for(;i<14;i++)
	{
		_snprintf(sText, MAX_PATH, "Slot%02d", arrEquipWear[i]);
		CUIControl* pOverImg = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pOverImg);

		rt = pOverImg->GetWindowBox();
		
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint))
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			break;
		}
	}

	int nWear = arrEquipWear[i];

	if( i == 14 )	return;

	// 임시
	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pObjPlayerInfo->m_nEquipWear[nWear], 0));
	if(pTable == NULL) 
	{
		pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
		return;
	}

	UIFunction::Set_ChildControlText( pToolTip, "ItemName", pTable->m_sName );

	CUIControl* pEnchant = pToolTip->FindEventControl( "Enchant" );
	assert(pEnchant);

	int nLine		= 0;
	int nLen		= strlen(pTable->m_sName);;
	int j			= 0;

	for( int k=0;k<LIST5_SIZE;k++)
	{
		_snprintf( sText, MAX_PATH, "Enchant_%d", k );
		UIFunction::SetChildToVisibility( pEnchant, sText, N3UI_VISIBLE_FALSE );
	}

	UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  CResourceManager::GetPtr()->m_pStaticMap[210020].c_str() );
	UIFunction::Set_Color(pToolTip, "ItemName", 0xffffffff);

	int nGrade = 0;

	for(;j<(int)pObjPlayerInfo->m_vItemList.size();j++)
	{
		if( pObjPlayerInfo->m_nEquipWear[nWear] == pObjPlayerInfo->m_vItemList.at(j)->m_nCode 
			&& pObjPlayerInfo->m_vItemList.at(j)->m_nEquipKind == EQUIP_KIND_YES )
		{
			_snprintf( sText, MAX_PATH, "LEVEL %d", pObjPlayerInfo->m_vItemList.at(j)->m_nLevel );
			UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  sText );

			// 등급에 따른 네임색 변경
			switch(pObjPlayerInfo->m_vItemList.at(j)->m_nGrade)
			{
			case 1:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffcacaca);	break;
			case 2:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffb3fe00);	break;
			case 3:	UIFunction::Set_Color(pToolTip, "ItemName", 0xff0496f4);	break;
			case 4:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffff56ed);	break;
			case 5:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffffb400);	break;
			}

			for( int k=0;k<LIST5_SIZE;k++)
			{
				_snprintf( sText, MAX_PATH, "Enchant_%d", k );
				CUIControl* pEOn = pEnchant->FindEventControl( sText );

				nGrade = pObjPlayerInfo->m_vItemList.at(j)->m_nGrade;

				if( k < nGrade )
					pEOn->SetVisible(N3UI_VISIBLE_TRUE);
				else
				{
					pEOn->SetVisible(N3UI_VISIBLE_FALSE);
					continue;
				}
				
				int nOption = pObjPlayerInfo->m_vItemList.at(j)->m_nOptionCode[k];

				if( nOption != 0 )
					UIFunction::ChangeTexture( pEOn, "EnchantOn" );
				else
				{
					UIFunction::ChangeTexture( pEOn, "EnchantOff" );
					continue;
				}

				if( 0 < k )	strcat(sAllOption, "\n");

				int nSituationCode	= ITEM_TYPE_SITUATION(nOption);
				int nFacultyCode	= ITEM_TYPE_FACULTY(nOption);
				int nFaculty		= ITEM_TYPE_VALUE(nOption);

				char sOption[STRLEN_64]="";

				_snprintf(sOption, STRLEN_64, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);

				strcat(sAllOption, sOption);
				nLine++;
				
				if(nLen<(int)strlen(sOption))
					nLen = strlen(sOption);
			}
		}
	}

	if( 0 < nGrade )
		nLine+=2;

	if( 90+(nLen*5) < 150 )
		pToolTip->SetSize(150, 70+(16*nLine), true);
	else
		pToolTip->SetSize(90+(nLen*5), 70+(16*nLine), true);

	if(i<7)
		pToolTip->SetLocation(m_pDNDPoint.x, m_pDNDPoint.y+5, true);
	else
		pToolTip->SetLocation(m_pDNDPoint.x-pToolTip->GetWidth(), m_pDNDPoint.y+5, true);

	UIFunction::Set_ChildControlText( pToolTip, "description", sAllOption );
}

void CInvenDialog::FUNCTION_UpdateSecond(char nCount)
{
	char sText[STRLEN_64]="";

	VectorAnyoneList* pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);

	if( !nCount )
	{
		UIFunction::Set_UIVisibility( "MenuTabButton06", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "MenuTabButton16", N3UI_VISIBLE_TRUE );

		VectorAnyoneList::iterator it;
		VectorPlayerList::iterator itp;
		for(it = pAnyoneList->begin();it != pAnyoneList->end();it++)
		{
			for(itp = (*it)->begin();itp != (*it)->end();itp++)
			{
				if( (*itp)->m_bPit == ROOM_PIT_IN )
					PutPitIn( (*itp)->GetObjSeq(), ROOM_PIT_OUT, true);
			}
		}
	}

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "RRInven" );
	assert(pControl);

	if( pControl->GetVisible() == N3UI_VISIBLE_TRUE )
	{
		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900123, 0));
		assert(pMTable);
		_snprintf( sText, STRLEN_64, pMTable->m_sMessage, nCount );
		UIFunction::Set_UIVisibility( "TimeDelayStc", N3UI_VISIBLE_TRUE );
		UIFunction::Set_ControlText( "TimeDelayStc", sText );
		
		UIFunction::Set_UIVisibility( "TimeDelayPrg", N3UI_VISIBLE_TRUE );
		CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl( "TimeDelayPrg" ));
		assert(pPrg);
		pPrg->SetValue(nCount);
	}
}