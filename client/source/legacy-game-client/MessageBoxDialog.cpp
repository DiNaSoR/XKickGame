//***********************************************************************************
//	File		:	MessageBoxDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "StdAfx.h"
#include "MessageBoxDialog.h"
#include "PutPacket.h"
#include "CreateCharacterProcess.h"
#include "UIIMEEditBox.h"
#include "ChoiceTrioProcess.h"
#include "UIFunction.h"
#include "ChoiceClassProcess.h"

CMessageBoxDialog::CMessageBoxDialog(void)
{
	m_nState = MESSAGEBOX_NONE;
	m_MessageIndex[MESSAGEBOX_NONE]			= 0;
}

CMessageBoxDialog::~CMessageBoxDialog(void)
{
}

void CMessageBoxDialog::Enable(int nState, int nMsgID, int nMsgType, int nErrorCode)
{
	m_nState = nState;
	
	CUIControl* pMainControl = NULL;
	CUIControl* pMessageControl = NULL;
	
	pMainControl = CClientUIManager::GetPtr()->FindEventControl("MessageBox");
	pMessageControl = CClientUIManager::GetPtr()->FindEventControl("Message_Text");
	
	if (!pMainControl || !pMessageControl)
	{
		MessageBox(NULL, "CMessageBoxDialog::Enable", "Error", MB_OK);
		return;
	}

	char sText[MAX_PATH]="";

	switch(m_nState)
	{
	case MESSAGEBOX_NONE:
	case MESSAGEBOX_EXIT:
	case MESSAGEBOX_LANGEXIT:
	case MESSAGEBOX_GAMEEXIT:
	case MESSAGEBOX_CHOICECLASS:
	case MESSAGEBOX_NEWLOGIN:
	case MESSAGEBOX_REFRESHITEM:
		{
			CResMessageTableFormat *Table = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, nMsgID ,0));//메세지 얻어오기
			
			if(!Table) 
				pMessageControl->SetText("The message code which is not defined");
			else
			{
				if(nErrorCode)
				{
					_snprintf(sText, MAX_PATH, "%s\n(Error Code: %d)\n", Table->m_sMessage, nErrorCode);
					pMessageControl->SetText(sText);
				}
				else
					pMessageControl->SetText(Table->m_sMessage);
			}
		}
		break;
	case MESSAGEBOX_DUPLICATION:
		{
			CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 200219, 0));
			assert(pTable);

			CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl("CharacterName");		// 캐릭터 명
			assert(pName);
	
			_snprintf(sText, MAX_PATH, "%s %s", pName->GetTextEx().c_str(), pTable->m_sMessage);
			pMessageControl->SetText(sText);
		}
		break;
	case MESSAGEBOX_JUMIN:
		break;
	}

	pMainControl->SetVisible(N3UI_VISIBLE_TRUE);

	pMainControl->Set_FocusedPage(pMainControl);
	CUIControl::RequestFocus(pMainControl);

	CUIControl* pYESNO = CClientUIManager::GetPtr()->FindEventControl("MESSAGE_YESNO");
	assert(pYESNO);
	CUIControl* pCONFIRMCANCEL = CClientUIManager::GetPtr()->FindEventControl("MESSAGE_CONFIRMCANCEL");
	assert(pCONFIRMCANCEL);
	CUIControl* pCONFIRM = CClientUIManager::GetPtr()->FindEventControl("MESSAGE_CONFIRM");
	assert(pCONFIRM);

	CUIControl* pText = CClientUIManager::GetPtr()->FindEventControl("Message_Text");
	assert(pText);

	UIFunction::Set_UIVisibility( "InputIme", N3UI_VISIBLE_FALSE );
	
	pText->SetVisible(N3UI_VISIBLE_TRUE);

	switch(nMsgType)
	{
	case MSGBOXTYPE_YESNO:
		{
			pYESNO->SetVisible(N3UI_VISIBLE_TRUE);
			pCONFIRMCANCEL->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRM->SetVisible(N3UI_VISIBLE_FALSE);
		}
		break;
	case MSGBOXTYPE_CONFIRMCANCEL:
		{
			pYESNO->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRMCANCEL->SetVisible(N3UI_VISIBLE_TRUE);
			pCONFIRM->SetVisible(N3UI_VISIBLE_FALSE);
		}
		break;
	case MSGBOXTYPE_CONFIRM:
		{
			pYESNO->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRMCANCEL->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRM->SetVisible(N3UI_VISIBLE_TRUE);
			
			CUIControl* pOk = pCONFIRM->FindEventControl( "Confirm" );
			assert(pOk);

			CUIControl::RequestFocus(pOk);
		}	
		break;
	case MSGBOXTYPE_IMECONFIRMCANCEL:
		{
			pYESNO->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRMCANCEL->SetVisible(N3UI_VISIBLE_FALSE);
			pCONFIRM->SetVisible(N3UI_VISIBLE_TRUE);
			pText->SetVisible(N3UI_VISIBLE_FALSE);
			UIFunction::Set_UIVisibility( "InputIme", N3UI_VISIBLE_TRUE );
		
			CUIIMEEditBox* pIme = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("ImeText"));
			assert(pIme);
			pIme->ClearText();
			CUIControl::RequestFocus(pIme);
		}
		break;
	}
}

