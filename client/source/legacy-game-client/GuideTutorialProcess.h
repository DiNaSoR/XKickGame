/*********************************************************************************************



*			GuideTutorialProcess.h

*			서버 선택 프로세스

*			설명 : 서버를 선택할수 있는 프로세스 이다


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "TutorialPlayProcess.h"

enum GUIDESTEP
{
	GT_INTRODUCE = 0,
	GT_INTRODUCE2,
	GT_CLASSTREE,
	GT_CLASSTREE2,
	GT_AREA,
	GT_ATTACKAREA,
    GT_SHORTSHOOT,
	GT_LONGSHOOT,
	GT_CROSSAREA,
	GT_DEFENCEAREA,
	GT_KEYCONTROL,
	GT_KEYCONTROL2,
	GT_KEYCONFIG,
	GT_FINAL,
	GT_ABILITY,
	GT_COUNT,
};

class CGuideTutorialProcess : public CBaseProcess
{
private:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_PLAY,
		STATE_END
	};

	char							m_nGuideTutorialStep;
	bool							m_bGT;
	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포

public:

	CGuideTutorialProcess();
	CGuideTutorialProcess(char nStep);
	virtual ~CGuideTutorialProcess(void);

	virtual void					Terminate();

	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	virtual void					RenderFrame();

	virtual bool					OnDefault(NiEventRef pEventRecord);

	CBaseProcess*					GetSameProcess();

	void							UpdateProcess(float fFrameTime,float fAccumTime);
	virtual void					InitProcess();

	void							LoadGround();

	void							FUNCTION_Init();
	void							FUNCTION_GuideStart(char nBG);

	void							GuideStepPlus();
	void							GuideStepMinus();
	char							GetGuideStep();
	void							SetGuideStep(char nStep);
};
