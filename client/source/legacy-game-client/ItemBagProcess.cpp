#include "stdafx.h"
#include "ItemBagProcess.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "PlayModeManager.h"
#include "ObjItemInfo.h"
#include "Common.h"
#include "SignBoard.h"
#include "UIFunction.h"
#include "UIComboBox.h"
#include "ResTableStack.h"
#include "UICheckBox.h"

CItemBagProcess::CItemBagProcess() : CBaseProcess()
{
	m_nProcessCmd			= PROCESS_ITEMBAG;
	m_pObjPlayerInfo		= NULL;
	m_bListShape			= false;
	m_bDnd					= false;
	m_bEntry				= false;
	m_bMix					= false;
	m_nCard					= -1;
	m_nSmallFractionStart	= 0;
	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호
	m_nSelectIndex			= -1;
	m_nCurrentPage			= 1;
	m_nTotalPage			= 0;
	m_nEntry				= 0;
	m_nCardTabIndex			= 0;
	m_nMixReservedRank		= -1;

	for(int i=0;i<3;i++)
		m_nMix[i] = 0;
}

CItemBagProcess::~CItemBagProcess(void)
{
}

void CItemBagProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CItemBagProcess::Update(float fFrameTime, float fAccumTime)
{
    CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );
	if(m_pObjPlayerInfo && m_pObjPlayerInfo->m_pModel)
	{
		CGameProject::s_pGameProject->BeginFrame();

		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime, true);

		m_cCharacterTexBoard.Render();

		CGameProject::s_pGameProject->EndFrame();

		CGameProject::s_pGameProject->DisplayFrame();
	}

	// 카드
	if( m_nBigFractionIndex == BF_CARD )
	{
		UIFunction::Set_UIVisibility( "Slot_Card", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Slot_Big", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Slot_Small", N3UI_VISIBLE_FALSE );
		FUNCTION_CardUpdate();
	}
	else
	{
		UIFunction::Set_UIVisibility( "Slot_Card", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Slot_Big", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Slot_Small", N3UI_VISIBLE_FALSE );
		FUNCTION_UpdateShop();
	}
}

void CItemBagProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	pObjGroundInfo->SetObjectPosition(NiPoint3(10000,0,0));

	//NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	//if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CItemBagProcess::LoadCharacter()
{
	CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pObjPlayerInfo, 0, 0);

	m_pObjPlayerInfo->m_cNodePoint = NiPoint3(10000,0,0);

	m_pObjPlayerInfo->m_cCurrentAction.m_fStartAngle = 180.f;

	CProcessManager::GetPtr()->AttachItem(m_pObjPlayerInfo);
}

void CItemBagProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);	

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "Detail_BG_00", "InventoryBg" );

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(BF_CLOTH);
	FUNCTION_ItemListExchange(false);

	FUNCTION_3DRender( "Detail_BG_00" , pObjGroundInfo->GetObjectCamera() );

	m_nState = STATE_START;	
}

void CItemBagProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	Update(fFrameTime, fAccumTime);

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
	//UICommonMethod::BGEffectAnimationPlay( "Detail_BG_00", "InventoryBg" );
	UICommonMethod::BasicInfoStop();
	//SignBoard::GetPtr()->DisplaySignBoard();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
}

void CItemBagProcess::UpdateState(float fFrameTime, float fAccumTime)
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
			LoadCharacter();

			m_bIsRender = true;
			
			FUNCTION_InitShop();

			m_nState = STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

bool CItemBagProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CItemBagProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

bool CItemBagProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
	{
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	}

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CItemBagProcess::RenderFrame()
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

CBaseProcess* CItemBagProcess::GetSameProcess()
{
	return new CItemBagProcess;
}

void CItemBagProcess::FUNCTION_InitShop()
{
	char sText[STRLEN_64]="";
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	for(int i = 0; i < nListCount; i++)
	{
		if(m_bListShape) _snprintf(sText, STRLEN_64, "Slot_Small");
		else _snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);
	
		if(!m_bListShape)
		{
			UIFunction::Set_UIVisibility( pControl, "ListName", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( pControl, "DetailViewBtn", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pControl, "over_CardBg", N3UI_VISIBLE_FALSE );

			if( m_nBigFractionIndex == BF_SPECIAL )
				UIFunction::Set_UIVisibility( pControl, "EnchantBtn", N3UI_VISIBLE_FALSE );
			else
				UIFunction::Set_UIVisibility( pControl, "EnchantBtn", N3UI_VISIBLE_TRUE );
		}
	}

	for(int i = 0; i < MAX_EQUIP; i++)
	{
		if(m_pObjPlayerInfo->m_nEquipWear[i] == 0) continue;

		CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_pObjPlayerInfo->m_nEquipWear[i], 0));
		assert(pTable);
		
		_snprintf(sText, STRLEN_64, "Slot%02d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		CUIControl* pImg = pControl->FindEventControl( "List_Img" );
		assert(pImg);

		if(m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN)
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

void CItemBagProcess::FUNCTION_InitCardShop()
{
	m_bDnd = false;
	m_bEntry = false;

	FUNCTION_GetCardInfo();

	PutCardEntry(CARD_ENTRY_GET);
}

void CItemBagProcess::FUNCTION_InitEntry()
{
	char sText[STRLEN_64]="";

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	char nUserPositionKind = pObjPlayerInfo->m_nPosition/LIST10_SIZE;
	
	if( 3<nUserPositionKind )
		nUserPositionKind = 6;

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "EntryBtn%d", i);
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);
		if( m_nEntry == i )
			pBtn->SetPressed(true);
		else
			pBtn->SetPressed(false);
	}

	for(int i = 0; i < 5; i++)
	{
		_snprintf(sText, STRLEN_64, "MY_Entry%d", i);
		CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pEntry);

		if( i==4 )	i++;

		CUIControl* pImg = pEntry->FindEventControl( "ListImg" );
		assert(pImg);
		
		pImg->SetVisible(N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility( pEntry, "Position_Mark", N3UI_VISIBLE_TRUE );

		if( nUserPositionKind  == i+1 )
			UIFunction::SetChildToVisibility( pEntry, "User_Mark", N3UI_VISIBLE_TRUE );
		else
			UIFunction::SetChildToVisibility( pEntry, "User_Mark", N3UI_VISIBLE_FALSE );

		for(int j=0;j<(int)m_vCIVectorList.size();j++)
		{
			char nEquip = m_vCIVectorList.at(j)->m_nEquipKind[m_nEntry];
			char nPosition = m_vCIVectorList.at(j)->m_nPosition;
			char nRank = m_vCIVectorList.at(j)->m_nRank;
			char nLv = m_vCIVectorList.at(j)->m_nLevel;
			int nCostume = m_vCIVectorList.at(j)->m_nCostume;

			if( nEquip == i+1 )
			{
				UICommonMethod::FUNCTION_CardInfoDraw(pImg, nPosition, nRank, nLv, nCostume);

				pImg->SetVisible(N3UI_VISIBLE_TRUE);
				break;
			}
		}
	}
}