void CMessageBoxDialog::OK()
{
	switch(m_nState)
	{
	case MESSAGEBOX_NONE:
		{
			CUIControl* pMainControl = CClientUIManager::GetPtr()->FindEventControl("MessageBox");
			pMainControl->SetVisible(N3UI_VISIBLE_FALSE);
		}
		break;
	case MESSAGEBOX_EXIT:
		{
			NiWindowRef hWnd = NiApplication::ms_pkApplication->GetAppWindow()->GetWindowReference();
			::PostMessage( hWnd, WM_DESTROY, 0, 0 );
		}
		break;
	case MESSAGEBOX_LANGEXIT:
		{
			CGameOptionDialog::GetPtr()->FUNCTION_LanguageAdjust();
			NiWindowRef hWnd = NiApplication::ms_pkApplication->GetAppWindow()->GetWindowReference();
			::PostMessage( hWnd, WM_DESTROY, 0, 0 );
		}
		break;
	case MESSAGEBOX_GAMEEXIT:
		PutGameExit(PLAYER_EXIT_TRANSPORT);
		break;
	case MESSAGEBOX_CHOICECLASS:
		{
			CProcessManager::GetPtr()->SetNextProcess(new CChoiceClassProcess());
		}
		break;
	case MESSAGEBOX_JUMIN:
		{
			CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			assert(pCurrentProcess);
			pCurrentProcess->Select_Delete();
		}
		break;
	case MESSAGEBOX_DUPLICATION:
		{
			CUIControl* pName = CClientUIManager::GetPtr()->FindEventControl("CharacterName");		// 캐릭터 명
			assert(pName);
			CCreateCharacterProcess* pCurrentProcess = dynamic_cast<CCreateCharacterProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			assert(pCurrentProcess);
			_snprintf(pCurrentProcess->m_sCharacterName, PLAYER_NAME_SIZE, "%s", pName->GetTextEx().c_str() );
			pCurrentProcess->m_bDuplication = true;
		}
		break;
	case MESSAGEBOX_NEWLOGIN:
		PutDrawforcePlayer(CGameProject::s_pGameProject->m_strName, CGameProject::s_pGameProject->m_strPass);
		break;
	case MESSAGEBOX_REFRESHITEM:
		PutRefreshShop(BUY_KIND_POINT);
		break;
	}

	CUIControl* pGameOption = CClientUIManager::GetPtr()->FindEventControl("GameOption");
	CUIControl* pPlayerInfo = CClientUIManager::GetPtr()->FindEventControl("PlayerInformation");
	if( pGameOption != NULL && pGameOption->GetVisible() != N3UI_VISIBLE_FALSE )
	{
		pGameOption->Set_FocusedPage(pGameOption);
		CUIControl::RequestFocus(pGameOption);
	}
	else if( pPlayerInfo != NULL && pPlayerInfo->GetVisible() != N3UI_VISIBLE_FALSE )
	{
		pPlayerInfo->Set_FocusedPage(pPlayerInfo);
		CUIControl::RequestFocus(pPlayerInfo);
	}
	else
		CUIControl::ClearFocus();
}

void CMessageBoxDialog::Cancel()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("GameOption");
	if(pControl != NULL)
	{
		pControl->Set_FocusedPage(pControl);
		CUIControl::RequestFocus(pControl);
	}
	else
		CUIControl::ClearFocus();
}

void CMessageBoxDialog::SetOkMessage(const char* strMessage)
{
	assert(strMessage);
	CUIControl* pMessageControl = CClientUIManager::GetPtr()->FindEventControl("Ok_Message");
	if (pMessageControl != NULL)
		pMessageControl->SetText(strMessage);
}

void CMessageBoxDialog::SetOkCanelMessage(const char* strMessage)
{
	assert(strMessage);
	CUIControl* pMessageControl = CClientUIManager::GetPtr()->FindEventControl("OkCancel_Message");
	if (pMessageControl != NULL)
		pMessageControl->SetText(strMessage);
}
