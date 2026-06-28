/*********************************************************************************************



*			GameLoginProcess.h

*			게임에 접속하기위한 로그인 프로세스

*			설명 : 서버와 연결하기위한 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "PlayMovie.h"
#include "UtlWebPage.h"

class CGameLoginProcess : public CBaseProcess
{
	//CWebPage						m_cWebPage;

public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_GROUND_LOAD,
		STATE_INIT_LOAD,
		STATE_END
	};

public:
	CGameLoginProcess();
	virtual ~CGameLoginProcess(void);

    virtual void					Terminate();

	virtual void	UpdateState(float fFrameTime, float fAccumTime);

    virtual void RenderFrame();

	void UpdateProcess(float fFrameTime,float fAccumTime);

	void	LoadGround();

	virtual void InitProcess();
};