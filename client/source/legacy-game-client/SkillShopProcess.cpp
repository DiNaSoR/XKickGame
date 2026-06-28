#include "stdafx.h"
#include "SkillShopProcess.h"
#include "UICommonMethod.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "PlayModeManager.h"
#include "CoreApi.h"
#include "ResTableStack.h"
#include "SignBoard.h"
#include "UIFunction.h"
#include "UICommon.h"

CSkillShopProcess::CSkillShopProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_SKILLSHOP;

	m_nBigFractionIndex		= -1;//대분류 번호
	m_nSmallFractionIndex	= -1;//소분류 번호
	m_nSmallFractionStart	= 0;

	m_nCurrentPage = 1;
	m_nTotalPage = 1;

    m_bSkillView = false;
    m_bBeforeStateMachineUpdate = true;

    m_nDetailTabIndex		= 0;// Detail Tab
    m_nSelectIndex			= 0;

	m_nSelectArea = 0;
}

CSkillShopProcess::~CSkillShopProcess(void)
{    
}

void CSkillShopProcess::Terminate()
{
	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CSkillShopProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CSkillShopProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_pSelectPlayerInfo = pObjPlayerInfo->Clone();

	CObjPlayerManager::GetPtr()->SetPlayerLists(m_pSelectPlayerInfo, m_pSelectPlayerInfo->m_nTeam);

	m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	m_pRoom->InitRoom();
	m_pRoom->SetParentSeq(m_pSelectPlayerInfo->GetObjSeq());

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "Detail_BG", "SkillShopBG" );

	FUNCTION_InitFraction();
	FUNCTION_BigFraction(0);
	FUNCTION_DetailTabChange(0);

	LoadGround();
	LoadCharacter();

	FUNCTION_3DRender( "SkillViewArea" , pObjGroundInfo->GetObjectCamera());

	m_nState = STATE_START;
}

void CSkillShopProcess::Update(float fFrameTime, float fAccumTime)
{
    CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );

	FUNCTION_UpdateShop();
}

void CSkillShopProcess::RenderFrame()
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

void CSkillShopProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	Update(fFrameTime, fAccumTime);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime, true);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );
	{	
		CGameProject::s_pGameProject->BeginFrame();

		CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
		assert(pObjGroundInfo);
		pObjGroundInfo->m_pModel->BeforeRender();

		m_cCharacterTexBoard.Render();

		CGameProject::s_pGameProject->EndFrame();

		CGameProject::s_pGameProject->DisplayFrame();
	}

	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
	//UICommonMethod::BGEffectAnimationPlay( "Detail_BG", "SkillShopBG" );
	UICommonMethod::BasicInfoStop();
	UICommonMethod::DisplayAll(m_pSelectPlayerInfo);
	FUNCTION_DetailTabUpdate(m_nSelectIndex);

	if(m_spCeremonyCameraNode) m_spCeremonyCameraNode->Update(fAccumTime);
}

void CSkillShopProcess::UpdateState(float fFrameTime, float fAccumTime)
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
			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
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

bool CSkillShopProcess::OnLostDevice()
{
    m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CSkillShopProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
    m_cCharacterTexBoard.OnResetDevice();
    return true;
}

bool CSkillShopProcess::OnDefault(NiEventRef pEventRecord)
{  
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
	{
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	}

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CSkillShopProcess::GetSameProcess()
{
	return new CSkillShopProcess;
}

void CSkillShopProcess::FUNCTION_InitShop()
{
}

void CSkillShopProcess::FUNCTION_UpdateShop()
{
	int nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";
	
	for(int i = 0; i < nListCount; i++)
	{
		_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pListBG->FindEventControl(sText);
		assert(pControl);

		int nCode = m_nCode[i];

		UIFunction::SetChildToVisibility( pControl, "SoldImg", N3UI_VISIBLE_FALSE );

		switch( m_nBigFractionIndex )
		{
		case 0:
			{
				CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
				
				CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, nCode, 0));
				if( pSkillTable == NULL )	return;

				vector< CObjSkillInfo* >::iterator iter;
				for(iter = pObjPlayerInfo->m_vSkillList.begin();iter != pObjPlayerInfo->m_vSkillList.end(); iter++)
				{
					int n = (*iter)->m_pTable->m_nType;
					if(pSkillTable->m_nType == (*iter)->m_pTable->m_nType)
					{
						UIFunction::SetChildToVisibility( pControl, "SoldImg", N3UI_VISIBLE_TRUE );
						break;
					}
				}			
			}
			break;
		case 1:
			{
			}	
			break;
		case 2:
			{
				CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

				CResCeremonyTableFormat* pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, nCode, 0));
				if( pCeremonyTable == NULL )	return;

				vector< CObjCeremonyInfo* >::iterator iter;
				for(iter = pObjPlayerInfo->m_vCeremonyList.begin();iter != pObjPlayerInfo->m_vCeremonyList.end(); iter++)
				{
					if(nCode == (*iter)->m_nCode)
					{
						UIFunction::SetChildToVisibility( pControl, "SoldImg", N3UI_VISIBLE_TRUE );
						break;
					}
				}				
			}
			break;
		}

		if(nCode != 0)
		{
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
}


void CSkillShopProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		FUNCTION_Selection();
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		//FUNCTION_SkillShopToolTipViewUpdate(lParam);
		break;
	}
}

//void CSkillShopProcess::FUNCTION_SkillShopToolTipViewUpdate(LPARAM lParam)
//{
//	char sText[MAX_PATH]="";
//
//	CUIControl* pBackPannel = NULL;
//
//	POINT pt;
//	RECT rt;
//
//	pt.x = LOWORD(lParam);
//	pt.y = HIWORD(lParam);
//	
//	// SHOP_SkillBG0 ~ SHOP_SkillBG27
//	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("Tooltip");
//	assert(pToolTip);
//	pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
//	
//	int i=0;
//	for(;i<28;i++)
//	{
//		_snprintf(sText, MAX_PATH, "SHOP_SkillBG%d", i);
//		CUIControl* pOverImg = CClientUIManager::GetPtr()->FindEventControl(sText);
//		assert(pOverImg);
//
//		rt = pOverImg->GetWindowBox();
//		
//		if(UICommonMethod::IsRectInPoint(&rt, &pt) && pOverImg->GetVisible() == N3UI_VISIBLE_TRUE)
//		{
//			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
//			break;
//		}
//		else
//			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
//	}
//
//	if(28 == i) return;
//
//	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
//	assert(pObjPlayerInfo);
//
//	CResMessageTableFormat* pMTable = NULL;
//
//	switch(m_nBigFractionIndex%10)
//	{
//	case 0:
//		{
//			CResSkillTableFormat *pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, 
//				pObjPlayerInfo->m_vSkillList.at( i + ( ( m_cMySkillWindow.GetCurrentPage() - 1 ) * 28 ) )->m_nCode, 0));
//			if(pTable == NULL) return;
//
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900011 ,0));
//			assert(pMTable);
//
//			_snprintf(sText, MAX_PATH, pMTable->m_sMessage, pTable->m_sName);
//
//			int nLen = strlen(sText);	
//
//			char sUpgradeLv[STRLEN_32]="";
//
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900012 ,0));
//			assert(pMTable);
//
//			_snprintf(sUpgradeLv, STRLEN_32, pMTable->m_sMessage, pObjPlayerInfo->m_vSkillList.at( i + ( ( m_cMySkillWindow.GetCurrentPage() - 1 ) * 28 ) )->m_nLevel );
//			strcat(sText, sUpgradeLv);		
//
//			pToolTip->SetLocation(LOWORD(lParam) - ( 65 + ( nLen * 5 ) ), HIWORD(lParam) - 10, true);
//			pToolTip->SetSize(60+(nLen*5), 35+22);
//			pToolTip->SetText(sText);
//		}
//		break;
//	case 1:
//		{
//			CResTrainingTableFormat *pTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, 
//				pObjPlayerInfo->m_vTrainingList.at( i + ( ( m_cMySkillWindow.GetCurrentPage() - 1 ) * 28 ) )->m_nCode, 0));
//			if(pTable == NULL) return;
//
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900013 ,0));
//			assert(pMTable);
//
//			_snprintf(sText, MAX_PATH, pMTable->m_sMessage, pTable->m_sName);
//
//			int nLen = strlen(sText);	
//
//			char sUpgradeLv[STRLEN_32]="";
//
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900014 ,0));
//			assert(pMTable);
//
//			_snprintf(sUpgradeLv, STRLEN_32, pMTable->m_sMessage, pObjPlayerInfo->m_vTrainingList.at( i + ( ( m_cMySkillWindow.GetCurrentPage() - 1 ) * 28 ) )->m_nLevel );
//			strcat(sText, sUpgradeLv);		
//
//			pToolTip->SetLocation(LOWORD(lParam) - ( 65 + ( nLen * 5 ) ), HIWORD(lParam) - 10, true);
//			pToolTip->SetSize(60+(nLen*5), 35+22);
//			pToolTip->SetText(sText);
//		}
//		break;
//	case 2:
//		{
//			CResCeremonyTableFormat *pTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 
//				pObjPlayerInfo->m_vCeremonyList.at( i + ( ( m_cMySkillWindow.GetCurrentPage() - 1 ) * 28 ) )->m_nCode, 0));
//			if(pTable == NULL) return;
//
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900099 ,0));
//			assert(pMTable);
//
//			_snprintf(sText, MAX_PATH, pMTable->m_sMessage, pTable->m_sName);
//
//			int nLen = strlen(sText);	
//
//			pToolTip->SetLocation(LOWORD(lParam) - ( 65 + ( nLen * 5 ) ), HIWORD(lParam) - 10, true);
//			pToolTip->SetSize(60+(nLen*5), 35+22);
//			pToolTip->SetText(sText);
//		}
//		break;
//	}
//}

