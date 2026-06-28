/*********************************************************************************************



*			TutorialStep2PlayProcess.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BasePlayProcess.h"

class CGamePlayProcess : public CBasePlayProcess
{
public:
	CGamePlayProcess();
	virtual ~CGamePlayProcess(void);

    virtual void        ProcessInput();
	virtual void		InitProcess();//초기화 함수

	virtual void		UpdateStateStart(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameSetting(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameInit(float fFrameTime, float fAccumTime);
	virtual void		UpdateStateGameReady(float fFrameTime, float fAccumTime);

protected:
    int m_nSnowRainEffectSeqNo;
};
