#include "stdafx.h"
#include "GameResultProcess.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "ReadyRoomProcess.h"
#include "ChoiceRoomProcess.h"
#include "GameResultDialog.h"
#include "TutorialMainProcess.h"
#include "PlayModeManager.h"
#include "UICommon.h"
#include "SignBoard.h"
#include "UIFunction.h"

void CMatchRecord::SetVariablePtr(CUIControl** _pVariableAddress)
{
	cMarks				=_pVariableAddress[0];
	cAssistance			=_pVariableAddress[1];
	cCutting			=_pVariableAddress[2];
	cEffactiveShooting	=_pVariableAddress[3];
	cEffactiveStill		=_pVariableAddress[4];
	cEffactiveTackle	=_pVariableAddress[5];
}

void CTeamMatchRecord::SetVariablePtr(CUIControl** _pVariableAddress)
{
	CMatchRecord::SetVariablePtr(_pVariableAddress);
	cBallShare	=_pVariableAddress[6];
}

void CPersonalMatchRecord::SetVariablePtr(CUIControl** _pVariableAddress)
{
	CMatchRecord::SetVariablePtr(_pVariableAddress+4);
	cBoldBack		=_pVariableAddress[0];
	cLevel			=_pVariableAddress[1];
	cPosition		=_pVariableAddress[2];
	cName			=_pVariableAddress[3];
	cExamination	=_pVariableAddress[11];
	cGainedExp		=_pVariableAddress[12];
	cGainedMileage	=_pVariableAddress[13];
	cGainedPromotion=_pVariableAddress[14];
}

CGameResultProcess::CGameResultProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_GAMERESULT;

	m_pObjPlayerInfo = NULL;

	//m_bOccupation = false;
}

CGameResultProcess::~CGameResultProcess(void)
{
	m_vPosition.clear();
	m_vLevel.clear();
	m_vRank.clear();
}

void CGameResultProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CGameResultProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CPlayModeManager::GetPtr()->LoadFile(".\\Script\\GameScript.xml");
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);
	
	Init();

	FUNCTION_3DRender( "GAMERESULT_STATESFACIAL_WINDOW" , pObjGroundInfo->GetObjectCamera());

	CGameProject::s_pGameProject->ShowPointer();

	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	FUNCTION_InitCardResult();

	srand(time(NULL));

	if( pRoom->m_bRecvCard )
		FUNCTION_CardResult();

	m_nState = STATE_START;
}

void CGameResultProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	Update(fFrameTime, fAccumTime);

	//UICommonMethod::DisPlayNotice();
	UICommonMethod::BasicInfoStop();
//	SignBoard::GetPtr()->DisplaySignBoard();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
}

void CGameResultProcess::LoadGround()
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

void CGameResultProcess::LoadCharacter()
{
	CPlayModeManager::GetPtr()->SetupCharacterLoading(m_pObjPlayerInfo, 0, 0);

	m_pObjPlayerInfo->m_cNodePoint = NiPoint3(10000,0,0);

	m_pObjPlayerInfo->m_cCurrentAction.m_fStartAngle = 180.f;

	//AI카드를 만들기 위해 테이블 로딩후 옷입기
	//CResTempCostumeTableFormat* pTempCostume = dynamic_cast<CResTempCostumeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPCOSTUMETABLE, nCode, 0));

	//pTempCostume->SetValue(m_pObjPlayerInfo->m_nEquipWear);

	CProcessManager::GetPtr()->AttachItem(m_pObjPlayerInfo);

	CObjectModelManager::GetPtr()->m_pBallNode = NULL;

	m_pObjPlayerInfo->m_cCurrentAction.m_nAniCode = m_nAnimationID;
}

void CGameResultProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_INIT_LOAD;
			nDisplayTime=0;
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
			//static DWORD DTime = 0;//에니를 정지시키기 위해 모든에니를 스텐드로 변경
			//static bool bTime = false;
			//static int fff = 39;

			//if(fff < 63)
			//{
			//	if(!bTime && timeGetTime() - DTime > 1000)
			//	{
			//		DTime = timeGetTime();
			//		if(m_pObjPlayerInfo->m_pModel)
			//			CObjectModelManager::GetPtr()->DeleteModelObject(m_pObjPlayerInfo->m_pModel);
			//		LoadGround();
			//		LoadCharacter(fff);
			//		bTime = true;
			//		fff++;
			//	}
			//	else if(bTime && timeGetTime() - DTime > 1000)
			//	{
			//		DTime = timeGetTime();
			//		CGameProject::s_pGameProject->m_bSaveScreenShot = true;
			//		bTime = false;
			//	}
			//}

			LoadCharacter();

			nDisplayTime = timeGetTime();
			m_bIsRender = true;

			m_nState = STATE_RESULT_PLAY;
		}
		break;
	case STATE_RESULT_PLAY:
		{
			CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
			if(pRoom == NULL) return;
			if( pRoom->m_bRecvCard )
				FUNCTION_CardUpdate();

			if(timeGetTime() - nDisplayTime > 8000)
			{
				if(pRoom->IsMeParent())
				{
					nDisplayTime = timeGetTime();
					PutGameEnd();
					/*if(pRoom->m_nMode != ROOM_MODE_QUEST)
						PutGameEnd();*/
				}
				/*if(pRoom->m_nMode == ROOM_MODE_QUEST)
					PutLeaveRoom();*/
			}

			if(CPacketManager::GetPtr()->m_bIsGameEndDone)
			{
				CPacketManager::GetPtr()->m_bIsGameEndDone = false;

				m_nState = STATE_END;
			}
		}
		break;
	case STATE_END:
		{
			//if(m_bOccupation == true) 
			//	CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_CHOICECLASS, 20900, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			//else
			//{
				CPacketManager::GetPtr()->m_bIsAthleteInfoDone = true;

				CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

				if( pRoom->m_nMode == ROOM_MODE_QUEST )
					CProcessManager::GetPtr()->SetChangeProcess(new CTutorialMainProcess);
				else
					CProcessManager::GetPtr()->SetChangeProcess(new CReadyRoomProcess);
			//}
		}
		break;
	}
}

void CGameResultProcess::ProcessInput()
{
	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_2))
		PutGameEnd();

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);
	CCameraController::GetPtr()->ProcessInput(CGameProject::s_pGameProject->GetFrameTime());
}

bool CGameResultProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CGameResultProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

CUIControl** CGameResultProcess::GetControlChildrenPtr(CUIControl* _cParent)
{
	CUIControl** cResult = new CUIControl*[_cParent->m_Controls.GetSize()];

	for( int i = 0; i < _cParent->m_Controls.GetSize(); i++ )
	{
		cResult[i] = _cParent->m_Controls.GetAt( i );
	}
	return cResult;
}

