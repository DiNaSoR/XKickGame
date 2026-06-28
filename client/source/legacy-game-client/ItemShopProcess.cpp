#include "stdafx.h"
#include "ItemShopProcess.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "PlayModeManager.h"
#include "ResTableStack.h"
#include "Common.h"
#include "CoreApi.h"
#include "ResItemFormat.h"
#include "SignBoard.h"
#include "UIFunction.h"

CItemShopProcess::CItemShopProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_ITEMSHOP;

	m_ItemTotalCount = 0;

	m_nSmallFractionStart	= 0;
	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호
	m_nSelectIndex			= -1;

	m_bLookChange			= false;

	memset(m_nShopEquipWear, 0, sizeof(m_nShopEquipWear));
	
	m_nCurrentTime = 0;

	m_nCurrentPage = 1;

	m_nTotalPage = 1;

	m_nSkin = 0;

	m_bCardAni = false;
}

CItemShopProcess::~CItemShopProcess(void)
{
}

void CItemShopProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CItemShopProcess::Update(float fFrameTime, float fAccumTime)
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

	FUNCTION_UpdateShop();
}

void CItemShopProcess::LoadGround()
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


void CItemShopProcess::LoadCharacter()
{
	CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pObjPlayerInfo, 0, 0);

	m_pObjPlayerInfo->m_cNodePoint = NiPoint3(10000,0,0);

	m_pObjPlayerInfo->m_cCurrentAction.m_fStartAngle = 180.f;

	CProcessManager::GetPtr()->AttachItem(m_pObjPlayerInfo);

	m_nSkin = m_pObjPlayerInfo->m_cShape.m_nSkin;

	for( int i=0; i<MAX_EQUIP; i++ )
	{
		m_nShopEquipWear[i]    = m_pObjPlayerInfo->m_nEquipWear[i];

		if( m_nShopEquipWear[i] == 0 ) 
		{
			m_pEquipWearModelID[i] = 0;
			m_pObjPlayerInfo->m_pEquipWearModelID[i] = 0;
		}
		else
		{
			m_pEquipWearModelID[i] = m_pObjPlayerInfo->m_pEquipWearModelID[i];
		}
	}

	CObjectModelManager::GetPtr()->m_pBallNode = NULL;
}

void CItemShopProcess::InitProcess()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->Init();

	srand(time(NULL));

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "Detail_Bg", "InventoryBg" );

	PutRandomshopitemList();

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(0);

	FUNCTION_3DRender( "Detail_Bg", pObjGroundInfo->GetObjectCamera());
}

void CItemShopProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	Update(fFrameTime, fAccumTime);

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
	//UICommonMethod::BGEffectAnimationPlay( "Detail_Bg", "InventoryBg" );
	UICommonMethod::BasicInfoStop();
	//SignBoard::GetPtr()->DisplaySignBoard();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);

	if( m_bCardAni )
		FUNCTION_CardBoosterUpdate();
}

void CItemShopProcess::RenderFrame()
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

void CItemShopProcess::UpdateState(float fFrameTime, float fAccumTime)
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

bool CItemShopProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CItemShopProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

bool CItemShopProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
	{
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	}

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CItemShopProcess::GetSameProcess()
{
	return new CItemShopProcess;
}

void CItemShopProcess::ReloadCharacter()
{
	for(int i = 0; i < MAX_EQUIP; i++)
	{
		if(m_nShopEquipWear[i] < 0) continue;

        CResItemFormat * pItemFormat    = dynamic_cast<CResItemFormat*>( m_pEquipWearModelID[i] );
        CResItemFormat * pOrgItemFormat = dynamic_cast<CResItemFormat*>( m_pObjPlayerInfo->m_pEquipWearModelID[i] );

        if( pItemFormat != 0 && pItemFormat != pOrgItemFormat )
        {
            CObjectModelManager::GetPtr()->DeleteModelObject( pItemFormat );
            m_pEquipWearModelID[i] = 0;
        }

		FUNCTION_ExampleExchangeItem(m_nShopEquipWear[i], i, true );
	}
}

void CItemShopProcess::CharacterEquipInit()
{	
	memcpy(m_nShopEquipWear, m_pObjPlayerInfo->m_nEquipWear, sizeof(m_nShopEquipWear));
	
	m_nSkin = m_pObjPlayerInfo->m_cShape.m_nSkin;

	ReloadCharacter();
}

