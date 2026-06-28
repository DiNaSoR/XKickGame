#include "stdafx.h"
#include "ChoiceRoomProcess.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "ReadyRoomProcess.h"
#include "ChoiceServerProcess.h"
#include "PlayModeManager.h"
#include "GuideTutorialProcess.h"
#include "UICommon.h"
#include "ResTableStack.h"
#include "SignBoard.h"
#include "ObjServerInfo.h"
#include "UIFunction.h"

CChoiceRoomProcess::CChoiceRoomProcess() : CBaseProcess()
{
	m_nProcessCmd	= PROCESS_CHOICEROOM;
	m_nRoomArrange	= LIST_KIND_ALL;
	m_nPopupIndex	= -1;
	m_nRoomPage		= 0;
	m_nLobbyPage	= 0;
	m_nSendTime		= 0;
	//m_nQuestCode	= 101;
	//m_nSelectQuestCode = 101;
}

CChoiceRoomProcess::~CChoiceRoomProcess(void)
{
}

void CChoiceRoomProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CChoiceRoomProcess::LoadGround()
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

void CChoiceRoomProcess::InitProcess()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->Init();

	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	FUNCTION_Init();

	//FUNCTION_IsChoiceClass();// 전직 여부 확인 후 전직창으로 이동

	PutRoomList(m_nRoomArrange, 0);
	PutLobbyList(0);

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);
	//UICommonMethod::BGEffectAnimationInit( "LobbyList_BG", "RoomList_BG" );

	m_nState = STATE_START;
}

void CChoiceRoomProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
    CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );

	UICommonMethod::DisPlayNotice();
	//UICommonMethod::BGEffectAnimationPlay( "LobbyList_BG", "RoomList_BG" );
	UICommonMethod::BasicInfoStop();

	UICommonMethod::DisplayMenuToolTip();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
	//SignBoard::GetPtr()->DisplaySignBoard();
}

void CChoiceRoomProcess::UpdateState(float fFrameTime, float fAccumTime)
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
			m_bIsRender = true;
			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
			//능력치 체크
			//CObjPlayerInfo*		pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
			//int					nTotal	= 0;
			//int					nSize	= pPlayer->m_cLevel.m_nLevel * 2;

			//nSize += (char)pPlayer->m_cLevel.m_nLevel/LIST10_SIZE;

			//for(int i=0;i<ARRAY_FACULTY_SIZE;++i)
			//{
			//	nTotal += pPlayer->m_cRaiseFaculty.m_nFaculty[i];
			//}

			//nTotal += pPlayer->m_cLevel.m_nFaculty;

			///*
			//// 조용길 - 서버에서 처리하기 위하여 임시 주석처리
			//
			//if(nTotal != nSize)
			//	CMessageBoxDialog::GetPtr()->Enable( 1, 200000+10, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, 0);
			//else
			//*/
			m_nState = STATE_PLAY;
		}
		break;
	case STATE_PLAY:
		{
			if(CPacketManager::GetPtr()->m_bIsChoiceRoomDone)
			{
				CPacketManager::GetPtr()->m_bIsChoiceRoomDone = false;
				PutRoomInfo();
				m_nState = STATE_SEND_INFO;
			}

			if(timeGetTime() - m_nSendTime > 5000)
			{
				m_nSendTime = timeGetTime();
				//printf( "RoomArrange : %d, RoomPage : %d, LobbyPage : %d\n", m_nRoomArrange, m_nRoomPage, m_nLobbyPage);
				PutRoomList(m_nRoomArrange, m_nRoomPage);
				//if( m_nLobbyTab == 3 )
				//	FUNCTION_SetQuestList();
				//else
				if( m_nLobbyTab == 2 )
					PutLobbyList(m_nLobbyPage);
			}
		}
		break;
	case STATE_SEND_INFO:
		{
			if(CPacketManager::GetPtr()->m_bIsAthleteInfoDone)
			{
				//아직 모델이 그려지지 않았으므로 true 상태로 넘긴다
				CPacketManager::GetPtr()->m_bIsAthleteInfoDone = true;
				m_nState = STATE_END;
			}
		}
		break;
	case STATE_END:
		{
			CProcessManager::GetPtr()->SetChangeProcess(new CReadyRoomProcess);
		}
		break;
	}
}

void CChoiceRoomProcess::RenderFrame()
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

bool CChoiceRoomProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
	{
		MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
	}

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CChoiceRoomProcess::GetSameProcess()
{
	return new CChoiceRoomProcess;
}

void CChoiceRoomProcess::EraseLobbyoneList()
{
	CObjectManager::GetPtr()->Terminate(OBJECT_LOBBYONELIST);
}

