#include "stdafx.h"
#include "SkillBagProcess.h"
#include "UICommonMethod.h"
#include "PlayModeManager.h"
#include "UICommon.h"
#include "PutPacket.h"
#include "SignBoard.h"
#include "UIFunction.h"
#include "ResTableStack.h"

CSkillBagProcess::CSkillBagProcess() : CBaseProcess()
{
	m_nProcessCmd			= PROCESS_SKILLBAG;
	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호
	m_nSmallFractionStart	= 0;
	m_nDetailTabIndex		= 0;// Detail Tab
	m_nCurrentPage			= 1;
	m_nTotalPage			= 0;
	m_bListShape			= false;
    m_nSelectIndex			= 0;
	m_nSelectArea			= -1;
}

CSkillBagProcess::~CSkillBagProcess(void)
{
}

void CSkillBagProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CSkillBagProcess::Update(float fFrameTime, float fAccumTime)
{
    CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );

	FUNCTION_UpdateShop();
}

void CSkillBagProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "Detail_Bg", "InventoryBg" );

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(0);
	FUNCTION_DetailTabChange(0);
	FUNCTION_ItemListExchange(false);

	m_nState = STATE_START;
}

void CSkillBagProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	Update(fFrameTime, fAccumTime);

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
	//UICommonMethod::BGEffectAnimationPlay( "Detail_Bg", "InventoryBg" );
	UICommonMethod::BasicInfoStop();
	FUNCTION_DetailTabUpdate(m_nSelectIndex);
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
}

void CSkillBagProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_INIT_LOAD;
		}
		break;
	case STATE_INIT_LOAD:
		{
			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{
			LoadGround();
			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
			m_bIsRender = true;

            //m_cMySkillWindow.Update();

			m_nState = STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

bool CSkillBagProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
	{
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	}

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CSkillBagProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		CUIControl* pPage = CClientUIManager::GetPtr()->FindEventControl( "PlayerInformation" );
		assert(pPage);

		if( pPage->GetVisible() == N3UI_VISIBLE_THISFALSE && CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex == 500 )
			CPlayerInformationDialog::GetPtr()->DrawGraph();

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();
	}
}

CBaseProcess* CSkillBagProcess::GetSameProcess()
{
	return new CSkillBagProcess;
}


void CSkillBagProcess::FUNCTION_InitSkillBag()
{
	UIFunction::Set_UIVisibility( "Sort", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "RemainSkillPointBox", N3UI_VISIBLE_TRUE );

	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "%d", m_pObjPlayerInfo->m_cLevel.m_nSkill);
	UIFunction::Set_ControlText( "RemainSkillPoint", sText );
	
	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) 
			_snprintf(sText, STRLEN_64, "Slot_Small");
		else 
			_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );

		if(!m_bListShape)
		{
			UIFunction::Set_UIVisibility( pControl, "SLevel", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "ListName", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "SkillUpButton", N3UI_VISIBLE_TRUE );
		}
	}
}

void CSkillBagProcess::FUNCTION_InitTrainingBag()
{
	UIFunction::Set_UIVisibility( "Sort", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "RemainSkillPointBox", N3UI_VISIBLE_FALSE );

	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) 
			_snprintf(sText, STRLEN_64, "Slot_Small");
		else 
			_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );

		if(!m_bListShape)
		{
			UIFunction::Set_UIVisibility( pControl, "ListName", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "SLevel", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pControl, "SkillUpButton", N3UI_VISIBLE_FALSE );
		}
	}
}

void CSkillBagProcess::FUNCTION_InitCeremonyBag()
{
	UIFunction::Set_UIVisibility( "Sort", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "RemainSkillPointBox", N3UI_VISIBLE_FALSE );
	
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) 
			_snprintf(sText, STRLEN_64, "Slot_Small");
		else 
			_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		if(!m_bListShape)
		{
			UIFunction::Set_UIVisibility( pControl, "ListName", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "SLevel", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "SkillUpButton", N3UI_VISIBLE_FALSE );
		}
	}
}

void CSkillBagProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		FUNCTION_Selection();
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
	
		FUNCTION_SkillBagToolTipViewUpdate();
		break;
	}
}

