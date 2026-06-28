#include "stdafx.h"
#include "ReplayProcess.h"
#include "ReplayMainProcess.h"
#include "UICommonMethod.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "MovieManager.h"
#include "UIFunction.h"

using namespace MovieManager;

CReplayProcess::CReplayProcess() : CBasePlayProcess()
{

}

CReplayProcess::~CReplayProcess(void)
{
}

void CReplayProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pPannelManager = new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode,pObjGroundInfo->m_pModel->m_spLightNode);

	m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	
	m_pRoom->m_nRandom = 0;

	CPlayModeManager::GetPtr()->ReSetupWindow(m_nProcessCmd*10000);

	CPlayModeManager::GetPtr()->SetupRobotLoading();
	CPlayModeManager::GetPtr()->SetupUI();
	CObjPlayerManager::GetPtr()->InitFormPriority();

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerViewPointer();//본인위치

	m_nCameraTarget = m_pSelectPlayerInfo->m_cSetting.m_nCameraTarget;
	m_nCameraTeam	= m_pSelectPlayerInfo->m_cSetting.m_nCameraTeam;
	m_nCameraType	= m_pSelectPlayerInfo->m_cSetting.m_nCameraType;
	m_nCameraZoom	= m_pSelectPlayerInfo->m_cSetting.m_nCameraZoom;

	CoolTimeIconInit();

	nsGameConfig::s_fStateSpeed = 1.0f;
	nsGameConfig::s_fSlowSpeed = 1.0f;
	
	FUNCTION_UIClear();

	TiXmlElement* pGameSoundXml = CGameProject::s_pGameProject->GetTiXml("OptionSound");

	// 배경음 설정
	TiXmlNode* pChild = pGameSoundXml->FirstChild( "BGSound" );
	TiXmlElement* pElement = (TiXmlElement*)pChild;

	m_nMusic = (char)atoi( pElement->Attribute( "Volume" ) );

	// 효과음 설정
	pChild = pChild->NextSibling( "Sound" );
	pElement = (TiXmlElement*)pChild;

	m_nSound = atoi( pElement->Attribute( "Volume" ) );

	FUNCTION_VolumeInit();

	m_nState = STATE_START;
}

void CReplayProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{	
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);    
	CEffectManager::GetPtr()->Update( fAccumTime );
}

void CReplayProcess::Terminate()
{
	SAFE_DELETE(m_pPannelManager);

	nsGameConfig::s_fStateSpeed = 1.0f;
	nsGameConfig::s_fSlowSpeed = 1.f;

	CObjBallManager::GetPtr()->GetBallPointer()->Terminate();

	CPacketManager::GetPtr()->m_bIsRobotInfoDone = false;

	CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

	m_pRoom->Init();

	CEffectManager::GetPtr()->Clear();

	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CReplayProcess::UpdateStateStart(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	CBasePlayProcess::UpdateStateStart(fFrameTime, fAccumTime);

	m_nState = STATE_LOADING;
}

void CReplayProcess::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameSetting(fFrameTime, fAccumTime);

	m_nState = STATE_GAME_INIT;

	CObjPlayerManager::GetPtr()->CreateAIMachine(0);//게임 초기화 부분
}

void CReplayProcess::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Reset();
	CBasePlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);

	CCameraController::GetPtr()->ChangeCameraType(m_nCameraType);//뷰어일때는 변경된거 기역해야된다

	CCameraController::GetPtr()->ChangeCameraTarget(m_nCameraTarget);
	this->SetShadowCamera();

	m_bEndReady = false;
}

void CReplayProcess::UpdateStateGameReady(float fFrameTime, float fAccumTime)
{
	FUNCTION_UIInit();
    ClothSimulationManager::GetPtr()->Update( fAccumTime );

	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
	
	UIFunction::Set_UIVisibility( "MuteOnButton", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "MuteOffButton", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "PauseButton", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "PlayButton", N3UI_VISIBLE_FALSE );

	FUNCTION_VolumeUpdate();
	FUNCTION_CameraTargetChange(m_nCameraTarget);
	FUNCTION_CameraTeamChange(m_nCameraTeam);
	FUNCTION_CameraChange(m_nCameraType);
	FUNCTION_ZoomChange(m_nCameraZoom);

	//업데이트 부
	Update(fFrameTime, fAccumTime);

	CObjBallManager::GetPtr()->Update();
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_REPLAY);
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);

	UIFunction::Set_UIVisibility("Static_Start", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP); //START 연출씬 세팅
	m_nTime = timeGetTime();

	NormalSound("game_start1.wav"); //Sound 세팅

	m_nState = STATE_GAME_START;
}

