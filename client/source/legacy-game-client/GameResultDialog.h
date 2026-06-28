//***********************************************************************************
//	File		:	GameResultDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#pragma once
#include "Singleton.h"

#include "Control.h"

class CGameResultDialog : public CSingleton<CGameResultDialog>
{
private:
	static CObjRoomInfo*	pRoom;
public:
	CGameResultDialog(void);
	~CGameResultDialog(void);
	static void						UpDateDialog(CObjRoomInfo*	_pRoom);
	void							FUNCTION_HeadResult();
	void							FUNCTION_EachCharacterResult(int i, CUIControl* cPerson);
	void							Init();
	void							Terminate();
};