void CSkillBagProcess::FUNCTION_SkillBagToolTipViewUpdate()
{
	char sText[STRLEN_64]="";

	RECT rt;

	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("ToolTip");
	assert(pToolTip);
	pToolTip->SetVisible(N3UI_VISIBLE_FALSE);

	if(m_bListShape) 
		_snprintf(sText, STRLEN_64, "Slot_Small");
	else 
		_snprintf(sText, STRLEN_64, "Slot_Big");

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pControl);

	int i=0;
	for(;i<nListCount;i++)
	{
		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pList = pControl->FindEventControl(sText);
		assert(pList);

		rt = pList->GetWindowBox();
		
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pList->GetVisible() == N3UI_VISIBLE_TRUE)
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			break;
		}
		else
			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	}

	if( nListCount == i ) return;

	CResMessageTableFormat* pMTable = NULL;
	
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pObjPlayerInfo);

	switch(m_nBigFractionIndex)
	{
	case 0:
		{
			CResSkillTableFormat *pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, 
				pObjPlayerInfo->m_vSkillList.at( i + ( ( m_nCurrentPage - 1 ) * nListCount ) )->m_nCode, 0));
			if(pTable == NULL) return;

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900011 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pTable->m_sName);
			UIFunction::Set_ChildControlText( pToolTip, "SName", sText );
			int nLen = strlen(sText);

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900012 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pObjPlayerInfo->m_vSkillList.at( i + ( ( m_nCurrentPage - 1 ) * nListCount ) )->m_nLevel );
			UIFunction::Set_UIVisibility( pToolTip, "description", N3UI_VISIBLE_TRUE );
			UIFunction::Set_ChildControlText( pToolTip, "description", sText );

			if( nLen < (int)strlen(sText) )
				nLen = (int)strlen(sText);
			
			pToolTip->SetSize(80+(nLen*5), 20+40);
			pToolTip->SetLocation(m_pDNDPoint.x - (pToolTip->GetWidth()/2), m_pDNDPoint.y + 10, true);
		}
		break;
	case 1:
		{
			CResTrainingTableFormat *pTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, 
				pObjPlayerInfo->m_vTrainingList.at( i + ( ( m_nCurrentPage - 1 ) * nListCount ) )->m_nCode, 0));
			if(pTable == NULL) return;

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900013, 0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pTable->m_sName);
			UIFunction::Set_ChildControlText( pToolTip, "SName", sText );
			int nLen = strlen(sText);	


			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900014, 0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pObjPlayerInfo->m_vTrainingList.at( i + ( ( m_nCurrentPage - 1 ) * nListCount ) )->m_nLevel );
			UIFunction::Set_UIVisibility( pToolTip, "description", N3UI_VISIBLE_TRUE );
			UIFunction::Set_ChildControlText( pToolTip, "description", sText );

			if( nLen < (int)strlen(sText) )
				nLen = (int)strlen(sText);

			pToolTip->SetSize(80+(nLen*5), 20+40);
			pToolTip->SetLocation(m_pDNDPoint.x - (pToolTip->GetWidth()/2), m_pDNDPoint.y + 10, true);
		}
		break;
	case 2:
		{
			CResCeremonyTableFormat *pTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 
				pObjPlayerInfo->m_vCeremonyList.at( i + ( ( m_nCurrentPage - 1 ) * nListCount ) )->m_nCode, 0));
			if(pTable == NULL) return;

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900099 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pTable->m_sName);
			UIFunction::Set_ChildControlText( pToolTip, "SName", sText );
			UIFunction::Set_UIVisibility( pToolTip, "description", N3UI_VISIBLE_FALSE );
			
			int nLen = (int)strlen(sText);

			pToolTip->SetSize(80+(nLen*5), 20+20);
			pToolTip->SetLocation(m_pDNDPoint.x - (pToolTip->GetWidth()/2), m_pDNDPoint.y + 10, true);
		}
		break;
	}
}

void CSkillBagProcess::FUNCTION_InitShop()
{
}