void CSkillShopProcess::FUNCTION_InitFraction()
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
        
		if( pTable->GetObjCode()/1000 == SKILLSHOP_FRACTION)
		{
			m_vFractionList[pTable->m_nBigFractionIndex].push_back(CResourceManager::GetPtr()->m_pStaticMap[ pTable->m_nTextCode ].c_str());
			m_vSmallFractionTypeList[pTable->m_nBigFractionIndex].push_back(pTable->m_nTypeCode);
		}
	}
}

void CSkillShopProcess::FUNCTION_BigFraction(int nBigFraction)
{
	m_nBigFractionIndex = nBigFraction;
	m_nSmallFractionIndex = 0;
	m_nSelectIndex = 0;
	m_nCurrentPage = 1;

	m_nDetailTabIndex = 0;

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

	FUNCTION_SmallFraction(0);
	FUNCTION_SmallFractionInit(nBigFraction);
}

void CSkillShopProcess::FUNCTION_SmallFractionInit(int nBigFraction)
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

void CSkillShopProcess::FUNCTION_SmallFraction(int nSmallFraction)
{
	m_nSmallFractionIndex = nSmallFraction;

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

	switch( m_nBigFractionIndex )
	{
	case 0:
		{
			if( nSmallFraction )
				InitSkillCode(0);
			else
				InitSkillCode(m_pSelectPlayerInfo->m_nPosition);
		}
		UIFunction::Set_UIVisibility( "SkillT", N3UI_VISIBLE_TRUE);
		UIFunction::Set_UIVisibility( "CeremonyT", N3UI_VISIBLE_FALSE);
		break;
	case 1:
        InitTrainingCode();//트레이닝 샵
		break;
	case 2:
		UIFunction::Set_UIVisibility( "SkillT", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility( "CeremonyT", N3UI_VISIBLE_TRUE);
		InitCeremonyCode();//세레모니 샵
		break;
	}

	FUNCTION_GoPage(m_nCurrentPage);
}

void CSkillShopProcess::FUNCTION_GoPage(int nPage)
{
	switch(m_nBigFractionIndex)
	{
	case 0:	UpdateSkillCode(nPage);//스킬 샵
		break;
	case 1:	UpdateTrainingCode(nPage);//트레이닝 샵
		break;
	case 2:	UpdateCeremonyCode(nPage);//세레모니 샵
		break;
	default:
		break;
	}

	FUNCTION_DetailTabChange(m_nDetailTabIndex);
	FUNCTION_PageUpdate(nPage);
}

void CSkillShopProcess::LoadCharacter()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);

			if(pAnyPlayer->m_nTeam == PLAYER_TEAM_VIEW)	continue;
			if(pAnyPlayer == NULL)						continue;
			if(pAnyPlayer->m_pModel)					continue;

			if(m_pSelectPlayerInfo->GetObjSeq() == m_pRoom->m_nParentSeq) CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, pAnyPlayer->GetKind());
			else CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, 0);

			CProcessManager::GetPtr()->AttachItem(pAnyPlayer); 	
		}
	}
	CObjectModelManager::GetPtr()->ShadowOnOff(true, true);
}