void CItemShopProcess::FUNCTION_InitShop()
{
	for(int i = 0; i < MAX_EQUIP; i++)
	{
		char sText[STRLEN_64]="";
		
		_snprintf(sText, STRLEN_64, "Slot%02d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		if( pControl == NULL)	continue;

		CUIControl* pImg = pControl->FindEventControl( "List_Img" );
		assert(pImg);

		if(m_nShopEquipWear[i] == 0)	
		{
			UIFunction::Set_UIVisibility( pControl, "List_Img", N3UI_VISIBLE_FALSE );
			continue;
		}

		CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_nShopEquipWear[i], m_nSkin));
		assert(pTable);

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

void CItemShopProcess::FUNCTION_BigFractionInitShop(char nBigFraction)
{
	for(int i = 0; i < LIST6_SIZE; i++)
	{
		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "Slot_Big");
		CUIControl* pBagListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pBagListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pBagListBG->FindEventControl(sText);
		assert(pControl);

		UIFunction::SetChildToVisibility(pControl, "BuyBtn", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "GiftBtn", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "ListPoint", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "PointIcon", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "ListCoin", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "CoinIcon", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "MoneyBox", N3UI_VISIBLE_FALSE);
		UIFunction::SetChildToVisibility(pControl, "ExchangeBtn", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("RefreshBtn", N3UI_VISIBLE_FALSE);
	
		switch(nBigFraction)
		{
		case 0:
			UIFunction::SetChildToVisibility(pControl, "BuyBtn", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "GiftBtn", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "ListPoint", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "PointIcon", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "MoneyBox", N3UI_VISIBLE_TRUE);
			UIFunction::Set_UIVisibility("RefreshBtn", N3UI_VISIBLE_TRUE);
			break;
		case 1:
			
			UIFunction::SetChildToVisibility(pControl, "ListPoint", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "PointIcon", N3UI_VISIBLE_TRUE);
			/*UIFunction::SetChildToVisibility(pControl, "ListCoin", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "CoinIcon", N3UI_VISIBLE_TRUE);
			*/UIFunction::SetChildToVisibility(pControl, "MoneyBox", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "ExchangeBtn", N3UI_VISIBLE_TRUE);
			break;
		case 2:
		case 3:
		case 4:
			UIFunction::SetChildToVisibility(pControl, "BuyBtn", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "GiftBtn", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "ListPoint", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "PointIcon", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "ListCoin", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "CoinIcon", N3UI_VISIBLE_TRUE);
			UIFunction::SetChildToVisibility(pControl, "MoneyBox", N3UI_VISIBLE_TRUE);
			break;
		}
	}
}

void CItemShopProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
			FUNCTION_Selection();
		break;
	case WM_MOUSEMOVE:
		{
			m_pDNDPoint.x = LOWORD(lParam);
			m_pDNDPoint.y = HIWORD(lParam);
		
			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "TOP_TabButton00" );
			assert(pControl);
			if( CUIControl::Get_MouseOveredControl() == pControl )
				UIFunction::Set_UIVisibility( "TodayMent", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( "TodayMent", N3UI_VISIBLE_FALSE );

			if( m_nBigFractionIndex != 2 )
				FUNCTION_ItemShopToolTipViewUpdate();
			FUNCTION_ItemShopEquipWearToolTipViewUpdate();
		}
		break;
	}
}

void CItemShopProcess::FUNCTION_UpdateShop()
{
	for(int i = 0; i < LIST6_SIZE; i++)
	{
		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "Slot_Big");
		CUIControl* pShopListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pShopListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pShopListBG->FindEventControl(sText);
		assert(pControl);

		if(m_CurrentItemList[i].m_nCode != 0)
		{
			if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )		// 마우스 오버 이미지 처리
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "over_Img", N3UI_VISIBLE_FALSE );

			if( m_nSelectIndex == i )
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_FALSE );

			if( 1 < m_CurrentItemList[i].m_nAmount )													// 1개 이상 아이템 총 개수 표시
			{
				UIFunction::Set_UIVisibility( pControl, "SHOP_ItemBack", N3UI_VISIBLE_TRUE );
				UIFunction::Set_UIVisibility( pControl, "SHOP_ItemNext", N3UI_VISIBLE_TRUE );
			}
			else
			{
				UIFunction::Set_UIVisibility( pControl, "SHOP_ItemBack", N3UI_VISIBLE_FALSE );
				UIFunction::Set_UIVisibility( pControl, "SHOP_ItemNext", N3UI_VISIBLE_FALSE );
			}

			if( 2 < m_CurrentItemList[i].m_nGrade )
				UIFunction::Set_UIVisibility( pControl, "BestItem", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "BestItem", N3UI_VISIBLE_FALSE );

			if( m_nBigFractionIndex == 1 )
			{
				UIFunction::Set_UIVisibility( pControl, "BuyBtn", N3UI_VISIBLE_FALSE );
				UIFunction::Set_UIVisibility( pControl, "GiftBtn", N3UI_VISIBLE_FALSE );

				int nCode = m_pObjPlayerInfo->m_nEquipWear[m_CurrentItemList[i].m_pTable->m_nWear]%1000000;
				if( (nCode/100000) > 0 )
					UIFunction::Set_UIVisibility( pControl, "ExchangeBtn", N3UI_VISIBLE_TRUE );
				else
					UIFunction::Set_UIVisibility( pControl, "ExchangeBtn", N3UI_VISIBLE_FALSE );
			}
			else
			{
				UIFunction::Set_UIVisibility( pControl, "BuyBtn", N3UI_VISIBLE_TRUE );
				UIFunction::Set_UIVisibility( pControl, "GiftBtn", N3UI_VISIBLE_TRUE );
				UIFunction::Set_UIVisibility( pControl, "ExchangeBtn", N3UI_VISIBLE_FALSE );
			}

			if( m_nSelectIndex != -1 && m_bLookChange )
			{
				m_bLookChange = false;

				if( m_CurrentItemList[m_nSelectIndex].m_nCode == 0 )	return;

				if(ITEM_TYPE_KNEE < m_CurrentItemList[m_nSelectIndex].m_pTable->m_nType)
					continue;

				CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_CurrentItemList[m_nSelectIndex].m_nCode, m_nSkin));
				
				if(pTable->m_nType == ITEM_TYPE_FACE && pTable->m_nColor != m_nSkin)
				{
					m_nSkin = pTable->m_nColor;
					m_nShopEquipWear[pTable->m_nWear] = m_CurrentItemList[m_nSelectIndex].m_nCode;
					ReloadCharacter();
				}
				else if(pTable->m_nType == ITEM_TYPE_GLOVE && m_CurrentItemList[m_nSelectIndex].m_nCode != m_nShopEquipWear[pTable->m_nWear])
				{
					m_nShopEquipWear[pTable->m_nWear] = m_CurrentItemList[m_nSelectIndex].m_nCode;
					ReloadCharacter();
				}
				else
				{
					if( m_CurrentItemList[m_nSelectIndex].m_nCode != m_nShopEquipWear[pTable->m_nWear] )
						FUNCTION_ExampleExchangeItem(m_CurrentItemList[m_nSelectIndex].m_nCode, pTable->m_nWear);
				}
			}
		}
	}
	
	FUNCTION_3DRotation();
}

