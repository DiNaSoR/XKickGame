//***********************************************************************************
//	File		:	MessageRooute.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	¨Ï 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "Stdafx.h"
#include <math.h>

#include "ClientUIManager.h"

extern CUIManager *g_pUIManager;

CClientUIManager::CClientUIManager()
{
	g_pUIManager = this;
}

CClientUIManager::~CClientUIManager()
{
}

//--------------------------------------------------------------------------------------
HRESULT CClientUIManager::DrawTexture( CUIControl* pControl, IDirect3DTexture9*	pTexture, RECT& rcScreen, D3DCOLOR color )
{
    //return S_OK;

	IDirect3DDevice9* pd3dDevice = GetD3DDevice();

	//pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	//pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVSRCALPHA );
	//pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);

	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_ALPHAREF,				0 );
	pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,			D3DCMP_NOTEQUAL);

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	N3UT_SCREEN_VERTEX vertices[4] =
	{
		(float) rcScreen.left -0.5f,  (float) rcScreen.top -0.5f,    0.1f, 1.0f, D3DCOLOR_ARGB(255 ,255, 255, 255), 0.f, 0.f,
		(float) rcScreen.right -0.5f, (float) rcScreen.top -0.5f,    0.1f, 1.0f, D3DCOLOR_ARGB(255 ,255, 255, 255), 1.f, 0.f, 
		(float) rcScreen.right -0.5f, (float) rcScreen.bottom -0.5f, 0.1f, 1.0f, D3DCOLOR_ARGB(255 ,255, 255, 255), 1.f, 1.f, 
		(float) rcScreen.left -0.5f,  (float) rcScreen.bottom -0.5f, 0.1f, 1.0f, D3DCOLOR_ARGB(255 ,255, 255, 255), 0.f, 1.f,
	};

//	IDirect3DDevice9* pd3dDevice = GetD3DDevice();
	pd3dDevice->SetFVF( N3UT_SCREEN_VERTEX_FVF );

	//pd3dDevice->SetTexture(0, pTexture);
	SetCurrentTexture(pTexture);

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, pControl->m_bAlphaBlend );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(N3UT_SCREEN_VERTEX) );

	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	return S_OK;
}

HRESULT CClientUIManager::DrawAlphaTexture(CUIControl* pControl, CUIControl* pAlphaControl, RECT& rcScreen, D3DCOLOR color )
{
	SN3BlendState *pControlBlendState = pControl->m_pElement->m_pStates[N3UI_STATE_NORMAL];
	SN3BlendState *pAlphaControlBlendState = pAlphaControl->m_pElement->m_pStates[N3UI_STATE_NORMAL];

	IDirect3DDevice9* pd3dDevice = GetD3DDevice();
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA) ;
	
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	pd3dDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_CURRENT );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );

	int nXEmpty = (pControlBlendState->pTextureNode->dwWidth/2)-((pAlphaControlBlendState->rcTexture.right-pAlphaControlBlendState->rcTexture.left)/2);
	int nYEmpty = (pControlBlendState->pTextureNode->dwHeight/2)-((pAlphaControlBlendState->rcTexture.bottom-pAlphaControlBlendState->rcTexture.top)/2);
	N3UT_SCREEN_VERTEX_MULTI vertices[4] =
	{
		(float) rcScreen.left - 0.5f + nXEmpty,  (float) rcScreen.top - 0.5f + nYEmpty,    0.1f, 1.0f, color, (float)(pControlBlendState->rcTexture.left+ nXEmpty) / (pControlBlendState->pTextureNode->dwWidth), (float)(pControlBlendState->rcTexture.top + nYEmpty) / (pControlBlendState->pTextureNode->dwHeight),	 	(2.f/1024.f), (210.f/1024.f),
		(float) rcScreen.right - 0.5f - nXEmpty, (float) rcScreen.top - 0.5f + nYEmpty,    0.1f, 1.0f, color, (float)(pControlBlendState->rcTexture.right- nXEmpty) / (pControlBlendState->pTextureNode->dwWidth), (float)(pControlBlendState->rcTexture.top + nYEmpty) / (pControlBlendState->pTextureNode->dwHeight),		(163.f/1024.f), (210.f/1024.f), 
		(float) rcScreen.right - 0.5f - nXEmpty, (float) rcScreen.bottom - 0.5f - nYEmpty, 0.1f, 1.0f, color, (float)(pControlBlendState->rcTexture.right- nXEmpty) / (pControlBlendState->pTextureNode->dwWidth), (float)(pControlBlendState->rcTexture.bottom - nYEmpty) / (pControlBlendState->pTextureNode->dwHeight),	(163.f/1024.f), (371.f/1024.f),
		(float) rcScreen.left - 0.5f + nXEmpty,  (float) rcScreen.bottom - 0.5f - nYEmpty, 0.1f, 1.0f, color, (float)(pControlBlendState->rcTexture.left+ nXEmpty) / (pControlBlendState->pTextureNode->dwWidth), (float)(pControlBlendState->rcTexture.bottom - nYEmpty) / (pControlBlendState->pTextureNode->dwHeight),	(2.f/1024.f), (371.f/1024.f),
	};	

	pd3dDevice->SetFVF( N3UT_SCREEN_VERTEX_MULTI_FVF);

//	SetCurrentTexture(pControlBlendState->pTextureNode->pTexture, 0);
//	SetCurrentTexture(pAlphaControlBlendState->pTextureNode->pTexture, 1);
	pd3dDevice->SetTexture(0, pControlBlendState->pTextureNode->pTexture);
	pd3dDevice->SetTexture(1, pAlphaControlBlendState->pTextureNode->pTexture);

	pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(N3UT_SCREEN_VERTEX_MULTI) );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