void CChoiceRoomProcess::FUNCTION_Init()
{
	/*CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTSERVERLIST));
	if((pServerInfo->m_nServerCode%10) == 3)
	{
		UIFunction::Set_UIVisibility( "CreateRoom_BUTTOM", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "LadderRoom_BUTTOM", N3UI_VISIBLE_TRUE );
	}
	else
	{	
		UIFunction::Set_UIVisibility( "CreateRoom_BUTTOM", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "LadderRoom_BUTTOM", N3UI_VISIBLE_FALSE );
	}*/

	//if( m_pObjPlayerInfo->m_nPosition == POSITION_NB )
	//{
	//	m_nLobbyTab = 3;
	//	CUIButton* pChannel = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("LobbyList_Tab004"));
	//	assert(pChannel);
	//	pChannel->SetPressed(true);
	//	UIFunction::Set_UIVisibility( "Btn_Blur", N3UI_VISIBLE_TRUE );
	//	UIFunction::Set_UIVisibility( "LobbyList_Tab004", N3UI_VISIBLE_TRUE );
	//}
	//else
	//{
	//	m_nLobbyTab = 0;
	//	CUIButton* pChannel = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl("LobbyList_Tab01"));
	//	assert(pChannel);
	//	pChannel->SetPressed(true);
	//	UIFunction::Set_UIVisibility( "Btn_Blur", N3UI_VISIBLE_FALSE );
	//	UIFunction::Set_UIVisibility( "LobbyList_Tab004", N3UI_VISIBLE_FALSE );
	//}

	m_nLobbyTab = 2;

	FUNCTION_ChangeTab(m_nLobbyTab);

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);

	for(int i=0;i<LIST5_SIZE;++i)
		m_cRoomList.m_cRoomData[i].m_nState = ROOM_STATE_EMPTY;

	for(int i=0;i<LIST10_SIZE;++i)
		m_cLobbyList.m_cLobbyData[i].m_nState = LOBBY_STATE_EMPTY;

	//InitQuestCode();
}

//void CChoiceRoomProcess::InitQuestCode()
//{
//	CResQuestTableStack *temp = dynamic_cast<CResQuestTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_QUESTTABLE));
//	assert(temp);
//
//	for(int i=0;i<temp->GetResourceSize();i++)
//	{
//		CResQuestTableFormat *pQuestTable = dynamic_cast<CResQuestTableFormat*>(temp->GetSequenceField(i));
//		assert(pQuestTable);
//
//		int nMyLevel = m_pObjPlayerInfo->m_cLevel.m_nLevel;
//
//		if( pQuestTable->m_nStart_Level <= nMyLevel && nMyLevel <= pQuestTable->m_nEnd_Level )
//            m_vQuestCodeList.push_back(pQuestTable->GetObjCode());
//	}
//}

