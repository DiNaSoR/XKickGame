#include "stdafx.h"
#include "ChoiceClassProcess.h"
#include "ChoiceRoomProcess.h"
#include "ObjServerInfo.h"
#include "PlayModeManager.h"
#include "UICommonMethod.h"
#include "UIManager.h"
#include "UIFunction.h"
#include "UIProgress.h"

CChoiceClassProcess::CChoiceClassProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_CHOICECLASS;
	m_pd3dDevice = NiDX9Renderer::GetRenderer()->GetD3DDevice();
	m_n1stClassStep = 0;
}

CChoiceClassProcess::CChoiceClassProcess(char nClassIndex) : CBaseProcess(), 
	m_nClassIndex(nClassIndex)
{
	m_nProcessCmd = PROCESS_CHOICECLASS;
	m_pd3dDevice = NiDX9Renderer::GetRenderer()->GetD3DDevice();
	m_n1stClassStep = 0;
}

CChoiceClassProcess::~CChoiceClassProcess(void)
{
}

void CChoiceClassProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CChoiceClassProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	FUNCTION_InitChoiceClass();

	FUNCTION_3DRender( "3DCharacter", pObjGroundInfo->GetObjectCamera());

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CChoiceClassProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	Update(fFrameTime, fAccumTime);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	
	UICommonMethod::BasicInfoStop();
	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
}

void CChoiceClassProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("ExplainBG");
		if(pControl->GetVisible() == N3UI_VISIBLE_TRUE)
			m_pPolyGonDraw.DrawPolygon( "PentagonBG", 5, 10.0f, m_nColor, m_vSink, m_fScale );

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();
	}
}

void CChoiceClassProcess::Update(float fFrameTime, float fAccumTime)
{
    if(m_pObjPlayerInfo && m_pObjPlayerInfo->m_pModel)
	{
		CGameProject::s_pGameProject->BeginFrame();

		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime, true);

		m_cCharacterTexBoard.Render();

		CGameProject::s_pGameProject->EndFrame();

		CGameProject::s_pGameProject->DisplayFrame();
	}
}

void CChoiceClassProcess::UpdateState(float fFrameTime, float fAccumTime)
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

			m_nState = STATE_PLAY;
		}
		break;
	case STATE_PLAY:
		{
			m_nState = STATE_END;
		}
		break;
	case STATE_END:
		{
		}
		break;
	}
}

void CChoiceClassProcess::LoadCharacter()
{
	CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pObjPlayerInfo, 0, 0);
	m_pObjPlayerInfo->m_cNodePoint = NiPoint3(10000,0,0);
	m_pObjPlayerInfo->m_cCurrentAction.m_fStartAngle = 180.f;
	CProcessManager::GetPtr()->AttachItem(m_pObjPlayerInfo);
}

void CChoiceClassProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	pObjGroundInfo->SetObjectPosition(NiPoint3(10000,0,0));

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

bool CChoiceClassProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CChoiceClassProcess::GetSameProcess()
{
	return new CChoiceClassProcess;
}

void CChoiceClassProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		FUNCTION_SkillNameToolTipViewUpdate();
		break;
	}
}

void CChoiceClassProcess::FUNCTION_SkillNameToolTipViewUpdate()
{
	char sText[MAX_PATH]="";
	
	RECT rt;

	_snprintf(sText, MAX_PATH, "ClassTree_0%d", m_nPosition);
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pControl);
    	
	CUIControl* pToolTip = CClientUIManager::GetPtr()->FindEventControl("Tooltip");
	assert(pToolTip);

	int i=0;

	for(;i<4;i++)
	{
		_snprintf(sText, MAX_PATH, "SkillBG%d", i);
		CUIControl* pOverImg = pControl->FindEventControl(sText);
		assert(pOverImg);

		rt = pOverImg->GetWindowBox();
		
		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint))
		{
			pToolTip->SetVisible(N3UI_VISIBLE_TRUE);
			break;
		}
		else
			pToolTip->SetVisible(N3UI_VISIBLE_FALSE);
	}

	if( i == 4 ) return;

	int arr1stSkill[3][4] = {	{ 101110, 112110, 113210, 121110 },		// FW
								{ 101314, 123110, 122110, 131120 },		// MF
								{ 141124, 171124, 191610, 201110 }};	// DF

	CResSkillTableFormat* pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, arr1stSkill[m_nPosition-1][i], 0));
	assert(pSkillTable);

	_snprintf(sText, MAX_PATH, pSkillTable->m_sName);
	pToolTip->SetText(sText);

	int nLen = strlen(pSkillTable->m_sName);

	pToolTip->SetLocation( m_pDNDPoint.x-5, m_pDNDPoint.y+15, true ); 
	pToolTip->SetSize( 40 + (nLen * 5), 40 );
}

