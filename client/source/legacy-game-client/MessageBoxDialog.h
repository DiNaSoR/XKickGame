//***********************************************************************************
//	File		:	MessageBoxDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef _MESSAGEBOX_DIALOG_H_
#define _MESSAGEBOX_DIALOG_H_

#include "Singleton.h"

class CMessageBoxDialog : public CSingleton<CMessageBoxDialog>
{
public:
	enum MESSAGEBOX
	{
		MESSAGEBOX_NONE				= 0x00,
		MESSAGEBOX_EXIT,
		MESSAGEBOX_LANGEXIT,
		MESSAGEBOX_GAMEEXIT,
		MESSAGEBOX_CHOICECLASS,
		MESSAGEBOX_JUMIN,
		MESSAGEBOX_DUPLICATION,
		MESSAGEBOX_REFRESHITEM,
		MESSAGEBOX_NEWLOGIN			= 100,
	};

	enum MSGBOXTYPE
	{
		MSGBOXTYPE_YESNO			= 0x00,
		MSGBOXTYPE_CONFIRMCANCEL,
		MSGBOXTYPE_CONFIRM,
		MSGBOXTYPE_IMECONFIRMCANCEL,
	};

public:

	int			m_nState;

	unsigned int	m_MessageIndex[100];

public:
	CMessageBoxDialog(void);
	~CMessageBoxDialog(void);

	void	Enable(int nState, int nMsgID, int nMsgType, int nErrorCode = 0);
	void	OK();
	void	Cancel();

	void	SetOkMessage(const char* strMessage);
	void	SetOkCanelMessage(const char* strMessage);
};

#endif