/////////////////////////////////////////////////////////////////////////
//UI관련 함수
void CSkillShopProcess::FUNCTION_ItemRender(int ItemIndex, int nSkillCode, int nPointPrice, int nCoinPrice, char *sName, char *sIconFile, int nSell)
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	int nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Slot_Big");

	CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
	assert(pListBG);

	_snprintf(sText, STRLEN_64, "ListBG%d", ItemIndex);
	CUIControl* pControl = pListBG->FindEventControl(sText);
	assert(pControl);

	if(nSkillCode > 0)
	{
		//해당 테이블 호출
		pControl->SetVisible(N3UI_VISIBLE_TRUE);

		UIFunction::SetChildToVisibility( pControl, "SkillViewBtn", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( pControl, "BuyBtn", N3UI_VISIBLE_TRUE );

		UIFunction::SetChildToVisibility( pControl, "Cash", N3UI_VISIBLE_TRUE );
		UIFunction::SetChildToVisibility( pControl, "Point", N3UI_VISIBLE_TRUE );
		
		if( m_nBigFractionIndex != 1 )
		{
			switch( m_nBigFractionIndex )
			{
			case 0:
				{
					CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, nSkillCode, 0));
					assert(pSkillTable);

					if( pSkillTable->m_nAniCode ) 
						UIFunction::SetChildToVisibility( pControl, "SkillViewBtn", N3UI_VISIBLE_TRUE );

					if( m_nSmallFractionIndex == 1 )
					{
						UIFunction::SetChildToVisibility( pControl, "BuyBtn", N3UI_VISIBLE_FALSE );
						UIFunction::SetChildToVisibility( pControl, "Cash", N3UI_VISIBLE_FALSE );
						UIFunction::SetChildToVisibility( pControl, "Point", N3UI_VISIBLE_FALSE );
						break;
					}
				}
				break;
			case 2:
				{
					UIFunction::SetChildToVisibility( pControl, "SkillViewBtn", N3UI_VISIBLE_TRUE );
				}
				break;
			}
		}
			
		if(strlen(sName)>16)
		{
			char* p;
			_snprintf(sText, STRLEN_64, "%s", sName );
			p = &sText[24];
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
			UIFunction::Set_ChildControlText( pControl, "ListName", sText );
		}
		else
			UIFunction::Set_ChildControlText( pControl, "ListName", sName );

		switch( m_nBigFractionIndex )
        {     
        case 0:	_snprintf(sText, STRLEN_64, "Icon/Skill/%s.png", sIconFile);	break;
		case 1:	_snprintf(sText, STRLEN_64, "Icon/Training/%s.png", sIconFile);	break;
		case 2:	_snprintf(sText, STRLEN_64, "Icon/Ceremony/%s.png", sIconFile);	break;
		}

		UIFunction::Set_ChildControlTexture( pControl, "ListImg", sText );

		if(nSell == BUY_KIND_POINT || nSell == BUY_KIND_BOTH)
		{
			CUIControl* pPoint = pControl->FindEventControl( "Point" );
			assert(pPoint);
			_snprintf( sText, STRLEN_64, "%d", nPointPrice );
			UIFunction::Set_ChildControlText( pPoint, "PointNum", sText );
			UIFunction::SetChildToVisibility( pControl, "Point", N3UI_VISIBLE_TRUE );
		}
		else
			UIFunction::SetChildToVisibility( pControl, "Point", N3UI_VISIBLE_FALSE );

		if(nSell == BUY_KIND_CASH || nSell == BUY_KIND_BOTH)
		{
			CUIControl* pCash = pControl->FindEventControl( "Cash" );
			assert(pCash);
			_snprintf( sText, STRLEN_64, "%d", nCoinPrice );
			UIFunction::Set_ChildControlText( pCash, "CoinNum", sText );
			UIFunction::SetChildToVisibility( pControl, "Cash", N3UI_VISIBLE_TRUE );
		}
		else
			UIFunction::SetChildToVisibility( pControl, "Cash", N3UI_VISIBLE_FALSE );
	}
	else pControl->SetVisible(N3UI_VISIBLE_FALSE);
}

void CSkillShopProcess::FUNCTION_3DRotation()
{
	CUIButton* pControl = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("WearItemButton"));
	assert(pControl);	
	if(pControl == NULL) return;
	if(pControl->GetRButtonClick())
	{
		//회전시킨다
		CResCharacterFormat* pCharacter = m_pSelectPlayerInfo->m_pModel;
		pCharacter->SetDirection(- pControl->m_OldPoint.x*0.01f - pControl->m_DeltaPoint.x*0.01f + 180.f);				
	}
}