void CGameResultProcess::Init()
{
	UIFunction::Set_UIVisibility( "ResultWin", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ResultLose", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ResultDraw", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ResultNogame", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ResultSuccess", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ResultFail", N3UI_VISIBLE_FALSE );
	
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);
	char text[STRLEN_64];

	CGameResultDialog::GetPtr()->FUNCTION_HeadResult();			// 상위 결과 요약 사항 표시
	
	CUIControl* cPerson;

	int Hindex=0;
	int Aindex=6;

	// MVP 플레이어 정보 표시
	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900038 ,0));
	assert(pMTable);
	sprintf(text, pMTable->m_sMessage, pRoom->m_cMvpPlayer.m_nLevel);	

	if( pRoom->m_cMvpPlayer.m_nLevel != 0)
		UIFunction::Set_ControlText( "Static_MvpLevel", text );

	_snprintf( text, STRLEN_64, "%s",pRoom->m_cMvpPlayer.m_sName );	
	UIFunction::Set_ControlText( "Static_MvpName", text );

	// 각 플레이어 경기 이력 표시 반복문
	for(int i=0;i<TEAM_SIZE*2;++i)
	{
		// 캐릭별 컨트롤 라인 묶음
		if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_HOME)
		{
			sprintf(text,"Static_UserRecord%d",Hindex++);	
			cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			sprintf(text,"PUBLIC_HPOSITION_IMAGE%d",pRoom->m_cEachResult[i].m_nPosition);
		}
		else if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_AWAY)
		{
			sprintf(text,"Static_UserRecord%d",Aindex++);	
			cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			sprintf(text,"PUBLIC_APOSITION_IMAGE%d",pRoom->m_cEachResult[i].m_nPosition);
		}
		else
		{
			if(Hindex<6)
			{
				sprintf(text,"Static_UserRecord%d",Hindex++);	
				cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			}
			else
			{
				sprintf(text,"Static_UserRecord%d",Aindex++);	
				cPerson = CClientUIManager::GetPtr()->FindEventControl(text);
			}
			cPerson->SetVisible(N3UI_VISIBLE_FALSE);
			continue;
		}

		// MVP 아이콘 표시
		if(pRoom->m_cMvpPlayer.m_nPlayerSeq != 0)
		{
			if(pRoom->m_cEachResult[i].m_nPlayerSeq==pRoom->m_cMvpPlayer.m_nPlayerSeq)
			{
				UIFunction::Set_UIVisibility( "Static_MvpPosition", N3UI_VISIBLE_TRUE );
				UICommonMethod::PositionIconDraw( "Static_MvpPosition", pRoom->m_cMvpPlayer.m_nPosition, UICommonMethod::AWAY_TEAM );
			}
		}
		else
			UIFunction::Set_UIVisibility( "Static_MvpPosition", N3UI_VISIBLE_FALSE );

		CUIControl* pStatic_M_bg = cPerson->FindEventControl("Static_M_bg");
		assert(pStatic_M_bg);
			
		if(m_pObjPlayerInfo->GetObjSeq() == pRoom->m_cEachResult[i].m_nPlayerSeq)
		{
			// 자기자신 플레이어 배경색 표시
			pStatic_M_bg->SetVisible(N3UI_VISIBLE_TRUE);
		/*	if(pRoom->m_cEachResult[i].m_nTeam==PLAYER_TEAM_HOME)
			{
				CUIControl* pStatic_H_Bold = CClientUIManager::GetPtr()->FindEventControl("Static_H_Bold");
				assert(pStatic_H_Bold);
				pStatic_H_Bold->SetVisible(N3UI_VISIBLE_TRUE);
			}
			else
			{
				CUIControl* pStatic_A_Bold = CClientUIManager::GetPtr()->FindEventControl("Static_A_Bold");
				assert(pStatic_A_Bold);
				pStatic_A_Bold->SetVisible(N3UI_VISIBLE_TRUE);
			}*/
			
			FUNCTION_AdditionCompensation(i);			// 추가 보상
		}
		else
			pStatic_M_bg->SetVisible(N3UI_VISIBLE_FALSE);

		CGameResultDialog::GetPtr()->FUNCTION_EachCharacterResult(i, cPerson);		// 각 캐릭터별 경기결과

		CUIControl* pStatic_Ai = cPerson->FindEventControl("Static_Ai");
		assert(pStatic_Ai);
		CUIControl* pStatic_Gained_Exp = cPerson->FindEventControl("Static_Gained_Exp");
		assert(pStatic_Gained_Exp);
		CUIControl* pStatic_Gained_Mileage = cPerson->FindEventControl("Static_Gained_Mileage");
		assert(pStatic_Gained_Mileage);
			
		// 플레이어일 경우 경험치 포인트 프로모션 표시
		if( (pRoom->m_cEachResult[i].m_nPlayerSeq<200000000) )
		{
			sprintf(text,"%d",pRoom->m_cEachResult[i].m_nExp);	
			pStatic_Gained_Exp->SetText(text);

			sprintf(text,"%d",pRoom->m_cEachResult[i].m_nPoint);	
			pStatic_Gained_Mileage->SetText(text);

			// 향후 피방 여부 표시
			CUIControl* pStatic_Gained_Promotion = cPerson->FindEventControl("Static_Gained_Promotion");
			assert(pStatic_Gained_Promotion);

			pStatic_Ai->SetVisible(N3UI_VISIBLE_FALSE);
		}
		else	// AI 플레이어 아이콘 표시
		{
			pStatic_Gained_Exp->SetText("");
			pStatic_Gained_Mileage->SetText("");
			pStatic_Ai->SetVisible(N3UI_VISIBLE_TRUE);
		}

		cPerson->SetVisible(N3UI_VISIBLE_TRUE);
	}

	int nPlayerNum = 0;
	nPlayerNum  = CObjRoomManager::GetPtr()->UserCount();
	
	// 게임 참여 유저수 표시(유저수에 비례하여 경험치 및 포인트 지급)
	CUIControl* pStatic_Bonus_Player  =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_Players");
	assert(pStatic_Bonus_Player);
	sprintf(text,"Player%02d", nPlayerNum);
	pStatic_Bonus_Player->SetElement((CClientUIManager::GetPtr()->FindEventControl(text))->m_pElement->Clone());

	CGameResultDialog::UpDateDialog(pRoom);

	if(pRoom->m_nMode != ROOM_MODE_QUEST)
	{
		FUNCTION_WinLose();

		switch(pRoom->IsMeWin())
		{
		case WIN_TYPE_NOGAME:
			NormalSound(20041);
			m_nAnimationID = 70011203;
			break;
		case WIN_TYPE_WIN:
			NormalSound(20031);
			m_nAnimationID = 70011201;
			break;
		case WIN_TYPE_LOSE:
			NormalSound(20051);
			m_nAnimationID = 70011202;
			break;
		case WIN_TYPE_DRAW:
			NormalSound(20041);
			m_nAnimationID = 70011203;
			break;
		default:
			m_nAnimationID = 70011201;
			break;
		}
	}
	else
	{
		bool bComplete = false;
		
		switch( pRoom->m_nQuestCode ) //105 ~ 107까지는 튜토리얼 10000 ~ 퀘스트
		{
		case 105:
			if(0<pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL])
				bComplete = true;
			break;
		case 106:
			if(pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]<pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL])
				bComplete = true;
			break;
		case 107:
			if(pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]<pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL])
				bComplete = true;
			break;
		}

		m_nAnimationID = 70011203;

		if( bComplete )
		{
			CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

			if((int)pSelectPlayerInfo->m_vQuestList.size())
			{
				int nCnt = 0;
				for(int i=0;i<(int)pSelectPlayerInfo->m_vQuestList.size();i++)
				{
					if( pSelectPlayerInfo->m_vQuestList.at(i)->m_nCode == pRoom->m_nQuestCode )
						break;
					nCnt++;
					if( nCnt == (int)pSelectPlayerInfo->m_vQuestList.size() )
					{
						CObjQuestInfo* pQuestList = new CObjQuestInfo();
						pQuestList->m_nAmount	+= 1;
						pQuestList->m_nCode		= pRoom->m_nQuestCode;
						
						pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
						PutQuestReward((short)pRoom->m_nQuestCode);
						break;
					}
				}
			}
			else
			{
				CObjQuestInfo* pQuestList = new CObjQuestInfo();
				pQuestList->m_nAmount	+= 1;
				pQuestList->m_nCode		= pRoom->m_nQuestCode;
				
				pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
				PutQuestReward((short)pRoom->m_nQuestCode);
			}

			CResQuestTableFormat* pTable = dynamic_cast<CResQuestTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, pRoom->m_nQuestCode, 0));
			assert(pTable);

			char sText[STRLEN_64]="";

			_snprintf(sText, STRLEN_64, "%d", pTable->m_cGift[0].m_nAmount);
			UIFunction::Set_ControlText( "Static_My_Gained_Point", sText );

			CUIControl* pResultSuccess = CClientUIManager::GetPtr()->FindEventControl( "ResultSuccess" );
			assert(pResultSuccess);
			pResultSuccess->SetVisible(N3UI_VISIBLE_TRUE);
			pResultSuccess->SetPlay(true, false, true);
		}
		else
		{
			CUIControl* pResultFail = CClientUIManager::GetPtr()->FindEventControl( "ResultFail" );
			assert(pResultFail);
			pResultFail->SetVisible(N3UI_VISIBLE_TRUE);
			pResultFail->SetPlay(true, false, true);
		}
	}
}

