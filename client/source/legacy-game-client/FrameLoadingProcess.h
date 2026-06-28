/*********************************************************************************************



*			FrameLoadingProcess.h

*			클라이언트 초기로딩 프로세스

*			설명 : 클라이언트 초기에 로딩하는 부분


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "ProcessObserver.h"

class CFrameLoadingProcess;

class LoadingObserver : public IProcessObserver
{
public:
    LoadingObserver( CFrameLoadingProcess * pFrameLoadingProcess );
    virtual ~LoadingObserver();

public:
    virtual void OnUpdate( int nValue );

protected:
    CFrameLoadingProcess * m_pFrameLoadingProcess;
};

class CFrameLoadingProcess : public CBaseProcess
{
public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_TABLE_OGG_LOAD,				//배경음 테이블 로딩
		STATE_TABLE_ANIMATION_LOAD,			//에니메이션 테이블 로딩
		STATE_TABLE_WAV_LOAD,				//효과음 테이블 로딩
		STATE_TABLE_GROUND_LOAD,			//그라운드 테이블 로딩
		STATE_TABLE_CHARACTER_LOAD,			//캐릭터 테이블 로딩
		STATE_GROUND_LOAD,					//그라운드 로딩
		STATE_WEB_LOGIN,					//아이디,패스를 매개변수로 받아 로긴
		STATE_CERTIFY_LOGIN_WAIT,			//인증서버 로긴 대기
		STATE_MEMBER_INFO_WAIT,				//맴버 정보 대기
		STATE_END1,
		STATE_END2
	};

	CUIProgress* m_pProgress;
    LoadingObserver * m_pLoadingObserver;

public:
	CFrameLoadingProcess();
	virtual ~CFrameLoadingProcess(void);

    virtual void					Terminate();

	void							LoadCharacter();
	virtual void					UpdateState(float fFrameTime, float fAccumTime);

	void							UpdateProcess(float fFrameTime,float fAccumTime);

	virtual void					InitProcess();

	void							UiStaticLanguageLoading();
};