/*********************************************************************************************
*			ReplayMainProcess.h

*			서버 선택 프로세스
*			설명 : 서버를 선택할수 있는 프로세스 이다

*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정
*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에
*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.

*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008
**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include <vector>

using namespace std;

class CReplayMainProcess : public CBaseProcess
{
public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_GROUND_LOAD,
		STATE_PLAY,
		STATE_END
	};

	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포

	typedef vector<std::string>		REPLAY_FILELIST;

	REPLAY_FILELIST					m_vFileList;

	char							m_nPage;
	char							m_nTotalPage;
	char							m_nKind;
	char							m_nSelectReplay;
	POINT							m_pDNDPoint;

public:
	CReplayMainProcess();
	virtual ~CReplayMainProcess(void);

	void			FUNCTION_UIInit();
	void			FUNCTION_UpdateScoreUI(char nHomeScore, char nAwayScore);
	void			FUNCTION_UpdatePage(char nPage);

    virtual void	Terminate();

	virtual void	UpdateState(float fFrameTime, float fAccumTime);
	virtual void	RenderFrame();

	void			LoadGround();

	virtual bool	OnDefault(NiEventRef pEventRecord);
	void			MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	CBaseProcess*	GetSameProcess();

	void UpdateProcess(float fFrameTime,float fAccumTime);
	virtual void InitProcess();

	void			ReplayInfomationUpdate(char idx);
	void			FileListUpdate(char nKind);

	void			ReplayListUpdate();
	void			ReplayListSelection();
	void			ReplayListDelete();
	void			ReplayListChoice(char idx);
};
