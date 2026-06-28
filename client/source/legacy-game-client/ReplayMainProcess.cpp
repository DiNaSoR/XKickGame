#include "stdafx.h"
#include "ReplayMainProcess.h"
#include "ChoiceRoomProcess.h"
#include "UICommonMethod.h"
#include "ObjServerInfo.h"
#include "UtlLog.h"
#include "../InterfaceLibrary/UIButton.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "ReplayPacket.h"
#include "UIFunction.h"
#include "ReplayLoadingProcess.h"

CReplayMainProcess::CReplayMainProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_REPLAYMAIN;
	m_nPage = 1;
	m_nTotalPage = 1;
	m_nKind = 0;
	m_nSelectReplay = -1;

	m_bIsRender = true;
}

CReplayMainProcess::~CReplayMainProcess(void)
{
}

void CReplayMainProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CReplayMainProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	FUNCTION_UIInit();

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "Detail_Bg", "InventoryBg" );

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_START;
}

void CReplayMainProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	
	//UICommonMethod::BGEffectAnimationPlay( "Detail_Bg", "InventoryBg" );
	UICommonMethod::BasicInfoStop();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
	UICommonMethod::DisPlayNotice();
}

void CReplayMainProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{		
			LoadGround();

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

void CReplayMainProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();
	}
}

bool CReplayMainProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CReplayMainProcess::GetSameProcess()
{
	return new CReplayMainProcess;
}

void CReplayMainProcess::FileListUpdate(char nKind)
{
	char sText[MAX_PATH]="";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = NULL;

	m_vFileList.clear();

	switch(nKind)
	{
	case 0:
	case 1:
		hFile = FindFirstFile(".\\Replay\\*.xkr", &FindFileData);
		break;
	case 2:
		hFile = FindFirstFile(".\\Replay\\Download\\*.xkr", &FindFileData);
		break;
	}

	int i = 0;

	for(i=0;i<LIST10_SIZE;i++)
	{
		_snprintf(sText, MAX_PATH, "RList%02d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}
		
	if(hFile == INVALID_HANDLE_VALUE) 
	{
		m_nTotalPage = 1;
		return;
	}

	i = 0;
	bool bFirst = true;

	while( hFile != INVALID_HANDLE_VALUE )
	{
		if( bFirst )
			bFirst = false;
		else
		{
			if(!FindNextFile(hFile, &FindFileData))
				break;
		}

		if( i < (LIST10_SIZE*(m_nPage-1)) ) 
		{	
			i++;
			continue;
		}
		

		_snprintf(sText, MAX_PATH, "RList%02d", i%LIST10_SIZE);
		CUIControl* pList = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pList);

		if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if( (int)strlen(FindFileData.cFileName) != 0 )
			{
				if( nKind == 0 )
				{
					if( (LIST10_SIZE*(m_nPage)) <= i )
					{
						i++;
						continue;
					}

					char sCur_Time[STRLEN_32]="";
					char sName[MAX_PATH]="";

					GetCurDateTime(sCur_Time, "%Y%m%d");

					strcpy(sName, &FindFileData.cFileName[11]);
					sName[8] = '\0';
					if(!strcmp(sCur_Time, sName))
					{
						m_vFileList.push_back(FindFileData.cFileName);
				
						_snprintf(sText, MAX_PATH, "Replay_Name%02d", i%LIST10_SIZE);
						UIFunction::Set_ControlText( sText, FindFileData.cFileName );
						pList->SetVisible(N3UI_VISIBLE_TRUE);
						i++;
					}
				}
				else
				{
					if( (LIST10_SIZE*(m_nPage)) <= i )
					{
						i++;
						continue;
					}

					m_vFileList.push_back(FindFileData.cFileName);
			
					_snprintf(sText, MAX_PATH, "Replay_Name%02d", i%LIST10_SIZE);
					UIFunction::Set_ControlText( sText, FindFileData.cFileName );
					pList->SetVisible(N3UI_VISIBLE_TRUE);
					i++;
				}
			}
		}
	}

	m_nTotalPage = i/LIST10_SIZE;
	if( i%LIST10_SIZE != 0 || i == 0 )
		m_nTotalPage++;

	FindClose(hFile);
}

