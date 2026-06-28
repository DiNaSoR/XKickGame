//***********************************************************************************
//	File		:	GameOptionDialog.h
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#pragma once

#include "Singleton.h"
#include "Control.h"

#include "AudioManager.h"

class CGameOptionDialog : public CSingleton<CGameOptionDialog>
{
protected:
	TiXmlElement*		m_pGameQuickChattingXml;
	TiXmlElement*		m_pGameBGMXml;
	TiXmlElement*		m_pGameSoundXml;
	TiXmlElement*		m_pGameScreenConf;
	TiXmlElement*		m_pGameVideoConf;

public:
	int					m_nBigFractionIndex;
	int					m_nOptionTabIndex;

	CObjPlayerInfo*		m_pObjPlayerInfo;
	CSCChangeSetting	m_pSetting;

///////////////////////////////////////////////////////////////////////////////////////////
//게임설정 옵션 텝 파트
protected:
	float				m_nMusic;
	int					m_nSound;
///////////////////////////////////////////////////////////////////////////////////////////
//비디오 옵션 텝 파트
protected:
	bool				m_bFullScreen;
	bool				m_bWideScreen;
	char				m_nPerformance;
	char				m_nCharacterLight;
	char				m_nLightMap;
	char				m_nWeather;
	char				m_nGround;
	char				m_nEffect;
	char				m_nShadow;
	char				m_nAnti;
	char				m_nSpecular;
	
///////////////////////////////////////////////////////////////////////////////////////////
//키설정 옵션 텝 파트
public:
	short				m_nDefineKey[ARRAY_KEY_SIZE];	//키 선택
	int					m_nAttackSkillCode[ARRAY_SKILL_KEY_SIZE];	//공격 스킬 선택
	int					m_nDefenceSkillCode[ARRAY_SKILL_KEY_SIZE];	//방어 스킬 선택
	int					m_nKeySelect;//선택된 키
	bool				m_bOption;//옵션창 활성 여부
	bool				m_bKeyConfig;
	bool				m_bControlKeyboard;
	bool				m_bAdd;
///////////////////////////////////////////////////////////////////////////////////////////
//배경음악 옵션 텝 파트
	char				m_nBGMPage;
	char				m_nBGMTotalPage;
	char				m_sFileList[LIST10_SIZE][MAX_PATH];
	bool				m_bUserBGM;
	
	typedef vector<int>	BGM_VECTORLIST;
	BGM_VECTORLIST		m_vBGMList;
	
///////////////////////////////////////////////////////////////////////////////////////////
//퀵채팅 옵션 텝 파트

	bool				m_bUserQC;
	
///////////////////////////////////////////////////////////////////////////////////////////
//언어 옵션 텝 파트
	
public:
	CGameOptionDialog(void);
	~CGameOptionDialog(void);

	void				Update();
	void				Terminate();

	void				FUNCTION_GameOptionInit(CObjPlayerInfo* pPlayer);

	void				FUNCTION_GameOptionOK();

///////////////////////////////////////////////////////////////////////////////////////////
//게임설정 옵션 텝 파트
	// 초기 로딩 부분
	void				XmlSetting();
	void				FUNCTION_GameOptionCameraLoading();
	void				FUNCTION_GameOptionZoomLoading();
	void				FUNCTION_GameOptionTargetLoading();
	void				FUNCTION_GameOptionTeamLoading();
	void				FUNCTION_GameOptionBoxLoading();
	
	void				FUNCTION_SoundInit();
	
	// 카메라
	void				FUNCTION_GameOptionCameraChange(char idx);
	// 줌
	void				FUNCTION_GameOptionZoomChange(char idx);
	// 타겟 카메라
	void				FUNCTION_GameOptionCameraTarget(char idx);
	// 팀 카메라
	void				FUNCTION_GameOptionCameraTeam(char idx);
	// 카메라 각도 설정
	void				FUNCTION_GameOptionRadianChange();

