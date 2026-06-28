#include "stdafx.h"
#include "GameLoginProcess.h"
#include "PutPacket.h"
#include "ChoiceTrioProcess.h"
#include "UIEditBox.h"
#include "UICommonMethod.h"

CGameLoginProcess::CGameLoginProcess() : CBaseProcess()
{
	m_nProcessCmd = PROCESS_GAMELOGIN;

	m_bIsRender = true;
}

CGameLoginProcess::~CGameLoginProcess(void)
{
}

void CGameLoginProcess::Terminate()
{
	CObjectModelManager::GetPtr()->EraseObject();

	CBaseProcess::Terminate();
}

void CGameLoginProcess::RenderFrame()
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

void CGameLoginProcess::InitProcess()
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);

	CPlayModeManager::GetPtr()->Init(pObjGroundInfo, m_nProcessCmd*10000);//스크립트 로딩후 모든 준비 완료

	CObjectManager::GetPtr()->DeleteAllObject(OBJECT_TRIOLIST);
	
	CUIEditBox* pPassControl = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("Login_Pass"));
	assert(pPassControl);
	pPassControl->ClearText();

	CUIEditBox* pIdControl = dynamic_cast<CUIEditBox*>(CClientUIManager::GetPtr()->FindEventControl("Login_ID"));
	assert(pIdControl);
	pIdControl->ClearText();

	UICommonMethod::DisplayGameVersion();

	CCameraController::GetPtr()->ChangeCamera();

	//m_cWebPage.Init(CGameProject::s_pGameProject->GetWindowReference(), CGameProject::s_pGameProject->GetInstanceReference(), "http://www.daum.net/", 524, 0, 500, 768);

	m_nState = STATE_START;
}

void CGameLoginProcess::UpdateProcess(float fFrameTime, float fAccumTime)
{
	CObjectModelManager::GetPtr()->Update(fFrameTime, fAccumTime);
	CClientUIManager::GetPtr()->OnUpdate(fFrameTime, fAccumTime);

	//m_cWebPage.Update(1);
}

void CGameLoginProcess::UpdateState(float fFrameTime, float fAccumTime)
{
    switch(m_nState)
    {
    case STATE_START:
        {
            m_nState = STATE_GROUND_LOAD;
        }
        break;
	case STATE_GROUND_LOAD:
		{		
			LoadGround();

			m_nState = STATE_INIT_LOAD;
		}
		break;
    case STATE_INIT_LOAD:
        {
            if(CPacketManager::GetPtr()->m_bIsCertifyLoginDone)//서버에서 패킷을 다받았으면
            {
                CPacketManager::GetPtr()->m_bIsCertifyLoginDone = false;
                PutMemberInfo();
            }

            if(CPacketManager::GetPtr()->m_bIsMemberInfoDone)
            {
                CPacketManager::GetPtr()->m_bIsMemberInfoDone = false;

                m_nState = STATE_END;                
            }
        }
        break;
    case STATE_END:
        {
            CProcessManager::GetPtr()->SetChangeProcess(new CChoiceTrioProcess);
        }
        break;
    }
}

void CGameLoginProcess::LoadGround()
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