void CItemBagProcess::FUNCTION_UpdateShop()
{
	int nListCount = 0;
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	for(int i = 0; i < nListCount; i++)
	{
		char sText[STRLEN_64]="";

		if(m_bListShape) _snprintf(sText, STRLEN_64, "Slot_Small");
		else _snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		CBagItemList pList;

		// 빈 슬록 예외처리
		if( (int)m_vSmallFractionTypeList[m_nBigFractionIndex].size() <= (m_nSmallFractionStart+m_nSmallFractionIndex) )	return;

		int nType = m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+m_nSmallFractionIndex);

		m_pObjPlayerInfo->GetBagItemList( ( m_nBigFractionIndex * 1000 ) + nType, nListCount, m_nCurrentPage, &pList );

		if(m_ItemList[i].m_nCode != 0)
		{
			if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )		// 마우스 오버 이미지 처리
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_FALSE );

			if( m_nSelectIndex == i )
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_FALSE );

			if(!m_bListShape)
			{
				int nOptionCount = 0;
				for(;nOptionCount<ITEM_OPTION_SIZE;nOptionCount++)
				{
					if( pList.m_pItem[i]->m_nOptionCode[nOptionCount] != 0 )
						nOptionCount++;
				}

				if( pList.m_pItem[i]->m_nGrade == nOptionCount) 
					UIFunction::Set_UIVisibility( pControl, "EnchantBtn", N3UI_VISIBLE_FALSE );
				UIFunction::Set_UIVisibility( pControl, "EnchantBtn", N3UI_VISIBLE_TRUE );

				if( 1 <pList.m_pItem[i]->m_nAmount )												// 1개 이상 아이템 총 개수 표시
				{
					UIFunction::Set_UIVisibility( pControl, "Item_Num", N3UI_VISIBLE_TRUE );
					CUIControl* pNum = pControl->FindEventControl( "Item_Num" );
					assert(pNum);
					_snprintf( sText, STRLEN_64, "x%d", pList.m_pItem[i]->m_nAmount );
					UIFunction::Set_ChildControlText( pNum, "Num", sText ); 
				}
				else
					UIFunction::Set_UIVisibility( pControl, "Item_Num", N3UI_VISIBLE_FALSE );

				if( pList.m_pItem[i]->m_nEquipKind == EQUIP_KIND_YES )								// 착용 및 사용 여부 표시
				{
					UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_TRUE );
					UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_FALSE );
				}
				else
				{
					UIFunction::Set_UIVisibility( pControl, "DivestButton", N3UI_VISIBLE_FALSE );
					UIFunction::Set_UIVisibility( pControl, "EquipButton", N3UI_VISIBLE_TRUE );
				}
			}
			
			if( pList.m_pItem[i]->m_nEquipKind == EQUIP_KIND_YES )								// 착용 및 사용 여부 표시
				UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "UseListImg", N3UI_VISIBLE_FALSE );
			
		}
	}

	FUNCTION_3DRotation();
}

void CItemBagProcess::FUNCTION_CardUpdate()
{
	int nListCount = LIST6_SIZE;

	CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl("Slot_Card");
	assert(pListBG);
		
	char sText[MAX_PATH]="";

	if( m_vCIVectorList.size()%nListCount == 0 )
		m_nTotalPage = m_vCIVectorList.size()/nListCount;
	else
		m_nTotalPage = (m_vCIVectorList.size()/nListCount)+1;

	if( m_vCIVectorList.size() == 0 ) m_nTotalPage++;

	if( m_nTotalPage < m_nCurrentPage )
		m_nCurrentPage = 1;
	else if( m_nCurrentPage < 1 )
		m_nCurrentPage = m_nTotalPage;

	FUNCTION_UpdatePage();

	int nPageNum = 0;

	for(int i=0;i<nListCount;i++)
	{
		_snprintf(sText, MAX_PATH, "ListBG%d", i);
		CUIControl* pParentListBG = pListBG->FindEventControl(sText);
		assert(pParentListBG);

		nPageNum = (m_nCurrentPage-1)*nListCount;

		if( (int)m_vCIVectorList.size() <= i+nPageNum )
		{
			pParentListBG->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}
		pParentListBG->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pBagListBG = pParentListBG->FindEventControl("ListImg");
		assert(pBagListBG);

		char nPosition = m_vCIVectorList.at(i+nPageNum)->m_nPosition;
		char nRank = m_vCIVectorList.at(i+nPageNum)->m_nRank;
		char nLv = m_vCIVectorList.at(i+nPageNum)->m_nLevel;
		char nEquip = m_vCIVectorList.at(i+nPageNum)->m_nEquipKind[m_nEntry];
		int nCostume = m_vCIVectorList.at(i+nPageNum)->m_nCostume;
		int nSeq = m_vCIVectorList.at(i+nPageNum)->m_nCardSeq;

		UICommonMethod::FUNCTION_CardInfoDraw(pBagListBG, nPosition, nRank, nLv, nCostume);

		if( m_nCardTabIndex == 0 )
		{
			UIFunction::Set_UIVisibility( pBagListBG, "MixListImg", N3UI_VISIBLE_FALSE );

			if( nEquip )
				UIFunction::Set_UIVisibility( pParentListBG, "UseListImg", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pParentListBG, "UseListImg", N3UI_VISIBLE_FALSE );
		}
		else if( m_nCardTabIndex == 1 )
		{
			UIFunction::Set_UIVisibility( pParentListBG, "UseListImg", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( pBagListBG, "MixListImg", N3UI_VISIBLE_FALSE );

			for(int i=0;i<3;i++)
			{
				if( m_nMix[i] == nSeq )
				{
					UIFunction::Set_UIVisibility( pBagListBG, "MixListImg", N3UI_VISIBLE_TRUE );
					break;
				}
			}
		}
	}
}

void CItemBagProcess::FUNCTION_GetCardInfo()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_vCIVectorList.clear();

	for(int i=0;i<(int)pObjPlayerInfo->m_vCardbotList.size();i++)
	{
		CCardInfo cCardInfo = (*pObjPlayerInfo->m_vCardbotList.at(i));

		switch( m_nSmallFractionIndex + m_nSmallFractionStart )
		{
		case 0:			// All
			m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 1:			// S Rank
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nRank == 4 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 2:			// A Rank
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nRank == 3 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 3:			// B Rank
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nRank == 2 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 4:			// C Rank
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nRank == 1 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 5:			// D Rank
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nRank == 0 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 6:			// 공격
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nPosition/LIST10_SIZE == 1 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 7:			// 미들
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nPosition/LIST10_SIZE == 2 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		case 8:			// 수비
			if( pObjPlayerInfo->m_vCardbotList.at(i)->m_nPosition/LIST10_SIZE == 3 || 
				pObjPlayerInfo->m_vCardbotList.at(i)->m_nPosition/LIST10_SIZE == 4 )
				m_vCIVectorList.push_back( pObjPlayerInfo->m_vCardbotList.at(i) );
			break;
		default:
			;
		}
	}
}

void CItemBagProcess::FUNCTION_TerminateShop() 
{

}

void CItemBagProcess::FUNCTION_InitFraction()
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

		if( pTable->GetObjCode()/1000 == ITEMBAG_FRACTION)
		{
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
			m_vSmallFractionTypeList[pTable->m_nBigFractionIndex].push_back(pTable->m_nTypeCode);
		}
	}
}

void CItemBagProcess::FUNCTION_BigFraction(int nBigFraction)
{
	m_nBigFractionIndex		= nBigFraction;
	m_nSmallFractionStart	= 0;
	m_nSelectIndex			= -1;
	m_nCurrentPage			= 1;

	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST5_SIZE; i++)
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

	UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_01", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_03", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Post_bg", N3UI_VISIBLE_FALSE );

	switch( nBigFraction )
	{
	case BF_CARD:
		FUNCTION_InitCardShop();
		UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_TRUE );
		FUNCTION_CardTabChange(0);
		break;
	case BF_POST:
		UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Post_bg", N3UI_VISIBLE_TRUE );
		FUNCTION_InitShop();
		break;
	case BF_CLOTH:
	case BF_CLUB:
	case BF_SPECIAL:
		UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_TRUE );
		FUNCTION_InitShop();
		break;
	}		
}

void CItemBagProcess::FUNCTION_SmallFractionInit(int nBigFraction)
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

void CItemBagProcess::FUNCTION_SmallFraction(int nSmallFractionIndex)
{
	m_nSmallFractionIndex = nSmallFractionIndex;

	m_nCurrentPage = 1;

	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST5_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "BOTTOM_TabButton0%d", i);
		CUIButton* pSmallFraction = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pSmallFraction);

		if(m_nSmallFractionIndex == i)
			pSmallFraction->SetPressed(true);
		else 
			pSmallFraction->SetPressed(false);
	}

	if( m_nBigFractionIndex == BF_CARD )
		FUNCTION_GetCardInfo();
	else
		FUNCTION_GetBagList();
}

