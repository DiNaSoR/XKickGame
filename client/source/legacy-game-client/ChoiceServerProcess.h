/*********************************************************************************************



*			ChoiceServerProcess.h

*			서버 선택 프로세스

*			설명 : 서버를 선택할수 있는 프로세스 이다


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "UtlWebPage.h"

class CChoiceServerProcess : public CBaseProcess
{
public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_PLAY,
		STATE_END
	};

	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포
	POINT							m_pDNDPoint;

	char							m_nServerTabIndex;
	char							m_nSelectServer;
	char							m_nAutoSelectServer;
	bool							m_bUp;

public:
	CChoiceServerProcess();
	CChoiceServerProcess(bool bUp);
	virtual ~CChoiceServerProcess(void);

	virtual void					InitProcess();
	CBaseProcess*					GetSameProcess();

	virtual void					RenderFrame();
	virtual bool					OnDefault(NiEventRef pEventRecord);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	virtual void					Terminate();

	void							LoadGround();

	//UI관련 함수
	void							FUNCTION_Init();
	void							FUNCTION_ChoiceServerListUpdateUI();
	void							FUNCTION_MouseOverCheck();
	void							FUNCTION_Selection();
	void							FUNCTION_TabChange(char idx);

	void							IsChoiceClass();
};
