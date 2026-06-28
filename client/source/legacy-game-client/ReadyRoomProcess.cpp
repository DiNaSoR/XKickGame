#include "stdafx.h"
#include "ReadyRoomProcess.h"
#include "UICommonMethod.h"
#include "GameResultProcess.h"
#include "GamePlayProcess.h"
#include "GameLoadingProcess.h"
#include "ReplayLoadingProcess.h"
#include "QuestLoadingProcess.h"
#include "ChoiceRoomProcess.h"
#include "PutPacket.h"
#include "GetPacket.h"
#include "PlayModeManager.h"
#include "CoreApi.h"
#include "SignBoard.h"
#include "CoreApi.h"

CReadyRoomProcess::CReadyRoomProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_READYROOM;
    m_pGroundInfo = 0;
    m_pkCasterNode = 0;
}

CReadyRoomProcess::~CReadyRoomProcess(void)
{
    m_pGroundInfo = 0;
}

void CReadyRoomProcess::Terminate()
{
	CObjPlayerManager::GetPtr()->ErasePlayerModelPoint();

	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CReadyRoomProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	m_pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	assert(m_pObjPlayerInfo);

	CChattingDialog::GetPtr()->FUNCTION_ChattingListSort(0);

	UICommonMethod::DisplayAll(m_pObjPlayerInfo);
    FUNCTION_3DRender( "ReadyRoomMain", false );

	FUNCTION_TeamChangeVisible(N3UI_VISIBLE_TRUE);

    CResGroundFormat * pGroundFormat = CObjectModelManager::GetPtr()->GetGround();
    if( pGroundFormat != 0 )
    {
        //CoreApi_RecursiveSetAlphaPropertyToMesh( pGroundFormat->m_spNode, eFalse, eNone );
        //pGroundFormat->m_spNode->UpdateProperties();
    }

	m_nState = STATE_START;
}

void CReadyRoomProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
    CPlayerInformationDialog::GetPtr()->Update( fFrameTime, fAccumTime );
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);
	CInvitationDialog::GetPtr()->Update();
    
//	if(m_pObjGroundInfo && m_pObjGroundInfo->m_pModel)
	{
		CGameProject::s_pGameProject->BeginFrame();

        if( m_pkCasterNode != 0 ) m_pkCasterNode->SetAppCulled( false );
        CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
		assert(pObjGroundInfo);

        if( m_pkCasterNode != 0 ) m_pkCasterNode->SetAppCulled( false );

        pObjGroundInfo->m_pModel->BeforeRender();

		CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);

        if( m_pGroundInfo != 0 ) m_pGroundInfo->m_pModel->m_spNode->Update( fAccumTime );        
        if( m_pkCasterNode != 0 ) m_pkCasterNode->SetAppCulled( true );

		m_cCharacterTexBoard.Render();        
        
        if( m_pkCasterNode != 0 )
        {        
            m_pkCasterNode->SetAppCulled( false );

            NiCamera            * pkCamera            = m_cCharacterTexBoard.GetCamera();
            NiRenderTargetGroup * pkRenderTargetGroup = m_cCharacterTexBoard.m_spRenderTargetGroup;
            XKickAccumulator    * pkAccumulator       = m_cCharacterTexBoard.m_spAccumulator;

            CoreApi_AlphaBlendObjectRender( m_pkCasterNode, pkCamera, pkRenderTargetGroup, pkAccumulator );
        }

        m_cCharacterTexBoard.m_pkRenderedTex->MakeTexture();

		CGameProject::s_pGameProject->EndFrame();
		CGameProject::s_pGameProject->DisplayFrame();
	}

	ViewOverCheck();
	UICommonMethod::BasicInfoStop();
	UICommonMethod::DisPlayNotice();
	UICommonMethod::DisplayMenuToolTip();
}

