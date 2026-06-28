//***********************************************************************************
//	File		:	CChattingDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************


#include "StdAfx.h"
#include "UIControl.h"
#include "UIListbox.h"
#include "PutPacket.h"
#include "PacketForm.h"
#include "GetPacket.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

#include ".\chattingdialog.h"
#define	MULTILINE	24

CChattingDialog::CChattingDialog(void)
{
	m_nWhisperIdIndex = -1;
	m_sWhisperId.clear();
	m_MessageList.clear();
	m_nPassTime = 0;
	m_nSendCount = 0;
}

CChattingDialog::~CChattingDialog(void)
{
}

bool CChattingDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("ChattingCommand");
	if(pControl == NULL) return false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_UP:
			if( CUIControl::Get_FocusedControl() == pControl )
			{
				if( (m_sWhisperId.size()-1) !=m_nWhisperIdIndex)
					m_nWhisperIdIndex++;
				KeyEventSecretListRefresh();
			}
			break;;
		case VK_DOWN:
			if( CUIControl::Get_FocusedControl() == pControl )
			{
				if(m_nWhisperIdIndex != -1)
					m_nWhisperIdIndex--;
				KeyEventSecretListRefresh();
			}
			break;
		case VK_RETURN:
			if( pControl != NULL )
			{
				if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMEPLAY) )
				{
					if(pControl != CUIControl::Get_FocusedControl())
					{
						pControl->SetVisible(N3UI_VISIBLE_TRUE);
						CUIControl::RequestFocus(pControl);
						CUIControl::Set_FocusedControl(pControl);						
					}
					else
					{
						CChattingDialog::GetPtr()->FUNCTION_ChattingEnterInGamePlay("ChattingCommand");
						CChattingDialog::GetPtr()->m_nWhisperIdIndex = -1;
						pControl->ClearText();
						pControl->ClearFocus();
						CUIControl::Set_FocusedControl(NULL);
					}
				}
				else
				{
					CUIControl::RequestFocus(pControl);
					CUIControl::Set_FocusedControl(pControl);
				}
			}
			break;
		}
		break;
	}
	if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMEPLAY) && pControl == CUIControl::Get_FocusedControl() )
		return true;
	else
		return false;
}

void CChattingDialog::SetChatMessage(const char* pMessage, bool bWrite, D3DCOLOR color)
{
	char szbuff[MAX_PATH];
	WCHAR wszbuff[MAX_PATH];
	sprintf(szbuff, "ChattingList");
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szbuff);
	if(pControl == NULL) return;

	std::string temp;
	char buff[MAX_PATH]="";
	temp = pMessage;
		
	ChattingStringFitter(&temp, "\\");
	ChattingStringFitter(&temp, "%");
	ChattingStringCommonFitter(&temp);

	_snprintf(buff, MAX_PATH, temp.c_str());
	
	if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMEPLAY) && color == COLOR_BLACK )
		return;

	if( pControl->GetTextColor() == COLOR_WHITE )
		color = COLOR_WHITE;

	if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMEPLAY) && color == COLOR_NOTICE )
		color = COLOR_WHITE;

	if( pControl->GetItemCount() > 0 )
	{
		if(!wcscmp(pControl->GetItemText(0), L""))
			pControl->RemoveItem(0);
	}

	SN3ListBoxItem *pNewItem = NULL;

	if(MULTILINE*2 < (int)strlen(buff))			//채팅창 줄나누기
	{
		int nTextSize = strlen(buff);
		int nBuffPos = 0;
		int nEmpty = 0;
		bool bEnd = false;

		ZeroMemory(szbuff, sizeof(szbuff));

		for(int i = 0; i < nTextSize; i++)
		{
			szbuff[nBuffPos] = buff[i];

			if(nBuffPos >= MULTILINE*2)
			{
				ZeroMemory(wszbuff, sizeof(wszbuff));

				StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(szbuff));

				pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
				if (bWrite == true && pNewItem != NULL)
				{
					pNewItem->NormalColor = color;
					pNewItem->PressColor = color;
				}
				WideCharToMultiByte(CP_ACP, 0, wszbuff, MAX_PATH, szbuff, MAX_PATH, NULL, NULL);
				nEmpty += strlen(szbuff);
				i = nEmpty-1;
				if(MULTILINE*2 >= nTextSize-i && nTextSize-i-1 != 0)
					bEnd = true;
				ZeroMemory(szbuff, sizeof(szbuff));
				nBuffPos = -1;
			}
			nBuffPos++;
		}
		if(bEnd)
		{
			ZeroMemory(wszbuff, sizeof(wszbuff));

			MultiByteToWideChar(CP_ACP, 0, szbuff, MAX_PATH, wszbuff, MAX_PATH);

			StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(szbuff));

			pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
		}
	}
	else
	{
		StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(buff));

		pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
	}
	pControl->SetScrollTrackPos(pControl->GetItemCount()-1);

	if (bWrite == true && pNewItem != NULL)
	{
		pNewItem->NormalColor = color;
		pNewItem->PressColor = color;
	}

	m_nFadeOutTime = timeGetTime();
	
	CHAT_MESSAGE Msg;

	Msg.sMessage	= buff;
	Msg.sColor		= color;

	m_MessageList.push_back(Msg);
}

