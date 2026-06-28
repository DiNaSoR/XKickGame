/*********************************************************************************************



*			TutorialPlayProcess.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BasePlayProcess.h"

class CTutorialPlayProcess : public CBasePlayProcess
{
protected:
//////////////////////////////////////////////////////////////////////////
//공통적인 튜토리얼 내용
	int								m_nTutorialStep1MaxCount;//튜토리얼 완료 갯수

	int								m_nTryTotalCount;//총 시도 횟수

	int								m_nTrySuccessCount;//시도 성공 횟수

	bool							m_bTryProcess;//튜토리얼 시도인지?

	CUIControl*						m_ppKeyUIControl[ARRAY_KEY_SIZE];//키를 나타내는 UI변수

	bool							m_bTutorialStep1End;//튜토리얼 끝났나?

	bool							m_bAssistant;//조교 시범

	bool							m_bStart;

	CObjPlayerInfo*					m_pAssistantPlayer;//조교

	int								m_nTutorialType;//튜토리얼 타입

	DWORD							m_nTutorialTime;

	bool							m_bTutorialTime;

	bool							m_bShadow;

public:
	CTutorialPlayProcess();
	virtual ~CTutorialPlayProcess(void);

	virtual void					InitProcess();

	virtual void					UpdateProcess(float fFrameTime,float fAccumTime);

	virtual void					ProcessInput();

	//////////////////////////////////////////////////////////////////////////
	//조건 함수삭제 예정
	virtual void					Update(float fFrameTime, float fAccumTime);

	virtual void					Terminate();//종료 함수

	void							LoadGround();

	void							LoadCharacter();

	NiNodePtr						LoadCollision(NiNode *pRootNode);

public:
	//////////////////////////////////////////////////////////////////////////
	//상태 정의 함수
	virtual void					UpdateStateStart(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateLoading(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateRestart(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameSetting(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameInit(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameReady(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameStart(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGamePlay(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGoalPlay(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateReplayReady(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateReplayPlay(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateCeremonyPlay(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameEndReady(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateGameEnd(float fFrameTime, float fAccumTime);
	virtual void					UpdateStateEnd(float fFrameTime, float fAccumTime);

	virtual void					DemonstrationGameInit();
	virtual void					DemonstrationPlay(float fFrameTime, float fAccumTime);//시범 플레이

	virtual void					FUNCTION_ExplainUIInit();
	virtual void					FUNCTION_KeyUIInit();
	virtual void					FUNCTION_DemonstrationGameUIInit();
	virtual void					FUNCTION_GamePlayExplainUIInit();
	virtual void					FUNCTION_GamePlayUIInit();
	virtual void					FUNCTION_ResultUIInit(bool bSuccess);
	
	virtual void					GamePlay(float fFrameTime, float fAccumTime);//게임 플레이
public:
	//////////////////////////////////////////////////////////////////////////
	//공통적인 튜토리얼 내용
	void							FUNCTION_InitKeyUIControl();

	void							FUNCTION_UpdateScoreUI(bool bSuccess);

	void							FUNCTION_UpdateKeyUI();

	virtual bool					TryProcess() { return true; }

	virtual bool					SuccessProcess() { return true; }

	virtual void					FailProcess() { }

	virtual void					SetEndingAnimation();

	void							SetStart(bool bStart)			{ m_bStart = bStart; }
	void							SetAssistant(bool bAssistant)	{ m_bAssistant = bAssistant; m_nState = STATE_GAME_INIT; }
	void							SetShadow(bool bShadow)			{ m_bShadow = bShadow; }

	bool							GetStart()	{ return m_bStart; }
	bool							GetAssistant()	{ return m_bAssistant; }

public:

	int								GetTutorialType() { return m_nTutorialType; }

	int								GetTrySuccessCount() { return m_nTrySuccessCount; } 

	void							FUNCTION_NextTutorial();
};