void CChoiceRoomProcess::SetRoomList(CSCRoomList* pList)
{
	char sText[STRLEN_64]="";

	for(int i = 0; i < LIST5_SIZE; i++)
	{
		// 일반 룸리스트 초기화
		_snprintf(sText, STRLEN_64, "Room%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pControl);
		pControl->SetVisible(N3UI_VISIBLE_FALSE);

		//// 퀘스트 룸리스트 초기화
		//_snprintf(sText, MAX_PATH, "QuestRoom%d", i);
		//pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		//assert(pControl);
		//pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}
	
	if(pList->m_nPage==-1)
		return;

	m_nRoomPage=pList->m_nPage;
	_snprintf(sText, STRLEN_64, "%d",m_nRoomPage+1 );
	UIFunction::Set_ControlText( "RoomPage", sText );
	memcpy(&m_cRoomList, pList, sizeof(CSCRoomList));

	for(int i = 0; i < LIST5_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "Room%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		if(pControl == NULL) return;
			
		switch( pList->m_cRoomData[i].m_nMode )
		{
		case ROOM_MODE_QUEST:
		case ROOM_MODE_TUTORIAL:
			UIFunction::SetChildToVisibility( pControl, "PUBLIC_QuestROOM_IMAGE", N3UI_VISIBLE_TRUE );
		case ROOM_MODE_NORMAL:
		case ROOM_MODE_LADDER:
		case ROOM_MODE_BET:
			{
				if(pList->m_cRoomData[i].m_nState != ROOM_STATE_EMPTY)
				{
					FUNCTION_RoomBasicInfo(pList, i);

					for(int pos = 0; pos < ROOM_SCALE_6; pos++)
					{
						_snprintf(sText, STRLEN_64, "CHOICEROOM_ROOMHPOSITION%d_IMAGEAREA", pos + 1);

						if(pList->m_cRoomData[i].m_cHomeSeat.m_nUsingPosition[pos] > 0)
							UICommonMethod::PositionIconDraw( pControl, sText, pList->m_cRoomData[i].m_cHomeSeat.m_nUsingPosition[pos], UICommonMethod::HOME_TEAM );
						else 
							UICommonMethod::PositionIconDraw( pControl, sText, pList->m_cRoomData[i].m_cHomeSeat.m_nReservePosition[pos], UICommonMethod::AWAY_TEAM );

						_snprintf(sText, STRLEN_64, "CHOICEROOM_ROOMAPOSITION%d_IMAGEAREA", pos + 1);

						if(pList->m_cRoomData[i].m_cAwaySeat.m_nUsingPosition[pos] > 0)
							UICommonMethod::PositionIconDraw( pControl, sText, pList->m_cRoomData[i].m_cAwaySeat.m_nUsingPosition[pos], UICommonMethod::HOME_TEAM );
						else
							UICommonMethod::PositionIconDraw( pControl, sText, pList->m_cRoomData[i].m_cAwaySeat.m_nReservePosition[pos], UICommonMethod::AWAY_TEAM );
					}
				}
			}
			break;
		case ROOM_MODE_TRAINING:
		case ROOM_MODE_TOURNAMENT:
		case ROOM_MODE_REPLAY:
			return;
		}
	}
}

void CChoiceRoomProcess::FUNCTION_RoomBasicInfo(CSCRoomList* pList, int i)
{
	char sText[MAX_PATH]="";

	CUIControl* pControl = NULL;

	switch(pList->m_cRoomData[i].m_nMode)
	{
	case ROOM_MODE_LADDER:
	case ROOM_MODE_NORMAL:
	case ROOM_MODE_BET:
	case ROOM_MODE_QUEST:
		{
			_snprintf(sText, MAX_PATH, "Room%d", i);
			pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pControl);

			pControl->SetVisible(N3UI_VISIBLE_TRUE);
		}
		break;
	case ROOM_MODE_TUTORIAL:
	case ROOM_MODE_TRAINING:
	case ROOM_MODE_TOURNAMENT:
	case ROOM_MODE_REPLAY:
		return;
	}

	int RoomNum = pList->m_cRoomData[i].m_nRoomSeq;

	// 방제 표시
	CUIControl* pRoomNameText = pControl->FindEventControl("CHOICEROOM_ROOMNAME_TEXTAREA");
	if(pRoomNameText == NULL) return;
	pRoomNameText->SetText(pList->m_cRoomData[i].m_sTitle);

	//100의 자리
	_snprintf(sText, STRLEN_64, "Num_%d", (RoomNum/100)%10);
	CUIControl* pHundredPlace = pControl->FindEventControl("CHOICEROOM_HUNDREDNM_IMAGEAREA");
	if(pHundredPlace == NULL) return;
	pHundredPlace->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
	//10의 자리
	_snprintf(sText, STRLEN_64, "Num_%d", (RoomNum/10)%10);
	CUIControl* pTenPlace = pControl->FindEventControl("CHOICEROOM_TENNM_IMAGEAREA");
	if(pTenPlace == NULL) return;
	pTenPlace->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());
	//1의 자리
	_snprintf(sText, STRLEN_64, "Num_%d", (RoomNum/1)%10);
	CUIControl* pOnePlace = pControl->FindEventControl("CHOICEROOM_ONENM_IMAGEAREA");
	if(pOnePlace == NULL) return;
	pOnePlace->SetElement((CClientUIManager::GetPtr()->FindEventControl(sText))->m_pElement->Clone());

	// 방 상태(PLAY, WAIT) 아이콘 표시
	if(pList->m_cRoomData[i].m_nCource == ROOM_COURCE_NONE)
	{
		UIFunction::Set_UIVisibility( pControl, "Waiting_Icon", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( pControl, "Playing_Icon", N3UI_VISIBLE_FALSE );
		
		pControl->FindEventControl("PlayRoom")->SetVisible(N3UI_VISIBLE_FALSE);
	}
	else
	{
		UIFunction::Set_UIVisibility( pControl, "Waiting_Icon", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( pControl, "Playing_Icon", N3UI_VISIBLE_TRUE );

		pControl->FindEventControl("PlayRoom")->SetVisible(N3UI_VISIBLE_TRUE);
	}

	// 비밀방 여부 아이콘 표시
	CUIControl* pRoomState = pControl->FindEventControl("PUBLIC_PASSWORDROOM2_IMAGE");
	if(pList->m_cRoomData[i].m_nState == ROOM_STATE_SECRET)
	{
		CUIControl* pSecret = CClientUIManager::GetPtr()->FindEventControl("SecretOn");
		assert(pSecret);
		pRoomState->SetElement(pSecret->m_pElement->Clone());
	}
	else
	{
		CUIControl* pNormal = CClientUIManager::GetPtr()->FindEventControl("SecretOff");
		assert(pNormal);
		pRoomState->SetElement(pNormal->m_pElement->Clone());
	}

	// 인원표시
	CUIControl* pRoomScale = pControl->FindEventControl("PUBLEC_ROOMMEMBER5NUM2_IMAGE");
	if(pList->m_cRoomData[i].m_nScaleCode == ROOM_SCALE_5)
	{
		CUIControl* pScale5 = CClientUIManager::GetPtr()->FindEventControl("RoomSize_5");
		assert(pScale5);
		pRoomScale->SetElement(pScale5->m_pElement->Clone());
	}
	else if(pList->m_cRoomData[i].m_nScaleCode == ROOM_SCALE_6)
	{
		CUIControl* pScale6 = CClientUIManager::GetPtr()->FindEventControl("RoomSize_6");
		assert(pScale6);
		pRoomScale->SetElement(pScale6->m_pElement->Clone());
	}

	// CLUB전 표시
	CUIControl* pRoomClub = pControl->FindEventControl("PUBLIC_ROOMMODECLUB2_IMAGE");
	if(pList->m_cRoomData[i].m_nCheckClub)
	{
		CUIControl* pClubT = CClientUIManager::GetPtr()->FindEventControl("ClubOn");
		assert(pClubT);
		pRoomClub->SetElement(pClubT->m_pElement->Clone());
	}
	else
	{
		CUIControl* pClubF = CClientUIManager::GetPtr()->FindEventControl("ClubOff");
		assert(pClubF);
		pRoomClub->SetElement(pClubF->m_pElement->Clone());
	}

	// AI표시 AIPLAYER2_IMAGE
	CUIControl* pRoomAI = pControl->FindEventControl("PUBLIC_AIPLAYER2_IMAGE");
	CUIControl* pAI = NULL;
	switch(pList->m_cRoomData[i].m_nAICode)
	{
	case 0:
		pAI = CClientUIManager::GetPtr()->FindEventControl("AiOff");
		assert(pAI);
		pRoomAI->SetElement(pAI->m_pElement->Clone());
		break;
	case 1:
		pAI = CClientUIManager::GetPtr()->FindEventControl("AiAllOn");
		assert(pAI);
		pRoomAI->SetElement(pAI->m_pElement->Clone());
		break;
	case 2:
		pAI = CClientUIManager::GetPtr()->FindEventControl("AiKeepOn");
		assert(pAI);
		pRoomAI->SetElement(pAI->m_pElement->Clone());
		break;
	}

	CUIControl* pRD = pControl->FindEventControl( "RD_Static" );
	assert(pRD);

	// 레벨 범위 표시
	CUIControl* pLevelScopeText = pRD->FindEventControl("CHOICEROOM_ROOMLVLIMIT_TEXTAREA");
	assert(pLevelScopeText);

	if( pList->m_cRoomData[i].m_nMode == ROOM_MODE_BET )
	{
		_snprintf(sText, STRLEN_8, "%d", pList->m_cRoomData[i].m_nPointCode);
		pLevelScopeText->SetText(sText);
	}
	else
	{
		_snprintf(sText, STRLEN_8, "%d~%d", pList->m_cRoomData[i].m_nStartLevel,pList->m_cRoomData[i].m_nEndLevel);
		pLevelScopeText->SetText(sText);
	}

	// 플레이어 수 표시
	CUIControl* pNumberOfPersonText = pRD->FindEventControl("CHOICEROOM_ROOMPLAYERNUMLIMIT_TEXTAREA");
	assert(pNumberOfPersonText);

	_snprintf(sText, STRLEN_64, "%d/%d",pList->m_cRoomData[i].m_nAthleteCount,pList->m_cRoomData[i].m_nMaxCount);
	pNumberOfPersonText->SetText(sText);

	// 관전자 수 표시
	CUIControl* pNumberOfViewText = pRD->FindEventControl("CHOICEROOM_ROOMPLOBNUMLIMIT_TEXTAREA");
	assert(pNumberOfViewText);

	_snprintf(sText, STRLEN_64, "%d/4",pList->m_cRoomData[i].m_nViewCount);
	pNumberOfViewText->SetText(sText);
}

void CChoiceRoomProcess::SetLobbyList(CSCLobbyList* pList)
{
	if(pList->m_nPage==-1)
		return;
	m_nLobbyPage=pList->m_nPage;
	memcpy(&m_cLobbyList, pList, sizeof(CSCLobbyList));
	
	char sLobbyPage[STRLEN_8];
	_snprintf(sLobbyPage, STRLEN_8, "%d", m_nLobbyPage+1 );
	UIFunction::Set_ControlText( "LobbyPage", sLobbyPage );

	char sText[STRLEN_64]="";
	
	for(int i = 0; i < LIST10_SIZE; i++)
	{
		_snprintf(sText, STRLEN_64, "LobbyList0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		if(pControl == NULL) return;
		if(pList->m_cLobbyData[i].m_nState != LOBBY_STATE_EMPTY)
		{
			pControl->SetVisible(N3UI_VISIBLE_TRUE);
			
			UIFunction::Set_ChildControlText( pControl, "CharacterName", pList->m_cLobbyData[i].m_sName );

			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, pList->m_cLobbyData[i].m_nLevel);
			UIFunction::Set_ChildControlText( pControl, "Lv", sText );

			UICommonMethod::PositionIconDraw( pControl, "PositionIcon", pList->m_cLobbyData[i].m_nPosition, UICommonMethod::HOME_TEAM );

			UIFunction::Set_ChildControlText( pControl, "Ment", pList->m_cLobbyData[i].m_sMent );
		}
		else
		{
			if(pControl)
				pControl->SetVisible(N3UI_VISIBLE_FALSE);
		}
	}
}

void CChoiceRoomProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_RBUTTONDOWN:
		FUNCTION_ChoiceUserListUIPopup();
		break;
	case WM_MOUSEMOVE:
		
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "LobbyList_PopUp" );
		assert(pControl);
		
		RECT rt;
		rt = pControl->GetWindowBox();

		if(!UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint))
			pControl->SetVisible(N3UI_VISIBLE_FALSE);

		FUNCTION_ChoiceUserListUI();
	
		FUNCTION_ChoiceRoomListUI();

		break;
	}
}