void CChattingDialog::SetChatMessageRefresh(const char* pMessage, bool bWrite, D3DCOLOR color)
{
	char szbuff[MAX_PATH];
	WCHAR wszbuff[MAX_PATH];
	sprintf(szbuff, "ChattingList");
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szbuff);
	if(pControl == NULL) return;
	
	SN3ListBoxItem *pNewItem = NULL;

	if(MULTILINE*2 < (int)strlen(pMessage))			//채팅창 줄나누기
	{
		int nTextSize = strlen(pMessage);
		char szbuff[MAX_PATH];
		int nBuffPos = 0;
		int nEmpty = 0;
		bool bEnd = false;

		ZeroMemory(szbuff, sizeof(szbuff));

		for(int i = 0; i < nTextSize; i++)
		{
			szbuff[nBuffPos] = pMessage[i];
			if(nBuffPos >= MULTILINE*2)
			{
				ZeroMemory(wszbuff, sizeof(wszbuff));

				StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(szbuff));

				pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
				if (bWrite == true && pNewItem != NULL)
				{
					pNewItem->NormalColor = color;
					pNewItem->PressColor = color;
				}
				WideCharToMultiByte(CP_ACP, 0, wszbuff, MAX_PATH, szbuff, MAX_PATH, NULL, NULL);
				nEmpty += strlen(szbuff);
				i = nEmpty-1;
				if(MULTILINE*2 >= nTextSize-i && nTextSize-i-1 != 0)
					bEnd = true;
				ZeroMemory(szbuff, sizeof(szbuff));
				nBuffPos = -1;
			}
			nBuffPos++;
		}
		if(bEnd)
		{
			ZeroMemory(wszbuff, sizeof(wszbuff));
			StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(szbuff));
			pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
		}
	}
	else
	{
		StringCchCopyW( wszbuff, MAX_PATH, MultiByteToWideChar(pMessage));
		pControl->AddItem(wszbuff, NULL, (void**)&pNewItem);
	}
	pControl->SetScrollTrackPos(pControl->GetItemCount()-1);

	if (bWrite == true && pNewItem != NULL)
	{
		pNewItem->NormalColor = color;
		pNewItem->PressColor = color;
	}
}

void CChattingDialog::FUNCTION_ChattingEnter(const char* szwindow)
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szwindow);
	if (pControl == NULL) return;

	if (pControl->GetTextLength() <= 0)
		return;
	else
	{
		std::string temp;
		char buff[MAX_PATH];
		temp = pControl->GetTextEx();
		ChattingStringFitter(&temp, "\\");
		ChattingStringFitter(&temp, "%");
		ChattingStringCommonFitter(&temp);

		sprintf(buff, temp.c_str());
		
		CCSSendMessage Msg;

		char* p = buff;

		m_sId.clear();
        
		if(buff[0] == '/') {			// 귓말일 경우
			p++;
			if(p[0] == '/') {			// 클럽말일 경우
				Msg.m_nChatKind = CHAT_KIND_CLUB;
				p++;
			}
			else
				Msg.m_nChatKind = CHAT_KIND_SECRET;

			while(1){
				if( Msg.m_nChatKind == CHAT_KIND_CLUB )
					break;
				m_sId.push_back(p[0]);
				p++;
				if(p[0] == ' ') {		// 아이디 뒤에 말이 있을경우
					m_sId.push_back('\0');
					p++;
					_snprintf(Msg.m_sToName, PLAYER_NAME_SIZE, "%s", m_sId.c_str() );
					break;
				}
				else if(p[0] == 0) {	// 귓말 아이디만 칠 경우
					m_sId.push_back('\0');
					_snprintf(Msg.m_sToName, PLAYER_NAME_SIZE, "%s", m_sId.c_str() );
					p[0] = '\0';

					break;
				}
			}
		}
		else {							// 보통 말일 경우
			p = buff;
			Msg.m_nChatKind = CHAT_KIND_NORMAL;
			_snprintf(Msg.m_sToName, PLAYER_NAME_SIZE, "%s", "" );
		}

		_snprintf(Msg.m_sMessage, MESSAGE_SIZE, p);

		if( (3 < m_nSendCount) && ((timeGetTime() - m_nPassTime) < 10000) )
		{
			char sText[STRLEN_64]="";

			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900073 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, 10-((timeGetTime()-m_nPassTime) /1000));
			SetChatMessage(sText, true, COLOR_ANNOUNCE);
			pControl->ClearText();
			return;
		}
        
		if( (timeGetTime() - m_nPassTime) < 2000 )
			m_nSendCount++;
		else
			m_nSendCount=0;

		m_nPassTime = timeGetTime();
		
		PutSendMessage(&Msg);

		pControl->ClearText();
	}
}

