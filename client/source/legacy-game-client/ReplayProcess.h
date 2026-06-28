/*********************************************************************************************



*			ReplayProcess.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BasePlayProcess.h"

class CReplayProcess : public CBasePlayProcess
{
protected:
	char				m_nSound;
	char				m_nMusic;

	bool				m_bEndReady;

public:
	CReplayProcess();
	virtual ~CReplayProcess(void);

	virtual void		InitProcess();//초기화 함수

	virtual void		UpdateProcess(float fFrameTime,float fAccumTime);//업데이트 함수
	virtual bool		OnDefault(NiEventRef pEventRecord);
	void				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void		Terminate();//종료 함수

	virtual void		ProcessInput();

	virtual void		UpdateStateStart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameInit(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameReady(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameStart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameSetting(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateReplayPlay(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameEndReady(float fFrameTime, float fAccumTime);

	void				FUNCTION_VolumeInit();
	void				FUNCTION_VolumeUpdate();

	char				GetSound();
	char				GetMusic();
	void				SetSound(char nSound);
	void				SetMusic(char nMusic);
};