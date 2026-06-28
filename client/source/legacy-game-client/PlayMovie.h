#pragma once

#include <atlbase.h>

#include "TextureRenderer.h"

class CPlayMovie
{
private:
	CTextureRenderer*		m_cTextureRenderer;
	CComPtr<IGraphBuilder>	m_pGraphicBuilder;
	CComPtr<IMediaControl>	m_pControl;
	CComPtr<IMediaEvent>	m_pEvent;
	CComPtr<IMediaPosition> m_pMediaPos;
	CComPtr<IBaseFilter>    m_pBaseFilter;
	CComPtr<IBasicAudio>	m_pBasicAudio;

public:
	CPlayMovie(LPCWSTR sFileName, HWND hWnd, LPDIRECT3DDEVICE9& pD3dDevice);
	~CPlayMovie(void);
	void SetMovieWindow(DWORD nXPos, DWORD nYPos, DWORD nXSize, DWORD nYSize);
	void Play();
	void Stop();
	void Pause();
	void SetVolume(long lVolume);

	LPDIRECT3DTEXTURE9* GetTexture();

	void CheckMovieStatus();

	void CleanUp();
};