#include "playmovie.h"
#include <assert.h>

CPlayMovie::CPlayMovie(LPCWSTR sFileName, HWND hWnd, LPDIRECT3DDEVICE9& pD3dDevice)
{
	CoInitialize(NULL); 

	HRESULT hr = S_OK;
	CComPtr<IBaseFilter>    pFSrc;          // Source Filter
	CComPtr<IPin>           pFSrcPinOut;    // Source Filter Output Pin
	m_cTextureRenderer = 0;

	hr = m_pGraphicBuilder.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC);
	if (FAILED(hr)){assert(0);}

	m_cTextureRenderer = new CTextureRenderer(NULL, &hr, pD3dDevice);
	if (FAILED(hr)){assert(0);}

	m_pBaseFilter = m_cTextureRenderer;
 
    hr = m_pGraphicBuilder->AddFilter( m_pBaseFilter, L"TEXTURERENDERER");
	if (FAILED(hr)){assert(0);}    

    hr = m_pGraphicBuilder->AddSourceFilter( sFileName, L"SOURCE", &pFSrc);
	if (FAILED(hr)){assert(0);}

	hr = pFSrc->FindPin(L"Output", &pFSrcPinOut);
	if (FAILED(hr)){assert(0);}

	hr = m_pGraphicBuilder->Render(pFSrcPinOut);
	if (FAILED(hr)){assert(0);}

	hr = m_pGraphicBuilder.QueryInterface(&m_pControl);
	if (FAILED(hr)){assert(0);}

	hr = m_pGraphicBuilder.QueryInterface(&m_pMediaPos);
	if (FAILED(hr)){assert(0);}

	hr = m_pGraphicBuilder.QueryInterface(&m_pEvent);
	if (FAILED(hr)){assert(0);}

	hr = m_pGraphicBuilder.QueryInterface(&m_pBasicAudio);
	if (FAILED(hr)){assert(0);}


}

CPlayMovie::~CPlayMovie(void)
{
	CleanUp();
	CoUninitialize();
}

void CPlayMovie::CleanUp()
{
	if( !(!m_pControl) ) 
	{
		Stop();
		m_pControl.Release();
	}

	m_cTextureRenderer->ReleaseTexture();


	if( !(!m_pEvent) ) 
	{
		m_pEvent.Release();
	}
	if( !(!m_pMediaPos) ) 
	{
		m_pMediaPos.Release();
	}

	if( !(!m_pGraphicBuilder) ) 
	{
		m_pGraphicBuilder.Release();
	}
    if( !(!m_pBaseFilter) )
    {
        m_pBaseFilter.Release();
    }

}

void CPlayMovie::SetMovieWindow(DWORD nXPos, DWORD nYPos, DWORD nXSize, DWORD nYSize)
{
}

void CPlayMovie::Play()
{
	HRESULT hr = m_pControl->Run(); 
	if (FAILED(hr)){assert(0);}
}

void CPlayMovie::Stop()
{
	HRESULT hr = m_pControl->Stop(); 
	if (FAILED(hr)){assert(0);}
}

void CPlayMovie::Pause()
{
	HRESULT hr = m_pControl->Pause();
	if (FAILED(hr)){assert(0);}
}

void CPlayMovie::SetVolume(long lVolume)
{
	HRESULT hr = m_pBasicAudio->put_Volume(lVolume);
	if (FAILED(hr)){assert(0);}
}

LPDIRECT3DTEXTURE9* CPlayMovie::GetTexture()
{
	return m_cTextureRenderer->GetTexture();
}

void CPlayMovie::CheckMovieStatus()
{
	long lEventCode;
	LONG_PTR lParam1, lParam2;
	HRESULT hr;

	if (!m_pEvent)
		return;

	// Check for completion events
	hr = m_pEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
	if (SUCCEEDED(hr))
	{
		// If we have reached the end of the media file, reset to beginning
		if (EC_COMPLETE == lEventCode)
		{
			hr = m_pMediaPos->put_CurrentPosition(0);
		}

		// Free any memory associated with this event
		hr = m_pEvent->FreeEventParams(lEventCode, lParam1, lParam2);
	}
}