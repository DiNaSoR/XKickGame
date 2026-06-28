#include "stdafx.h"
#include "BasePlayProcess.h"
#include "PutPacket.h"
#include "UICommonMethod.h"
#include "UIFunction.h"
#include "ResAICharacterFormat.h"//로그 관련 나중에 삭제 요망
#include "CoreApi.h"
#include "ClothSimulationManager.h"

CObjPlayerInfo* CBasePlayProcess::m_pSelectPlayerInfo = NULL;

CBasePlayProcess::CBasePlayProcess() : CBaseProcess()
{
    CGameProject::s_pGameProject->SetMoveWindow( false );

	m_nProcessCmd = PROCESS_GAMEPLAY;
	m_bIsRender = false;
	srand(time(NULL));
	m_pGameOptionXml = CGameProject::s_pGameProject->GetTiXml("QuickChatting");
	m_bRobot = false;
	m_pCeremonyTable = NULL;

	m_cBackUpInputKey.InitCommand();

	m_nInputKeySendTime = 0;

	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	pSelectPlayerInfo->CameraLimitTest();

	//임시
	m_bNewbieHelp = true;

	m_nForm = 0;

	//케릭터에 들어있는 반사맵을 켜거나 끌때 사용한다
	//여기 있는 이유는 게임에만 적용될수 있도록 한것인데
	//추후에는 게임 로딩으로 옴겨야 된다
    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();
    bool bEnvironmentMap = pkVideoInfomation->GetLightMap() == 1;
    CObjectModelManager::GetPtr()->SetEnvironmentMapEnable( bEnvironmentMap );//캐릭터의 반사맵

    bool bGameEffectEnable = pkVideoInfomation->GetGameEffect() == 1;
    CEffectManager::GetPtr()->SetGameTypeEffectEnable( bGameEffectEnable );//게임중 이팩트(먼지, 패스트 런 이팩트 등)

    bool bWeatherEffect = pkVideoInfomation->GetWeather() == 1;
    CEffectManager::GetPtr()->SetWeatherTypeEffectEnable( bWeatherEffect );//게임중 비나 눈효과를 키거나 끈다
}

CBasePlayProcess::~CBasePlayProcess(void)
{
}

void CBasePlayProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{	
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);    
	CEffectManager::GetPtr()->Update( fAccumTime );

	CChattingDialog::GetPtr()->TimeDeleyToDeleteChattingList();

	if(CPacketManager::GetPtr()->m_bIsLeaveRoom)//다음 단계 초기화 부
	{
		CPacketManager::GetPtr()->m_bIsLeaveRoom = false;

		if( m_pRoom->IsMeParent() )
			PutGameResult();

		m_nState = STATE_GAME_END;
	}

	if( m_pRoom->m_nMode != ROOM_MODE_QUEST )
		AngerAnimation();
}

void CBasePlayProcess::Terminate()
{
    CGameProject::s_pGameProject->SetMoveWindow( true );

    CObjectModelManager::GetPtr()->SetLOD( CObjectModelManager::eHighest );

    CObjectModelManager::GetPtr()->DestroyMultiThreadUpdate();

	CObjectModelManager::GetPtr()->SetAngerMode(false);

	if( m_pRoom->m_nMode != ROOM_MODE_QUEST )
	{
		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl( "AngerAni" );
		assert(pAni);
		pAni->SetVisible(N3UI_VISIBLE_FALSE);
		pAni->SetPlay(false);
	}

	SAFE_DELETE(m_pPannelManager);

	CObjBallManager::GetPtr()->GetBallPointer()->Terminate();

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = false;

//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Terminate();
//#else
//#endif

	CObjPlayerManager::GetPtr()->DeleteAlPlayerInList();

	CObjPlayerManager::GetPtr()->ErasePlayerModelPoint();

	CObjectModelManager::GetPtr()->EraseObject();

	CEffectManager::GetPtr()->Clear();

	CInputManager::GetPtr()->Terminate();

	CBaseProcess::Terminate();
}