void CReplayProcess::UpdateStateGameStart(float fFrameTime, float fAccumTime)
{
	//업데이트 부
    ClothSimulationManager::GetPtr()->Update( fAccumTime );

	char sText[STRLEN_64]="";

	for(int i=0;i<TEAM_SIZE*2;i++)
	{
		_snprintf(sText, STRLEN_64, "Effect_Animation%02d", i);
		CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pAni);
		pAni->SetPlay(false);
		pAni->SetVisible(N3UI_VISIBLE_FALSE);
	}

	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_REPLAY);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	if(IsPlayStartEnd())//다음 단계 초기화 부
	{
		m_nState = STATE_REPLAY_PLAY;
		FUNCTION_ChangeTutorialPlayUI();
	}
}

void CReplayProcess::UpdateStateReplayPlay(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update( fAccumTime );
	//업데이트 부
	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_REPLAY);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();
	char sText[STRLEN_64]="";
	CUIControl* pSpeed = CClientUIManager::GetPtr()->FindEventControl("Static_Speed");
	assert(pSpeed);
	_snprintf(sText, STRLEN_64, "x%0.1f", nsGameConfig::s_fSlowSpeed);
	UIFunction::Set_ControlText( "Static_Speed", sText );

	if(pBall->m_bIsGoalIn)
	{
		m_nState = STATE_GAME_END_READY;
		m_nTime = timeGetTime();
	}

	if(IsReplayGoalEnd())//다음 단계 초기화 부
	{
		printf("End Replay..\n");
		m_nTime = timeGetTime();
		CProcessManager::GetPtr()->SetNextProcess(new CReplayMainProcess);

		m_nState = STATE_END;
	}
}

void CReplayProcess::UpdateStateGameEndReady(float fFrameTime, float fAccumTime)
{
    ClothSimulationManager::GetPtr()->Update(fAccumTime);

	Update(fFrameTime, fAccumTime);
	CFootballManager::GetPtr()->UpdateFootball(fFrameTime, fAccumTime, PLAY_STATE_REPLAY);
	CObjBallManager::GetPtr()->Update();
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CCameraController::GetPtr()->UpdateCamera(fFrameTime, fAccumTime);

	CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();

	CPacketManager::GetPtr()->m_cReplayQue.RunReplayData(m_pRoom->m_fStampTime);

	CUIControl* pAni = CClientUIManager::GetPtr()->FindEventControl("Replay_Start");
	assert(pAni);

	if(!m_bEndReady && timeGetTime() - m_nTime > 1000)
	{
		pAni->SetVisible(N3UI_VISIBLE_TRUE);
		pAni->SetPlay(true);
		m_bEndReady = true;
	}
	else if(m_bEndReady && (pAni->GetFrame() >= 17 || pAni->GetVisible() == N3UI_VISIBLE_FALSE) )
	{
		if(m_pRoom->m_nPlayType == PLAY_TYPE_GOLDEN) //골든골이었다면 종료쪽으로
		{
			m_nState = STATE_GAME_END;
		}
		else //일반골이었다면 재시작쪽으로
		{
			m_nState = STATE_GAME_INIT;

			if(m_nCameraTeam == 0)//사이드
				UIFunction::Set_UIVisibility("GAMEPLAY_SIDEVIEW_BG",N3UI_VISIBLE_TRUE);
			else
				UIFunction::Set_UIVisibility("GAMEPLAY_FRONTVIEW_BG",N3UI_VISIBLE_TRUE);
		}
		HideGoalInfo();
		m_bEndReady = false;
	}
}