void CItemShopProcess::FUNCTION_Selection()
{
	int nListCount = 0;

	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST6_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pListBG->FindEventControl(sText);
		assert(pControl);

		if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )
		{
			m_bLookChange = true;
			m_nSelectIndex = i;
		}
	}
}

void CItemShopProcess::FUNCTION_ExampleExchangeItem(int nCode, int nWear, bool bAlways )
{
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Slot%02d", nWear);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
	if( pControl == NULL )	return;

	CUIControl* pImg = pControl->FindEventControl( "List_Img" );
	assert(pImg);

	if( nCode == 0 )
	{
		UIFunction::Set_UIVisibility( pControl, "List_Img", N3UI_VISIBLE_FALSE );
		return;
	}

	//가라입기
	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, nCode, m_nSkin));
    assert(pTable);

    if( nWear != -1 )
        if( pTable->m_nWear != nWear ) return;
		
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

    if( m_nShopEquipWear[nWear] == nCode && bAlways == false) return;
	
	CResItemFormat * pEquipWear = NULL;
    CResItemFormat * pOrgWear	= NULL; 

    pEquipWear = dynamic_cast<CResItemFormat*>( m_pEquipWearModelID[ nWear ] );
    pOrgWear   = dynamic_cast<CResItemFormat*>( m_pObjPlayerInfo->m_pEquipWearModelID[ nWear ] );

    m_nShopEquipWear[ nWear ]    = nCode;
    
    if( pEquipWear != pOrgWear ) 
    {
        CObjectModelManager::GetPtr()->DeleteModelObject( pEquipWear );
        m_pEquipWearModelID[ nWear ] = 0;
    }

    m_pEquipWearModelID[ nWear ] = CObjectModelManager::GetPtr()->ItemLoadStickCharacter( nCode, m_pObjPlayerInfo->m_pModel, m_nSkin );

	RemoveItemDivestS( m_pObjPlayerInfo->m_pModel->m_spNode, m_nShopEquipWear );

	for(int i=0;i<=ITEM_EQUIP_KNEE;i++)
	{
		pEquipWear = dynamic_cast<CResItemFormat*>( m_pEquipWearModelID[ i ] );
		pOrgWear   = dynamic_cast<CResItemFormat*>( m_pObjPlayerInfo->m_pEquipWearModelID[ i ] );
		
		if( pEquipWear == NULL || pOrgWear == NULL ) continue;
		if( pEquipWear != pOrgWear ) 
			pOrgWear->m_spNode->SetAppCulled( true );	
	}	

    NiNode * pkModelNode = m_pObjPlayerInfo->m_pModel->m_spNode;
    assert( pkModelNode != 0 );

    CoreApi_RecursiveSpecularEnable( pkModelNode, true );
    CoreApi_RecursivePrepack( pkModelNode );
}

void CItemShopProcess::FUNCTION_InitFraction()
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

		if( pTable->GetObjCode()/1000 == ITEMSHOP_FRACTION)
		{
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
			m_vSmallFractionTypeList[pTable->m_nBigFractionIndex].push_back(pTable->m_nTypeCode);
		}
	}
}

void CItemShopProcess::FUNCTION_BigFraction(int nBigFraction)
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
}

void CItemShopProcess::FUNCTION_SmallFractionInit(int nBigFraction)
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

	FUNCTION_BigFractionInitShop(nBigFraction);
}

void CItemShopProcess::FUNCTION_SmallFraction(int nSmallFraction)
{
	m_nSmallFractionIndex = nSmallFraction;

	InitItemCode( m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+nSmallFraction) );

	m_nCurrentPage = 1;

	UpdateItemCode(m_nCurrentPage);

	FUNCTION_UpdatePage(m_nCurrentPage);

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
}

void CItemShopProcess::FUNCTION_GoPage(int nPage)
{
	m_nCurrentPage += nPage;						// 현재 페이지 설정
	
	FUNCTION_UpdatePage(m_nCurrentPage);

	if( m_nBigFractionIndex == 2 )
		InitCardList();

	UpdateItemCode(m_nCurrentPage);
}

short CItemShopProcess::FUNCTION_UpdatePage(short nPage)
{
	char text[STRLEN_64];

	if(nPage <= 0)									// 페이지가 미만, 초과일 경우
		m_nCurrentPage = nPage = m_nTotalPage;
	else if( m_nTotalPage < nPage )
		m_nCurrentPage = 1;

	_snprintf(text, STRLEN_64, "%d/%d", m_nCurrentPage, m_nTotalPage);
	UIFunction::Set_ControlText( "ItemPageName", text );

	return nPage;
}

