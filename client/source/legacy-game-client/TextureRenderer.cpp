#include "stdafx.h"
#include "TextureRenderer.h"

CTextureRenderer::CTextureRenderer( LPUNKNOWN pUnk, HRESULT *phr, LPDIRECT3DDEVICE9 &pD3dDevice)
                                    :   m_pD3dDevice(pD3dDevice), 
                                        CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer),
                                        NAME("Texture Renderer"), pUnk, phr),
                                        m_bUseDynamicTextures(FALSE)
{
	m_pTexture = new LPDIRECT3DTEXTURE9;
	*m_pTexture = NULL;
	ASSERT(phr);
	if (phr)
		*phr = S_OK;
}

CTextureRenderer::~CTextureRenderer()
{
    static int nDeleteCount = 0;

    char buf[512];
    sprintf( buf, "Delete Count : %d \n", ++nDeleteCount );
    OutputDebugString( buf );

	delete m_pTexture;
}

LPDIRECT3DTEXTURE9* CTextureRenderer::GetTexture()
{
	return m_pTexture;
}

void CTextureRenderer::ReleaseTexture()
{
	if((*m_pTexture) != NULL)
		(*m_pTexture)->Release();
}

HRESULT CTextureRenderer::CheckMediaType(const CMediaType *pmt)
{
	HRESULT   hr = E_FAIL;
	VIDEOINFO *pVideoInfo=0;

	CheckPointer(pmt,E_POINTER);

	if( *pmt->FormatType() != FORMAT_VideoInfo ) {
		return E_INVALIDARG;
	}

	pVideoInfo = (VIDEOINFO *)pmt->Format();

	if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
		IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24))
	{
		hr = S_OK;
	}

	return hr;
}

void Test( CTextureRenderer * pTest, int nTemp )
{
    pTest->m_lVidWidth = nTemp;
}

void CTextureRenderer::SetTemp( LONG nTemp )
{
    Test( this, nTemp );
}

HRESULT CTextureRenderer::SetMediaType(const CMediaType *pmt)
{        

	HRESULT hr;

	UINT uintWidth = 2;
	UINT uintHeight = 2;

	D3DCAPS9 caps;
	VIDEOINFO *pviBmp;                 
	pviBmp = (VIDEOINFO *)pmt->Format();

	m_lVidWidth  = pviBmp->bmiHeader.biWidth;
	m_lVidHeight = abs(pviBmp->bmiHeader.biHeight);
	m_lVidPitch  = (m_lVidWidth * 3 + 3) & ~(3);

	ZeroMemory( &caps, sizeof(D3DCAPS9));
	hr = m_pD3dDevice->GetDeviceCaps( &caps );
	if( caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES )
	{
		m_bUseDynamicTextures = TRUE;
	}    

	if( caps.TextureCaps & D3DPTEXTURECAPS_POW2 )
	{
		while( (LONG)uintWidth < m_lVidWidth )
		{
			uintWidth = uintWidth << 1;
		}
		while( (LONG)uintHeight < m_lVidHeight )
		{
			uintHeight = uintHeight << 1;
		}
	}
	else
	{
		uintWidth = m_lVidWidth;
		uintHeight = m_lVidHeight;
	}

	hr = E_UNEXPECTED;
	if( m_bUseDynamicTextures )
	{
		hr = m_pD3dDevice->CreateTexture(uintWidth, uintHeight, 1, D3DUSAGE_DYNAMIC,
			D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,
			m_pTexture, NULL);

		if( FAILED(hr))
		{
			m_bUseDynamicTextures = FALSE;
		}
	}
	if( FALSE == m_bUseDynamicTextures )
	{
		hr = m_pD3dDevice->CreateTexture(uintWidth, uintHeight, 1, 0,
			D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,
			m_pTexture, NULL);
	}
	if( FAILED(hr))
	{
		return hr;
	}

	D3DSURFACE_DESC ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	if ( FAILED( hr = (*m_pTexture)->GetLevelDesc( 0, &ddsd ) ) ) {
		return hr;
	}


	CComPtr<IDirect3DSurface9> pSurf;

	if (SUCCEEDED(hr = (*m_pTexture)->GetSurfaceLevel(0, &pSurf)))
		pSurf->GetDesc(&ddsd);

	m_TextureFormat = ddsd.Format;

	if (m_TextureFormat != D3DFMT_X8R8G8B8 &&
		m_TextureFormat != D3DFMT_A1R5G5B5) {
			return VFW_E_TYPE_NOT_ACCEPTED;
		}

		return S_OK;
}