void CReplayMainProcess::FUNCTION_UIInit()
{
	SN3UIUserFunctionParameter cParam;
	cParam.AddParam( "Index", "0" );
	CClientUIManager::GetPtr()->ProcUserFunction( "REPLAY_TAB", &cParam );

	FUNCTION_UpdatePage(0);

	if(1<=(int)m_vFileList.size())
	{
		SN3UIUserFunctionParameter cParam;
		cParam.AddParam( "Index", "0" );
	    CClientUIManager::GetPtr()->ProcUserFunction( "REPLAY_SELECT", &cParam );
	}
}

void CReplayMainProcess::ReplayInfomationUpdate(char idx)
{
	char sText[MAX_PATH]="";

	CReplayReview		cReview;

	memset(&cReview, 0, sizeof(CReplayReview));

	switch(m_nKind)
	{
	case 0:
	case 1:
		_snprintf(sText, MAX_PATH, ".\\Replay\\%s", m_vFileList.at(idx).c_str());
		break;
	case 2:
		_snprintf(sText, MAX_PATH, ".\\Replay\\Download\\%s", m_vFileList.at(idx).c_str());
		break;
	}

	cReview = CPacketManager::GetPtr()->m_cReplayQue.ReadReplayReview(sText);
	
	FUNCTION_UpdateScoreUI(cReview.m_nHomeScore, cReview.m_nAwayScore);

	for(int i=0;i<TEAM_SIZE;i++)
	{
		//_snprintf(sText, MAX_PATH, "HUser0%d", i);
		//CClientUIManager::GetPtr()->FindEventControl(sText)->SetVisible(N3UI_VISIBLE_FALSE);
		//
		//_snprintf(sText, MAX_PATH, "AUser0%d", i);
		//CClientUIManager::GetPtr()->FindEventControl(sText)->SetVisible(N3UI_VISIBLE_FALSE);
		
		_snprintf(sText, MAX_PATH, "HUser0%d_Position", i);
		CClientUIManager::GetPtr()->FindEventControl(sText)->SetVisible(N3UI_VISIBLE_FALSE);

		_snprintf(sText, MAX_PATH, "AUser0%d_Position", i);
		CClientUIManager::GetPtr()->FindEventControl(sText)->SetVisible(N3UI_VISIBLE_FALSE);		
	}

	for(int i=0;i<TEAM_SIZE;i++)
	{
		// 홈 인원수에 따른 정보 및 포지션 위치 표시여부
		_snprintf(sText, MAX_PATH, "Static_UserRecord%d", i);
		CUIControl* pHome = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pHome);

		if(cReview.m_cHomePlayer[i].m_nPlayerSeq == 0)
			pHome->SetVisible(N3UI_VISIBLE_FALSE);
		else
		{
			//_snprintf(sText, MAX_PATH, "HUser0%d", cReview.m_cHomePlayer[i].m_nForm);
			//CUIControl* pGroundPosition = CClientUIManager::GetPtr()->FindEventControl(sText);
			
			//pGroundPosition->SetVisible(N3UI_VISIBLE_TRUE);
			//pGroundPosition->SetText(cReview.m_cHomePlayer[i].m_sName);

			pHome->SetVisible(N3UI_VISIBLE_TRUE);

			_snprintf(sText, MAX_PATH, "HUser0%d_Position", cReview.m_cHomePlayer[i].m_nForm);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );

			UICommonMethod::PositionIconDraw( sText, cReview.m_cHomePlayer[i].m_nPosition, UICommonMethod::HOME_TEAM );
		}		

		// 어웨이 인원수에 따른 정보 및 포지션 위치 표시여부
		_snprintf(sText, MAX_PATH, "Static_UserRecord%d", i+6);
		CUIControl* pAway = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pAway);
		
		if(cReview.m_cAwayPlayer[i].m_nPlayerSeq == 0)
			pAway->SetVisible(N3UI_VISIBLE_FALSE);			
		else
		{
			//_snprintf(sText, MAX_PATH, "AUser0%d", cReview.m_cAwayPlayer[i].m_nForm);
			//CUIControl* pGroundPosition = CClientUIManager::GetPtr()->FindEventControl(sText);
			
			//pGroundPosition->SetVisible(N3UI_VISIBLE_TRUE);
			//pGroundPosition->SetText(cReview.m_cAwayPlayer[i].m_sName);

			pAway->SetVisible(N3UI_VISIBLE_TRUE);

			_snprintf(sText, MAX_PATH, "AUser0%d_Position", cReview.m_cAwayPlayer[i].m_nForm);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );

			UICommonMethod::PositionIconDraw( sText, cReview.m_cAwayPlayer[i].m_nPosition, UICommonMethod::AWAY_TEAM );
		}

		if( (cReview.m_cHomePlayer[i].m_nPlayerSeq == 0) && (cReview.m_cAwayPlayer[i].m_nPlayerSeq == 0) )
			continue;

		// 레벨 출력
		CUIControl* pLevel = pHome->FindEventControl("Static_Level");
		assert(pLevel);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cHomePlayer[i].m_nLevel);
		pLevel->SetText(sText);

		pLevel = pAway->FindEventControl("Static_Level");
		assert(pLevel);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cAwayPlayer[i].m_nLevel);
		pLevel->SetText(sText);

		// 포지션 마크 출력
		UICommonMethod::PositionIconDraw( pHome, "Static_Position", cReview.m_cHomePlayer[i].m_nPosition, UICommonMethod::HOME_TEAM );
		UICommonMethod::PositionIconDraw( pHome, "Static_Position", cReview.m_cAwayPlayer[i].m_nPosition, UICommonMethod::AWAY_TEAM );

		// 이름 표시
		CUIControl* pName = pHome->FindEventControl("Static_Name");
		assert(pName);
		pName->SetText(cReview.m_cHomePlayer[i].m_sName);

		pName = pAway->FindEventControl("Static_Name");
		assert(pName);
		pName->SetText(cReview.m_cAwayPlayer[i].m_sName);
		
		// 득점 표시
		CUIControl* pScore = pHome->FindEventControl("Static_Points");
		assert(pScore);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cHomePlayer[i].m_nGoal);
		pScore->SetText(sText);

		pScore = pAway->FindEventControl("Static_Points");
		assert(pScore);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cAwayPlayer[i].m_nGoal);
		pScore->SetText(sText);

		// 어시스트 표시
		CUIControl* pAssist = pHome->FindEventControl("Static_Assistance");
		assert(pAssist);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cHomePlayer[i].m_nAssist);
		pAssist->SetText(sText);

		pAssist = pAway->FindEventControl("Static_Assistance");
		assert(pAssist);
		_snprintf(sText, MAX_PATH, "%d", cReview.m_cAwayPlayer[i].m_nAssist);
		pAssist->SetText(sText);

		// 평점 표시
		CUIControl* pMarkPoint = pHome->FindEventControl("Static_Marks");
		assert(pMarkPoint);
		_snprintf(sText, MAX_PATH, "%.1f", (float)cReview.m_cHomePlayer[i].m_nMark/10);
		pMarkPoint->SetText(sText);

		pMarkPoint = pAway->FindEventControl("Static_Marks");
		assert(pMarkPoint);
		_snprintf(sText, MAX_PATH, "%.1f", (float)cReview.m_cAwayPlayer[i].m_nMark/10);
		pMarkPoint->SetText(sText);
	}
}