void CItemBagProcess::FUNCTION_GoPage(int nPage)
{
	m_nSelectIndex = -1;

	m_nCurrentPage = nPage;
	if( m_nBigFractionIndex == BF_CARD )
		FUNCTION_CardUpdate();
	else
		FUNCTION_GetBagList();		
}

void CItemBagProcess::FUNCTION_UpdatePage()
{
	char sText[STRLEN_32]="";

	_snprintf(sText, STRLEN_64, "%d/%d", m_nCurrentPage, m_nTotalPage );
	UIFunction::Set_ControlText( "ItemBagPageName", sText );
}


void CItemBagProcess::FUNCTION_GetBagList()
{
	int nListCount = 0;
	
	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;
	
	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);		

	// 빈 슬록 예외처리
	if( (int)m_vSmallFractionTypeList[m_nBigFractionIndex].size() <= (m_nSmallFractionStart+m_nSmallFractionIndex) ) return;

	CBagItemList pList;
	int nType = m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+m_nSmallFractionIndex);

	m_nTotalPage = m_pObjPlayerInfo->GetBagItemList( ( m_nBigFractionIndex * 1000 ) + nType, nListCount, m_nCurrentPage, &pList);
	m_nCurrentPage = pList.m_nPage;
	
	for(int i = 0; i < nListCount; i++)
	{
		if( pList.m_pItem[i] == NULL )
			m_ItemList[i].m_nCode = 0;
		else
			m_ItemList[i] = *pList.m_pItem[i];

		FUNCTION_ItemRender(i);			
	}

	FUNCTION_UpdatePage();
}

void CItemBagProcess::ExchangeItem()
{
	//CObjectModelManager::GetPtr()->DeleteModelObject(m_pObjPlayerInfo->m_pModel);
    for( int i=0; i<MAX_EQUIP; i++ )
    {
        int nCode = m_pObjPlayerInfo->m_nEquipWear[i];
        if( nCode > 0 )
        {
            CObjectModelManager::GetPtr()->DeleteModelObject( m_pObjPlayerInfo->m_pEquipWearModelID[i] );
            m_pObjPlayerInfo->m_pEquipWearModelID[i] = 0;
        }
    }

	//CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pObjPlayerInfo, 0);

	//CPlayModeManager::GetPtr()->SetupCharacterPoint(m_pObjPlayerInfo, 0);

	CProcessManager::GetPtr()->AttachItem(m_pObjPlayerInfo);
}

/////////////////////////////////////////////////////////////////////////
//UI관련 함수
void CItemBagProcess::FUNCTION_ItemRender(int ItemIndex)
{
	char sText[STRLEN_64]="";
	
	if(m_bListShape) _snprintf(sText, STRLEN_64, "Slot_Small");
	else _snprintf(sText, STRLEN_64, "Slot_Big");

	CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pBagListBG);

	_snprintf(sText, STRLEN_64, "ListBG%d", ItemIndex);
	CUIControl* pBuyItemBG = pBagListBG->FindEventControl(sText);

	if(m_ItemList[ItemIndex].m_nCode > 0)
	{
		pBuyItemBG->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pItemName = pBuyItemBG->FindEventControl("ListName");//리스트 이름

		if(strlen(m_ItemList[ItemIndex].m_pTable->m_sName)>16)
		{
			char* p;
			_snprintf(sText, STRLEN_64, "%s", m_ItemList[ItemIndex].m_pTable->m_sName );
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
			if(pItemName) pItemName->SetText(m_ItemList[ItemIndex].m_pTable->m_sName);

		switch(m_ItemList[ItemIndex].m_nGrade)
		{
		case 1:	UIFunction::Set_Color(pBuyItemBG, "ListName", 0xffcacaca);	break;
		case 2:	UIFunction::Set_Color(pBuyItemBG, "ListName", 0xffb3fe00);	break;
		case 3:	UIFunction::Set_Color(pBuyItemBG, "ListName", 0xff0496f4);	break;
		case 4:	UIFunction::Set_Color(pBuyItemBG, "ListName", 0xffff56ed);	break;
		case 5:	UIFunction::Set_Color(pBuyItemBG, "ListName", 0xffffb400);	break;
		}

		CUIControl* pItemImg = pBuyItemBG->FindEventControl("ListImg");//리스트 이미지
		assert(pItemImg);

		if( m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
			_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", m_ItemList[ItemIndex].m_pTable->m_sIconFile20);
		else
			_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", m_ItemList[ItemIndex].m_pTable->m_sIconFile);

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);
	}
	else pBuyItemBG->SetVisible(N3UI_VISIBLE_FALSE);
}

void CItemBagProcess::FUNCTION_3DRotation()
{
	CUIButton* pControl = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("WearItem3D"));
	assert(pControl);	
	if(pControl->GetRButtonClick())
	{
		//회전시킨다
		CResCharacterFormat* pCharacter = m_pObjPlayerInfo->m_pModel;
		assert(pCharacter);
		pCharacter->SetDirection(- pControl->m_OldPoint.x*0.01f - pControl->m_DeltaPoint.x*0.01f + 180.f);				
	}
}

void CItemBagProcess::FUNCTION_ItemListExchange(bool bItemListShape)
{
	m_bListShape = bItemListShape;

	m_nSelectIndex = -1;

	CUIControl* pBigListBG = CClientUIManager::GetPtr()->FindEventControl("Slot_Big");
	if(pBigListBG == NULL) return;
	CUIControl* pSmallListBG = CClientUIManager::GetPtr()->FindEventControl("Slot_Small");
	if(pSmallListBG == NULL) return;

	if(m_bListShape)
	{
		pBigListBG->SetVisible(N3UI_VISIBLE_FALSE);
		pSmallListBG->SetVisible(N3UI_VISIBLE_TRUE);
	}
	else
	{
		pBigListBG->SetVisible(N3UI_VISIBLE_TRUE);
		pSmallListBG->SetVisible(N3UI_VISIBLE_FALSE);
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

void CItemBagProcess::FUNCTION_DivestBagItem(char idx)
{
	if( m_ItemList[idx].m_nCode == 0 || m_ItemList[idx].m_nItemSeq == 0 )
		return;

	PutDivestItem( m_ItemList[idx].m_nItemSeq );
}

void CItemBagProcess::FUNCTION_DivestBagItemS(char idx)
{
	int nSeq = 0;

	for(int i=0;i < (int)m_pObjPlayerInfo->m_vItemList.size();i++)
	{		
		if( m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_nWear == idx && m_pObjPlayerInfo->m_vItemList.at(i)->m_nEquipKind == EQUIP_KIND_YES )
		{
			nSeq = m_pObjPlayerInfo->m_vItemList.at(i)->m_nItemSeq;
			break;
		}
	}

	if( m_pObjPlayerInfo->m_nEquipWear[idx] == 0 || nSeq == 0 )
		return;

	PutDivestItem( nSeq );
}

//void CItemBagProcess::FUNCTION_DivestBagItemS(char idx)
//{
//	for(int i=0;i < static_cast<int>(m_pObjPlayerInfo->m_vItemList.size());i++)
//	{		
//		if( (idx == m_pObjPlayerInfo->m_vItemList.at(i)->m_nItemSeq) && 
//			(m_pObjPlayerInfo->m_vItemList.at(i)->m_nEquipKind == EQUIP_KIND_YES) )
//		{
//			if(100 == m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_nWear)
//			{
//				PutDivestItem( idx );
//				return;
//			}
//
//			char text[STRLEN_64];
//			sprintf(text, "BAG_WearItemImg%d", m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_nType);
//			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(text);
//			if(!pControl) return;
//
//			if(strcmp(m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_sIconFile,"-") && m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_nOrder >= 100)
//			{
//				if( m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
//					sprintf(text, "Icon/Item/%s.png", m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_sIconFile20);
//				else
//					sprintf(text, "Icon/Item/%s.png", m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_sIconFile);
//
//				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pControl, text);
//			}
//			else
//			{
//				sprintf(text, "Basic_%d.png", m_pObjPlayerInfo->m_vItemList.at(i)->m_pTable->m_nType);
//				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pControl, text);
//			}
//			
//			PutDivestItem( m_pObjPlayerInfo->m_vItemList.at(i)->m_nItemSeq );
//			break;
//		}
//	}
//}

void CItemBagProcess::FUNCTION_EquipBagItem(char idx)
{
	if( m_ItemList[idx].m_nCode == 0 || m_ItemList[idx].m_nItemSeq == 0 )
		return;

	PutEquipItem(m_ItemList[idx].m_nItemSeq);
}

void CItemBagProcess::FUNCTION_ExchangeItem(int nCode, char nWear)
{
	char sText[STRLEN_64]="";

	char nSkin = m_pObjPlayerInfo->m_cShape.m_nSkin;

	m_pObjPlayerInfo->m_nEquipWear[nWear] = nCode;
	m_pObjPlayerInfo->m_pEquipWearModelID[nWear] = CObjectModelManager::GetPtr()->ExchangeItem(m_pObjPlayerInfo->m_nEquipWear[nWear], nSkin, m_pObjPlayerInfo->m_pEquipWearModelID[nWear], m_pObjPlayerInfo->m_pModel);

	if( nCode == 0 )
	{
		_snprintf( sText, STRLEN_64, "Slot%02d", nWear );
		UIFunction::SetChildToVisibility( sText, "List_Img", N3UI_VISIBLE_FALSE );
	}
	else
	{
		CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, nCode, nSkin));
		assert(pTable);

		if(ITEM_TYPE_KNEE < pTable->m_nType )
			return;

		_snprintf(sText, STRLEN_64, "Slot%02d", nWear);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		CUIControl* pImg = pControl->FindEventControl( "List_Img" );
		assert(pImg);

		if(m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN)
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

		RemoveItemDivest( m_pObjPlayerInfo->m_pModel->m_spNode );
	}
}

void CItemBagProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
            FUNCTION_Selection();

			if( m_nBigFractionIndex == BF_CARD )
				FUNCTION_DNDDOWN();
		}
		break;
	case WM_LBUTTONUP:
		{
			if( m_nBigFractionIndex == BF_CARD )
				FUNCTION_DNDUP();
		}
		break;
	case WM_MOUSEMOVE:
		{
			m_pDNDPoint.x = LOWORD(lParam);
			m_pDNDPoint.y = HIWORD(lParam);

			if(m_bDnd || m_bEntry || m_bMix)
			{
				FUNCTION_DNDMOVE();

				UIFunction::Set_UIVisibility( "CToolTip", N3UI_VISIBLE_FALSE );
			}
			else
			{
				if( m_nBigFractionIndex == BF_CARD )
				{
					FUNCTION_ItemBagCardToolTipViewUpdate();
					if( m_nCardTabIndex == 0 )
						FUNCTION_ItemBagEntryToolTipViewUpdate();
				}
				else
					FUNCTION_ItemBagToolTipViewUpdate();

				CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "WearSlot" );
				assert(pControl);

				CUIControl* pControl2 = CClientUIManager::GetPtr()->FindEventControl( "Detail_BG_00" );
				assert(pControl2);

				if( pControl->GetVisible() == N3UI_VISIBLE_TRUE && pControl2->GetVisible() == N3UI_VISIBLE_TRUE )
					FUNCTION_ItemBagEquipWearToolTipViewUpdate();
			}
		}
		break;
	}
}

void CItemBagProcess::FUNCTION_Selection()
{
	char sText[STRLEN_64]="";
	int nListCount = 0;

	if(m_bListShape) nListCount = LIST20_SIZE;
	else nListCount = LIST6_SIZE;

	if( m_nBigFractionIndex == 1 )
	{
		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl( "Slot_Card" );
		assert(pBagListBG);
		
		for(int i=0;i<nListCount;i++)
		{
			_snprintf(sText, STRLEN_64, "ListBG%d", i);
			CUIControl* pCard = pBagListBG->FindEventControl( sText );
			assert(pCard);

			CUIControl* pBtn = pCard->FindEventControl( "DetailViewBtn" );
			assert(pBtn);

			if( UICommonMethod::IsRectInPoint( &pBtn->GetWindowBox(), &m_pDNDPoint ) )
				return;
		}
	}

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
			UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "Detail_BG_01", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_FALSE );
			UIFunction::Set_UIVisibility( "Detail_BG_03", N3UI_VISIBLE_FALSE );

			switch( m_nBigFractionIndex )
			{
			case 0:				// 
				UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_TRUE );
				break;
			case 1:
				UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_TRUE );
				break;
			case 2:
			case 3:
			case 4:
				UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_TRUE );
				break;
			}
			m_nSelectIndex = i;
		}
	}
}

