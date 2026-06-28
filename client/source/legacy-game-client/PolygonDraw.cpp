#include "StdAfx.h"
#include "PolygonDraw.h"
#include "UIManager.h"
#include "PlayModeManager.h"
#include "UICommonMethod.h"

extern CUIManager *g_pUIManager;

CPolygonDraw::CPolygonDraw(void)
{
	m_pd3dDevice = NiDX9Renderer::GetRenderer()->GetD3DDevice();
}

CPolygonDraw::CPolygonDraw(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
}

CPolygonDraw::~CPolygonDraw(void)
{
}


void CPolygonDraw::DrawPolygon(char* sTargetControl, int nPolygonNum, float fBaseScale, D3DXCOLOR* color, D3DXVECTOR3 vSink, float fVertexScale[LIST32_SIZE])
{
	if(nPolygonNum < 3 || nPolygonNum > 32)
	{
		MessageBox(NULL, "폴리곤 수는 3~32 개만 가능합니다.", "에러!", MB_OK);
		return;
	}

	Vertex v[LIST32_SIZE];
	//
	D3DXMATRIX mRad;
	D3DXVECTOR3 vPentagon[LIST32_SIZE];
	D3DXMatrixIdentity(&mRad);
	D3DXMatrixRotationZ(&mRad, ((D3DX_PI*2)/(float)nPolygonNum));

	CUIControl* pControl = CClientUIManager::GetPtr()->FindEventControl(sTargetControl);
	RECT rc = pControl->GetWindowBox();

	POINT center;
	center.x = rc.right - ((rc.right - rc.left)/2);
	center.y = rc.bottom - ((rc.bottom - rc.top)/2);
	center.x += static_cast<LONG>(vSink.x);
	center.y += static_cast<LONG>(vSink.y);

	vPentagon[0].x = 0.0f;
	vPentagon[0].y = -fBaseScale;
	vPentagon[0].z = 0.0f;

	v[0].m_vP.x = static_cast<float>(center.x);
	v[0].m_vP.y = static_cast<float>(center.y);
	v[0].m_vP.z = 0.0f;
    v[0].m_fW = 1.0f;
    v[0].m_nColor = D3DCOLOR_ARGB( 100, 100, 100, 100 );

	for(int i=1;i<=nPolygonNum;i++)
	{
		v[i].m_vP.x = static_cast<float>(center.x);
		v[i].m_vP.y = static_cast<float>(center.y);
		v[i].m_vP.z = 0.0f;

        D3DXVec3TransformNormal(&vPentagon[i], &vPentagon[i-1], &mRad);
	}

    vPentagon[0] *= fVertexScale[0];//5.0f;
	for(int i=1;i<=nPolygonNum;i++)
	{
		vPentagon[i] *= fVertexScale[i];//5.0f;
		v[i].m_vP += vPentagon[i-1];
		v[i].m_fW = 1.0f;
		v[i].m_nColor = color[i];
	}
	v[nPolygonNum+1] = v[1];

	m_pd3dDevice->SetVertexShader(0);
	m_pd3dDevice->SetPixelShader(0);

	m_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

    m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );    

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

	HRESULT hr = m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nPolygonNum, v, sizeof(Vertex) );

    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}