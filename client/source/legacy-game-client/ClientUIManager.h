//***********************************************************************************
//	File		:	CClientUIManager.h
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef _CLIENT_DIALOG_MANAGER_H_
#define _CLIENT_DIALOG_MANAGER_H_

#include "UIManager.h"
#include "Singleton.h"

class CClientUIManager : public CUIManager, public CSingleton<CClientUIManager>
{
public:

public:
			CClientUIManager();
    virtual	~CClientUIManager();

	virtual HRESULT DrawTexture( CUIControl* pControl, IDirect3DTexture9*	pTexture, RECT& rcScreen, D3DCOLOR color );
	virtual HRESULT DrawAlphaTexture(CUIControl* pControl, CUIControl* pAlphaControl, RECT& rcScreen, D3DCOLOR color );
};

namespace UIMessage
{
	void				SendMessage_SignIn();
};


#endif