void CChoiceClassProcess::FUNCTION_InitChoiceClass()
{
	char sText[MAX_PATH]="";

	InitPolygonDraw();
	PositionDecision();

	FUNCTION_PositionExplain(m_nPosition);

	UIFunction::Set_UIVisibility( "ExplainBG", N3UI_VISIBLE_TRUE );
	_snprintf( sText, MAX_PATH, CResourceManager::GetPtr()->m_pStaticMap[ 50004 ].c_str(), m_pObjPlayerInfo->m_sName );
	UIFunction::Set_ControlText( "ClassExplain", sText );

	UIFunction::Set_ControlText( "CharacterName", m_pObjPlayerInfo->m_sName );
	
	//UIFunction::Set_UIVisibility( "MenuTabButto00", N3UI_VISIBLE_FALSE );
	//UIFunction::Set_UIVisibility( "Go_ChoiceServer_Btn", N3UI_VISIBLE_TRUE );
	//UIFunction::Set_UIVisibility( "Back_Btn", N3UI_VISIBLE_FALSE );



	//	_snprintf(sText, MAX_PATH, "2nd Class_Tree0%d", i);
	//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

	//	_snprintf(sText, MAX_PATH, "Static_Select%dA", i);
	//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	//	_snprintf(sText, MAX_PATH, "Static_Select%dB", i);
	//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	//	_snprintf(sText, MAX_PATH, "Static_Select%dC", i);
	//	UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	//}

	//UIFunction::Set_UIVisibility( "Choice_Class_Title", N3UI_VISIBLE_TRUE );		// 클래스 선택 타이틀
	//UIFunction::Set_UIVisibility( "Create_Character_Title", N3UI_VISIBLE_FALSE );	// 캐릭터 생성 타이틀

	//UIFunction::Set_UIVisibility( "Style_Static", N3UI_VISIBLE_FALSE );				// 캐릭터 스타일 선택 창
	//UIFunction::Set_UIVisibility( "Statistics_Graph", N3UI_VISIBLE_TRUE );			// 전직 설명 창
	//UIFunction::Set_UIVisibility( "Create_Character", N3UI_VISIBLE_FALSE );			// 생성 버튼
	//UIFunction::Set_UIVisibility( "Choice_Class_Confrim", N3UI_VISIBLE_TRUE );		// 전직 버튼
	//UIFunction::Set_UIVisibility( "Choice_Taxt02", N3UI_VISIBLE_FALSE );			// 추천 포지션
	//	
	//CUIControl* pText = CClientUIManager::GetPtr()->FindEventControl( "Choice_Taxt01" );	// 캐릭터 명
	//assert(pText);

	//pText->SetText(CResourceManager::GetPtr()->m_pStaticMap[ 50004 ].c_str());
	//
	//UIFunction::Set_ControlText( "CName", m_pPlayerInfo->m_sName );

	//_snprintf( sText, MAX_PATH, pText->GetTextEx().c_str(), m_pPlayerInfo->m_sName );
	//pText->SetText(sText);

	//// 능력치 보이기
	//_snprintf( sText, MAX_PATH, "Static_Select%dA", m_nPosition - 1 );
	//UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
	//_snprintf( sText, MAX_PATH, "Static_Select%dB", m_nPosition - 1 );
	//UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
	//_snprintf( sText, MAX_PATH, "Static_Select%dC", m_nPosition - 1 );
	//UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );

	//FUNCTION_SelectPositionVeiw(m_nPosition - 1);
}