void CItemBagProcess::FUNCTION_DNDDOWN()
{
	char sText[STRLEN_64]="";

	CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl( "Slot_Card" );
	assert(pBagListBG);

	CUIControl* pParentDNDCard = CClientUIManager::GetPtr()->FindEventControl( "DNDCardBG" );
	assert(pParentDNDCard);
	CUIControl* pDNDCard = pParentDNDCard->FindEventControl( "ListImg" );
	assert(pDNDCard);

	int nPageNum = LIST6_SIZE * (m_nCurrentPage-1);

	if( nPageNum < 0 ) return;
		
	if( m_nSelectIndex != -1 )		// 카드를 집었다면
	{
		if( (int)m_vCIVectorList.size() <= m_nSelectIndex+nPageNum ) return;

		char nPosition = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nPosition;
		char nRank = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nRank;
		char nLv = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nLevel;
		char nEquip = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nEquipKind[m_nEntry];
		int nSeq = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nCardSeq;
		int nCostume = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nCostume;

		if( m_nCardTabIndex == 0 )			// 이미 엔트리에 착용한 카드라면
		{
			if(nEquip)
				return;

			for(int i = 0; i < 5; i++)
			{
				_snprintf(sText, STRLEN_64, "MY_Entry%d", i);
				CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pEntry);

				if(i==4) i++;

				CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

				if( FUNCTION_IsEntryPostion(i, nPosition) && (!FUNCTION_IsEntryPostion(i, pObjPlayerInfo->m_nPosition) || i==3) )
					UIFunction::Set_UIVisibility(pEntry, "Click_Img", N3UI_VISIBLE_TRUE);			
			}

			m_nCard = m_vCIVectorList.at(m_nSelectIndex+nPageNum)->m_nCardSeq;			
		}
		else if( m_nCardTabIndex == 1 )		// 조합창에 올려진 카드라면
		{
			for(int i=0;i<3;i++)
			{
				if( m_nMix[i] == nSeq )
					return;
			}
		}

		m_bDnd = true;

		pParentDNDCard->SetVisible(N3UI_VISIBLE_TRUE);

		UICommonMethod::FUNCTION_CardInfoDraw(pDNDCard, nPosition, nRank, nLv, nCostume);

		pParentDNDCard->SetWindowRect( m_pDNDPoint.x-( pParentDNDCard->GetWidth()/2), m_pDNDPoint.y-(pParentDNDCard->GetHeight()/2), pParentDNDCard->GetWidth(), pParentDNDCard->GetHeight(), true );
	}
	
	if( m_nCardTabIndex == 0 )		// 엔트리 카드를 집었다면
	{
		for(int i = 0; i < 5; i++)
		{
			//엔트리 영역 검사
			_snprintf( sText, STRLEN_64, "MY_Entry%d", i);
			CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pEntry);

			if(i==4) i++;
			
			CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

			char nUserPositionKind = pObjPlayerInfo->m_nPosition/LIST10_SIZE;
		
			if( 3<nUserPositionKind )
				nUserPositionKind = 6;

			if( nUserPositionKind == i+1 )
				continue;

			// 엔트리 영역의 카드를 집었다면
			if( UICommonMethod::IsRectInPoint(&pEntry->GetWindowBox(), &m_pDNDPoint) )
			{
				for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
				{
					int nEquipa = pObjPlayerInfo->m_vCardbotList.at(j)->m_nEquipKind[m_nEntry];
					if( nEquipa == i+1 )
					{
						m_nCard = i;

						char nPosition = pObjPlayerInfo->m_vCardbotList.at(j)->m_nPosition;
						char nRank = pObjPlayerInfo->m_vCardbotList.at(j)->m_nRank;
						char nLv = pObjPlayerInfo->m_vCardbotList.at(j)->m_nLevel;
						int nCostume = pObjPlayerInfo->m_vCardbotList.at(j)->m_nCostume;
						
						UICommonMethod::FUNCTION_CardInfoDraw(pDNDCard, nPosition, nRank, nLv, nCostume);

						pParentDNDCard->SetWindowRect( m_pDNDPoint.x-( pParentDNDCard->GetWidth()/2), m_pDNDPoint.y-(pParentDNDCard->GetHeight()/2), pParentDNDCard->GetWidth(), pParentDNDCard->GetHeight(), true );

						m_bEntry = true;

						pParentDNDCard->SetVisible(N3UI_VISIBLE_TRUE);
						break;
					}
				}
				break;
			}
		}
	}

	if( m_nCardTabIndex == 1 )		// 조합 카드를 집었다면
	{
		CUIControl* pMixBG = CClientUIManager::GetPtr()->FindEventControl( "Detail_Card1" );
		assert(pMixBG);
		for(int i = 0; i < 3; i++)
		{	
			_snprintf( sText, STRLEN_64, "MIX_%d", i );
			CUIControl* pMIXCard = pMixBG->FindEventControl( sText );
			assert(pMIXCard);

			if( UICommonMethod::IsRectInPoint(&pMIXCard->GetWindowBox(), &m_pDNDPoint) )
			{
				CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

				for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
				{
					int nSeq = pObjPlayerInfo->m_vCardbotList.at(j)->m_nCardSeq;
					if( nSeq == m_nMix[i] )
					{
						char nPosition = pObjPlayerInfo->m_vCardbotList.at(j)->m_nPosition;
						char nRank = pObjPlayerInfo->m_vCardbotList.at(j)->m_nRank;
						char nLv = pObjPlayerInfo->m_vCardbotList.at(j)->m_nLevel;
						char nCostume = pObjPlayerInfo->m_vCardbotList.at(j)->m_nCostume;

						UICommonMethod::FUNCTION_CardInfoDraw(pDNDCard, nPosition, nRank, nLv, nCostume);

						m_nCard = i;
						m_bMix = true;
						break;
					}
				}
				
				pParentDNDCard->SetVisible(N3UI_VISIBLE_TRUE);
			}
		}
	}
}

void CItemBagProcess::FUNCTION_DNDMOVE()
{
	CUIControl* pParentDNDCard = CClientUIManager::GetPtr()->FindEventControl( "DNDCardBG" );
	assert(pParentDNDCard);

	pParentDNDCard->SetWindowRect( m_pDNDPoint.x-( pParentDNDCard->GetWidth()/2), m_pDNDPoint.y-(pParentDNDCard->GetHeight()/2), pParentDNDCard->GetWidth(), pParentDNDCard->GetHeight(), true );
}

void CItemBagProcess::FUNCTION_DNDUP()
{
	char sText[STRLEN_64]="";

	CUIControl* pParentDNDCard = CClientUIManager::GetPtr()->FindEventControl( "DNDCardBG" );
	assert(pParentDNDCard);
	CUIControl* pDNDCard = pParentDNDCard->FindEventControl( "ListImg" );
	assert(pDNDCard);

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	int nPageNum = LIST6_SIZE * (m_nCurrentPage-1);

	// 집은 인벤카드를 놓았다면
	if( m_bDnd && m_nCardTabIndex == 0 )
	{
		for(int i = 0; i < 5; i++)
		{
			_snprintf(sText, STRLEN_64, "MY_Entry%d", i);
			CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pEntry);
			CUIControl* pImg = pEntry->FindEventControl( "ListImg" );
			assert(pImg);

			UIFunction::Set_UIVisibility(pEntry, "Click_Img", N3UI_VISIBLE_FALSE);

			if( i==4 ) i++;

			char nUserPositionKind = pObjPlayerInfo->m_nPosition/LIST10_SIZE;
	
			if( 3<nUserPositionKind )
				nUserPositionKind = 6;

			if( nUserPositionKind == i+1 )
				continue;

			char nPosition = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nPosition;
			char nRank = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nRank;
			char nLv = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nLevel;
			char nEquip = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nEquipKind[m_nEntry];
			int nCostume = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nCostume;

			// 엔트리영역에 카드를 놓았다면
			if( UICommonMethod::IsRectInPoint(&pEntry->GetWindowBox(), &m_pDNDPoint) )
			{
				if( FUNCTION_IsEntryPostion( i, nPosition ) )
				{
					for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
					{
						if( pObjPlayerInfo->m_vCardbotList.at(j)->m_nEquipKind[m_nEntry] == i+1 )
							PutDivestCard(pObjPlayerInfo->m_vCardbotList.at(j)->m_nCardSeq, m_nEntry);
					}					
				}
				else
					break;

				pImg->SetVisible(N3UI_VISIBLE_TRUE);

				UICommonMethod::FUNCTION_CardInfoDraw(pImg, nPosition, nRank, nLv, nCostume);

				PutEquipCard(m_nCard, i+1, m_nEntry);
				break;
			}
		}
		m_bDnd = false;
		m_nSelectIndex = -1;
	}

	if( m_bDnd && m_nCardTabIndex == 1 )		// 조합 카드에 놓았다면
	{
		CUIControl* pMixBG = CClientUIManager::GetPtr()->FindEventControl( "Detail_Card1" );
		assert(pMixBG);
		for(int i = 0; i < 3; i++)
		{	
			_snprintf( sText, STRLEN_64, "MIX_%d", i );
			CUIControl* pMIXCard = pMixBG->FindEventControl( sText );
			assert(pMIXCard); 

			CUIControl* pCardBG = pMIXCard->FindEventControl( "ListImg" );
			assert(pCardBG);

			if( UICommonMethod::IsRectInPoint(&pMIXCard->GetWindowBox(), &m_pDNDPoint) )
			{
				UIFunction::SetChildToVisibility( "MixResultBG", "Cover", N3UI_VISIBLE_TRUE );

				char nPosition = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nPosition;
				char nRank = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nRank;
				char nLv = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nLevel;
				char nEquip = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nEquipKind[m_nEntry];
				int nSeq = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nCardSeq;
				int nCostume = m_vCIVectorList.at(nPageNum+m_nSelectIndex)->m_nCostume;

				if( m_nMixReservedRank == -1 )
					m_nMixReservedRank = nRank;
				else
				{
					if( m_nMixReservedRank != nRank )
					{
						CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 22000, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
						break;
					}
				}

				pCardBG->SetVisible(N3UI_VISIBLE_TRUE);

				UICommonMethod::FUNCTION_CardInfoDraw(pCardBG, nPosition, nRank, nLv, nCostume);

				m_nMix[i] = nSeq;
				break;
			}
		}
		m_bDnd = false;
		m_nSelectIndex = -1;
	}

	if( m_bEntry && m_nCardTabIndex == 0 )		// 집은 엔트리 카드를 놓았다면
	{
		int nCount = 0;
			
		// 킵퍼이면
		if( m_nCard == 5 )
			_snprintf(sText, MAX_PATH, "MY_Entry%d", m_nCard - 1);
		else
			_snprintf(sText, MAX_PATH, "MY_Entry%d", m_nCard);

		CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pEntry);

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
		char nUserPositionKind = pObjPlayerInfo->m_nPosition/LIST10_SIZE;
		
		if( 4==m_nCard )
			nUserPositionKind = 6;

		if( !UICommonMethod::IsRectInPoint(&pEntry->GetWindowBox(), &m_pDNDPoint) )
		{
			for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
			{
				if(pObjPlayerInfo->m_vCardbotList.at(j)->m_nEquipKind[m_nEntry] == m_nCard+1)
				{
					PutDivestCard(pObjPlayerInfo->m_vCardbotList.at(j)->m_nCardSeq, m_nEntry);
					break;
				}
			}			

			CUIControl* pImg = pEntry->FindEventControl( "ListImg" );
			assert(pImg);

			if( nUserPositionKind  == m_nCard+1 )
			{
				CClientUIManager::GetPtr()->TextureChange( N3UI_STATE_NORMAL, pImg, "Icon/Card/Card_User.png" );
				pImg->SetVisible( N3UI_VISIBLE_TRUE );
			}
			else
				pImg->SetVisible(N3UI_VISIBLE_FALSE);
		}
		m_bEntry = false;
	}

	// 집은 조합될 카드를 밖에 놓았다면
	if( m_bMix && m_nCardTabIndex == 1 )
	{
		CUIControl* pMixBG = CClientUIManager::GetPtr()->FindEventControl( "Detail_Card1" );
		assert(pMixBG);
		
		_snprintf( sText, STRLEN_64, "MIX_%d", m_nCard );
		CUIControl* pMIXCard = pMixBG->FindEventControl( sText );
		assert(pMIXCard);

		CUIControl* pCardBG = pMIXCard->FindEventControl( "ListImg" );
		assert(pCardBG);

		if( !UICommonMethod::IsRectInPoint(&pMIXCard->GetWindowBox(), &m_pDNDPoint) )
		{		
			pCardBG->SetVisible(N3UI_VISIBLE_FALSE);

			m_nMix[m_nCard] = 0;
			m_bMix = false;

			for( int i=0;i<3;i++)
			{
				if( m_nMix[i] != 0 )
				{
					pParentDNDCard->SetVisible(N3UI_VISIBLE_FALSE);
					return;
				}
			}

			m_nMixReservedRank = -1;
		}
	}
	
	pParentDNDCard->SetVisible(N3UI_VISIBLE_FALSE);
}