void CBasePlayProcess::UpdateState(float fFrameTime, float fAccumTime)
{
    CObjectModelManager::GetPtr()->SetLOD( CObjectModelManager::eAutomate );

	switch(m_nState)
	{
	case STATE_START:
		{            
			UpdateStateStart(fFrameTime, fAccumTime);
		}
		break;
	case STATE_LOADING:
		{
			UpdateStateLoading(fFrameTime, fAccumTime);
		}
		break;
	case STATE_RESTART:
		{
			UpdateStateRestart(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_SETTING:
		{
			UpdateStateGameSetting(fFrameTime, fAccumTime);
		}
		break;
	case STATE_PRODUCTION_INIT:
		{
			UpdateStateProductionInit(fFrameTime, fAccumTime);
		}
		break;
	case STATE_PRODUCTION_PLAY:
		{
            CObjectModelManager::GetPtr()->SetLOD( CObjectModelManager::eHighest );
			UpdateStateProductionPlay(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_INIT:
		{
			UpdateStateGameInit(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_READY:
		{
			UpdateStateGameReady(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_START:
		{
			UpdateStateGameStart(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_PLAY:
		{
			UpdateStateGamePlay(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GOAL_PLAY:
		{
			UpdateStateGoalPlay(fFrameTime, fAccumTime);
		}
		break;
	case STATE_REPLAY_READY:
		{
			UpdateStateReplayReady(fFrameTime, fAccumTime);
		}
		break;
	case STATE_REPLAY_PLAY:
		{
            CObjectModelManager::GetPtr()->SetLOD( CObjectModelManager::eHighest );
			UpdateStateReplayPlay(fFrameTime, fAccumTime);
		}
		break;
	case STATE_CEREMONY_PLAY:
		{
            CObjectModelManager::GetPtr()->SetLOD( CObjectModelManager::eHighest );
			UpdateStateCeremonyPlay(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_END_READY:
		{
			UpdateStateGameEndReady(fFrameTime, fAccumTime);
		}
		break;
	case STATE_GAME_END:
		{
			UpdateStateGameEnd(fFrameTime, fAccumTime);
		}
		break;
	case STATE_END:
		{
			UpdateStateEnd(fFrameTime, fAccumTime);
		}
		break;
	}

	CInputManager::GetPtr()->Update();
}

bool CBasePlayProcess::OnDefault(NiEventRef pEventRecord)
{
	if(!CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam))
	{
		CInputManager::GetPtr()->MouseMsgProc(pEventRecord->uiMsg, pEventRecord->lParam, pEventRecord->wParam);
		CInputManager::GetPtr()->KeyboardMsgProc(pEventRecord->uiMsg, pEventRecord->lParam, pEventRecord->wParam);
	}

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CBasePlayProcess::UpdateStateStart(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	CFblEngine::GetPtr()->m_bIsGamePlay = false; //엔진 가동 시작하지 못하게 함
	CFblArea::GetPtr()->InitArea();
	CCharacterCollisionDetector::GetPtr()->Init(CFblArea::GetPtr()->GetGroundX(), CFblArea::GetPtr()->GetGroundY(), CFblArea::GetPtr()->GetPenaltyRadius(), CFblArea::GetPtr()->m_fPenaltyWidth);

	CObjBallManager::GetPtr()->GetBallPointer()->InitBallInGame(CObjRoomManager::GetPtr()->GetRoomPointer()->m_nAttackTeam);

	m_nCameraType = m_pSelectPlayerInfo->m_cSetting.m_nCameraType;
	m_nCameraTarget = m_pSelectPlayerInfo->m_cSetting.m_nCameraTarget;
	m_nCameraTeam = m_pSelectPlayerInfo->m_cSetting.m_nCameraTeam;
	m_nCameraZoom = m_pSelectPlayerInfo->m_cSetting.m_nCameraZoom;

    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();
	m_bShadow = pkVideoInfomation->GetShadow() == 0 ? false : true;//CGameOptionDialog::GetPtr()->GetShadowOn() == 0 ? false : true;

	CObjectModelManager::GetPtr()->ShadowOnOff(m_bShadow, false);

	if( m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_VIEW || m_pRoom->m_nMode == ROOM_MODE_REPLAY )
	{
		FUNCTION_CameraChange(m_nCameraType);
		FUNCTION_ZoomChange(m_nCameraZoom);
		FUNCTION_CameraTeamChange(m_nCameraTeam);
		FUNCTION_CameraTargetChange(m_nCameraTarget);
	}

	m_nCameraTeam = CCameraController::GetPtr()->Init(m_pSelectPlayerInfo->m_nTeam, m_pSelectPlayerInfo->m_nForm, m_nCameraType, m_nCameraTarget, m_nCameraTeam, m_nCameraZoom, CFblArea::GetPtr()->GetGroundY(), m_pRoom->m_nScaleCode);

	CPacketManager::GetPtr()->m_bIsRecording = false;
}
void CBasePlayProcess::UpdateStateLoading(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_SETTING;
}
void CBasePlayProcess::UpdateStateRestart(float fFrameTime, float fAccumTime)
{
	m_nState = STATE_GAME_SETTING;
}
void CBasePlayProcess::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CInputManager::GetPtr()->Init(m_pSelectPlayerInfo, CGameProject::s_pGameProject->GetWindowReference(), CGameProject::s_pGameProject->GetInputGamePad());

	SetSoundSetting();

	//test
//	if(m_pRoom->m_nUserCount == 1)
	{
		m_nState = STATE_GAME_INIT;
	}
//	else
	{
		m_nState = STATE_PRODUCTION_INIT;
	}

	printf("Init ENd..\n");
}
void CBasePlayProcess::UpdateStateProductionInit(float fFrameTime, float fAccumTime)
{
	//업데이트 부

	if(1)//다음 단계 초기화 부
	{
		CObjBallInfo *pBall = CObjBallManager::GetPtr()->GetBallPointer();

		pBall->SetCurrentPoint();
		CObjPlayerManager::GetPtr()->SetBoneBallSize(0.85f);

		SetStartAnimation();
		m_nTime = timeGetTime();
		m_bIsRender = true;
		m_nState = STATE_PRODUCTION_PLAY;
	}
}
void CBasePlayProcess::UpdateStateProductionPlay(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(timeGetTime() - m_nTime > m_pCeremonyTable->m_fTime*1000)//다음 단계 초기화 부
	{
		CFblEngine::GetPtr()->SetMeParent(CObjRoomManager::GetPtr()->GetRoomPointer()->IsMeParent());

		m_nState = STATE_GAME_INIT;
	}
}

void CBasePlayProcess::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Reset();

	CFblEngine::GetPtr()->InitFootballTime();
	CObjPlayerManager::GetPtr()->InitAthleteInPlay();
	CObjBallManager::GetPtr()->InitBallInPlay();
	CPlayModeManager::GetPtr()->InitPlayerPointSetting();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CObjPlayerManager::GetPtr()->SetBoneBallSize(1.1f);

	m_bIsRender = true;

//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
	{
		CAIManager::GetPtr()->Init(CFblArea::GetPtr()->GetGroundX(), CFblArea::GetPtr()->GetGroundY(), CFblArea::GetPtr()->GetPenaltyRadius(), true);
		CAIManager::GetPtr()->PlayerDutyClear();

		CAIDomainManager::GetPtr()->Init(m_pRoom->m_nScaleCode);
	}
//#else
//#endif

	m_nState = STATE_GAME_READY;
}

void CBasePlayProcess::UpdateStateGameReady(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_START);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
}

void CBasePlayProcess::UpdateStateGameStart(float fFrameTime, float fAccumTime)
{
	char sText[STRLEN_64]="";

	for(int i=0;i<TEAM_SIZE*2;i++)
	{
		_snprintf(sText, STRLEN_64, "Effect_Animation%02d", i);
		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pAni);
		pAni->SetPlay(false);
		pAni->SetVisible(N3UI_VISIBLE_FALSE);
	}

	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_START);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsPlayStartEnd())//다음 단계 초기화 부
	{
		CPacketManager::GetPtr()->m_bIsRecording = true;
		m_nState = STATE_GAME_PLAY;
	
		m_pSelectPlayerInfo->SetObjectShow(RESOURCE_PASSDIRECTIONMODEL, true);

		if(m_pRoom->IsMeParent())
		{
			CObjPlayerManager::GetPtr()->SetUpdateAIMachine(true);
			m_pRoom->SetAngerMode();
			PutAngerInfo();
		}

		FUNCTION_ChangeTutorialPlayUI();

		nsGameConfig::s_fSlowStep = 1.0f;
	}
}

void CBasePlayProcess::UpdateStateGamePlay(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	CObjPlayerManager::GetPtr()->Update(fFrameTime, fAccumTime);
	UpdateAngerMode(fFrameTime, fAccumTime);

	CObjBallInfo*	pBall = CObjBallManager::GetPtr()->GetBallPointer();

	this->CoolTimeIconUpdate();
	Update(fFrameTime, fAccumTime);
	InputKey();

	if(m_pRoom->IsMeParent())
	{
		CAIDomainManager::GetPtr()->Update(fFrameTime, fAccumTime);
	}

//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
//#else
//#endif

	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_PLAY);
	CObjBallManager::GetPtr()->Update();

	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(pBall->m_bIsGoalIn)//다음 단계 초기화 부
	{
		if(m_pRoom->IsMeParent())
		{
			CFblEngine::GetPtr()->m_bIsGoalState = true;
			PutGoalIn(); //골여부와 세레모니 패킷 보내기
			CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);
		}

		if(m_pRoom->m_nPlayType == PLAY_TYPE_GOLDEN)
			UIFunction::Set_UIVisibility("Static_GoldenGoal", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//GOLDENGOAL 연출씬 세팅
		else
			UIFunction::Set_UIVisibility("Static_Goal", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//GOAL 연출씬 세팅

		this->SetShadowCamera();

		m_nTime = timeGetTime();

		m_nState = STATE_GOAL_PLAY;

		m_pSelectPlayerInfo->SetObjectShow(RESOURCE_PASSDIRECTIONMODEL, false);
		return;
	}

	//경기 종료 여부
	if(m_pRoom->m_fCurrentTime > m_pRoom->m_fPlayTime) //일반 경기 시간은 끝났다
	{
		if(m_pRoom->m_fCurrentTime > (m_pRoom->m_fPlayTime+GOLDEN_TIME)) //골든골 경기 시간까지 끝났다
		{
			if(m_pRoom->IsMeParent())
				PutGameResult();

			CPacketManager::GetPtr()->m_bIsRecording = false;

			m_nState = STATE_GAME_END;
		}
		else
		{
			if(m_pRoom->m_nPlayType == PLAY_TYPE_NORMAL) //일반 경기중
			{
				//비기고 있다면 골든골 경기로 변경
				if(m_pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL] == m_pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL])
				{
					m_pRoom->m_nPlayType = PLAY_TYPE_GOLDEN;
					UIFunction::Set_UIVisibility("Static_GoldenGoalMark",N3UI_VISIBLE_TRUE);//골든골 표시 켜기(미니맵 위에)

					UIFunction::Set_UIVisibility("Static_GoldenStart", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP);//GOLDEN 연출씬 세팅
				}
				else //비기지 않았다면 경기 종료
				{
					if(m_pRoom->IsMeParent())
						PutGameResult();

					CPacketManager::GetPtr()->m_bIsRecording = false;
					m_nState = STATE_GAME_END;
				}
			}
		}
	}
}

void CBasePlayProcess::UpdateStateGoalPlay(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	//업데이트 부
	Update(fFrameTime, fAccumTime);

//#ifdef __AITYPE__
	if(m_pRoom->IsMeParent())
		CAIManager::GetPtr()->Update(fFrameTime, fAccumTime);
//#else
//#endif

	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_GOAL);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if( m_pRoom->m_nMode == ROOM_MODE_QUEST )
		m_bNewbieHelp = true;

	if(IsRecordEnd()) //1초간 더 리플레이용 레코드 저장
	{
		CPacketManager::GetPtr()->m_bIsRecording = false;
	}

	if(IsPlayGoalEnd())//다음 단계 초기화 부
	{
		printf("End Goal..\n");
		m_pRoom->m_fStampTime = CPacketManager::GetPtr()->m_cReplayQue.SetReplayIterator(m_pRoom->m_fStampTime);

		CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();

		if((m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && pBall->m_nShootKind == BALL_SHOOT_LONGSHOOT && pBall->m_nGoalTeam == m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && pBall->m_nShootKind == BALL_SHOOT_HEADSHOOT && pBall->m_nGoalTeam != m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && pBall->m_nShootKind == BALL_SHOOT_HEADSHOOT && pBall->m_nGoalTeam == m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && pBall->m_nShootKind == BALL_SHOOT_LONGSHOOT && pBall->m_nGoalTeam != m_pSelectPlayerInfo->m_nTeam))
		{
			CCameraController::GetPtr()->ChangeCameraTarget(CAMERATARGET_BALL);
			CCameraController::GetPtr()->SetYaw(D3DXToRadian(30.f));
			CCameraController::GetPtr()->SetPitch(D3DXToRadian(90.f));
			CCameraController::GetPtr()->SetDistance(13.f);
		}
		else if((m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && pBall->m_nShootKind == BALL_SHOOT_HEADSHOOT && pBall->m_nGoalTeam == m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && pBall->m_nShootKind == BALL_SHOOT_LONGSHOOT && pBall->m_nGoalTeam != m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && pBall->m_nShootKind == BALL_SHOOT_LONGSHOOT && pBall->m_nGoalTeam == m_pSelectPlayerInfo->m_nTeam)
			|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && pBall->m_nShootKind == BALL_SHOOT_HEADSHOOT && pBall->m_nGoalTeam != m_pSelectPlayerInfo->m_nTeam))
		{
			CCameraController::GetPtr()->ChangeCameraTarget(CAMERATARGET_BALL);
			CCameraController::GetPtr()->SetYaw(D3DXToRadian(30.f));
			CCameraController::GetPtr()->SetPitch(D3DXToRadian(-90.f));
			CCameraController::GetPtr()->SetDistance(13.f);
		}
		else
		{
			if((m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && pBall->m_nGoalTeam == m_pSelectPlayerInfo->m_nTeam)
				|| (m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && pBall->m_nGoalTeam != m_pSelectPlayerInfo->m_nTeam))
			{
				CCameraController::GetPtr()->SetCameraType(D4WAY45TURNMOVE);
				CCameraController::GetPtr()->SetCameraTarget(CAMERATARGET_BALL);
				CCameraController::GetPtr()->CameraSetting( );
			}
			else
			{
				CCameraController::GetPtr()->SetCameraType(D4WAY45TURNMOVE);
				CCameraController::GetPtr()->SetCameraTarget(CAMERATARGET_BALL);
				CCameraController::GetPtr()->CameraSetting();
			}
		}

		this->SetShadowCamera();

		UIFunction::Set_UIVisibility("Static_ReplayAni", N3UI_VISIBLE_TRUE, ANIMATION_LOOP);
		UIFunction::Set_UIVisibility("ReplayMark",N3UI_VISIBLE_TRUE);
		CPacketManager::GetPtr()->m_bIsRecording = false;

		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Replay_Start");
		pAni->SetVisible(N3UI_VISIBLE_TRUE);
		pAni->SetPlay(true);		

		m_nTime = timeGetTime();

		m_nState = STATE_REPLAY_READY;
	}
}

void CBasePlayProcess::UpdateStateReplayReady(float fFrameTime, float fAccumTime)
{    
    //CObjBallManager::GetPtr()->GetBallPointer()->m_bIsGoalIn = false;
    ClothSimulationManager::GetPtr()->Reset();
    //ClothSimulationManager::GetPtr()->SetAlways( true );

	UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_FALSE);	

	if(timeGetTime() - m_nTime > 800)
	{
		m_nState = STATE_REPLAY_PLAY;
		nsGameConfig::s_fSlowStep = 0.2f;
		CObjPlayerManager::GetPtr()->SetBoneBallSize(1.0f);

		IsAngerMode();

		if(m_pRoom->IsMeParent())
			CObjPlayerManager::GetPtr()->SetUpdateAIMachine(true);
	}
}

void CBasePlayProcess::UpdateStateReplayPlay(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_REPLAY);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsReplayGoalEnd())//다음 단계 초기화 부
	{
		printf("End Replay..\n");

		SetCeremonyAnimation();

		m_nTime = timeGetTime();

		nsGameConfig::s_fSlowStep = 1.f;
		CObjPlayerManager::GetPtr()->SetBoneBallSize(0.85f);

		m_nState = STATE_CEREMONY_PLAY;
		UIFunction::Set_UIVisibility("ReplayMark",N3UI_VISIBLE_FALSE);
		ShowGoalInfo();
	}
}

void CBasePlayProcess::UpdateStateCeremonyPlay(float fFrameTime, float fAccumTime)
{
	UIFunction::Set_UIVisibility("GAMEPLAY_FRONTVIEW_BG",N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility("GAMEPLAY_SIDEVIEW_BG",N3UI_VISIBLE_FALSE);
	UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "SkillGages", N3UI_VISIBLE_FALSE );

	char sText[STRLEN_64]="";

    for(int i=0;i<TEAM_SIZE*2;i++)
	{
		_snprintf(sText, STRLEN_64, "Effect_Animation%02d", i);
		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pAni);
		pAni->SetPlay(false);
		pAni->SetVisible(N3UI_VISIBLE_FALSE);
	}

	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_CEREMONY);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsPlayCeremonyEnd())//다음 단계 초기화 부
	{
		CObjPlayerManager::GetPtr()->SetBoneBallSize(1.1f);
		m_nState = STATE_GAME_END_READY;
	}
}