void CChoiceClassProcess::FUNCTION_ClassNextExplain()
{
	char sText[MAX_PATH]="";

	m_n1stClassStep++;

	switch( m_n1stClassStep )
	{
	case 1:
		{
			_snprintf( sText, MAX_PATH, CResourceManager::GetPtr()->m_pStaticMap[ 50004+m_nPosition ].c_str(), m_pObjPlayerInfo->m_sName );
			UIFunction::Set_ControlText( "ClassExplain", sText );
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900095 ,0));
			assert(pMTable);
			_snprintf( sText, MAX_PATH, pMTable->m_sMessage, UICommonMethod::PositionNtoPositionStr(m_nPosition*LIST10_SIZE).c_str() );
			UIFunction::Set_ControlText( "RecommendationClass", sText );
		}
		break;
	case 2:
		{
			UIFunction::Set_UIVisibility( "ExplainBG", N3UI_VISIBLE_FALSE );
		}
		break;

	}
}

void CChoiceClassProcess::FUNCTION_PositionExplain(char nPosition)
{
	m_nPosition = nPosition;

	char sText[MAX_PATH]="";

	for(int i=1;i<4;i++)
	{
		_snprintf(sText, MAX_PATH, "ClassTree_0%d", i);
		if( i == nPosition )
		{
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
			_snprintf( sText, MAX_PATH, "%s", CResourceManager::GetPtr()->m_pStaticMap[ 80050+nPosition ].c_str() );
			UIFunction::Set_ControlText( "ClassExplain_Text", sText );
			_snprintf( sText, MAX_PATH, "%s", CResourceManager::GetPtr()->m_pStaticMap[ 80053+nPosition ].c_str() );
			UIFunction::Set_ControlText( "AutoGrowthExplain_Text", sText );
		}
		else
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

		_snprintf( sText, MAX_PATH, "Position_Btn_1%d", i );
		if( i == nPosition )
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
		else
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	}

	int arr1stSkill[3][4] = {	{ 1011, 1121, 1132, 1211 },		// FW
								{ 1013, 1231, 1221, 1311 },		// MF
								{ 1411, 1711, 1916, 2011 }};	// DF

	char sImg[STRLEN_64]="";

	for(int i=0;i<4;i++)
	{
		_snprintf( sText, MAX_PATH, "ClassTree_0%d", nPosition);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pControl);
		_snprintf( sText, MAX_PATH, "SkillBG%d", i );
		CUIControl* pSkillBG = pControl->FindEventControl( sText );
		assert(pSkillBG);
		_snprintf( sImg, STRLEN_64, "Icon/Skill/Skill_%d.png", arr1stSkill[nPosition-1][i] );
		UIFunction::Set_ChildControlTexture( pSkillBG, "SkillImg", sImg ); 
	}

	// 각 능력치 표시
	for(int i=0;i<15;i++)
	{
		_snprintf(sText, MAX_PATH, "Ability%d_Static", i);
		
		CResPositionTableFormat *pPositioinTable = dynamic_cast<CResPositionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_POSITIONTABLE, nPosition*LIST10_SIZE , 0));
		assert(pPositioinTable);
		if( i == pPositioinTable->m_nAuto0 || i == pPositioinTable->m_nAuto1 || i == pPositioinTable->m_nAuto2 )
		{
			UIFunction::SetChildToVisibility( sText, "Line", N3UI_VISIBLE_TRUE );
			UIFunction::SetChildToVisibility( sText, "Automatic_Img", N3UI_VISIBLE_TRUE );
		}
		else
		{
			UIFunction::SetChildToVisibility( sText, "Line", N3UI_VISIBLE_FALSE );
			UIFunction::SetChildToVisibility( sText, "Automatic_Img", N3UI_VISIBLE_FALSE );
		}

		char sNum[STRLEN_32]="";
		_snprintf(sNum, STRLEN_32, "%d", pPositioinTable->m_nAbility[i]);
		UIFunction::Set_ChildControlText( sText, "Static_AbilityNum", sNum );
		UIFunction::SetProgressValue( sText, "GaugeBar_Static", "AbilityPrg", pPositioinTable->m_nAbility[i]);

		/*dynamic_cast<CUIProgress*>(pControl->FindEventControl("ProgressItem"))->SetValue(pPositioinTable->m_nAbility[i]+m_pPlayerInfo->GetItemFaculty(i));
		dynamic_cast<CUIProgress*>(pControl->FindEventControl("ProgressTraining"))->SetValue(pPositioinTable->m_nAbility[i]+m_pPlayerInfo->GetItemFaculty(i)+m_pPlayerInfo->GetTrainingFaculty(i));*/
	}
}

