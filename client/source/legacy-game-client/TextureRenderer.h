#pragma once

#include <d3d9.h>
#include <streams.h>

struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd70767}")) CLSID_TextureRenderer;

class CTextureRenderer : public CBaseVideoRenderer
{
public:
	LPDIRECT3DDEVICE9		m_pD3dDevice;
	LPDIRECT3DTEXTURE9*     m_pTexture;
	D3DFORMAT               m_TextureFormat;

	void *                  temp;

public:
	CTextureRenderer(LPUNKNOWN pUnk,HRESULT *phr, LPDIRECT3DDEVICE9& pD3dDevice);
	~CTextureRenderer();
	LPDIRECT3DTEXTURE9* GetTexture();
	void ReleaseTexture();
    
    void SetTemp( LONG nTemp );

public:
	HRESULT CheckMediaType(const CMediaType *pmt );     
	virtual HRESULT SetMediaType(const CMediaType *pmt );      
	HRESULT DoRenderSample(IMediaSample *pMediaSample); 

	BOOL m_bUseDynamicTextures;
	LONG m_lVidWidth;   
	LONG m_lVidHeight;  
	LONG m_lVidPitch;   
};