void CReplayMainProcess::FUNCTION_UpdateScoreUI(char nHomeScore, char nAwayScore)
{
	char sText[STRLEN_64]="";
	
	//10의 자리
	_snprintf(sText, STRLEN_64, "BNum_%d", (nHomeScore/10)%10);
	CUIControl* pNumber = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pNumber);

	CUIControl* pScore = CClientUIManager::GetPtr()->FindEventControl( "Home_TenScore" );
	assert(pScore);

	pScore->SetElement(pNumber->m_pElement->Clone());

	//1의 자리
	_snprintf(sText, STRLEN_64, "BNum_%d", (nHomeScore)%10);
	pNumber = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pNumber);

	pScore = CClientUIManager::GetPtr()->FindEventControl( "Home_OneScore" );
	assert(pScore);

	pScore->SetElement(pNumber->m_pElement->Clone());


	//10의 자리
	_snprintf(sText, STRLEN_64, "BNum_%d", (nAwayScore/10)%10);
	pNumber = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pNumber);

	pScore = CClientUIManager::GetPtr()->FindEventControl( "Away_TenScore" );
	assert(pScore);

	pScore->SetElement(pNumber->m_pElement->Clone());

	//1의 자리
	_snprintf(sText, STRLEN_64, "BNum_%d", (nAwayScore)%10);
	pNumber = CClientUIManager::GetPtr()->FindEventControl( sText );
	assert(pNumber);

	pScore = CClientUIManager::GetPtr()->FindEventControl( "Away_OneScore" );
	assert(pScore);

	pScore->SetElement(pNumber->m_pElement->Clone());

	if( nAwayScore < nHomeScore )
		UIFunction::Set_ControlText( "WhoWin", "HOME WIN" );
	else
		UIFunction::Set_ControlText( "WhoWin", "AWAY WIN" );
}

