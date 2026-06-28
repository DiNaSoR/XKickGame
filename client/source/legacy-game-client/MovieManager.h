#pragma once

#include "stdafx.h"
#include "PlayMovie.h"

namespace MovieManager
{

	static CPlayMovie * pCurrentMovie = NULL;
	static string sCurrentMovieFile;
	static string sCurrentTargetUI;
	static bool bMute = false;
	static float m_fVolume = 0.f;

	CPlayMovie * GetMovie();

	LPDIRECT3DDEVICE9 GetMovieDevice();
	void ReleaseMovie();
	void SetMovieOnTexture(const string sFileName, const string sTargetUI);
	void PlayMovie();
	void PauseMovie();
	void ContinueMovie();
	void SoundOffMovie();
	void SoundOnMovie();
	void SetVolume(float vol);
	void OnLostDevice();
	void OnResetDevice();
};