void CSkillShopProcess::FUNCTION_View()
{
	FUNCTION_DetailTabChange(1);
	switch(m_nBigFractionIndex)
	{
	case 0:	
		{
			FUNCTION_SkillView();//스킬 샵
		}
		break;
	case 1:	_Assert(NULL, "트레이닝 샵은", 0, "뷰를 띠우면 안된다", 0);//트레이닝 샵
		break;
	case 2:	
		{
			FUNCTION_CeremonyView();//세레모니 샵
		}
		break;
	default:
		break;
	}
}

void CSkillShopProcess::FUNCTION_SkillView()
{
//    if( m_nSelectCode == -1 ) return;//test 
	VectorAnyoneList* pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			CObjPlayerInfo* pAnyPlayer = (*iPlayer);

			if(m_pSelectPlayerInfo->GetObjSeq() == pAnyPlayer->GetObjSeq()) continue;

			pAnyPlayer->m_cCurrentAction.m_nAniCode = STAND_CODE;
			pAnyPlayer->SetNodePoint(NiPoint3(1000,0,0));
			pAnyPlayer->m_cNodePoint = NiPoint3(1000,0,0);
		}
	}

    CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[m_nSelectIndex], 0));    
	assert(pSkillTable);	
	if(pSkillTable == NULL) return;

	UIFunction::Set_ControlText( "SkillName_TextT", pSkillTable->m_sName );

	m_pSelectPlayerInfo->m_cCurrentAction.m_fStartAngle = 180.f;
    m_pSelectPlayerInfo->m_pModel->SetTargetAnimation( pSkillTable->m_nAniCode, 1.f);

	m_bSkillView = true;

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_cCharacterTexBoard.SetCamera( pObjGroundInfo->GetObjectCamera() );
}

void CSkillShopProcess::FUNCTION_CeremonyView()
{
//    if( m_nSelectCode == -1 ) return;//test 
	VectorAnyoneList* pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);

	CResCeremonyTableFormat* pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, m_nCode[m_nSelectIndex],0));
	if(pCeremonyTable == NULL) return;

	UIFunction::Set_ControlText( "SkillName_TextT", pCeremonyTable->m_sName );

	int i = 1;
	int j = 0;

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			if((*iPlayer)->m_nTeam != PLAYER_TEAM_HOME && (*iPlayer)->m_nTeam != PLAYER_TEAM_AWAY)
				continue;

			CObjPlayerInfo* pAnyPlayer = (*iPlayer);

			pAnyPlayer->SetNodePoint(NiPoint3::ZERO);

			if(m_pSelectPlayerInfo->GetObjSeq() == pAnyPlayer->GetObjSeq())
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = pCeremonyTable->m_nWinner[0];
				pAnyPlayer->m_cCurrentAction.m_nActionSeq++;
			}
			else if(pAnyPlayer->m_nTeam == PLAYER_TEAM_HOME)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = pCeremonyTable->m_nWinner[i];
				pAnyPlayer->m_cCurrentAction.m_nActionSeq++;
				i++;
			}
			else
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = pCeremonyTable->m_nLoser[j];
				pAnyPlayer->m_cCurrentAction.m_nActionSeq++;
				j++;
			}
		}
	}

    m_bSkillView = true;

	LoadCameraType(pCeremonyTable->m_sCameraFile);

	m_cCharacterTexBoard.SetCamera( m_spCeremonyCamera );
}

void CSkillShopProcess::FUNCTION_Replay()
{
	switch(m_nBigFractionIndex)
	{
	case 0:	
		{
			CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[m_nSelectIndex], 0));
			if(pSkillTable == NULL)	return;
			if( m_nDetailTabIndex == 1 && pSkillTable->m_nAniCode )
				FUNCTION_SkillView();//스킬 샵
		}
		break;
	case 1:	_Assert(NULL, "트레이닝 샵은", 0, "뷰를 띠우면 안된다", 0);//트레이닝 샵
		break;
	case 2:	FUNCTION_CeremonyView();//세레모니 샵
		break;
	default:
		break;
	}
}