/////////////////////////////////////////////////////////////////////////
//UI관련 함수
void CItemShopProcess::FUNCTION_ItemRender( int idx )
{
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Slot_Big");
	CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pListBG);

	_snprintf(sText, STRLEN_64, "ListBG%d", idx);
	CUIControl* pList = pListBG->FindEventControl(sText);

	if(m_CurrentItemList[idx].m_nCode > 0)
	{
		pList->SetVisible(N3UI_VISIBLE_TRUE);

		CUIControl* pItemName = pList->FindEventControl("ListName");//리스트 이름
		assert(pItemName);
				
		if(strlen(m_CurrentItemList[idx].m_pTable->m_sName)>16)
		{
			char* p;
			_snprintf(sText, STRLEN_64, "%s", m_CurrentItemList[idx].m_pTable->m_sName );
			p = &sText[27];
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
			if(pItemName) pItemName->SetText(m_CurrentItemList[idx].m_pTable->m_sName);		

		if( m_nBigFractionIndex == 0 )
		{
			switch(m_CurrentItemList[idx].m_nGrade)
			{
			case 1:	UIFunction::Set_Color(pList, "ListName", 0xffcacaca);	break;
			case 2:	UIFunction::Set_Color(pList, "ListName", 0xffb3fe00);	break;
			case 3:	UIFunction::Set_Color(pList, "ListName", 0xff0496f4);	break;
			case 4:	UIFunction::Set_Color(pList, "ListName", 0xffff56ed);	break;
			case 5:	UIFunction::Set_Color(pList, "ListName", 0xffffb400);	break;
			}
		}
		else
			UIFunction::Set_Color(pList, "ListName", 0xffffffff);

		CUIControl* pItemImg = pList->FindEventControl("ListImg");			//리스트 이미지
		assert(pItemImg);

		CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_CurrentItemList[idx].m_nCode, m_nSkin));
		assert(pTable);

		if(m_pObjPlayerInfo->m_cShape.m_nGender == PLAYER_GENDER_WOMAN)
			_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", pTable->m_sIconFile20);
		else
			_snprintf(sText, STRLEN_64, "Icon/Item/%s.png", pTable->m_sIconFile);

		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pItemImg, sText);

		switch( m_nBigFractionIndex )
		{
		case 0:
			{
				_snprintf(sText, STRLEN_64, "%d", m_CurrentItemList[idx].m_nPrice);
				UIFunction::Set_ChildControlText( pList, "ListPoint", sText );
			}
			break;
		case 1:
			{
				switch(m_CurrentItemList[idx].m_pTable->m_nSell)
				{
				case BUY_KIND_POINT:
					{
						int nPoint = m_CurrentItemList[idx].m_pTable->m_nPoint*((float)m_CurrentItemList[idx].m_pTable->m_nExchange/100);
						_snprintf(sText, STRLEN_64, "%d", nPoint);
						UIFunction::Set_ChildControlText( pList, "ListPoint", sText );
					}
					break;
				case BUY_KIND_CASH:
					{
						int nCoin = m_CurrentItemList[idx].m_pTable->m_nCash*((float)m_CurrentItemList[idx].m_pTable->m_nExchange/100);
						_snprintf(sText, STRLEN_64, "%d", nCoin);
						UIFunction::Set_ChildControlText( pList, "ListCoin", sText );
					}
					break;
				case BUY_KIND_BOTH:
					{
						int nPoint = m_CurrentItemList[idx].m_pTable->m_nPoint*((float)m_CurrentItemList[idx].m_pTable->m_nExchange/100);
						_snprintf(sText, STRLEN_64, "%d", nPoint);
						UIFunction::Set_ChildControlText( pList, "ListPoint", sText );
						int nCoin = m_CurrentItemList[idx].m_pTable->m_nCash*((float)m_CurrentItemList[idx].m_pTable->m_nExchange/100);
						_snprintf(sText, STRLEN_64, "%d", nCoin);
						UIFunction::Set_ChildControlText( pList, "ListCoin", sText );
					}
					break;
				}
			}
			break;
		case 2:
		case 3:
		case 4:
			{
				_snprintf(sText, STRLEN_64, "%d", m_CurrentItemList[idx].m_pTable->m_nCash);
				UIFunction::Set_ChildControlText( pList, "ListCoin", sText );
				_snprintf(sText, STRLEN_64, "%d", m_CurrentItemList[idx].m_pTable->m_nPoint);
				UIFunction::Set_ChildControlText( pList, "ListPoint", sText );
			}
			break;
		}
	}
	else pList->SetVisible(N3UI_VISIBLE_FALSE);
}

void CItemShopProcess::FUNCTION_3DRotation()
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