void CChattingDialog::WhisperIdSave(string sId)
{
	vector<string>::iterator iter;
	
	for(iter = m_sWhisperId.begin();iter != m_sWhisperId.end();iter++) {
		m_sWhisperId.size();
		if( !sId.compare(*iter ) ) {
			m_sWhisperId.erase(iter);
			break;
		}
	}
	
	iter = m_sWhisperId.begin();

	if(m_sWhisperId.size()==10) {
		m_sWhisperId.erase(iter);
	}
	m_sWhisperId.push_back(sId);
}

void CChattingDialog::FUNCTION_ChattingEnterInGamePlay(const char* szwindow)
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szwindow);
	if (pControl == NULL) return;

	if (pControl->GetTextLength() <= 0)
		return;
	else
	{
		CUIControl* pChat = CClientUIManager::GetPtr()->FindEventControl( "ChattingList" );
		assert(pChat);
		if( 6<pChat->GetItemCount() )
			pChat->RemoveItem(0);

		std::string temp;
		char buff[MAX_PATH];
		temp = pControl->GetTextEx();
		ChattingStringFitter(&temp, "\\");
		ChattingStringFitter(&temp, "%");
		ChattingStringCommonFitter(&temp);

		sprintf(buff, temp.c_str());
		
		CCSSendMessage Msg;

		char* p = buff;

		m_sId.clear();
        
		if(buff[0] == '/') {			// 귓말일 경우
			p++;
			if(p[0] == '/') {			// 클럽말일 경우
				Msg.m_nChatKind = CHAT_KIND_CLUB;
				p++;
			}
			else
				Msg.m_nChatKind = CHAT_KIND_SECRET;

			while(1){
				if( Msg.m_nChatKind == CHAT_KIND_CLUB )
					break;
				m_sId.push_back(p[0]);
				p++;
				if(p[0] == ' ') {		// 아이디 뒤에 말이 있을경우
					m_sId.push_back('\0');
					p++;
					strcpy(Msg.m_sToName, m_sId.c_str());
					break;
				}
				else if(p[0] == 0) {	// 귓말 아이디만 칠 경우
					m_sId.push_back('\0');
					strcpy(Msg.m_sToName, m_sId.c_str());
					p[0] = '\0';

					break;
				}
			}
		}
		else {							// 보통 말일 경우
			p = buff;
			Msg.m_nChatKind = CHAT_KIND_PLAY;
			strcpy(Msg.m_sToName, "");
		}

		_snprintf(Msg.m_sMessage, MESSAGE_SIZE, p);

		if( (3 < m_nSendCount) && ((timeGetTime() - m_nPassTime) < 10000) )
		{
			char sText[STRLEN_64]="";

			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900073 ,0));
			assert(pMTable);

			_snprintf(sText, STRLEN_64, pMTable->m_sMessage, 10-((timeGetTime()-m_nPassTime) /1000));
			SetChatMessage(sText, true, COLOR_ANNOUNCE);
			pControl->ClearText();
			return;
		}
        
		if( (timeGetTime() - m_nPassTime) < 2000 )
			m_nSendCount++;
		else
			m_nSendCount=0;

		m_nPassTime = timeGetTime();

		sprintf(m_cChatting, Msg.m_sMessage);

		PutSendMessage(&Msg);

		CInputManager::GetPtr()->InitInputKey();

		pControl->ClearText();
	}
}

