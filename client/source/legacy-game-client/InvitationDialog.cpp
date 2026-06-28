#include "StdAfx.h"
#include "InvitationDialog.h"
#include "PutPacket.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "../InterfaceLibrary/UIIMEEditBox.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

CInvitationDialog::CInvitationDialog(void)
{
	m_bIsUpdate		= false;
	m_nUpdateTime	= 0;
	m_nTab			= 0;
	m_nPage			= 0;
}

CInvitationDialog::~CInvitationDialog(void)
{

}

void CInvitationDialog::Terminate()
{

}

void CInvitationDialog::Init()
{
	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	
	PutLobbyList(m_nPage);

	m_bIsUpdate = true;

	CUIIMEEditBox* pName=dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl("Invite_Msg"));
	assert(pName);
	pName->ClearText();

	char sText[STRLEN_64]="";

	for(int i=0;i<LIST10_SIZE;i++)
	{
		_snprintf(sText, STRLEN_64, "Invite_ChkBox%d", i);
		CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChkBox);
		pChkBox->SetCheckValue(false);
	}
}

void CInvitationDialog::Update()
{
	if(m_bIsUpdate && timeGetTime() - m_nUpdateTime > 3000 )
	{
		m_nUpdateTime = timeGetTime();

		switch( m_nTab )
		{
		case 0:
			PutLobbyList(m_nPage);
			break;
		case 1:
			break;
		case 2:
			PutBuddyInfo(m_nPage);
			break;
		}
	}
}

void CInvitationDialog::ListCheck(int idx)
{
	char sText[STRLEN_64]="";
	sprintf(sText, "Invite_ChkBox%d", idx);
	
	CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
	assert(pChkBox);

	if(m_LobbyList[idx].m_nState != LOBBY_STATE_EMPTY)
	{
		if(pChkBox->GetChecked())
			pChkBox->SetCheckValue(false);
		else
			pChkBox->SetCheckValue(true);
	}
}

void CInvitationDialog::LobbyListUpdate()
{
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "%d", m_nPage+1);
	UIFunction::Set_ControlText( "InvitePageNum", sText );

	for(int i=0;i<LIST10_SIZE;i++)
	{
		switch( m_nTab )
		{
		case 0:					// 대기자 리스트
			{
				if(m_LobbyList[i].m_nState == LOBBY_STATE_EMPTY)
				{
					_snprintf(sText, STRLEN_64, "Choice_ListNum0%d", i);
					CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
					pControl->SetVisible(N3UI_VISIBLE_FALSE);
				}
				else
				{
					_snprintf(sText, STRLEN_64, "Choice_ListNum0%d", i);
					CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
					pControl->SetVisible(N3UI_VISIBLE_TRUE);

					_snprintf(sText, STRLEN_64, "Invite_Name%d", i);
					CUIControl* pName = pControl->FindEventControl(sText);
					pName->SetText(m_LobbyList[i].m_sName);

					_snprintf(sText, STRLEN_64, "Invite_Position_Icon%d", i);
					UICommonMethod::PositionIconDraw(sText, m_LobbyList[i].m_nPosition, UICommonMethod::HOME_TEAM);
					
					_snprintf(sText, STRLEN_64, "Invite_Lv%d", i);
					CUIControl* pLv = pControl->FindEventControl(sText);
					_snprintf(sText, STRLEN_64, "%d", m_LobbyList[i].m_nLevel);
					pLv->SetText(sText);

					_snprintf(sText, STRLEN_64, "Invite_Ment%d", i);
					CUIControl* pMent = pControl->FindEventControl(sText);
					pMent->SetText(m_LobbyList[i].m_sMent);
				}
			}
			break;
		case 1:					// 클럽 리스트
			{

			}
			break;
		case 2:					// 친구 리스트
			{
				CBuddyInfo BInfo = (*m_pObjPlayerInfo->m_vBuddyList.at(i+(m_nPage*LIST10_SIZE)));
				if(BInfo.m_nState == LOBBY_STATE_EMPTY)
					break;
				else
				{
					_snprintf(sText, STRLEN_64, "Choice_ListNum0%d", i);
					CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sText);
					pControl->SetVisible(N3UI_VISIBLE_TRUE);

					_snprintf(sText, STRLEN_64, "Invite_Name%d", i);
					CUIControl* pName = pControl->FindEventControl(sText);
					pName->SetText(BInfo.m_sName);

					_snprintf(sText, STRLEN_64, "Invite_Position_Icon%d", i);
					UICommonMethod::PositionIconDraw(sText, m_LobbyList[i].m_nPosition, UICommonMethod::HOME_TEAM);

					_snprintf(sText, STRLEN_64, "Invite_Lv%d", i);
					CUIControl* pLv = pControl->FindEventControl(sText);
					_snprintf(sText, STRLEN_64, "%d", BInfo.m_nLevel);
					pLv->SetText(sText);

					_snprintf(sText, STRLEN_64, "Invite_Ment%d", i);
					CUIControl* pMent = pControl->FindEventControl(sText);
					//pMent->SetText(BInfo.m_sMent);
				}
			}
			break;
		}
	}
}

void CInvitationDialog::InvitationOk()
{
	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl("Invite_Msg");
	assert(pControl);

	int nCheckNum=0;

	for(int i=0;i<LIST10_SIZE;i++)
	{
		char sText[STRLEN_64]="";
		sprintf(sText, "Invite_ChkBox%d", i);
		CUICheckBox* pChkBox = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pChkBox);
		
		if(m_LobbyList[i].m_nState != LOBBY_STATE_EMPTY && pChkBox->GetChecked())
		{
			PutInvitePlayer(m_LobbyList[i].m_nPlayerSeq, pControl->GetTextEx().c_str());
		}
		else
			nCheckNum++;
	}

	if(nCheckNum == 10)
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20500, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
	else
	{
		SN3UIUserFunctionParameter cParam;
		cParam.AddParam( "Window", "InviteRoom" );

		CClientUIManager::GetPtr()->ProcUserFunction( "HideWindow", &cParam );
	}

	m_bIsUpdate = false;
}

void CInvitationDialog::InvitationCancel()
{
	m_bIsUpdate = false;
}

void CInvitationDialog::LoadLobbyList(CSCLobbyList* pList)
{
	for(int i=0;i<LIST10_SIZE;i++)
		m_LobbyList[i] = pList->m_cLobbyData[i];
}