void CBasePlayProcess::UpdateStateGameEndReady(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Reset();
	{
		if(m_pRoom->m_nPlayType == PLAY_TYPE_GOLDEN) //골든골이었다면 종료쪽으로
		{
			if( m_pRoom->IsMeParent() )
				PutGameResult();
			m_nState = STATE_GAME_END;

		}
		else //일반골이었다면 재시작쪽으로
		{
			m_nState = STATE_GAME_INIT;

			if(m_pRoom->IsMeParent())
				CObjPlayerManager::GetPtr()->SetUpdateAIMachine(false);

			if(m_nCameraTeam == 0)//사이드
				UIFunction::Set_UIVisibility("GAMEPLAY_SIDEVIEW_BG",N3UI_VISIBLE_TRUE);
			else
				UIFunction::Set_UIVisibility("GAMEPLAY_FRONTVIEW_BG",N3UI_VISIBLE_TRUE);

			UIFunction::Set_UIVisibility( "Dialog_Gage", N3UI_VISIBLE_TRUE );
			UIFunction::Set_UIVisibility( "SkillGages", N3UI_VISIBLE_TRUE );
		}
		HideGoalInfo();
	}
}

void CBasePlayProcess::UpdateStateGameEnd(float fFrameTime, float fAccumTime)
{
	FUNCTION_UIClear();

	//if(m_pRoom->m_nMode == ROOM_MODE_QUEST && m_pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL] > m_pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL])
	//{
	//	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
	//	if(pMember == NULL) return;

	//	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	//	if((int)pSelectPlayerInfo->m_vQuestList.size())
	//	{
	//		int nCnt = 0;
	//		for(int i=0;i<(int)pSelectPlayerInfo->m_vQuestList.size();i++)
	//		{
	//			if( pSelectPlayerInfo->m_vQuestList.at(i)->m_nCode == m_pRoom->m_nQuestCode )
	//			{
	//				pSelectPlayerInfo->m_vQuestList.at(i)->m_nAmount++;
	//				break;
	//			}
	//			nCnt++;
	//			if( nCnt == (int)pSelectPlayerInfo->m_vQuestList.size() )
	//			{
	//				CObjQuestInfo* QuestList;
	//				QuestList = new CObjQuestInfo();

	//				QuestList->m_nCode		= m_pRoom->m_nQuestCode;
	//				QuestList->m_nAmount	= 0;
	//				QuestList->m_nQuestSeq	= 0;
	//				QuestList->m_nPlayDate	= 0;

	//				pSelectPlayerInfo->m_vQuestList.push_back(QuestList);
	//				break;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		CObjQuestInfo* QuestList;
	//		QuestList = new CObjQuestInfo();

	//		QuestList->m_nCode		= m_pRoom->m_nQuestCode;
	//		QuestList->m_nAmount	= 0;
	//		QuestList->m_nQuestSeq	= 0;
	//		QuestList->m_nPlayDate	= 0;

	//		pSelectPlayerInfo->m_vQuestList.push_back(QuestList);
	//	}
	//}

	//업데이트 부
	CResSoundTableFormat *pSoundTable = NULL;

	NormalSound("game_end1.wav"); //Sound 세팅

	m_nState = STATE_END;
}