void CItemShopProcess::InitItemCode(int nType)
{
	m_ItemTotalCount = 0;

	CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	ZeroMemory(m_CurrentItemList, sizeof(m_CurrentItemList));
	ZeroMemory(m_ItemList, sizeof(m_ItemList));

    switch( m_nBigFractionIndex )
	{
	case 0:							// 오늘의 상품
		{
			for(int i=0;i<(int)pPlayer->m_vRandomItemList.size();i++)
			{
				if( pPlayer->m_vRandomItemList.at(i).m_nCode == 0 )	continue;

				CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pPlayer->m_vRandomItemList.at(i).m_nCode, 0));
				assert(pTable);

				if( ITEM_TYPE_FOOD <= pTable->m_nType )
					continue;

				if( nType != ITEM_TYPE_ALL ) 
					if( nType != pTable->m_nType )
						continue;

				m_ItemList[m_ItemTotalCount].m_nCode		= pPlayer->m_vRandomItemList.at(i).m_nCode;
				m_ItemList[m_ItemTotalCount].m_nPrice		= pPlayer->m_vRandomItemList.at(i).m_nPrice;
				m_ItemList[m_ItemTotalCount].m_nGrade		= pPlayer->m_vRandomItemList.at(i).m_nGrade;
				m_ItemList[m_ItemTotalCount].m_nItemSeq		= pPlayer->m_vRandomItemList.at(i).m_nItemSeq;
				m_ItemList[m_ItemTotalCount].m_nLevel		= pPlayer->m_vRandomItemList.at(i).m_nLevel;
				m_ItemList[m_ItemTotalCount].m_nEquipKind	= pPlayer->m_vRandomItemList.at(i).m_nEquipKind;
				m_ItemList[m_ItemTotalCount].m_nClass		= pPlayer->m_vRandomItemList.at(i).m_nClass;
				m_ItemList[m_ItemTotalCount].m_nAmount		= pPlayer->m_vRandomItemList.at(i).m_nAmount;
				m_ItemList[m_ItemTotalCount].m_pTable		= pTable;
				memcpy( m_ItemList[m_ItemTotalCount++].m_nOptionCode, pPlayer->m_vRandomItemList.at(i).m_nOptionCode, sizeof(int)*LIST5_SIZE );
			}
		}
		break;
	case 1:							// 디자인 샵
		{
			CResItemTableStack *temp = dynamic_cast<CResItemTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_ITEMTABLE));	
			assert(temp);

			for(int i=0;i<temp->GetResourceSize();i++)
			{
				CResItemTableFormat *pItemTable = dynamic_cast<CResItemTableFormat*>(temp->GetSequenceField(i));
				assert(pItemTable);

				int rem_now = pItemTable->GetObjCode()%10;
				int rem_before = m_ItemList[m_ItemTotalCount-1].m_nCode%10;

				if( (pItemTable->m_nOrder < 100) || ((pItemTable->m_nGender != 0) &&	// 공짜 아이템 이거나 해당 캐릭의 성별과  맞지 않으면
					(pItemTable->m_nGender != m_pObjPlayerInfo->m_cShape.m_nGender)) )
					continue;

				if( ITEM_TYPE_FOOD <= pItemTable->m_nType )
					continue;

				if( nType != ITEM_TYPE_ALL ) 
					if( nType != pItemTable->m_nType )
						continue;

				if( (m_ItemList[m_ItemTotalCount-1].m_nCode - rem_before) == (pItemTable->GetObjCode() - rem_now) && ( pItemTable->m_nType != ITEM_TYPE_FACE && i != 0 ) )
				{
					if( ((m_ItemList[m_ItemTotalCount-1].m_nCode - rem_before) == (pItemTable->GetObjCode() - rem_now)) )
						m_ItemList[m_ItemTotalCount-1].m_nAmount++;
				}
				else
				{
					m_ItemList[m_ItemTotalCount].m_nAmount++;
					m_ItemList[m_ItemTotalCount].m_nCode		= pItemTable->GetObjCode();
					m_ItemList[m_ItemTotalCount].m_nPrice		= 0;
					m_ItemList[m_ItemTotalCount].m_nGrade		= 0;
					m_ItemList[m_ItemTotalCount].m_nItemSeq		= 0;
					m_ItemList[m_ItemTotalCount].m_nLevel		= pItemTable->m_nLimit;
					m_ItemList[m_ItemTotalCount].m_nEquipKind	= 0;
					m_ItemList[m_ItemTotalCount].m_nClass		= 0;
					m_ItemList[m_ItemTotalCount++].m_pTable		= pItemTable;
				}
			}
		}
		break;
	case 2:							// 카드
		{
			InitCardList();
		}
		return;
	case 3:							// 클럽
		{
		}
		break;
	case 4:							// 특수
		{
			CResItemTableStack *temp = dynamic_cast<CResItemTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_ITEMTABLE));	
			assert(temp);

			for(int i=0;i<temp->GetResourceSize();i++)
			{
				CResItemTableFormat *pItemTable = dynamic_cast<CResItemTableFormat*>(temp->GetSequenceField(i));
				assert(pItemTable);

				int rem_now = pItemTable->GetObjCode()%10;
				int rem_before = m_ItemList[m_ItemTotalCount-1].m_nCode%10;

				if( pItemTable->m_nType < ITEM_TYPE_FOOD  && ITEM_TYPE_CARD <= pItemTable->m_nType )
					continue;

				if( nType != ITEM_TYPE_ALL ) 
				{
					if( nType/100 != pItemTable->m_nType/100 )
						continue;
				}
				else
				{
					if( pItemTable->m_nType < ITEM_TYPE_FOOD || ITEM_TYPE_CARD <= pItemTable->m_nType )
						continue;
				}

				m_ItemList[m_ItemTotalCount].m_nCode		= pItemTable->GetObjCode();
				m_ItemList[m_ItemTotalCount].m_nPrice		= 0;
				m_ItemList[m_ItemTotalCount].m_nGrade		= 0;
				m_ItemList[m_ItemTotalCount].m_nItemSeq		= 0;
				m_ItemList[m_ItemTotalCount].m_nLevel		= pItemTable->m_nLimit;
				m_ItemList[m_ItemTotalCount].m_nEquipKind	= 0;
				m_ItemList[m_ItemTotalCount].m_nClass		= 0;
				m_ItemList[m_ItemTotalCount++].m_pTable		= pItemTable;
			}
		}
		break;
	}

	m_nTotalPage = m_ItemTotalCount/LIST6_SIZE;				// 총 페이지 수 구하기

	if((m_ItemTotalCount%LIST6_SIZE)!=0)						// 나머지가 없으면
		m_nTotalPage++;
}

