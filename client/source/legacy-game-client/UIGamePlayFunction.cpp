#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include "GameResultProcess.h"
#include "ResCharacterFormat.h"
#include "ReplayMainProcess.h"
#include "BasePlayProcess.h"
#include "ReplayProcess.h"
#include "MovieManager.h"
#include "PutPacket.h"
#include "UIFunction.h"
#include "TutorialPlayProcess.h"

using namespace MovieManager;

IMPLEMENT_USERFUNCTION( GamePlay_OK ) 
{
	//테스트 UI
	CProcessManager::GetPtr()->SetNextProcess(new CGameResultProcess);
}

//////////////////////////////////////////////////////////////////////////
// 리플레이 컨트롤 커맨드

IMPLEMENT_USERFUNCTION( RPC_VOLPLUS )		// 볼륨 +
{
	CReplayProcess* pCurrentProcess = dynamic_cast<CReplayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	char nSound = pCurrentProcess->GetSound();

	nSound+=2;
	if(10<nSound)
		nSound = 10;
	CAudioManager::GetPtr()->SoundVolume(((float)nSound/10));
	pCurrentProcess->SetSound(nSound);

	char nMusic = pCurrentProcess->GetMusic();

	nMusic+=4;
	if(20<nMusic)
		nMusic = 20;
	CAudioManager::GetPtr()->MusicVolume( nMusic );
	SetVolume(nMusic);
	pCurrentProcess->SetMusic( nMusic );

	pCurrentProcess->FUNCTION_VolumeUpdate();
}

IMPLEMENT_USERFUNCTION( RPC_VOLMINUS )		// 볼륨 -
{
	CReplayProcess* pCurrentProcess = dynamic_cast<CReplayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	char nSound = pCurrentProcess->GetSound();

	nSound-=2;
	if(nSound<0)
		nSound = 0;
	CAudioManager::GetPtr()->SoundVolume(((float)nSound/10));
	pCurrentProcess->SetSound(nSound);

	char nMusic = pCurrentProcess->GetMusic();

	nMusic-=4;
	if(nMusic<0)
		nMusic = 0;
	CAudioManager::GetPtr()->MusicVolume( nMusic );
	SetVolume(nMusic);
	pCurrentProcess->SetMusic( nMusic );

	pCurrentProcess->FUNCTION_VolumeUpdate();
}