	// 카메라 각도 타입, 팀, 타겟, 줌
	void				GameOptionCameraTypeChange(char nCode);
	void				GameOptionCameraTeamChange(char nCode);
	void				GameOptionCameraTargetChange(char nCode);
	void				GameOptionCameraZoomChange(char nCode);

	// 게임설정 이름표시, 귓속말, 초대 관련
	void				GameOptionLabelChange(char nCode);
	void				GameOptionWhisperChange(char nCode);
	void				GameOptionInviteChange(char nCode);

public:
	float				GetMusicVol();
	int					GetSoundVol();
	void				SetMusicVol(float nVol);
	void				SetSoundVol(int nVol);
	void				FUNCTION_GameOptionEffectSound(char idx);
	void				FUNCTION_GameOptionBgm(char idx);
	void				FUNCTION_SoundSave();
	void				FUNCTION_SoundLoading();
	
	void				FUNCTION_GameConfigInit();

///////////////////////////////////////////////////////////////////////////////////////////
//비디오 옵션 텝 파트
public:
	void				SetVideoScreen(char idx);
	void				SetWideScreen(char idx);
	char				GetShadowOn();
	
	void				FUNCTION_VideoConfLoad();
	void				FUNCTION_VideoInit();
	void				FUNCTION_VideoSave();

	void				FUNCTION_SetPerformanceChange(char idx);
	void				FUNCTION_SetCharacterLightChange(char idx);
	void				FUNCTION_SetLightMapChange(char idx);
	void				FUNCTION_SetWeatherChange(char idx);
	void				FUNCTION_SetGroundChange(char idx);
	void				FUNCTION_SetEffectChange(char idx);
	void				FUNCTION_SetShadowChange(char idx);
	void				FUNCTION_SetAntialiasingChange(char idx);
	void				FUNCTION_SetSpecularChange(char idx);

	void				FUNCTION_VideoChange(bool bFull);
	void				FUNCTION_ModeChange(bool bWide);
	void				FUNCTION_VideoLoading();
	void				FUNCTION_ScreenConfigInit();
	void				FUNCTION_ScreenConfigSave();
	
    int                 GetCharacterLightOption();
    int                 GetReflectionMapOption();
    int                 GetSpecularOption();
    int                 GetGameEffectOption();
    int                 GetWeatherEffectOption();
    int                 GetGroundEffectOption();
///////////////////////////////////////////////////////////////////////////////////////////
//키설정 옵션 텝 파트
	void				ControllerToggle();
	void				FUNCTION_ControlChange();
	void				FUNCTION_ControlConfSet(bool bAdd);
	void				FUNCTION_KeyInit();
	void				FUNCTION_KeyLoading();
	void				SetKeySelect(int keyselect);
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

///////////////////////////////////////////////////////////////////////////////////////////
//배경음악 옵션 텝 파트
	void				FUNCTION_DefaultMusicInit();
	void				FUNCTION_UserMusicInit();
	void				FUNCTION_UserMusicConf(char idx);
	void				FUNCTION_BGMPageUpdate();
	void				FUNCTION_BGMScriptSettingInit();
	void				FUNCTION_BGMScriptClear();
	void				FUNCTION_BGMScriptSettingSave();
	void				BGM_Choice(bool bUser);
	char				Mp3OggLoad(char nKind, char nPage);
	char*				LoadUserBGM(char nCode);
	
///////////////////////////////////////////////////////////////////////////////////////////
//퀵채팅 옵션 텝 파트
	void				FUNCTION_QuickChoice(bool bUser);
	void				FUNCTION_QuickChattingInit();
	void				FUNCTION_QuickChattingEmpty();
	void				FUNCTION_QuickChattingCheck(int idx, int flag);
	void				FUNCTION_QuickChattingSave();
	bool				SaveXml(const char *pszbuff);

///////////////////////////////////////////////////////////////////////////////////////////
//언어 옵션 텝 파트
	char				m_nLang;
	char				m_nLangMax;
	void				FUNCTION_LanguageLoading();
	void				FUNCTION_LanguageSelect(char bPlus);
	void				FUNCTION_LanguageAdjust();
};