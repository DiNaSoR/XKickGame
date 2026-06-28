#include "stdafx.h"
#include "QuestPlayProcess.h"
#include "FblArea.h"
#include "ObjPlayerManager.h"
#include "ObjRoomManager.h"
#include "UICommonMethod.h"
#include "ResBallFormat.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "FootballManager.h"
#include "PlayModeManager.h"
#include "DutyMachine_Base.h"
#include "PutPacket.h"
#include "UtlLog.h"
#include "EffectManager.h"
#include "ReadyRoomProcess.h"
#include "UIFunction.h"

CQuestPlayProcess::CQuestPlayProcess() : CBasePlayProcess()
{
	m_nInputKeySendTime = 0;
	m_nQuestType = TUTORIAL_1TYPE;

	m_bIsRender = false;
	m_nQuestStep1MaxCount = 5;

	m_nTryTotalCount = 0;//총 시도 횟수

	m_nTrySuccessCount = 0;//시도 성공 횟수

	m_bTryProcess = false;

	m_bQuestStep1End = false;

	m_pPannelManager = NULL;

    m_fAccumTime = 0.0f;

	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		m_ppKeyUIControl[i] = NULL;
	}

	m_bStart = true;
}

CQuestPlayProcess::~CQuestPlayProcess(void)
{
}

void CQuestPlayProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pPannelManager = new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode, pObjGroundInfo->m_pModel->m_spLightNode);

	m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	char sText[STRLEN_64]="";
	sprintf(sText,".\\Script\\Quest%d.xml", m_nQuestType);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, sText, 1000000 + m_nQuestType);

	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	memcpy(&m_pSelectPlayerInfo->m_cSetting, &pSelectPlayerInfo->m_cSetting, sizeof(pSelectPlayerInfo->m_cSetting));

	memcpy(m_pSelectPlayerInfo->m_bIsUseKey, pSelectPlayerInfo->m_bIsUseKey, sizeof(pSelectPlayerInfo->m_bIsUseKey));

	m_nTime = timeGetTime();

	m_nState = STATE_START;
}

void CQuestPlayProcess::FUNCTION_InitKeyUIControl()
{
	char temp[STRLEN_128];

	for(int i = 0; i < ARRAY_KEY_LEFTSHOOT; i++)
	{
		sprintf(temp,"Static_DirectionKey_%d", i);
		m_ppKeyUIControl[i] = CClientUIManager::GetPtr()->FindEventControl(temp);
	}

	for(int i = 0; i < 11; i++)
		UIFunction::Set_ChildNumVisibility("Dialog_TutoObject", "Static_TutoHotKey0", i+101, N3UI_VISIBLE_FALSE);

	CUIControl* pControl = UIFunction::Set_ChildNumVisibility("Dialog_TutoObject", "Static_TutoHotKey0", m_nQuestType, N3UI_VISIBLE_TRUE);
	if(pControl == NULL) return;

	CUIControl* pTutoHotKeyBg = pControl->FindEventControl("Static_TutoHotKeyBg");
	if(pTutoHotKeyBg == NULL) return;

	for(int i = ARRAY_KEY_LEFTSHOOT; i < ARRAY_KEY_SIZE; i++)
	{
		sprintf(temp,"Static_TutoHotKey_%d", i);
		m_ppKeyUIControl[i] = pTutoHotKeyBg->FindEventControl(temp);
	}
}

void CQuestPlayProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CEffectManager::GetPtr()->Update( fAccumTime );
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
}

void CQuestPlayProcess::ProcessInput()
{
	//CBasePlayProcess::ProcessInput();

	//if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_M))
	//{
	//	m_bStart = true;
	//}
}

void CQuestPlayProcess::Update(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->GetObjectNode()->Update(fAccumTime);
	CCharacterCollisionDetector::GetPtr()->Update(fFrameTime, fAccumTime);

	m_pPannelManager->InitPannelPoint();

	m_pPannelManager->Update(true);

	const DWORD& nRemainTime = m_pRoom->GetPlayTime();
}

void CQuestPlayProcess::Terminate()
{
	CCameraController::GetPtr()->ChangeCamera();

	CBasePlayProcess::Terminate();

	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);
}

void CQuestPlayProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	//m_pPannelManager=new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode,pObjGroundInfo->m_pModel->m_spLightNode);
}

void CQuestPlayProcess::LoadCharacter()
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

			if(pAnyPlayer->m_nPosition == POSITION_GK)
				pAnyPlayer->m_pModel->m_spCollisionNode = LoadCollision(pAnyPlayer->GetRootNode());

			CProcessManager::GetPtr()->AttachItem(pAnyPlayer); 	
		}
	}
	CObjectModelManager::GetPtr()->ShadowOnOff(true, true);
}

NiNodePtr CQuestPlayProcess::LoadCollision(NiNode *pRootNode)
{
	NiStream kStream;
	char	FullName[STRLEN_128];

	sprintf(FullName, "Object\\Goal_Collision.nif");

	if( kStream.Load( FullName ) == false )
		return NULL;

	NiNodePtr spGroundObjectNode = (NiNode*)kStream.GetObjectAt(0);
	kStream.RemoveAllObjects();

	if(pRootNode)
	{
		pRootNode->AttachChild(spGroundObjectNode);//
		spGroundObjectNode->SetAppCulled(true);
		pRootNode->Update(0.0f);
		pRootNode->UpdateNodeBound();
		pRootNode->UpdateProperties();
		pRootNode->UpdateEffects();
	}

	return spGroundObjectNode;
}

//튜토리얼 공통적인 내용
void CQuestPlayProcess::FUNCTION_UpdateScoreUI(bool bSuccess)
{
	char text[MAX_PATH];

	sprintf(text, "ONum_%d", m_nTryTotalCount);
	UIFunction::ChangeTexture("Static_CounterCurrent", text);

	sprintf(text, "ONum_%d", m_nQuestStep1MaxCount);
	UIFunction::ChangeTexture("Static_CounterTotal", text);

	sprintf(text, "Static_Counter_IconBg%d", m_nTryTotalCount - 1);
	CUIControl* pTarget = CClientUIManager::GetPtr()->FindEventControl(text);
	assert(pTarget);

	if(bSuccess)	UIFunction::ChangeTexture(pTarget, "Static_Counter_Green");
	else 			UIFunction::ChangeTexture(pTarget, "Static_Counter_Red");
}

void CQuestPlayProcess::FUNCTION_UpdateKeyUI()
{
	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		if(m_ppKeyUIControl[i])
		{
			if(CInputManager::GetPtr()->m_CInputAdapter.Get_ControlKeyState(i) != KEY_NONE)
				m_ppKeyUIControl[i]->SetVisible(N3UI_VISIBLE_TRUE);
			else
				m_ppKeyUIControl[i]->SetVisible(N3UI_VISIBLE_FALSE);
		}
	}
}

void CQuestPlayProcess::FUNCTION_ExplainUIInit()
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "ReplayActionBtn", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ExitBtn", N3UI_VISIBLE_FALSE );
	
	UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ExitBtn", N3UI_VISIBLE_FALSE );
	//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_FALSE );
}

void CQuestPlayProcess::FUNCTION_KeyUIInit()
{
	UIFunction::Set_UIVisibility( "Key_Q0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_W0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_E0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_A0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_S0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_D0", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Key_F0", N3UI_VISIBLE_TRUE );
}

void CQuestPlayProcess::FUNCTION_DemonstrationGameUIInit()
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_TRUE );
}	

void CQuestPlayProcess::FUNCTION_GamePlayExplainUIInit()
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );

	// 초기화
	char text[MAX_PATH];

	sprintf(text, "ONum_%d", m_nTryTotalCount);
	UIFunction::ChangeTexture("Static_CounterCurrent", text);

	sprintf(text, "ONum_%d", m_nQuestStep1MaxCount);
	UIFunction::ChangeTexture("Static_CounterTotal", text);

	for(int i = 0; i < m_nQuestStep1MaxCount; i++)
	{
		sprintf(text, "Static_Counter_IconBg%d", i);
		CUIControl* pTarget = CClientUIManager::GetPtr()->FindEventControl(text);

		UIFunction::ChangeTexture(pTarget, "Static_Counter_Default");
	}
}