void CChoiceRoomProcess::FUNCTION_ChoiceRoomListUI()
{
	RECT rt;

	for(int i = 0; i < LIST5_SIZE; i++)
	{
		if(m_cRoomList.m_cRoomData[i].m_nState != LOBBY_STATE_EMPTY)
		{
			char sText[STRLEN_64]="";
			_snprintf(sText, STRLEN_64, "Room%d", i);
			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pControl);

			CUIControl* pBlur = pControl->FindEventControl( "PlayRoom" );
			assert(pBlur);

			_snprintf(sText, STRLEN_64, "Room%dButton", i);
			rt = pControl->GetWindowBox();
			if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) && pBlur->GetVisible() == N3UI_VISIBLE_FALSE )
				UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_TRUE );		
			else
				UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_FALSE );
		}
	}
}

void CChoiceRoomProcess::FUNCTION_ChangeTab(char idx)
{
	char sText[STRLEN_64]="";
	m_nLobbyTab = idx;

	for(int i = 0; i < LIST10_SIZE; i++)
	{
		_snprintf(sText, MAX_PATH, "LobbyList0%d", i);
		CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	}
		
	for(int i=0;i<3;i++)
	{
		//if( m_pObjPlayerInfo->m_nPosition == POSITION_NB && i==3)
		//	sprintf(sText, "LobbyList_Tab00%d", i+1);
		//else
			_snprintf(sText, STRLEN_64, "LobbyList_Tab0%d", i);

		CUIButton* pChannel = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChannel);
		
		if(i==m_nLobbyTab)
			pChannel->SetPressed(true);
		else
			pChannel->SetPressed(false);
	}

	//UIFunction::Set_UIVisibility( "CHOICEROOM_QUESTLIST_IMAGE", N3UI_VISIBLE_FALSE );
	
	switch(m_nLobbyTab)
	{
	case 1:
		m_nLobbyPage=0;
		//PutBuddyList(m_nLobbyPage);
		break;
	case 0:
		m_nLobbyPage=0;
		//PutClubList(m_nLobbyPage);
		break;
	case 2:
		m_nLobbyPage=0;
		PutLobbyList(m_nLobbyPage);
		break;
	/*case 1:*/
	//case 2:
	//	{
	//		for(int i = 0; i < LIST10_SIZE; i++)
	//		{
	//			_snprintf(sText, MAX_PATH, "CHOICEROOM_USERLISTSELECT%d_BG", i);
	//			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
	//			pControl->SetVisible(N3UI_VISIBLE_FALSE);
	//		}
	//	}
	//	break;
	//case 3:
	//	m_nLobbyPage=0;
	//	UIFunction::Set_UIVisibility( "CHOICEROOM_QUESTLIST_IMAGE", N3UI_VISIBLE_TRUE );
	//	FUNCTION_SetQuestList();
		break;
	}
}

