#include "stdafx.h"
#include "PacketManager.h"
#include "PutPacket.h"
#include "ChoiceServerProcess.h"
#include "ObjServerInfo.h"
#include "../InterfaceLibrary/UIButton.h"

//////////////////////////////////////////////////////////////////////////
//서버 선택부분
IMPLEMENT_USERFUNCTION( CHOICE_SERVER ) 
{
	if(!CPacketManager::GetPtr()->m_bIsCertifyToGame)
	{
		CPacketManager::GetPtr()->m_bIsCertifyToGame = true;
		PutCertifyExit(PLAYER_EXIT_TRANSPORT);

		int	index = atol( pParam->GetValue( "Index" ) );

		CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
		assert(pCurrentProcess);

		CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, index));
		assert(pServerInfo);

		CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTSERVERLIST, pServerInfo);

		pCurrentProcess->m_pObjPlayerInfo->m_nServerCode = pServerInfo->m_nServerCode;

		PutGameLogin(&pServerInfo->m_cAddress);
	}
}

IMPLEMENT_USERFUNCTION( SERVER_TAB )		// 서버 탭
{
	int	index = atol( pParam->GetValue( "Index" ) );

	CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	assert(pCurrentProcess);
	pCurrentProcess->FUNCTION_TabChange(index);
}

IMPLEMENT_USERFUNCTION( SELECT_SERVER_CHOICE )
{
	if(!CPacketManager::GetPtr()->m_bIsCertifyToGame)
	{
		CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
		assert(pCurrentProcess);

		if( pCurrentProcess->m_nSelectServer != -1 )
		{
			CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, pCurrentProcess->m_nSelectServer));
			assert(pServerInfo);

			CPacketManager::GetPtr()->m_bIsCertifyToGame = true;
			PutCertifyExit(PLAYER_EXIT_TRANSPORT);
			
			CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTSERVERLIST, pServerInfo);

			pCurrentProcess->m_pObjPlayerInfo->m_nServerCode = pServerInfo->m_nServerCode;

			PutGameLogin(&pServerInfo->m_cAddress);
		}
	}
}

IMPLEMENT_USERFUNCTION( QUICK_SERVER_CHOICE )
{
    if(!CPacketManager::GetPtr()->m_bIsCertifyToGame)
	{
		CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
		assert(pCurrentProcess);

		CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, pCurrentProcess->m_nAutoSelectServer));
		assert(pServerInfo);

		CPacketManager::GetPtr()->m_bIsCertifyToGame = true;
		PutCertifyExit(PLAYER_EXIT_TRANSPORT);
		
		CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTSERVERLIST, pServerInfo);

		pCurrentProcess->m_pObjPlayerInfo->m_nServerCode = pServerInfo->m_nServerCode;

		PutGameLogin(&pServerInfo->m_cAddress);
	}
}

IMPLEMENT_USERFUNCTION( NEWBIE_SERVER )
{
	if(!CPacketManager::GetPtr()->m_bIsCertifyToGame)
	{
		CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
		assert(pCurrentProcess);

		CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, 0));
		assert(pServerInfo);

		CPacketManager::GetPtr()->m_bIsCertifyToGame = true;
		PutCertifyExit(PLAYER_EXIT_TRANSPORT);
		
		CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTSERVERLIST, pServerInfo);

		pCurrentProcess->m_pObjPlayerInfo->m_nServerCode = pServerInfo->m_nServerCode;

		PutGameLogin(&pServerInfo->m_cAddress);
	}
}