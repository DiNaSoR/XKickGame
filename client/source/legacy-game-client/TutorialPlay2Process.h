/*********************************************************************************************



*			TutorialPlay2Process.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BasePlayProcess.h"
#include "TutorialPlayProcess.h"

class CTutorialPlay2Process : public CTutorialPlayProcess
{
	bool							m_bChangeShootingZoon;//슈팅존 변경여부

	NiAVObject*						m_pkShootingZoon;//활성화 된 슈팅존모델

	int								m_nTutorialPartKey2;//튜토리얼 파트2에 대한 키값 저장변수

public:
	CTutorialPlay2Process();
	virtual ~CTutorialPlay2Process(void);

	virtual bool					TryProcess();

	virtual bool					SuccessProcess();

	virtual void					FailProcess();

	virtual void					InputKey();

	virtual void					UpdateStateGameInit(float fFrameTime, float fAccumTime);

private:
	void							ChangeShootingZoon();//슈팅존 변경
	bool							CheckShootingZoon();//슈팅존 채크
	void							FindShootingZoon();//슈팅존 찾는 함수

	virtual void					DemonstrationGameInit();
	virtual void					DemonstrationPlay(float fFrameTime, float fAccumTime);

	virtual void					FUNCTION_ExplainUIInit();
	virtual void					FUNCTION_KeyUIInit();
	virtual void					FUNCTION_GamePlayExplainUIInit();
	virtual void					FUNCTION_DemonstrationGameUIInit();
	virtual void					FUNCTION_GamePlayUIInit();
	virtual void					FUNCTION_ResultUIInit(bool bSuccess);
};
