//***********************************************************************************
//	File		:	CChattingDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#ifndef _CHATTING_DIALOG_H_
#define _CHATTING_DIALOG_H_

#include "Singleton.h"

using namespace std;
#include <string>
#include <deque>
#include <vector>

typedef struct
{
	std::string				sMessage;
	D3DXCOLOR				sColor;
}CHAT_MESSAGE;

class CChattingDialog : public CSingleton<CChattingDialog>
{
public:
	
	typedef std::deque<CHAT_MESSAGE>	CHAT_STRUCT_LIST;
	CHAT_STRUCT_LIST				m_MessageList;

	typedef vector<string>			WhisperId;
	WhisperId						m_sWhisperId;
	string							m_sId;
	int								m_nWhisperIdIndex;
	DWORD							m_nPassTime;
	char							m_nSendCount;
	DWORD							m_nFadeOutTime;

	char							m_cChatting[MAX_PATH];

public:
	CChattingDialog(void);
	virtual ~CChattingDialog(void);

	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void SetChatMessage(const char* pMessage, bool bWrite = true, D3DCOLOR color = 0xff00ff00);
	void SetChatMessageRefresh(const char* pMessage, bool bWrite = true, D3DCOLOR color = 0xff00ff00);

	void FUNCTION_ChattingEnter(const char* szwindow);

	void FUNCTION_ChattingEnterInGamePlay(const char* szwindow);
	void FUNCTION_ChattingFunctionKeyInGamePlay(const char* szwindow);

	void FUNCTION_ChattingListSort(char idx);

	void FUNCTION_ChattingWhisperListPopup();
	
	void KeyEventSecretListRefresh();

	void ChattingListClear();

	void ChattingListDelete();

	void WhisperIdSave(string sId);

	void EnableChatting();

	void ChattingStringFitter(std::string* str, const char* xChar);
	void ChattingStringCommonFitter(std::string* str);

    void TimeDeleyToDeleteChattingList();

	int		URLEncode2(const char* sIn, BYTE** ppOut, int& ALen);
	char*	URLdecode( const char* in );
};

#endif