void CReplayMainProcess::FUNCTION_UpdatePage(char nPage)
{
	char sText[STRLEN_64]="";

	m_nPage+=nPage;
	
	if(m_nPage<1)
		m_nPage = m_nTotalPage;
	
	if(m_nTotalPage<m_nPage)
		m_nPage = 1;
	
	FileListUpdate(m_nKind);

	_snprintf(sText, STRLEN_64, "%d/%d", m_nPage, m_nTotalPage);
	UIFunction::Set_ControlText( "ReplayPageName", sText );
}

void CReplayMainProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		ReplayListSelection();
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		ReplayListUpdate();
		break;
	}
}

void CReplayMainProcess::ReplayListUpdate()
{
	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST10_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "RList0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		RECT rt = pControl->GetWindowBox();

		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pControl->GetVisible() == N3UI_VISIBLE_TRUE)
			UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_TRUE );
		else
			UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_FALSE );
	}
}

void CReplayMainProcess::ReplayListSelection()
{
	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST10_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "RList0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);

		RECT rt = pControl->GetWindowBox();

		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pControl->GetVisible() == N3UI_VISIBLE_TRUE)
		{
			m_nSelectReplay = i;
			ReplayInfomationUpdate(m_nSelectReplay);
			UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_TRUE );
		}
		else
			UIFunction::Set_UIVisibility( pControl, "Click_Img", N3UI_VISIBLE_FALSE );
	}
}

void CReplayMainProcess::ReplayListChoice(char idx)
{
	if( idx == 10 )
		idx = m_nSelectReplay;

	if(idx == -1)
		return;

	char sText[MAX_PATH]="";

	switch(m_nKind)
	{
	case 0:
	case 1:
		_snprintf(sText, MAX_PATH, ".\\Replay\\%s", m_vFileList.at(idx).c_str());
		break;
	case 2:
		_snprintf(sText, MAX_PATH, ".\\Replay\\Download\\%s", m_vFileList.at(idx).c_str());
		break;
	}

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->m_nMode = ROOM_MODE_REPLAY;

	CProcessManager::GetPtr()->SetChangeProcess(new CReplayLoadingProcess(sText));	
}

void CReplayMainProcess::ReplayListDelete()
{
	char sText[STRLEN_64]="";

	for( int i=0;i<LIST10_SIZE;i++ )
	{
		_snprintf(sText, STRLEN_64, "ReplayDeleteChk0%d", i);
		CUICheckBox* pChk = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChk);

		if(pChk->GetChecked())
		{
			switch(m_nKind)
			{
			case 0:
			case 1:
				_snprintf(sText, MAX_PATH, ".\\Replay\\%s", m_vFileList.at(i).c_str());
				break;
			case 2:
				_snprintf(sText, MAX_PATH, ".\\Replay\\Download\\%s", m_vFileList.at(i).c_str());
				break;
			}
			remove(sText);
			pChk->SetCheckValue(false);
		}
	}

	m_nPage=1;

	FUNCTION_UpdatePage(0);
}

void CReplayMainProcess::LoadGround()
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