void CChattingDialog::FUNCTION_ChattingFunctionKeyInGamePlay(const char* szwindow)
{
	if( (0 < m_nSendCount) && ((timeGetTime() - m_nPassTime) < 5000) )
		return;
	    
	if( (timeGetTime() - m_nPassTime) < 500 )
		m_nSendCount++;
	else
		m_nSendCount=0;

	m_nPassTime = timeGetTime();

	std::string buff;
	buff = szwindow;

	CCSSendMessage Msg;

	_snprintf(Msg.m_sMessage, MESSAGE_SIZE, buff.c_str());

	Msg.m_nChatKind = CHAT_KIND_PLAY;
	_snprintf(Msg.m_sToName, PLAYER_NAME_SIZE, "%s", "" );

	PutSendMessage(&Msg);

	CInputManager::GetPtr()->InitInputKey();
}

void CChattingDialog::FUNCTION_ChattingWhisperListPopup()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl( "Static_WhisperPopUp" );
	if(pControl == NULL) return;
	if( pControl->GetVisible() == N3UI_VISIBLE_TRUE )
		pControl->SetVisible(N3UI_VISIBLE_FALSE);
	else {
		pControl->SetVisible(N3UI_VISIBLE_TRUE);

		vector<string>::reverse_iterator iter;
		int i=0;
		char sText[MAX_PATH]="";

		for(iter = CChattingDialog::GetPtr()->m_sWhisperId.rbegin();iter != CChattingDialog::GetPtr()->m_sWhisperId.rend();iter++)
		{
            _snprintf(sText, MAX_PATH, "Static_WhisperList%d", i);
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
			_snprintf(sText, MAX_PATH, "Static_WhisperList%d", i++);
			UIFunction::Set_ControlText( sText, (*iter).c_str());
		}

		for(;i<LIST5_SIZE;i++)
		{
            sprintf(sText, "Static_WhisperList%d", i);
			pControl = CClientUIManager::GetPtr()->FindEventControl( sText );
			pControl->SetVisible(N3UI_VISIBLE_FALSE);
		}
	}		
}
void CChattingDialog::TimeDeleyToDeleteChattingList()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("ChattingList");
	if (pControl == NULL) return;
	
	if(7000<(timeGetTime()-m_nFadeOutTime))
	{
		m_nFadeOutTime = timeGetTime();
		m_MessageList.pop_front();
		pControl->RemoveItem(0);
	}
}

void CChattingDialog::EnableChatting()
{
	char szbuff[MAX_PATH];
	sprintf(szbuff, "ChattingCommand");
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szbuff);
	if(pControl == NULL) return;
	CUIControl::RequestFocus(pControl);
}

void CChattingDialog::ChattingListClear()
{
	char szbuff[MAX_PATH];
	sprintf(szbuff, "ChattingList");
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(szbuff);
	if(pControl == NULL) return;
	pControl->RemoveAllItems();	
}

void CChattingDialog::ChattingListDelete()
{
	if(!m_MessageList.empty())
		m_MessageList.clear();
	CUIControl* pChatCommand = CClientUIManager::GetPtr()->FindEventControl("ChattingCommand");
	if(pChatCommand == NULL) return;
	pChatCommand->ClearText();
}

void CChattingDialog::KeyEventSecretListRefresh()
{
	CUIControl* pCommand = CClientUIManager::GetPtr()->FindEventControl( "ChattingCommand" );
	assert(pCommand);

	char sText[MAX_PATH]="";
	char sR[MAX_PATH]="";

	if(m_nWhisperIdIndex == -1)
		pCommand->ClearText();
	else if(m_sWhisperId.size() > 0) {
		sprintf(sR, "/%s ", m_sWhisperId[(m_sWhisperId.size()-m_nWhisperIdIndex)-1].c_str());
		pCommand->SetText(sR);
	}
}

void CChattingDialog::ChattingStringFitter(std::string* str, const char* xChar)
{
	std::string findString = xChar;
	std::string::size_type offset = 0;

	while(true)
	{
		offset = str->find(findString,offset);
		if(std::string::npos == offset)
			break;
		else
			str->replace(offset, findString.length(), "");
	}
}

void CChattingDialog::ChattingStringCommonFitter(std::string* str)
{
	std::string::size_type offset = 0;

	for(int i=0;i<static_cast<int>(str->length());i++)
	{
		for(int j=0;j<32;j++)
		{
			if( str->at(i) == j )
			{
				str->replace(i, 1, "");
				i--;
			}
		}

		for(int j=127;j<161;j++)
		{
			if( str->at(i) == j )
			{
				str->replace(i, 1, "");
				i--;
			}
		}
	}	
}