void CQuestPlayProcess::FUNCTION_GamePlayUIInit()
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_FALSE );

	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_TRUE );
}

void CQuestPlayProcess::FUNCTION_ResultUIInit(bool bSuccess)
{
	UIFunction::Set_UIVisibility( "TutorialBg", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "Dialog_TutoObject", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "ExitBtn", N3UI_VISIBLE_TRUE );
	
	
	if( bSuccess )
	{
		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900105, 0));
		assert(pMTable);

		UIFunction::Set_ControlText( "Explain_Text", pMTable->m_sMessage );
		
		UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_TRUE );
		//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_TRUE );
	}
	else
	{
		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900106, 0));
		assert(pMTable);

		UIFunction::Set_ControlText( "Explain_Text", pMTable->m_sMessage );

		UIFunction::Set_UIVisibility( "ItemBG", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "ItemExplainBG", N3UI_VISIBLE_FALSE );
		//UIFunction::Set_UIVisibility( "Conversation_Text", N3UI_VISIBLE_FALSE );

		UIFunction::Set_UIVisibility( "ReplayActionBtn", N3UI_VISIBLE_TRUE );
	}
}

void CQuestPlayProcess::FUNCTION_NextQuest()
{
	int nCurrentQuest = m_nQuestType;

	nCurrentQuest++;

	int nCnt = 0;
	
	if( 104 < nCurrentQuest )
		nCurrentQuest = 100;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	if((int)pObjPlayerInfo->m_vQuestList.size())
	{
        for( int i=0;i<(int)pObjPlayerInfo->m_vQuestList.size();i++ )
		{
			if( pObjPlayerInfo->m_vQuestList.at(i)->m_nCode == nCurrentQuest )
			{
				nCnt++;

				if( nCnt == 5)
					break;

				i=-1;
				nCurrentQuest++;
				
				if( 104 < nCurrentQuest )
					nCurrentQuest = 100;
				
				continue;
			}
		}
	}

	//if( nCnt == 5 )
	//	CProcessManager::GetPtr()->SetNextProcess(new CQuestMainProcess);
	//else
	//{
	//	if( nCurrentQuest == 100 )
	//		CProcessManager::GetPtr()->SetNextProcess(new CGuideQuestProcess(14));
	//	else
	//		CProcessManager::GetPtr()->ChangeQuesteMachine(nCurrentQuest);
	//}
}

void CQuestPlayProcess::SetEndingAnimation()
{
	VectorAnyoneList* pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);

	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();	

	if(m_nTrySuccessCount > 0)
	{
		if(pMember->m_nQuest > 62)
			m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1073, 0));//성공일때
		else
			m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1071 + Random(2), 0));//성공일때
	}
	else
		m_pCeremonyTable	= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1074, 0));//실패일때

	assert(m_pCeremonyTable);

	int i = 1;
	int j = 0;

	CCameraController::GetPtr()->ChangeCameraType(CEREMONYFIX, m_pCeremonyTable->m_sCameraFile, m_pCeremonyTable->m_nNif);
	SetShadowCamera();

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			if((*iPlayer)->m_nTeam == PLAYER_TEAM_VIEW)
				continue;

			if((*iPlayer)->m_nTeam == PLAYER_TEAM_STAY)
				continue;

			CObjPlayerInfo* pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->GetObjSeq() == m_pSelectPlayerInfo->GetObjSeq()) 
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[0];
			}
			else if(pAnyPlayer->m_nTeam == m_pSelectPlayerInfo->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[i];
				i++;
			}
			else if(pAnyPlayer->m_nTeam != m_pSelectPlayerInfo->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nLoser[j];
				j++;
			}
		}
	}

	m_nTime = timeGetTime();
}

void CQuestPlayProcess::UpdateStateStart(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	CBasePlayProcess::UpdateStateStart(fFrameTime, fAccumTime);

	CCameraController::GetPtr()->ChangeCamera();

	m_nState = STATE_LOADING;
}