void CReplayProcess::ProcessInput()
{
	CBasePlayProcess::ProcessInput();

	if(STATE_GAME_READY < m_nState)
	{
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_TAB))
		{
			CObjPlayerInfo*		pAnyPlayer;
			VectorAnyoneList*	pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);

			m_pSelectPlayerInfo->m_cSetting.m_nLabel++;
			if(m_pSelectPlayerInfo->m_cSetting.m_nLabel == 3)
				m_pSelectPlayerInfo->m_cSetting.m_nLabel = 0;

			int	i = 0;

			for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
			{
				for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
				{
					pAnyPlayer = (*iPlayer);
					if(pAnyPlayer->m_nTeam==PLAYER_TEAM_VIEW)
						continue;

					InitPositionMarkPoint(i, pAnyPlayer);
					UpdatePositionMarkPoint(i, pAnyPlayer);
					i++;
				}
			}
		}
	}

	if(STATE_GAME_READY < m_nState && m_pSelectPlayerInfo->m_nTeam == PLAYER_TEAM_VIEW)
	{
		//도움말 표시
		if(CInputManager::GetPtr()->KeyIsDown(input::KEY_H))
			UIFunction::Set_UIVisibility( "Dialog_Help_View", N3UI_VISIBLE_TRUE);
		else
			UIFunction::Set_UIVisibility( "Dialog_Help_View", N3UI_VISIBLE_FALSE);

		for(int i = KEY_F1; i <= KEY_F12; i++)
		{
			if(CInputManager::GetPtr()->KeyWasPressed(i))
			{
				int nTeam = (i - KEY_F1)/6 + 1;
				int nForm = (i - KEY_F1)%6;

				bool bIsStay = false;
				CObjPlayerManager::GetPtr()->GetPlayerPointer(bIsStay, nTeam, nForm);//해당 선수의 포인터 얻어오기

				if(bIsStay)
				{
					if(m_nCameraTeam == 0)	m_nCameraTeam = 0;
					else					m_nCameraTeam = nTeam;
					CCameraController::GetPtr()->SetCameraForm(i - KEY_F1);
					CCameraController::GetPtr()->SetCameraTeam(m_nCameraTeam);
					CCameraController::GetPtr()->CameraSetting();
				}
			}
		}

		//카메라 타입 변경
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSINSERT))//
		{
			m_nCameraType = CCameraController::GetPtr()->GetCameraType();

			if(ROTATEENDFIX < ++m_nCameraType) m_nCameraType = D2WAYTILTMOVE;

			CCameraController::GetPtr()->ChangeCameraType((eCAMERATYPE)(m_nCameraType));
			this->SetShadowCamera();
		}
		//카메라 타겟 변경
		if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_SYSDELETE))
		{
			m_nCameraTarget = CCameraController::GetPtr()->GetCameraTarget();

			m_nCameraTarget = m_nCameraTarget + 20;

			if(CAMERATARGET_REGION < m_nCameraTarget) m_nCameraTarget = CAMERATARGET_CHAR;

			CCameraController::GetPtr()->ChangeCameraTarget((eCAMERATARGET)m_nCameraTarget);
			this->SetShadowCamera();
		}

		for(int i = KEY_1; i <= KEY_7; i++)
		{
			if(CInputManager::GetPtr()->KeyWasPressed(i))
			{
				CCameraController::GetPtr()->ChangeCameraType((eCAMERATYPE)(D2WAYTILTMOVE + i - KEY_1));
				this->SetShadowCamera();
			}
		}
	}

	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_ESCAPE))
	{
		FUNCTION_UIClear();
	
		CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	
		SetSound(pObjPlayerInfo->m_cSetting.m_nSound);
		SetMusic(pObjPlayerInfo->m_cSetting.m_nMusic);
		FUNCTION_VolumeInit();

		CResOggTableFormat *temp = dynamic_cast<CResOggTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OGGTABLE, 0 ,0));
		assert(temp);
		
		CGameOptionDialog::GetPtr()->XmlSetting();

		// 배경음 셋팅
		if(strlen(CGameOptionDialog::GetPtr()->LoadUserBGM(0)))
		{
			CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\Music\\");
			CAudioManager::GetPtr()->CreateMusic(CGameOptionDialog::GetPtr()->LoadUserBGM(0), true);//사운드 플레이
			CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\BGM\\");
		}
		else
			CAudioManager::GetPtr()->CreateMusic(temp->m_sDataFile, true);//사운드 플레이

		CProcessManager::GetPtr()->SetNextProcess(new CReplayMainProcess);
		m_nState = STATE_GAME_END;
	}
}

void CReplayProcess::FUNCTION_VolumeUpdate()
{
	CObjMemberInfo* pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
	assert(pMember);

	int vol = (m_nSound*2) + m_nMusic;

	vol *= 10;
	vol /= 4;

	CUIProgress* pProgress = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("Volume_progress"));
	assert(pProgress);
    
	pProgress->SetValue(vol);
}

void CReplayProcess::FUNCTION_VolumeInit()
{
	CAudioManager::GetPtr()->SoundVolume(((float)m_nSound/10));
	CAudioManager::GetPtr()->MusicVolume( m_nMusic );
	SetVolume(m_nMusic);
}

char CReplayProcess::GetSound()
{
	return m_nSound;
}

char CReplayProcess::GetMusic()
{
	return m_nMusic;
}

void CReplayProcess::SetSound(char nSound)
{
	m_nSound = nSound;
}

void CReplayProcess::SetMusic(char nMusic)
{
	m_nMusic = nMusic;
}

bool CReplayProcess::OnDefault(NiEventRef pEventRecord)
{
	MsgProc( pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam );

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CReplayProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_LEFT:
				CPacketManager::GetPtr()->m_cReplayQue.RewindReplayIterator(11.0f);
				break;
			case VK_RIGHT:
				CPacketManager::GetPtr()->m_cReplayQue.ForwardReplayIterator(9.0f);
				break;
			case VK_UP:
				{
					nsGameConfig::s_fStateSpeed += (float)0.1;

					if(nsGameConfig::s_fStateSpeed > 2.0)
						nsGameConfig::s_fStateSpeed = 2.0;

					nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
				}
				break;
			case VK_DOWN:
				{
					nsGameConfig::s_fStateSpeed -= (float)0.1;

					if(nsGameConfig::s_fStateSpeed < 0.2)
						nsGameConfig::s_fStateSpeed = (float)0.2;

					nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
				}
				break;
			}
		}
		break;
	}
}
	