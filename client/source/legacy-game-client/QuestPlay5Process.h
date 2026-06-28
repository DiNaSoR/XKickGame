/*********************************************************************************************



*			QuestPlay5Process.h

*			경기 프로세스

*			설명 : 게임 경기 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BasePlayProcess.h"
#include "QuestPlayProcess.h"

class CQuestPlay5Process : public CQuestPlayProcess
{
public:
	CQuestPlay5Process();
	virtual ~CQuestPlay5Process(void);

	virtual bool					TryProcess();

	virtual bool					SuccessProcess();

	virtual void					FailProcess();
};