void CGameResultProcess::FUNCTION_QuestResult()
{
	char sText[MAX_PATH]="";

	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	CResQuestTableFormat * pQuestTable = dynamic_cast<CResQuestTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_QUESTTABLE, pRoom->m_nQuestCode, 0));
	assert(pQuestTable);

	// 퀘스트 명
	UIFunction::Set_ControlText( "QuestName", pQuestTable->m_sTitle);

	// 퀘스트 종류
	CResMessageTableFormat* pMTable = NULL;

	switch(pQuestTable->m_nKind)
	{
	case 0:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900084, 0));
		assert(pMTable);
		UIFunction::Set_ControlText( "QuestType", pMTable->m_sMessage);
		break;
	case 1:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900085, 0));
		assert(pMTable);
		UIFunction::Set_ControlText( "QuestType", pMTable->m_sMessage);
		break;
	case 2:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900086, 0));
		assert(pMTable);
		UIFunction::Set_ControlText( "QuestType", pMTable->m_sMessage);
		break;
	}

	UIFunction::Set_ControlText( "QuestGiftPoint", "0" );
	UIFunction::Set_ControlText( "QuestGiftExp", "0" );

	pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900050, 0));
	assert(pMTable);
	UIFunction::Set_ControlText( "GiftItemName", pMTable->m_sMessage );
	
	for(int i=0;i<3;i++)
	{
		switch(pQuestTable->m_cGift[i].m_nKind)
		{
		case OBJECT_KIND_ITEM:
			{
				CResItemTableFormat * pItemTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pQuestTable->m_cGift[i].m_nCode, 0 ));
				if(pItemTable == NULL) return;

				CUIControl* pQuestGiftIcon = CClientUIManager::GetPtr()->FindEventControl( "QuestGiftIcon" );
				assert(pQuestGiftIcon);
				
				CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pQuestGiftIcon, pItemTable->m_sIconFile );

				UIFunction::Set_ControlText( "GiftItemName", pItemTable->m_sName );
			}
			break;
		case OBJECT_KIND_POINT:
			{
				_snprintf( sText, MAX_PATH, "%d", pQuestTable->m_cGift[i].m_nAmount );
				UIFunction::Set_ControlText( "QuestGiftPoint", sText );
			}
			break;
		case OBJECT_KIND_EXP:
			{
				_snprintf( sText, MAX_PATH, "%d", pQuestTable->m_cGift[i].m_nAmount );
				UIFunction::Set_ControlText( "QuestGiftExp", sText );
			}
			break;
		case OBJECT_KIND_NONE:
			continue;
		}
	}
}

