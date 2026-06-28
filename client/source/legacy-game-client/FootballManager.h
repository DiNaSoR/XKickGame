#ifndef __FOOTBALL_MANAGER_H__
#define __FOOTBALL_MANAGER_H__

#include "Singleton.h"

class CFootballManager : public CSingleton < CFootballManager >
{
public:
	//int					m_nPlayState;

public:
	CFootballManager();
	~CFootballManager();

	void							UpdateFootball(float fFrameTime, float fAccumTime, int nState);
};

#endif