void CChoiceRoomProcess::FUNCTION_ChoiceUserListUI()
{
	for(int i = 0; i < LIST10_SIZE; i++)
	{
		if(m_cLobbyList.m_cLobbyData[i].m_nState != LOBBY_STATE_EMPTY)
		{
			RECT rt;
			char sText[STRLEN_64]="";

			_snprintf(sText, STRLEN_64, "LobbyList0%d", i);
			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pControl);

			rt = pControl->GetWindowBox();
		
			if( UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) )
				UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_TRUE );
			else
				UIFunction::Set_UIVisibility( pControl, "Over_img", N3UI_VISIBLE_FALSE);
		}
	}
}

void CChoiceRoomProcess::FUNCTION_ChoiceUserListUIPopup()
{
	for(int i = 0; i < LIST10_SIZE; i++)
	{
		if(m_cLobbyList.m_cLobbyData[i].m_nState != LOBBY_STATE_EMPTY)
		{
			RECT rt;
			char sText[STRLEN_64]="";

			_snprintf(sText, STRLEN_64, "LobbyList0%d", i);
			CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
			assert(pControl);

			rt = pControl->GetWindowBox();
		
			if( UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint) )
			{
				m_nPopupIndex = i;

				_snprintf(sText, STRLEN_64, "LobbyList0%d", i);
				CUIControl* pList = CClientUIManager::GetPtr()->FindEventControl( "LobbyList_PopUp" );
				assert(pControl);
				
				pList->SetVisible(N3UI_VISIBLE_TRUE);
				pList->SetLocation(m_pDNDPoint.x-50, m_pDNDPoint.y-10, true);
				break;
			}
		}
	}
}