void CItemShopProcess::UpdateItemCode(int nPage)
{
	ZeroMemory(m_CurrentItemList, sizeof(m_CurrentItemList));

	 switch( m_nBigFractionIndex )
	{
	case 0:							// 오늘의 상품
	case 1:							// 디자인 샵
	case 3:							// 클럽
	case 4:							// 특수
		{
			for(int i=0;i<LIST6_SIZE;i++)
			{
				int nPos = ((nPage-1) * LIST6_SIZE) + i;

				m_CurrentItemList[i].m_nCode		= m_ItemList[nPos].m_nCode;
				m_CurrentItemList[i].m_nPrice		= m_ItemList[nPos].m_nPrice;
				m_CurrentItemList[i].m_nGrade		= m_ItemList[nPos].m_nGrade;
				m_CurrentItemList[i].m_nItemSeq		= m_ItemList[nPos].m_nItemSeq;
				m_CurrentItemList[i].m_nLevel		= m_ItemList[nPos].m_nLevel;
				m_CurrentItemList[i].m_nEquipKind	= m_ItemList[nPos].m_nEquipKind;
				m_CurrentItemList[i].m_nClass		= m_ItemList[nPos].m_nClass;
				m_CurrentItemList[i].m_nAmount		= m_ItemList[nPos].m_nAmount;
				m_CurrentItemList[i].m_pTable		= m_ItemList[nPos].m_pTable;
				memcpy( m_CurrentItemList[i].m_nOptionCode, m_ItemList[nPos].m_nOptionCode, sizeof(int)*LIST5_SIZE );

				if(m_CurrentItemList[i].m_nCode == 0)
					FUNCTION_ItemRender(i);
				else 
					FUNCTION_ItemRender(i);
			}
		}
		break;
	case 2:							// 카드
		{
			UpdateCardList();
		}
		break;
	}
}

void CItemShopProcess::InitCardList()
{
	for(int i=0;i<LIST512_SIZE;i++)
		m_cCardList[i].SetObjCode(0);

	CResAiCardTableStack *temp = dynamic_cast<CResAiCardTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_AICARDTABLE));
	assert(temp);

	int nCount = 0;

	for( int i = 0 ;i < temp->GetResourceSize(); i++ )
	{
		CResAiCardTableFormat *pAiCardTable = dynamic_cast<CResAiCardTableFormat*>(temp->GetSequenceField(i));
		assert(pAiCardTable);

		int nType = m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+m_nSmallFractionIndex);

		if( nType == ITEM_TYPE_CARD )
			m_cCardList[nCount++] = *pAiCardTable;
		else
		{
			if( pAiCardTable->m_nType == nType )
				m_cCardList[nCount++] = *pAiCardTable;
			else
				continue;
		}
	}

	m_nTotalPage = nCount/LIST6_SIZE;

    if( nCount%LIST6_SIZE )
		m_nTotalPage++;
}

void CItemShopProcess::UpdateCardList()
{
	int nPageNum = LIST6_SIZE*(m_nCurrentPage-1);

	for(int i = 0; i < LIST6_SIZE; i++)
	{
		char sText[STRLEN_64]="";

		_snprintf(sText, STRLEN_64, "Slot_Big");
		CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pListBG->FindEventControl(sText);
		assert(pControl);

		if( m_cCardList[nPageNum+i].GetObjCode() == 0 )
		{
			UIFunction::SetChildToVisibility( pControl, "ListPoint", N3UI_VISIBLE_FALSE );
			UIFunction::SetChildToVisibility( pControl, "PointIcon", N3UI_VISIBLE_FALSE );
			UIFunction::SetChildToVisibility( pControl, "ListCoin", N3UI_VISIBLE_FALSE );
			UIFunction::SetChildToVisibility( pControl, "CoinIcon", N3UI_VISIBLE_FALSE );
			pControl->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}
		else
		{
			UIFunction::SetChildToVisibility( pControl, "ListPoint", N3UI_VISIBLE_TRUE );
			UIFunction::SetChildToVisibility( pControl, "PointIcon", N3UI_VISIBLE_TRUE );
			UIFunction::SetChildToVisibility( pControl, "ListCoin", N3UI_VISIBLE_TRUE );
			UIFunction::SetChildToVisibility( pControl, "CoinIcon", N3UI_VISIBLE_TRUE );
			pControl->SetVisible(N3UI_VISIBLE_TRUE);
		}

		UIFunction::Set_ChildControlText( pControl, "ListName", m_cCardList[nPageNum+i].m_sName );

		UIFunction::Set_Color( pControl, "ListName", 0xffffffff);

		_snprintf( sText, STRLEN_64, "%d", m_cCardList[nPageNum+i].m_nPoint );
		UIFunction::Set_ChildControlText( pControl, "ListPoint", sText );
		_snprintf( sText, STRLEN_64, "%d", m_cCardList[nPageNum+i].m_nCash );
		UIFunction::Set_ChildControlText( pControl, "ListCoin", sText );

		switch( m_cCardList[nPageNum+i].m_nType )
		{
		case 10:
			UIFunction::Set_ChildControlTexture( pControl, "ListImg", "./Icon/Item/Card_BoosterPack_01.png" );
			break;
		case 20:
			UIFunction::Set_ChildControlTexture( pControl, "ListImg", "./Icon/Item/Card_BoosterPack_02.png" );
			break;
		case 30:
			UIFunction::Set_ChildControlTexture( pControl, "ListImg", "./Icon/Item/Card_BoosterPack_03.png" );
			break;
		}
	}
}

void CItemShopProcess::FUNCTION_ColorNext(int idx)
{
	m_CurrentItemList[idx].m_nCode++;
	
	CResItemTableFormat *pItemTable = NULL;
	
	if( m_CurrentItemList[idx].m_nAmount < (m_CurrentItemList[idx].m_nCode%10) )
		m_CurrentItemList[idx].m_nCode -= m_CurrentItemList[idx].m_nAmount;

	FUNCTION_ItemRender( idx );
}