void CItemBagProcess::FUNCTION_CardTabChange(char idx)
{
	m_nCardTabIndex = idx;

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
		if( idx == 1 )
		{
			_snprintf( sText, STRLEN_64, "MIX_%d", i );
			CUIControl* pMIXCard = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pMIXCard);

			CUIControl* pCardBG = pMIXCard->FindEventControl( "ListImg" );
			assert(pCardBG);

			if( m_nMix[i] == 0 )
				pCardBG->SetVisible(N3UI_VISIBLE_FALSE);
		}

		_snprintf( sText, STRLEN_64, "CardDetail_TabButton0%d", i );
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);

		if( i == idx )
			pBtn->SetPressed(true);
		else
			pBtn->SetPressed(false);

		if( i == 2 ) continue;
		_snprintf( sText, STRLEN_64, "Detail_Card%d", i );
		if( i == idx )
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
		else
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	}
}
void CItemBagProcess::FUNCTION_CardMix()
{
	int  m_nCardSeq[CARD_MIX_SIZE]={0,};

	m_nCardSeq[0] = m_nMix[0];
	m_nCardSeq[1] = m_nMix[1];
	m_nCardSeq[2] = m_nMix[2];

	PutMixCard1( CARD_MIX_RANK, m_nCardSeq );
}

void CItemBagProcess::FUNCTION_CardMixResult(CCardInfo cCardInfo)
{
	char sText[STRLEN_64]="";

	CUIControl* pMixBG = CClientUIManager::GetPtr()->FindEventControl( "MixResultBG" );
	assert(pMixBG);

	CUIControl* pMixR = pMixBG->FindEventControl( "ListImg" );
	assert(pMixR);

	UICommonMethod::FUNCTION_CardInfoDraw(pMixR, cCardInfo.m_nPosition, cCardInfo.m_nRank, cCardInfo.m_nLevel, cCardInfo.m_nCostume);

	UIFunction::SetChildToVisibility( pMixBG, "Cover", N3UI_VISIBLE_FALSE );

	CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	for(int i=0;i<3;i++)
	{
		_snprintf( sText, STRLEN_64, "MIX_%d", i );
		CUIControl* pMIXCard = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pMIXCard);

		CUIControl* pCardBG = pMIXCard->FindEventControl( "ListImg" );
		assert(pCardBG);

		pCardBG->SetVisible(N3UI_VISIBLE_FALSE);

		VectorCardbotList::iterator it;

		for(it = pPlayer->m_vCardbotList.begin();it!=pPlayer->m_vCardbotList.end();it++)
		{
			if( m_nMix[i] == (*it)->m_nCardSeq )
			{
				pPlayer->m_vCardbotList.erase(it);
				break;
			}
		}

		m_nMix[i] = 0;
		m_nMixReservedRank = -1;
	}

	FUNCTION_InitCardShop();
	FUNCTION_GoPage(1);
}

bool CItemBagProcess::FUNCTION_IsEntryPostion( char nEntry, char nPosition )
{
	switch( nEntry )
	{
	case 0:
		{
			if((int)(nPosition/LIST10_SIZE) == 1)
				return true;
			else
				return false;
		}
	case 1:
		{
			if((int)(nPosition/LIST10_SIZE) == 2)
				return true;
			else
				return false;
		}
	case 2:
		{
			if((int)(nPosition/LIST10_SIZE) == 3)
				return true;
			else
				return false;
		}
	case 3:	
		{
			if(nPosition == 40)
				return false;
			else
				return true;
		}
	case 5:
		{
			if((int)(nPosition/LIST10_SIZE) == 4)
				return true;
			else
				return false;	
		}
	default:
		return false;
	}
}

void CItemBagProcess::FUNCTION_ItemBagToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	char sAllOption[MAX_PATH]="";

	CUIControl* pBackPannel = NULL;

	RECT rt;
	
	int nCount = 0;

	if(m_bListShape)	// 메인 작은 아이콘
	{
		// List1_bg, ListBG0~ListBG19
		pBackPannel = CClientUIManager::GetPtr()->FindEventControl("Slot_Small");
		assert(pBackPannel);

		nCount = 20;
	}
	else
	{
		// List0_bg, ListBG0~ListBG5
		pBackPannel = CClientUIManager::GetPtr()->FindEventControl("Slot_Big");
		assert(pBackPannel);

		nCount = LIST6_SIZE;
	}

	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("ToolTip");
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

	if( m_ItemList[i].m_nLevel == 0 )
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  CResourceManager::GetPtr()->m_pStaticMap[210020].c_str() );
	else
	{
		_snprintf( sText, MAX_PATH, "LEVEL %d", m_ItemList[i].m_nLevel );
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  sText );
	}

	UIFunction::Set_ChildControlText( pToolTip, "ItemName", m_ItemList[i].m_pTable->m_sName );

	// 등급에 따른 네임색 변경
	switch(m_ItemList[i].m_nGrade)
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
	int nLen		= strlen(m_ItemList[i].m_pTable->m_sName);

	for( int j=0;j<LIST5_SIZE;j++)
	{
		_snprintf( sText, MAX_PATH, "Enchant_%d", j );
		CUIControl* pEOn = pEnchant->FindEventControl( sText );
		
		if( j < m_ItemList[i].m_nGrade )
			pEOn->SetVisible(N3UI_VISIBLE_TRUE);
		else
		{
			pEOn->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}
		
		if( m_ItemList[i].m_nOptionCode[j] != 0 )
			UIFunction::ChangeTexture( pEOn, "EnchantOn" );
		else
		{
			UIFunction::ChangeTexture( pEOn, "EnchantOff" );
			continue;
		}

		if( 0 < j )	strcat(sAllOption, "\n");

		int nSituationCode	= ITEM_TYPE_SITUATION(m_ItemList[i].m_nOptionCode[j]);
		int nFacultyCode	= ITEM_TYPE_FACULTY(m_ItemList[i].m_nOptionCode[j]);
		int nFaculty		= ITEM_TYPE_VALUE(m_ItemList[i].m_nOptionCode[j]);

		char sOption[STRLEN_64]="";

		_snprintf(sOption, STRLEN_64, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);

		strcat(sAllOption, sOption);
		nLine++;
		
		if(nLen<(int)strlen(sOption))
			nLen = strlen(sOption);
	}

	if( 0 < m_ItemList[i].m_nGrade )
		nLine+=2;

	if( 90+(nLen*5) < 150 )
		pToolTip->SetSize(150, 70+(16*nLine), true);
	else
		pToolTip->SetSize(90+(nLen*5), 70+(16*nLine), true);

	pToolTip->SetLocation(m_pDNDPoint.x-(pToolTip->GetWidth()/2), m_pDNDPoint.y+5, true);

	UIFunction::Set_ChildControlText( pToolTip, "description", sAllOption );
}