void CBasePlayProcess::UpdateStateEnd(float fFrameTime, float fAccumTime)
{
}

void CBasePlayProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		this->DrawSkillCoolTimeInfo();

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();

	}
}

void CBasePlayProcess::ProcessInput()
{
	CCameraController::GetPtr()->ProcessInput(CGameProject::s_pGameProject->GetFrameTime()); 

    if( CInputManager::GetPtr()->KeyWasPressed(KEY_P) )
    {
        ClothSimulationManager::GetPtr()->Reset();
    }

	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSHOME))
	{
		m_bShadow = !m_bShadow;
		CObjectModelManager::GetPtr()->ShadowOnOff(m_bShadow, false);
	}

	char myTeam = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->m_nTeam;

	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_H) && m_pRoom->m_nMode != ROOM_MODE_REPLAY && myTeam != PLAYER_TEAM_VIEW/*&& pSelectPlayerInfo->m_nPosition == POSITION_NB*/)
	{
		m_bNewbieHelp = !m_bNewbieHelp;

		if( m_bNewbieHelp )
			UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_TRUE);
		else
			UIFunction::Set_UIVisibility("Dialog_Help_Play01", N3UI_VISIBLE_FALSE);	
	}
	
	if(	CInputManager::GetPtr()->KeyIsDown(input::KEY_H) && m_pRoom->m_nMode != ROOM_MODE_REPLAY && myTeam != PLAYER_TEAM_VIEW )
		UIFunction::Set_UIVisibility("Dialog_Help_Play", N3UI_VISIBLE_TRUE);
	else
		UIFunction::Set_UIVisibility("Dialog_Help_Play", N3UI_VISIBLE_FALSE);

	if(STATE_GAME_READY < m_nState && myTeam != PLAYER_TEAM_VIEW /*&& 1 < CObjRoomManager::GetPtr()->UserCount()*/ )
	{
		for(int i = KEY_F1;i < KEY_F6; i++)
		{
			if(m_pRoom->m_nMode == ROOM_MODE_TOURNAMENT && CInputManager::GetPtr()->KeyWasPressed(i))
			{
				int nForm = (i - KEY_F1)%6;

				bool bIsStay = false;

				CObjPlayerInfo * pPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(bIsStay, m_pSelectPlayerInfo->m_nTeam, nForm);//해당 선수의 포인터 얻어오기

				if(pPlayer == NULL) continue;
				if(pPlayer->GetObjSeq() == m_pSelectPlayerInfo->GetObjSeq()) continue;

				PutSwitchValue(SWITCH_PLAYERCALL, 100*m_pSelectPlayerInfo->m_nForm + nForm);
			}
		}

		for(int i = KEY_F1;i < KEY_F9; i++)
		{
			if(CInputManager::GetPtr()->KeyWasPressed(i))
			{
				FUNCTION_QuickChattingPrint(i-KEY_F1);
			}
		}

		//카메라 타입 변경
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSINSERT))//
		{
			m_nCameraType = CCameraController::GetPtr()->GetCameraType();

			if(D4WAYTILTMOVE < ++m_nCameraType) m_nCameraType = D2WAYTILTMOVE;

			CCameraController::GetPtr()->ChangeCameraType((eCAMERATYPE)(m_nCameraType));
			this->SetShadowCamera();
		}
		
		//카메라 타겟 변경
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSDELETE))
		{
			m_nCameraTarget = CCameraController::GetPtr()->GetCameraTarget();

			m_nCameraTarget = m_nCameraTarget + 20;

			if(CAMERATARGET_BALLTOCHAR < m_nCameraTarget) m_nCameraTarget = CAMERATARGET_CHAR;
            //if(CAMERATARGET_REGION < m_nCameraTarget) m_nCameraTarget = CAMERATARGET_CHAR;

			CCameraController::GetPtr()->ChangeCameraTarget((eCAMERATARGET)m_nCameraTarget);
			this->SetShadowCamera();
		}

		static bool bForm = true;

		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_GRAVE))
		{
			if( bForm ) bForm = false;
			else		bForm = true;
		}

		CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

		char nTS_Lv = 0;
		char nTL_Lv = 0;

		if( pSelectPlayerInfo->GetSkillEquipCode(SKILL_TARGETSHORTPASS) != NULL )
			nTS_Lv = pSelectPlayerInfo->GetSkillEquipCode(SKILL_TARGETSHORTPASS)->m_nLevel;

		if( pSelectPlayerInfo->GetSkillEquipCode(SKILL_TARGETLONGPASS) != NULL )
			nTL_Lv = pSelectPlayerInfo->GetSkillEquipCode(SKILL_TARGETLONGPASS)->m_nLevel;

		char nHave = nTS_Lv + nTL_Lv;

		if( nHave )
		{
			if(STATE_GOAL_PLAY <= m_nState )
				UpdateFormNumber(false, myTeam, pSelectPlayerInfo->GetObjSeq(), nHave);
			else
				UpdateFormNumber(bForm, myTeam, pSelectPlayerInfo->GetObjSeq(), nHave);
		}
	}

	if(STATE_GAME_READY < m_nState && m_nCameraType != CEREMONYFIX)
	{
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSEND))//팀변경
		{
			//홈이면 0 1 어웨이면 0 2
			if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME)
				m_nCameraTeam = m_nCameraTeam == 0 ? 1 : 0;
			else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY)
				m_nCameraTeam = m_nCameraTeam == 0 ? 2 : 0;
			else
				m_nCameraTeam = (++m_nCameraTeam)%3;

			CCameraController::GetPtr()->SetCameraTeam(m_nCameraTeam);
			CCameraController::GetPtr()->CameraSetting();
			FUNCTION_ChangeTutorialPlayUI();
		}

		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSPGUP))
		{
			--m_nCameraZoom;

			if(m_nCameraZoom < 0) m_nCameraZoom = 0;

			CCameraController::GetPtr()->SetCameraZoom(m_nCameraZoom);
			CCameraController::GetPtr()->CameraSetting();
		}

		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSPGDN))
		{
			++m_nCameraZoom;

			if(CAMERAZOOM_COUNT - 1 < m_nCameraZoom) m_nCameraZoom = CAMERAZOOM_COUNT - 1;

			CCameraController::GetPtr()->SetCameraZoom(m_nCameraZoom);
			CCameraController::GetPtr()->CameraSetting();
		}
	}
    
	//////////////////////////////////////////////////////////////////////////	
	//test 나중에 삭제되야된다
	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_L))
	{
		CResourceManager::GetPtr()->DeleteResource(RESOURCE_CAMERATABLE);
		CResourceManager::GetPtr()->LoadResource(RESOURCE_CAMERATABLE);
		CCameraController::GetPtr()->CameraSetting();
	}

	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SPACE))
	{
        //m_nState = STATE_GAME_INIT;
        //ClothSimulationManager::GetPtr()->Reset();

		PutSwitchValue(SWITCH_PASSCALL, true);
//		CAIManager::GetPtr()->SetPassCall(true);
	}
}