void CSkillShopProcess::InitSkillCode( char nPosition )
{
	int nTotalCount = 0;
	int nCode = 0;
	int nType = 0;

	ZeroMemory(m_nSkillCode, sizeof(m_nSkillCode));
	
	CResSkillTableStack *temp = dynamic_cast<CResSkillTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_SKILLTABLE));
	assert(temp);

	for(int i=0;i<temp->GetResourceSize();i++)
	{
		CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(temp->GetSequenceField(i));
		assert(pSkillTable);

		if( nPosition )
		{
			if( (nPosition/10)*10 != pSkillTable->m_nPosition && nPosition != pSkillTable->m_nPosition )
				continue;
		}
		else
		{
			if( pSkillTable->m_nPosition == nPosition )
				continue;
			
			int j=0;
			for(;j<nTotalCount;j++)
			{
				if( m_nSkillType[j] == pSkillTable->m_nType )
					break;
			}

			if(j<nTotalCount)
				continue;
		}

		if(!pSkillTable->m_nSell)
			continue;

		m_nSkillType[nTotalCount] = pSkillTable->m_nType;
		m_nSkillCode[nTotalCount++] = pSkillTable->GetObjCode();
	}

	m_nTotalPage = nTotalCount/LIST6_SIZE;
	if( (nTotalCount%LIST6_SIZE) != 0 )
		m_nTotalPage++;
}

void CSkillShopProcess::InitTrainingCode()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	int nTotalCount = 0;
	int nCode = 0;
	int nType = 0;

	ZeroMemory(m_nTrainingCode, sizeof(m_nTrainingCode));
	
	CResTrainingTableStack *temp = dynamic_cast<CResTrainingTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_TRAININGTABLE));
	assert(temp);
	for(int i=0;i<temp->GetResourceSize();i++)
	{
		CResTrainingTableFormat* pTrainingTable = dynamic_cast<CResTrainingTableFormat*>(temp->GetSequenceField(i));
		assert(pTrainingTable);

		if( (nCode !=0) && (nType == pTrainingTable->m_nType) )
		{
			if(nCode == pTrainingTable->GetObjCode())
			{
				if(!pTrainingTable->m_nSell)
					continue;
				m_nTrainingCode[nTotalCount++] = pTrainingTable->GetObjCode();
				nCode=0;nType=0;
				continue;
			}
			else
				continue;
		}

		for(int i = 0;i < static_cast<int>(pObjPlayerInfo->m_vTrainingList.size());i++)
		{
			if( pObjPlayerInfo->m_vTrainingList.at(i)->m_pTable->m_nType == pTrainingTable->m_nType )
			{
				nCode = pObjPlayerInfo->m_vTrainingList.at(i)->m_nCode;
				nType = pObjPlayerInfo->m_vTrainingList.at(i)->m_pTable->m_nType;
				nCode++;
				break;
			}
			nCode=-1;
		}

		if(pObjPlayerInfo->m_vTrainingList.size()==0)
			nCode=-1;

		if( (nCode==-1) )
		{
			if( (pTrainingTable->GetObjCode()%100) == 1 )
				m_nTrainingCode[nTotalCount++] = pTrainingTable->GetObjCode();
			nCode=0;
		}
	}
	
	m_nTotalPage = nTotalCount/LIST6_SIZE;
	if( (nTotalCount%LIST6_SIZE) != 0 )
		m_nTotalPage++;
}

void CSkillShopProcess::InitCeremonyCode()
{
	int nTotalCount = 0;
	int nCode = 0;
	int nType = 0;

	ZeroMemory(m_nCeremonyCode, sizeof(m_nCeremonyCode));
	
	CResCeremonyTableStack *temp = dynamic_cast<CResCeremonyTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_CEREMONYTABLE));
	assert(temp);
	for(int i=0;i<temp->GetResourceSize();i++)
	{
		CResCeremonyTableFormat *pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(temp->GetSequenceField(i));
		assert(pCeremonyTable);
		if(!pCeremonyTable->m_nSell)
			continue;
		m_nCeremonyCode[nTotalCount++] = pCeremonyTable->GetObjCode();
	}
	
	m_nTotalPage = nTotalCount/LIST6_SIZE;
	if( (nTotalCount%LIST6_SIZE) != 0 )
		m_nTotalPage++;
}

void CSkillShopProcess::UpdateSkillCode( char nPage )
{
	int nCode = 0;
	ZeroMemory(m_nCode, sizeof(m_nCode));
	
	nPage = FUNCTION_PageUpdate(nPage);

	for(int i = 0; i < LIST6_SIZE; i++)
	{
		m_nCode[i] = m_nSkillCode[LIST6_SIZE*(nPage-1)+i];

		CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[i], 0));

		if(pSkillTable)
			FUNCTION_ItemRender(i, m_nCode[i], GetSkillPoint(m_nCode[i]), GetSkillCash(m_nCode[i]), pSkillTable->m_sName, pSkillTable->m_sIconFile, pSkillTable->m_nSell);
		else FUNCTION_ItemRender(i, 0, 0, 0);
	}
}