void CSkillBagProcess::FUNCTION_UpdateShop()
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "%d", m_pObjPlayerInfo->m_cLevel.m_nSkill);
	UIFunction::Set_ControlText( "RemainSkillPoint", sText );
	
	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) 
			_snprintf(sText, STRLEN_64, "Slot_Small");
		else 
			_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		if(m_nCode[i] != 0)
		{
			switch( m_nBigFractionIndex )
			{
			case 0:		// 스킬 백
				{
					CObjSkillInfo* pSkillInfo = m_cBagSkillList.m_pSkill[i];
					char nEquip = pSkillInfo->m_nEquipKind;
					char nSkillLevel = pSkillInfo->m_nLevel;
					
					if( nEquip == EQUIP_KIND_YES )								// 착용 및 사용 여부 표시
						UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_TRUE );
					else
						UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );
				
					if(!m_bListShape)
					{
						if( nEquip == EQUIP_KIND_YES )								// 착용 및 사용 여부 표시
						{
							UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
							UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_FALSE );
						}
						else
						{
							UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_FALSE );
							UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
						}

						CUIControl* pSLevel = pControl->FindEventControl( "SLevel" );
						assert(pSLevel);

						_snprintf( sText, STRLEN_64, "Level0%d", nSkillLevel );
						UIFunction::ChangeTexture( pSLevel, sText );					
					}
				}
				break;
			case 1:		// 트레이닝 백
				{
					UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );
				}
				break;
			case 2:		// 세레모니 백
				{
					CObjCeremonyInfo* pCeremonyInfo = m_cBagCeremonyList.m_pCeremony[i];
					char nEquip = pCeremonyInfo->m_nEquipKind;
					
					if( nEquip )								// 착용 및 사용 여부 표시
					{
						UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_TRUE );
						UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
						UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_FALSE );
					}
					else
					{
						UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );
						UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_FALSE );
						UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
					}
				}
				break;
			}

			if( m_nSelectIndex == i )
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_FALSE );

			if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )		// 마우스 오버 이미지 처리
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_FALSE );
		}
	}
    //m_cMySkillWindow.Update();
}

void CSkillBagProcess::FUNCTION_TerminateShop() 
{

}

void CSkillBagProcess::FUNCTION_DetailTabUpdate(int ItemIndex)
{
	if(m_nDetailTabIndex)	// Explain
	{
		switch(m_nBigFractionIndex)
		{
		case 0:
			m_cMySkillWindow.UpdateSkillInfo();
			break;
		case 1:
			m_cMySkillWindow.UpdateTrainingInfo();
			break;
		case 2:
			m_cMySkillWindow.UpdateCeremonyInfo();
			break;
		default:
			break;
		}
	}
	else
	{
		switch(m_nBigFractionIndex)
		{
		case 0:
			m_nSelectArea = UpdateSkillExplan( ItemIndex, m_nCode, m_nSelectArea );
			break;
		case 1:
			UpdateTrainingExplan( ItemIndex, m_nCode );
			break;
		case 2:
			UpdateCeremonyExplan( ItemIndex, m_nCode );
			break;

		default:
			break;
		}
	}
}

void CSkillBagProcess::FUNCTION_InitFraction()
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
		// 분류 테이블 로딩
		CResFractionTableFormat *pTable = dynamic_cast<CResFractionTableFormat*>(temp->GetSequenceField(i));
		assert(pTable);
        
		if( pTable->GetObjCode()/1000 == SKILLBAG_FRACTION)
		{
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
			m_vSmallFractionTypeList[pTable->m_nBigFractionIndex].push_back(pTable->m_nTypeCode);
		}
	}
}

void CSkillBagProcess::FUNCTION_BigFraction(int nBigFractionIndex)
{
	m_nBigFractionIndex = nBigFractionIndex;
	m_nSmallFractionIndex = 0;
	m_nSelectIndex = -1;
	m_nCurrentPage = 1;

	m_cMySkillWindow.SetMode(m_nBigFractionIndex);

	char sText[STRLEN_64]="";

	for(int i = 0; i < 3; i++)
	{
		_snprintf(sText, STRLEN_64, "TOP_TabButton0%d", i);
		CUIButton* pBigFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pBigFraction);

		_snprintf(sText, STRLEN_64, "STC_BG_0%d", i);
		if( m_nBigFractionIndex == i )
		{
			pBigFraction->SetPressed(true);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
		}
		else
		{
			pBigFraction->SetPressed(false);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
		}
	}

	switch( nBigFractionIndex )
	{
	case 0:
		FUNCTION_InitSkillBag();
		break;
	case 1:
		FUNCTION_InitTrainingBag();
		break;
	case 2:
		FUNCTION_InitCeremonyBag();
		break;
	}

	FUNCTION_SmallFraction(0);
	FUNCTION_SmallFractionInit(nBigFractionIndex);

	FUNCTION_GetBagList();
}