void CBasePlayProcess::InputKey()
{
	//선수가 아니라면 입력키 무시
	if(!m_pSelectPlayerInfo || ( m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_HOME && m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_AWAY) ) return;

	CInputKey cInputKey = CInputManager::GetPtr()->GetInputKey();

	m_cSendInputKey = cInputKey;

	if(timeGetTime() - m_nInputKeySendTime > 50)
	{
		if(!(m_cBackUpInputKey == m_cSendInputKey))
		{
			m_cBackUpInputKey = m_cSendInputKey;
			m_pSelectPlayerInfo->m_cInputKey = m_cBackUpInputKey;
			PutInputKey(m_pSelectPlayerInfo->GetObjSeq(), m_pSelectPlayerInfo->m_cInputKey); //네트워크로 키 전송
		}
		m_nInputKeySendTime = timeGetTime();
	}
}

void CBasePlayProcess::Update(float fFrameTime, float fAccumTime)
{   
	m_fAccumTime = fAccumTime;

	CCharacterCollisionDetector::GetPtr()->Update(fFrameTime, fAccumTime);

	m_pPannelManager->InitPannelPoint();

	m_pPannelManager->Update(m_nState != STATE_CEREMONY_PLAY && m_nState != STATE_PRODUCTION_PLAY);

	if(STATE_GAME_READY<m_nState)
	{
		FUNCTION_UpdateTutorialPlayUI();
	}

	FUNCTION_UpdateTutorialPlayScoreUI();

	FUNCTION_UpdateTutorialPlayTimeUI();

	const DWORD& nRemainTime = m_pRoom->GetPlayTime();

	if( ((nRemainTime <= 60 ) && (nRemainTime >= 58)) || ((nRemainTime <= 30 ) && (nRemainTime >= 28)) || ((nRemainTime <= 10 ) && (nRemainTime >= 0)) && (m_nState == STATE_GAME_PLAY))
	{
		UIFunction::Set_UIVisibility("Dialog_CountNumber",N3UI_VISIBLE_TRUE);
		DisplayRemainTime(nRemainTime);
	}
	else
		UIFunction::Set_UIVisibility("Dialog_CountNumber",N3UI_VISIBLE_FALSE);
}

