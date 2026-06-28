/*********************************************************************************************



*			GameLoadingProcess.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"

class CBaseLoadingProcess : public CBaseProcess
{
public:

	enum THREADSTATE
	{
		STATE_THREADSTART = 0x00,
		STATE_THREADINIT_LOAD,
		STATE_THREADGROUND_LOAD,
		STATE_THREADCHARACTER_LOAD,
		STATE_THREADEND
	};

	ULONG							m_hLoadingThread;//스레드 핸들

	bool							m_bLoadingThreadDown;//스레드 정지변수

	int								m_nThreadState;

	CObjPlayerInfo *				m_pSelectPlayerInfo;

	DWORD							m_nTime;

	DWORD							m_nInputKeySendTime;

	CObjPlayerInfo*					m_pLeaveRoomPlayerPoint;

public:
	CBaseLoadingProcess();
	virtual ~CBaseLoadingProcess(void);

	virtual void					Terminate();

			bool					UpdateThread();
	virtual void					RenderFrame();

	virtual void					ProcessInput();

	static void						LoadingThread(LPVOID pV);

	virtual bool					OnDefault(NiEventRef pEventRecord);
	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();

	void							SetLeaveRoomPlayerPoint(CObjPlayerInfo *pLeaver) { m_pLeaveRoomPlayerPoint = pLeaver; }

	virtual void					GotoGamePlay() = 0;
public:

	virtual void					SendMyProgress(DWORD MyProgress);
public:
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	void							TerminateThread();

	virtual void					InitProcess() = 0;

public:

	void							LoadGround();
	bool							LoadCharacter();

	NiNodePtr						LoadCollision(NiNode *pRootNode);

	//UI 관련 함수
	virtual void					FUNCTION_GameLoadingViewChange();
	void							UpdateProgress();

public:
	void							InitUserProgress();
	void							UpdateUserProgress();
	void							ClearUserProgress();
};