void CSkillBagProcess::FUNCTION_SmallFractionInit(int nBigFraction)
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

void CSkillBagProcess::FUNCTION_SmallFraction(int nSmallFractionIndex)
{
	m_nSmallFractionIndex = nSmallFractionIndex;

	m_nCurrentPage = 1;

	char sText[STRLEN_64]="";
	
	for(int i = 0; i < 3; i++)
	{
		_snprintf( sText, STRLEN_64, "BOTTOM_TabButton0%d", i);
		CUIButton* pSmallFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pSmallFraction);

		if(m_nSmallFractionIndex == i)
			pSmallFraction->SetPressed(true);
		else 
			pSmallFraction->SetPressed(false);
	}

	FUNCTION_GetBagList();
}

void CSkillBagProcess::FUNCTION_GoPage(int nPage)
{
	m_nSelectIndex = -1;

    if( nPage > m_nTotalPage )
		nPage = 1;
	if( nPage < 1 )
		nPage = m_nTotalPage;

    m_nCurrentPage = nPage;    
	FUNCTION_GetBagList();
}

void CSkillBagProcess::FUNCTION_GetBagList()
{
	switch(m_nBigFractionIndex)
	{
	case 0:	FUNCTION_GetSkillBagList(m_nCurrentPage);//스킬 샵
		break;
	case 1:	FUNCTION_GetTrainingBagList(m_nCurrentPage);//트레이닝 샵
		break;
	case 2:	FUNCTION_GetCeremonyBagList(m_nCurrentPage);//세레모니 샵
		break;
	default:
		break;
	}
}

void CSkillBagProcess::FUNCTION_GetSkillBagList(int nPage)
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;	

    m_cBagSkillList.Clear();

	m_pObjPlayerInfo->GetBagSkillList(nListCount, nPage, &m_cBagSkillList);
	m_nTotalPage = m_pObjPlayerInfo->GetBagSkillTotalPage(nListCount);

	for(int i = 0; i < nListCount; i++)
	{
		if(m_cBagSkillList.m_pSkill[i] == NULL) 
		{
			m_nCode[i] = 0;
			FUNCTION_ItemRender(i, 0, 0, 0 );
		}
		else
		{
			m_nCode[i] = m_cBagSkillList.m_pSkill[i]->m_nCode;
            int nEquip = m_cBagSkillList.m_pSkill[i]->m_nEquipKind;

			CResSkillTableFormat *pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[i], 0));			
			assert(pTable);

            if( pTable != 0 )
            {            
				FUNCTION_ItemRender(i, m_nCode[i], m_cBagSkillList.m_pSkill[i]->m_nLevel, nEquip, pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
            }
		}
	}
}

void CSkillBagProcess::FUNCTION_GetTrainingBagList(int nPage)
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;	

    m_cBagTrainingList.Clear();

	m_pObjPlayerInfo->GetBagTrainingList(nListCount, nPage, &m_cBagTrainingList);
	m_nTotalPage = m_pObjPlayerInfo->GetBagTrainingTotalPage(nListCount);

	for(int i = 0; i < nListCount; i++)
	{
		if(m_cBagTrainingList.m_pTraining[i] == NULL) 
		{
			m_nCode[i] = 0;
			FUNCTION_ItemRender(i, 0, 0, 0);
		}
		else
		{
			m_nCode[i] = m_cBagTrainingList.m_pTraining[i]->m_nCode;
			CResTrainingTableFormat *pTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, m_nCode[i], 0));
			assert(pTable);

			FUNCTION_ItemRender(i, m_nCode[i], pTable->m_nLimit, 0, pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
		}
	}
}