bool CBasePlayProcess::IsPlayStartEnd()
{
	if(timeGetTime() - m_nTime > 1500)
		return true;
	else
		return false;
}

bool CBasePlayProcess::IsRecordEnd()
{
	if(timeGetTime() - m_nTime > 1000)
		return true;
	else
		return false;
}

bool CBasePlayProcess::IsPlayGoalEnd()
{
	if(timeGetTime() - m_nTime > 3000)
		return true;
	else
		return false;
}

bool CBasePlayProcess::IsReplayGoalEnd()
{
	if(CPacketManager::GetPtr()->m_cReplayQue.RunReplayData(m_pRoom->m_fStampTime))
		return false;
	else
		return true;
}

bool CBasePlayProcess::IsPlayCeremonyEnd()
{
	if(timeGetTime() - m_nTime > m_pCeremonyTable->m_fTime*1000)
	{
		CCameraController::GetPtr()->ChangeCameraType(m_nCameraType);
		CCameraController::GetPtr()->ChangeCameraTarget(m_nCameraTarget);
		this->SetShadowCamera();

		return true;
	}
	else
		return false;
}

void CBasePlayProcess::SetStartAnimation()
{
	VectorAnyoneList*	pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);

	m_pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 101+(m_pRoom->m_nRandom%2), 0));
	assert(m_pCeremonyTable);

	int i = 0;
	int j = 0;

	CCameraController::GetPtr()->ChangeCameraType(CEREMONYFIX, m_pCeremonyTable->m_sCameraFile, m_pCeremonyTable->m_nNif );
	SetShadowCamera();

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			if((*iPlayer)->m_nTeam != PLAYER_TEAM_HOME && (*iPlayer)->m_nTeam != PLAYER_TEAM_AWAY)
				continue;

			CObjPlayerInfo* pAnyPlayer = (*iPlayer);

			pAnyPlayer->SetNodePoint(NiPoint3::ZERO);

			if(pAnyPlayer->m_nTeam == PLAYER_TEAM_HOME)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[i];
				i++;
			}
			else
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nLoser[j];
				j++;
			}
		}
	}

	m_nTime = timeGetTime();
}

