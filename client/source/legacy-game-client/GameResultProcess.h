/*********************************************************************************************



*			GameResultProcess.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "3DRenderedTexBoard.h"

class CMatchRecord
{
private:
	CUIControl *cMarks;
	CUIControl *cAssistance;
	CUIControl *cCutting;
	CUIControl *cEffactiveShooting;
	CUIControl *cEffactiveStill;
	CUIControl *cEffactiveTackle;
public:
	virtual void SetVariablePtr(CUIControl** _pVariableAddress);
};

class CTeamMatchRecord:public CMatchRecord
{
private:
	CUIControl *cBallShare;
public:
	void SetVariablePtr(CUIControl** pVariableAddress);
};

class CPersonalMatchRecord:public CMatchRecord
{
private:
	CUIControl *cBoldBack;
	CUIControl *cLevel;
	CUIControl *cPosition;
	CUIControl *cName;
	CUIControl *cExamination;
	CUIControl *cGainedExp;
	CUIControl *cGainedMileage;
	CUIControl *cGainedPromotion;

public:
	void SetVariablePtr(CUIControl** _pVariableAddress);
};

class CGameResultProcess : public CBaseProcess
{
	CTeamMatchRecord				cHTeamMatchRecord;
	CTeamMatchRecord				cATeamMatchRecord;

	int								m_nPosIndex;

	DWORD							nDisplayTime;

	int								m_nAnimationID;

public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_RESULT_PLAY,
		STATE_END
	};

	float							m_fLifeTime;
	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포
	//bool							m_bOccupation;

	CCardInfo						m_CInfo;
	char							m_nRand;
	typedef vector<char>			VectorPositionList;
	typedef vector<char>			VectorLevelList;
	typedef vector<char>			VectorRankList;

	VectorPositionList				m_vPosition;
	VectorLevelList					m_vLevel;
	VectorRankList					m_vRank;

private:
	CUIControl**					GetControlChildrenPtr(CUIControl* _cParent);

public:

	virtual void					Init();
	virtual void					Update(float fFrameTime, float fAccumTime);

	void							LoadGround();

	virtual void					Terminate();

	void							UpdateCamera();

	void							LoadCharacter();

	void							FUNCTION_WinLose();
	void							FUNCTION_AdditionCompensation(int i);
	void							FUNCTION_QuestIsComplete();
	void							FUNCTION_QuestResult();
	void							FUNCTION_InitCardResult();
	void							FUNCTION_CardUpdate();
	void							FUNCTION_CardResult();

public:
	CGameResultProcess();
	virtual ~CGameResultProcess(void);

	virtual void					UpdateState(float fFrameTime, float fAccumTime);

	virtual void					ProcessInput();

	void							UpdateProcess(float fFrameTime,float fAccumTime);

	virtual void					RenderFrame();

	virtual void					InitProcess();
	bool							OnLostDevice();
	bool							OnResetDevice();
};