void CChattingDialog::FUNCTION_ChattingListSort( char idx )
{
	CChattingDialog::GetPtr()->ChattingListClear();

	std::deque<CHAT_MESSAGE>::iterator iter;

	switch( idx )
	{
	case 0:				// 전체
		{
			for(iter = CChattingDialog::GetPtr()->m_MessageList.begin();iter != CChattingDialog::GetPtr()->m_MessageList.end();iter++)
				CChattingDialog::GetPtr()->SetChatMessageRefresh(iter->sMessage.c_str(), true, iter->sColor);
		}
		break;
	case 1:				// 클럽
		{
			for(iter = CChattingDialog::GetPtr()->m_MessageList.begin();iter != CChattingDialog::GetPtr()->m_MessageList.end();iter++)
			{
				if( iter->sColor == (D3DXCOLOR)COLOR_CLUB )
					CChattingDialog::GetPtr()->SetChatMessageRefresh(iter->sMessage.c_str(), true, iter->sColor);
			}
		}
		break;
	case 2:				// 귓말
		{
			for(iter = CChattingDialog::GetPtr()->m_MessageList.begin();iter != CChattingDialog::GetPtr()->m_MessageList.end();iter++)
			{
				if( iter->sColor == (D3DXCOLOR)COLOR_SECRET )
					CChattingDialog::GetPtr()->SetChatMessageRefresh(iter->sMessage.c_str(), true, iter->sColor);
			}
		}
		break;
	case 3:				// 공지, 초대
		{
			for(iter = CChattingDialog::GetPtr()->m_MessageList.begin();iter != CChattingDialog::GetPtr()->m_MessageList.end();iter++)
			{
				if( iter->sColor == (D3DXCOLOR)COLOR_NOTICE )
					CChattingDialog::GetPtr()->SetChatMessageRefresh(iter->sMessage.c_str(), true, iter->sColor);
			}
		}
		break;
	}

	char sText[STRLEN_64]="";

	for( int i=0;i<4;i++ )
	{
		_snprintf( sText, STRLEN_64, "Chat_Btn_0%d", i);
		CUIButton* pChatKind = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChatKind);
		if( idx == i )	
			pChatKind->SetPressed(true);
		else
			pChatKind->SetPressed(false);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

int CChattingDialog::URLEncode2(const char* sIn, BYTE** ppOut, int& ALen)
{ 
	const int nLen = strlen(sIn) + 1;

	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn;
	BYTE b = 0;

	*ppOut = NULL;
	int nOutLen = nLen * 3 - 2;
	//alloc out buffer
	//pOutBuf = (LPBYTE)sOut.GetBuffer(nLen * 3 - 2);//new BYTE [nLen * 3];
	pOutBuf = new BYTE[ nOutLen + 1];
	memset(pOutBuf, 0, nOutLen + 1);

	if(pOutBuf)
	{
		pInTmp = pInBuf;
		pOutTmp = pOutBuf;

		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else if(*pInTmp == ' ')
			{
				*pOutTmp++ = '+';
			}
			else
			{
				*pOutTmp++ = '%';
				*pOutTmp++ = toHex(*pInTmp>>4);
				*pOutTmp++ = toHex(*pInTmp%16);
			}
			pInTmp++;
		}

		*pOutTmp = '\0';
		*ppOut=pOutBuf;

	return 1;
	}
	return 0;
}

// guess rafaelcn already found answer, but
// I use something like this
// helper to convert %XX to chars
inline char x2c ( unsigned char* in)
{
	register char digit;
	digit = ( (*in) >= 'A' ? (((*in) & 0xdf) - 'A')+10 : ((*in) - '0'));
	digit *= 16;
	in++;
	digit += ((*in) >= 'A' ? (((*in) & 0xdf) - 'A')+10 : ((*in) - '0'));
	return(digit);
}

//do the work
char* CChattingDialog::URLdecode( const char* in )
{
	int len = strlen(in);
	unsigned char* inBuff = (unsigned char*)(in);
	unsigned char* out = new unsigned char[len+1];
	unsigned char* outBuff = out;

	while( *inBuff )
	{
		if ( *inBuff == '%' )
		{
			inBuff++;
			*outBuff = x2c(inBuff);
			inBuff++;
		}
		else if ( *inBuff == '+' )
		{
			*outBuff=' ';
		}
		else
		{
			*outBuff=*inBuff;
		}
		outBuff++;
		inBuff++;

	}
	*outBuff =0;
	return (char*)out;
}

//int main(int argc, char* argv[])
//{
//	printf("Hello World!\n");
//	char *pchIn = "±?°æ?ð AμAc";
//
//	BYTE* pOut = NULL;
//	int nLen = 0;
//
//	URLEncode2(pchIn, &pOut, nLen);
//
//	if(pOut)
//	{
//		printf("[Encode] pchIn:%s, pOut:%s\n\n", pchIn, pOut);
//	}
//}