void CReadyRoomProcess::UpdateState(float fFrameTime, float fAccumTime)
{
	switch(m_nState)
	{
	case STATE_START:
		{
			if(CProcessManager::GetPtr()->m_nBeforeProcessCmd == PROCESS_GAMERESULT)
			{
				CResOggTableFormat *temp = dynamic_cast<CResOggTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OGGTABLE, 0 ,0));
				assert(temp);

				if(strlen(CGameOptionDialog::GetPtr()->LoadUserBGM(0)))
				{
					CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\Music\\");
					CAudioManager::GetPtr()->CreateMusic(CGameOptionDialog::GetPtr()->LoadUserBGM(0), true);//사운드 플레이
					CAudioManager::GetPtr()->SetAudioPath(".\\Sound\\", ".\\BGM\\");
				}
				else
					CAudioManager::GetPtr()->CreateMusic(temp->m_sDataFile, true);//사운드 플레이
			}

			m_nState = STATE_INIT_LOAD;
		}
		break;
	case STATE_INIT_LOAD:
		{
			//PutRoomInfo();

			m_nState = STATE_GROUND_LOAD;
		}
		break;
	case STATE_GROUND_LOAD:
		{
			m_nUDPTime = 0;

			CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
			if(pRoom == NULL) return;

			pRoom->m_nCountDown		= 0;
			pRoom->m_nCountTime		= 0;

			SetUIReadyRoomPositionSetting();               

            CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
            assert(pObjGroundInfo);

			CCameraController::GetPtr()->SetCamera(pObjGroundInfo->GetCamera());

            NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->GetObjectByName( "CasterNode" );
            pObjGroundInfo->m_pModel->SetCasterNode( pkCasterNode );

            pObjGroundInfo->m_pModel->m_spNode->UpdateProperties();
            pObjGroundInfo->m_pModel->m_spNode->UpdateEffects();
            pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );

            m_pkCasterNode = pkCasterNode;

			m_nState = STATE_CHARACTER_LOAD;
		}
		break;
	case STATE_CHARACTER_LOAD:
		{
			if(CPacketManager::GetPtr()->m_bIsAthleteInfoDone)
			{
				CPacketManager::GetPtr()->m_bIsAthleteInfoDone = false;

				LoadCharacter();

                NiIntegerExtraData kShadowExtraData(1);
                kShadowExtraData.SetName( "Shadow" );

                CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
				assert(pObjGroundInfo);

                NiNode * pkCasterNode = (NiNode*) pObjGroundInfo->m_pModel->m_spNode->GetObjectByName( "CasterNode" );
                CoreApi_RecursiveSetExtraDataToGeometry( pkCasterNode, &kShadowExtraData );    

                pObjGroundInfo->m_pModel->m_spNode->Update( 0.0f );

				m_bIsRender = true;
				SetUIReadyRoomAthleteSetting();
				Set_ReadyRoomUI();
                SetGameModeSetting();
                
				CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
                m_pGroundInfo->m_pModel->m_spNode->Update( fFrameTime );
			}

			CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

			//if(pRoom->m_nMode == ROOM_MODE_NORMAL)
			{
				//방안에 있는 사람들끼리 UDP가 되는지 확인하는 루틴(감정표현으로도 겸용해서 사용)
				if(timeGetTime() - m_nUDPTime > 1000)
				{
					m_nUDPTime = timeGetTime();
					PutEmotionInfo(0);
				}
			}

			if(CPacketManager::GetPtr()->m_bIsGameReadyDone)
			{
				CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
				if(pRoom == NULL) return;

				if(pRoom->IsMeRoomJang()) //내가 방장이라면
				{
					if(timeGetTime() - pRoom->m_nCountTime > 2000)
					{
						pRoom->m_nCountTime = timeGetTime();
						PutGameReady(PLAYER_READY_REQUESTION);
					}
				}
			}

			if(CPacketManager::GetPtr()->m_bIsGameCountDone)
			{
				CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
				if(pRoom == NULL) return;

				if(pRoom->m_nCountDown > 0)
				{
					if(pRoom->IsMeRoomJang()) //내가 방장이라면
					{
						if(timeGetTime() - pRoom->m_nCountTime > 1000)
						{
							//CPacketManager::GetPtr()->m_bIsGameCountDone = false;
							pRoom->m_nCountTime = timeGetTime();
							PutGameCount(--pRoom->m_nCountDown);
						}
					}
				}
				else
				{
					CPacketManager::GetPtr()->m_bIsGameCountDone = false;
					m_nState = STATE_END;
					CObjPlayerManager::GetPtr()->DisplayRelayState();
				}
			}
		}
		break;
	case STATE_END:
		{
			CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

			CObjPlayerManager::GetPtr()->EraseLoadingStepPlayer();

			if(pRoom->m_nMode == ROOM_MODE_NORMAL || pRoom->m_nMode == ROOM_MODE_BET || pRoom->m_nMode == ROOM_MODE_SINGLE)
				CProcessManager::GetPtr()->SetChangeProcess(new CGameLoadingProcess);
			else if(pRoom->m_nMode == ROOM_MODE_LADDER)
				CProcessManager::GetPtr()->SetChangeProcess(new CGameLoadingProcess);
			else if(pRoom->m_nMode == ROOM_MODE_QUEST)
			{
				char sText[STRLEN_64]="";
				sprintf(sText,".\\Script\\Quest%d.xml",pRoom->m_nQuestCode);
				CProcessManager::GetPtr()->SetChangeProcess(new CQuestLoadingProcess(sText, pRoom->m_nQuestCode));
			}
		}
		break;
	}
}