void CGameResultProcess::FUNCTION_AdditionCompensation(int i)
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	assert(pRoom);
	char sText[STRLEN_64]="";

	// 획득 경험치
	_snprintf(sText, STRLEN_64, "%d", pRoom->m_cEachResult[i].m_nExp );	
	UIFunction::Set_ControlText( "Static_My_Gained_Exp", sText );

	// 획득 포인트
	_snprintf(sText, STRLEN_64, "%d",pRoom->m_cEachResult[i].m_nPoint );	
	UIFunction::Set_ControlText( "Static_My_Gained_Point", sText );

	// 추가보상 보너스 MVP
	CUIControl* pStatic_Bonus_MVP  =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_MVP");
	_Assert(pStatic_Bonus_MVP, "Not found control", 0, "", 0);
	if(pRoom->m_cEachResult[i].m_nIsMvp)
		pStatic_Bonus_MVP->SetVisible(N3UI_VISIBLE_TRUE);
	else
		pStatic_Bonus_MVP->SetVisible(N3UI_VISIBLE_FALSE);

	// 추가보상 경험치 아이템
	CUIControl* pStatic_Bonus_ExpItem  =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_ExpItem");
	_Assert(pStatic_Bonus_ExpItem, "Not found control", 0, "", 0);
	if(pRoom->m_cEachResult[i].m_nIsExpItem)
		pStatic_Bonus_ExpItem->SetVisible(N3UI_VISIBLE_TRUE);
	else
		pStatic_Bonus_ExpItem->SetVisible(N3UI_VISIBLE_FALSE);

	// 추가보상 포인트 아이템
	CUIControl* pStatic_Bonus_PointItem  =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_PointItem");
	_Assert(pStatic_Bonus_PointItem, "Not found control", 0, "", 0);
	if(pRoom->m_cEachResult[i].m_nIsPointItem)
		pStatic_Bonus_PointItem->SetVisible(N3UI_VISIBLE_TRUE);
	else
		pStatic_Bonus_PointItem->SetVisible(N3UI_VISIBLE_FALSE);

	// 추가보상 보너스 GOLDEN TIME
	CUIControl* pStatic_Bonus_GoldenTime =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_GoldenTime");
	_Assert(pStatic_Bonus_PointItem, "Not found control", 0, "", 0);
	if(pRoom->m_cEachResult[i].m_nIsGoldenTime)
		pStatic_Bonus_GoldenTime->SetVisible(N3UI_VISIBLE_TRUE);
	else
		pStatic_Bonus_GoldenTime->SetVisible(N3UI_VISIBLE_FALSE);

	// 추가보상 보너스 이벤트
	CUIControl* pStatic_Bonus_Event =  CClientUIManager::GetPtr()->FindEventControl("Static_Bonus_Event");
	_Assert(pStatic_Bonus_Event, "Not found control", 0, "", 0);
	if(pRoom->m_cEachResult[i].m_nIsEvent) 
		pStatic_Bonus_Event->SetVisible(N3UI_VISIBLE_TRUE);
	else
		pStatic_Bonus_Event->SetVisible(N3UI_VISIBLE_FALSE);

	/*if(m_pObjPlayerInfo->m_cLevel.m_nLevel == 19 && pRoom->m_cEachResult[i].m_nIsLevelUp)
		m_bOccupation = true;*/
}