void CChoiceClassProcess::InitPolygonDraw()
{
	srand(time(NULL));
	for(int i=0;i<LIST32_SIZE;i++)
		m_fScale[i] = (float)(rand()%4)+1;

	m_vSink = D3DXVECTOR3( -1.8f, 1.2f, 0.0f );

	m_nColor[0] = D3DXCOLOR( 255, 0, 0, 188 );
	m_nColor[1] = D3DXCOLOR( 128, 128, 0, 188 );
	m_nColor[2] = D3DXCOLOR( 0, 255, 0, 188 );
	m_nColor[3] = D3DXCOLOR( 0, 128, 128, 188 );
	m_nColor[4] = D3DXCOLOR( 0, 0, 255, 188 );
	m_nColor[5] = D3DXCOLOR( 128, 0, 128, 188 );
}

void CChoiceClassProcess::PositionDecision()
{
	char sText[MAX_PATH]="";
	float nFW=0, nMF=0, nDF=0;
	m_nPosition=0;

	nFW = m_fScale[0];
	nMF = m_fScale[1];
	nMF += m_fScale[2];
	nMF /= 2.0f;
	nDF = m_fScale[3];
	nDF += m_fScale[4];
	nDF /= 2.0f;

	if(nFW>nMF)
	{
		if(nFW>nDF)
			m_nPosition = 1;
		else
			m_nPosition = 3;
	}
	else
	{
		if(nMF>nDF)
			m_nPosition = 2;
		else
			m_nPosition = 3;
	}
}

