#pragma once

#include "3DElement.h"

class CUIControl;

using namespace std;

class C3DRenderedTexBoard : public C3DRenderedTexture
{
private:
    NiCamera    *                   m_pCamera;
    NiNode      *                   m_pNode;
    string                          m_sName;
    CUIControl  *                   m_pTargetControl;
    NiDX9Renderer::FramebufferMode  m_eFrameBufferMode;

private:
    void CreateTexture();

public:
    C3DRenderedTexBoard();
    C3DRenderedTexBoard(NiCamera* pCamera, NiNode* pNode);

    virtual ~C3DRenderedTexBoard();

    void Init(NiCamera* pCamera, NiNode* pNode, NiDX9Renderer::FramebufferMode eFrameBufferMode );

    void SetCamera( NiCamera * pkCamera );
    void SetNode( NiNode * pkNode );
    void SetFrameBufferMode( NiDX9Renderer::FramebufferMode eFrameBufferMode );

    void FUNCTION_3DRender(const char* sWindow);
    void Render();

    std::string GetRenderWindowName();
    NiCamera * GetCamera() { return m_pCamera;  }

    virtual bool OnLostDevice();
    virtual bool OnResetDevice();
};