void CGameResultProcess::FUNCTION_WinLose()
{
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	CUIControl* pResultWin = CClientUIManager::GetPtr()->FindEventControl( "ResultWin" );
	assert(pResultWin);
	CUIControl* pResultLose = CClientUIManager::GetPtr()->FindEventControl( "ResultLose" );
	assert(pResultLose);
	CUIControl* pResultDraw = CClientUIManager::GetPtr()->FindEventControl( "ResultDraw" );
	assert(pResultDraw);
	CUIControl* pResultNogame = CClientUIManager::GetPtr()->FindEventControl( "ResultNogame" );
	assert(pResultNogame);

	switch(pRoom->IsMeWin())
	{
	case WIN_TYPE_NOGAME:
		pResultNogame->SetVisible(N3UI_VISIBLE_TRUE);
		pResultNogame->SetPlay(true, false, true);
		NormalSound(20041);
		m_nAnimationID = 70011203;
		break;
	case WIN_TYPE_WIN:
		pResultWin->SetVisible(N3UI_VISIBLE_TRUE);
		pResultWin->SetPlay(true, false, true);
		NormalSound(20031);
		m_nAnimationID = 70011201;
		break;
	case WIN_TYPE_LOSE:
		pResultLose->SetVisible(N3UI_VISIBLE_TRUE);
		pResultLose->SetPlay(true, false, true);
		NormalSound(20051);
		m_nAnimationID = 70011202;
		break;
	case WIN_TYPE_DRAW:
		pResultDraw->SetVisible(N3UI_VISIBLE_TRUE);
		pResultDraw->SetPlay(true, false, true);
		NormalSound(20041);
		m_nAnimationID = 70011203;
		break;
	default:
		m_nAnimationID = 70011201;
		break;
	}
}

