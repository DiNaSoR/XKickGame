#include "StdAfx.h"
#include ".\3drenderedtexboard.h"
#include "UIControl.h"

C3DRenderedTexBoard::C3DRenderedTexBoard()
{
    m_pTargetControl = 0;
}

C3DRenderedTexBoard::C3DRenderedTexBoard(NiCamera* pCamera, NiNode* pNode)
:m_pCamera(pCamera), m_pNode(pNode)
{
}


C3DRenderedTexBoard::~C3DRenderedTexBoard(void)
{
}

void C3DRenderedTexBoard::FUNCTION_3DRender(const char* sWindow)
{
	m_sName = string(sWindow);

	Terminate();
    CreateTexture();
}

std::string C3DRenderedTexBoard::GetRenderWindowName()
{
    return m_sName;
}

void C3DRenderedTexBoard::CreateTexture()
{
    m_pTargetControl = CClientUIManager::GetPtr()->FindEventControl(m_sName.c_str());

    if( m_pTargetControl == 0 ) return;

    Create( m_pTargetControl->GetWidth(), m_pTargetControl->GetHeight(), m_eFrameBufferMode );
    m_pTargetControl->SetTexture(GetTexture());
}

void C3DRenderedTexBoard::Render()
{	
    if( m_spTexture != 0 )
    {    
        C3DRenderedTexture::Render(m_pCamera, m_pNode);
    }
}

void C3DRenderedTexBoard::Init(NiCamera* pCamera, NiNode* pNode, NiDX9Renderer::FramebufferMode eFrameBufferMode )
{
	m_pCamera=pCamera;
	m_pNode=pNode;
    m_eFrameBufferMode = eFrameBufferMode;
}

void C3DRenderedTexBoard::SetCamera( NiCamera * pkCamera )
{
    m_pCamera = pkCamera;
}

void C3DRenderedTexBoard::SetNode( NiNode * pkNode )
{
    m_pNode = pkNode;
}

bool C3DRenderedTexBoard::OnLostDevice()
{
	C3DRenderedTexture::Terminate();

	return true;
}

bool C3DRenderedTexBoard::OnResetDevice()
{
    FUNCTION_3DRender(m_sName.c_str());

    m_pTargetControl = 0;
    this->Render();

    return true;
}

void C3DRenderedTexBoard::SetFrameBufferMode( NiDX9Renderer::FramebufferMode eFrameBufferMode )
{
    m_eFrameBufferMode = eFrameBufferMode;
}