void CSkillShopProcess::UpdateTrainingCode( char nPage )
{
	ZeroMemory(m_nCode, sizeof(m_nCode));

	nPage = FUNCTION_PageUpdate(nPage);

	for(int i = 0; i < LIST6_SIZE; i++)
	{
		m_nCode[i] = m_nTrainingCode[LIST6_SIZE*(nPage-1)+i];

		CResTrainingTableFormat *pTrainingTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, m_nCode[i], 0));
	
		if(pTrainingTable)
			FUNCTION_ItemRender(i, m_nCode[i], GetTrainingPoint(m_nCode[i]), GetTrainingCash(m_nCode[i]), pTrainingTable->m_sName, pTrainingTable->m_sIconFile, pTrainingTable->m_nSell);
		else FUNCTION_ItemRender(i, 0, 0, 0);
	}
}

void CSkillShopProcess::UpdateCeremonyCode( char nPage )
{
	int nCode = 0;

	ZeroMemory(m_nCode, sizeof(m_nCode));
	
	nPage = FUNCTION_PageUpdate(nPage);

	for(int i = 0; i < LIST6_SIZE; i++)
	{
		m_nCode[i] = m_nCeremonyCode[LIST6_SIZE*(nPage-1)+i];

		CResCeremonyTableFormat *pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, m_nCode[i], 0));		
		
		if(pCeremonyTable)
			FUNCTION_ItemRender(i, m_nCode[i], GetCeremonyPoint(m_nCode[i]), GetCeremonyCash(m_nCode[i]), pCeremonyTable->m_sName, pCeremonyTable->m_sIconFile, pCeremonyTable->m_nSell);
		else FUNCTION_ItemRender(i, 0, 0, 0);
	}
}

char CSkillShopProcess::FUNCTION_PageUpdate(char nPage)
{
	char sText[STRLEN_64]="";

	if( nPage )
		m_nSelectIndex = 0;

	m_nCurrentPage = nPage;

	if(nPage==0)									// 페이지가 미만, 초과일 경우
		m_nCurrentPage = nPage = m_nTotalPage;
	else if(nPage>m_nTotalPage)
		m_nCurrentPage = nPage = 1;

	_snprintf(sText, STRLEN_64, "%d/%d", m_nCurrentPage, m_nTotalPage);
	UIFunction::Set_ControlText( "SkillPageName", sText );

	return nPage;
}

void CSkillShopProcess::UpdatePositionArrange()
{
	CUIComboBox* pPosition_ComboBox = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl("PositionArrangeComboBox"));	// 선공
	assert(pPosition_ComboBox);
	char nPosition = 0;

	switch(pPosition_ComboBox->GetSelectedItemIndex())
	{
	case 0:		nPosition = 0;	break;	// ALL
	case 1:		nPosition = 10;	break;	// FW
	case 2:		nPosition = 11;	break;	// ST
	case 3:		nPosition = 12;	break;	// CF
	case 4:		nPosition = 13;	break;	// WF
	case 5:		nPosition = 14;	break;	// SS
	case 6:		nPosition = 20;	break;	// MF
	case 7:		nPosition = 21;	break;	// AM
	case 8:		nPosition = 22;	break;	// CM
	case 9:		nPosition = 23;	break;	// SM
	case 10:	nPosition = 24;	break;	// DM
	case 11:	nPosition = 30;	break;	// DF
	case 12:	nPosition = 31;	break;	// SW
	case 13:	nPosition = 32;	break;	// CB
	case 14:	nPosition = 33;	break;	// SP
	}
	InitSkillCode(nPosition);
	FUNCTION_GoPage(1);
}

void CSkillShopProcess::LoadCameraType(const char *sCameraFileName)
{
	m_spCeremonyCameraNode = 0;

	char temp[MAX_PATH];

	sprintf(temp, "Camera\\%s.nif", "CereCamera01");

	NiStream kStream;

	if(!kStream.Load(temp)) return ;

	assert(NiIsKindOf(NiNode, kStream.GetObjectAt(0)));
	m_spCeremonyCameraNode = (NiNode*)kStream.GetObjectAt(0);

	m_spCeremonyCamera = (NiCamera*) m_spCeremonyCameraNode->GetObjectByName("object0");

	m_spCeremonyCameraNode->Update(0.f);

	sprintf(temp, "Camera\\%s.kf", sCameraFileName);

	if( m_pkControllerManager != 0 ) m_pkControllerManager->DeactivateAll();

	m_spCeremonyCameraNode->RemoveController( m_pkControllerManager );
	m_pkControllerManager = 0;

	m_pkControllerManager = NiNew NiControllerManager( m_spCeremonyCameraNode );

	if(!m_pkControllerManager->AddAllSequencesFromFile( temp )) return ;

	NiControllerSequence *pkDestSequence = m_pkControllerManager->GetSequenceAt(0);            

	m_pkControllerManager->DeactivateAll();

	pkDestSequence->ResetSequence();

	m_pkControllerManager->ActivateSequence( pkDestSequence );
}

