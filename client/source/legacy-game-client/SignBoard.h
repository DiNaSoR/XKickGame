//***********************************************************************************
//	File		:	GameOptionDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#pragma once

#include "Singleton.h"
#include "ComStructure.h"
#include <vector>
#include "PacketForm.h"

using namespace std;

class SignBoard : public CSingleton<SignBoard>
{
private:
	typedef vector<CScheduleData>	SIGNBOARD_VECTORLIST;
	SIGNBOARD_VECTORLIST			m_bSignBoard;
	bool							m_bXKickOnline;
	char							m_nSignBC;
	CSCScheduleList					m_cSchedules;
	DWORD							m_nTickCount;

protected:

public:
	SignBoard(void);
	~SignBoard(void);

	void DisplaySignBoardTime();
	void DisplaySignBoard();
	void InitSignBoard(CSCScheduleList Schedules);
	char DayToCode(char nDay);
};