HRESULT CTextureRenderer::DoRenderSample( IMediaSample * pSample )
{
	BYTE  *pBmpBuffer, *pTxtBuffer; 
	LONG  lTxtPitch;                
	BYTE  * pbS = NULL;
	DWORD * pdwS = NULL;
	DWORD * pdwD = NULL;
	UINT row, col, dwordWidth;

	CheckPointer(pSample,E_POINTER);
	CheckPointer((*m_pTexture),E_UNEXPECTED);

	pSample->GetPointer( &pBmpBuffer );

	D3DLOCKED_RECT d3dlr;
	if( m_bUseDynamicTextures )
	{
		if( FAILED((*m_pTexture)->LockRect(0, &d3dlr, 0, D3DLOCK_DISCARD)))
			return E_FAIL;
	}
	else
	{
		if (FAILED((*m_pTexture)->LockRect(0, &d3dlr, 0, 0)))
			return E_FAIL;
	}
	pTxtBuffer = static_cast<byte *>(d3dlr.pBits);
	lTxtPitch = d3dlr.Pitch;

	pTxtBuffer += lTxtPitch*m_lVidHeight;

	if (m_TextureFormat == D3DFMT_X8R8G8B8)
	{
		dwordWidth = m_lVidWidth / 4; 

		for( row = 0; row< (UINT)m_lVidHeight; row++)
		{
			pdwS = ( DWORD*)pBmpBuffer;
			pdwD = ( DWORD*)pTxtBuffer;

			for( col = 0; col < dwordWidth; col ++ )
			{
				pdwD[0] =  pdwS[0] | 0xFF000000;
				pdwD[1] = ((pdwS[1]<<8)  | 0xFF000000) | (pdwS[0]>>24);
				pdwD[2] = ((pdwS[2]<<16) | 0xFF000000) | (pdwS[1]>>16);
				pdwD[3] = 0xFF000000 | (pdwS[2]>>8);
				pdwD +=4;
				pdwS +=3;
			}

			pbS = (BYTE*) pdwS;
			for( col = 0; col < (UINT)m_lVidWidth % 4; col++)
			{
				*pdwD = 0xFF000000     |
					(pbS[2] << 16) |
					(pbS[1] <<  8) |
					(pbS[0]);
				pdwD++;
				pbS += 3;
			}

			pBmpBuffer  += m_lVidPitch;
			pTxtBuffer	-= lTxtPitch;
		}
	}

	if (m_TextureFormat == D3DFMT_A1R5G5B5)
	{
		for(int y = 0; y < m_lVidHeight; y++ )
		{
			BYTE *pBmpBufferOld = pBmpBuffer;
			BYTE *pTxtBufferOld = pTxtBuffer;

			for (int x = 0; x < m_lVidWidth; x++)
			{
				*(WORD *)pTxtBuffer = (WORD)
					(0x8000 +
					((pBmpBuffer[2] & 0xF8) << 7) +
					((pBmpBuffer[1] & 0xF8) << 2) +
					(pBmpBuffer[0] >> 3));

				pTxtBuffer += 2;
				pBmpBuffer += 3;
			}

			pBmpBuffer = pBmpBufferOld + m_lVidPitch;
			pTxtBuffer = pTxtBufferOld - lTxtPitch;
		}
	}

	if (FAILED((*m_pTexture)->UnlockRect(0)))
		return E_FAIL;

	return S_OK;
}