void CGameResultProcess::Update(float fFrameTime, float fAccumTime)
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

void CGameResultProcess::FUNCTION_InitCardResult()
{
	char sText[STRLEN_64]="";

	UIFunction::Set_UIVisibility( "ResultCardBG", N3UI_VISIBLE_FALSE );

	CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( "ResultCardBG" );
	assert(pCard);
	CUIControl* pImg = CClientUIManager::GetPtr()->FindEventControl( "ListImg" );
	assert(pImg);

	UIFunction::SetChildToVisibility( pImg, "CardAni", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Card_Msg", N3UI_VISIBLE_FALSE );

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "Marking0%d", i);
		UIFunction::SetChildToVisibility( pImg, sText, N3UI_VISIBLE_FALSE );
		_snprintf(sText, STRLEN_64, "ResultCardBG%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );

		// 일치 표시 초기화
		CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( sText );
		assert(pCard);
		
		UIFunction::SetChildToVisibility( pCard, "CardAni", N3UI_VISIBLE_TRUE );

		UIFunction::SetChildToVisibility( pCard, "Marking00", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( pCard, "Marking01", N3UI_VISIBLE_FALSE );
		UIFunction::SetChildToVisibility( pCard, "Marking02", N3UI_VISIBLE_FALSE );
	}

	m_vPosition.clear();
	m_vLevel.clear();
	m_vRank.clear();

	char ArrPosition[12] = { 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 40 };

	for(char i=0;i<12;i++)
		m_vPosition.push_back(ArrPosition[i]);
	for(char i=1;i<50;i++)
		m_vLevel.push_back(i);
	for(char i=0;i<5;i++)
		m_vRank.push_back(i);

		CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	CResMessageTableFormat* pMTable = NULL;

	switch(pRoom->m_nCardMsg)
	{
	case -3: //인벤토리가 꽉찼습니다.
		{
			UIFunction::Set_UIVisibility( "Card_Msg", N3UI_VISIBLE_TRUE );
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900089 ,0));
			assert(pMTable);
			UIFunction::Set_ControlText( "Card_Msg", pMTable->m_sMessage );
		}
		break;
	case -4: //카드를 받을 조건이 되지 않습니다.
		{	
			UIFunction::Set_UIVisibility( "Card_Msg", N3UI_VISIBLE_TRUE );
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900090 ,0));
			assert(pMTable);
			UIFunction::Set_ControlText( "Card_Msg", pMTable->m_sMessage );
		}
		break;
	case -5: //카드를 받을 조건이 되지 않습니다.
		{	
			UIFunction::Set_UIVisibility( "Card_Msg", N3UI_VISIBLE_TRUE );
			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900092 ,0));
			assert(pMTable);
			UIFunction::Set_ControlText( "Card_Msg", pMTable->m_sMessage );
		}
		break;
	default:
		UIFunction::Set_ControlText( "Card_Msg", "" );
	}
}	