//void CCreateCharacterProcess::FUNCTION_Init2ndChoiceClass()
//{
//	char sText[MAX_PATH]="";
//
//	// 이전 버튼
//	UIFunction::Set_UIVisibility( "MenuTabButtoX", N3UI_VISIBLE_TRUE );
//	UIFunction::Set_UIVisibility( "MenuTabButto00", N3UI_VISIBLE_FALSE );
//	UIFunction::Set_UIVisibility( "Go_ChoiceServer_Btn", N3UI_VISIBLE_TRUE );
//	UIFunction::Set_UIVisibility( "Back_Btn", N3UI_VISIBLE_FALSE );
//
//	for(int i=0;i<3;i++)
//	{
//		_snprintf(sText, MAX_PATH, "1st Class_Tree0%d", i);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//		_snprintf(sText, MAX_PATH, "2nd Class_Tree0%d", i);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//
//		_snprintf(sText, MAX_PATH, "Static_Select%dA", i);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//		_snprintf(sText, MAX_PATH, "Static_Select%dB", i);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//		_snprintf(sText, MAX_PATH, "Static_Select%dC", i);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//	}
//
//	UIFunction::Set_UIVisibility( "Choice_Class_Title", N3UI_VISIBLE_TRUE );		// 클래스 선택 타이틀
//	UIFunction::Set_UIVisibility( "Create_Character_Title", N3UI_VISIBLE_FALSE );	// 캐릭터 생성 타이틀
//
//	UIFunction::Set_UIVisibility( "Style_Static", N3UI_VISIBLE_FALSE );				// 캐릭터 스타일 선택 창
//	UIFunction::Set_UIVisibility( "Create_Character", N3UI_VISIBLE_FALSE );			// 생성 버튼
//	UIFunction::Set_UIVisibility( "Choice_Class_Confrim", N3UI_VISIBLE_TRUE );		// 전직 버튼
//		
//	m_nPosition = m_pPlayerInfo->m_nPosition;
//
//	FUNCTION_SelectPositionVeiw(0);
//}
//
//void CCreateCharacterProcess::FUNCTION_Explain()
//{
//	char sText[MAX_PATH]="";
//
//	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Choice_Taxt02" );
//	assert(pControl);
//
//	CResMessageTableFormat* pMTable = NULL;
//	
//	if( pControl->GetVisible() == N3UI_VISIBLE_FALSE )	// 첫번재로 버튼 눌렀을 때
//	{
//		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900095 ,0));
//		assert(pMTable);
//		
//		_snprintf( sText, MAX_PATH, pMTable->m_sMessage, UICommonMethod::PositionNtoPositionStr(m_nPosition*LIST10_SIZE).c_str() );
//		pControl->SetText(sText);
//		pControl->SetVisible(N3UI_VISIBLE_TRUE);
//
//		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900095 + m_nPosition ,0));
//		assert(pMTable);
//		_snprintf( sText, MAX_PATH, pMTable->m_sMessage, m_pPlayerInfo->m_sName );
//		UIFunction::Set_ControlText( "Choice_Taxt01", sText );
//	}
//	else												// 두번째로 버튼 눌렀을 때
//	{
//		FUNCTION_SelectPositionVeiw(m_nPosition - 1);
//		UIFunction::Set_UIVisibility( "Statistics_Graph", N3UI_VISIBLE_FALSE );
//	}
//}
//
//void CCreateCharacterProcess::FUNCTION_SelectPositionVeiw(char idx)
//{
//	char sText[MAX_PATH]="";
//	CResPositionTableFormat *pPositioinTable = NULL;
//	CUIControl* pControl = NULL;
//
//	if( LIST10_SIZE <= m_nPosition )		// 2차 전직
//	{
//		m_nPosition = m_pPlayerInfo->m_nPosition + idx + 1;
//
//		_snprintf( sText, MAX_PATH, "2nd Class_Tree0%d", (m_nPosition/LIST10_SIZE)-1 );
//		pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
//		assert(pControl);
//
//		for(int i=0;i<4;i++)
//		{
//			_snprintf( sText, MAX_PATH, "Position_0%d", i );
//			CUIButton* pBtn = dynamic_cast<CUIButton*>(pControl->FindEventControl( sText ));
//			assert(pBtn);
//			pBtn->SetPressed(false);
//		}
//
//		char sImg[STRLEN_64]="";
//
//		// 포지션 테이블 로딩
//		if ( m_nPosition == 34 )
//		{
//			pPositioinTable = dynamic_cast<CResPositionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_POSITIONTABLE, POSITION_GK, 0));
//			assert(pPositioinTable);
//		}
//		else
//		{
//			pPositioinTable = dynamic_cast<CResPositionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_POSITIONTABLE, m_nPosition, 0));
//			assert(pPositioinTable);
//		}
//
//		// 2차 전직 트리보기
//		_snprintf( sText, MAX_PATH, "2nd Class_Tree0%d", (m_nPosition/LIST10_SIZE)-1 );
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );	
//	}
//	else							// 1차 전직
//	{
//		m_nPosition = idx + 1;
//
//		_snprintf( sText, MAX_PATH, "1st Class_Tree0%d", idx );
//		pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
//		assert(pControl);
//
//		// 초기화
//		for(int i=0;i<3;i++)
//		{
//			_snprintf( sText, MAX_PATH, "1st Class_Tree0%d", i );
//			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//
//			_snprintf( sText, MAX_PATH, "Position_0%d", i );
//			CUIButton* pBtn = dynamic_cast<CUIButton*>(pControl->FindEventControl( sText ));
//			assert(pBtn);
//			pBtn->SetPressed(false);
//
//			_snprintf(sText, MAX_PATH, "Static_Select%dA", i);
//			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//			_snprintf(sText, MAX_PATH, "Static_Select%dB", i);
//			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//			_snprintf(sText, MAX_PATH, "Static_Select%dC", i);
//			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
//		}
//
//		// 능력치 보이기
//		_snprintf(sText, MAX_PATH, "Static_Select%dA", idx);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
//		_snprintf(sText, MAX_PATH, "Static_Select%dB", idx);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
//		_snprintf(sText, MAX_PATH, "Static_Select%dC", idx);
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
//
//		// 포지션 테이블 로딩
//		pPositioinTable = dynamic_cast<CResPositionTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_POSITIONTABLE, m_nPosition*LIST10_SIZE, 0));
//		assert(pPositioinTable);
//
//		// 1차 전직 트리보기
//		_snprintf( sText, MAX_PATH, "1st Class_Tree0%d", idx );
//		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
//	}
//
//	pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
//	assert(pControl);
//
//	char sImg[STRLEN_64]="";
//
//	if( LIST10_SIZE <= m_nPosition )		// 2차 전직 대표 스킬 표시
//	{
//		int arr2ndSkill[3][4][12] = {
//									{{ 1133, 1142, 1171, 1181, 1163, 1012, 1232, 1522, 1913, 2052, 2053, 9013 },	// ST
//									{ 1051, 1134, 1161, 1111, 1113, 1053, 1923, 1233, 1232, 2051, 9021, 2111 },		// CF
//									{ 1014, 1212, 1234, 1519, 1611, 1612, 1614, 1617, 1914, 1919, 2112, 1111 },		// WF
//									{ 1012, 1013, 1122, 1142, 1163, 1234, 1513, 1512, 1921, 2052, 9013, 1212 }},	// SS
//									{{1016, 1052, 1112, 1121, 1122, 1163, 1212, 1512, 1515, 1517, 1913, 2051 },		// AM
//									{ 1122, 1235, 1312, 1321, 1322, 1513, 1620, 1621, 1731, 2012, 1121, 1511 },		// CM
//									{ 1015, 1302, 1611, 1613, 1614, 1618, 1619, 1913, 1919, 1920, 2113, 1519 },		// SM
//									{ 1016, 1312, 1412, 1520, 1621, 1711, 1731, 2012, 2032, 2053, 1912, 2023 }},	// DM
//									{{1017, 1312, 1414, 1412, 1721, 1912, 1722, 2022, 2042, 2012, 1917, 2061 },		// SW
//									{ 1414, 1421, 1520, 1712, 1912, 1917, 2012, 2023, 2041, 2061, 1917, 1518 },		// CB
//									{ 1182, 1413, 1520, 1712, 1912, 1917, 2012, 2021, 2031, 2061, 2051, 1516 },		// SP
//									{ 1301, 1401, 1411, 1711, 1911, 1916, 1918, 2011, 9024, 9025, 9026, 9050 }}		// GK
//									};
//
//		for(int i=0;i<12;i++)
//		{
//			_snprintf( sText, MAX_PATH, "SHOP_SkillBG%d", i );
//			CUIControl* pImg = pControl->FindEventControl( sText );
//
//			_snprintf( sImg, STRLEN_64, "Icon/Skill/Skill_%d.png", arr2ndSkill[(m_nPosition/LIST10_SIZE)-1][idx][i] );
//			UIFunction::Set_ChildControlTexture( pImg, "ListImg0", sImg ); 
//		}
//
//		CUIControl* pText = pControl->FindEventControl( "Position_2nd_Explain" );	// 캐릭터 명
//		assert(pText);
//
//		switch(m_nPosition/LIST10_SIZE)
//		{
//		case 1:
//			pText->SetText(CResourceManager::GetPtr()->m_pStaticMap[ 80057+idx ].c_str());
//			break;
//		case 2:
//			pText->SetText(CResourceManager::GetPtr()->m_pStaticMap[ 80061+idx ].c_str());
//			break;
//		case 3:
//			pText->SetText(CResourceManager::GetPtr()->m_pStaticMap[ 80065+idx ].c_str());
//			break;
//		}
//	}
//	else									// 1차 전직 대표 스킬 표시
//	{
//		int arr1stSkill[3][4] = {	{ 1011, 1121, 1132, 1211 },		// FW
//									{ 1013, 1231, 1221, 1311 },		// MF
//									{ 1411, 1711, 1916, 2011 }};	// DF
//
//		CUIControl* pText = pControl->FindEventControl( "Position_1st_Explain" );	// 캐릭터 명
//		assert(pText);
//
//		pText->SetText(CResourceManager::GetPtr()->m_pStaticMap[ 80053+m_nPosition ].c_str());
//
//		for(int i=0;i<4;i++)
//		{
//			_snprintf( sText, MAX_PATH, "SHOP_SkillBG%d", i );
//			CUIControl* pImg = pControl->FindEventControl( sText );
//
//			_snprintf( sImg, STRLEN_64, "Icon/Skill/Skill_%d.png", arr1stSkill[idx][i] );
//			UIFunction::Set_ChildControlTexture( pImg, "ListImg0", sImg ); 
//		}
//	}
//
//	// 선택된 포지션 표시
//	_snprintf( sText, MAX_PATH, "Position_0%d", idx%LIST10_SIZE );
//	CUIButton* pBtn = dynamic_cast<CUIButton*>(pControl->FindEventControl( sText ));
//	assert(pBtn);
//	pBtn->SetPressed(true);
//
//	// 각 능력치 표시
//	for(int i=0;i<18;i++)
//	{
//		_snprintf(sText, MAX_PATH, "Faculty%02d", i);
//		pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
//		CUIControl* pFacultyNum = pControl->FindEventControl("Static_AbilityNum");
//		_snprintf(sText, MAX_PATH, "%d", pPositioinTable->m_nAbility[i]);
//		pFacultyNum->SetText(sText);
//		dynamic_cast<CUIProgress*>(pControl->FindEventControl("ProgressBase"))->SetValue(pPositioinTable->m_nAbility[i]);
//		dynamic_cast<CUIProgress*>(pControl->FindEventControl("ProgressItem"))->SetValue(pPositioinTable->m_nAbility[i]+m_pPlayerInfo->GetItemFaculty(i));
//		dynamic_cast<CUIProgress*>(pControl->FindEventControl("ProgressTraining"))->SetValue(pPositioinTable->m_nAbility[i]+m_pPlayerInfo->GetItemFaculty(i)+m_pPlayerInfo->GetTrainingFaculty(i));
//	}
//}
//
//int arr2ndSkill[3][4][12] = {
//							{{ 1133, 1142, 1171, 1181, 1163, 1012, 1232, 1522, 1913, 2052, 2053, 9013 },	// ST
//							{ 1051, 1134, 1161, 1111, 1113, 1053, 1923, 1233, 1232, 2051, 9021, 2111 },		// CF
//							{ 1014, 1212, 1234, 1519, 1611, 1612, 1614, 1617, 1914, 1919, 2112, 1111 },		// WF
//							{ 1012, 1013, 1122, 1142, 1163, 1234, 1513, 1512, 1921, 2052, 9013, 1212 }},	// SS
//							{{1016, 1052, 1112, 1121, 1122, 1163, 1212, 1512, 1515, 1517, 1913, 2051 },		// AM
//							{ 1122, 1235, 1312, 1321, 1322, 1513, 1620, 1621, 1731, 2012, 1121, 1511 },		// CM
//							{ 1015, 1302, 1611, 1613, 1614, 1618, 1619, 1913, 1919, 1920, 2113, 1519 },		// SM
//							{ 1016, 1312, 1412, 1520, 1621, 1711, 1731, 2012, 2032, 2053, 1912, 2023 }},	// DM
//							{{1017, 1312, 1414, 1412, 1721, 1912, 1722, 2022, 2042, 2012, 1917, 2061 },		// SW
//							{ 1414, 1421, 1520, 1712, 1912, 1917, 2012, 2023, 2041, 2061, 1917, 1518 },		// CB
//							{ 1182, 1413, 1520, 1712, 1912, 1917, 2012, 2021, 2031, 2061, 2051, 1516 },		// SP
//							{ 1301, 1401, 1411, 1711, 1911, 1916, 1918, 2011, 9024, 9025, 9026, 9050 }}		// GK
//							};