//void CChoiceRoomProcess::FUNCTION_SetQuestList()
//{
//	char sText[MAX_PATH]="";
//
//	char sLobbyPage[STRLEN_8];
//	_snprintf( sLobbyPage, STRLEN_8, "%d", m_nLobbyPage+1 );
//	UIFunction::Set_ControlText( "LobbyPage", sLobbyPage );
//
//	for(int i=0;i<LIST5_SIZE;i++)
//	{
//		_snprintf(sText, MAX_PATH, "CHOICEROOM_QUESTLISTSELECT%d_BG", i);
//		CUIControl* pQuestList = CClientUIManager::GetPtr()->FindEventControl(sText);
//		assert(pQuestList);
//		pQuestList->SetVisible(N3UI_VISIBLE_FALSE);
//
//		int nQuestCode = 0;
//
//		if( (i+(m_nLobbyPage*LIST5_SIZE)) < (int)m_vQuestCodeList.size() )
//			nQuestCode = m_vQuestCodeList.at(i+(m_nLobbyPage*LIST5_SIZE));
//		else
//			continue;
//
//		CResQuestTableFormat * pQuestTable = dynamic_cast<CResQuestTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, nQuestCode, 0));
//		if(pQuestTable == NULL) 
//			continue;
//		else
//			pQuestList->SetVisible(N3UI_VISIBLE_TRUE);
//
//		CUIControl* pQuestComplete = pQuestList->FindEventControl( "Complete_Icon" );
//		assert(pQuestComplete);
//
//		pQuestComplete->SetVisible(N3UI_VISIBLE_FALSE);
//
//		for(int i=0;i<(int)m_pObjPlayerInfo->m_vQuestList.size();i++)
//		{
//			if( m_pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == nQuestCode )
//			{
//				pQuestComplete->SetVisible(N3UI_VISIBLE_TRUE);
//				break;
//			}
//		}
//
//		CUIControl* pQuestIcon = pQuestList->FindEventControl("QuestIcon");
//		assert(pQuestIcon);
//		CUIControl* pPosition = CClientUIManager::GetPtr()->FindEventControl(pQuestTable->m_sIconFile);
//		assert(pPosition);
//		
//		pQuestIcon->SetElement(pPosition->m_pElement->Clone());
//
//		CUIControl* pPositionMark = pQuestList->FindEventControl( "PositionIconP" );
//		assert(pPositionMark);
//		
//		_snprintf(sText, MAX_PATH, "PUBLIC_APOSITION_IMAGE%d", pQuestTable->m_nPosition);
//		CUIControl* pCPM = CClientUIManager::GetPtr()->FindEventControl( sText );
//		assert(pCPM);
//		
//		pPositionMark->SetElement(pCPM->m_pElement->Clone());		
//
//		CUIControl* pQuestTitle = pQuestList->FindEventControl("Q_Title");
//		assert(pQuestTitle);
//		pQuestTitle->SetText(pQuestTable->m_sTitle);
//
//        CUIControl* pQuestKind = pQuestList->FindEventControl("Q_Kind");
//		assert(pQuestKind);
//
//		CResMessageTableFormat* pMTable = NULL;
//
//		switch(pQuestTable->m_nKind)
//		{
//		case 0:
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900084, 0));
//			assert(pMTable);
//			pQuestKind->SetText(pMTable->m_sMessage);
//			break;
//		case 1:
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900085, 0));
//			assert(pMTable);
//			pQuestKind->SetText(pMTable->m_sMessage);
//			break;
//		case 2:
//			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900086, 0));
//			assert(pMTable);
//			pQuestKind->SetText(pMTable->m_sMessage);
//			break;
//		}		
//
//		CUIControl* pQuestLv = pQuestList->FindEventControl("Q_Lv");
//		assert(pQuestLv);
//		_snprintf(sText, MAX_PATH, "%d - %d", pQuestTable->m_nStart_Level, pQuestTable->m_nEnd_Level);
//		pQuestLv->SetText(sText);
//	}
//}
//
void CChoiceRoomProcess::FUNCTION_CreateQuest()
{
	CResQuestTableFormat * pQuestTable = dynamic_cast<CResQuestTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, 10000, 0));
	assert(pQuestTable);

	switch(pQuestTable->m_nKind)
	{
	case 0:
		switch(pQuestTable->GetObjCode())
		{
		case 101:
			CProcessManager::GetPtr()->SetChangeProcess(new CGuideTutorialProcess(14));
		}
		break;
	case 1:
	case 2:
		{
			CCSCreateQuest cCreateQuest;

			cCreateQuest.m_nState = ROOM_STATE_NORMAL;    
			cCreateQuest.m_nMode  = ROOM_MODE_QUEST;

			_snprintf( cCreateQuest.m_sTitle, TITLE_NAME_SIZE, "%s", pQuestTable->m_sTitle );
			cCreateQuest.m_nQuestCode   	= 10000;
			cCreateQuest.m_nAttackCode  	= ROOM_ATTACK_HOME;
			cCreateQuest.m_nScaleCode   	= pQuestTable->m_nScale;
			cCreateQuest.m_nAICode			= ROOM_AI_NONE;
			cCreateQuest.m_nStartLevel  	= pQuestTable->m_nStart_Level;
			cCreateQuest.m_nEndLevel    	= pQuestTable->m_nEnd_Level;

			cCreateQuest.m_nCheckClub		= 0;
			cCreateQuest.m_nCheckTime		= 0;
			cCreateQuest.m_nCheckWeather	= 0;
			cCreateQuest.m_nCheckView		= 0;
			cCreateQuest.m_nCheckViewChat	= 0;
			cCreateQuest.m_nMaxCount		= pQuestTable->m_nJoin;

			memcpy( &cCreateQuest.m_nHomePosition[0], &pQuestTable->m_nHomePosition[0], sizeof( pQuestTable->m_nHomePosition));
			memcpy( &cCreateQuest.m_nAwayPosition[0], &pQuestTable->m_nAwayPosition[0], sizeof( pQuestTable->m_nAwayPosition));

			PutCreateQuest( &cCreateQuest );
		}
		break;
	}
}