void CItemShopProcess::FUNCTION_ColorPrev(int idx)
{
    m_CurrentItemList[idx].m_nCode--;

	CResItemTableFormat *pItemTable = NULL;

	if( (m_CurrentItemList[idx].m_nCode%10) < 1 )
		m_CurrentItemList[idx].m_nCode += m_CurrentItemList[idx].m_nAmount;

	FUNCTION_ItemRender( idx );
}

void CItemShopProcess::FUNCTION_DivestItem(int idx)
{
    assert( idx >= 0 && idx < MAX_EQUIP );
    if( idx < 0 || idx >= MAX_EQUIP) return;

    CResItemFormat * pEquipWear = dynamic_cast<CResItemFormat*>(m_pEquipWearModelID[idx]);
    CResItemFormat * pOrgWear   = dynamic_cast<CResItemFormat*>(m_pObjPlayerInfo->m_pEquipWearModelID[idx]);

    if( pEquipWear == pOrgWear )
	{
		FUNCTION_InitShop();
		return;
	}

    bool bAlwaysExchange = (m_nSkin != m_pObjPlayerInfo->m_cShape.m_nSkin);
    if( pOrgWear != 0 && bAlwaysExchange == false ) 
    {
        pOrgWear->m_spNode->SetAppCulled( false );
    }

    CObjectModelManager::GetPtr()->DeleteModelObject( pEquipWear );
    
	m_nShopEquipWear[idx]    = m_pObjPlayerInfo->m_nEquipWear[idx];
	m_pEquipWearModelID[idx] = pOrgWear;

	FUNCTION_ExampleExchangeItem(m_nShopEquipWear[idx], idx, bAlwaysExchange );

	if(idx == 0)
	{
		m_nSkin = m_pObjPlayerInfo->m_cShape.m_nSkin;
		ReloadCharacter();
	}

    RemoveItemDivestS( m_pObjPlayerInfo->m_pModel->m_spNode, m_nShopEquipWear );

	for(int i=0;i<=ITEM_EQUIP_KNEE;i++)
	{
		pEquipWear = dynamic_cast<CResItemFormat*>( m_pEquipWearModelID[ i ] );
		pOrgWear   = dynamic_cast<CResItemFormat*>( m_pObjPlayerInfo->m_pEquipWearModelID[ i ] );
		
		if( pEquipWear == NULL || pOrgWear == NULL ) continue;

		if( pEquipWear != pOrgWear ) 
			pOrgWear->m_spNode->SetAppCulled( true );	
	}	

	FUNCTION_InitShop();
}

void CItemShopProcess::FUNCTION_ItemShopToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	char sAllOption[MAX_PATH]="";

	RECT rt;
	
	int nCount = LIST6_SIZE;

	CUIControl* pBackPannel = CClientUIManager::GetPtr()->FindEventControl("Slot_Big");
	assert(pBackPannel);

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

	// 임시
	CResItemTableFormat *pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, m_CurrentItemList[i].m_nCode, 0));
	if(pTable == NULL) return;

	if( m_CurrentItemList[i].m_nLevel == 0 )
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  CResourceManager::GetPtr()->m_pStaticMap[210020].c_str() );
	else
	{
		_snprintf( sText, MAX_PATH, "LEVEL %d", m_CurrentItemList[i].m_nLevel );
		UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  sText );
	}

	UIFunction::Set_ChildControlText( pToolTip, "ItemName", pTable->m_sName );

	// 등급에 따른 네임색 변경
	switch(m_CurrentItemList[i].m_nGrade)
	{
	case 1:		UIFunction::Set_Color(pToolTip, "ItemName", 0xffcacaca);	break;
	case 2:		UIFunction::Set_Color(pToolTip, "ItemName", 0xffb3fe00);	break;
	case 3:		UIFunction::Set_Color(pToolTip, "ItemName", 0xff0496f4);	break;
	case 4:		UIFunction::Set_Color(pToolTip, "ItemName", 0xffff56ed);	break;
	case 5:		UIFunction::Set_Color(pToolTip, "ItemName", 0xffffb400);	break;
	default:	UIFunction::Set_Color(pToolTip, "ItemName", 0xffffe4e1);
	}

	CUIControl* pEnchant = pToolTip->FindEventControl( "Enchant" );
	assert(pEnchant);

	int nLine		= 0;
	int nLen		= strlen(pTable->m_sName);;

	for( int j=0;j<LIST5_SIZE;j++)
	{
		_snprintf( sText, MAX_PATH, "Enchant_%d", j );
		CUIControl* pEOn = pEnchant->FindEventControl( sText );
		
		if( j < m_CurrentItemList[i].m_nGrade )
			pEOn->SetVisible(N3UI_VISIBLE_TRUE);
		else
		{
			pEOn->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}
		
		if( m_CurrentItemList[i].m_nOptionCode[j] != 0 )
			UIFunction::ChangeTexture( pEOn, "EnchantOn" );
		else
		{
			UIFunction::ChangeTexture( pEOn, "EnchantOff" );
			continue;
		}

		if( 0 < j )	strcat(sAllOption, "\n");

		int nSituationCode	= ITEM_TYPE_SITUATION(m_CurrentItemList[i].m_nOptionCode[j]);
		int nFacultyCode	= ITEM_TYPE_FACULTY(m_CurrentItemList[i].m_nOptionCode[j]);
		int nFaculty		= ITEM_TYPE_VALUE(m_CurrentItemList[i].m_nOptionCode[j]);

		char sOption[STRLEN_64]="";

		_snprintf(sOption, STRLEN_64, "%s %s + %d", UICommonMethod::SituationCodeToStr(nSituationCode), UICommonMethod::OptionCodeToStr(nFacultyCode), nFaculty);

		strcat(sAllOption, sOption);
		nLine++;
		
		if(nLen<(int)strlen(sOption))
			nLen = strlen(sOption);
	}

	if( 0 < m_CurrentItemList[i].m_nGrade )
		nLine+=2;

	if( 90+(nLen*5) < 150 )
		pToolTip->SetSize(150, 70+(16*nLine), true);
	else
		pToolTip->SetSize(90+(nLen*5), 70+(16*nLine), true);

	pToolTip->SetLocation(m_pDNDPoint.x-(pToolTip->GetWidth()/2), m_pDNDPoint.y+5, true);

	UIFunction::Set_ChildControlText( pToolTip, "description", sAllOption );	
}

