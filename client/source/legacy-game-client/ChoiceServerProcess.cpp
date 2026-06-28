#include "stdafx.h"
#include "ChoiceServerProcess.h"
#include "UICommonMethod.h"
#include "ChoiceRoomProcess.h"
#include "ObjServerInfo.h"
#include "PutPacket.h"
#include "PlayModeManager.h"
#include "UIProgress.h"
#include "UIFunction.h"
#include "TutorialMainProcess.h"

CChoiceServerProcess::CChoiceServerProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_CHOICESERVER;
	
	m_nServerTabIndex	= 0;
	m_nSelectServer		= -1;

	m_bUp = false;
}

CChoiceServerProcess::CChoiceServerProcess(bool bUp) : CBaseProcess()
{
	m_nProcessCmd = PROCESS_CHOICESERVER;
	
	m_nServerTabIndex	= 0;
	m_nSelectServer		= -1;

	m_bUp = bUp;
}

CChoiceServerProcess::~CChoiceServerProcess(void)
{
}

void CChoiceServerProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CChoiceServerProcess::LoadGround()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

	NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );

	if(pkCasterNode) pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

	pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
	pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
	pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );
}

void CChoiceServerProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

    m_pObjPlayerInfo->ResetSkill();
    m_pObjPlayerInfo->ResetTraining();
    m_pObjPlayerInfo->ResetCeremony();

	//IsChoiceClass();

	FUNCTION_Init();
	
	CObjectManager::GetPtr()->Terminate(OBJECT_SERVERLIST);

	m_nState = STATE_START;
}

void CChoiceServerProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);

	char sText[MAX_PATH]="";
	
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	
	UICommonMethod::BasicInfoStop();
	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
}

void CChoiceServerProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			m_nState = STATE_INIT_LOAD;
		}
		break;
	case STATE_INIT_LOAD:
		{
			PutServerList(1);

			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{
			LoadGround();

			m_bIsRender = true;

			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
			m_nState = STATE_PLAY;
		}
		break;
	case STATE_PLAY:
		{
			if(CPacketManager::GetPtr()->m_bIsPlayerInfoDone)
			{
				CPacketManager::GetPtr()->m_bIsPlayerInfoDone = false;

				m_nState = STATE_END;
			}
		}
		break;
	case STATE_END:
		{
			if( m_pObjPlayerInfo->m_nPosition == POSITION_NB )
				CProcessManager::GetPtr()->SetChangeProcess(new CTutorialMainProcess);
			else
				CProcessManager::GetPtr()->SetChangeProcess(new CChoiceRoomProcess);

			CPacketManager::GetPtr()->m_bIsCertifyToGame = false;
		}
		break;
	}
}

void CChoiceServerProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();
	}
}

bool CChoiceServerProcess::OnDefault(NiEventRef pEventRecord)
{
	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") && m_nAutoSelectServer )
        MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);	

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

CBaseProcess* CChoiceServerProcess::GetSameProcess()
{
	return new CChoiceServerProcess;
}

void CChoiceServerProcess::FUNCTION_Init()
{
	// 빠른입장 서버 선택
	if( m_pObjPlayerInfo->m_nPosition == POSITION_NB ) 
	{
		m_nAutoSelectServer = 0;
		
		UIFunction::Set_UIVisibility( "JoinBtn00", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "JoinBtn01", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "JoinBtn10", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "ServerCover", N3UI_VISIBLE_TRUE );

		CObjMemberInfo*		pMember		= CObjMemberManager::GetPtr()->GetMemberPointer();
		
		if( !IsBit(pMember->m_nQuest, 0) )
		{
			UIFunction::Set_UIVisibility( "NpcGuide", N3UI_VISIBLE_TRUE );
			CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "CSExplainBG" );
			assert(pBG);

			UIFunction::ChangeChildTexture( pBG, "GT_Static", "CSMent00"  );
			CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
			assert(pShadowLine);
		
			pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
			pShadowLine->SetInitUpdate(false);
			pShadowLine->SetPlay(true, false, false, 0);
		}

		if( m_bUp )
		{
            UIFunction::Set_UIVisibility( "NpcGuide", N3UI_VISIBLE_TRUE );
			CUIControl* pBG = CClientUIManager::GetPtr()->FindEventControl( "CSExplainBG" );
			assert(pBG);

			UIFunction::ChangeChildTexture( pBG, "GT_Static", "CSMent01"  );
			CUIControl* pShadowLine = pBG->FindEventControl( "ShadowLine" );
			assert(pShadowLine);
		
			pShadowLine->SetVisible(N3UI_VISIBLE_TRUE);
			pShadowLine->SetInitUpdate(false);
			pShadowLine->SetPlay(true, false, false, 0);
		}
	}
	else
	{
		m_nAutoSelectServer = 1;

		UIFunction::Set_UIVisibility( "JoinBtn00", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "JoinBtn01", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "JoinBtn10", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "ServerCover", N3UI_VISIBLE_FALSE );
	}    
}