void CChoiceRoomProcess::FUNCTION_RoomPageStart()
{
	PutRoomList(LIST_KIND_ALL, 0);
}
void CChoiceRoomProcess::FUNCTION_RoomPagePrv()
{
	if(m_nRoomPage>0)
	{
		PutRoomList(LIST_KIND_ALL, m_nRoomPage-1);
	}
}
void CChoiceRoomProcess::FUNCTION_RoomPageNext()
{
	PutRoomList(LIST_KIND_ALL, m_nRoomPage+1);
}
void CChoiceRoomProcess::FUNCTION_RoomPage3Next()
{
	PutRoomList(LIST_KIND_ALL, m_nRoomPage+3);
}

void CChoiceRoomProcess::FUNCTION_LobbyPageStart()
{
	switch(m_nLobbyTab)
	{
	case 0:
	case 1:
	case 2:
		PutLobbyList(0);
		break;
	//case 3:
	//	m_nLobbyPage = 0;
	//	FUNCTION_SetQuestList();
	//	break;
	}	
}
void CChoiceRoomProcess::FUNCTION_LobbyPagePrv()
{
	switch(m_nLobbyTab)
	{
	case 0:
	case 1:
	case 2:
		if(m_nLobbyPage>0)
			PutLobbyList(m_nLobbyPage-1);
		break;
	//case 3:
	//	m_nLobbyPage--;
	//	if(m_nLobbyPage<0)
	//	{	
	//		m_nLobbyPage = (m_vQuestCodeList.size()/LIST5_SIZE);

	//		if( (m_vQuestCodeList.size()%LIST5_SIZE) == 0 )
	//			m_nLobbyPage--;
	//	}
	//	FUNCTION_SetQuestList();
		//break;
	}
}
void CChoiceRoomProcess::FUNCTION_LobbyPageNext()
{
	switch(m_nLobbyTab)
	{
	case 0:
	case 1:
	case 2:
		PutLobbyList(m_nLobbyPage+1);
		break;
	case 3:
		{
			m_nLobbyPage++;

			int nMaxPage = (m_vQuestCodeList.size()/LIST5_SIZE);

			if( (m_vQuestCodeList.size()%LIST5_SIZE) == 0 )
				nMaxPage--;

			if(nMaxPage<m_nLobbyPage)
				m_nLobbyPage=0;
		}
		//FUNCTION_SetQuestList();
		break;
	}	
}

void CChoiceRoomProcess::FUNCTION_LobbyPage3Next()
{
	switch(m_nLobbyTab)
	{
	case 0:
	case 1:
	case 2:
		PutLobbyList(m_nLobbyPage+3);
		break;
	case 3:
		{
			m_nLobbyPage+=3;

			int nMaxPage = (m_vQuestCodeList.size()/LIST5_SIZE);

			if( (m_vQuestCodeList.size()%LIST5_SIZE) == 0 )
				nMaxPage--;

			if(nMaxPage<m_nLobbyPage)
				m_nLobbyPage=nMaxPage;
		}
		//FUNCTION_SetQuestList();
		break;
	}	
}

bool CChoiceRoomProcess::FUNCTION_ChoiceRoomIsSecret(const DWORD& nIndex)
{
	if(m_cRoomList.m_cRoomData[nIndex%10].m_nState == ROOM_STATE_SECRET)
		return true;
	else
		return false;
}

void CChoiceRoomProcess::FUNCTION_ChoiceRoomConfirmPass(const DWORD& nIndex)
{
	CCSChoiceRoom	cChoiceRoom;
	std::string 	sText;

	cChoiceRoom.m_nRoomSeq	= m_cRoomList.m_cRoomData[nIndex].m_nRoomSeq;
	cChoiceRoom.m_nType		= 0;

	CUIEditBox* pEditBox = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("Room_Password"));
	assert(pEditBox);
	if(pEditBox == NULL) return;
	sText = pEditBox->GetTextEx();
	_snprintf(cChoiceRoom.m_sPass, 5, "%s", sText.c_str() );
	PutChoiceRoom(&cChoiceRoom);
}