void CQuestPlayProcess::UpdateStateLoading(float fFrameTime, float fAccumTime)
{
	CObjBallManager::GetPtr()->GetBallPointer()->SetCurrentPoint();
	m_nState = STATE_GAME_SETTING;
}
void CQuestPlayProcess::UpdateStateRestart(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_SETTING;
}
void CQuestPlayProcess::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameSetting(fFrameTime, fAccumTime);

	CObjPlayerManager::GetPtr()->CreateAIMachine(1);//게임 초기화 부분

	CFblEngine::GetPtr()->SetMeParent(true);

	//FUNCTION_InitKeyUIControl();

	//FUNCTION_UpdateScoreUI(true);	

	//FUNCTION_UIInit();

	m_nState = STATE_GAME_INIT;
}
void CQuestPlayProcess::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);

//#ifdef __AITYPE__
	CAIManager::GetPtr()->Init(CFblArea::GetPtr()->GetGroundX(), CFblArea::GetPtr()->GetGroundY(), CFblArea::GetPtr()->GetPenaltyRadius(), false);
//#else
//#endif

	CCameraController::GetPtr()->ChangeCameraType(D4WAYTILTMOVE);
	CCameraController::GetPtr()->ChangeCameraTarget(m_nCameraTarget);
	SetShadowCamera();

	m_pPannelManager->Update(false);

	m_bTryProcess = false;
}

void CQuestPlayProcess::UpdateStateGameReady(float fFrameTime, float fAccumTime)
{
	//초기 튜토리얼 입장 모습
	CBasePlayProcess::UpdateStateGameReady(fFrameTime, fAccumTime);
    ClothSimulationManager::GetPtr()->Update( fAccumTime );

	FUNCTION_ExplainUIInit();

	if(m_bStart)
	{
		FUNCTION_DemonstrationGameUIInit();

		CInputManager::GetPtr()->InitInputKey();

		UIFunction::Set_UIVisibility("Static_Start", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP); //START 연출씬 세팅
		m_nTime = timeGetTime();

		NormalSound("game_start1.wav"); //Sound 세팅

		m_nState = STATE_GAME_START;
	}
}
void CQuestPlayProcess::UpdateStateGameStart(float fFrameTime, float fAccumTime)
{
	ClothSimulationManager::GetPtr()->Update( fAccumTime );
	Update(fFrameTime, fAccumTime);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsPlayStartEnd() && m_bStart)
	{
		CObjPlayerManager::GetPtr()->SetUpdateAIMachine(true);

		m_nState = STATE_GAME_PLAY;
		//		FUNCTION_ChangeQuestPlayUI();

		m_nQuestTime = timeGetTime();

		FUNCTION_GamePlayUIInit();
	}
}
void CQuestPlayProcess::UpdateStateGamePlay(float fFrameTime, float fAccumTime)
{
	ClothSimulationManager::GetPtr()->Update( fAccumTime );
	CObjBallInfo*	pBall = CObjBallManager::GetPtr()->GetBallPointer();

	Update(fFrameTime, fAccumTime);

	if(!m_bTryProcess || (m_nQuestType == 104 || m_nQuestType == 108))//다이렉트랑 걷어내기
		InputKey();

	//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
	//#else
	//#endif

	CObjBallManager::GetPtr()->Update();

	if(CInputManager::GetPtr()->GetSoundPlay())
	{
		CAudioManager::GetPtr()->Create2DSound("gui_ok.wav");
	}

	FUNCTION_UpdateKeyUI();

	GamePlay(fFrameTime, fAccumTime);//시범 플레이	

	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	CInputManager::GetPtr()->Update();
}