void CItemBagProcess::FUNCTION_ItemBagEquipWearToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	char sAllOption[MAX_PATH]="";

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
	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_pObjPlayerInfo->m_nEquipWear[nWear], 0));
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
	UIFunction::Set_Color( pToolTip, "ItemName", 0xffffffff);

	int nGrade = 0;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

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
				
				int nOption = m_pObjPlayerInfo->m_vItemList.at(j)->m_nOptionCode[k];

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


void CItemBagProcess::FUNCTION_ItemBagCardToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	RECT rt;
	int nCount = 0;

	CUIControl* pBackPannel = CClientUIManager::GetPtr()->FindEventControl("Slot_Card");
	assert(pBackPannel);

	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("CToolTip");
	assert(pToolTip);
	pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	
	int nPageNum	= (m_nCurrentPage-1)*LIST6_SIZE;
	char nRank		= 0;
	char nLv		= 0;
	char nPosition	= 0;
	char nSLv		= 0;

	for(int i=0;i<LIST6_SIZE;i++)
	{
		_snprintf(sText, MAX_PATH, "ListBG%d", i);
		CUIControl* pOverItem = pBackPannel->FindEventControl(sText);
		assert(pOverItem);

		rt = pOverItem->GetWindowBox();

		if( m_vCIVectorList.size() < i+nPageNum )	continue;
		
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pOverItem->GetVisible() == N3UI_VISIBLE_TRUE)
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);

			nRank		= m_vCIVectorList.at(i+nPageNum)->m_nRank;
			nLv			= m_vCIVectorList.at(i+nPageNum)->m_nLevel;
			nPosition	= m_vCIVectorList.at(i+nPageNum)->m_nPosition;
			nSLv		= m_vCIVectorList.at(i+nPageNum)->m_nSkill;

			// Rank
			_snprintf( sText, MAX_PATH, "%c RANK", UICommonMethod::RankNtoRankChar(nRank) );
			UIFunction::Set_ChildControlText( "CToolTip", "rank", sText );
			UIFunction::Set_Color( "CToolTip", "rank", UICommonMethod::RankNtoRankColor(nRank) );

			// Level
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
			assert(pMTable);
			_snprintf( sText, MAX_PATH, pMTable->m_sMessage, nLv );
			UIFunction::Set_ChildControlText( "CToolTip", "level", sText );

			// Position
			_snprintf( sText, MAX_PATH, "%s", UICommonMethod::PositionNtoPositionStr(nPosition).c_str() );
			UIFunction::Set_ChildControlText( "CToolTip", "position", sText );

			// Skill Level
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900100 ,0));
			assert(pMTable);
			_snprintf( sText, MAX_PATH, pMTable->m_sMessage, nSLv );
			UIFunction::Set_ChildControlText( "CToolTip", "slevel", sText );

			pToolTip->SetLocation(m_pDNDPoint.x-(pToolTip->GetWidth()/2), m_pDNDPoint.y+5, true);
			break;
		}
		else
			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

void CItemBagProcess::FUNCTION_ItemBagEntryToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	RECT rt;
	
	for(int i=0;i<LIST5_SIZE;i++)
	{
		char nForm		= 0;
		char nRank		= 0;
		char nLv		= 0;
		char nPosition	= 0;
		char nSLv		= 0;

		nForm = i;
		// 킵퍼이면
		if( i == 5 )
			_snprintf(sText, MAX_PATH, "MY_Entry%d", i - 1);
		else
			_snprintf(sText, MAX_PATH, "MY_Entry%d", i);

		CUIControl* pEntry = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pEntry);

		rt = pEntry->GetWindowBox();

		if( 3<nForm )	 nForm++;

		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		// 엔트리 영역에 카드가 있다면
		for(int j=0;j<(int)pObjPlayerInfo->m_vCardbotList.size();j++)
		{
			if(pObjPlayerInfo->m_vCardbotList.at(j)->m_nEquipKind[m_nEntry] == nForm+1 )
			{
				nRank		= pObjPlayerInfo->m_vCardbotList.at(j)->m_nRank;
				nLv			= pObjPlayerInfo->m_vCardbotList.at(j)->m_nLevel;
				nPosition	= pObjPlayerInfo->m_vCardbotList.at(j)->m_nPosition;
				nSLv		= pObjPlayerInfo->m_vCardbotList.at(j)->m_nSkill;
				break;
			}
		}
	
		CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("EToolTip");
		assert(pToolTip);
	
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && nLv )
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);

			// Rank
			_snprintf( sText, MAX_PATH, "%c RANK", UICommonMethod::RankNtoRankChar(nRank) );
			UIFunction::Set_ChildControlText( pToolTip, "rank", sText );
			UIFunction::Set_Color( pToolTip, "rank", UICommonMethod::RankNtoRankColor(nRank) );

			// Level
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
			assert(pMTable);
			_snprintf( sText, MAX_PATH, pMTable->m_sMessage, nLv );
			UIFunction::Set_ChildControlText( pToolTip, "level", sText );

			// Position
			_snprintf( sText, MAX_PATH, "%s", UICommonMethod::PositionNtoPositionStr(nPosition).c_str() );
			UIFunction::Set_ChildControlText( pToolTip, "position", sText );

			// Skill Level
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900100 ,0));
			assert(pMTable);
			_snprintf( sText, MAX_PATH, pMTable->m_sMessage, nSLv );
			UIFunction::Set_ChildControlText( pToolTip, "slevel", sText );

			pToolTip->SetLocation(m_pDNDPoint.x-(pToolTip->GetWidth()/2), m_pDNDPoint.y+5, true);
			break;
		}
		else
			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	}
}

