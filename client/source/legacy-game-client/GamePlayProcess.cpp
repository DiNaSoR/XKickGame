#include "stdafx.h"
#include "GamePlayProcess.h"
#include "PutPacket.h"
#include "UIFunction.h"

CGamePlayProcess::CGamePlayProcess() : CBasePlayProcess()
{
    m_nSnowRainEffectSeqNo = -1;
}

CGamePlayProcess::~CGamePlayProcess(void)
{
    if( m_nSnowRainEffectSeqNo != -1 ) CEffectManager::GetPtr()->RemoveBySeqNo( m_nSnowRainEffectSeqNo );
}

void CGamePlayProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	m_pPannelManager = new CObjPannelManager(pObjGroundInfo->m_pModel->m_spNode,pObjGroundInfo->m_pModel->m_spLightNode);

	m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	CPlayModeManager::GetPtr()->LoadFile(".\\Script\\GameScript.xml");
	CPlayModeManager::GetPtr()->ReSetupWindow(m_nProcessCmd*10000);
	CObjPlayerManager::GetPtr()->InitFormPriority();

	CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	m_pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);//본인위치

	memcpy(&m_pSelectPlayerInfo->m_cSetting, &pSelectPlayerInfo->m_cSetting, sizeof(pSelectPlayerInfo->m_cSetting));

	memcpy(m_pSelectPlayerInfo->m_bIsUseKey, pSelectPlayerInfo->m_bIsUseKey, sizeof(pSelectPlayerInfo->m_bIsUseKey));
	
	_snprintf(m_pSelectPlayerInfo->m_sName, PLAYER_NAME_SIZE, pSelectPlayerInfo->m_sName);

	m_nTime = timeGetTime();

	PutGameLoad(100);

	m_nState = STATE_START;    

    CObjRoomInfo * pRoomInfo = CObjRoomManager::GetPtr()->GetRoomPointer();
    switch( pRoomInfo->m_nWeatherCode )
    {
    case ROOM_WEATHER_RAIN:
        m_nSnowRainEffectSeqNo = CEffectManager::GetPtr()->CreateEffect( 7, NiPoint3(0.0f,0.0f,0.0f) );
    break;
    case ROOM_WEATHER_SNOW:
        m_nSnowRainEffectSeqNo = CEffectManager::GetPtr()->CreateEffect( 6, NiPoint3(0.0f,0.0f,0.0f) );
    break;
    }  
}

void CGamePlayProcess::ProcessInput()
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

		for(int i = KEY_F1; i <= KEY_F10; i++)
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

	if(CInputManager::GetPtr()->KeyWasPressed(input::KEY_F9))
	{
		static DWORD sAutotime = 0;

		CObjPlayerInfo* pPlayerInfo = CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL);

		if(pPlayerInfo->m_nPosition != POSITION_NB) 
		{
			if(pPlayerInfo->GetKind() == PLAYER_KIND_USERBOT)
			{
				PutAutopilotMode(false);
				_snprintf(pPlayerInfo->m_sHeadwordFile, 128, "Auto_Pilot_Off");
			}
			else //if(timeGetTime() - sAutotime > 30000)
			{
				sAutotime = timeGetTime();
				PutAutopilotMode(true);
				_snprintf(pPlayerInfo->m_sHeadwordFile, 128, "Auto_Pilot_On");
			}
		}
	}

	//마우스 포인터 숨기기
	{
		static DWORD sDeltatime = 0;

		if(CInputManager::GetPtr()->GetMouseIsMove())
		{
			CGameProject::s_pGameProject->ShowPointer();	

			sDeltatime = timeGetTime();
		}
		else if(timeGetTime() - sDeltatime > 3000)
			CGameProject::s_pGameProject->HidePointer();
	}

    if( CInputManager::GetPtr()->KeyWasPressed(KEY_R))
    {
        if( m_nSnowRainEffectSeqNo != -1 ) CEffectManager::GetPtr()->RemoveBySeqNo( m_nSnowRainEffectSeqNo );

        m_nSnowRainEffectSeqNo = CEffectManager::GetPtr()->CreateEffect( 7, NiPoint3(0.0f,0.0f,0.0f) );
    }

    if( CInputManager::GetPtr()->KeyWasPressed(KEY_N))
    {
        if( m_nSnowRainEffectSeqNo != -1 ) CEffectManager::GetPtr()->RemoveBySeqNo( m_nSnowRainEffectSeqNo );

        m_nSnowRainEffectSeqNo = CEffectManager::GetPtr()->CreateEffect( 6, NiPoint3(0.0f,0.0f,0.0f) );
    }

	if(m_pRoom->IsMeParent() && !strcmp(CChattingDialog::GetPtr()->m_cChatting, "Exit"))
	{
		memset(CChattingDialog::GetPtr()->m_cChatting, 0, sizeof(CChattingDialog::GetPtr()->m_cChatting));
		PutGameResult();
		m_nState = STATE_GAME_END;		
	}
}

void CGamePlayProcess::UpdateStateStart(float fFrameTime, float fAccumTime)
{
	if(CObjPlayerManager::GetPtr()->Check_ProgressFull(100))
	{
		CBasePlayProcess::UpdateStateStart(fFrameTime, fAccumTime);

		CPacketManager::GetPtr()->m_cReplayQue.InitReplayData(); //리플레이 초기화
		CPacketManager::GetPtr()->m_cReplayQue.SaveReplayHeader(); //리플레이 헤더 저장

		CPlayModeManager::GetPtr()->SetupUI();
		CoolTimeIconInit();
		FUNCTION_UIClear();
		m_nState = STATE_LOADING;
	}
}

void CGamePlayProcess::UpdateStateGameSetting(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameSetting(fFrameTime, fAccumTime);

	CObjPlayerManager::GetPtr()->CreateAIMachine(0);//게임 초기화 부분
}

void CGamePlayProcess::UpdateStateGameInit(float fFrameTime, float fAccumTime)
{
	CBasePlayProcess::UpdateStateGameInit(fFrameTime, fAccumTime);

	PutGamePlay();

	if(m_pSelectPlayerInfo->m_nTeam != PLAYER_TEAM_VIEW)
		CCameraController::GetPtr()->ChangeCameraType(D4WAYTILTMOVE);//경기중에는 이방식
	else
		CCameraController::GetPtr()->ChangeCameraType(m_nCameraType);//뷰어일때는 변경된거 기역해야된다

	CCameraController::GetPtr()->ChangeCameraTarget(m_nCameraTarget);
	this->SetShadowCamera();

	CPacketManager::GetPtr()->m_bIsRecording = false;
}

void CGamePlayProcess::UpdateStateGameReady(float fFrameTime, float fAccumTime)
{
	//업데이트 부
	CBasePlayProcess::UpdateStateGameReady(fFrameTime, fAccumTime);

	if(CPacketManager::GetPtr()->m_bIsGamePlayDone)//연출 세팅을 위한 플레그
	{
		CPacketManager::GetPtr()->m_bIsGamePlayDone = false;

		CInputManager::GetPtr()->InitInputKey();

		FUNCTION_UIInit();

		UIFunction::Set_UIVisibility("Static_Start", N3UI_VISIBLE_TRUE, ANIMATION_CLAMP); //START 연출씬 세팅
		m_nTime = timeGetTime();

		NormalSound("game_start1.wav"); //Sound 세팅

		m_nState = STATE_GAME_START;
	}
}