void CQuestPlayProcess::GamePlay(float fFrameTime, float fAccumTime)
{
	if(!m_bTryProcess && TryProcess())
	{
		m_nTime = timeGetTime();
		m_bTryProcess = true;
	}

	if(m_bTryProcess && SuccessProcess())//성공은 즉시 처리
	{
		//		CFblEngine::GetPtr()->m_bIsGoalState = true;

		FUNCTION_UpdateScoreUI(true);

		//성공
		UIFunction::Set_UIVisibility("Static_Success", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//성공 연출씬 세팅

		//CObjPlayerManager::GetPtr()->SetPlayerAnimation(PLAYER_TEAM_STAY, ANIMATION_SUCCESS);

		CInputManager::GetPtr()->InitInputKey();
		m_pSelectPlayerInfo->m_cInputKey.InitCommand();
		m_pSelectPlayerInfo->InitDirectKey();

		m_nTime = timeGetTime();

		m_nState = STATE_GOAL_PLAY;	
	}
	else if(m_bTryProcess && timeGetTime() - m_nTime > 3500)
	{
		FailProcess();

		FUNCTION_UpdateScoreUI(false);

		//실패
		UIFunction::Set_UIVisibility("Static_Retry", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//실패 연출씬 세팅

		//CObjPlayerManager::GetPtr()->SetPlayerAnimation(PLAYER_TEAM_STAY, ANIMATION_FAILURE);

		CInputManager::GetPtr()->InitInputKey();
		m_pSelectPlayerInfo->m_cInputKey.InitCommand();
		m_pSelectPlayerInfo->InitDirectKey();

		CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);

		m_nTime = timeGetTime();

		m_nState = STATE_GOAL_PLAY;
	}
	else 
	{
		CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	}
}

void CQuestPlayProcess::UpdateStateGoalPlay(float fFrameTime, float fAccumTime)
{
	Update(fFrameTime, fAccumTime);
	ClothSimulationManager::GetPtr()->Update( fAccumTime );
	//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
	//#else
	//#endif

	CObjBallManager::GetPtr()->Update();
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(timeGetTime() - m_nTime > 2000)
	{
		if(m_nQuestStep1MaxCount <= m_nTryTotalCount)
		{
			//결과 패킷 보내기
			if(m_nTrySuccessCount > 0)
			{
				FUNCTION_ResultUIInit(true);

				CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
				if(pMember == NULL) return;

				CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

				if((int)pSelectPlayerInfo->m_vQuestList.size())
				{
					int nCnt = 0;
					for(int i=0;i<(int)pSelectPlayerInfo->m_vQuestList.size();i++)
					{
						if( pSelectPlayerInfo->m_vQuestList.at(i)->m_nCode == m_nQuestType )
							break;
						nCnt++;
						if( nCnt == (int)pSelectPlayerInfo->m_vQuestList.size() )
						{
							CObjQuestInfo* pQuestList = new CObjQuestInfo();
							pQuestList->m_nCode		= m_nQuestType;

							pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
							PutQuestReward((short)m_nQuestType);
							break;
						}
					}
					m_nState = STATE_GAME_END_READY;
					return;
				}
				else
				{
					CObjQuestInfo* pQuestList = new CObjQuestInfo();
					pQuestList->m_nCode		= m_nQuestType;

					pSelectPlayerInfo->m_vQuestList.push_back(pQuestList);
					PutQuestReward((short)m_nQuestType);
					m_nState = STATE_GAME_END_READY;
					return;
				}
			}
			else
				FUNCTION_ResultUIInit(false);

			m_nTime = timeGetTime();
		}
		else 
		{
			CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);
			m_nState = STATE_GAME_INIT;
		}
	}
}
void CQuestPlayProcess::UpdateStateReplayReady(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_REPLAY_PLAY;
}
void CQuestPlayProcess::UpdateStateReplayPlay(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_CEREMONY_PLAY;
}
void CQuestPlayProcess::UpdateStateCeremonyPlay(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_END_READY;
}
void CQuestPlayProcess::UpdateStateGameEndReady(float fFrameTime, float fAccumTime)
{
	//m_nState = STATE_GAME_END;
}
void CQuestPlayProcess::UpdateStateGameEnd(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	NormalSound("game_end1.wav"); //Sound 세팅
	SetEndingAnimation();
	ClothSimulationManager::GetPtr()->Reset();

	m_nState = STATE_END;
}
void CQuestPlayProcess::UpdateStateEnd(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	Update(fFrameTime, fAccumTime);
	ClothSimulationManager::GetPtr()->Reset();

	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(!m_bQuestStep1End && timeGetTime() - m_nTime > 2000)
	{
		m_bQuestStep1End = true;
		PutGameResult();
	}
}