IMPLEMENT_USERFUNCTION( RPC_MIN )			// 리플레이 리모컨 최소화
{
	UIFunction::Set_UIVisibility( "Dialog_RemoteCamera", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_RemoteMin", N3UI_VISIBLE_TRUE );	
}

IMPLEMENT_USERFUNCTION( RPCCMC_MAX )		// 최대화
{
	CObjRoomInfo* m_pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	if(m_pRoom->m_nMode == ROOM_MODE_REPLAY)
		UIFunction::Set_UIVisibility( "Dialog_RemoteCamera", N3UI_VISIBLE_TRUE );
	else
		UIFunction::Set_UIVisibility( "Dialog_RemoteMovie", N3UI_VISIBLE_TRUE );

	UIFunction::Set_UIVisibility( "Dialog_RemoteMin", N3UI_VISIBLE_FALSE );	
}

IMPLEMENT_USERFUNCTION( RPC_QUIT )			// 리플레이 종료
{
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CReplayProcess* pCurrentProcess = dynamic_cast<CReplayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->SetSound(pObjPlayerInfo->m_cSetting.m_nSound);
	pCurrentProcess->SetMusic(pObjPlayerInfo->m_cSetting.m_nMusic);
	pCurrentProcess->FUNCTION_VolumeInit();

	CProcessManager::GetPtr()->SetNextProcess(new CReplayMainProcess);
}

IMPLEMENT_USERFUNCTION( RPC_MUTEON )		// 음소거 ON
{
	CAudioManager::GetPtr()->SoundVolume(0);
	CAudioManager::GetPtr()->MusicVolume(0);
	SetVolume(0);

    UIFunction::Set_UIVisibility( "MuteOnButton", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "MuteOffButton", N3UI_VISIBLE_FALSE );
}

IMPLEMENT_USERFUNCTION( RPC_MUTEOFF )		// 음소거 OFF
{
	CReplayProcess* pCurrentProcess = dynamic_cast<CReplayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	CAudioManager::GetPtr()->SoundVolume( ((float)pCurrentProcess->GetSound()/10) );
	CAudioManager::GetPtr()->MusicVolume( pCurrentProcess->GetMusic() );
	SetVolume( pCurrentProcess->GetMusic() );

	UIFunction::Set_UIVisibility( "MuteOnButton", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "MuteOffButton", N3UI_VISIBLE_TRUE );
}

IMPLEMENT_USERFUNCTION( RPC_PLAY )			// REPLAY PLAY
{
	UIFunction::Set_UIVisibility( "PauseButton", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "PlayButton", N3UI_VISIBLE_FALSE );
	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed  = nsGameConfig::s_fOriginSpeed;
}

//IMPLEMENT_USERFUNCTION( RPC_STOP )			// REPLAY STOP
//{
//	CPacketManager::GetPtr()->m_cReplayQue.SetReplayIterator(0.0f);
//}

IMPLEMENT_USERFUNCTION( RPC_PAUSE )			// REPLAY PAUSE
{
	UIFunction::Set_UIVisibility( "PauseButton", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "PlayButton", N3UI_VISIBLE_TRUE );
	nsGameConfig::s_fOriginSpeed = nsGameConfig::s_fStateSpeed;
	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed = 0.f;
}

IMPLEMENT_USERFUNCTION( RPC_PREVSHORT )		// REPLAY PREV <
{
	CPacketManager::GetPtr()->m_cReplayQue.RewindReplayIterator(11.0f);
}

IMPLEMENT_USERFUNCTION( RPC_PREVLONG )		// REPLAY PREV <<
{
	CPacketManager::GetPtr()->m_cReplayQue.RewindReplayIterator(31.0f);
}

IMPLEMENT_USERFUNCTION( RPC_NEXTSHORT )		// REPLAY NEXT >
{
	CPacketManager::GetPtr()->m_cReplayQue.ForwardReplayIterator(9.0f);
}

IMPLEMENT_USERFUNCTION( RPC_NEXTLONG )		// REPLAY NEXT >>
{
	CPacketManager::GetPtr()->m_cReplayQue.ForwardReplayIterator(29.0f);
}

IMPLEMENT_USERFUNCTION( RPC_SPEED_NORMAL )
{
	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed = (float)1.f;
}

IMPLEMENT_USERFUNCTION( RPC_SLOW )			// REPLAY SLOW
{
	nsGameConfig::s_fStateSpeed -= (float)0.1;

	if(nsGameConfig::s_fStateSpeed < 0.2)
		nsGameConfig::s_fStateSpeed = (float)0.2;

	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
}

//IMPLEMENT_USERFUNCTION( RPC_SLOWEST )		// REPLAY SLOWEST
//{
//	nsGameConfig::s_fStateSpeed = (float)0.2;
//
//	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
//}

IMPLEMENT_USERFUNCTION( RPC_FAST )			// REPLAY FAST
{
	nsGameConfig::s_fStateSpeed += (float)0.1;

	if(nsGameConfig::s_fStateSpeed > 2.0)
		nsGameConfig::s_fStateSpeed = 2.0;

	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
}

//IMPLEMENT_USERFUNCTION( RPC_FASTEST )		// REPLAY FASTEST
//{
//	nsGameConfig::s_fStateSpeed = 2.0;
//	nsGameConfig::s_fSlowSpeed = nsGameConfig::s_fStateSpeed;
//}

//////////////////////////////////////////////////////////////////////////
// 카메라 컨트롤 커맨드

IMPLEMENT_USERFUNCTION( CMC_MIN )			// 카메라 리모컨 최소화
{
	UIFunction::Set_UIVisibility( "Dialog_RemoteMovie", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "Dialog_RemoteMin", N3UI_VISIBLE_TRUE );
}

IMPLEMENT_USERFUNCTION( CMC_CAMERA_PREV )	// CAMERA PREV
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int nCameraType = pCurrentProcess->GetCameraType();

	nCameraType--;

	if( nCameraType < D2WAYTILTMOVE )
		pCurrentProcess->SetCameraType( ROTATEENDFIX );
	else
		pCurrentProcess->SetCameraType( nCameraType );

	pCurrentProcess->FUNCTION_CameraChange(pCurrentProcess->GetCameraType());
}

IMPLEMENT_USERFUNCTION( CMC_CAMERA_NEXT )	// CAMERA NEXT
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int nCameraType = pCurrentProcess->GetCameraType();

	nCameraType++;

	if( nCameraType > ROTATEENDFIX )
		pCurrentProcess->SetCameraType( D2WAYTILTMOVE );
	else
		pCurrentProcess->SetCameraType( nCameraType );

	pCurrentProcess->FUNCTION_CameraChange(pCurrentProcess->GetCameraType());
}