void CSkillBagProcess::FUNCTION_GetCeremonyBagList(int nPage)
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	m_cBagCeremonyList.Clear();

	m_pObjPlayerInfo->GetBagCeremonyList(nListCount, nPage, &m_cBagCeremonyList);
	m_nTotalPage = m_pObjPlayerInfo->GetBagCeremonyTotalPage(nListCount);

	for(int i = 0; i < nListCount; i++)
	{
		if(m_cBagCeremonyList.m_pCeremony[i] == NULL) 
		{
			m_nCode[i] = 0;
			FUNCTION_ItemRender(i, 0, 0, 0);
		}
		else
		{
			m_nCode[i] = m_cBagCeremonyList.m_pCeremony[i]->m_nCode;

			CResCeremonyTableFormat *pTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, m_nCode[i], 0));			
			assert(pTable);
			FUNCTION_ItemRender(i, m_nCode[i], pTable->m_nLimit, 0, pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//UI관련 함수
void CSkillBagProcess::FUNCTION_ItemRender(int ItemIndex, int nSkillCode, int nLimitLevel, int nEquip, char *sName, char *sIconFile, int nSell)
{
	char sText[STRLEN_64]="";

	if(m_bListShape)	
		_snprintf(sText, STRLEN_64, "Slot_Small");
	else				
		_snprintf(sText, STRLEN_64, "Slot_Big");

	CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pBagListBG);

	_snprintf(sText, STRLEN_64, "ListBG%d", ItemIndex);
	CUIControl* pControl = pBagListBG->FindEventControl(sText);
	assert(pControl);

	if(nSkillCode > 0)
	{
		pControl->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pItemImg = pControl->FindEventControl("ListImg");//리스트 이미지
		assert(pItemImg);

		switch( m_nBigFractionIndex%10 )
		{
		case 0:
			_snprintf(sText, STRLEN_64, "Icon/Skill/%s.png", sIconFile);
			break;
		case 1:
			_snprintf(sText, STRLEN_64, "Icon/Training/%s.png", sIconFile);
			break;
		case 2:
			_snprintf(sText, STRLEN_64, "Icon/Ceremony/%s.png", sIconFile);
			break;
		}
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);

		CUIControl* pItemName = pControl->FindEventControl("ListName");//리스트 이름

		if(strlen(sName)>16)
		{
			char* p;
			_snprintf(sText, STRLEN_64, "%s", sName );
			p = &sText[15];
			while(true)
			{
				if( *p == ' ' || *p == '(' )
				{
					*p='\0';
					strcat(sText, "..");
					break;
				}
				else
					p--;
			}
			if(pItemName) pItemName->SetText(sText);
		}
		else
			if(pItemName) pItemName->SetText(sName);
	}
	else pControl->SetVisible(N3UI_VISIBLE_FALSE);

	CUIControl* pPageName = CClientUIManager::GetPtr()->FindEventControl("SkillBagPageName");
	if(pPageName == NULL) return;
	_snprintf(sText, STRLEN_64, "%d/%d", m_nCurrentPage, m_nTotalPage);
	pPageName->SetText(sText);
}

void CSkillBagProcess::FUNCTION_3DRotation()
{
	CUIButton* pControl = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("WearItemButton"));
	assert(pControl);
	if(pControl->GetRButtonClick())
	{
		//회전시킨다
		CResCharacterFormat* pCharacter = m_pObjPlayerInfo->m_pModel;
		pCharacter->SetDirection(- pControl->m_OldPoint.x*0.01f - pControl->m_DeltaPoint.x*0.01f + 180.f);				
	}
}

void CSkillBagProcess::FUNCTION_ItemListExchange(bool bItemListShape)
{
	m_bListShape = bItemListShape;

	m_nCurrentPage = 1;
	FUNCTION_GoPage(m_nCurrentPage);

	m_nSelectIndex = -1;

	if(m_bListShape)
	{
		UIFunction::Set_UIVisibility( "Slot_Big", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Slot_Small", N3UI_VISIBLE_TRUE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "Slot_Big", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Slot_Small", N3UI_VISIBLE_FALSE );
	}

	// 버튼 크기 활성표시
	CUIButton* pBigButton = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "Slot_BigButton" ));
	assert(pBigButton);
	CUIButton* pSmallButton = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( "Slot_SmallButton" ));
	assert(pSmallButton);

	if( m_bListShape )
	{
		pBigButton->SetPressed(false);
		pSmallButton->SetPressed(true);
	}
	else
	{
		pBigButton->SetPressed(true);
		pSmallButton->SetPressed(false);
	}

	FUNCTION_GetBagList();
}