void CChoiceRoomProcess::EnterRoom(const DWORD& nIndex)
{
	CCSChoiceRoom	cChoiceRoom;

	cChoiceRoom.m_nRoomSeq	= m_cRoomList.m_cRoomData[nIndex%10].m_nRoomSeq;

	if( m_cRoomList.m_cRoomData[nIndex%10].m_nMode == ROOM_MODE_BET && m_pObjPlayerInfo->m_cLevel.m_nLevel < 20 )
	{
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 220517, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, 0);
		return;
	}
	
	if(nIndex>=10)
		cChoiceRoom.m_nType		= 1;		// 관전 입장
	else
		cChoiceRoom.m_nType		= 0;		// 일반 입장
	PutChoiceRoom(&cChoiceRoom);
}

void CChoiceRoomProcess::EnterLadderRoom()
{	
	CCSCreateRoom		cPacket;

	_snprintf(cPacket.m_sTitle, TITLE_NAME_SIZE, "%s", "" );
	_snprintf(cPacket.m_sPass, 5, "%s", "" );
	cPacket.m_nState			= ROOM_STATE_NORMAL;
	cPacket.m_nMode				= ROOM_MODE_LADDER;
	cPacket.m_nAttackCode		= ROOM_ATTACK_RANDOM;
	cPacket.m_nScaleCode		= ROOM_SCALE_5;
	cPacket.m_nAICode			= ROOM_AI_ALL;
	cPacket.m_nPointCode		= 0;
	cPacket.m_nStartLevel		= 1;
	cPacket.m_nEndLevel			= 60;

	cPacket.m_nCheckClub		= false;
	cPacket.m_nCheckTime		= true;
	cPacket.m_nCheckWeather		= true;
	cPacket.m_nCheckViewChat	= false;
	cPacket.m_nCheckView		= false;
	cPacket.m_nMaxCount			= 2;

	cPacket.m_nHomePosition[0]	= cPacket.m_nAwayPosition[0] = POSITION_FW;
	cPacket.m_nHomePosition[1]	= cPacket.m_nAwayPosition[1] = POSITION_MF;
	cPacket.m_nHomePosition[2]	= cPacket.m_nAwayPosition[2] = POSITION_DF;
	cPacket.m_nHomePosition[3]	= cPacket.m_nAwayPosition[3] = POSITION_ALL;
	cPacket.m_nHomePosition[4]	= cPacket.m_nAwayPosition[4] = POSITION_NONE;
	cPacket.m_nHomePosition[5]	= cPacket.m_nAwayPosition[5] = POSITION_GK;

	PutCreateRoom(&cPacket);
}

//void CChoiceRoomProcess::FUNCTION_IsChoiceClass()
//{
//	if((m_pObjPlayerInfo->m_nPosition == POSITION_NB) && (m_pObjPlayerInfo->m_cLevel.m_nLevel >= 10))	// 1차 전직
//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_CHOICECLASS, 20900, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
//
//	if((m_pObjPlayerInfo->m_nPosition%10)==0 && (m_pObjPlayerInfo->m_cLevel.m_nLevel >= 20))			// 2차 전직
//		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_CHOICECLASS, 20900, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
//}


//void CChoiceRoomProcess::FUNCTION_QuestListOverCheck()
//{
//	char sText[MAX_PATH]="";
//
//	CUIControl* pOver = CClientUIManager::GetPtr()->FindEventControl( "QuestList_Over" );
//	assert(pOver);
//
//	CUIControl* pNormal = CClientUIManager::GetPtr()->FindEventControl( "QuestList_Normal" );
//	assert(pNormal);
//
//	for(int i=0;i<LIST5_SIZE;i++)
//	{
//		_snprintf(sText, MAX_PATH, "QuestList0%d", i);
//		CUIControl* pButton = CClientUIManager::GetPtr()->GetPtr()->FindEventControl( sText );
//		assert(pButton);
//
//		_snprintf(sText, MAX_PATH, "CHOICEROOM_QUESTLISTSELECT%d_BG", i);
//		CUIControl* pBg = CClientUIManager::GetPtr()->GetPtr()->FindEventControl( sText );
//		assert(pBg);
//
//		if(pButton == CUIControl::Get_MouseOveredControl() || m_nSelectQuestCode == i+101+(m_nLobbyPage*LIST5_SIZE))
//			pBg->SetElement(pOver->m_pElement->Clone());
//		else
//			pBg->SetElement(pNormal->m_pElement->Clone());
//	}
//}
//
//void CChoiceRoomProcess::FUNCTION_CreateQuestInfo()
//{
//	ViewQuestDetail( m_nQuestCode, true );
//}