void CBasePlayProcess::SetCeremonyAnimation()
{
	CObjBallInfo*		pBall		= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList*	pAnyoneList = &(CObjPlayerManager::GetPtr()->m_vTotalList);
	CObjPlayerInfo*		pKickPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBall->m_nKickSeq);

	if(pBall->m_nGoalType == GOAL_TYPE_GOLDEN)//골든골 세팅
		m_pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1091, 0));
	else if(pBall->m_nGoalType == GOAL_TYPE_OWN)//자살골 세팅
		m_pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1081, 0));
	else
	{
		if(pRoom->m_nCeremonyCode == 0)
			return;
		m_pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, pRoom->m_nCeremonyCode, 0));
//        m_pCeremonyTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, 1005, 0));
	}

	assert(m_pCeremonyTable);

	int i = 1;
	int j = 0;

	CCameraController::GetPtr()->ChangeCameraType(CEREMONYFIX, m_pCeremonyTable->m_sCameraFile, m_pCeremonyTable->m_nNif);
	this->SetShadowCamera();

	for(VectorAnyoneList::iterator iAnyone = pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			if((*iPlayer)->m_nTeam==PLAYER_TEAM_VIEW)
				continue;

			CObjPlayerInfo* pAnyPlayer = (*iPlayer);
			if(pAnyPlayer->GetObjSeq() == pKickPlayer->GetObjSeq()) 
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[0];
			}
			else if(pAnyPlayer->m_nTeam == pKickPlayer->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nWinner[i];
				i++;
			}
			else if(pAnyPlayer->m_nTeam != pKickPlayer->m_nTeam)
			{
				pAnyPlayer->m_cCurrentAction.m_nAniCode = m_pCeremonyTable->m_nLoser[j];
				j++;
			}
		}
	}

	m_nTime = timeGetTime();
}

int CBasePlayProcess::GetCameraType()
{
	return m_nCameraType;
}

int CBasePlayProcess::GetCameraTarget()
{
	return m_nCameraTarget;
}

int CBasePlayProcess::GetCameraTeam()
{
	return m_nCameraTeam;
}

int CBasePlayProcess::GetCameraZoom()
{
	return m_nCameraZoom;
}

int CBasePlayProcess::GetCameraRadian()
{
	return m_nRadian;
}

void CBasePlayProcess::SetCameraType(int nType)
{
	m_nCameraType = nType;
	CCameraController::GetPtr()->SetCameraType(m_nCameraType);
	CCameraController::GetPtr()->CameraSetting();
}

void CBasePlayProcess::SetCameraTarget(int nTarget)
{
	m_nCameraTarget = nTarget;
	CCameraController::GetPtr()->SetCameraTarget(m_nCameraTarget);
	CCameraController::GetPtr()->CameraSetting();
}

void CBasePlayProcess::SetCameraTeam(int nTeam)
{
	m_nCameraTeam = nTeam;
	CCameraController::GetPtr()->SetCameraTeam(m_nCameraTeam);
	CCameraController::GetPtr()->CameraSetting();
}

void CBasePlayProcess::SetCameraZoom(int nZoom)
{
	m_nCameraZoom = nZoom;
	CCameraController::GetPtr()->SetCameraZoom(m_nCameraZoom);
	CCameraController::GetPtr()->CameraSetting();
}

void CBasePlayProcess::SetCameraRadian(int nRadian)
{
	m_nRadian = nRadian;
}

void CBasePlayProcess::IsAngerMode()
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl( "AngerAni" );
	assert(pAni);

	if(CObjectModelManager::GetPtr()->GetAngerMode())
	{
		CAudioManager::GetPtr()->MusicVolume(CGameOptionDialog::GetPtr()->GetMusicVol());
		SetObjectMaterialColorSetting(false);
		CObjectModelManager::GetPtr()->SetAngerMode( false );
        char nShadow = VideoInfomation::GetPtr()->GetShadow();//CGameOptionDialog::GetPtr()->GetShadowOn();
        CObjectModelManager::GetPtr()->ShadowOnOff( nShadow == 0 ? false : true, false);

		pAni->SetVisible(N3UI_VISIBLE_FALSE);
		pAni->SetPlay(false);

		UIFunction::Set_UIVisibility("Progress_Fury", N3UI_VISIBLE_FALSE);
		UIFunction::Set_UIVisibility("Fury_Ball_BG", N3UI_VISIBLE_FALSE);
	}
}