void CGameResultProcess::FUNCTION_CardUpdate()
{
	char sText[STRLEN_64]="";

	int nTime = timeGetTime() - nDisplayTime;

	if(4000<nTime)
	{
		CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
		if(pRoom == NULL) return;
		pRoom->m_bRecvCard = false;

		for(int i=0;i<3;i++)
		{
			// 일치 표시
			_snprintf(sText, STRLEN_64, "ResultCardBG%d", i );
			CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pCard);

			_snprintf(sText, STRLEN_64, "Marking0%d", m_nRand);
			UIFunction::SetChildToVisibility( pCard, sText, N3UI_VISIBLE_TRUE );
		}
	}

	for(int i=0;i<nTime/1000;i++)
	{
		if( 3 == i )
		{
			CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( "ResultCardBG" );
			assert(pCard);
			CUIControl* pImg = CClientUIManager::GetPtr()->FindEventControl( "ListImg" );
			assert(pImg);
			UIFunction::SetChildToVisibility( pImg, "CardAni", N3UI_VISIBLE_FALSE );

			_snprintf(sText, STRLEN_64, "Marking0%d", m_nRand);
			UIFunction::SetChildToVisibility( pImg, sText, N3UI_VISIBLE_TRUE );
			continue;
		}
		else if( 2<i ) continue;

		_snprintf(sText, STRLEN_64, "ResultCardBG%d", i );
		UIFunction::SetChildToVisibility( sText, "CardAni", N3UI_VISIBLE_FALSE );
	}
}

void CGameResultProcess::FUNCTION_CardResult()
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_CInfo = (*pObjPlayerInfo->m_vCardbotList.at(pObjPlayerInfo->m_vCardbotList.size()-1));
	char nPosition = m_CInfo.m_nPosition;
	char nRank = m_CInfo.m_nRank;
	char nLv = m_CInfo.m_nLevel;
	int nCostume = m_CInfo.m_nCostume;

	char sText[STRLEN_64]="";

	int nRand=0;
	m_nRand = rand()%3;

	UIFunction::Set_UIVisibility( "ResultCardBG", N3UI_VISIBLE_TRUE );

	for(int i=0;i<3;i++)
	{
		_snprintf(sText, STRLEN_64, "ResultCardBG%d", i );
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
	}

	for(int i=0;i<4;i++)
	{
		if( m_nRand == 0 || i == 3 )
			nRank = m_CInfo.m_nRank;
		else
		{
			nRand = rand()%m_vRank.size();
			nRank = m_vRank.at(nRand);
			m_vRank.erase(&m_vRank[nRand]);
		}

		if( m_nRand == 1 || i == 3 )
			nPosition = m_CInfo.m_nPosition;
		else
		{
			nRand = rand()%m_vPosition.size();
			nPosition = m_vPosition.at(nRand);
			m_vPosition.erase(&m_vPosition[nRand]);
		}

		if( m_nRand == 2 || i == 3 )
			nLv = m_CInfo.m_nLevel;
		else
		{
			nRand = rand()%m_vLevel.size();
			nLv = m_vLevel.at(nRand);
			m_vLevel.erase(&m_vLevel[nRand]);
		}

		if( i == 3 )
		{
			nCostume = m_CInfo.m_nCostume;

			CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( "ResultCardBG" );
			assert(pCard);

			CUIControl* pImg = CClientUIManager::GetPtr()->FindEventControl( "ListImg" );
			assert(pImg);

			UICommonMethod::FUNCTION_CardInfoDraw(pImg, nPosition, nRank, nLv, nCostume);
		}
		else
		{
			nCostume = rand()%63;

			_snprintf(sText, STRLEN_64, "ResultCardBG%d", i );
			CUIControl* pCard = CClientUIManager::GetPtr()->FindEventControl( sText );
			assert(pCard);

			UICommonMethod::FUNCTION_CardInfoDraw(pCard, nPosition, nRank, nLv, nCostume);
		}
	}
}

void CGameResultProcess::RenderFrame()
{
	if( m_bIsRender )
	{
		CObjectModelManager::GetPtr()->Render();

		CClientUIManager::GetPtr()->OnRender();
	}
}