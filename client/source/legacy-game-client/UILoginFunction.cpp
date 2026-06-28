#include "stdafx.h"
#include "PutPacket.h"
#include "ClientUIManager.h"

//로그인 부분
IMPLEMENT_USERFUNCTION( Login_OK ) 
{
	UIMessage::SendMessage_SignIn();
	PutCertifyLogin(CGameProject::s_pGameProject->m_strName, CGameProject::s_pGameProject->m_strPass);
}
