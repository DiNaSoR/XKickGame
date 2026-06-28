#include "stdafx.h"
#include "ChoiceTrioProcess.h"
#include "ChoiceServerProcess.h"
#include "ChoiceRoomProcess.h"
#include "ReadyRoomProcess.h"
#include "GamePlayProcess.h"

#include "ItemShopProcess.h"
#include "ItemBagProcess.h"
#include "SkillShopProcess.h"
#include "SkillBagProcess.h"
#include "GuideTutorialProcess.h"
#include "ChoiceClassProcess.h"
#include "CreateCharacterProcess.h"
#include "TutorialMainProcess.h"

#include "UICommonMethod.h"
#include "UIFunction.h"
#include "ObjServerInfo.h"

#include "GetPacket.h"
#include "PutPacket.h"

#include "PlayModeManager.h"
#include "GameLoadingProcess.h"
#include "GameResultProcess.h"

#include "UtlLog.h"

#include "Common.h"
//////////////////////////////////////////////////////////////////////////
// 서버에서 받는 패킷 함수
//////////////////////////////////////////////////////////////////////////
void GetCertifyLogin(CHeadPacket* pPacket)
{
	CSCCertifyLogin*	pBody		= (CSCCertifyLogin*)pPacket;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_nMemberSeq = pBody->m_nMemberSeq;
		CPacketManager::GetPtr()->m_bIsCertifyLoginDone = true;
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -21:
	case -22:
	case -23:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -13: //존재하지 않는 계정입니다.
	case -14: //해당 계정의 암호가 틀립니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -24: //이미 접속된 계정입니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NEWLOGIN, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRMCANCEL);
		break;
	case -15: //운영자에 의해 블럭된 계정입니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetInstantLogin(CHeadPacket* pPacket)
{
	CObjPlayerInfo*		pPlayer			= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CSCGameLogin*		pBody			= (CSCGameLogin*)pPacket;
	int					nMsgCode		= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CProcessManager::GetPtr()->SetChangeProcess(new CChoiceServerProcess);
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
// 공지사항 목록 (CYG)
void GetNoticeList(CHeadPacket* pPacket)
{

	CSCNoticeList*	pBody		= (CSCNoticeList*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		for(int i=0;i<pBody->m_nCount;i++)
			CGameProject::s_pGameProject->s_Notice.m_cNoticeList[i] = pBody->m_cNoticeList[i];
		CGameProject::s_pGameProject->s_Notice.m_nCount = pBody->m_nCount;

		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCertifyExit(CHeadPacket* pPacket)
{
	CSCCertifyExit*	pBody		= (CSCCertifyExit*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		if(pBody->m_nReason == PLAYER_EXIT_DRAWFORCE)
		{
			NiWindowRef hWnd = NiApplication::ms_pkApplication->GetAppWindow()->GetWindowReference();
			PostMessage( hWnd, WM_DESTROY, 0, 0 );
			return;
		}

		CPacketManager::GetPtr()->CloseTCPSocket(); //인증서버 연결 종료
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetMemberInfo(CHeadPacket* pPacket)
{
	CSCMemberInfo*		pBody		= (CSCMemberInfo*)pPacket;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_nLastSeq			= pBody->m_nLastSeq;
		pMember->m_nCount			= pBody->m_nCount;
		pMember->m_nTutorial		= pBody->m_nTutorial;
		pMember->m_nQuest			= pBody->m_nQuest;
		pMember->m_cMoney			= pBody->m_cMoney;
		pMember->m_nLoginDate		= pBody->m_nLoginDate;
		pMember->m_nDeleteDate		= pBody->m_nDeleteDate;

		CPacketManager::GetPtr()->m_bIsMemberInfoDone = true;
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCharacterInfo(CHeadPacket* pPacket)
{
	CSCCharacterInfo*	pBody		= (CSCCharacterInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if(pBody->m_cCharacterInfo.m_nPlayerSeq == 0)
				return;

			pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_TRIOLIST, pBody->m_cCharacterInfo.m_nPlayerSeq));
			if(pPlayer == NULL) //해당 캐릭터 인스턴스 존재하지 않음
			{
				pPlayer = new CObjPlayerInfo;
				CObjectManager::GetPtr()->PushObject(OBJECT_TRIOLIST, pPlayer);

                CObjMemberInfo * pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
                assert( pMember != NULL );

                if( pMember->m_nLastSeq == 0 )
                {
                    pMember->m_nLastSeq = pBody->m_cCharacterInfo.m_nPlayerSeq;
                }
			}

			pPlayer->SetObjSeq( pBody->m_cCharacterInfo.m_nPlayerSeq );
			pPlayer->m_nOrder		= pBody->m_cCharacterInfo.m_nOrder;
			pPlayer->m_nPosition	= pBody->m_cCharacterInfo.m_nPosition;
			pPlayer->m_nCondition	= pBody->m_cCharacterInfo.m_nCondition;
			pPlayer->m_nAlias		= pBody->m_cCharacterInfo.m_nAlias;
			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, pBody->m_cCharacterInfo.m_sName);
			memcpy(&pPlayer->m_cLevel, &pBody->m_cCharacterInfo.m_cLevel, sizeof(CLevel));
			memcpy(&pPlayer->m_cShape, &pBody->m_cCharacterInfo.m_cShape, sizeof(CShape));
			memcpy(&pPlayer->m_nEquipWear, &pBody->m_cCharacterInfo.m_nEquipWear, sizeof(int)*MAX_EQUIP);
			memcpy(&pPlayer->m_nHomeWear, &pBody->m_cCharacterInfo.m_nHomeWear, sizeof(int)*4);
			memcpy(&pPlayer->m_nAwayWear, &pBody->m_cCharacterInfo.m_nAwayWear, sizeof(int)*4);
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCharacterEnd(CHeadPacket* pPacket)
{
	CSCCharacterEnd*	pBody		= (CSCCharacterEnd*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			//? 임시방편적 코드 반드시 드러내야된다
			CBaseProcess* temp = CProcessManager::GetPtr()->Get_CurrentProcess();

			CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(temp);

			if(!pCurrentProcess) return;

			_Assert(pCurrentProcess, "GetCharacterEnd", 0, "", 0);

			pCurrentProcess->LoadCharacter();
			pCurrentProcess->Update_CharacterInfo();
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCreateCharacter(CHeadPacket* pPacket)
{
	CSCCreateCharacter*	pBody		= (CSCCreateCharacter*)pPacket;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
        {        
            CObjMemberInfo * pMember = CObjMemberManager::GetPtr()->GetMemberPointer();
            pMember->m_nLastSeq = 0;

			CProcessManager::GetPtr()->SetChangeProcess(new CChoiceTrioProcess);
        }
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -14:
	case -99:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -13:
	case -15:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetDeleteCharacter(CHeadPacket* pPacket)
{
	CSCDeleteCharacter*	pBody		= (CSCDeleteCharacter*)pPacket;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	CChoiceTrioProcess* pCurrentProcess = dynamic_cast<CChoiceTrioProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(!pCurrentProcess) return;
	_Assert(pCurrentProcess, "GetDeleteCharacter", 0, "", 0);
	int					nSelectSeq	= pCurrentProcess->m_cRotateCharacter.GetSelectSequence();

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_nDeleteDate		= pBody->m_nDeleteDate;
		CObjectManager::GetPtr()->DeleteObject(OBJECT_TRIOLIST, nSelectSeq);
        if( nSelectSeq == pMember->m_nLastSeq )
        {
            int nObjectSize = CObjectManager::GetPtr()->GetObjectSize( OBJECT_TRIOLIST );
            if( nObjectSize > 0 )
            {
                CResBaseFormat * pChar = CObjectManager::GetPtr()->GetSequenceField( OBJECT_TRIOLIST, 0 );
                pMember->m_nLastSeq    = pChar->GetObjSeq();
            }
        }

		pCurrentProcess->m_pObjPlayerInfo = NULL;
		pCurrentProcess->FUNCTION_Init();
		pCurrentProcess->Update_CharacterInfo();
		pCurrentProcess->FUNCTION_DateUpdate();

		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //주민등록번호가 틀립니다.
	case -15: //삭제한지 7일이 되지 않아 삭제할 수 없습니다.
	case -16: //해당 캐릭터가 존재하지 않습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChoiceCharacter(CHeadPacket* pPacket)
{
	CSCChoiceCharacter*	pBody		= (CSCChoiceCharacter*)pPacket;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		CChattingDialog::GetPtr()->ChattingListClear();
		CChattingDialog::GetPtr()->ChattingListDelete();

		//pMember->m_nLastSeq = pBody->m_nPlayerSeq;
		if( pPlayer->m_nPosition == POSITION_NB && pPlayer->m_cLevel.m_nLevel == 1 && !IsBit(pMember->m_nQuest, 0) )
		{
		//	if( pPlayer->m_nPosition == POSITION_NB && IsBit(pMember->m_nQuest, 0) )
		//		CProcessManager::GetPtr()->SetChangeProcess(new CTutorialMainProcess());

			if( pPlayer->m_nPosition == POSITION_NB && !IsBit(pMember->m_nQuest, 0) )
				CProcessManager::GetPtr()->SetChangeProcess(new CGuideTutorialProcess(0));
		}
		else
			CProcessManager::GetPtr()->SetChangeProcess(new CChoiceServerProcess);

		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetServerList(CHeadPacket* pPacket)
{
	CSCServerList*	pBody		= (CSCServerList*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CObjStack * pStack = CObjectManager::GetPtr()->GetObjStack( OBJECT_SERVERLIST );
			pStack->Terminate();

			for(int i = 0; i < LIST10_SIZE; i++)
			{
				if(pBody->m_cServerData[i].m_nState != SERVER_STATE_EMPTY)
				{
					CObjServerInfo* pServerInfo = new CObjServerInfo;

					pServerInfo->m_nState		= pBody->m_cServerData[i].m_nState;
					pServerInfo->m_nServerCode	= pBody->m_cServerData[i].m_nServerCode;
					_snprintf(pServerInfo->m_sTitle, SERVER_NAME_SIZE, "%s", pBody->m_cServerData[i].m_sTitle );
					pServerInfo->m_nMax			= pBody->m_cServerData[i].m_nMax;
					pServerInfo->m_nCurrent		= pBody->m_cServerData[i].m_nCurrent;
					pServerInfo->m_cAddress		= pBody->m_cServerData[i].m_cAddress;
					pServerInfo->m_nServerMatch	= pBody->m_cServerData[i].m_nServerMatch;

					CObjectManager::GetPtr()->PushObject(OBJECT_SERVERLIST, pServerInfo);
				}
			}
			//? 임시방편적 코드 반드시 드러내야된다

			//if( pPlayer->m_nPosition == POSITION_NB && pPlayer->m_cLevel.m_nLevel == 1 && !IsBit(pMember->m_nQuest, 0))
			//{
			//	CPacketManager::GetPtr()->m_bIsCertifyToGame = true;
			//	PutCertifyExit(PLAYER_EXIT_TRANSPORT);

			//	CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, 0));
			//	assert(pServerInfo);
			//	CObjectManager::GetPtr()->ClearInPushObject(OBJECT_SELECTSERVERLIST, pServerInfo);
			//	pPlayer->m_nServerCode = pServerInfo->m_nServerCode;

			//	PutGameLogin(&pServerInfo->m_cAddress);
			//}

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_CHOICESERVER) )
			{
				CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
				CChoiceServerProcess* pCurrentProcess = dynamic_cast<CChoiceServerProcess*>(temp);
				if(!pCurrentProcess) return;
				_Assert(pCurrentProcess, "GetServerList", 0, "", 0);
				pCurrentProcess->FUNCTION_TabChange(0);
			}
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetExecuteTutorial(CHeadPacket* pPacket)
{
	CSCExecuteTutorial*	pBody		= (CSCExecuteTutorial*)pPacket;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_nTutorial		= pBody->m_nTutorial;
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetExecuteQuest(CHeadPacket* pPacket)
{
	CSCExecuteQuest*	pBody		= (CSCExecuteQuest*)pPacket;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_nQuest		= pBody->m_nQuest;
		pMember->m_cMoney		= pBody->m_cMoney;
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameLogin(CHeadPacket* pPacket)
{
	CSCGameLogin*	pBody		= (CSCGameLogin*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			PutScheduleList( pPlayer->m_nServerCode );

			PutUDPPunching();
			PutUDPConfirm();
		}
		break;
	case -10:
		{
			CPacketManager::GetPtr()->m_bIsCertifyToGame = false;
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_GAMEEXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
	case -14:
	case -15:
	case -16:
	case -17:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -1: //해당 캐릭터가 이미 접속중입니다.
	case -2: //최대 접속자 수를 초과하였습니다.
		{
			CPacketManager::GetPtr()->m_bIsCertifyToGame = false;
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_GAMEEXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameExit(CHeadPacket* pPacket)
{
	CSCGameExit*		pBody		= (CSCGameExit*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	switch(pBody->m_nResponse)
	{
	case 0:
		if(pBody->m_nReason == PLAYER_EXIT_DRAWFORCE)
		{
			NiWindowRef hWnd = NiApplication::ms_pkApplication->GetAppWindow()->GetWindowReference();
			PostMessage( hWnd, WM_DESTROY, 0, 0 );
		}
		pPlayer->TerminatePlayer(); //플레이어 데이터 정리

		CPacketManager::GetPtr()->CloseTCPSocket(); //게임서버 연결 종료
		CPacketManager::GetPtr()->CloseUDPSocket(); //UDP 연결 종료

		PutInstantLogin(); //인증 서버 접속 요청(간단 접속)
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void GetUDPConfirm(CHeadPacket* pPacket)
{
	CSCUDPConfirm*	pBody		= (CSCUDPConfirm*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0: //펀칭 성공
		PutPlayerInfo();
		break;
	case -11: //재전송 요청
		PutUDPPunching();
		Sleep(500);
		PutUDPConfirm();
		break;
	case -12: //네트웍(UDP) 설정에 실패하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetPlayerInfo(CHeadPacket* pPacket)
{
	CSCPlayerInfo*		pBody		= (CSCPlayerInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CPlayerInfo*		pInfo		= &(pBody->m_cPlayerInfo);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pPlayer->InitPlayer();

			pPlayer->SetObjSeq( pInfo->m_nPlayerSeq );
			pPlayer->m_cLevel						= pInfo->m_cLevel;
			pPlayer->m_cBaseFaculty					= pInfo->m_cBaseFaculty;
			pPlayer->m_cRaiseFaculty				= pInfo->m_cRaiseFaculty;
			pPlayer->m_cTrainingFaculty				= pInfo->m_cTrainingFaculty;
			pPlayer->m_cTotalRecord					= pInfo->m_cTotalRecord;
			pPlayer->m_cQuarterRecord				= pInfo->m_cQuarterRecord;
			pPlayer->m_cTotalRanking				= pInfo->m_cTotalRanking;
			pPlayer->m_cQuarterRanking				= pInfo->m_cQuarterRanking;
			pPlayer->m_cItemOption					= pInfo->m_cItemOption;
			
			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, pInfo->m_sName);
			_snprintf(pPlayer->m_sMent, PLAYER_MENT_SIZE, pInfo->m_sMent);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetItemInfo(CHeadPacket* pPacket)
{
	CSCItemInfo*		pBody		= (CSCItemInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CGameItem*			pInfo;
	CObjItemInfo*		pItem;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo	= &pBody->m_cItemInfo[i];
				pItem	= new CObjItemInfo;

				pItem->m_nItemSeq		= pInfo->m_nItemSeq;
				pItem->m_nCode			= pInfo->m_nCode;
				pItem->m_nClass			= pInfo->m_nClass;
				pItem->m_nGrade			= pInfo->m_nGrade;
				pItem->m_nLevel			= pInfo->m_nLevel;
				pItem->m_nPrice			= pInfo->m_nPrice;
				pItem->m_nEquipKind		= pInfo->m_nEquipKind;
				pItem->m_nAmount		= pInfo->m_nAmount;

				memcpy(pItem->m_nOptionCode, pInfo->m_nOptionCode, sizeof(int)*ITEM_OPTION_SIZE);
				
				//테이블 포인터 저장
				pItem->m_pTable			= dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pInfo->m_nCode, 0));
				assert(pItem->m_pTable);

				pPlayer->m_vItemList.push_back(pItem);
			}
			// CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetSkillInfo(CHeadPacket* pPacket)
{
	CSCSkillInfo*		pBody		= (CSCSkillInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CSkillInfo*			pInfo;
	CObjSkillInfo*		pSkill;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo	= &pBody->m_cSkillInfo[i];

				//pPlayer->CreateBagSkill(pInfo);
				pSkill	= new CObjSkillInfo;

				pSkill->m_nSkillSeq		= pInfo->m_nSkillSeq;
				pSkill->m_nCode			= pInfo->m_nCode;
				pSkill->m_nEquipKind	= pInfo->m_nEquipKind;
				pSkill->m_nLevel		= pInfo->m_nLevel;

				pSkill->m_pTable		= dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pInfo->m_nCode, 0));
				assert(pSkill->m_pTable);
				pSkill->m_fCoolTime		= 0.0f;
				pSkill->m_fEndTime		= pSkill->m_pTable->m_fCool[pSkill->m_nLevel - 1];


				pPlayer->m_vSkillList.push_back(pSkill);
			}
			// CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetTrainingInfo(CHeadPacket* pPacket)
{
	CSCTrainingInfo*	pBody		= (CSCTrainingInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CTrainingInfo*		pInfo;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo		= &pBody->m_cTrainingInfo[i];

				pPlayer->CreateBagTraining(pInfo);
				/*
				pTraining	= new CObjTrainingInfo;

				pTraining->m_nTrainingSeq	= pInfo->m_nTrainingSeq;
				pTraining->m_nCode			= pInfo->m_nCode;
				pTraining->m_nEquipKind		= pInfo->m_nEquipKind;
				pTraining->m_nLevel			= pInfo->m_nLevel;

				//테이블 포인터 저장
				pTraining->m_pTable			= dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, pInfo->m_nCode, 0));
				assert(pTraining->m_pTable);

				pPlayer->m_vTrainingList.push_back(pTraining);
				*/
			}
			// CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCeremonyInfo(CHeadPacket* pPacket)
{
	CSCCeremonyInfo*	pBody		= (CSCCeremonyInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CCeremonyInfo*		pInfo;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo		= &pBody->m_cCeremonyInfo[i];

				pPlayer->CreateBagCeremony(pInfo);
				/*
				pCeremony	= new CObjCeremonyInfo;

				pCeremony->m_nCeremonySeq	= pInfo->m_nCeremonySeq;
				pCeremony->m_nCode			= pInfo->m_nCode;
				pCeremony->m_nEquipKind		= pInfo->m_nEquipKind;

				//테이블 포인터 저장
				pCeremony->m_pTable			= dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, pInfo->m_nCode, 0));
				assert(pCeremony->m_pTable);

				pPlayer->m_vCeremonyList.push_back(pCeremony);
				*/
			}
			// CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetQuestInfo(CHeadPacket* pPacket)
{
	CSCQuestInfo*		pBody		= (CSCQuestInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CQuestInfo*			pInfo;
	CObjQuestInfo*		pQuest;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			//pPlayer->m_vQuestList.clear();

			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo	= &pBody->m_cQuestInfo[i];
				pQuest	= new CObjQuestInfo();

				pQuest->m_nQuestSeq		= pInfo->m_nQuestSeq;
				pQuest->m_nCode			= pInfo->m_nCode;
				pQuest->m_nAmount		= pInfo->m_nAmount;
				pQuest->m_nPlayDate		= pInfo->m_nPlayDate;

				pPlayer->m_vQuestList.push_back(pQuest);
			}
			//CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetSaleList(CHeadPacket* pPacket)
{
	CSCSaleList*	pBody		= (CSCSaleList*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
            CPlayerInformationDialog::GetPtr()->SetTradePage(pBody->m_nPeriod, pBody->m_nTotalPage, pBody->m_nPage );

            int nCnt = sizeof(pBody->m_cSaleList) / sizeof(pBody->m_cSaleList[0]);
            for( int i=0; i< nCnt; i++ )
                CPlayerInformationDialog::GetPtr()->AddSaleList( pBody->m_nPeriod, pBody->m_cSaleList[i] );
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -13: //존재하지 않는 페이지 요청(그냥 아무 일도 없이 무시한다)
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetRoomInfo(CHeadPacket* pPacket)
{
	CSCRoomInfo*		pBody		= (CSCRoomInfo*)pPacket;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pRoom->InitRoom();

			pRoom->m_nState				= pBody->m_cRoomInfo.m_nState;
			pRoom->m_nMode				= pBody->m_cRoomInfo.m_nMode;
			pRoom->m_nCource			= pBody->m_cRoomInfo.m_nCource;
			pRoom->SetObjSeq(pBody->m_cRoomInfo.m_nRoomSeq);
			pRoom->m_nRoomJangTeam		= pBody->m_cRoomInfo.m_nRoomJangTeam;
			pRoom->m_nHomeJangSeq		= pBody->m_cRoomInfo.m_nHomeJangSeq;
			pRoom->m_nAwayJangSeq		= pBody->m_cRoomInfo.m_nAwayJangSeq;

			_snprintf(pRoom->m_sTitle, TITLE_NAME_SIZE, pBody->m_cRoomInfo.m_sTitle);
			_snprintf(pRoom->m_sPass, 5, pBody->m_cRoomInfo.m_sPass);
			pRoom->m_nQuestCode			= pBody->m_cRoomInfo.m_nQuestCode;
			pRoom->m_nGroundCode		= pBody->m_cRoomInfo.m_nGroundCode;
			pRoom->m_nBallCode			= pBody->m_cRoomInfo.m_nBallCode;
			pRoom->m_nTimeCode			= pBody->m_cRoomInfo.m_nTimeCode;
			pRoom->m_nWeatherCode		= pBody->m_cRoomInfo.m_nWeatherCode;
			pRoom->m_nAttackCode		= pBody->m_cRoomInfo.m_nAttackCode;
			pRoom->m_nScaleCode			= pBody->m_cRoomInfo.m_nScaleCode;
			pRoom->m_nAICode			= pBody->m_cRoomInfo.m_nAICode;
			pRoom->m_nPointCode			= pBody->m_cRoomInfo.m_nPointCode;
			pRoom->m_nStartLevel		= pBody->m_cRoomInfo.m_nStartLevel;
			pRoom->m_nEndLevel			= pBody->m_cRoomInfo.m_nEndLevel;
			pRoom->m_nAttackTeam		= pBody->m_cRoomInfo.m_nAttackTeam;
			pRoom->m_nMaxCount			= pBody->m_cRoomInfo.m_nMaxCount;

			pRoom->m_nCheckClub			= pBody->m_cRoomInfo.m_nCheckClub;
			pRoom->m_nCheckTime			= pBody->m_cRoomInfo.m_nCheckTime;
			pRoom->m_nCheckWeather		= pBody->m_cRoomInfo.m_nCheckWeather;
			pRoom->m_nCheckView			= pBody->m_cRoomInfo.m_nCheckView;
			pRoom->m_nCheckViewChat		= pBody->m_cRoomInfo.m_nCheckViewChat;
			pRoom->m_cHomeSeat			= pBody->m_cRoomInfo.m_cHomeSeat;
			pRoom->m_cAwaySeat			= pBody->m_cRoomInfo.m_cAwaySeat;
			pRoom->m_cViewSeat			= pBody->m_cRoomInfo.m_cViewSeat;

			if( pBody->m_cRoomInfo.m_nMode == ROOM_MODE_LADDER )
				PutCardbotInfo(pPlayer->GetObjSeq());

			if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM))
			{
				CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
				CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
				if(!pCurrentProcess) return;
				_Assert(pCurrentProcess, "GetSetRoom", 0, "", 0);
				pCurrentProcess->Set_ReadyRoomUI();
				pCurrentProcess->SetUIReadyRoomPositionSetting();
			}
			//else
			//	CPacketManager::GetPtr()->m_bIsRoomInfoDone = true;
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetRoomList(CHeadPacket* pPacket)
{
	CSCRoomList*	pBody		= (CSCRoomList*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			int cnt=0;
			for(int i=0;i<LIST5_SIZE;i++)
			{
				if(pBody->m_cRoomData[i].m_nState != 0)
					cnt++;
			}
			printf("GetRoomList %d Loading Ok page : %d\n", cnt, pBody->m_nPage);
			//? 임시방편
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetRoomList", 0, "", 0);
			pCurrentProcess->SetRoomList(pBody);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetLobbyList(CHeadPacket* pPacket)
{
	CSCLobbyList*	pBody		= (CSCLobbyList*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();

			if(temp->IsCurrentProcess(PROCESS_CHOICEROOM))
			{
				CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(temp);
				if(!pCurrentProcess) return;
				_Assert(pCurrentProcess, "GetLobbyList", 0, "", 0);
				pCurrentProcess->SetLobbyList(pBody);
			}
			else if(temp->IsCurrentProcess(PROCESS_READYROOM))
			{
				CInvitationDialog::GetPtr()->LoadLobbyList(pBody);
				CInvitationDialog::GetPtr()->LobbyListUpdate();
			}
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void GetCreateRoom(CHeadPacket* pPacket)
{
	CSCCreateRoom*	pBody		= (CSCCreateRoom*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		//PutRoomList(LIST_KIND_ALL, 0);
		break;
	case -1: //생성 가능한 최대 방 개수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetSetRoom(CHeadPacket* pPacket)
{
	CSCSetRoom*		pBody		= (CSCSetRoom*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			/*
			strcpy(pRoom->m_sTitle	, pBody->m_sTitle);
			strcpy(pRoom->m_sPass	, pBody->m_sPass);
			
			pRoom->m_nState			= pBody->m_nState;
			pRoom->m_nMode			= pBody->m_nMode;
			pRoom->m_nAttackCode	= pBody->m_nAttackCode;
			pRoom->m_nScaleCode		= pBody->m_nScaleCode;
			pRoom->m_nAICode		= pBody->m_nAICode;
			pRoom->m_nPointCode		= pBody->m_nPointCode;
			pRoom->m_nStartLevel	= pBody->m_nStartLevel;
			pRoom->m_nEndLevel		= pBody->m_nEndLevel;
			pRoom->m_nCheckClub		= pBody->m_nCheckClub;
			pRoom->m_nCheckTime		= pBody->m_nCheckTime;
			pRoom->m_nCheckWeather	= pBody->m_nCheckWeather;
			pRoom->m_nCheckView		= pBody->m_nCheckView;
			pRoom->m_nCheckViewChat	= pBody->m_nCheckViewChat;
			pRoom->m_nMaxCount		= pBody->m_nMaxCount;			
			
			if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM))
			{
				CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
				_Assert(pCurrentProcess, "GetSetRoom", 0, "", 0);
				pCurrentProcess->Set_ReadyRoomUI();
			}*/
		}
		break;
	case -1:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //현재 인원수보다 적게 설정할 수 없습니다.
	case -12: //레벨 설정이 잘못되었습니다.
	case -13: //현재 인원의 레벨보다 낮게 설정할 수 없습니다.
	case -14: //현재 인원의 레벨보다 높게 설정할 수 없습니다.
	case -15: //플레이어중 포인트가 부족한 사람이 있습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChoiceRoom(CHeadPacket* pPacket)
{
	CSCChoiceRoom*		pBody		= (CSCChoiceRoom*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CPacketManager::GetPtr()->m_bIsChoiceRoomDone = true;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -11: //해당 방이 존재하지 않습니다.
	case -12: //게임이 진행중이므로 진입할 수 없습니다.
	case -13: //방 비밀번호가 맞지 않습니다.
	case -14: //포지션이 맞지 않아 진입할 수 없습니다.
	case -15: //최대 허용 인원수를 초과하였습니다.
	case -16: //해당자가 이미 방안에 존재합니다.
	case -18: //내기방에 입장할 수 있는 포인트가 부족합니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void GetQuickRoom(CHeadPacket* pPacket)
{
	CSCQuickRoom*		pBody		= (CSCQuickRoom*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CPacketManager::GetPtr()->m_bIsChoiceRoomDone = true;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -11: //진입할 수 있는 방이 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void GetLeaveRoom(CHeadPacket* pPacket)
{
	CSCLeaveRoom*		pBody		= (CSCLeaveRoom*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjPlayerInfo*		pLeaver;
	int					nLeaveSeq;
	char				sName[PLAYER_NAME_SIZE];
	char				sMessage[MESSAGE_SIZE];

	pLeaver = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBody->m_nLeavePlayerSeq);
	if(pLeaver == NULL)		return;

	nLeaveSeq = pLeaver->GetObjSeq();
	_snprintf(sName, PLAYER_NAME_SIZE, pLeaver->m_sName);

	//////////////////////////////////////////////////////////////////////////
	if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM))
	{
		//초기화
		pRoom->m_nCource		= ROOM_COURCE_NONE;
		pRoom->m_nCountDown		= 0;
		pRoom->m_nCountTime		= 0;
		CPacketManager::GetPtr()->m_bIsGameCountDone = false;
		CPacketManager::GetPtr()->m_bIsGameReadyDone = false;

		if(pBody->m_nResponse > 0) //정상적인 플레이어 삭제
		{
			if(pRoom->m_nMode == ROOM_MODE_LADDER)
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteTeamReserveSeat(pLeaver->m_nTeam);
				CObjPlayerManager::GetPtr()->DeleteTeamPlayerInList(PLAYER_TEAM_TOTAL, pLeaver->m_nTeam); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}
			else
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteReserveSeat(pLeaver->m_nTeam, pLeaver->m_nSeat); //예약 슬롯에서 삭제
				CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, pLeaver); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}

			//? 임시방편적 코드 반드시 드러내야된다
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetLeaveRoom", 0, "", 0);
			pCurrentProcess->SetUIReadyRoomAthleteSetting();
			pCurrentProcess->Set_ReadyRoomUI();
		}

		switch(pBody->m_nResponse)
		{
		case 1: //방에서 나감
			if(pPlayer->GetObjSeq() == nLeaveSeq) //본인이 나감
			{
				CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceRoomProcess);
			}
			else
			{
				//%s님이 방에서 나가셨습니다.
				CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20102, 0));
				assert(pTable);
				_snprintf(sMessage, MESSAGE_SIZE, pTable->m_sMessage, sName);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_ANNOUNCE);
			}
			break;
		case 2: //강퇴 당함
			if(pPlayer->GetObjSeq() == nLeaveSeq ) //본인이 강퇴당함
			{
				CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);

				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceRoomProcess);
			}
			else
			{
				//%s님이 강제 퇴장 되셨습니다.
				CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20103, 0));
				assert(pTable);
				_snprintf(sMessage, MESSAGE_SIZE, pTable->m_sMessage, sName);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_ANNOUNCE);
			}
			break;
		case 3: //접속이 끊김
			break;
		case -11: //시스템에 문제가 발생하였습니다.
		case -12:
		case -13:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
			break;
		default:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMEPLAY))
	{
		pRoom->m_nCource		= ROOM_COURCE_NONE;
		pRoom->m_nCountDown		= 0;
		pRoom->m_nCountTime		= 0;
		CPacketManager::GetPtr()->m_bIsGameCountDone = false;
		CPacketManager::GetPtr()->m_bIsGameReadyDone = false;

		if(pBody->m_nResponse > 0) //정상적인 플레이어 삭제
		{
			int nTeam = pLeaver->m_nTeam;

			if(pRoom->m_nMode == ROOM_MODE_LADDER)
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteTeamReserveSeat(pLeaver->m_nTeam);
				CObjPlayerManager::GetPtr()->DeleteTeamPlayerInList(PLAYER_TEAM_TOTAL, pLeaver->m_nTeam); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}
			else
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteReserveSeat(pLeaver->m_nTeam, pLeaver->m_nSeat); //예약 슬롯에서 삭제
				CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, pLeaver); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}

			//프로세서 이동 처리
			//관전자가 나갔을 경우 계속 경기 속행, 선수가 나갔을 경우 경기 중단
			if(nTeam == PLAYER_TEAM_VIEW)
			{
				//ChoiceRoom 프로세서로 이동
			}
			else
			{
				CPacketManager::GetPtr()->m_bIsLeaveRoom = true;
			}
		}

		//pLeaver->SetKind(PLAYER_KIND_ROBOT);//나간 케릭터 AI변환

		switch(pBody->m_nResponse)
		{
		case 1: //방에서 나감
			if(pPlayer->GetObjSeq() == nLeaveSeq) //본인이 나감
			{
				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceRoomProcess);

				CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);
			}
			else
			{
				//%s님이 방에서 나가셨습니다.
				CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20102, 0));
				assert(pTable);
				_snprintf(sMessage, MESSAGE_SIZE, pTable->m_sMessage, sName);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_ANNOUNCE);
			}
			break;
		case 2: //강퇴 당함
		case 3: //접속이 끊김
			break;
		case -11: //시스템에 문제가 발생하였습니다.
		case -12:
		case -13:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
			break;
		default:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMERESULT))
	{
		//초기화
		pRoom->m_nCource		= ROOM_COURCE_NONE;
		pRoom->m_nCountDown		= 0;
		pRoom->m_nCountTime		= 0;
		CPacketManager::GetPtr()->m_bIsGameCountDone = false;
		CPacketManager::GetPtr()->m_bIsGameReadyDone = false;

		//결과화면에서는 이미 모든 선수리스트가 지워진 상태이므로 더이상 수행할 것이 없다
		if(pBody->m_nResponse > 0) //정상적인 플레이어 삭제
		{
			int nTeam = pLeaver->m_nTeam;

			if(pRoom->m_nMode == ROOM_MODE_LADDER)
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteTeamReserveSeat(pLeaver->m_nTeam);
				CObjPlayerManager::GetPtr()->DeleteTeamPlayerInList(PLAYER_TEAM_TOTAL, pLeaver->m_nTeam); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}
			else
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteReserveSeat(pLeaver->m_nTeam, pLeaver->m_nSeat); //예약 슬롯에서 삭제
				CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, pLeaver); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}

			if(nTeam == PLAYER_TEAM_VIEW)
			{
				//ChoiceRoom 프로세서로 이동
			}
			else if(pPlayer->GetObjSeq() == nLeaveSeq) //본인이 나감
			{
				CObjPlayerManager::GetPtr()->DeleteAllPlayerInList(PLAYER_TEAM_TOTAL);
				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceRoomProcess);
			}
			else
			{
				CPacketManager::GetPtr()->m_bIsLeaveRoom = true;
			}
		}

		switch(pBody->m_nResponse)
		{
		case 1: //방에서 나감
		case 2: //강퇴 당함
		case 3: //접속이 끊김
			break;
		case -11: //시스템에 문제가 발생하였습니다.
		case -12:
		case -13:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
			break;
		default:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_GAMELOADING))
	{
		//초기화
		pRoom->m_nCource		= ROOM_COURCE_NONE;
		pRoom->m_nCountDown		= 0;
		pRoom->m_nCountTime		= 0;
		CPacketManager::GetPtr()->m_bIsGameCountDone = false;
		CPacketManager::GetPtr()->m_bIsGameReadyDone = false;

		if(pBody->m_nResponse > 0) //정상적인 플레이어 삭제
		{
			if(pRoom->m_nMode == ROOM_MODE_LADDER)
			{
				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteTeamReserveSeat(pLeaver->m_nTeam);
				CObjPlayerManager::GetPtr()->DeleteTeamPlayerInList(PLAYER_TEAM_TOTAL, pLeaver->m_nTeam); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}
			else
			{

				CObjRoomManager::GetPtr()->GetRoomPointer()->DeleteReserveSeat(pLeaver->m_nTeam, pLeaver->m_nSeat); //예약 슬롯에서 삭제
				//의문 로딩중에 나가면 왜 나간 캐릭터 리스트삭제를 안하나? 검증해봐야될듯
				//CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, pLeaver); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			}

			printf("로딩중 한명나감 삭제처리됨\n");

			if(pLeaver->m_nTeam == PLAYER_TEAM_VIEW)
			{
				//ChoiceRoom 프로세서로 이동
			}
			else
			{
				CBaseLoadingProcess* pCurrentProcess = dynamic_cast<CBaseLoadingProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				pCurrentProcess->SetLeaveRoomPlayerPoint(pLeaver);

				CPacketManager::GetPtr()->m_bIsLeaveRoom = true;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeParent(CHeadPacket* pPacket)
{
	CSCChangeParent*	pBody		= (CSCChangeParent*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pAnyPlayer;

	switch(pBody->m_nResponse)
	{
	case 0:
		pRoom->m_nParentSeq = pBody->m_nParentSeq;
		memcpy(&pRoom->m_cParentAddress, &pBody->m_cParentAddress, sizeof(CAddress));

		pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pRoom->m_nParentSeq);
		printf("@@@@@@@@@@ Choice Parent(%d: %s) @@@@@@@@@@\n", pAnyPlayer->GetObjSeq(), pAnyPlayer->m_sName);
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeJang(CHeadPacket* pPacket)
{
	CSCChangeJang*		pBody		= (CSCChangeJang*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pRoom->m_nRoomJangTeam	= pBody->m_nRoomJangTeam;
			pRoom->m_nHomeJangSeq	= pBody->m_nHomeJangSeq;
			pRoom->m_nAwayJangSeq	= pBody->m_nAwayJangSeq;

			// 드러내야 하는 코드라고 함 (CYG)
			if(CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM))
			{
				CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
				CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
				pCurrentProcess->SetUIReadyRoomAthleteSetting();
				pCurrentProcess->Set_ReadyRoomUI();
			}
		}

		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetAthleteInfo(CHeadPacket* pPacket)
{
	CSCAthleteInfo*		pBody		= (CSCAthleteInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= NULL;
	CAthleteInfo*		pInfo		= &(pBody->m_cAthleteInfo);
	VectorAnyoneList*	pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);
	CObjPlayerInfo*		pMyPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	int					i;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			/*for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
			{
				for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
				{
					if((*iPlayer)->GetObjSeq() == pInfo->m_nPlayerSeq)
						return;
				}
			}*/

			//이미 리스트에 등재된 플레이어라면 넘어간다(같은 데이터를 또 받은 경우)
			/*if(CObjPlayerManager::GetPtr()->IsExistPlayer(pInfo->m_nPlayerSeq))
				return;*/

			bool bChange = false;
			
			pPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pInfo->m_nPlayerSeq);

			if( pPlayer )
			{
				for( int i=0;i<MAX_EQUIP;i++ )
				{
					if( pPlayer->m_nEquipWear[i] != pInfo->m_nEquipWear[i] )
					{
						bChange = true;
						break;
					}
				}
			}

			if( bChange )
				CObjPlayerManager::GetPtr()->DeletePlayerInList(PLAYER_TEAM_TOTAL, pPlayer); //나간 캐릭터 리스트 삭제+인스턴스 삭제
			
			pPlayer = new CObjPlayerInfo;
			if(pPlayer == NULL)		return;
			
			pPlayer->InitPlayerInRoom(PLAYER_KIND_USER);

			pPlayer->SetObjSeq( pInfo->m_nPlayerSeq);

			pPlayer->m_nPosition			= pInfo->m_nPosition;
			pPlayer->m_nTeam				= pInfo->m_nTeam;
			pPlayer->m_nSeat				= pInfo->m_nSeat;
			pPlayer->m_cLevel				= pInfo->m_cLevel;
			pPlayer->m_cShape				= pInfo->m_cShape;
			pPlayer->m_cUDPAddress			= pInfo->m_cAddress;
			pPlayer->m_cBaseFaculty			= pInfo->m_cBaseFaculty;
			pPlayer->m_cRaiseFaculty		= pInfo->m_cRaiseFaculty;
			pPlayer->m_cTrainingFaculty		= pInfo->m_cTrainingFaculty;
			pPlayer->m_cItemOption			= pInfo->m_cItemOption;
			_snprintf(pPlayer->m_sMent, PLAYER_MENT_SIZE, "%s",		pInfo->m_sMent );

			for(i=0;i<MAX_SKILL;++i)
				pPlayer->CreateBagSkill(&pInfo->m_cSkillInfo[i]);

			for(i=0;i<MAX_CEREMONY;++i)
				pPlayer->CreateBagCeremony(&pInfo->m_cCeremonyInfo[i]);

			memcpy(&pPlayer->m_nEquipWear,	&pInfo->m_nEquipWear,		sizeof(int)*MAX_EQUIP);
			memcpy(&pPlayer->m_nHomeWear,	&pInfo->m_nHomeWear,		sizeof(int)*4);
			memcpy(&pPlayer->m_nAwayWear,	&pInfo->m_nAwayWear,		sizeof(int)*4);

			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, pInfo->m_sName);
			_snprintf(pPlayer->m_sMent, PLAYER_MENT_SIZE, pInfo->m_sMent);

			if( bChange )
				CObjPlayerManager::GetPtr()->SetPlayerLists(pPlayer, pPlayer->m_nTeam);	
			else
			{
				CObjPlayerManager::GetPtr()->SetPlayerLists(pPlayer, pPlayer->m_nTeam);
				CObjRoomManager::GetPtr()->GetRoomPointer()->SetReserveSeat(pPlayer->m_nTeam, pPlayer->m_nSeat, pPlayer->m_nPosition, pPlayer->GetObjSeq());
			}
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetAthleteEnd(CHeadPacket* pPacket)
{
	CSCAthleteEnd*		pBody		= (CSCAthleteEnd*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CPacketManager::GetPtr()->m_bIsAthleteInfoDone = true;
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetRobotInfo(CHeadPacket* pPacket)
{
	//test 나중에 삭제 요망
	CObjRoomInfo *pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	CSCRobotInfo*		pBody		= (CSCRobotInfo*)pPacket;

	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= NULL;
	CRobotInfo*			pInfo		= &(pBody->m_cRobotInfo);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pPlayer = new CObjPlayerInfo;
			if(pPlayer == NULL)		return;

			pPlayer->InitPlayerInRoom(PLAYER_KIND_ROBOT);

			pPlayer->SetObjSeq(pInfo->m_nRobotSeq);
			pPlayer->m_nTeam						= pInfo->m_nTeam;
			pPlayer->m_nSeat						= pInfo->m_nSeat;
			pPlayer->m_cLevel.m_nLevel				= (50 < pInfo->m_nLevel) ? 50 : pInfo->m_nLevel;
			pPlayer->m_cShape.m_nUniform			= PLAYER_UNIFORM_NONE;
			pPlayer->m_nPosition					= pInfo->m_nPosition;

			pPlayer->m_cRobot.m_nRank				= 0;
			pPlayer->m_cRobot.m_nSkill				= 3;
			pPlayer->m_cRobot.m_nType				= 0;
			pPlayer->m_cRobot.m_nCostume			= pInfo->m_nCostume;

			pPlayer->m_nLoadingStep					= 100; //인공지능은 로딩이 무조건 다 된것으로 간주

			CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + pPlayer->m_nPosition*100 + pPlayer->m_cLevel.m_nLevel, 0));
			assert(pTempAbility);
			CResTempCostumeTableFormat* pTempCostume = dynamic_cast<CResTempCostumeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPCOSTUMETABLE, pPlayer->m_cRobot.m_nCostume, 0));
			assert(pTempCostume);

			CFaculty cFaculty;
			pTempAbility->SetValue(pPlayer->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, pPlayer->m_cTrainingFaculty.m_nFaculty, 0);

			pTempCostume->SetValue(pPlayer->m_nEquipWear);

			CObjStack* pStack = CObjectManager::GetPtr()->GetObjStack(static_cast<OBJECT_TYPE>(pPlayer->m_nPosition*10));
			if(pStack) pPlayer->SetSkillStack(pStack, pPlayer->m_cRobot.m_nSkill);

			pPlayer->m_cShape.m_nGender = (pPlayer->m_nEquipWear[0]%100/10);

	//		GetPositionString(pPlayer->m_nPosition, pPlayer->m_sName);//AI들의 이름

			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, "COMPUTER");
			//_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, "N%d L%d F%d", pPlayer->GetObjSeq()%1000, pPlayer->m_cLevel.m_nLevel, pPlayer->m_nForm);

			CObjPlayerManager::GetPtr()->SetPlayerLists(pPlayer, pPlayer->m_nTeam);
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetRobotEnd(CHeadPacket* pPacket)
{
	CSCRobotEnd*		pBody		= (CSCRobotEnd*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CPacketManager::GetPtr()->m_bIsRobotInfoDone = true;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeGround(CHeadPacket* pPacket)
{
	CSCChangeGround*	pBody		= (CSCChangeGround*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(!pCurrentProcess) return;
	_Assert(pCurrentProcess, "GetChangeGround", 0, "", 0);

	switch(pBody->m_nResponse)
	{
	case 0:
		pRoom->m_nGroundCode	= pBody->m_nGroundCode;
		pCurrentProcess->FUNCTION_UpdateMapChange();
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeBall(CHeadPacket* pPacket)
{
	CSCChangeBall*		pBody		= (CSCChangeBall*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(!pCurrentProcess) return;
	_Assert(pCurrentProcess, "GetChangeBall", 0, "", 0);

	switch(pBody->m_nResponse)
	{
	case 0:
		pRoom->m_nBallCode	= pBody->m_nBallCode;
		pCurrentProcess->FUNCTION_UpdateBallChange();
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetForceOut(CHeadPacket* pPacket)
{
	CSCForceOut*		pBody		= (CSCForceOut*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
	if(!pCurrentProcess) return;
	_Assert(pCurrentProcess, "GetForceOut", 0, "", 0);

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -3: //방장만 강제 퇴장시킬 수 있습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -4: //운영자는 퇴장 시키지 못함(그냥 아무일도 일어나지 않는다)
	case -5: //이미 존재하지 않는 경우 자동 퇴장 되도록 기다린다.
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetInvitePlayer(CHeadPacket* pPacket)
{
	CSCInvitePlayer*	pBody		= (CSCInvitePlayer*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	
	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CResMessageTableFormat* pTable = NULL;
			char sText[MAX_PATH]="";
			if(strlen(pBody->m_sMessage) == 0)
			{
				pTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900001 ,0));
				assert(pTable);
				_snprintf(sText, MAX_PATH, pTable->m_sMessage, pBody->m_sFromName, pBody->m_nRoomSeq);
			}
			else
			{			
				pTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900002 ,0));
				assert(pTable);
				_snprintf(sText, MAX_PATH, pTable->m_sMessage, pBody->m_sFromName, pBody->m_nRoomSeq, pBody->m_sMessage);
			}
			
			CChattingDialog::GetPtr()->SetChatMessage(sText, true, COLOR_SECRET);
			NormalSound(60001);
		}
		break;
	case -1: //해당 사용자가 존재하지 않습니다.
	case -2: //상대방이 초대거부 상태입니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameReady(CHeadPacket* pPacket)
{
	CSCGameReady*		pBody		= (CSCGameReady*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		switch(pBody->m_nReady)
		{
		case PLAYER_READY_QUESTION:
			{
				pRoom->m_nCource = ROOM_COURCE_READY;

				//네트워크를 설정하고 있습니다.
				CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20100, 0));
				assert(pTable);
				CChattingDialog::GetPtr()->SetChatMessage(pTable->m_sMessage, true, COLOR_ANNOUNCE);

				if(CPacketManager::GetPtr()->m_bIsAthleteInfoDone) //선수정보가 새로 도착하여 모델을 만들 것들이 남아있음
					PutGameReady(PLAYER_READY_NO);
				else //모든 모델들이 다 만들어져서 대기하고 있음
					PutGameReady(PLAYER_READY_YES);

				pRoom->m_nCountTime = timeGetTime();
				CPacketManager::GetPtr()->m_bIsGameReadyDone = true;

				if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
				{
					CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
					if(!pCurrentProcess) return;
					
					if( pRoom->m_nMode == ROOM_MODE_NORMAL )
						pCurrentProcess->FUNCTION_TeamChangeVisible(N3UI_VISIBLE_FALSE);
				}
			}
			break;
		case PLAYER_READY_REQUESTION:
			if(CPacketManager::GetPtr()->m_bIsAthleteInfoDone) //선수정보가 새로 도착하여 모델을 만들 것들이 남아있음
				PutGameReady(PLAYER_READY_NO);
			else //모든 모델들이 다 만들어져서 대기하고 있음
				PutGameReady(PLAYER_READY_YES);
			break;
		case PLAYER_READY_COMPLETE:
			if(pRoom->IsMeRoomJang())
				PutGameStart();

			CPacketManager::GetPtr()->m_bIsGameReadyDone = false;
			break;
		case PLAYER_READY_CANCEL:
			{
				char				sMessage[MESSAGE_SIZE];
				CObjPlayerInfo*		pCancelPlayer;

				pRoom->m_nCource = ROOM_COURCE_NONE;

				if( pBody->m_nCancelTeam != PLAYER_TEAM_NONE )
				{
					//플레이를 취소시킨 사용자 가져오기
					pCancelPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pRoom->GetTeamJang(pBody->m_nCancelTeam));
					//%s님에 의해 플레이가 취소되었습니다.
					CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20101, 0));
					assert(pTable);
					_snprintf(sMessage, MESSAGE_SIZE, pTable->m_sMessage, pCancelPlayer->m_sName);
				} else
				{
					CResMessageTableFormat *pTable = dynamic_cast<CResMessageTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 20104, 0));
					_snprintf(sMessage, MESSAGE_SIZE, "%s", pTable->m_sMessage );
				}
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_ANNOUNCE);

				pRoom->m_nCountDown		= 0;
				pRoom->m_nCountTime		= 0;
				CPacketManager::GetPtr()->m_bIsGameCountDone = false;
				CPacketManager::GetPtr()->m_bIsGameReadyDone = false;

				if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
				{
					CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
					if(!pCurrentProcess) return;
					
					UIFunction::Set_UIVisibility( "MenuTabButton06", N3UI_VISIBLE_TRUE );
					UIFunction::Set_UIVisibility( "MenuTabButton16", N3UI_VISIBLE_FALSE );

					if( pRoom->m_nMode == ROOM_MODE_NORMAL )
						pCurrentProcess->FUNCTION_TeamChangeVisible(N3UI_VISIBLE_TRUE);
				}
			}
			break;
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -2: //방장만 플레이를 시작할 수 있습니다.
	case -3: //방장과 주장만 플레이를 취소할 수 있습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameStart(CHeadPacket* pPacket)
{
	CSCGameStart*		pBody		= (CSCGameStart*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	VectorAnyoneList* pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pRoom->m_nCource		= ROOM_COURCE_READY;

			pRoom->m_nParentSeq   	= pBody->m_nParentSeq;
			pRoom->m_nWeatherCode 	= pBody->m_nWeather;
			pRoom->m_nTimeCode    	= pBody->m_nRoomTimeType;
			pRoom->m_nRandom    	= pBody->m_nRandom;

			int nBGMCount = CResourceManager::GetPtr()->GetResourceSize(RESOURCE_OGGTABLE);

			CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			CGameProject::s_pGameProject->s_nBGM = (char)(pRoom->m_nRandom%100)%nBGMCount;

			pRoom->m_cParentAddress	= pBody->m_cParentAddress;
			pRoom->m_nAttackTeam	= pBody->m_nAttackTeam;
			pRoom->m_cMission		= pBody->m_cMission;

			int nCount = 0;

			if(pRoom->IsMeRoomJang()) //내가 방장이라면
			{
				VectorAnyoneList::iterator it;
				VectorPlayerList::iterator itp;
				for(it = pAnyoneList->begin();it != pAnyoneList->end();it++)
				{
					for(itp = (*it)->begin();itp != (*it)->end();itp++)
					{
						if( (*itp)->m_bPit == ROOM_PIT_IN )
							nCount++;
					}
				}
				
				if( nCount )
					pRoom->m_nCountDown = 15;
				else
					pRoom->m_nCountDown = 5;

				pRoom->m_nCountTime = timeGetTime();
				PutGameCount(pRoom->m_nCountDown);
			}
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
	case -12:
	case -13:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14://포인트가 부족한 플레이어가 있어서 게임을 시작하지 못했습니다.
	case -15://인원이 맞지 않습니다.
		{
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetChangeTeam", 0, "", 0);
			pCurrentProcess->FUNCTION_TeamChangeVisible(N3UI_VISIBLE_TRUE);
		}
	case -16://관전자는 내기방에 입장할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameCount(CHeadPacket* pPacket)
{
	CSCGameCount*		pBody		= (CSCGameCount*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		if(pBody->m_nCount == 5 || pBody->m_nCount == 15 )//초기화는 5일때만 한다
		{
			pRoom->m_nCource = ROOM_COURCE_COUNT;
			CPacketManager::GetPtr()->m_bIsGameCountDone = true;
		}

		pRoom->m_nCountDown = pBody->m_nCount;

		if(pBody->m_nCount > 0) //카운트다운 중
		{
			//카운트다운을 표시한다.
			char				sMessage[MESSAGE_SIZE];

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
				CInvenDialog::GetPtr()->FUNCTION_UpdateSecond(pBody->m_nCount-5);
			
			_snprintf(sMessage, MESSAGE_SIZE, "Count Down.....%d", pBody->m_nCount);
			CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_NOTICE);
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameLoad(CHeadPacket* pPacket)
{
	CSCGameLoad*		pBody		= (CSCGameLoad*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pAnyPlayer	= NULL;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pRoom->m_nCource = ROOM_COURCE_LOAD;

			pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBody->m_nPlayerSeq);

			pAnyPlayer->m_nLoadingStep = pBody->m_nStep;

			//printf("GameLoad(sName)=%s (ObjSeq)=%d (LoadingStep)=%d\n", pAnyPlayer->m_sName, pAnyPlayer->GetObjSeq(), pAnyPlayer->m_nLoadingStep);
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGamePlay(CHeadPacket* pPacket)
{
	CSCGamePlay*		pBody		= (CSCGamePlay*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		pRoom->m_nCource = ROOM_COURCE_PLAY;

		CPacketManager::GetPtr()->m_bIsGamePlayDone = true;
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameResult(CHeadPacket* pPacket)
{
	CSCGameResult*	pBody		= (CSCGameResult*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		pRoom->m_nCource = ROOM_COURCE_RESULT;

		pRoom->m_cMvpPlayer.m_nPlayerSeq	= pBody->m_nMvpSeq;
		pRoom->m_cMvpPlayer.m_nLevel		= pBody->m_nMvpLevel;
		pRoom->m_cMvpPlayer.m_nPosition		= pBody->m_nMvpPosition;
		_snprintf(pRoom->m_cMvpPlayer.m_sName, PLAYER_NAME_SIZE, pBody->m_sMvpName);
		pRoom->m_fCurrentTime				= pBody->m_fCurrentTime;
		pRoom->m_cHomeResult				= pBody->m_cHomeResult;
		pRoom->m_cAwayResult				= pBody->m_cAwayResult;
		memcpy(&pRoom->m_cEachResult, &pBody->m_cEachResult, sizeof(CEachResult)*TEAM_SIZE*2);

		CProcessManager::GetPtr()->SetChangeProcess(new CGameResultProcess);

		//리플레이 헤더 저장용
		CPacketManager::GetPtr()->m_cReplayQue.m_cRoomInfo.m_nHomeScore		= (char)pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL];
		CPacketManager::GetPtr()->m_cReplayQue.m_cRoomInfo.m_nAwayScore		= (char)pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL];

		for(int i=0;i<(TEAM_SIZE*2);++i)
		{
			for(int j=0;j<(TEAM_SIZE*2);++j)
			{
				if(pRoom->m_cEachResult[i].m_nPlayerSeq == CPacketManager::GetPtr()->m_cReplayQue.m_cAthleteInfo[j].m_nPlayerSeq)
				{
					CPacketManager::GetPtr()->m_cReplayQue.m_cAthleteInfo[j].m_nGoal	= pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_GOAL];
					CPacketManager::GetPtr()->m_cReplayQue.m_cAthleteInfo[j].m_nAssist	= pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_ASSIST];
					CPacketManager::GetPtr()->m_cReplayQue.m_cAthleteInfo[j].m_nMark	= pRoom->m_cEachResult[i].m_cPlayerResult.m_nResult[ARRAY_RESULT_MARK];
				}
			}
		}

		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGameEnd(CHeadPacket* pPacket)
{
	CSCGameEnd*			pBody		= (CSCGameEnd*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		CChattingDialog::GetPtr()->ChattingListClear();
		CChattingDialog::GetPtr()->ChattingListDelete();

		pRoom->m_nCource = ROOM_COURCE_NONE;

		CPacketManager::GetPtr()->m_bIsGameEndDone = true;
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetLevelUp(CHeadPacket* pPacket)
{
	CSCLevelUp*			pBody		= (CSCLevelUp*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pAnyPlayer;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if(pBody->m_nPlayerSeq == pPlayer->GetObjSeq()) //본인이 레벨업 됐다
			{
				pMember->m_cMoney		= pBody->m_cMoney;
				pPlayer->m_cLevel		= pBody->m_cLevel;
				pPlayer->m_cBaseFaculty	= pBody->m_cBaseFaculty;
			}

			//if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_CHOICEROOM) )
			//{
			//	CChoiceRoomProcess* pCurrentProcess = dynamic_cast<CChoiceRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			//	assert(pCurrentProcess);
			//	pCurrentProcess->FUNCTION_IsChoiceClass();
			//}

			pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBody->m_nPlayerSeq);
			if(pAnyPlayer == NULL)		return;

			//해당 선수에 갱신해 놓는다
			pAnyPlayer->m_cLevel		= pBody->m_cLevel;
			pAnyPlayer->m_cBaseFaculty	= pBody->m_cBaseFaculty;
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeTeam(CHeadPacket* pPacket)
{
	CSCChangeTeam*		pBody		= (CSCChangeTeam*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pAnyPlayer;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBody->m_nPlayerSeq);
			if(pAnyPlayer == NULL)		return;

			//해당 캐릭터를 이전 팀에서 빼낸다
			CObjPlayerManager::GetPtr()->ErasePlayerInList(PLAYER_TEAM_TOTAL, pAnyPlayer);

			pAnyPlayer->m_nTeam	= pBody->m_nToTeam;
			pAnyPlayer->m_nSeat	= pBody->m_nSeat;
			pRoom->m_cHomeSeat	= pBody->m_cHomeSeat;
			pRoom->m_cAwaySeat	= pBody->m_cAwaySeat;
			pRoom->m_cViewSeat	= pBody->m_cViewSeat;

			//해당 캐릭터를 변경된 팀에 넣는다
			CObjPlayerManager::GetPtr()->SetPlayerLists(pAnyPlayer, pAnyPlayer->m_nTeam);

			//? 임시방편적 코드 반드시 드러내야된다
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetChangeTeam", 0, "", 0);
			CPlayModeManager::GetPtr()->SetupCharacterPoint(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nSeat);
			pCurrentProcess->SetUIReadyRoomAthleteSetting();
			pCurrentProcess->Set_ReadyRoomUI();
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -2: //같은 자리로 변경할 수 없습니다.
	case -11: //변경할 수 있는 자리가 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangePosition(CHeadPacket* pPacket)
{
	CSCChangePosition*	pBody		= (CSCChangePosition*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pRoom->m_cHomeSeat	= pBody->m_cHomeSeat;
			pRoom->m_cAwaySeat	= pBody->m_cAwaySeat;

			//? 임시방편
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetChangePosition", 0, "", 0);
			pCurrentProcess->SetUIReadyRoomPositionSetting();
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -2: //주장만 포지션을 변경할 수 있습니다.
	case -11: //공격수 포지션 설정이 잘못되었습니다.
	case -12: //미드필더 포지션 설정이 잘못되었습니다.
	case -13: //수비수 포지션 설정이 잘못되었습니다.
	case -14: //골키퍼 포지션 설정이 잘못되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeMent(CHeadPacket* pPacket)
{
	CSCChangeMent*		pBody		= (CSCChangeMent*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			_snprintf(pPlayer->m_sMent, PLAYER_MENT_SIZE, pBody->m_sMent);
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetGrowupCharacter(CHeadPacket* pPacket)
{
	CSCGrowupCharacter*			pBody			= (CSCGrowupCharacter*)pPacket;
	int							nMsgCode		= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*				pPlayer			= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pPlayer->m_cLevel			= pBody->m_cPlayerInfo.m_cLevel;
			pPlayer->m_nPosition		= pBody->m_nPosition;
			pPlayer->m_cBaseFaculty		= pBody->m_cPlayerInfo.m_cBaseFaculty;
			pPlayer->m_cRaiseFaculty	= pBody->m_cPlayerInfo.m_cRaiseFaculty;
			pPlayer->m_cTrainingFaculty = pBody->m_cPlayerInfo.m_cTrainingFaculty;
			pPlayer->m_cQuarterRanking	= pBody->m_cPlayerInfo.m_cQuarterRanking;
			pPlayer->m_cQuarterRecord	= pBody->m_cPlayerInfo.m_cQuarterRecord;
			pPlayer->m_cTotalRanking	= pBody->m_cPlayerInfo.m_cTotalRanking;
			pPlayer->m_cTotalRecord		= pBody->m_cPlayerInfo.m_cTotalRecord;
			pPlayer->m_cItemOption		= pBody->m_cPlayerInfo.m_cItemOption;
			pPlayer->SetObjSeq(pBody->m_cPlayerInfo.m_nPlayerSeq);
			_snprintf(pPlayer->m_sMent, PLAYER_MENT_SIZE, "%s",	pBody->m_cPlayerInfo.m_sMent );
			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, "%s",	pBody->m_cPlayerInfo.m_sName );

			/*if( CPacketManager::GetPtr()->m_bIsCertifyToGame == false && CPacketManager::GetPtr()->m_bIsPlayerInfoDone == false )
			{
				CPacketManager::GetPtr()->m_bIsPlayerInfoDone = false;
				CPacketManager::GetPtr()->m_bIsCertifyToGame = false;
				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceServerProcess(true));
			}
			else*/
			{
				CPacketManager::GetPtr()->m_bIsPlayerInfoDone = false;
				CPacketManager::GetPtr()->m_bIsCertifyToGame = false;
				PutGameExit(PLAYER_EXIT_TRANSPORT);
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 퀘스트에 대한 보상을한다 (CYG)
void GetQuestReward(CHeadPacket* pPacket)
{
	CSCQuestReward*	pBody		= (CSCQuestReward*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void GetShopItemList(CHeadPacket* pPacket)
{
	CSCShopItemList*		pBody		= (CSCShopItemList*)pPacket;
	int						nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	//CResItemTableFormat*	pTable;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			////? 임시방편적 코드 반드시 드러내야된다
			//CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			//CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(temp);
			//if(!pCurrentProcess) return;
			//_Assert(pCurrentProcess, "GetShopItemList", 0, "", 0);

			///////히트상품 페이지표시 하드코딩, 정식 구현 후 삭제해야함////////////////
			//if(pCurrentProcess->m_nBigFractionIndex == 400 && pCurrentProcess->m_nSmallFractionIndex == 2 )
			//{
			//	pCurrentProcess->m_nCurrentPage=0;
			//	pCurrentProcess->m_nTotalPage=0;
			//}
			////////////////////////////////////////////////////////////////////////////
			//else
			//{
			//	pCurrentProcess->m_nCurrentPage = pBody->m_nCurrentPage;
			//	pCurrentProcess->m_nTotalPage = pBody->m_nTotalPage;
			//	pCurrentProcess->FUNCTION_UpdatePage(pBody->m_nCurrentPage);
			//}

			//for(int i = 0; i < LIST6_SIZE; i++)
			//{
			//	pCurrentProcess->m_CurrentItemList[i].nCode = pBody->m_cShopItemData[i].m_nCode;
			//	pTable = dynamic_cast<CResItemTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_ITEMTABLE, pCurrentProcess->m_CurrentItemList[i].nCode, 0));
			//	assert(pTable);
			//	if(pTable) pCurrentProcess->FUNCTION_ItemRender(i, pCurrentProcess->m_CurrentItemList[i].nCode, GetItemPoint(pCurrentProcess->m_CurrentItemList[i].nCode), GetItemCash(pCurrentProcess->m_CurrentItemList[i].nCode), pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
			//	else pCurrentProcess->FUNCTION_ItemRender(i, 0, 0, 0);
			//}
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetUpdateItem(CHeadPacket* pPacket)
{
	CSCUpdateItem*		pBody		= (CSCUpdateItem*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjItemInfo		cItem;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			cItem.m_nItemSeq	= pBody->m_nItemSeq;
			cItem.m_nCode		= pBody->m_nCode;
			cItem.m_nClass		= pBody->m_nClass;
			cItem.m_nEquipKind	= pBody->m_nEquipKind;
			cItem.m_nGrade		= pBody->m_nGrade;
			cItem.m_nLevel		= pBody->m_nLevel;
			cItem.m_nPrice		= pBody->m_nPrice;
			cItem.m_nAmount		= pBody->m_nAmount;
			memcpy(&cItem.m_nOptionCode, &pBody->m_nOptionCode, sizeof(int)*ITEM_OPTION_SIZE);

			switch(pBody->m_nUpdateKind)
			{
			case UPDATE_KIND_CREATE:
				{
					pPlayer->CreateBagItem(&cItem);

					if(cItem.m_nEquipKind == EQUIP_KIND_YES)
					{
						pPlayer->EquipBagItem(cItem.m_nItemSeq);

						if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMSHOP) )
						{
							CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
							assert(pCurrentProcess);

							short nWear = pBody->m_nCode/1000000;

							short WearToType[MAX_EQUIP] = 
							{
								ITEM_TYPE_FACE,		ITEM_TYPE_HAIR,		ITEM_TYPE_FACEPAINT,
								ITEM_TYPE_SHIRTS,	ITEM_TYPE_PANTS,	ITEM_TYPE_GLOVE,	ITEM_TYPE_SHOES,	ITEM_TYPE_SOCKS,
								ITEM_TYPE_EYE,		ITEM_TYPE_EAR,		ITEM_TYPE_NECK,		ITEM_TYPE_MASK,		ITEM_TYPE_MUFFLER,		ITEM_TYPE_BAG,
								ITEM_TYPE_WRIST,	ITEM_TYPE_ARM,		ITEM_TYPE_KNEE
							};

							for( int i=0;i<MAX_EQUIP;i++ )
							{
								if( WearToType[i] == nWear )
								{
									if( i == ITEM_EQUIP_FACE )
									{
										pPlayer->m_cShape.m_nSkin = pBody->m_nCode%100;
										pCurrentProcess->m_nSkin = pPlayer->m_cShape.m_nSkin;
									}

									pPlayer->m_nEquipWear[i] = pBody->m_nCode;
									pPlayer->m_pEquipWearModelID[i] = CObjectModelManager::GetPtr()->ExchangeItem( pPlayer->m_nEquipWear[i], pPlayer->m_cShape.m_nSkin, pPlayer->m_pEquipWearModelID[i], pPlayer->m_pModel );
									pCurrentProcess->FUNCTION_ExampleExchangeItem( pBody->m_nCode, i, true );
									break;
								}
							}
						}
					}
				}
				break;
			case UPDATE_KIND_DELETE:
				pPlayer->DeleteBagItem(pBody->m_nItemSeq);
				break;
			case UPDATE_KIND_CHANGE:
				{
					pPlayer->ChangeBagItem(&cItem);

					if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMSHOP) )
					{
						CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
						assert(pCurrentProcess);

						short nWear = pBody->m_nCode/1000000;

						if( nWear <= ITEM_TYPE_KNEE )		// 표시되는 오브젝트라면
						{
							short WearToType[MAX_EQUIP] = 
							{
								ITEM_TYPE_FACE,		ITEM_TYPE_HAIR,		ITEM_TYPE_FACEPAINT,
								ITEM_TYPE_SHIRTS,	ITEM_TYPE_PANTS,	ITEM_TYPE_GLOVE,	ITEM_TYPE_SHOES,	ITEM_TYPE_SOCKS,
								ITEM_TYPE_EYE,		ITEM_TYPE_EAR,		ITEM_TYPE_NECK,		ITEM_TYPE_MASK,		ITEM_TYPE_MUFFLER,		ITEM_TYPE_BAG,
								ITEM_TYPE_WRIST,	ITEM_TYPE_ARM,		ITEM_TYPE_KNEE
							};

							for( int i=0;i<MAX_EQUIP;i++ )
							{
								if( WearToType[i] == nWear )
								{
									if( i == ITEM_EQUIP_FACE )
									{
										pPlayer->m_cShape.m_nSkin = pBody->m_nCode%100;
										pCurrentProcess->m_nSkin = pPlayer->m_cShape.m_nSkin;
									}

									pPlayer->m_nEquipWear[i] = pBody->m_nCode;
									pPlayer->m_pEquipWearModelID[i] = CObjectModelManager::GetPtr()->ExchangeItem( pPlayer->m_nEquipWear[i], pPlayer->m_cShape.m_nSkin, pPlayer->m_pEquipWearModelID[i], pPlayer->m_pModel );
									pCurrentProcess->FUNCTION_ExampleExchangeItem( pBody->m_nCode, i, true );
									break;
								}
							}
						}
					}
				}
				break;
			}
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetEquipItem(CHeadPacket* pPacket)
{
	CSCEquipItem*		pBody		= (CSCEquipItem*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjItemInfo*		pItem;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pItem = pPlayer->GetBagItem(pBody->m_nItemSeq);
			if(pItem == NULL)	return;

			for(int i = 0;i < static_cast<int>(pPlayer->m_vItemList.size()); i++)
			{
				if(  ITEM_TYPE_RUN <= pPlayer->m_vItemList.at(i)->m_pTable->m_nType )
					continue;

				if( (pItem->m_pTable->m_nType == pPlayer->m_vItemList.at(i)->m_pTable->m_nType) && 
					(pItem->m_pTable->m_nWear == pPlayer->m_vItemList.at(i)->m_pTable->m_nWear) )
					pPlayer->m_vItemList.at(i)->m_nEquipKind = 0;
			}

			pPlayer->EquipBagItem(pBody->m_nItemSeq);
			memcpy(&pPlayer->m_nEquipWear,	&pBody->m_nEquipWear, sizeof(int)*MAX_EQUIP);
			pPlayer->m_cItemOption			= pBody->m_cItemOption;

			CPlayerInformationDialog::GetPtr()->FUNCTION_PlayerConsumptionInformation();

			int nWear = pItem->m_pTable->m_nWear;

			if( 100 == nWear )	return;

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
				CInvenDialog::GetPtr()->FUNCTION_GetItemBagList();
			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
			{
				CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);

				if(pItem->m_pTable->m_nType == ITEM_TYPE_FACE || pItem->m_pTable->m_nType == ITEM_TYPE_GLOVE)
				{
					pCurrentProcess->ExchangeItem();
					pCurrentProcess->FUNCTION_ExchangeItem(pItem->m_nCode, nWear);
				}
				else
					pCurrentProcess->FUNCTION_ExchangeItem(pItem->m_nCode, nWear);
			}
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 아이템을 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetDivestItem(CHeadPacket* pPacket)
{
	CSCDivestItem*		pBody		= (CSCDivestItem*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjItemInfo*		pItem;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pItem = pPlayer->GetBagItem(pBody->m_nItemSeq);
			if(pItem == NULL)	return;

			pItem->m_nEquipKind				= pBody->m_nEquipKind;
			memcpy(&pPlayer->m_nEquipWear,	&pBody->m_nEquipWear, sizeof(int)*MAX_EQUIP);
			pPlayer->m_cItemOption			= pBody->m_cItemOption;


			CPlayerInformationDialog::GetPtr()->FUNCTION_PlayerConsumptionInformation();

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
				CInvenDialog::GetPtr()->FUNCTION_GetItemBagList();
			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
			{
				CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				if(!pCurrentProcess) return;
				_Assert(pCurrentProcess, "GetDivestItem", 0, "", 0);

				int nWear = pItem->m_pTable->m_nWear;

				if( 100 == nWear )	return;

				if( pItem->m_pTable->m_nType == ITEM_TYPE_FACE)
				{
					pCurrentProcess->ExchangeItem();
					pCurrentProcess->FUNCTION_ExchangeItem(pPlayer->m_nEquipWear[nWear], nWear);
				}
				else
				{
					CObjectModelManager::GetPtr()->DeleteModelObject(pPlayer->m_pEquipWearModelID[nWear]);
					pCurrentProcess->FUNCTION_ExchangeItem(pPlayer->m_nEquipWear[nWear], nWear);
				}

				RemoveItemDivest( pPlayer->m_pModel->m_spNode );
			}
		}
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 아이템을 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetBuyItem(CHeadPacket* pPacket)
{
	CSCBuyItem*			pBody		= (CSCBuyItem*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_cMoney				= pBody->m_cMoney;
		pPlayer->m_cShape				= pBody->m_cShape;
		PutUpdateOption();
		memcpy(&pPlayer->m_nEquipWear,	&pBody->m_nEquipWear, sizeof(int)*MAX_EQUIP);

		//아이템이 정상적으로 구매되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20201, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -3: //보관 가능한 아이템 최대 갯수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //코인 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310411, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -12: //코인 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310412, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -13: //코인 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310413, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //코인이 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310414, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -21: //포인트 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310421, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -22: //포인트 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310422, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -23: //포인트 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310423, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -24: //포인트가 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310424, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetExchangeItem(CHeadPacket* pPacket)
{
	CSCExchangeItem*	pBody		= (CSCExchangeItem*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_cMoney				= pBody->m_cMoney;
		pPlayer->m_cShape				= pBody->m_cShape;
		memcpy(&pPlayer->m_nEquipWear,	&pBody->m_nEquipWear, sizeof(int)*MAX_EQUIP);

		//아이템이 정상적으로 교환되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20202, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -2: //해당 아이템을 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //코인 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310411, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -12: //코인 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310412, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -13: //코인 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310413, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //코인이 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310414, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -21: //포인트 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310421, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -22: //포인트 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310422, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -23: //포인트 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310423, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -24: //포인트가 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310424, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetShopSkillList(CHeadPacket* pPacket)
{
	CSCShopSkillList*		pBody		= (CSCShopSkillList*)pPacket;
	int						nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CResSkillTableFormat*	pTable;

	switch(pBody->m_nResponse)
	{
		case 0:
			{
				//? 임시방편적 코드 반드시 드러내야된다
				CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
				CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
				if(!pCurrentProcess) return;
				_Assert(pCurrentProcess, "GetShopSkillList", 0, "", 0);

				pCurrentProcess->m_nCurrentPage = pBody->m_nCurrentPage;
				pCurrentProcess->m_nTotalPage = pBody->m_nTotalPage;

				for(int i = 0; i < LIST6_SIZE; i++)
				{
					pCurrentProcess->m_nCode[i] = pBody->m_cShopSkillData[i].m_nCode;
					pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pCurrentProcess->m_nCode[i], 0));
					assert(pTable);
					if(pTable) pCurrentProcess->FUNCTION_ItemRender(i, pCurrentProcess->m_nCode[i], GetSkillPoint(pCurrentProcess->m_nCode[i]), GetSkillCash(pCurrentProcess->m_nCode[i]), pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
					else pCurrentProcess->FUNCTION_ItemRender(i, 0, 0, 0);
				}
			}
			break;
		default:
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetUpdateSkill(CHeadPacket* pPacket)
{
	CSCUpdateSkill*		pBody		= (CSCUpdateSkill*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjSkillInfo		cSkill;

	switch(pBody->m_nResponse)
	{
	case 0:
		cSkill.m_nSkillSeq		= pBody->m_nSkillSeq;
		cSkill.m_nCode			= pBody->m_nCode;
		cSkill.m_nEquipKind		= pBody->m_nEquipKind;
		cSkill.m_nLevel			= pBody->m_nLevel;

		cSkill.m_pTable			= dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pBody->m_nCode, 0));
		assert(cSkill.m_pTable);
		cSkill.m_fCoolTime		= 0.0f;
		cSkill.m_fEndTime		= cSkill.m_pTable->m_fCool[cSkill.m_nLevel - 1];

		switch(pBody->m_nUpdateKind)
		{
		case UPDATE_KIND_CREATE:
			pPlayer->CreateBagSkill(&cSkill);
			break;
		case UPDATE_KIND_DELETE:
			pPlayer->DeleteBagSkill(pBody->m_nSkillSeq);
			break;
		case UPDATE_KIND_CHANGE:
			pPlayer->ChangeBagSkill(&cSkill);
			break;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetEquipSkill(CHeadPacket* pPacket)
{
	CSCEquipSkill*		pBody		= (CSCEquipSkill*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjSkillInfo*		pSkill;

	switch(pBody->m_nResponse)
	{
	case 0:
		pSkill = pPlayer->GetBagSkill(pBody->m_nSkillSeq);
		if(pSkill == NULL)	return;

		pSkill->m_nEquipKind				= pBody->m_nEquipKind;

		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
			CInvenDialog::GetPtr()->FUNCTION_GetSkillBagList();
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 스킬을 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 320212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetDivestSkill(CHeadPacket* pPacket)
{
	CSCDivestSkill*		pBody		= (CSCDivestSkill*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjSkillInfo*		pSkill;

	switch(pBody->m_nResponse)
	{
	case 0:
		pSkill = pPlayer->GetBagSkill(pBody->m_nSkillSeq);
		if(pSkill == NULL)	return;

		pSkill->m_nEquipKind				= pBody->m_nEquipKind;

		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
			CInvenDialog::GetPtr()->FUNCTION_GetSkillBagList();
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 스킬을 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 320212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetBuySkill(CHeadPacket* pPacket)
{
	CSCBuySkill*		pBody		= (CSCBuySkill*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_cMoney				= pBody->m_cMoney;

		//스킬이 정상적으로 구매되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20203, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -3: //보관 가능한 아이템 최대 갯수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -5: //이미 소유하고 있는 스킬입니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //코인 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310411, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -12: //코인 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310412, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -13: //코인 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310413, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //코인이 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310414, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -21: //포인트 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310421, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -22: //포인트 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310422, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -23: //포인트 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310423, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -24: //포인트가 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310424, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetShopTrainingList(CHeadPacket* pPacket)
{
	CSCShopTrainingList*		pBody		= (CSCShopTrainingList*)pPacket;
	int							nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CResTrainingTableFormat*	pTable;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			//? 임시방편적 코드 반드시 드러내야된다
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetShopTrainingList", 0, "", 0);

			pCurrentProcess->m_nCurrentPage = pBody->m_nCurrentPage;
			pCurrentProcess->m_nTotalPage = pBody->m_nTotalPage;

			for(int i = 0; i < LIST6_SIZE; i++)
			{
				pCurrentProcess->m_nCode[i] = pBody->m_cShopTrainingData[i].m_nCode;
				pTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, pCurrentProcess->m_nCode[i], 0));
				assert(pTable);
				if(pTable) pCurrentProcess->FUNCTION_ItemRender(i, pCurrentProcess->m_nCode[i], GetTrainingPoint(pCurrentProcess->m_nCode[i]), GetTrainingCash(pCurrentProcess->m_nCode[i]), pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
				else pCurrentProcess->FUNCTION_ItemRender(i, 0, 0, 0);
			}
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetUpdateTraining(CHeadPacket* pPacket)
{
	CSCUpdateTraining*	pBody		= (CSCUpdateTraining*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjTrainingInfo	cTraining;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			cTraining.m_nTrainingSeq	= pBody->m_nTrainingSeq;
			cTraining.m_nCode			= pBody->m_nCode;
			cTraining.m_nLevel			= pBody->m_nLevel;

			switch(pBody->m_nUpdateKind)
			{
			case UPDATE_KIND_CREATE:
				pPlayer->CreateBagTraining(&cTraining);
				break;
			case UPDATE_KIND_DELETE:
				pPlayer->DeleteBagTraining(pBody->m_nTrainingSeq);
				break;
			case UPDATE_KIND_CHANGE:
				pPlayer->ChangeBagTraining(&cTraining);
				break;
			}

			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetUpdateTraining", 0, "", 0);

			pCurrentProcess->UpdateTrainingCode(pCurrentProcess->m_nCurrentPage);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetBuyTraining(CHeadPacket* pPacket)
{
	CSCBuyTraining*		pBody		= (CSCBuyTraining*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pMember->m_cMoney				= pBody->m_cMoney;
			pPlayer->m_cTrainingFaculty		= pBody->m_cTrainingFaculty;

			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetBuyTraining", 0, "", 0);
			
			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_SKILLSHOP) )
			{
				pCurrentProcess->InitTrainingCode();
				pCurrentProcess->UpdateTrainingCode(pCurrentProcess->m_nCurrentPage);
			}

			//훈련이 정상적으로 구매되었습니다.
			CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20205, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -3: //보관 가능한 아이템 최대 갯수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //코인 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310411, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -12: //코인 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310412, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -13: //코인 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310413, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //코인이 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310414, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -15: //이미 더 높은 수준의 트레이닝이 수행되었습니다. (CYG)
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310415, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -16: // 레벨에 맞지 않는 트레이닝 (CYG)
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310416, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -21: //포인트 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310421, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -22: //포인트 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310422, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -23: //포인트 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310423, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -24: //포인트가 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310424, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetShopCeremonyList(CHeadPacket* pPacket)
{
	CSCShopCeremonyList*		pBody		= (CSCShopCeremonyList*)pPacket;
	int							nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CResCeremonyTableFormat*	pTable;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			//? 임시방편적 코드 반드시 드러내야된다
			CBaseProcess *temp = CProcessManager::GetPtr()->Get_CurrentProcess();
			CSkillShopProcess* pCurrentProcess = dynamic_cast<CSkillShopProcess*>(temp);
			if(!pCurrentProcess) return;
			_Assert(pCurrentProcess, "GetShopCeremonyList", 0, "", 0);

			pCurrentProcess->m_nCurrentPage = pBody->m_nCurrentPage;
			pCurrentProcess->m_nTotalPage = pBody->m_nTotalPage;

			for(int i = 0; i < LIST6_SIZE; i++)
			{
				pCurrentProcess->m_nCode[i] = pBody->m_cShopCeremonyData[i].m_nCode;
				pTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, pCurrentProcess->m_nCode[i], 0));
				assert(pTable);
				if(pTable) pCurrentProcess->FUNCTION_ItemRender(i, pCurrentProcess->m_nCode[i], GetCeremonyPoint(pCurrentProcess->m_nCode[i]), GetCeremonyCash(pCurrentProcess->m_nCode[i]), pTable->m_sName, pTable->m_sIconFile, pTable->m_nSell);
				else pCurrentProcess->FUNCTION_ItemRender(i, 0, 0, 0);
			}
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetUpdateCeremony(CHeadPacket* pPacket)
{
	CSCUpdateCeremony*	pBody		= (CSCUpdateCeremony*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjCeremonyInfo	cCeremony;

	switch(pBody->m_nResponse)
	{
	case 0:
		cCeremony.m_nCeremonySeq	= pBody->m_nCeremonySeq;
		cCeremony.m_nCode			= pBody->m_nCode;
		cCeremony.m_nEquipKind		= pBody->m_nEquipKind;

		switch(pBody->m_nUpdateKind)
		{
		case UPDATE_KIND_CREATE:
			pPlayer->CreateBagCeremony(&cCeremony);
			break;
		case UPDATE_KIND_DELETE:
			pPlayer->DeleteBagCeremony(pBody->m_nCeremonySeq);
			break;
		case UPDATE_KIND_CHANGE:
			pPlayer->ChangeBagCeremony(&cCeremony);
			break;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetEquipCeremony(CHeadPacket* pPacket)
{
	CSCEquipCeremony*	pBody		= (CSCEquipCeremony*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjCeremonyInfo*	pCeremony;

	switch(pBody->m_nResponse)
	{
	case 0:
		pCeremony = pPlayer->GetBagCeremony(pBody->m_nCeremonySeq);
		if(pCeremony == NULL)	return;

		pCeremony->m_nEquipKind				= pBody->m_nEquipKind;
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 세레모니를 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 330212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetDivestCeremony(CHeadPacket* pPacket)
{
	CSCDivestCeremony*	pBody		= (CSCDivestCeremony*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjCeremonyInfo*	pCeremony;

	switch(pBody->m_nResponse)
	{
	case 0:
		pCeremony = pPlayer->GetBagCeremony(pBody->m_nCeremonySeq);
		if(pCeremony == NULL)	return;

		pCeremony->m_nEquipKind				= pBody->m_nEquipKind;
		break;
	case -11: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -12: //해당 세레모니를 찾을 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 330212, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetBuyCeremony(CHeadPacket* pPacket)
{
	CSCBuyCeremony*		pBody		= (CSCBuyCeremony*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_cMoney				= pBody->m_cMoney;

		//세레모니가 정상적으로 구매되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20204, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -3: //보관 가능한 아이템 최대 갯수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -11: //코인 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310411, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -12: //코인 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310412, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -13: //코인 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310413, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -14: //코인이 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310414, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -21: //포인트 가격 정보가 손상되었습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310421, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -22: //포인트 가격 정보에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310422, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -23: //포인트 관련 데이터베이스에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 310423, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -24: //포인트가 부족하여 구입할 수 없습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 310424, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetQuestList(CHeadPacket* pPacket)
{
	CSCQuestList*			pBody		= (CSCQuestList*)pPacket;
	int						nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			int          nTotalPage    = pBody->m_nTotalPage;
            int          nCurrentPage  = pBody->m_nCurrentPage;
            CQuestData * pcQuestData   = &pBody->m_cQuestData[0];

            int nSize = sizeof( pBody->m_cQuestData ) / sizeof( CQuestData );
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetUpdateQuest(CHeadPacket* pPacket)
{
	CSCUpdateQuest*		pBody		= (CSCUpdateQuest*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);
	CObjQuestInfo		cQuest;

	switch(pBody->m_nResponse)
	{
	case 0:
		cQuest.m_nQuestSeq		= pBody->m_nQuestSeq;
		cQuest.m_nCode			= pBody->m_nCode;
		cQuest.m_nAmount		= pBody->m_nAmount;
		cQuest.m_nPlayDate		= pBody->m_nPlayDate;

		switch(pBody->m_nUpdateKind)
		{
		case UPDATE_KIND_CREATE:
			pPlayer->CreateBagQuest(&cQuest);
			break;
		case UPDATE_KIND_DELETE:
			pPlayer->DeleteBagQuest(pBody->m_nQuestSeq);
			break;
		case UPDATE_KIND_CHANGE:
			pPlayer->ChangeBagQuest(&cQuest);
			break;
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetCreateQuest(CHeadPacket* pPacket)
{
	CSCCreateQuest*	pBody		= (CSCCreateQuest*)pPacket;
	int				nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //생성 가능한 최대 방 개수를 초과하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100099, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetSendMessage(CHeadPacket* pPacket)
{
	CSCSendMessage*		pBody		= (CSCSendMessage*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	char				sMessage[CHAT_SIZE];

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			switch(pBody->m_nChatKind)
			{
			case CHAT_KIND_NORMAL:
				_snprintf(sMessage, CHAT_SIZE, "%s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_NORMAL);
				break;
			case CHAT_KIND_PLAY:
				{
					VectorAnyoneList* pAnyoneList = CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);
					CObjPlayerInfo*	pPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
					assert(pPlayer);

					int nTeamSend = 0;
					int nTeamRecv = 0;

					for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
					{
						for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
						{
							if((*iPlayer)->GetObjSeq() == pBody->m_nPlayerSeq)
								nTeamSend = (*iPlayer)->m_nTeam;
							if((*iPlayer)->GetObjSeq() == pPlayer->GetObjSeq())
								nTeamRecv = (*iPlayer)->m_nTeam;
						}
					}

					if(nTeamSend == nTeamRecv)
					{
						_snprintf(sMessage, CHAT_SIZE, "%s: %s", pBody->m_sFromName, pBody->m_sMessage);
						CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_CHAT_BLUE);
					}
					else
					{
						_snprintf(sMessage, CHAT_SIZE, "%s: %s", pBody->m_sFromName, pBody->m_sMessage);
						CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_CHAT_RED);
					}
				}
				break;
			case CHAT_KIND_TEAM:
				_snprintf(sMessage, CHAT_SIZE, "[팀채팅]%s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_TEAM);
				break;
			case CHAT_KIND_SECRET:
				CChattingDialog::GetPtr()->WhisperIdSave(CChattingDialog::GetPtr()->m_sId);
				_snprintf(sMessage, CHAT_SIZE, "[귓속말]To %s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_SECRET);
				break;
			case CHAT_KIND_SECRET2:
				_snprintf(sMessage, CHAT_SIZE, "[귓속말]From %s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_SECRET);
				break;
			case CHAT_KIND_CLUB:
				_snprintf(sMessage, CHAT_SIZE, "[클럽]%s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_CLUB);
				break;
			case CHAT_KIND_ANNOUNCE:
				_snprintf(sMessage, CHAT_SIZE, "%s", pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_ANNOUNCE);
				break;
			case CHAT_KIND_NOTICE:
				_snprintf(sMessage, CHAT_SIZE, "[공지사항] %s", pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_NOTICE);
				break;
			case CHAT_KIND_OPERATOR:
				_snprintf(sMessage, CHAT_SIZE, "%s: %s", pBody->m_sFromName, pBody->m_sMessage);
				CChattingDialog::GetPtr()->SetChatMessage(sMessage, true, COLOR_OPERATOR);
				break;
			}
			//CChattingDialog::GetPtr()->SetChatMessage(pBody->m_sMessage, true, D3DCOLOR_ARGB(255, 0, 0, 0));
		}
		break;
	case -2:
		{
			CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, nMsgCode ,0));
			assert(pMTable);
			CChattingDialog::GetPtr()->SetChatMessage(pMTable->m_sMessage, true, COLOR_SECRET);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 캐릭터 찾기 (CYG)
void GetCharacterSearch(CHeadPacket* pPacket)
{
	CSCCharacterSearch*	pBody		= (CSCCharacterSearch*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if(pBody->m_nPlayerSeq)		// 같은 이름이 존재합니다.
				CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 200213, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
			else						// 은(는) 사용할 수 있는 아이디 입니다\이 아이디를 사용하시겠습니까?
				CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_DUPLICATION, 0, CMessageBoxDialog::MSGBOXTYPE_CONFIRMCANCEL);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 아이템 전달 (소포시스템) (CYG)
void GetPostItem(CHeadPacket* pPacket)
{
	CSCPostItem*	pBody		= (CSCPostItem*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 미션에 대한 보상을 한다. (CYG)
void GetMissionReward(CHeadPacket* pPacket)
{
	CSCMissionReward*	pBody		= (CSCMissionReward*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
	case -3:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 골든,토너먼트,클럽등의 스케쥴 타임 (CYG)
void GetScheduleList(CHeadPacket* pPacket)
{
	CSCScheduleList*	pBody		= (CSCScheduleList*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		//SignBoard::GetPtr()->InitSignBoard( *pBody );
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
	case -3:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 플레이어 정보 끝 (CYG)
void GetPlayerinfoEnd(CHeadPacket* pPacket)
{
	CSCPlayerinfoEnd*	pBody		= (CSCPlayerinfoEnd*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CPacketManager::GetPtr()->m_bIsPlayerInfoDone = true;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
	case -2:
	case -3:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 오토파일럿 모드 설정 (CYG)
void GetAutopilotMode(CHeadPacket* pPacket)
{
	CSCAutopilotMode*	pBody		= (CSCAutopilotMode*)pPacket;
	int	nMsgCode					= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CObjPlayerInfo* pPlayerInfo = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_TOTAL, pBody->m_nPlayerSeq);

			if(pBody->m_bEnable) pPlayerInfo->SetKind(PLAYER_KIND_USERBOT);
			else pPlayerInfo->SetKind(PLAYER_KIND_USER);
		}
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 현재 날씨 얻어오기 (CYG)
void GetCurrentWeather(CHeadPacket* pPacket)
{
	CSCCurrentWeather*	pBody		= (CSCCurrentWeather*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		UICommonMethod::WeatherChange(pBody->m_nCurrentWeather);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 소모성 아이템 사용 통지 (CYG)
void GetConsumeItem(CHeadPacket* pPacket)
{
	CSCConsumeItem*	pBody		= (CSCConsumeItem*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 현재 시간 얻어오기 (CYG)
void GetCurrentTimePacket(CHeadPacket* pPacket)
{
	CSCCurrentTime*	pBody		= (CSCCurrentTime*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		UICommonMethod::TimeChange(pBody->m_nRoomTimeType);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 접속중인 사용자 강제퇴장 (CYG)
void GetDrawforcePlayer(CHeadPacket* pPacket)
{
	CSCDrawforcePlayer*	pBody		= (CSCDrawforcePlayer*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// TCP Ping (CYG)
void GetTcpPing(CHeadPacket* pPacket)
{
	CSCTCPPing*	pBody		= (CSCTCPPing*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		PutTcpPing();
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

// 스킬 강화 (CYG)
void GetUpgradeSkill(CHeadPacket* pPacket)
{
	CSCUpgradeSkill*	pBody		= (CSCUpgradeSkill*)pPacket;
	int	nMsgCode					= (pBody->m_nCommand*100) - pBody->m_nResponse;

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pObjPlayerInfo->m_cLevel.m_nSkill = pBody->m_nSkillPoint;
			for(int i=0;i<(int)pObjPlayerInfo->m_vSkillList.size();i++)
			{
				int seq = pObjPlayerInfo->m_vSkillList.at(i)->m_nSkillSeq;
				if(pObjPlayerInfo->m_vSkillList.at(i)->m_nSkillSeq == pBody->m_nSkillSeq)
					pObjPlayerInfo->m_vSkillList.at(i)->m_nLevel = pBody->m_nLevel;
			}
				
			break;
		}
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}


// 아이템 능력치 갱신 (CYG)
void GetUpdateOption(CHeadPacket* pPacket)
{
	CSCUpdateOption*		pBody		= (CSCUpdateOption*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

    CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		pObjPlayerInfo->m_cItemOption = pBody->m_cItemOption;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////

// 옵션 정보 (CYG)
void GetSettingInfo(CHeadPacket* pPacket)
{
	CSCSettingInfo*	pBody		= (CSCSettingInfo*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if(pObjPlayerInfo != NULL)
			{
				pObjPlayerInfo->m_cSetting = pBody->m_cSetting;
				//CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(pObjPlayerInfo);
			}
			else
			{
				CGameOptionDialog::GetPtr()->m_pObjPlayerInfo->m_cSetting = pBody->m_cSetting;
				//CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(CGameOptionDialog::GetPtr()->m_pObjPlayerInfo);
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 같은 방에 있는 사람들끼리 동기화여부를 확인하는 프로토콜 (CYG)

void GetSynchPlayer(CHeadPacket* pPacket)
{
	CSCSynchPlayer*	pBody		= (CSCSynchPlayer*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드로봇 정보 (CYG)
void GetCardbotInfo(CHeadPacket* pPacket)
{
	CSCCardbotInfo*		pBody		= (CSCCardbotInfo*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= NULL;
	CCardBotInfo*		pInfo		= &(pBody->m_cCardBotInfo);

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pPlayer = new CObjPlayerInfo;
			if(pPlayer == NULL) return;

			pPlayer->InitPlayerInRoom(PLAYER_KIND_CARDBOT);

			pPlayer->SetObjSeq(pInfo->m_cCard.m_nCardSeq + 1000000000);
			pPlayer->m_nTeam						= pInfo->m_nTeam;
			pPlayer->m_nSeat						= pInfo->m_nSeat - 1;
			pPlayer->m_cLevel.m_nLevel				= pInfo->m_cCard.m_nLevel;
			pPlayer->m_cShape.m_nUniform			= PLAYER_UNIFORM_NONE;
			pPlayer->m_nPosition					= pInfo->m_cCard.m_nPosition;

			pPlayer->m_cRobot.m_nRank				= pInfo->m_cCard.m_nRank;
			pPlayer->m_cRobot.m_nSkill				= pInfo->m_cCard.m_nSkill;
			pPlayer->m_cRobot.m_nType				= pInfo->m_cCard.m_nType;
			pPlayer->m_cRobot.m_nCostume			= pInfo->m_cCard.m_nCostume;

			pPlayer->m_nLoadingStep					= 100; //인공지능은 로딩이 무조건 다 된것으로 간주

			CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + pPlayer->m_nPosition*100 + pPlayer->m_cLevel.m_nLevel, 0));
			assert(pTempAbility);
			CResTempCostumeTableFormat* pTempCostume = dynamic_cast<CResTempCostumeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPCOSTUMETABLE, pPlayer->m_cRobot.m_nCostume, 0));
			assert(pTempCostume);

			CFaculty cFaculty;
			pTempAbility->SetValue(pPlayer->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, pPlayer->m_cTrainingFaculty.m_nFaculty, pPlayer->m_cRobot.m_nRank);

			pTempCostume->SetValue(pPlayer->m_nEquipWear);

			CObjStack* pStack = CObjectManager::GetPtr()->GetObjStack(static_cast<OBJECT_TYPE>(pPlayer->m_nPosition*10));
			if(pStack) pPlayer->SetSkillStack(pStack, pPlayer->m_cRobot.m_nSkill);

			pPlayer->m_cShape.m_nGender = (pPlayer->m_nEquipWear[0]%100/10);

	//		GetPositionString(pPlayer->m_nPosition, pPlayer->m_sName);//AI들의 이름

			_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, "CARDBOT");
			//_snprintf(pPlayer->m_sName, PLAYER_NAME_SIZE, "N%d L%d F%d", pPlayer->GetObjSeq()%1000, pPlayer->m_cLevel.m_nLevel, pPlayer->m_nForm);

			CObjPlayerManager::GetPtr()->SetPlayerLists(pPlayer, pPlayer->m_nTeam);
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드봇 정보 끝 (CYG)
void GetCardbotEnd(CHeadPacket* pPacket)
{
	CSCCardbotEnd*	pBody		= (CSCCardbotEnd*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		CPacketManager::GetPtr()->m_bIsAthleteInfoDone = true;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드템 정보 (CYG)
void GetCardInfo(CHeadPacket* pPacket)
{
	CSCCardInfo*		pBody		= (CSCCardInfo*)pPacket;
	int	nMsgCode					= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CCardInfo*			pInfo;

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<pBody->m_nCount;++i)
			{
				pInfo = new CCardInfo();
				
				pInfo->m_nCardSeq	= pBody->m_cCardInfo[i].m_nCardSeq;
				pInfo->m_nCostume	= pBody->m_cCardInfo[i].m_nCostume;
				pInfo->m_nEnchant	= pBody->m_cCardInfo[i].m_nEnchant;
				memcpy( pInfo->m_nEquipKind, pBody->m_cCardInfo[i].m_nEquipKind, sizeof(int)*3 );
				pInfo->m_nLevel		= pBody->m_cCardInfo[i].m_nLevel;
				pInfo->m_nPlayerSeq = pBody->m_cCardInfo[i].m_nPlayerSeq;
				pInfo->m_nPosition	= pBody->m_cCardInfo[i].m_nPosition;
				pInfo->m_nRank		= pBody->m_cCardInfo[i].m_nRank;
				pInfo->m_nSkill		= pBody->m_cCardInfo[i].m_nSkill;
				pInfo->m_nState		= pBody->m_cCardInfo[i].m_nState;
				pInfo->m_nTierd		= pBody->m_cCardInfo[i].m_nTierd;
				pInfo->m_nType		= pBody->m_cCardInfo[i].m_nType;
				
				pPlayer->m_vCardbotList.push_back(pInfo);
			}
			
			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
			{
				CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);
				
				if( pBody->m_nCount != MAX_CARD_LIST )
					pCurrentProcess->FUNCTION_InitEntry();
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드장착 (CYG)
void GetEquipCard(CHeadPacket* pPacket)
{
	CSCEquipCard*	pBody		= (CSCEquipCard*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			for(int i=0;i<(int)pPlayer->m_vCardbotList.size();i++)
			{
				if( pPlayer->m_vCardbotList.at(i)->m_nCardSeq == pBody->m_nCardSeq )
				{

					if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
					{
						CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
						assert(pCurrentProcess);

						pPlayer->m_vCardbotList.at(i)->m_nEquipKind[pCurrentProcess->m_nEntry] = pBody->m_nEquipKind;
						
						pCurrentProcess->FUNCTION_GetCardInfo();
					}
					break;
				}
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드해제 (CYG)
void GetDivestCard(CHeadPacket* pPacket)
{
	CSCDivestCard*	pBody		= (CSCDivestCard*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
			{
				CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);
				
				if( pBody->m_nCardSeq == 0 )
				{
					for(int i=0;i<(int)pPlayer->m_vCardbotList.size();i++)
						pPlayer->m_vCardbotList.at(i)->m_nEquipKind[pCurrentProcess->m_nEntry] = 0;
					pCurrentProcess->FUNCTION_GetCardInfo();
					pCurrentProcess->FUNCTION_InitEntry();
				}
				else
				{
					for(int i=0;i<(int)pPlayer->m_vCardbotList.size();i++)
					{
						if( pPlayer->m_vCardbotList.at(i)->m_nCardSeq == pBody->m_nCardSeq )
						{
							pPlayer->m_vCardbotList.at(i)->m_nEquipKind[pCurrentProcess->m_nEntry] = pBody->m_nEquipKind;
							CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
							assert(pCurrentProcess);
							pCurrentProcess->FUNCTION_GetCardInfo();
							break;
						}
					}
				}
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드획득 (CYG)
void GetGetCard(CHeadPacket* pPacket)
{
	CSCGetCard*	pBody		= (CSCGetCard*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			CCardInfo* CInfo = new CCardInfo();
			memcpy(CInfo, &pBody->m_cCard, sizeof(pBody->m_cCard));

			pPlayer->m_vCardbotList.push_back(CInfo);

			pRoom->m_bRecvCard = true;

			pRoom->m_nCardMsg = pBody->m_nResponse;
		}
		break;
	case -3: //인벤토리가 꽉찼습니다.
	case -4: //카드를 받을 조건이 되지 않습니다.
	case -5: //크레딧이 부족합니다.
		pRoom->m_nCardMsg = pBody->m_nResponse;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 아이템조합 타입 I (CYG)
void GetMixItem1(CHeadPacket* pPacket)
{
	CSCMixItem1*	pBody	= (CSCMixItem1*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 아이템조합 타입 II (CYG)
void GetMixItem2(CHeadPacket* pPacket)
{
	CSCMixItem2*	pBody		= (CSCMixItem2*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드 조합 I (CYG)
void GetMixCard1(CHeadPacket* pPacket)
{
	CSCMixCard1*	pBody		= (CSCMixCard1*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
		{
			CCardInfo* pCardInfo = new CCardInfo();

			pCardInfo->m_nCardSeq	= pBody->m_cCard.m_nCardSeq;
			pCardInfo->m_nCostume	= pBody->m_cCard.m_nCostume;
			pCardInfo->m_nEnchant	= pBody->m_cCard.m_nEnchant;
			memcpy( pCardInfo->m_nEquipKind, pBody->m_cCard.m_nEquipKind, sizeof(int)*3 );
			pCardInfo->m_nLevel		= pBody->m_cCard.m_nLevel;
			pCardInfo->m_nPlayerSeq = pBody->m_cCard.m_nPlayerSeq;
			pCardInfo->m_nPosition	= pBody->m_cCard.m_nPosition;
			pCardInfo->m_nRank		= pBody->m_cCard.m_nRank;
			pCardInfo->m_nSkill		= pBody->m_cCard.m_nSkill;
			pCardInfo->m_nState		= pBody->m_cCard.m_nState;
			pCardInfo->m_nTierd		= pBody->m_cCard.m_nTierd;
			pCardInfo->m_nType		= pBody->m_cCard.m_nType;

			pPlayer->m_vCardbotList.push_back(pCardInfo);

			// 카드 뽑기 연출;
			CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			assert(pCurrentProcess);

			pCurrentProcess->FUNCTION_CardMixResult(pBody->m_cCard);
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드 조합 II (CYG)
void GetMixCard2(CHeadPacket* pPacket)
{
	CSCMixCard2*	pBody		= (CSCMixCard2*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 친구 정보 (CYG)
void GetBuddyInfo(CHeadPacket* pPacket)
{
	CSCBuddyInfo*	pBody		= (CSCBuddyInfo*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*	pMyPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pMyPlayer->m_vBuddyList.clear();

			for(int i=0;i<pBody->m_nCount;i++)
			{
				CBuddyInfo* CInfo = new CBuddyInfo();
				memcpy(CInfo, &pBody->m_cBuddyInfo[i], sizeof(pBody->m_cBuddyInfo[i]));

				pMyPlayer->m_vBuddyList.push_back(CInfo);
				CPlayerInformationDialog::GetPtr()->m_nAthleteTotalPage = pBody->m_nTotalPage;
			}
			CPlayerInformationDialog::GetPtr()->FUNCTION_AtheletListInformation( CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex, pBody->m_nPage );
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 골인 TCP 서버 처리 (CYG)
void GetGoalinTcp(CHeadPacket* pPacket)
{
	CSCGoalinTcp*	pBody		= (CSCGoalinTcp*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		GetGoalIn( &(pBody->m_cGoalPacket) );
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

void GetGoalinTcp_Replay(CHeadPacket* pPacket)
{
	CSCGoalinTcp*	pBody		= (CSCGoalinTcp*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		GetGoalIn_Replay( &(pBody->m_cGoalPacket) );
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

// ON/OFF 값 공유 (CYG)
void GetSwitchValue(CHeadPacket* pPacket)
{
	CSCSwitchValue*	pBody		= (CSCSwitchValue*)pPacket;
	int	nMsgCode				= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjRoomInfo*	pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjPlayerInfo*	pMyPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			switch(pBody->m_nType) 
			{
			case SWITCH_PASSCALL:
				CObjPlayerManager::GetPtr()->SetPassCallSetting(pBody->m_nPlayerSeq, pBody->m_nValue);
				break;
			case SWITCH_PLAYERCALL:
				{
					int nForm1 = (pBody->m_nValue)/100;
					int nForm2 = pBody->m_nValue%100;

					bool bIsStay = false;
					CObjPlayerInfo * pSelectPlayerInfo = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_nPlayerSeq);
					CObjPlayerInfo * pPlayerInfo1 = CObjPlayerManager::GetPtr()->GetPlayerPointer(bIsStay, pSelectPlayerInfo->m_nTeam, nForm1);
					CObjPlayerInfo * pPlayerInfo2 = CObjPlayerManager::GetPtr()->GetPlayerPointer(bIsStay, pSelectPlayerInfo->m_nTeam, nForm2);

					if(!(pPlayerInfo1 && pPlayerInfo2)) return;		

					pPlayerInfo1->SetKind(PLAYER_KIND_CARDBOT);//

					pPlayerInfo2->m_bPutUDP = pPlayerInfo1->m_bPutUDP;

					memcpy(&pPlayerInfo2->m_cUDPAddress, &pPlayerInfo1->m_cUDPAddress, sizeof(pPlayerInfo1->m_cUDPAddress));

					memcpy(&pPlayerInfo2->m_cSetting, &pPlayerInfo1->m_cSetting, sizeof(pPlayerInfo1->m_cSetting));

					memcpy(pPlayerInfo2->m_bIsUseKey, pPlayerInfo1->m_bIsUseKey, sizeof(pPlayerInfo1->m_bIsUseKey));

					pPlayerInfo2->SetKind(PLAYER_KIND_USER);//

					if(pMyPlayer->GetObjSeq() == pSelectPlayerInfo->GetObjSeq())
					{
						CBasePlayProcess* pCurrentProcess = dynamic_cast<CBasePlayProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
						pCurrentProcess->SetSelectPlayer(pPlayerInfo2);
					}
				}
				break;
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 친구추가 (CYG)
void GetAddBuddy(CHeadPacket* pPacket)
{
	CSCAddBuddy*	pBody		= (CSCAddBuddy*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 블랙리스트 정보 (CYG)
void GetBlacklistInfo(CHeadPacket* pPacket)
{
	CSCBlacklistInfo*	pBody		= (CSCBlacklistInfo*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 블랙리스트 추가 (CYG)
void GetAddBlacklist(CHeadPacket* pPacket)
{
	CSCAddBlacklist*	pBody		= (CSCAddBlacklist*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 주간 랭킹 정보 (로그아웃할 때까지 한번만 호출하는 패킷 / 복수호출 금지 ) (CYG)
void GetWeeklyRecord(CHeadPacket* pPacket)
{
	CSCWeeklyRecord*	pBody		= (CSCWeeklyRecord*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 친구삭제 (CYG)
void GetDelBuddy(CHeadPacket* pPacket)
{
	CSCDelBuddy*	pBody		= (CSCDelBuddy*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 블랙리스트 삭제 (CYG)
void GetDelBlacklist(CHeadPacket* pPacket)
{
	CSCDelBlacklist*	pBody		= (CSCDelBlacklist*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}

// 주간 랭킹 정보  (CYG)
void GetWeeklyRanking(CHeadPacket* pPacket)
{
	CSCWeeklyRanking*	pBody		= (CSCWeeklyRanking*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 아이템 선물 (CYG)
void GetGiftItem(CHeadPacket* pPacket)
{
	CSCGiftItem*	pBody		= (CSCGiftItem*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 랜덤샵의 목록을 요청한다 (CYG)
void GetRandomshopitemList(CHeadPacket* pPacket)
{
	CSCRandomshopitemList*	pBody		= (CSCRandomshopitemList*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*	pPlayer	= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pPlayer->m_vRandomItemList.clear();

			for(int i=0;i<pBody->m_nItemCnt;i++)
			{
				CObjItemInfo cItem;
				cItem.m_nItemSeq	= pBody->m_cShopItemData[i].m_nItemSeq;
				cItem.m_nCode		= pBody->m_cShopItemData[i].m_nCode;
				cItem.m_nClass		= pBody->m_cShopItemData[i].m_nClass;
				cItem.m_nEquipKind	= pBody->m_cShopItemData[i].m_nEquipKind;
				cItem.m_nGrade		= pBody->m_cShopItemData[i].m_nGrade;
				cItem.m_nLevel		= pBody->m_cShopItemData[i].m_nLevel;
				cItem.m_nPrice		= pBody->m_cShopItemData[i].m_nPrice;
				cItem.m_nAmount		= pBody->m_cShopItemData[i].m_nAmount;
				memcpy(cItem.m_nOptionCode, pBody->m_cShopItemData[i].m_nOptionCode, sizeof(int)*LIST5_SIZE);

				pPlayer->m_vRandomItemList.push_back(cItem);
			}

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMSHOP) )
			{
				CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);

				pCurrentProcess->InitItemList();
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 랜덤샾의 아이템을 구입한다. (CYG)
void GetBuyRandomitem(CHeadPacket* pPacket)
{
	CSCBuyRandomitem* pBody		= (CSCBuyRandomitem*)pPacket;
	int	nMsgCode				= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo* pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	CObjMemberInfo*	pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			VectorRandomItemList::iterator it;
			for(  it = pPlayer->m_vRandomItemList.begin();it !=pPlayer->m_vRandomItemList.end();it++ )
			{
				CObjItemInfo Item = (*it);
				if(	Item.m_nItemSeq == pBody->m_nIdx )
				{
					pPlayer->m_vRandomItemList.erase(it);
					break;
				}
			}

			pMember->m_cMoney		= pBody->m_cMoney;
			memcpy(pPlayer->m_nEquipWear, pBody->m_nEquipWear, sizeof(int)*MAX_EQUIP);

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMSHOP) )
			{
				CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);

				pCurrentProcess->InitItemList();
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 아이템을 강화한다. (CYG)
void GetEnchantItem(CHeadPacket* pPacket)
{
	CSCEnchantItem*	pBody		= (CSCEnchantItem*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;

	switch(pBody->m_nResponse)
	{
	case 0:
		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
		{
			CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			assert(pCurrentProcess);

			pCurrentProcess->FUNCTION_GetBagList();
			pCurrentProcess->FUNCTIOIN_Enchant(pCurrentProcess->m_nEnchantIndex);
		}
		
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 900102, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 랜덤샵을 새로고침한다. (CYG)
void GetRefreshShop(CHeadPacket* pPacket)
{
	CSCRefreshShop*	pBody		= (CSCRefreshShop*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*	pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		pMember->m_cMoney = pBody->m_cMoney;
		PutRandomshopitemList();
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드부스터 구입 (CYG)
void GetBuyCardbooster(CHeadPacket* pPacket)
{
	CSCBuyCardbooster*	pBody		= (CSCBuyCardbooster*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjMemberInfo*	pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
	CObjPlayerInfo* pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			pMember->m_cMoney = pBody->m_cMoney;

			CCardInfo*	pInfo = new CCardInfo();
			pInfo->m_nCardSeq			= pBody->m_cCardInfo.m_nCardSeq;
			pInfo->m_nCardSeq			= pBody->m_cCardInfo.m_nCardSeq;
			pInfo->m_nCostume			= pBody->m_cCardInfo.m_nCostume;
			pInfo->m_nEnchant			= pBody->m_cCardInfo.m_nEnchant;
			memcpy(pInfo->m_nEquipKind, pBody->m_cCardInfo.m_nEquipKind, sizeof(int)*3);
			pInfo->m_nLevel				= pBody->m_cCardInfo.m_nLevel;
			pInfo->m_nPlayerSeq			= pBody->m_cCardInfo.m_nPlayerSeq;
			pInfo->m_nPosition			= pBody->m_cCardInfo.m_nPosition;
			pInfo->m_nRank				= pBody->m_cCardInfo.m_nRank;
			pInfo->m_nSkill				= pBody->m_cCardInfo.m_nSkill;
			pInfo->m_nState				= pBody->m_cCardInfo.m_nState;
			pInfo->m_nTierd				= pBody->m_cCardInfo.m_nTierd;
			pInfo->m_nType				= pBody->m_cCardInfo.m_nType;

			pPlayer->m_vCardbotList.push_back(pInfo);

			if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMSHOP) )
			{
				// 카드 뽑기 연출;
				CItemShopProcess* pCurrentProcess = dynamic_cast<CItemShopProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
				assert(pCurrentProcess);
				pCurrentProcess->FUNCTION_SetCardBooster(pInfo);
			}
		}		
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 카드 엔트리 세팅 (CYG)
void GetCardEntry(CHeadPacket* pPacket)
{
	CSCCardEntry*	pBody		= (CSCCardEntry*)pPacket;
	int	nMsgCode			= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo* pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_ITEMBAG) )
		{
			CItemBagProcess* pCurrentProcess = dynamic_cast<CItemBagProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
			assert(pCurrentProcess);
			pCurrentProcess->m_nEntry = pBody->m_nEntry;

			pCurrentProcess->FUNCTION_InitEntry();
		}

		if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
		{
			CInvenDialog::GetPtr()->FUNCTION_InitEntryInven(pBody->m_nEntry);
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
// 피트인 (CYG)
void GetPitIn(CHeadPacket* pPacket)
{
	CSCPitIn* pBody					= (CSCPitIn*)pPacket;
	int	nMsgCode					= (pBody->m_nCommand*100) - pBody->m_nResponse;
	VectorAnyoneList* pAnyoneList	= CObjPlayerManager::GetPtr()->GetAnyoneList(PLAYER_TEAM_TOTAL);
	CObjRoomInfo* pRoom				= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			int nCount = 0;

			VectorAnyoneList::iterator it;
			VectorPlayerList::iterator itp;
			for(it = pAnyoneList->begin();it != pAnyoneList->end();it++)
			{
				for(itp = (*it)->begin();itp != (*it)->end();itp++)
				{
					if( (*itp)->m_bPit == ROOM_PIT_IN )
						nCount++;

					if( (*itp)->GetObjSeq() == pBody->m_nPlayerSeq )
					{
						if( pBody->m_bAdminReq == true )
						{
							SN3UIUserFunctionParameter cParam, cParam2;
							CClientUIManager::GetPtr()->ProcUserFunction( "INVEN_CANCEL", &cParam );
							cParam2.AddParam( "Window", "RRInven" );
							CClientUIManager::GetPtr()->ProcUserFunction( "HideWindow", &cParam2 );
						}
						else
						{
							if( CProcessManager::GetPtr()->Get_CurrentProcess()->IsCurrentProcess(PROCESS_READYROOM) )
							{
								(*itp)->m_bPit = pBody->m_bInOut;
								CReadyRoomProcess* pCurrentProcess = dynamic_cast<CReadyRoomProcess*>(CProcessManager::GetPtr()->Get_CurrentProcess());
								assert(pCurrentProcess);
								pCurrentProcess->FUNCTION_PitInUpdate();
							}
						}
					}
				}
			}

			if( pRoom->IsMeRoomJang() && nCount && CPacketManager::GetPtr()->m_bIsGameCountDone )
			{
                pRoom->m_nCountDown = 5;
				pRoom->m_nCountTime = timeGetTime();
				PutGameCount(pRoom->m_nCountDown);
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//--SOURCE_AUTO_INSERT_SCRIPT

void GetRaiseFaculty(CHeadPacket* pPacket)
{
	CSCRaiseFaculty*	pBody		= (CSCRaiseFaculty*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pPlayer		= dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(pPlayer);

	switch(pBody->m_nResponse)
	{
	case 0:
		pPlayer->m_cLevel			= pBody->m_cLevel;
		pPlayer->m_cRaiseFaculty	= pBody->m_cRaiseFaculty;

		CPacketManager::GetPtr()->m_bIsRaiseFacultyDone = true;
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -11: //특성치 수치에 문제가 발생하였습니다.
	case -12: //보너스 특성치가 부족합니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetChangeSetting(CHeadPacket* pPacket)
{
	CSCChangeSetting*	pBody		= (CSCChangeSetting*)pPacket;
	int					nMsgCode	= (pBody->m_nCommand*100) - pBody->m_nResponse;
	CObjPlayerInfo*		pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	switch(pBody->m_nResponse)
	{
	case 0:
		{
			if( pBody->m_nInitSetting == 1 )
			{
				switch(CGameOptionDialog::GetPtr()->m_nOptionTabIndex%600)
				{
				case 0:
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting = CGameOptionDialog::GetPtr()->m_pObjPlayerInfo->m_cSetting;

					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nCameraType = pBody->m_cSetting.m_nCameraType;
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nCameraTarget = pBody->m_cSetting.m_nCameraTarget;
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nCameraTeam = pBody->m_cSetting.m_nCameraTeam;
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nCameraZoom = pBody->m_cSetting.m_nCameraZoom;

					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nLabel = pBody->m_cSetting.m_nLabel;
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nWhisper = pBody->m_cSetting.m_nWhisper;
					CGameOptionDialog::GetPtr()->m_pSetting.m_cSetting.m_nInvite = pBody->m_cSetting.m_nInvite;

					CGameOptionDialog::GetPtr()->SetSoundVol(EFFECTVOLUME);
					CGameOptionDialog::GetPtr()->SetMusicVol(MUSICVOLUME);
					CGameOptionDialog::GetPtr()->FUNCTION_GameConfigInit();
					break;
				case 1:
					CGameOptionDialog::GetPtr()->FUNCTION_ScreenConfigInit();
					break;
				case 2:
					memcpy(CGameOptionDialog::GetPtr()->m_nDefineKey, pBody->m_cSetting.m_nDefineKey, sizeof(pBody->m_cSetting.m_nDefineKey)) ;
					memcpy(CGameOptionDialog::GetPtr()->m_nAttackSkillCode, pBody->m_cSetting.m_nAttackSkillCode, sizeof(pBody->m_cSetting.m_nAttackSkillCode)) ;
					memcpy(CGameOptionDialog::GetPtr()->m_nDefenceSkillCode, pBody->m_cSetting.m_nDefenceSkillCode, sizeof(pBody->m_cSetting.m_nDefenceSkillCode)) ;
	
					CGameOptionDialog::GetPtr()->FUNCTION_KeyInit();
					break;
				case 3:
					CGameOptionDialog::GetPtr()->BGM_Choice(false);
					break;
				case 4:
					CGameOptionDialog::GetPtr()->FUNCTION_QuickChattingEmpty();
					break;
				case 5:
					break;
				}
			}
			else
			{
				if( pObjPlayerInfo == NULL )
				{
					CGameOptionDialog::GetPtr()->m_pObjPlayerInfo->m_cSetting = pBody->m_cSetting;
					CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(CGameOptionDialog::GetPtr()->m_pObjPlayerInfo);
				}
				else
				{
					pObjPlayerInfo->m_cSetting = pBody->m_cSetting;
					CGameOptionDialog::GetPtr()->FUNCTION_GameOptionInit(pObjPlayerInfo);
				}
			}
		}
		break;
	case -1: //시스템에 문제가 발생하였습니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, 100011, CMessageBoxDialog::MSGBOXTYPE_CONFIRM, nMsgCode);
		break;
	case -11: //특성치 수치에 문제가 발생하였습니다.
	case -12: //보너스 특성치가 부족합니다.
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	default:
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_EXIT, nMsgCode, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
// P2P Protocol
//////////////////////////////////////////////////////////////////////////
void GetObjectAction(CHeadPacket* pPacket)
{
	CPPObjectAction*	pBody		= (CPPObjectAction*)pPacket;
	CObjPlayerInfo*		pAnyPlayer;
	CObjBallInfo*		pBall		= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	//공 관련 데이터 받기
	pBall->m_nState			= pBody->m_cBallAction.m_nState;
	pBall->m_nBallerSeq		= pBody->m_cBallAction.m_nBallerSeq;
	pBall->m_nNoMarkSeq		= pBody->m_cBallAction.m_nNoMarkSeq;
	pBall->m_fOriginTime	= pBody->m_cBallAction.m_fOriginTime;
	pBall->m_cOriginPoint	= ConvertLocationToPoint(&pBody->m_cBallAction.m_cOriginLocation);
	pBall->m_cCurrentPoint	= ConvertLocationToPoint(&pBody->m_cBallAction.m_cCurrentLocation);
	pBall->m_cOriginVector	= pBody->m_cBallAction.m_cOriginVector;
	pRoom->m_fPlayTime		= (float)pBody->m_cBallAction.m_nPlayTime / 100.0f;
	pRoom->m_fCurrentTime	= pBody->m_cBallAction.m_fCurrentTime;


	//선수 관련 데이터 받기
	for(int i=0;i<(pBody->m_nAmount);++i)
	{
		pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_cPlayerAction[i].m_nPlayerSeq);
		if(pAnyPlayer == NULL)		continue;

		pAnyPlayer->m_cCurrentAction.m_nActionSeq	= pBody->m_cPlayerAction[i].m_nActionSeq;
		pAnyPlayer->m_cCurrentAction.m_nAniCode		= pBody->m_cPlayerAction[i].m_nAniCode;
		pAnyPlayer->m_cCurrentAction.m_fStartAngle	= pBody->m_cPlayerAction[i].m_fStartAngle;
		pAnyPlayer->m_cCurrentAction.m_fEndAngle	= pBody->m_cPlayerAction[i].m_fEndAngle;
		pAnyPlayer->m_cCurrentAction.m_nAniSpeed	= pBody->m_cPlayerAction[i].m_nAniSpeed;
		pAnyPlayer->m_cCurrentAction.m_nMoveSpeed	= pBody->m_cPlayerAction[i].m_nMoveSpeed;
		pAnyPlayer->m_cCurrentAction.m_nSkillType	= pBody->m_cPlayerAction[i].m_nSkillType;
		pAnyPlayer->m_cCurrentAction.m_nHeadCode	= pBody->m_cPlayerAction[i].m_nHeadCode;

		pAnyPlayer->m_cNodePoint					= ConvertLocationToPoint(&pBody->m_cPlayerAction[i].m_cNodeLocation);
		pAnyPlayer->m_cTargetPoint					= ConvertLocationToPoint(&pBody->m_cPlayerAction[i].m_cTargetLocation);
	}
}
//////////////////////////////////////////////////////////////////////////
void GetObjectAction_Replay(CHeadPacket* pPacket)
{
	CPPObjectAction*	pBody		= (CPPObjectAction*)pPacket;
	CObjPlayerInfo*		pAnyPlayer;
	CObjBallInfo*		pBall		= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	//공 관련 데이터 받기
	pBall->m_nState			= pBody->m_cBallAction.m_nState;
	pBall->m_nBallerSeq		= pBody->m_cBallAction.m_nBallerSeq;
	pBall->m_nNoMarkSeq		= pBody->m_cBallAction.m_nNoMarkSeq;
	pBall->m_fOriginTime	= pBody->m_cBallAction.m_fOriginTime;
	pBall->m_cOriginPoint	= ConvertLocationToPoint(&pBody->m_cBallAction.m_cOriginLocation);
	pBall->m_cCurrentPoint	= ConvertLocationToPoint(&pBody->m_cBallAction.m_cCurrentLocation);
	pBall->m_cOriginVector	= pBody->m_cBallAction.m_cOriginVector;
	pBall->m_nSkillType		= pBody->m_cBallAction.m_nSkillType;
	pRoom->m_fPlayTime		= (float)pBody->m_cBallAction.m_nPlayTime / 100.0f;
	pRoom->m_fCurrentTime	= pBody->m_cBallAction.m_fCurrentTime;

	//선수 관련 데이터 받기
	for(int i=0;i<(pBody->m_nAmount);++i)
	{
		pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_cPlayerAction[i].m_nPlayerSeq);
		if(pAnyPlayer == NULL)		continue;

		pAnyPlayer->m_cCurrentAction.m_nActionSeq	= pBody->m_cPlayerAction[i].m_nActionSeq;
		pAnyPlayer->m_cCurrentAction.m_nAniCode		= pBody->m_cPlayerAction[i].m_nAniCode;
		pAnyPlayer->m_cCurrentAction.m_fStartAngle	= pBody->m_cPlayerAction[i].m_fStartAngle;
		pAnyPlayer->m_cCurrentAction.m_fEndAngle	= pBody->m_cPlayerAction[i].m_fEndAngle;
		pAnyPlayer->m_cCurrentAction.m_nAniSpeed	= pBody->m_cPlayerAction[i].m_nAniSpeed;
		pAnyPlayer->m_cCurrentAction.m_nMoveSpeed	= pBody->m_cPlayerAction[i].m_nMoveSpeed;
		pAnyPlayer->m_cCurrentAction.m_nSkillType	= pBody->m_cPlayerAction[i].m_nSkillType;
		pAnyPlayer->m_cCurrentAction.m_nHeadCode	= pBody->m_cPlayerAction[i].m_nHeadCode;

		pAnyPlayer->m_cNodePoint					= ConvertLocationToPoint(&pBody->m_cPlayerAction[i].m_cNodeLocation);
		pAnyPlayer->m_cTargetPoint					= ConvertLocationToPoint(&pBody->m_cPlayerAction[i].m_cTargetLocation);
		pAnyPlayer->m_bReplayReady					= true;
	}
}
//////////////////////////////////////////////////////////////////////////
void GetBallAction(CHeadPacket* pPacket)
{
	CPPBallAction*		pBody	= (CPPBallAction*)pPacket;
	CObjBallInfo*		pBall	= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();

	pBall->m_nState				= pBody->m_nState;
	pBall->m_nBallerSeq			= pBody->m_nBallerSeq;
	pBall->m_fOriginTime		= pBody->m_fOriginTime;
	pBall->m_cOriginPoint		= ConvertLocationToPoint(&pBody->m_cOriginLocation);
	pBall->m_cCurrentPoint		= ConvertLocationToPoint(&pBody->m_cCurrentLocation);
	pBall->m_cOriginVector		= pBody->m_cOriginVector;
	pBall->m_nSkillType			= pBody->m_nSkillType;
	pBall->m_nBallSound			= pBody->m_nBallSound;
    pBall->m_nBallEffect        = pBody->m_nBallEffect;
}

void GetBallAction_Replay(CHeadPacket* pPacket, float fTime) //리플레이용
{
	CPPBallAction*		pBody	= (CPPBallAction*)pPacket;
	CObjBallInfo*		pBall	= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();

	pBall->m_nState				= pBody->m_nState;
	pBall->m_nBallerSeq			= pBody->m_nBallerSeq;
	pBall->m_fOriginTime		= pBody->m_fOriginTime + fTime;
	pBall->m_cOriginPoint		= ConvertLocationToPoint(&pBody->m_cOriginLocation);
	pBall->m_cCurrentPoint		= ConvertLocationToPoint(&pBody->m_cCurrentLocation);
	pBall->m_cOriginVector		= pBody->m_cOriginVector;
	pBall->m_nSkillType			= pBody->m_nSkillType;
	pBall->m_nBallSound			= pBody->m_nBallSound;
	pBall->m_nBallEffect        = pBody->m_nBallEffect;
	printf("origin time(%f)\n", pBall->m_fOriginTime);
}
//////////////////////////////////////////////////////////////////////////
void GetObjectLocation(CHeadPacket* pPacket)
{
	CPPObjectLocation*	pBody		= (CPPObjectLocation*)pPacket;
	CObjPlayerInfo*		pAnyPlayer;

	//선수 관련 데이터 받기
	for(int i=0;i<(pBody->m_nAmount);++i)
	{
		pAnyPlayer = CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_cPlayerLocation[i].m_nPlayerSeq);
		if(pAnyPlayer == NULL)		continue;

//		pAnyPlayer->m_cCurrentPoint.x			= (float)(pBody->m_cPlayerLocation[i].m_nX / 100.0f);
//		pAnyPlayer->m_cCurrentPoint.y			= (float)(pBody->m_cPlayerLocation[i].m_nY / 100.0f);

		pAnyPlayer->m_cNodePoint.x			= (float)(pBody->m_cPlayerLocation[i].m_nX / 100.0f);
		pAnyPlayer->m_cNodePoint.y			= (float)(pBody->m_cPlayerLocation[i].m_nY / 100.0f);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
void GetGoalIn(CHeadPacket* pPacket)
{
	CPPGoalIn*			pBody	= (CPPGoalIn*)pPacket;
	CObjRoomInfo*		pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjBallInfo*		pBall	= CObjBallManager::GetPtr()->GetBallPointer();

	pBall->m_bIsGoalIn									= true;

	pBall->m_nGoalType									= pBody->m_nGoalType;
	pBall->m_nGoalTeam									= pBody->m_nGoalTeam;
	pBall->m_nShootKind									= pBody->m_nShootKind;
	pRoom->m_nCeremonyCode								= pBody->m_nCeremonyCode;
	pBall->m_nKickSeq									= pBody->m_nGoalSeq;
	pBall->m_nAssistSeq									= pBody->m_nAssistSeq;
	pRoom->m_nGoalerScore								= pBody->m_nGoalerScore;
	pRoom->m_nAssisterScore								= pBody->m_nAssisterScore;
	pRoom->m_fCurrentTime								= pBody->m_fCurrentTime;
	pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]	= pBody->m_nHomeScore;
	pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]	= pBody->m_nAwayScore;
}
//////////////////////////////////////////////////////////////////////////
void GetGoalIn_Replay(CHeadPacket* pPacket)
{
	CPPGoalIn*			pBody	= (CPPGoalIn*)pPacket;
	CObjRoomInfo*		pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();
	CObjBallInfo*		pBall	= CObjBallManager::GetPtr()->GetBallPointer();

	
	/*
	pBall->m_bIsGoalIn									= true;

	pBall->m_nGoalType									= pBody->m_nGoalType;
	pBall->m_nGoalTeam									= pBody->m_nGoalTeam;
	pBall->m_nShootKind									= pBody->m_nShootKind;
	pRoom->m_nCeremonyCode								= pBody->m_nCeremonyCode;
	pBall->m_nKickSeq									= pBody->m_nGoalSeq;
	pBall->m_nAssistSeq									= pBody->m_nAssistSeq;
	pRoom->m_nGoalerScore								= pBody->m_nGoalerScore;
	pRoom->m_nAssisterScore								= pBody->m_nAssisterScore;
	pRoom->m_fCurrentTime								= pBody->m_fCurrentTime;
	*/
	pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL]	= pBody->m_nHomeScore;
	pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL]	= pBody->m_nAwayScore;
}
//////////////////////////////////////////////////////////////////////////
void GetInputKey(CHeadPacket* pPacket)
{
	CPPInputKey*		pBody	= (CPPInputKey*)pPacket;
	CObjPlayerInfo*		pPlayer	= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_nPlayerSeq);

	if(pPlayer == NULL)		return;

	pPlayer->m_cInputKey = pBody->m_cInputKey;
}
//////////////////////////////////////////////////////////////////////////
void GetEmotionInfo(CHeadPacket* pPacket)
{
	CPPEmotionInfo*		pBody	= (CPPEmotionInfo*)pPacket;
	CObjPlayerInfo*		pPlayer	= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_nPlayerSeq);

	if(pPlayer == NULL)		return;

	pPlayer->m_bGetUDP	= true;
	pPlayer->m_bPutUDP	= pBody->m_bCheckUDP; //상대방이 내것을 받았다는 표시
}
//////////////////////////////////////////////////////////////////////////
void GetAngerInfo(CHeadPacket* pPacket)
{
	CPPAngerInfo*		pBody	= (CPPAngerInfo*)pPacket;
	CObjRoomInfo*		pRoom	= CObjRoomManager::GetPtr()->GetRoomPointer();

	pRoom->m_nHomeAngerTime	= pBody->m_nHomeAngerTime;
	pRoom->m_nAwayAngerTime	= pBody->m_nAwayAngerTime;
	pRoom->m_fTotalAngerTime = 0;
}
//////////////////////////////////////////////////////////////////////////
void GetUDPPing(CHeadPacket* pPacket)
{
	CSCUDPPing*		pBody	= (CSCUDPPing*)pPacket;
	//printf( "UDP PING...........\n" );

	//	CObjPlayerInfo*		pPlayer	= CObjPlayerManager::GetPtr()->GetPlayerPointer(PLAYER_TEAM_ATHLETE, pBody->m_nPlayerSeq);

	//	if(pPlayer == NULL)		return;

	//	pPlayer->m_cInputKey = pBody->m_cInputKey;
}


