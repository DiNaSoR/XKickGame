/*********************************************************************************************



*			BaseGameProcess.h

*			기본 게임 프로세스

*			설명 : 기본 게임 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "ObjPannelManager.h"
#include "CoolTimeIcon.h"

class CBasePlayProcess : public CBaseProcess
{
protected:
	enum STATE
	{
		STATE_START = 0x00,	//시작
		STATE_LOADING,		//로딩
		STATE_RESTART,		//다시 시작
		STATE_GAME_SETTING,//게임 세팅
		STATE_PRODUCTION_INIT,//연출 초기화
		STATE_PRODUCTION_PLAY,//연출 플레이
		STATE_GAME_INIT,//게임 초기화
		STATE_GAME_READY,//게임 대기
		STATE_GAME_START,//게임 시작
		STATE_GAME_PLAY,//게임 플레이
		STATE_GOAL_PLAY,//골 플레이
		STATE_REPLAY_READY,//리플레이 준비
		STATE_REPLAY_PLAY,//리플레이 플레이
		STATE_CEREMONY_PLAY,//세레모니 플레이
		STATE_GAME_END_READY,//게임 종료 준비
		STATE_GAME_END,//게임 플레이 종료
		STATE_END//게임 종료
	};

	CObjRoomInfo*				m_pRoom;

	static CObjPlayerInfo*		m_pSelectPlayerInfo;	

	TiXmlElement*				m_pGameOptionXml;

	bool						m_bRobot;

	CResCeremonyTableFormat*	m_pCeremonyTable;

	CInputKey					m_cBackUpInputKey;

	CInputKey					m_cSendInputKey;

	CUIControl*					m_pCameraCurrentBG;

	DWORD						m_nTime;

	DWORD						m_nInputKeySendTime;

	CObjPannelManager*			m_pPannelManager;

	float                       m_fAccumTime;

	static const int			m_snCoolTimeIconCnt = 5;

	CoolTimeInfo				m_cCoolTimeInfoArray[ m_snCoolTimeIconCnt ];

	int							m_nForm;

	int							m_nCameraType;//카메라 타입 저장변수

	int							m_nCameraTarget;//카메라 타겟 저장변수

	int							m_nCameraTeam;//카메라 방향//사이드//프론트

	int							m_nCameraZoom;//카메라 줌 저장변수

	bool						m_bShadow;

	int							m_nRadian;//카메라 각도

	bool						m_bNewbieHelp;

protected:

public:
	CBasePlayProcess();
	virtual ~CBasePlayProcess(void);

	virtual void		InitProcess() {};//초기화 함수

	virtual void		UpdateProcess(float fFrameTime,float fAccumTime);//업데이트 함수

	virtual void		UpdateState(float fFrameTime, float fAccumTime);//상태업데이트 함수

	virtual void		RenderFrame();

	virtual void		Terminate();//종료 함수

	virtual void		ProcessInput();//키 설정
//////////////////////////////////////////////////////////////////////////
//조건 함수삭제 예정
	virtual void		Update(float fFrameTime, float fAccumTime);

	virtual void		InputKey();

	bool				IsPlayStartEnd();
	bool				IsRecordEnd();
	bool				IsPlayGoalEnd();
	bool				IsReplayGoalEnd();
	bool				IsPlayCeremonyEnd();

	virtual void		SetStartAnimation();
	void				SetCeremonyAnimation();
	virtual void		SetEndingAnimation() {}

	int					GetCameraType();
	int					GetCameraTarget();
	int					GetCameraTeam();
	int					GetCameraZoom();
	int					GetCameraRadian();

	void				SetCameraType(int nType);
	void 				SetCameraTarget(int nTarget);
	void				SetCameraTeam(int nTeam);
	void 				SetCameraZoom(int nZoom);
	void				SetCameraRadian(int nRadian);

//////////////////////////////////////////////////////////////////////////
//상태 정의 함수
	virtual void		UpdateStateStart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateLoading(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateRestart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameSetting(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateProductionInit(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateProductionPlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameInit(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameReady(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameStart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGamePlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGoalPlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateReplayReady(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateReplayPlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateCeremonyPlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameEndReady(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameEnd(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateEnd(float fFrameTime, float fAccumTime);
//////////////////////////////////////////////////////////////////////////
//디바이스 관련 함수
	//virtual bool		OnLostDevice() { CObjectModelManager::GetPtr()->OnLostDevice(); return true; }
	//virtual bool		OnResetDevice() { CObjectModelManager::GetPtr()->OnResetDevice(); return true; }


	virtual bool		OnDefault(NiEventRef pEventRecord);	


//////////////////////////////////////////////////////////////////////////
//UI 관련 함수
	void				FUNCTION_InitTutorialPlayUI();
	void				FUNCTION_UpdateTutorialPlayUI();//

	void				FUNCTION_ChangeTutorialPlayUI();//카메라 모드 관련 UI

	void				FUNCTION_UpdateTutorialPlayScoreUI();//점수 관련 UI

	void				FUNCTION_UpdateTutorialPlayTimeUI();

	void				FUNCTION_QuickChattingPrint(int i);

	virtual void		FUNCTION_UIClear();
	virtual void		FUNCTION_UIInit();

	void				FUNCTION_SkillEffect(int nPlayer, CObjPlayerInfo* pAnyPlayer);
	void				FUNCTION_SkillEffectUpdate(int nPlayer, CObjPlayerInfo* pAnyPlayer);

	void				FUNCTION_CameraChange(int nType);
	void				FUNCTION_ZoomChange(int nType);
	void				FUNCTION_CameraTeamChange(int nType);
	void				FUNCTION_CameraTargetChange(int nType);

	void				InitPositionMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer);
	void				InitMiniMapMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer);
	void				InitMiniMapMarkPoint(CObjBallInfo* pBallInfo);
	void				UpdatePositionMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer);
	void				UpdateMiniMapMarkPoint(int nPlayer, CObjPlayerInfo* pAnyPlayer);
	void				UpdateMiniMapMarkPoint(CObjBallInfo* pBallInfo);

	void				UpdateFormNumber(bool bView, char nTeam, int nObjSeq, char nHave);

	void				SetMiniMapMarkPoint(CUIControl* pControl, NiPoint3 cPoint);

	void 				FUNCTION_UpdatePlayerHasGoalNameUI(CObjPlayerInfo* pAnyPlayer);
	void 				Set_GoalType(const DWORD& nShootingPlayerTeam, const DWORD& nGoalType);
	void 				Set_GoalInTime(const float& fPlayTime);
	void 				DisplayRemainTime(const DWORD& nRemainTime);

	void				AngerAnimation();
	void 				ShowGoalInfo();
	void 				HideGoalInfo();

	void                CoolTimeIconInit();
	void                CoolTimeIconUpdate();
	void                DrawSkillCoolTimeInfo();

	void                SetShadowCamera();

	void				IsAngerMode();

	void				UpdateAngerMode(float fFrameTime, float fAccumTime);

	void				SetSoundSetting(bool bAnger = false);

	void				SetObjectMaterialColorSetting(bool bAnger);

	void				SetSelectPlayer(CObjPlayerInfo* pSelectPlayerInfo);

	static CObjPlayerInfo* GetSelectPlayer() { return m_pSelectPlayerInfo; }
};