void CItemShopProcess::FUNCTION_ItemShopEquipWearToolTipViewUpdate()
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

	UIFunction::Set_Color( pToolTip, "ItemName", 0xffffe4e1);

	int nGrade = 0;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	for(;j<(int)m_pObjPlayerInfo->m_vItemList.size();j++)
	{
		if( m_pObjPlayerInfo->m_nEquipWear[nWear] == m_pObjPlayerInfo->m_vItemList.at(j)->m_nCode 
			&& m_pObjPlayerInfo->m_vItemList.at(j)->m_nEquipKind == EQUIP_KIND_YES )
		{
			_snprintf( sText, MAX_PATH, "LEVEL %d", m_pObjPlayerInfo->m_vItemList.at(j)->m_nLevel );
			UIFunction::Set_ChildControlText( pToolTip, "ItemLevel",  sText );
			// 등급에 따른 네임색 변경
			switch(m_pObjPlayerInfo->m_vItemList.at(j)->m_nGrade)
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

				nGrade = m_pObjPlayerInfo->m_vItemList.at(j)->m_nGrade;

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

void CItemShopProcess::InitItemList()
{
	InitItemCode(m_vSmallFractionTypeList[m_nBigFractionIndex].at(m_nSmallFractionStart+m_nSmallFractionIndex));
	UpdateItemCode(1);
	m_nCurrentPage = 1;
	FUNCTION_GoPage(0);
}

void CItemShopProcess::FUNCTION_BuyItem(char idx)
{
	if( m_nBigFractionIndex == 2 )
	{
		CShopDialog::GetPtr()->FUNCTION_InitShopDialog(
			&m_cCardList[idx],
			m_nBigFractionIndex,
			idx, 
			BUYITEM_KIND, 
			m_nCurrentPage
		);
	}
	else
	{
		CShopDialog::GetPtr()->FUNCTION_InitShopDialog(
			&m_CurrentItemList[idx],
			m_nBigFractionIndex,
			idx, 
			BUYITEM_KIND, 
			m_nCurrentPage
		);
	}
}

void CItemShopProcess::FUNCTION_ExchangeItem(char idx)
{
	CShopDialog::GetPtr()->FUNCTION_InitShopDialog(
		&m_CurrentItemList[idx],
		m_nBigFractionIndex,
		idx, 
		EXCHANGEITEM_KIND, 
		m_nCurrentPage
	);
}

void CItemShopProcess::FUNCTION_SetCardBooster(CCardInfo* pInfo)
{
	m_cCardInfo.m_nCardSeq		= pInfo->m_nCardSeq;
	m_cCardInfo.m_nCostume		= pInfo->m_nCostume;
	m_cCardInfo.m_nEnchant		= pInfo->m_nEnchant;
	memcpy(m_cCardInfo.m_nEquipKind, pInfo->m_nEquipKind, sizeof(int)*3);
	m_cCardInfo.m_nLevel		= pInfo->m_nLevel;
	m_cCardInfo.m_nPlayerSeq	= pInfo->m_nPlayerSeq;
	m_cCardInfo.m_nPosition		= pInfo->m_nPosition;
	m_cCardInfo.m_nRank			= pInfo->m_nRank;
	m_cCardInfo.m_nSkill		= pInfo->m_nSkill;
	m_cCardInfo.m_nState		= pInfo->m_nState;
	m_cCardInfo.m_nTierd		= pInfo->m_nTierd;
	m_cCardInfo.m_nType			= pInfo->m_nType;

	m_bCardAni = true;
	m_nCurrentTime = timeGetTime();
}

void CItemShopProcess::FUNCTION_CardBoosterUpdate()
{
	char sText[STRLEN_64]="";

	CUIControl* pCardDialogBG = CClientUIManager::GetPtr()->FindEventControl( "CardBooster_Dialog" );
	assert(pCardDialogBG);
	
	pCardDialogBG->SetVisible( N3UI_VISIBLE_TRUE );
	pCardDialogBG->Set_FocusedPage(pCardDialogBG);
	CUIControl::RequestFocus(pCardDialogBG);

	CUIControl* pBoosterCard = CClientUIManager::GetPtr()->FindEventControl( "BoosterCardImg" );
	assert(pBoosterCard);

	if((timeGetTime()-m_nCurrentTime)<2000)
	{
		UIFunction::Set_UIVisibility( "SelectCard", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "CardAni", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "ConfirmBtn", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Msg", N3UI_VISIBLE_FALSE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "SelectCard", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "CardAni", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "ConfirmBtn", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Msg", N3UI_VISIBLE_TRUE );

		UIFunction::Set_ControlText( "Msg", CResourceManager::GetPtr()->m_pStaticMap[ 170008+m_cCardInfo.m_nRank ].c_str() );

		UICommonMethod::FUNCTION_CardInfoDraw( pBoosterCard, m_cCardInfo.m_nPosition, m_cCardInfo.m_nRank, m_cCardInfo.m_nLevel, m_cCardInfo.m_nCostume);

		m_bCardAni = false;
	}
}