void CChoiceServerProcess::FUNCTION_ChoiceServerListUpdateUI()
{
	char sText[MAX_PATH]="";

	for( int i=0;i<LIST6_SIZE;i++)
	{
		_snprintf( sText, MAX_PATH, "ServerList0%d", i);
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
	}

	switch( m_nServerTabIndex )
	{
	case 0:
		{
			for( int i=0;i<2;i++)
			{
				_snprintf( sText, MAX_PATH, "ServerList0%d", i);
				CUIControl* pServer = CClientUIManager::GetPtr()->FindEventControl( sText );
				assert(pServer);

				UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
				
				CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, i));
				assert(pServerInfo);

				float fServerMax = (float)pServerInfo->m_nMax;			// 서버 최대 인원
				float fServerNow = (float)pServerInfo->m_nCurrent;		// 서버 현재 인원

				switch( pServerInfo->m_nState )
				{
				case 1:
					UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon02" );
					break;
				case 2:
					if( (float)( fServerNow / fServerMax ) > 0.9 )
						UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon03" );
					else if( (float)( fServerNow / fServerMax ) > 0.5 )
						UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon00" );
					else
						UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon01" );
					break;
				}

				// 서버 코드
				_snprintf(sText, MAX_PATH, "%02d-%02d", pServerInfo->m_nServerCode/100, pServerInfo->m_nServerCode%100 );
				UIFunction::Set_ChildControlText( pServer, "ServerNumber", sText );
				
				// 서버 명
				UIFunction::Set_ChildControlText( pServer, "ServerName", pServerInfo->m_sTitle );
			}
		}
		break;
	case 1:
	//	{
	//		CUIControl* pServer = CClientUIManager::GetPtr()->FindEventControl( "ServerList00" );
	//		assert(pServer);

	//		UIFunction::Set_UIVisibility( "ServerList00", N3UI_VISIBLE_TRUE );
	//			
	//		CObjServerInfo* pServerInfo = dynamic_cast<CObjServerInfo*>(CObjectManager::GetPtr()->GetSequenceField(OBJECT_SERVERLIST, 2));
	//		assert(pServerInfo);

	//		float fServerMax = (float)pServerInfo->m_nMax;			// 서버 최대 인원
	//		float fServerNow = (float)pServerInfo->m_nCurrent;		// 서버 현재 인원

	//		switch( pServerInfo->m_nState )
	//		{
	//		case 1:
	//			UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon02" );
	//			break;
	//		case 2:
	//			if( (float)( fServerNow / fServerMax ) > 0.9 )
	//				UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon03" );
	//			else if( (float)( fServerNow / fServerMax ) > 0.5 )
	//				UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon00" );
	//			else
	//				UIFunction::ChangeChildTexture(pServer, "Server_State", "ServerIcon01" );
	//			break;
	//		}

	//		// 서버 코드
	//		_snprintf(sText, MAX_PATH, "%02d-%02d", pServerInfo->m_nServerCode/100, pServerInfo->m_nServerCode%100 );
	//		UIFunction::Set_ChildControlText( pServer, "ServerNumber", sText );
	//		
	//		// 서버 명
	//		UIFunction::Set_ChildControlText( pServer, "ServerName", pServerInfo->m_sTitle );
	//	}
		break;
	case 2:
		break;
	}
}

void CChoiceServerProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		FUNCTION_Selection();
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		FUNCTION_MouseOverCheck();
		break;
	}
}

void CChoiceServerProcess::FUNCTION_TabChange(char idx)
{
	m_nServerTabIndex = idx;

	char sText[STRLEN_64]="";

	for(int i=0;i<3;i++)
	{
        _snprintf(sText, STRLEN_64, "Channel0%d", i);
		CUIButton* pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( sText ));
		assert(pBtn);

		if( i == idx )
			pBtn->SetPressed(true);
		else
			pBtn->SetPressed(false);
	}

	FUNCTION_ChoiceServerListUpdateUI();
}

void CChoiceServerProcess::FUNCTION_MouseOverCheck()
{
	char sText[MAX_PATH]="";

	RECT rt;

	for( int i=0;i<LIST6_SIZE;i++)
	{
		_snprintf(sText, MAX_PATH, "ServerList0%d", i);
		CUIControl* pServer = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pServer);

		rt = pServer->GetWindowBox();

		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint))
			UIFunction::SetChildToVisibility( pServer, "Over_img", N3UI_VISIBLE_TRUE);
		else
			UIFunction::SetChildToVisibility( pServer, "Over_img", N3UI_VISIBLE_FALSE);
	}
}

void CChoiceServerProcess::FUNCTION_Selection()
{
	char sText[MAX_PATH]="";

	RECT rt;

	for( int i=0;i<LIST6_SIZE;i++)
	{
		_snprintf(sText, MAX_PATH, "ServerList0%d", i);
		CUIControl* pServer = CClientUIManager::GetPtr()->FindEventControl(sText);
		assert(pServer);

		rt = pServer->GetWindowBox();

		UIFunction::SetChildToVisibility( pServer, "Click_Img", N3UI_VISIBLE_FALSE);

		if(UICommonMethod::IsRectInPoint(&rt, &m_pDNDPoint))
		{
			m_nSelectServer = i;
			UIFunction::SetChildToVisibility( pServer, "Click_Img", N3UI_VISIBLE_TRUE);
		}
	}
}

void CChoiceServerProcess::IsChoiceClass()
{
	if( m_pObjPlayerInfo->m_nPosition == POSITION_NB && LIST5_SIZE <= m_pObjPlayerInfo->m_cLevel.m_nLevel )
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_CHOICECLASS, 20900, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
}