/*********************************************************************************************



*			GameProject.h

*			게임 프로젝트

*			설명 : 게임브리오의 NiApplication을 상속받아 만든 클래스

*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#ifndef __GAMEPROJECT_H__
#define __GAMEPROJECT_H__

#include "stdafx.h"

#include "ProcessManager.h"
#include "ClientUIManager.h"
#include "CameraController.h"
#include "CharacterCollisionDetector.h"
#include "UtlMath.h"

#include "PacketManager.h"
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "ObjectModelManager.h"

#include "ObjMemberManager.h"
#include "ObjPlayerManager.h"
#include "ObjBallManager.h"
#include "ObjRoomManager.h"
#include "EffectManager.h"

#include "AudioManager.h"
#include "InputManager.h"
#include "InputAdapter.h"
#include "AIManager.h"
#include "FootballManager.h"
#include "ClothSimulationManager.h"

#include "ChattingDialog.h"
#include "GameOptionDialog.h"
#include "MessageBoxDialog.h"
#include "InvitationDialog.h"
#include "CreateRoomDialog.h"
#include "ShopDialog.h"
#include "SkillShopDialog.h"
#include "PlayerInformationDialog.h"
#include "InvenDialog.h"
//#include "SignBoard.h"

#include "CollisionDetector.h"

#include "FblEngine.h"
#include "UtlFps.h"
#include "UtlLabel.h"
#include "PlayModeManager.h"
#include "LoadPackFile.h"
#include "GRegKey.h"
#include "GameOptionInfo.h"
#include "AIDomainManager.h"
#include "VideoInfomation.h"

enum SCREEN_MODE_KIND
{
	SCREEN_MODE_NORMAL,
	SCREEN_MODE_WIDE
};

class CShaderHelper;

class CGameProject : public NiApplication
{
	bool						m_bCursor;

	bool						m_bWideScreen;
	POINT						m_WindowPosition;
	

public:
	CPacketManager				m_cPacketManager;//

	unsigned int				m_uiScreenWidth;
	unsigned int				m_uiScreenHeight;

	static	CGameProject*		s_pGameProject;

	CSCNoticeList				s_Notice;

	char						s_nBGM;

	CClientUIManager			m_DialogManager;//클라이언트 UI다이얼로그 메니져

	CCharacterCollisionDetector m_cCharacterCollisionDetector;

	CShaderHelper*				m_pkShaderHelper;

	bool						m_bLostDevice;
	bool						m_bRestoreDevice;

	NiAlphaAccumulatorPtr       m_spAlphaAccumulator;

	float						m_fFramePerSecond;

	HCURSOR						m_hCursor;

	bool						m_bPlayingMusic;

	DWORD						m_nHackTime;

	TiXmlDocument				m_cTiXmlDocument;

	float						m_fGlobalAniSpeed;

#ifdef	_PACKING
	CLoadPackFile				m_cLoadPackFile;
#endif

//////////////////////////////////////////////////////////////////////////
//싱글톤 클레스
	CProcessManager				m_ProcessManager;//싱글톤 클래스 생성

	CCameraController			m_CameraController;//카메라 컨트롤러

	CObjMemberManager			m_ObjMemberInfoManager;//유저정보

	CObjPlayerManager			m_cObjPlayerManager;//

	CObjBallManager				m_cObjBallInfoManager;//

	CObjRoomManager				m_cObjRoomInfoManager;//

	CObjectManager				m_ObjectManager;//오브젝트 메니져

	CResourceManager			m_ResourceManager;//리소스 메니져

	CAIManager					m_AIManager;//AI메니져

	CObjectModelManager			m_ObjectModelManager;//오브젝트 메니져

	CAudioManager				m_AudioManager;

	CInputManager				m_InputManager;

	CFblEngine					m_cFootBall;//축구 관련 클래스

	CFblArea					m_cArea;//영역 관련 클래스

	CCollisionDetector			m_cCollisionDetector;//충돌관련 클레스

	CUtlFps						m_cUtlFps;//FPS클래스

	CUtlLabel					m_cUtlLabel;

	CFootballManager			m_cFootballManager;//

	CPlayModeManager			m_cPlayModeManager;//
    CEffectManager              m_cEffectManager;

	GRegKey						m_cReg;

	CAIDomainManager			m_cAIDomainManager;
    ClothSimulationManager      m_cClothSimulationManager;
    
    VideoInfomation             m_cVideoInfomation;
//////////////////////////////////////////////////////////////////////////
//Dialog
	CChattingDialog				m_ChattingDialog;
	CGameOptionDialog			m_cGameOptionDialog;//게임 옵션 다이얼로그
	CMessageBoxDialog			m_MessageBoxDialog;
	CInvitationDialog			m_InvitatonDialog;
	CCreateRoomDialog			m_cCreateRoomDialog;//방생성 다이얼로그
	CShopDialog					m_cShopDialog;
	CSkillShopDialog			m_cSkillShopDialog;
	CPlayerInformationDialog	m_cPlayerInformationDialog;
	CInvenDialog				m_cInvenDialog;
	//SignBoard					m_cSignBoard;
//////////////////////////////////////////////////////////////////////////
//Renderer

/////////////////////////////////////////////////////////////////////////
// GameOption
////////////////////////////////////////////////////////////////////////
    CGameOptionInfo             m_cGameOptionInfo;

//?	NiRendererPtr				m_spRenderer;	

    bool						m_bNeedRecreateRenderer;

    HWND						m_hTopLevelHwnd;

	POINT						m_nCaptionSize;

	char						m_strName[MAX_PATH];//유저의 이름

	char						m_strPass[MAX_PATH];//유저의 패스워드
    
    bool                        m_bSaveScreenShot;
    std::string                 m_strScreenShotFileName;

	char						m_sClientVersion[STRLEN_32];

    NiDX9Renderer::FramebufferMode m_eCurrentFrameBufferMode;

public:
	CGameProject(void);
	virtual ~CGameProject(void);

	TiXmlElement*				GetTiXml(char *pName);

	virtual void Terminate();
	virtual bool Initialize();

	bool OnLostDevice();
	bool OnResetDevice( bool b);

	virtual bool CreateScene();
	virtual bool CreateCamera();

	bool							LoadFile(const char* filename);
	bool							LoadFileConf(const char* filename);

	void 							SetupGameMode();

	bool GetWideScreenFromFile();
	bool GetFullScreenFromFile();

	virtual void OnIdle(void);

	virtual void UpdateFrame();
	virtual void RenderFrame();

	virtual void ProcessInput();

	virtual bool OnDefault(NiEventRef pEventRecord);

	void InitialUserFunction();

	NiNode				*GetRootNode() { return m_spScene; }

	BOOL GetScreenMode();

    void SetScreenShotFileName( const char * pScreenShotFileName );
    void SaveScreenShot();

public:
	float	GetFramerate()			{ return m_fFrameRate; }
	float	GetFrameTime()			{ return m_fFrameTime; }
	float	GetAccumTime()			{ return m_fAccumTime; }

	void							HidePointer();
	void							ShowPointer();

    virtual bool					CreateRenderer();
	void							ReCreateRenderer();
	NiRendererPtr					GetRenderer()				{ return m_spRenderer; }
	void							SetWideScreen(bool bWideScreen);
	bool							GetWideScreen() { return m_bWideScreen; }

	bool							CreateFontAndLabel();    
    void                            SetMoveWindow( bool bEnable );

	NiInputGamePad *				GetInputGamePad();

protected:
    void                            SaveSurfaceImage();

    //안티앨리어싱 옵션(상,중,하) 에 따라서 적합한 FrameBufferMode 를 리턴한다.
    NiDX9Renderer::FramebufferMode  SelectFrameBufferMode();    

protected:
    virtual bool                    CreateInputSystem();
    NiInputSystem::CreateParams*    GetInputCreationParameters();
    void                            UpdateInput();

public:
    virtual bool                    RecreateInputSystem();

protected:
    bool                            m_bMoveWindow;
    NiActionMapPtr                  m_spEmptyActionMap;
    NiActionMapPtr                  m_spActionMap;
    NiInputGamePadPtr               m_aspGamePad[NiInputSystem::MAX_GAMEPADS];
};


#endif //__GAMEPROJECT_H__