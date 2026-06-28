//***********************************************************************************
//	File		:	BaseProcess.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "stdafx.h"
#include ".\baseprocess.h"
#include "UICommonMethod.h"
#include "PlayModeManager.h"

CBaseProcess::CBaseProcess()
: sKeyWord_ToolTipListNum("ToolTipListNum"), sKeyWord_ToolTip("ToolTip")
{
	g_pFont = NULL;

	m_bIsRender = false;

	m_nState	= STATE_START;

	m_nProcessCmd = PROCESS_NONE;

    CObjectModelManager::GetPtr()->SetNeedMultiThreadInit( true );
}

CBaseProcess::~CBaseProcess(void)
{
	map<string, CUIPopUp*>::iterator pIndex=m_mPopUpList.begin();
	map<string, CUIPopUp*>::iterator pIndex_end=m_mPopUpList.end();

	while(pIndex!=pIndex_end)
	{
		delete (*pIndex).second;
		m_mPopUpList.erase( pIndex++);
	}
}

//---------------------------------------------------------------------------
void CBaseProcess::Terminate()
{
	BaseOnLostDevice();

	SAFE_RELEASE( g_pFont );
}

bool CBaseProcess::CreateScene()
{
	D3DXCreateFont( NiDX9Renderer::GetRenderer()->GetD3DDevice(), 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
							OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont );
	g_pFont->OnResetDevice();

	InitProcess();

	return true;
}

void CBaseProcess::BaseOnLostDevice()
{
	if( g_pFont )
	{
		g_pFont->OnLostDevice();
	}
	OnLostDevice();
}

void CBaseProcess::BaseOnResetDevice()
{
	g_pFont->OnResetDevice();

	OnResetDevice();
}

CBaseProcess* CBaseProcess::GetSameProcess()
{
	return NULL;
}

bool CBaseProcess::IsCurrentProcess(int nProcess)
{
	if(m_nProcessCmd == nProcess)
		return true;
	else
		return false;
}

void CBaseProcess::UpdateFrame(float fFrameTime, float fAccumTime)
{
	UpdatePopUp();
	UpdateToolTip();

	UpdateProcess(fFrameTime, fAccumTime);//업데이트 함수
	UpdateState(fFrameTime, fAccumTime);//상태 업데이트 함수
}

void CBaseProcess::UpdatePopUp()
{
	map<string, CUIPopUp*>::iterator pIndex=m_mPopUpList.begin();
	map<string, CUIPopUp*>::iterator pIndex_end=m_mPopUpList.end();

	while(pIndex!=pIndex_end)
	{
		if(!((*pIndex).second->WhetherHoverControls()))
		{
			delete (*pIndex).second;
			m_mPopUpList.erase(pIndex++);
		}
		else
		{
			pIndex++;
		}
	}
}

void CBaseProcess::Add_PopUp(const char* sTargetControl)
{

	CUIPopUp * pkPopup = m_mPopUpList[sTargetControl];
	if( pkPopup == 0 )
	{
		pkPopup = new CUIPopUp(sTargetControl);

		m_mPopUpList[sTargetControl]= pkPopup;
	}
}

void CBaseProcess::Add_PopUpHoverArea(const char* sTargetControl, const char* sControlName)
{
	CUIPopUp* pTemp=m_mPopUpList[sTargetControl];

	assert(pTemp);

	pTemp->Add_Control(sControlName);
}

void CBaseProcess::InitToolTipList()
{
	CUIControl* pToolTipListNum=CClientUIManager::GetPtr()->FindEventControl(sKeyWord_ToolTipListNum);
	if(pToolTipListNum==NULL)
		return;

	char sText[STRLEN_64];

    WideCharToMultiByte(CP_ACP, 0, pToolTipListNum->GetText(), STRLEN_32,sText , STRLEN_32, NULL, NULL);
	
	DWORD nToolTipNum=atoi(sText);
    
	DWORD nIndex=0;

	char sToolTipName[STRLEN_64];

	char sTipName[STRLEN_64];
 
	while(nIndex<nToolTipNum)
	{
		sprintf(sToolTipName,"%s%d",sKeyWord_ToolTip.c_str(),nIndex);
		CUIControl* pControl=CClientUIManager::GetPtr()->FindEventControl(sToolTipName);
		if(pControl == NULL) return;
		WideCharToMultiByte(CP_ACP, 0, pControl->GetText(), STRLEN_32,sTipName , STRLEN_32, NULL, NULL);

		m_mToolTipList[sToolTipName]= sTipName ;

		nIndex++;
	}
}
void CBaseProcess::UpdateToolTip()
{
	map<string, string>::iterator pIndex=m_mToolTipList.begin();
	map<string, string>::iterator pIndex_end=m_mToolTipList.end();

	while(pIndex!=pIndex_end)
	{
		if(CClientUIManager::GetPtr()->FindEventControl((*pIndex).first)==CUIControl::Get_MouseOveredControl())
		{
			Add_PopUp((*pIndex).second.c_str());
			Add_PopUpHoverArea((*pIndex).second.c_str(), (*pIndex).second.c_str());
			Add_PopUpHoverArea((*pIndex).second.c_str(), (*pIndex).first.c_str());
		}
		pIndex++;
	}
}

void CBaseProcess::RenderFrame()
{
	if(m_bIsRender)
	{
		CCameraController::GetPtr()->BeginCamera();
        		
	    CClientUIManager::GetPtr()->OnRender();
		
		CCameraController::GetPtr()->EndCamera();
	}
}
void CBaseProcess::ProcessInput()	
{	
}

bool CBaseProcess::OnDefault(NiEventRef pEventRecord)
{
	//CGameOptionDialog::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);

	//	CInputManager::GetPtr()->KeyboardMsgProc(pEventRecord->uiMsg, pEventRecord->lParam, pEventRecord->wParam);

	return CClientUIManager::GetPtr()->MsgProc(pEventRecord->hWnd, pEventRecord->uiMsg, pEventRecord->wParam, pEventRecord->lParam);
}

void CBaseProcess::FUNCTION_3DRender(const char* sWindow, bool bAlphaBlend)
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sWindow);
	if (pControl == NULL) return;

	pControl->m_bAlphaBlend = bAlphaBlend;

    m_cCharacterTexBoard.Init(pObjGroundInfo->GetCamera(), CObjectModelManager::GetPtr()->GetRootNode(), CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.FUNCTION_3DRender(sWindow);
}

void CBaseProcess::FUNCTION_3DRender(const char* sWindow, NiCamera* pCamera, bool bAlphaBlend)
{
	CObjGroundInfo* pObjGroundInfo = dynamic_cast<CObjGroundInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_MAP));
	assert(pObjGroundInfo);
	if(pObjGroundInfo == NULL) return;

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sWindow);
	if (pControl == NULL) return;

	pControl->m_bAlphaBlend = bAlphaBlend;

	m_cCharacterTexBoard.Init(pCamera, CObjectModelManager::GetPtr()->GetRootNode(), CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
	m_cCharacterTexBoard.FUNCTION_3DRender(sWindow);
}

bool CBaseProcess::OnLostDevice()
{
    m_cCharacterTexBoard.OnLostDevice();
    return true;
}

bool CBaseProcess::OnResetDevice()
{
    m_cCharacterTexBoard.SetFrameBufferMode( CGameProject::s_pGameProject->m_eCurrentFrameBufferMode );
    m_cCharacterTexBoard.OnResetDevice();
    return true;
}