/*	NiPoint3 vCharPosition = CAvatar::Get()->GetPosition();
	NiTPointerList<CEntity*> * pkEntityList = &CCharacterManager::Get()->m_kEntityList;
	NiTListIterator kPos = pkEntityList->GetHeadPos();
	while (kPos)
	{
		CCharacter* pkCharacter = (CCharacter*)pkEntityList->GetNext(kPos);
		if(pkCharacter->GetCharType() != CCharacter::CHAR_TYPE_AVATAR && pkCharacter->GetTeam() == CAvatar::Get()->GetTeam())
		{
			D3DCOLOR RaderColor;
			int n;
			if(pkCharacter->GetCharType() == CCharacter::CHAR_TYPE_PLAYER)
			{
				RaderColor = 0xffff0000;
				n = 4;
			}
			else
			{
				RaderColor = 0xffffff00;
				n = 3;
			}
			NiPoint3 vPos = pkCharacter->GetPosition();
			NiPoint2 vMiniPos;
			vMiniPos.x = vCharPosition.x - vPos.x;
			vMiniPos.y = vCharPosition.y - vPos.y;
			char szbuff[MAX_PATH];
			sprintf(szbuff, "Character");
			CUIControl* pControlMapCharacter = CClientUIManager::GetPtr()->FindEventControl(szbuff);
			RECT rc;
			int CenterX = pControlMapCharacter->m_rcWindowBox.left + ((pControlMapCharacter->m_rcWindowBox.right - pControlMapCharacter->m_rcWindowBox.left)/2);
			int CenterY = pControlMapCharacter->m_rcWindowBox.top + ((pControlMapCharacter->m_rcWindowBox.bottom - pControlMapCharacter->m_rcWindowBox.top)/2);
			rc.left = CenterX + (int)(-(vMiniPos.x)*(2.f/10.f));
			rc.top = CenterY + (int)((vMiniPos.y)*(2.f/10.f));
			rc.right = CenterX + (int)(-(vMiniPos.x)*(2.f/10.f));
			rc.bottom = CenterY + (int)((vMiniPos.y)*(2.f/10.f));

			float fLength = sqrt((((-(vMiniPos.x)*(2.f/10.f)))*((-(vMiniPos.x)*(2.f/10.f)))) + (((vMiniPos.y)*(2.f/10.f)))*(((vMiniPos.y)*(2.f/10.f))));
			float fRaderLength = sqrt((CenterY - 16.f)*(CenterY - 16.f));
			if( fRaderLength >= fLength)
			{
				N3UT_SCREEN_VERTEX_UNTEX vertices[4] =
				{
					(float) rc.left -0.5f - n,  (float) rc.top -0.5f - n,    0.1f, 1.0f, RaderColor,
					(float) rc.right -0.5f + n, (float) rc.top -0.5f - n,    0.1f, 1.0f, RaderColor,
					(float) rc.right -0.5f + n, (float) rc.bottom -0.5f + n, 0.1f, 1.0f, RaderColor,
					(float) rc.left -0.5f - n,  (float) rc.bottom -0.5f + n, 0.1f, 1.0f, RaderColor,
				};

				IDirect3DDevice9* pd3dDevice = GetD3DDevice();

				SetCurrentTexture(NULL);
				//pd3dDevice->SetTexture(0, NULL);
				pd3dDevice->SetFVF( N3UT_SCREEN_VERTEX_UNTEX_FVF );

				pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(N3UT_SCREEN_VERTEX_UNTEX) );
			}
			else
			{
				int x = CenterX + (int)((float)(fRaderLength/fLength)*(-(vMiniPos.x)*(2.f/10.f))); 
				int y = CenterY + (int)((float)(fRaderLength/fLength)*((vMiniPos.y)*(2.f/10.f)));

				N3UT_SCREEN_VERTEX_UNTEX vertices[4] =
				{
					(float) x -0.5f - n,  (float) y -0.5f - n,    0.1f, 1.0f, RaderColor,
					(float) x -0.5f + n, (float) y -0.5f - n,    0.1f, 1.0f, RaderColor,
					(float) x -0.5f + n, (float) y -0.5f + n, 0.1f, 1.0f, RaderColor,
					(float) x -0.5f - n,  (float) y -0.5f + n, 0.1f, 1.0f, RaderColor,
				};

				IDirect3DDevice9* pd3dDevice = GetD3DDevice();

				SetCurrentTexture(NULL);
				//pd3dDevice->SetTexture(0, NULL);
				pd3dDevice->SetFVF( N3UT_SCREEN_VERTEX_UNTEX_FVF );

				pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(N3UT_SCREEN_VERTEX_UNTEX) );
			}
		}
	}
*/
	return S_OK;
}

void UIMessage::SendMessage_SignIn()
{
	std::string strName, strPass;

//	memset(strName, 0, sizeof(strName));
//	memset(strPass, 0, sizeof(strPass));

	CUIControl* pIdControl = CClientUIManager::GetPtr()->FindEventControl("Login_ID");
	if(pIdControl == NULL) return;
	CUIControl* pPassControl = CClientUIManager::GetPtr()->FindEventControl("Login_Pass");
	if(pPassControl == NULL) return;

	strName = pIdControl->GetTextEx();
	strPass = pPassControl->GetTextEx();

	if(pIdControl && pPassControl && strName.size() != 0 && strPass.size() != 0)
	{
		_snprintf(CGameProject::s_pGameProject->m_strName, MAX_PATH, "%s", strName.c_str() );
		_snprintf(CGameProject::s_pGameProject->m_strPass, MAX_PATH, "%s", strPass.c_str() );
	}
}
