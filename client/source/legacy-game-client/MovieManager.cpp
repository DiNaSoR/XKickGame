#include "MovieManager.h"
#include "GameProject.h"

CPlayMovie * MovieManager::GetMovie()
{
	return pCurrentMovie;
}

LPDIRECT3DDEVICE9 MovieManager::GetMovieDevice() 
{
	NiRendererPtr pNiRendererPtr = CGameProject::s_pGameProject->GetRenderer();
	NiRenderer* pNiRenderer = pNiRendererPtr->GetRenderer();
	NiD3DRenderer* pNiD3DRenderer = NiDynamicCast(NiD3DRenderer, pNiRenderer);
	assert(pNiD3DRenderer);
	return pNiD3DRenderer->GetD3DDevice();
}
	
void MovieManager::ReleaseMovie()
{
	if(pCurrentMovie)
	{
		delete pCurrentMovie;
		pCurrentMovie = NULL;
	}
	sCurrentMovieFile.clear();
}

void MovieManager::SetMovieOnTexture(const string sFileName, const string sTargetUI)
{
	if(sFileName.empty())
		return;

	sCurrentMovieFile = ".\\Movie\\";
	sCurrentMovieFile.append(sFileName);
	sCurrentTargetUI = sTargetUI;

	WCHAR m_strText[STRLEN_32];

	StringCchCopyW( m_strText, MAX_PATH, MultiByteToWideChar(sCurrentMovieFile.c_str()));

	LPDIRECT3DDEVICE9 pDevice = GetMovieDevice();

	pCurrentMovie = new CPlayMovie(m_strText,NULL,pDevice);

	pCurrentMovie->SetVolume((LONG)(DSBVOLUME_MIN + (m_fVolume*100.f)));

	CClientUIManager::GetPtr()->FindEventControl(sTargetUI.c_str())->SetTexture(*(pCurrentMovie->GetTexture()));
}

void MovieManager::PlayMovie()
{
	if(pCurrentMovie)
		pCurrentMovie->Play();
}

void MovieManager::PauseMovie()
{
	if(pCurrentMovie)
		pCurrentMovie->Pause();
}

void MovieManager::ContinueMovie()
{
	if(pCurrentMovie)
		pCurrentMovie->Play();
}

void MovieManager::SoundOffMovie()
{
	if(pCurrentMovie)
	{
		pCurrentMovie->SetVolume(-10000L);
		bMute = true;
	}
}

void MovieManager::SoundOnMovie()
{
	if(pCurrentMovie)
	{
		pCurrentMovie->SetVolume(0L);
		bMute = false;
	}
}

void MovieManager::SetVolume(float vol)
{
	m_fVolume = vol;
	if(m_fVolume > 20.f) m_fVolume = 20.f;
	if(m_fVolume == 0.f) m_fVolume = -80.f;
	
	m_fVolume += 80.f;

	if( pCurrentMovie )
	{
		pCurrentMovie->SetVolume((LONG)(DSBVOLUME_MIN + (m_fVolume*100.f)));
	}
}

void MovieManager::OnLostDevice()
{
	if(pCurrentMovie)
	{
		delete pCurrentMovie;
		pCurrentMovie = NULL;
	}
}

void MovieManager::OnResetDevice()
{
	SetMovieOnTexture(sCurrentMovieFile, sCurrentTargetUI);

	PlayMovie();
}