void CSkillBagProcess::FUNCTION_MySkillPageChange(char idx)
{
	if( 0 < idx )
		m_cMySkillWindow.NextPage();
	else if( idx < 0 )
		m_cMySkillWindow.PreviousPage();

	FUNCTION_DetailTabUpdate(m_nDetailTabIndex);
}

void CSkillBagProcess::FUNCTION_AllDivest()
{
	switch(m_nBigFractionIndex)
	{
	case 0:		// Skill
		{
			for( size_t i=0; i<m_pObjPlayerInfo->m_vSkillList.size(); i++ )
				m_pObjPlayerInfo->m_vSkillList.at(i)->m_nEquipKind = 0;
			PutDivestSkill( 0 );
		}
		break;
	case 1:		// Training
		break;
	case 2:		// Ceremony
		{
			for( size_t i=0; i<m_pObjPlayerInfo->m_vCeremonyList.size(); i++ )
				m_pObjPlayerInfo->m_vCeremonyList.at(i)->m_nEquipKind = 0;
			PutDivestCeremony( 0 );
		}
		break;
	default:
		break;
	}
}

void CSkillBagProcess::FUNCTION_Selection()
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) 
			_snprintf(sText, STRLEN_64, "Slot_Small");
		else 
			_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )
		{
			m_nSelectIndex = i;

			FUNCTION_DetailTabChange(0);

			if( !m_nBigFractionIndex )
				m_nSelectArea = 0;
			break;
		}
	}
}

void CSkillBagProcess::FUNCTION_SkillEquip(char idx)
{
	switch( m_nBigFractionIndex )
	{
	case 0:		// 스킬
		{
			if( m_cBagSkillList.m_pSkill[ idx ]->m_nEquipKind != EQUIP_KIND_YES )
				PutEquipSkill( m_cBagSkillList.m_pSkill[ idx ]->m_nSkillSeq );
		}
		break;
	case 1:		// 트레이닝
		{}
		break;
	case 2:		// 세레모니
		{
			int nSeq = 1;
			
			for(int i=0;i<(int)m_pObjPlayerInfo->m_vCeremonyList.size();i++)
			{
				if( nSeq == m_pObjPlayerInfo->m_vCeremonyList.at(i)->m_nEquipKind )
				{
					nSeq++;
					i=-1;
					continue;
				}
			}
			
			if( m_cBagCeremonyList.m_pCeremony[ idx ]->m_nEquipKind != EQUIP_KIND_YES )
				PutEquipCeremony( m_cBagCeremonyList.m_pCeremony[ idx ]->m_nCeremonySeq, nSeq );
		}
		break;
	}
}

void CSkillBagProcess::FUNCTION_SkillDivest(char idx)
{
	switch( m_nBigFractionIndex )
	{
	case 0:		// 스킬
		{
			if( m_cBagSkillList.m_pSkill[ idx ]->m_nEquipKind == EQUIP_KIND_YES )
				PutDivestSkill( m_cBagSkillList.m_pSkill[ idx ]->m_nSkillSeq );
		}
		break;
	case 1:		// 트레이닝
		{}
		break;
	case 2:		// 세레모니
		{
			if( m_cBagCeremonyList.m_pCeremony[ idx ]->m_nEquipKind == EQUIP_KIND_YES )
				PutDivestCeremony( m_cBagCeremonyList.m_pCeremony[ idx ]->m_nCeremonySeq );
		}
		break;
	}
}

void CSkillBagProcess::FUNCTION_SkillUpgrade(char idx)
{
	PutUpgradeSkill(m_cBagSkillList.m_pSkill[ idx ]->m_nSkillSeq);
}

void CSkillBagProcess::FUNCTION_DetailTabChange(char idx)
{
	char sText[STRLEN_64]="";

	m_nDetailTabIndex = idx;

	for(int i=0;i<2;i++)
	{
		_snprintf(sText, STRLEN_64, "Detail_TabButton0%d", i);
		CUIButton* pDetailTab = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pDetailTab);

		_snprintf(sText, STRLEN_64, "Detail_0%d", i);

		if( i == idx )
		{
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_TRUE);
			pDetailTab->SetPressed(true);
		}
		else
		{
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
			pDetailTab->SetPressed(false);
		}
	}
}

void CSkillBagProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}