void CSkillShopProcess::FUNCTION_DetailTabChange(char idx)
{
	char sText[STRLEN_64]="";

	m_nDetailTabIndex = idx;

	if( m_nBigFractionIndex == 0 && m_nSmallFractionIndex == 1 )
	{
		UIFunction::Set_UIVisibility( "PositionArrangeBox", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Sort", N3UI_VISIBLE_TRUE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "PositionArrangeBox", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Sort", N3UI_VISIBLE_FALSE );
	}

	switch(idx)
	{
	case 0:		// 상세보기
		{
			//switch(m_nBigFractionIndex)
			//{
			//case 0:		// 스킬
			//case 1:		// 트레이닝
			//case 2:		// 세레모니
			//}
		}
		break;
	case 1:		// 미리보기
		{
			switch(m_nBigFractionIndex)
			{
			case 0:		// 스킬
				{
					CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[m_nSelectIndex], 0));
					if(pSkillTable == NULL)	return;

					if( idx == 1 && pSkillTable->m_nAniCode )
						FUNCTION_SkillView();
				}
				break;
			case 1:		// 트레이닝
				break;
			case 2:		// 세레모니
				FUNCTION_CeremonyView();
				break;
			}
		}
		break;
	}

	if( idx == 1 && m_nBigFractionIndex == 1 )
		return;

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

void CSkillShopProcess::FUNCTION_DetailTabUpdate( int ItemIndex )
{
	if( m_nDetailTabIndex == 0 )
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

void CSkillShopProcess::FUNCTION_Selection()
{
	int nListCount = LIST6_SIZE;

	char sText[STRLEN_64]="";

	for(int i = 0; i < nListCount; i++)
	{
		_snprintf(sText, STRLEN_64, "Slot_Big");

		CUIControl* pListBG = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pListBG);

		_snprintf(sText, STRLEN_64, "ListBG%d", i);
		CUIControl* pControl = pListBG->FindEventControl(sText);
		assert(pControl);

		if( UICommonMethod::IsRectInPoint(&pControl->GetWindowBox(), &m_pDNDPoint) )
		{
			m_nSelectIndex = i;

			if( !m_nBigFractionIndex )
				m_nSelectArea = 0;

			if( m_nBigFractionIndex == 1 )
				return;

			switch( m_nBigFractionIndex )
			{
			case 0:
				{
					CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[m_nSelectIndex], 0));
					if(pSkillTable == NULL)	return;

					if( m_nDetailTabIndex == 1 && pSkillTable->m_nAniCode )
						FUNCTION_SkillView();
				}
				break;
			case 1:
				break;
			case 2:
				{
					if( m_nDetailTabIndex == 1 )
						FUNCTION_CeremonyView();
				}			
				break;
			}
		}
	}
}

void CSkillShopProcess::FUNCTION_Buy(char idx)
{
	switch( m_nBigFractionIndex )
	{
	case 0:
		{
			CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, m_nCode[idx], 0));
			assert(pSkillTable);
			CSkillShopDialog::GetPtr()->FUNCTION_InitShopDialog(pSkillTable->m_sName, pSkillTable->m_sIconFile, m_nBigFractionIndex, m_nCode[idx], pSkillTable->m_nSell, idx);
		}
		break;
	case 1:
		{
            CResTrainingTableFormat *pTrainingTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, m_nCode[idx], 0));
			assert(pTrainingTable);
			CSkillShopDialog::GetPtr()->FUNCTION_InitShopDialog(pTrainingTable->m_sName, pTrainingTable->m_sIconFile, m_nBigFractionIndex, m_nCode[idx], pTrainingTable->m_nSell, idx);
		}
		break;
	case 2:
		{
			CResCeremonyTableFormat *pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, m_nCode[idx], 0));
			assert(pCeremonyTable);
			CSkillShopDialog::GetPtr()->FUNCTION_InitShopDialog(pCeremonyTable->m_sName, pCeremonyTable->m_sIconFile, m_nBigFractionIndex, m_nCode[idx], pCeremonyTable->m_nSell, idx);
		}
		break;
	}
}