bool CReadyRoomProcess::OnLostDevice()
{
	m_cCharacterTexBoard.OnLostDevice();
	return true;
}

bool CReadyRoomProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.OnResetDevice();
	return true;
}

bool CReadyRoomProcess::OnDefault(NiEventRef pEventRecord)
{
	MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	if( CClientUIManager::GetPtr()->FindEventControl( "RRInven" )->GetVisible() == N3UI_VISIBLE_TRUE )
		CInvenDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	if( !strcmp(CUIControl::Get_FocusedPage()->m_strName, "Page") )
		CChattingDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CReadyRoomProcess::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		m_pDNDPoint.x = LOWORD(lParam);
		m_pDNDPoint.y = HIWORD(lParam);
		FUNCTION_MapNameToolTipUpdate();
		break;
	}
}

void CReadyRoomProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CObjectModelManager::GetPtr()->Render();
		CClientUIManager::GetPtr()->OnRender();

		CClientUIManager::GetPtr()->m_pStateBlock->Capture();
		CClientUIManager::GetPtr()->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

		CUIControl* pPage = CClientUIManager::GetPtr()->FindEventControl( "PlayerInformation" );
		assert(pPage);

		if( pPage->GetVisible() == N3UI_VISIBLE_THISFALSE && CPlayerInformationDialog::GetPtr()->m_nSmallFractionIndex == 500 )
			CPlayerInformationDialog::GetPtr()->DrawGraph();

		CClientUIManager::GetPtr()->m_pSprite->End();
		CClientUIManager::GetPtr()->m_pStateBlock->Apply();
	}
}

CBaseProcess* CReadyRoomProcess::GetSameProcess()
{
	return new CReadyRoomProcess;
}

void CReadyRoomProcess::LoadCharacter()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

    m_pGroundInfo = pObjGroundInfo;

	CResGroundFormat *pGround = dynamic_cast<CResGroundFormat*>(pObjGroundInfo->m_pModel);
	assert(pGround);

	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList	= &(CObjPlayerManager::GetPtr()->m_vTotalList);

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;
			if(pAnyPlayer->m_pModel)	continue;

			CPlayModeManager::GetPtr()->SetupCharacterLoading(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nSeat);

			pAnyPlayer->SetScale();

			if(pAnyPlayer->GetKind() == PLAYER_KIND_CARDBOT)
				CProcessManager::GetPtr()->AttachItem(pAnyPlayer, 0.6f);
			else CProcessManager::GetPtr()->AttachItem(pAnyPlayer);
		}
	}
}