void CItemBagProcess::FUNCTIOIN_Enchant(char idx)
{
	UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_01", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_FALSE );

	m_nEnchantIndex = idx;
	m_nEnchantBuyKind = BUY_KIND_POINT;

	FUNCTION_EnchantChange(1);

	char sText[MAX_PATH]="";

	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_ItemList[idx].m_nCode, 0));
	assert(pTable);

	if( m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN )
		_snprintf(sText, MAX_PATH, "./Icon/Item/%s.png", pTable->m_sIconFile20);
	else
		_snprintf(sText, MAX_PATH, "./Icon/Item/%s.png", pTable->m_sIconFile);

	CUIControl* pEnchantBG = CClientUIManager::GetPtr()->FindEventControl( "EnchantItemBG" );
	assert(pEnchantBG);

	UIFunction::Set_ChildControlTexture( pEnchantBG, "ListImg", sText );

	UIFunction::Set_ChildControlText( pEnchantBG, "ItemName", m_ItemList[idx].m_pTable->m_sName );

	_snprintf( sText, STRLEN_64, "LEVEL %d", m_ItemList[idx].m_nLevel );
	UIFunction::Set_ChildControlText( pEnchantBG, "ItemLevel", sText );

	switch( m_ItemList[idx].m_nGrade )
	{
		case 1:	UIFunction::Set_Color(pEnchantBG, "ItemName", 0xffcacaca);	break;
		case 2:	UIFunction::Set_Color(pEnchantBG, "ItemName", 0xffb3fe00);	break;
		case 3:	UIFunction::Set_Color(pEnchantBG, "ItemName", 0xff0496f4);	break;
		case 4:	UIFunction::Set_Color(pEnchantBG, "ItemName", 0xffff56ed);	break;
		case 5:	UIFunction::Set_Color(pEnchantBG, "ItemName", 0xffffb400);	break;
	}

	int nEnchant = 0;

	for( int i=0;i<LIST5_SIZE;i++)
	{
		_snprintf( sText, MAX_PATH, "GradeBG0%d", i );
		if( m_ItemList[idx].m_nGrade == i+1 )
			UIFunction::SetChildToVisibility( pEnchantBG, sText, N3UI_VISIBLE_TRUE );
		else
			UIFunction::SetChildToVisibility( pEnchantBG, sText, N3UI_VISIBLE_FALSE );

		_snprintf( sText, MAX_PATH, "Enchant_%d", i );

		if( i < m_ItemList[idx].m_nGrade )
			UIFunction::SetChildToVisibility( pEnchantBG, sText, N3UI_VISIBLE_TRUE );
		else
			UIFunction::SetChildToVisibility( pEnchantBG, sText, N3UI_VISIBLE_FALSE );

		CUIControl* pOptionBox = pEnchantBG->FindEventControl( "OptionBox" );
		assert(pOptionBox);

		int nOption = m_ItemList[idx].m_nOptionCode[i];

		if( nOption != 0 )
		{
			nEnchant++;

			UIFunction::ChangeChildTexture( pEnchantBG, sText, "EnchantOn" );

			int nSituationCode	= ITEM_TYPE_SITUATION(nOption);
			int nFacultyCode	= ITEM_TYPE_FACULTY(nOption);
			int nFaculty		= ITEM_TYPE_VALUE(nOption);
	        
			char sOption[MAX_PATH]="";
			_snprintf(sOption, MAX_PATH, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);
			_snprintf(sText, MAX_PATH, "Option_%d", i);
			UIFunction::Set_ChildControlText( pOptionBox, sText, sOption );
			UIFunction::SetChildToVisibility( pOptionBox, sText, N3UI_VISIBLE_TRUE );
		}
		else
		{
			UIFunction::ChangeChildTexture( pEnchantBG, sText, "EnchantOff" );
			_snprintf(sText, MAX_PATH, "Option_%d", i);
			UIFunction::SetChildToVisibility( pOptionBox, sText, N3UI_VISIBLE_FALSE );
		}
	}

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900101 ,0));
	assert(pMTable);
	_snprintf( sText, MAX_PATH, pMTable->m_sMessage, m_ItemList[idx].m_nGrade - nEnchant );
	UIFunction::Set_ChildControlText( pEnchantBG, "RemainEnchantNum", sText );
}

void CItemBagProcess::FUNCTION_EnchantChange(char idx)
{	
	char sText[STRLEN_64]="";

	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("CoinChkBox"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("PointChkBox"));
	assert(pChkBox1);

	int nLevel = m_ItemList[m_nEnchantIndex].m_nLevel - (m_ItemList[m_nEnchantIndex].m_nLevel % 5) + 5;

	switch(idx)
	{
	case 0:
		{
			m_nEnchantBuyKind = BUY_KIND_CASH;

			for(int i=0;i<3;i++)
			{
				CResEnchantTableFormat* pTable = dynamic_cast<CResEnchantTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_ENCHANTTABLE, nLevel + (1000 * (i+1)), 0));
				assert(pTable);
				_snprintf( sText, STRLEN_64, "EnchantBtn%d", i );
				
				char sCoin[STRLEN_32]="";
				
				_snprintf( sCoin, STRLEN_32, "%d", pTable->m_nCash );
				UIFunction::Set_ControlText( sText, sCoin );
			}

			pChkBox0->SetCheckValue(true);
			pChkBox1->SetCheckValue(false);
		}
		break;
	case 1:
		{
			m_nEnchantBuyKind = BUY_KIND_POINT;
			for(int i=0;i<3;i++)
			{
				CResEnchantTableFormat* pTable = dynamic_cast<CResEnchantTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_ENCHANTTABLE, nLevel + (1000 * (i+1)), 0));
				assert(pTable);

				_snprintf( sText, STRLEN_64, "EnchantBtn%d", i );
				
				char sPoint[STRLEN_32]="";
				
				_snprintf( sPoint, STRLEN_32, "%d", pTable->m_nPoint );
				UIFunction::Set_ControlText( sText, sPoint );
			}

			pChkBox0->SetCheckValue(false);
			pChkBox1->SetCheckValue(true);
		}
		break;
	default:;
	}	
}

void CItemBagProcess::FUNCTION_EnchantOk(char idx)
{
	int nLevel = m_ItemList[m_nEnchantIndex].m_nLevel - (m_ItemList[m_nEnchantIndex].m_nLevel % 5) + 5;

	CResEnchantTableFormat* pTable = dynamic_cast<CResEnchantTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_ENCHANTTABLE, nLevel+(1000*(idx+1)), 0));
	assert(pTable);

	switch( m_nEnchantBuyKind )
	{
	case BUY_KIND_CASH:
		PutEnchantItem(m_ItemList[m_nEnchantIndex].m_nItemSeq, pTable->m_nCash, idx, m_nEnchantBuyKind );
		break;
	case BUY_KIND_POINT:
		PutEnchantItem(m_ItemList[m_nEnchantIndex].m_nItemSeq, pTable->m_nPoint, idx, m_nEnchantBuyKind );
		break;
	}
}

void CItemBagProcess::FUNCTION_CardDetailView(char idx)
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "Detail_BG_00", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_01", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_02", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Detail_BG_03", N3UI_VISIBLE_TRUE );

	int nPageNum = LIST6_SIZE * (m_nCurrentPage-1);

	CUIControl* pDetailCardBG = CClientUIManager::GetPtr()->FindEventControl( "Detail_CardBG" );
	assert(pDetailCardBG);

	CUIControl* pListBG = pDetailCardBG->FindEventControl( "ListBG" );
	assert(pListBG);

	CUIControl* pCard = pListBG->FindEventControl( "ListImg");
	assert(pCard);

	int nRank		= m_vCIVectorList.at(nPageNum+idx)->m_nRank;
	int nLv		= m_vCIVectorList.at(nPageNum+idx)->m_nLevel;
	int nPosition	= m_vCIVectorList.at(nPageNum+idx)->m_nPosition;
	int nCostume = m_vCIVectorList.at(nPageNum+idx)->m_nCostume;
	
	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	
	_snprintf( sText, STRLEN_64, pMTable->m_sMessage, nLv );

	UIFunction::Set_ChildControlText( pDetailCardBG, "Level", sText );
	UIFunction::Set_ChildControlText( pDetailCardBG, "Position", UICommonMethod::PositionNtoPositionStr(nPosition).c_str() );
	UIFunction::Set_ChildControlText( pDetailCardBG, "Rank", CResourceManager::GetPtr()->m_pStaticMap[ 150033-nRank ].c_str() );

	UICommonMethod::FUNCTION_CardInfoDraw(pCard, nPosition, nRank, nLv, nCostume);
}