void CBasePlayProcess::UpdateAngerMode(float fFrameTime, float fAccumTime)
{
	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl( "AngerAni" );
	assert(pAni);

	if(m_pRoom->IsMeParent())
	{
		if(m_pRoom->m_nHomeAngerTime != 0)
		{
			m_pRoom->m_fTotalAngerTime += fFrameTime;
			if(m_pRoom->m_fTotalAngerTime > m_pRoom->m_nHomeAngerTime)
			{
				m_pRoom->m_nHomeAngerTime = 0;
				PutAngerInfo();
			}
		}
		if(m_pRoom->m_nAwayAngerTime != 0)
		{
			m_pRoom->m_fTotalAngerTime += fFrameTime;
			if(m_pRoom->m_fTotalAngerTime > m_pRoom->m_nAwayAngerTime)
			{
				m_pRoom->m_nAwayAngerTime = 0;
				PutAngerInfo();
			}
		}
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && m_pRoom->m_nHomeAngerTime != 0)
	{
		m_pRoom->m_fTotalAngerTime += fFrameTime;
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && m_pRoom->m_nAwayAngerTime != 0)
	{
		m_pRoom->m_fTotalAngerTime += fFrameTime;
	}

	if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME && m_pRoom->m_nHomeAngerTime != 0)
	{
		if(!CObjectModelManager::GetPtr()->GetAngerMode())
		{
			CAudioManager::GetPtr()->MusicVolume(0);
			SetObjectMaterialColorSetting(true);
			CObjectModelManager::GetPtr()->SetAngerMode( true );
			CObjectModelManager::GetPtr()->ShadowOnOff(false, false);

			pAni->SetVisible(N3UI_VISIBLE_TRUE);
			pAni->SetPlay(true, true, false);

			m_pRoom->m_fTotalAngerTime = 0;
		}
		UIFunction::SetProgressValue("Progress_Fury", (m_pRoom->m_fTotalAngerTime/((float)m_pRoom->m_nHomeAngerTime))*100);
		UIFunction::SetFrameValue("Fury_Ball_BG", "Static_Fury_Ball", (m_pRoom->m_fTotalAngerTime/((float)m_pRoom->m_nHomeAngerTime))*100);
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_HOME)
	{
		if(CObjectModelManager::GetPtr()->GetAngerMode())
		{
			CAudioManager::GetPtr()->MusicVolume(CGameOptionDialog::GetPtr()->GetMusicVol());
			SetObjectMaterialColorSetting(false);
			CObjectModelManager::GetPtr()->SetAngerMode( false );
            char nShadow = VideoInfomation::GetPtr()->GetShadow();//CGameOptionDialog::GetPtr()->GetShadowOn();
            CObjectModelManager::GetPtr()->ShadowOnOff( nShadow == 0 ? false : true, false);

			pAni->SetVisible(N3UI_VISIBLE_FALSE);
			pAni->SetPlay(false);

			UIFunction::Set_UIVisibility("Progress_Fury", N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility("Fury_Ball_BG", N3UI_VISIBLE_FALSE);
		}
	}

	if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY && m_pRoom->m_nAwayAngerTime != 0)
	{
		if(!CObjectModelManager::GetPtr()->GetAngerMode())
		{
			CAudioManager::GetPtr()->MusicVolume(0);
			SetObjectMaterialColorSetting(true);
			CObjectModelManager::GetPtr()->SetAngerMode( true );
			CObjectModelManager::GetPtr()->ShadowOnOff(false, false);

			pAni->SetVisible(N3UI_VISIBLE_TRUE);
			pAni->SetPlay(true, true, false);

			m_pRoom->m_fTotalAngerTime = 0;
		}
		UIFunction::SetProgressValue("Progress_Fury", (m_pRoom->m_fTotalAngerTime/((float)m_pRoom->m_nAwayAngerTime))*100);
		UIFunction::SetFrameValue("Fury_Ball_BG", "Static_Fury_Ball", (m_pRoom->m_fTotalAngerTime/((float)m_pRoom->m_nAwayAngerTime))*100);
	}
	else if(m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_AWAY)
	{
		if(CObjectModelManager::GetPtr()->GetAngerMode())
		{
			CAudioManager::GetPtr()->MusicVolume(CGameOptionDialog::GetPtr()->GetMusicVol());
			SetObjectMaterialColorSetting(false);
			CObjectModelManager::GetPtr()->SetAngerMode( false );
            //char nShadow = VideoInfomation::GetPtr()->GetShadow();//CGameOptionDialog::GetPtr()->GetShadowOn();
			CObjectModelManager::GetPtr()->ShadowOnOff( false, false);

			pAni->SetVisible(N3UI_VISIBLE_FALSE);
			pAni->SetPlay(false);

			UIFunction::Set_UIVisibility("Progress_Fury", N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility("Fury_Ball_BG", N3UI_VISIBLE_FALSE);
		}
	}
}

void CBasePlayProcess::SetSoundSetting(bool bAnger)
{
	if(bAnger)
	{
//		CAudioManager::GetPtr()->StopMusic();
	}
	else if(strlen(CGameOptionDialog::GetPtr()->LoadUserBGM(CGameProject::s_pGameProject->s_nBGM)))
	{
		CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\Music\\");
		CAudioManager::GetPtr()->CreateMusic(CGameOptionDialog::GetPtr()->LoadUserBGM(CGameProject::s_pGameProject->s_nBGM), true);//사운드 플레이
		CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\BGM\\");
	}
	else
	{
		CResOggTableFormat *temp = dynamic_cast<CResOggTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OGGTABLE, Random(2)+1, 0));
		assert(temp);

		CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\BGM\\");
		CAudioManager::GetPtr()->CreateMusic(temp->m_sDataFile, true);//사운드 플레이
	}
}

void CBasePlayProcess::SetObjectMaterialColorSetting(bool bAnger)
{
	if( bAnger )
	{
		CoreApi_RecursiveSaveMaterialColor( m_pSelectPlayerInfo->GetRootNode() );

		CoreApi_RecursiveSetAmbientToMesh( m_pSelectPlayerInfo->GetRootNode(), NiColor(1.0f,0.5f,0.5f) );
		CoreApi_RecursiveSetEmissiveToMesh( m_pSelectPlayerInfo->GetRootNode(), NiColor(1.0f,0.5f,0.5f) );
		CoreApi_RecursiveSetDiffuseToMeshEffect( m_pSelectPlayerInfo->GetRootNode(), NiColor(1.0f,0.5f,0.5f) );
	}
	else
	{
		CoreApi_RecursiveRestoreMaterialColor( m_pSelectPlayerInfo->GetRootNode() );
	}
}

void CBasePlayProcess::SetSelectPlayer(CObjPlayerInfo* pSelectPlayerInfo)
{
	m_pSelectPlayerInfo = pSelectPlayerInfo;

	CInputManager::GetPtr()->ReInit(m_pSelectPlayerInfo);

	CCameraController::GetPtr()->SetCameraForm(m_pSelectPlayerInfo->m_nForm + (m_pSelectPlayerInfo->m_nTeam-1)*6);
	CCameraController::GetPtr()->SetCameraTeam(m_nCameraTeam);
	CCameraController::GetPtr()->CameraSetting();
}