IMPLEMENT_USERFUNCTION( CMC_ZOOM_PREV )		// ZOOM PREV
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int nZoom = pCurrentProcess->GetCameraZoom();

	nZoom--;

	if( nZoom < CAMERAZOOM_0 )
		pCurrentProcess->SetCameraZoom( CAMERAZOOM_3 );
	else
		pCurrentProcess->SetCameraZoom( nZoom );

	pCurrentProcess->FUNCTION_ZoomChange( pCurrentProcess->GetCameraZoom() );
}

IMPLEMENT_USERFUNCTION( CMC_ZOOM_NEXT )		// ZOOM NEXT
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	int nZoom = pCurrentProcess->GetCameraZoom();

	nZoom++;

	if( nZoom > CAMERAZOOM_3 )
		pCurrentProcess->SetCameraZoom( CAMERAZOOM_0 );
	else
		pCurrentProcess->SetCameraZoom( nZoom );

	pCurrentProcess->FUNCTION_ZoomChange( pCurrentProcess->GetCameraZoom() );
}

IMPLEMENT_USERFUNCTION( CMC_HOME )			// CAMERA HOME
{	
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->SetCameraTeam( CAMERATEAM_HOME );

	pCurrentProcess->FUNCTION_CameraTeamChange( pCurrentProcess->GetCameraTeam() );

	pCurrentProcess->FUNCTION_ChangeTutorialPlayUI();
}

IMPLEMENT_USERFUNCTION( CMC_SIDE )			// CAMERA SIDE
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->SetCameraTeam( CAMERATEAM_NONE );

	pCurrentProcess->FUNCTION_CameraTeamChange( pCurrentProcess->GetCameraTeam() );

	pCurrentProcess->FUNCTION_ChangeTutorialPlayUI();
}

IMPLEMENT_USERFUNCTION( CMC_AWAY )			// CAMERA AWAY
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	pCurrentProcess->SetCameraTeam( CAMERATEAM_AWAY );

	pCurrentProcess->FUNCTION_CameraTeamChange( pCurrentProcess->GetCameraTeam() );

	pCurrentProcess->FUNCTION_ChangeTutorialPlayUI();
}

IMPLEMENT_USERFUNCTION( CMC_TARGET_PREV )	// TARGET PREV
{
	CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	
	int nTarget = pCurrentProcess->GetCameraTarget();

	nTarget+=20;

	if( CAMERATARGET_REGION < nTarget )
		pCurrentProcess->SetCameraTarget( CAMERATARGET_CHAR );
	else
		pCurrentProcess->SetCameraTarget( nTarget );

	pCurrentProcess->FUNCTION_CameraTargetChange( nTarget );
}

IMPLEMENT_USERFUNCTION( CMC_TARGET_NEXT )	// TARGET NEXT
{
    CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	
	int nTarget = pCurrentProcess->GetCameraTarget();

	nTarget-=20;

	if( nTarget < CAMERATARGET_CHAR )
		pCurrentProcess->SetCameraTarget( CAMERATARGET_REGION );
	else
		pCurrentProcess->SetCameraTarget( nTarget );

	pCurrentProcess->FUNCTION_CameraTargetChange( nTarget );
}

// 퀘스트 관련
IMPLEMENT_USERFUNCTION( QUEST_END )		// 퀘스트 종료
{
	PutGameResult();
}

// 튜토리얼 관련
IMPLEMENT_USERFUNCTION( NEXT_TUTO_EXPLAIN )
{
	CTutorialPlayProcess* pCurrentProcess = dynamic_cast<CTutorialPlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);

	CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "TutorialBg" );
	assert(pBG);
	CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
	assert(pShadowLine);

	if( pShadowLine->IsPlay() )
	{
		pShadowLine->SetInitUpdate(false);
		pShadowLine->SetPlay(true, false, false, 199);

		pCurrentProcess->SetShadow(false);
	}
	else
	{
		if( pCurrentProcess->GetStart() == true && pCurrentProcess->GetAssistant() == false )
			pCurrentProcess->FUNCTION_NextTutorial();

		if(pCurrentProcess->GetStart())
			pCurrentProcess->SetAssistant(false);
		else
			pCurrentProcess->SetStart(true);

		pCurrentProcess->SetShadow(true);
	}
}

IMPLEMENT_USERFUNCTION( REPLAY_ACTION )
{
	CTutorialPlayProcess* pCurrentProcess = dynamic_cast<CTutorialPlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	
	if(pCurrentProcess->GetStart())
		pCurrentProcess->SetAssistant(true);
}

IMPLEMENT_USERFUNCTION( RECHALLENGE_TUTO )
{
	CTutorialPlayProcess* pCurrentProcess = dynamic_cast<CTutorialPlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	CProcessManager::GetPtr()->ChangeTutorialeMachine(pCurrentProcess->GetTutorialType());
}