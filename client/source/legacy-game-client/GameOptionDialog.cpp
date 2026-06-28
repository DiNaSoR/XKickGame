//***********************************************************************************
//	File		:	GameOptionDialog.cpp
//	Desc		:
//	Begin		:
//	Copyright	:	ⓒ 2008 by XKick CO., Ltd
//	Author		:
//	Update		:
//***********************************************************************************

#include "StdAfx.h"
#include "GameOptionDialog.h"
#include "ObjMemberManager.h"
#include "ComStructure.h"
#include "../InterfaceLibrary/UIRadioButton.h"
#include "../InterfaceLibrary/UISlider.h"
#include "../InterfaceLibrary/UICheckBox.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "../InterfaceLibrary/UIIMEEDITBox.h"
#include "../InterfaceLibrary/UIProgress.h"
#include "PutPacket.h"
#include "MovieManager.h"
#include "ResTableStack.h"
#include "UICommonMethod.h"
#include <NiDX9Select.h>
#include "UIFunction.h"
#include "VideoInfomation.h"

using namespace MovieManager;

CGameOptionDialog::CGameOptionDialog(void)
{
	m_bOption=false;
	m_bKeyConfig=false;
	m_nOptionTabIndex = 600;
	m_nBGMPage = 1;
	m_nBGMTotalPage = 1;
	m_vBGMList.clear();
	
	m_nMusic = 16;
	m_nSound = 8;

	m_bFullScreen = false;
	m_bWideScreen = false;
	m_bControlKeyboard = true;
	m_bAdd = false;
	m_bUserBGM = false;
	m_bUserQC = false;
}

CGameOptionDialog::~CGameOptionDialog(void)
{
}

void CGameOptionDialog::Update()
{

}

int CGameOptionDialog::GetCharacterLightOption()
{
    return m_nCharacterLight;
}

int CGameOptionDialog::GetReflectionMapOption()
{
    return m_nLightMap;
}

int CGameOptionDialog::GetSpecularOption()
{
    return m_nSpecular;
}

int CGameOptionDialog::GetGameEffectOption()
{
    return m_nEffect;
}

int CGameOptionDialog::GetWeatherEffectOption()
{
    return m_nWeather;
}

int CGameOptionDialog::GetGroundEffectOption()
{
    return m_nGround;
}

void CGameOptionDialog::Terminate()
{
}

void CGameOptionDialog::XmlSetting()
{
	m_pGameQuickChattingXml = CGameProject::s_pGameProject->GetTiXml("QuickChatting");
	m_pGameBGMXml = CGameProject::s_pGameProject->GetTiXml("BGM");
	m_pGameSoundXml = CGameProject::s_pGameProject->GetTiXml("OptionSound");
	m_pGameScreenConf = CGameProject::s_pGameProject->GetTiXml("GameMode");
	m_pGameVideoConf = CGameProject::s_pGameProject->GetTiXml("VideoOption");
}

void CGameOptionDialog::FUNCTION_GameOptionInit(CObjPlayerInfo* pPlayer)
{
	if(pPlayer != NULL)
		m_pObjPlayerInfo = pPlayer;
	XmlSetting();

	//게임 설정
	m_bOption = true;
	FUNCTION_GameOptionCameraLoading();
	FUNCTION_GameOptionZoomLoading();
	FUNCTION_GameOptionTargetLoading();
	FUNCTION_GameOptionTeamLoading();
	FUNCTION_GameOptionBoxLoading();

	FUNCTION_VideoLoading();//비디오 옵션
	FUNCTION_KeyLoading();//키설정

	FUNCTION_SoundInit();
	
	//배경음악 설정 로딩
	FUNCTION_BGMScriptSettingInit();
	BGM_Choice(m_bUserBGM);

	FUNCTION_QuickChattingInit();//퀵채팅
}

void CGameOptionDialog::FUNCTION_GameOptionOK()
{
	// 패킷을 보낸다
	CCSChangeSetting pSetting;
	pSetting.m_nInitSetting = 0;
	memcpy(pSetting.m_cChangeSetting.m_nDefineKey, m_nDefineKey, sizeof(m_nDefineKey));
	memcpy(pSetting.m_cChangeSetting.m_nAttackSkillCode, m_nAttackSkillCode, sizeof(m_nAttackSkillCode));
	memcpy(pSetting.m_cChangeSetting.m_nDefenceSkillCode, m_nDefenceSkillCode, sizeof(m_nDefenceSkillCode));

	// 카메라 세팅 저장
	pSetting.m_cChangeSetting.m_nCameraType = m_pSetting.m_cSetting.m_nCameraType; 
	pSetting.m_cChangeSetting.m_nCameraTarget = m_pSetting.m_cSetting.m_nCameraTarget; 
	pSetting.m_cChangeSetting.m_nCameraTeam = m_pSetting.m_cSetting.m_nCameraTeam; 
	pSetting.m_cChangeSetting.m_nCameraZoom = m_pSetting.m_cSetting.m_nCameraZoom;
	pSetting.m_cChangeSetting.m_nRadian = m_pSetting.m_cSetting.m_nRadian;

	// 이름표시, 귓속말, 초대 저장
	pSetting.m_cChangeSetting.m_nLabel = m_pSetting.m_cSetting.m_nLabel;
	pSetting.m_cChangeSetting.m_nWhisper = m_pSetting.m_cSetting.m_nWhisper;
	pSetting.m_cChangeSetting.m_nInvite = m_pSetting.m_cSetting.m_nInvite;
	
	PutChangeSetting(&pSetting, m_pObjPlayerInfo->GetObjSeq() );

	// 효과음 배경음 저장
	SetVolume(m_nMusic);
	CAudioManager::GetPtr()->SoundVolume((float)m_nSound/10);
	CAudioManager::GetPtr()->MusicVolume(m_nMusic);
	FUNCTION_SoundSave();

	// 스크린 모드설정 저장	
	FUNCTION_VideoSave();
    FUNCTION_ScreenConfigSave();
	FUNCTION_QuickChattingSave();
	FUNCTION_BGMScriptSettingSave();    

    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();
    if( pkVideoInfomation->GetAntiAliasing() != m_nAnti )
    {
        CMessageBoxDialog::GetPtr()->Enable( CMessageBoxDialog::MESSAGEBOX_NONE, 21000, CMessageBoxDialog::MSGBOXTYPE_CONFIRM );
    }

    LoadVideoInfomation();

	m_bKeyConfig = false;
}

///////////////////////////////////////////////////////////////////////////////////////////
//게임설정 옵션 텝 파트

void CGameOptionDialog::FUNCTION_SoundLoading()
{
	
	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Bgm"));
	assert(pPrg);
    
	pPrg->SetValue((int)m_nMusic*LIST5_SIZE);
	CAudioManager::GetPtr()->MusicVolume( m_nMusic );
	SetVolume( m_nMusic );
	
	pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Sound"));
	assert(pPrg);
	pPrg->SetValue(m_nSound*LIST10_SIZE);
	CAudioManager::GetPtr()->SoundVolume( (float)m_nSound / 10 );
}

void CGameOptionDialog::FUNCTION_SoundInit()
{
	// 배경음 설정
	TiXmlNode* pChild = m_pGameSoundXml->FirstChild( "BGSound" );
	TiXmlElement* pElement = (TiXmlElement*)pChild;

	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Bgm"));
	assert(pPrg);
    m_nMusic = (float)atof( pElement->Attribute( "Volume" ) );

	pPrg->SetValue((int)m_nMusic*LIST5_SIZE);
	CAudioManager::GetPtr()->MusicVolume( m_nMusic );
	SetVolume( m_nMusic );

	// 효과음 설정
	pChild = pChild->NextSibling( "Sound" );
	pElement = (TiXmlElement*)pChild;

	m_nSound = atoi( pElement->Attribute( "Volume" ) );

	pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Sound"));
	assert(pPrg);
	pPrg->SetValue(m_nSound*LIST10_SIZE);
	CAudioManager::GetPtr()->SoundVolume( (float)m_nSound / 10 );
}

void CGameOptionDialog::FUNCTION_GameOptionEffectSound(char idx)
{
	m_nSound+=idx;

	if(10<m_nSound)
		m_nSound = 10;
	
	if(m_nSound<0)
		m_nSound = 0;

	CAudioManager::GetPtr()->SoundVolume((float)m_nSound/10);

	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Sound"));
	assert(pPrg);
	pPrg->SetValue(m_nSound*LIST10_SIZE);
}

void CGameOptionDialog::FUNCTION_GameOptionBgm(char idx)
{
	m_nMusic+=idx;

	if(20<m_nMusic)
		m_nMusic = 20;

	if(m_nMusic<0)
		m_nMusic = 0;

	CAudioManager::GetPtr()->MusicVolume( m_nMusic );
	SetVolume(m_nMusic);
	
	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Bgm"));
	assert(pPrg);
	pPrg->SetValue((int)m_nMusic*LIST5_SIZE);
}

void CGameOptionDialog::FUNCTION_SoundSave()
{
	char sText[STRLEN_32]="";
	
	TiXmlNode* pChild = m_pGameSoundXml->FirstChild( "BGSound" );
	
	TiXmlElement* pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", (int)m_nMusic);
	pElement->SetAttribute("Volume", sText);
	
	pChild = pChild->NextSibling( "Sound" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nSound);
	pElement->SetAttribute("Volume", sText);
	
	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

float CGameOptionDialog::GetMusicVol()
{
	return m_nMusic;
}

int CGameOptionDialog::GetSoundVol()
{
	return m_nSound;
}

void CGameOptionDialog::SetMusicVol(float nVol)
{
	m_nMusic = nVol;
	SetVolume(m_nMusic);
	CAudioManager::GetPtr()->MusicVolume(m_nMusic);
}

void CGameOptionDialog::SetSoundVol(int nVol)
{
	m_nSound = nVol;
	CAudioManager::GetPtr()->SoundVolume((float)m_nSound/10);
}

void CGameOptionDialog::FUNCTION_GameOptionCameraLoading()
{
	char nCurrentCamera = m_pObjPlayerInfo->m_cSetting.m_nCameraType;

	m_pSetting.m_cSetting.m_nCameraType = nCurrentCamera;
	GameOptionCameraTypeChange(nCurrentCamera);
}

void CGameOptionDialog::FUNCTION_GameOptionZoomLoading()
{	
	char nCurrentZoom = m_pObjPlayerInfo->m_cSetting.m_nCameraZoom;
    
	m_pSetting.m_cSetting.m_nCameraZoom = nCurrentZoom;
	GameOptionCameraZoomChange(nCurrentZoom);
}

void CGameOptionDialog::FUNCTION_GameOptionTargetLoading()
{
    char nCameraTarget = m_pObjPlayerInfo->m_cSetting.m_nCameraTarget;

	m_pSetting.m_cSetting.m_nCameraTarget = nCameraTarget;
	GameOptionCameraTargetChange(nCameraTarget);
}

void CGameOptionDialog::FUNCTION_GameOptionTeamLoading()
{
	char nCameraTeam = m_pObjPlayerInfo->m_cSetting.m_nCameraTeam;

	m_pSetting.m_cSetting.m_nCameraTeam = nCameraTeam;
	GameOptionCameraTeamChange(nCameraTeam);
}

void CGameOptionDialog::GameOptionCameraTypeChange(char nCode)
{
	switch(nCode)
	{
	case D2WAYTILTMOVE:
		UIFunction::Set_ControlText( "OP_Stc_Camera" , "2WAY" );
		break;
	case D4WAYTILTMOVE:
		UIFunction::Set_ControlText( "OP_Stc_Camera" , "4WAY" );
		break;
	default:
		UIFunction::Set_ControlText( "OP_Stc_Camera" , "4WAY" );
		m_pSetting.m_cSetting.m_nCameraType = D4WAYTILTMOVE;
	}
}

void CGameOptionDialog::GameOptionCameraTeamChange(char nCode)
{
	switch(nCode)
	{
	case CAMERATEAM_NONE:
		UIFunction::Set_ControlText( "OP_Stc_CameraTeam" , "SIDE" );
		break;
	case CAMERATEAM_HOME:
		UIFunction::Set_ControlText( "OP_Stc_CameraTeam" , "FRONT" );
		break;
	default:
		UIFunction::Set_ControlText( "OP_Stc_CameraTeam" , "FRONT" );
		m_pSetting.m_cSetting.m_nCameraTeam = CAMERATEAM_HOME;
	}
}

void CGameOptionDialog::GameOptionCameraTargetChange(char nCode)
{
	CResMessageTableFormat* pMTable = NULL;

	switch(nCode)
	{
	case CAMERATARGET_CHAR:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900068 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "OP_Stc_CameraTarget" , pMTable->m_sMessage );
		break;
	case CAMERATARGET_BALLTOCHAR:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900069 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "OP_Stc_CameraTarget" , pMTable->m_sMessage );
		break;
	case CAMERATARGET_BALL:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900070 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "OP_Stc_CameraTarget" , pMTable->m_sMessage );
		break;
	case CAMERATARGET_REGION:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900071 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "OP_Stc_CameraTarget" , pMTable->m_sMessage );
		break;
	default:
		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900068 ,0));
		assert(pMTable);
		UIFunction::Set_ControlText( "OP_Stc_CameraTarget" , pMTable->m_sMessage );
		m_pSetting.m_cSetting.m_nCameraTarget = CAMERATARGET_CHAR;
	}
}

void CGameOptionDialog::GameOptionCameraZoomChange(char nCode)
{
    switch(nCode)
	{
	case CAMERAZOOM_0:
		UIFunction::Set_ControlText( "OP_Stc_Zoom" , "Zoom1" );
		break;
	case CAMERAZOOM_1:
		UIFunction::Set_ControlText( "OP_Stc_Zoom" , "Zoom2" );
		break;
	case CAMERAZOOM_2:
		UIFunction::Set_ControlText( "OP_Stc_Zoom" , "Zoom3" );
		break;
	case CAMERAZOOM_3:
		UIFunction::Set_ControlText( "OP_Stc_Zoom" , "Zoom4" );
		break;
	default:
		UIFunction::Set_ControlText( "OP_Stc_Zoom" , "Zoom3" );
		m_pSetting.m_cSetting.m_nCameraZoom = CAMERAZOOM_2;
	}
}

void CGameOptionDialog::FUNCTION_GameOptionBoxLoading()
{
	if( m_pObjPlayerInfo->m_cSetting.m_nLabel < 0 || 2 < m_pObjPlayerInfo->m_cSetting.m_nLabel )
		m_pObjPlayerInfo->m_cSetting.m_nLabel = 0;

	if( m_pObjPlayerInfo->m_cSetting.m_nWhisper < 0 || 1 < m_pObjPlayerInfo->m_cSetting.m_nWhisper )
		m_pObjPlayerInfo->m_cSetting.m_nWhisper = 1;

	if( m_pObjPlayerInfo->m_cSetting.m_nInvite < 0 || 1 < m_pObjPlayerInfo->m_cSetting.m_nInvite )
		m_pObjPlayerInfo->m_cSetting.m_nInvite = 1;

	m_pSetting.m_cSetting.m_nLabel = m_pObjPlayerInfo->m_cSetting.m_nLabel;
	m_pSetting.m_cSetting.m_nWhisper = m_pObjPlayerInfo->m_cSetting.m_nWhisper;
	m_pSetting.m_cSetting.m_nInvite = m_pObjPlayerInfo->m_cSetting.m_nInvite;

	GameOptionLabelChange(m_pObjPlayerInfo->m_cSetting.m_nLabel);
	GameOptionWhisperChange(m_pObjPlayerInfo->m_cSetting.m_nWhisper);
	GameOptionInviteChange(m_pObjPlayerInfo->m_cSetting.m_nInvite);
}

void CGameOptionDialog::FUNCTION_GameOptionCameraChange(char idx)
{
	int nCurrentCamera = m_pSetting.m_cSetting.m_nCameraType;

	nCurrentCamera+=idx;
	if( nCurrentCamera < D2WAYTILTMOVE )
		nCurrentCamera = D4WAYTILTMOVE;
	if( D4WAYTILTMOVE < nCurrentCamera )
		nCurrentCamera = D2WAYTILTMOVE;

	m_pSetting.m_cSetting.m_nCameraType = nCurrentCamera;
	GameOptionCameraTypeChange(nCurrentCamera);
}

void CGameOptionDialog::FUNCTION_GameOptionZoomChange(char idx)
{
	int nCurrentZoom = m_pSetting.m_cSetting.m_nCameraZoom;

	nCurrentZoom+=idx;

	if( nCurrentZoom < CAMERAZOOM_0 )
		nCurrentZoom = CAMERAZOOM_3;

	if( CAMERAZOOM_3 < nCurrentZoom )
		nCurrentZoom = CAMERAZOOM_0;
	
	m_pSetting.m_cSetting.m_nCameraZoom = nCurrentZoom;
	GameOptionCameraZoomChange(nCurrentZoom);
}

// 타겟 카메라
void CGameOptionDialog::FUNCTION_GameOptionCameraTarget(char idx)
{
    CUIControl* pCameraTarget = CClientUIManager::GetPtr()->FindEventControl( "OP_Stc_CameraTarget" );
	assert(pCameraTarget);

	int nCameraTarget = m_pSetting.m_cSetting.m_nCameraTarget;
	
	nCameraTarget+=idx*20;

	if( nCameraTarget < CAMERATARGET_CHAR )
		nCameraTarget = CAMERATARGET_BALLTOCHAR;

	if(CAMERATARGET_BALLTOCHAR < nCameraTarget )
		nCameraTarget = CAMERATARGET_CHAR;

	m_pSetting.m_cSetting.m_nCameraTarget = nCameraTarget;
	GameOptionCameraTargetChange(nCameraTarget);
}

// 팀 카메라
void CGameOptionDialog::FUNCTION_GameOptionCameraTeam(char idx)
{
    CUIControl* pCameraTeam = CClientUIManager::GetPtr()->FindEventControl( "OP_Stc_CameraTeam" );
	assert(pCameraTeam);

	int nCameraTeam = m_pSetting.m_cSetting.m_nCameraTeam;

	nCameraTeam+=idx;

	if( nCameraTeam < CAMERATEAM_NONE )
		nCameraTeam = CAMERATEAM_HOME;

	if( CAMERATEAM_HOME < nCameraTeam )
		nCameraTeam = CAMERATEAM_NONE;
	
	m_pSetting.m_cSetting.m_nCameraTeam = nCameraTeam;
	GameOptionCameraTeamChange(nCameraTeam);
}

// 카메라 각도 설정
void CGameOptionDialog::FUNCTION_GameOptionRadianChange()
{
	CUISlider* pSlider = dynamic_cast<CUISlider*>(CClientUIManager::GetPtr()->FindEventControl("OP_Sld_Rad"));
	assert(pSlider);
	pSlider->SetRange(0, 90);
	
	m_pObjPlayerInfo->m_cSetting.m_nRadian = (char)pSlider->GetValue();	
}

void CGameOptionDialog::GameOptionLabelChange(char nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_LabelNo"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_LabelPosition"));
	assert(pChkBox1);
	CUICheckBox* pChkBox2 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_LabelAll"));
	assert(pChkBox2);

	m_pSetting.m_cSetting.m_nLabel = nCode;

	switch(nCode)
	{
	case 0:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(false);
		break;
	case 1:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		pChkBox2->SetCheckValue(false);
		break;
	case 2:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(false);
		pChkBox2->SetCheckValue(true);
		break;
	default:
		 m_pSetting.m_cSetting.m_nLabel = 0;
	}
}

void CGameOptionDialog::GameOptionWhisperChange(char nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_WhisperYes"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_WhisperNo"));
	assert(pChkBox1);

	m_pSetting.m_cSetting.m_nWhisper = nCode;

	switch(nCode)
	{
	case 0:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		break;
	case 1:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		break;
	default:
		m_pSetting.m_cSetting.m_nWhisper = 1;
	}
}

void CGameOptionDialog::GameOptionInviteChange(char nCode)
{
	CUICheckBox* pChkBox0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_InviteYes"));
	assert(pChkBox0);
	CUICheckBox* pChkBox1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Chk_InviteNo"));
	assert(pChkBox1);

	m_pSetting.m_cSetting.m_nInvite = nCode;

	switch(nCode)
	{
	case 0:
		pChkBox0->SetCheckValue(true);
		pChkBox1->SetCheckValue(false);
		break;
	case 1:
		pChkBox0->SetCheckValue(false);
		pChkBox1->SetCheckValue(true);
		break;
	default:
		m_pSetting.m_cSetting.m_nInvite = 1;
	}
}

void CGameOptionDialog::FUNCTION_GameConfigInit()
{
	// 카메라 세팅 저장
	GameOptionCameraTargetChange(m_pSetting.m_cSetting.m_nCameraTarget);
	GameOptionCameraTeamChange(m_pSetting.m_cSetting.m_nCameraTeam);
	GameOptionCameraTypeChange(m_pSetting.m_cSetting.m_nCameraType);
	GameOptionCameraZoomChange(m_pSetting.m_cSetting.m_nCameraZoom);

	// 이름표시, 귓속말, 초대 저장
	GameOptionLabelChange(m_pSetting.m_cSetting.m_nLabel);
	GameOptionWhisperChange(m_pSetting.m_cSetting.m_nWhisper);
	GameOptionInviteChange(m_pSetting.m_cSetting.m_nInvite);

	// 효과음, 배경음
	CUIProgress* pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Sound"));
	assert(pPrg);
	pPrg->SetValue(m_nSound*LIST10_SIZE);
	
	pPrg = dynamic_cast<CUIProgress*>(CClientUIManager::GetPtr()->FindEventControl("OP_Prg_Bgm"));
	assert(pPrg);
	pPrg->SetValue((int)m_nMusic*LIST5_SIZE);
}

///////////////////////////////////////////////////////////////////////////////////////////
//비디오 옵션 텝 파트

void CGameOptionDialog::FUNCTION_VideoInit()
{
	// 설정된 비디오 값 불러오기
	FUNCTION_SetPerformanceChange( 3 );
	FUNCTION_SetCharacterLightChange( m_nCharacterLight );
	FUNCTION_SetLightMapChange( m_nLightMap );
	FUNCTION_SetWeatherChange( m_nWeather );
	FUNCTION_SetGroundChange( m_nGround );
	FUNCTION_SetEffectChange( m_nEffect );
	FUNCTION_SetShadowChange( m_nShadow );
	FUNCTION_SetAntialiasingChange( m_nAnti );
	FUNCTION_SetSpecularChange( m_nSpecular );
}

void CGameOptionDialog::FUNCTION_VideoConfLoad()
{
	// 설정된 비디오 값 불러오기
	TiXmlNode* pChild = m_pGameVideoConf->FirstChild( "Performance" );
	TiXmlElement* pElement = (TiXmlElement*)pChild;
	m_nPerformance = atoi( pElement->Attribute( "Value" ) );
	
	pChild = pChild->NextSibling( "CharacterLight" );
	pElement = (TiXmlElement*)pChild;
	m_nCharacterLight = atoi( pElement->Attribute( "Value" ) );
	
	pChild = pChild->NextSibling( "LightMap" );
	pElement = (TiXmlElement*)pChild;
	m_nLightMap = atoi( pElement->Attribute( "Value" ) );

	pChild = pChild->NextSibling( "Weather" );
	pElement = (TiXmlElement*)pChild;
	m_nWeather = atoi( pElement->Attribute( "Value" ) );
	
	pChild = pChild->NextSibling( "Ground" );
	pElement = (TiXmlElement*)pChild;
	m_nGround = atoi( pElement->Attribute( "Value" ) );

	pChild = pChild->NextSibling( "Effect" );
	pElement = (TiXmlElement*)pChild;
	m_nEffect = atoi( pElement->Attribute( "Value" ) );
	
	pChild = pChild->NextSibling( "Shadow" );
	pElement = (TiXmlElement*)pChild;
	m_nShadow = atoi( pElement->Attribute( "Value" ) );

	pChild = pChild->NextSibling( "Anti" );
	pElement = (TiXmlElement*)pChild;
	m_nAnti = atoi( pElement->Attribute( "Value" ) );

	pChild = pChild->NextSibling( "Specular" );
	pElement = (TiXmlElement*)pChild;
	m_nSpecular = atoi( pElement->Attribute( "Value" ) );
}

void CGameOptionDialog::FUNCTION_VideoSave()
{
	char sText[STRLEN_32]="";
	
	TiXmlNode* pChild = m_pGameVideoConf->FirstChild( "Performance" );
	
	TiXmlElement* pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nPerformance);
	pElement->SetAttribute("Value", sText);
	
	pChild = pChild->NextSibling( "CharacterLight" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nCharacterLight);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "LightMap" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nLightMap);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "Weather" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nWeather);
	pElement->SetAttribute("Value", sText);
	
	pChild = pChild->NextSibling( "Ground" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nGround);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "Effect" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nEffect);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "Shadow" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nShadow);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "Anti" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nAnti);
	pElement->SetAttribute("Value", sText);

	pChild = pChild->NextSibling( "Specular" );
	pElement = (TiXmlElement*)pChild;

	_snprintf(sText, STRLEN_32, "%d", m_nSpecular);
	pElement->SetAttribute("Value", sText);

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

void CGameOptionDialog::FUNCTION_SetPerformanceChange(char idx)
{
	CUICheckBox* pChkBoxPerformance0 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Performance0"));
	assert(pChkBoxPerformance0);

	CUICheckBox* pChkBoxPerformance1 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Performance1"));
	assert(pChkBoxPerformance1);

	CUICheckBox* pChkBoxPerformance2 = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Performance2"));
	assert(pChkBoxPerformance2);

	if( idx == 3 )
	{
		switch(m_nPerformance)
		{
		case 0:
			pChkBoxPerformance0->SetCheckValue(true);
			pChkBoxPerformance1->SetCheckValue(false);
			pChkBoxPerformance2->SetCheckValue(false);
			break;
		case 1:
			pChkBoxPerformance0->SetCheckValue(false);
			pChkBoxPerformance1->SetCheckValue(true);
			pChkBoxPerformance2->SetCheckValue(false);
			break;
		case 2:
			pChkBoxPerformance0->SetCheckValue(false);
			pChkBoxPerformance1->SetCheckValue(false);
			pChkBoxPerformance2->SetCheckValue(true);
			break;
		}
	}
	else
	{
		m_nPerformance = idx;

		switch(idx)
		{
		case 0:
			{
				pChkBoxPerformance0->SetCheckValue(true);
				pChkBoxPerformance1->SetCheckValue(false);
				pChkBoxPerformance2->SetCheckValue(false);
				
				m_nCharacterLight	= 0;
				m_nLightMap			= 0;
				m_nWeather			= 0;
				m_nGround			= 0;
				m_nEffect			= 0;
				m_nShadow			= 0;
				m_nAnti				= 0;
				m_nSpecular			= 0;
			}
			break;
		case 1:
			{
				pChkBoxPerformance0->SetCheckValue(false);
				pChkBoxPerformance1->SetCheckValue(true);
				pChkBoxPerformance2->SetCheckValue(false);
				
				m_nCharacterLight	= 1;
				m_nLightMap			= 1;
				m_nWeather			= 1;
				m_nGround			= 0;
				m_nEffect			= 1;
				m_nShadow			= 1;
				m_nAnti				= 1;
				m_nSpecular			= 0;
			}
			break;
		case 2:
			{
				pChkBoxPerformance0->SetCheckValue(false);
				pChkBoxPerformance1->SetCheckValue(false);
				pChkBoxPerformance2->SetCheckValue(true);

				m_nCharacterLight	= 1;
				m_nLightMap			= 1;
				m_nWeather			= 1;
				m_nGround			= 1;
				m_nEffect			= 1;
				m_nShadow			= 1;
				m_nAnti				= 1;
				m_nSpecular			= 1;
			}
			break;
		}

		FUNCTION_SetCharacterLightChange( m_nCharacterLight );
		FUNCTION_SetLightMapChange( m_nLightMap );
		FUNCTION_SetWeatherChange( m_nWeather );
		FUNCTION_SetGroundChange( m_nGround );
		FUNCTION_SetEffectChange( m_nEffect );
		FUNCTION_SetShadowChange( m_nShadow );
		FUNCTION_SetAntialiasingChange( m_nAnti );
		FUNCTION_SetSpecularChange( m_nSpecular );
	}
}

void CGameOptionDialog::FUNCTION_SetCharacterLightChange(char idx)
{
	CUICheckBox* pChkBoxCharacterLightOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_CharacterLight1"));
	assert(pChkBoxCharacterLightOn);

	CUICheckBox* pChkBoxCharacterLightOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_CharacterLight0"));
	assert(pChkBoxCharacterLightOff);

	m_nCharacterLight = idx;

	switch(idx)
	{
	case 0:
		pChkBoxCharacterLightOn->SetCheckValue(false);
		pChkBoxCharacterLightOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxCharacterLightOn->SetCheckValue(true);
		pChkBoxCharacterLightOff->SetCheckValue(false);
		break;
	}
}

void CGameOptionDialog::FUNCTION_SetLightMapChange(char idx)
{
	CUICheckBox* pChkBoxLightMapOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_LightMap1"));
	assert(pChkBoxLightMapOn);

	CUICheckBox* pChkBoxLightMapOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_LightMap0"));
	assert(pChkBoxLightMapOff);

	m_nLightMap = idx;

	switch(idx)
	{
	case 0:
		pChkBoxLightMapOn->SetCheckValue(false);
		pChkBoxLightMapOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxLightMapOn->SetCheckValue(true);
		pChkBoxLightMapOff->SetCheckValue(false);
		break;
	}
}

void CGameOptionDialog::FUNCTION_SetWeatherChange(char idx)
{
	CUICheckBox* pChkBoxWeatherOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Weather1"));
	assert(pChkBoxWeatherOn);

	CUICheckBox* pChkBoxWeatherOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Weather0"));
	assert(pChkBoxWeatherOff);

	m_nWeather = idx;

	switch(idx)
	{
	case 0:
		pChkBoxWeatherOn->SetCheckValue(false);
		pChkBoxWeatherOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxWeatherOn->SetCheckValue(true);
		pChkBoxWeatherOff->SetCheckValue(false);
		break;
	}
}

void CGameOptionDialog::FUNCTION_SetGroundChange(char idx)
{
    CUICheckBox* pChkBoxGroundOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Ground1"));
	assert(pChkBoxGroundOn);

	CUICheckBox* pChkBoxGroundOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Ground0"));
	assert(pChkBoxGroundOff);

	m_nGround = idx;

	switch(idx)
	{
	case 0:
		pChkBoxGroundOn->SetCheckValue(false);
		pChkBoxGroundOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxGroundOn->SetCheckValue(true);
		pChkBoxGroundOff->SetCheckValue(false);
		break;
	}
}

void CGameOptionDialog::FUNCTION_SetEffectChange(char idx)
{
    CUICheckBox* pChkBoxEffectOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Effect1"));
	assert(pChkBoxEffectOn);

	CUICheckBox* pChkBoxEffectOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Effect0"));
	assert(pChkBoxEffectOff);

	m_nEffect = idx;

	switch(idx)
	{
	case 0:
		pChkBoxEffectOn->SetCheckValue(false);
		pChkBoxEffectOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxEffectOn->SetCheckValue(true);
		pChkBoxEffectOff->SetCheckValue(false);
		break;
	}
}

void CGameOptionDialog::FUNCTION_SetShadowChange(char idx)
{
	CUICheckBox* pChkBoxShadowOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Shadow0"));
	assert(pChkBoxShadowOff);

	CUICheckBox* pChkBoxShadowOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Shadow1"));
	assert(pChkBoxShadowOn);

	m_nShadow = idx;

	switch(idx)
	{
	case 0:
		pChkBoxShadowOff->SetCheckValue(true);
		pChkBoxShadowOn->SetCheckValue(false);
		break;
	case 1:
		pChkBoxShadowOff->SetCheckValue(false);
		pChkBoxShadowOn->SetCheckValue(true);
		break;	
	}
}

void CGameOptionDialog::FUNCTION_SetAntialiasingChange(char idx)
{
	CUICheckBox* pChkBoxSoftLow = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Soft0"));
	assert(pChkBoxSoftLow);

	CUICheckBox* pChkBoxSoftMid = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Soft1"));
	assert(pChkBoxSoftMid);

	//CUICheckBox* pChkBoxSoftHigh = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Soft2"));
	//assert(pChkBoxSoftHigh);

	m_nAnti = idx;

	switch(idx)
	{
	case 0:
		pChkBoxSoftLow->SetCheckValue(false);
		pChkBoxSoftMid->SetCheckValue(true);
		//pChkBoxSoftHigh->SetCheckValue(false);
		break;
	case 1:
		pChkBoxSoftLow->SetCheckValue(true);
		pChkBoxSoftMid->SetCheckValue(false);
		//pChkBoxSoftHigh->SetCheckValue(false);
		break;
	/*case 2:
		pChkBoxSoftLow->SetCheckValue(false);
		pChkBoxSoftMid->SetCheckValue(false);
		pChkBoxSoftHigh->SetCheckValue(true);
		break;*/
	}
}

void CGameOptionDialog::FUNCTION_SetSpecularChange(char idx)
{
	CUICheckBox* pChkBoxSpecularOn = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Specular1"));
	assert(pChkBoxSpecularOn);

	CUICheckBox* pChkBoxSpecularOff = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Specular0"));
	assert(pChkBoxSpecularOff);

	m_nSpecular = idx;

	switch(idx)
	{
	case 0:
		pChkBoxSpecularOn->SetCheckValue(false);
		pChkBoxSpecularOff->SetCheckValue(true);
		break;
	case 1:
		pChkBoxSpecularOn->SetCheckValue(true);
		pChkBoxSpecularOff->SetCheckValue(false);
		break;
	
	}
}

void CGameOptionDialog::FUNCTION_VideoChange(bool bFull)
{
	CUICheckBox* pChkBoxFullScreen = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Screen0"));
	assert(pChkBoxFullScreen);

	CUICheckBox* pChkBoxWindowScreen = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Screen1"));
	assert(pChkBoxWindowScreen);

	if(bFull)
	{
		pChkBoxFullScreen->SetCheckValue(true);
		pChkBoxWindowScreen->SetCheckValue(false);
	}
	else
	{
		pChkBoxFullScreen->SetCheckValue(false);
		pChkBoxWindowScreen->SetCheckValue(true);
	}
}

void CGameOptionDialog::FUNCTION_ModeChange(bool bWide)
{
	CUICheckBox* pChkBoxNormalScreen = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Mode0"));
	assert(pChkBoxNormalScreen);

	CUICheckBox* pChkBoxWideScreen = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("GOP_Chk_Mode1"));
	assert(pChkBoxWideScreen);

	if(bWide)
	{
		pChkBoxNormalScreen->SetCheckValue(false);
		pChkBoxWideScreen->SetCheckValue(true);
	}
	else
	{
		pChkBoxNormalScreen->SetCheckValue(true);
		pChkBoxWideScreen->SetCheckValue(false);
	}
}

void CGameOptionDialog::SetVideoScreen(char idx)
{
	if(idx)
		m_bFullScreen = false;
	else
		m_bFullScreen = true;

	if( m_bFullScreen && m_bWideScreen && !NiDX9Select::IsSelectResolution(1280, 720) )
	{
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20600, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		m_bFullScreen = false;
	}
	
	FUNCTION_VideoChange(m_bFullScreen);	
}

void CGameOptionDialog::SetWideScreen(char idx)
{
	if(idx)
		m_bWideScreen = true;
	else
		m_bWideScreen = false;

	if( m_bFullScreen && m_bWideScreen && !NiDX9Select::IsSelectResolution(1280, 720) )
	{
		CMessageBoxDialog::GetPtr()->Enable(CMessageBoxDialog::MESSAGEBOX_NONE, 20600, CMessageBoxDialog::MSGBOXTYPE_CONFIRM);
		m_bWideScreen = false;
	}
	
	FUNCTION_ModeChange(m_bWideScreen);
}

char CGameOptionDialog::GetShadowOn()
{
	return m_nShadow;
}

void CGameOptionDialog::FUNCTION_VideoLoading()
{
	m_bFullScreen = CGameProject::s_pGameProject->GetFullscreen();
	m_bWideScreen = CGameProject::s_pGameProject->GetWideScreen();
	
	FUNCTION_VideoConfLoad();
	FUNCTION_VideoInit();

	FUNCTION_VideoChange(m_bFullScreen);
	FUNCTION_ModeChange(m_bWideScreen);
}

void CGameOptionDialog::FUNCTION_ScreenConfigInit()
{
	FUNCTION_SetPerformanceChange(1);
	m_bWideScreen = false;
	m_bFullScreen = false;
	FUNCTION_VideoChange(m_bFullScreen);
	FUNCTION_ModeChange(m_bWideScreen);
}

void CGameOptionDialog::FUNCTION_ScreenConfigSave()
{
    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();

	if(CGameProject::s_pGameProject->GetWideScreen() == m_bWideScreen && 
		CGameProject::s_pGameProject->GetFullscreen() == m_bFullScreen )
		return;

	if( CGameProject::s_pGameProject->GetWideScreen() != m_bWideScreen )
	{
		CBaseProcess* pSameProcess = CProcessManager::GetPtr()->Get_CurrentProcess()->GetSameProcess();
		CProcessManager::GetPtr()->SetNextProcess(pSameProcess);
	}

	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(CGameProject::s_pGameProject->m_cTiXmlDocument.FirstChild( "GameMode" ));
	assert(pElement);

	if(m_bFullScreen)
		pElement->SetAttribute("Full", "true");
	else
		pElement->SetAttribute("Full", "false");

	if(m_bWideScreen)
		pElement->SetAttribute("Wide", "true");
	else
		pElement->SetAttribute("Wide", "false");

	CGameProject::s_pGameProject->SetWideScreen(m_bWideScreen);
	CGameProject::s_pGameProject->SetFullscreen(m_bFullScreen);

	CGameProject::s_pGameProject->ReCreateRenderer();
	
	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

///////////////////////////////////////////////////////////////////////////////////////////
//키설정 옵션 텝 파트

void CGameOptionDialog::ControllerToggle()
{
	m_bControlKeyboard = !m_bControlKeyboard;
	FUNCTION_ControlChange();
}
void CGameOptionDialog::FUNCTION_ControlChange()
{
	if( m_bControlKeyboard )
		UIFunction::Set_ChildControlText( "Controller_Select", "ControllerName", CResourceManager::GetPtr()->m_pStaticMap[ 190042 ].c_str() );
	else
		UIFunction::Set_ChildControlText( "Controller_Select", "ControllerName", CResourceManager::GetPtr()->m_pStaticMap[ 190081 ].c_str() );
	
	if( m_bControlKeyboard )
	{
		UIFunction::Set_UIVisibility( "Img_Keyboard", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "Img_Joypad", N3UI_VISIBLE_FALSE );
	}
	else
	{
		UIFunction::Set_UIVisibility( "Img_Keyboard", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "Img_Joypad", N3UI_VISIBLE_TRUE );
	}
}

void CGameOptionDialog::FUNCTION_ControlConfSet(bool bAdd)
{
	m_bAdd = bAdd;

	CUICheckBox* pRadBasic = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl( "OP_Rad_BasicConf" ));
	assert(pRadBasic);
	CUICheckBox* pRadAdd = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl( "OP_Rad_AddConf" ));
	assert(pRadAdd);

	if( m_bAdd )
	{
		pRadBasic->SetCheckValue(false);
		pRadAdd->SetCheckValue(true);
		UIFunction::Set_UIVisibility( "KeySetting0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "KeySetting1", N3UI_VISIBLE_TRUE );
	}
	else
	{
		pRadBasic->SetCheckValue(true);
		pRadAdd->SetCheckValue(false);
		UIFunction::Set_UIVisibility( "KeySetting0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "KeySetting1", N3UI_VISIBLE_FALSE );
	}
}

void CGameOptionDialog::FUNCTION_KeyLoading()
{
	char sText[MAX_PATH]="";

	FUNCTION_ControlChange();
	FUNCTION_ControlConfSet(m_bAdd);

	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		m_nDefineKey[i] = m_pObjPlayerInfo->m_cSetting.m_nDefineKey[i];

		_snprintf(sText, MAX_PATH, "KeySelect%d", i);

		CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, m_nDefineKey[i] ,0));
		if( temp == NULL )
			UIFunction::Set_ControlText( sText, "NONE" );
		else
			UIFunction::Set_ControlText( sText, temp->m_sName );
	}

	CUIComboBox* pComboBox0 = NULL;
	CUIComboBox* pComboBox1 = NULL;

	if(m_pObjPlayerInfo == NULL) return;

	for(int i=0;i<ARRAY_SKILL_KEY_SIZE;i++)
	{
		m_nAttackSkillCode[i] = m_pObjPlayerInfo->m_cSetting.m_nAttackSkillCode[i];		//공격 스킬 선택
		m_nDefenceSkillCode[i] = m_pObjPlayerInfo->m_cSetting.m_nDefenceSkillCode[i];	//방어 스킬 선택

		_snprintf( sText, MAX_PATH, "Combo_Skill_%d0" , i );
		pComboBox0 = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pComboBox0);

		CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900072 ,0));
		assert(pMTable);

		pComboBox0->RemoveAllItems();
		pComboBox0->AddItem(pMTable->m_sMessage, NULL);

		_snprintf( sText, MAX_PATH, "Combo_Skill_%d1" , i );
		pComboBox1 = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pComboBox1);

		pComboBox1->RemoveAllItems();
		pComboBox1->AddItem(pMTable->m_sMessage, NULL);

		for(int j=0;j<(int)m_pObjPlayerInfo->m_vSkillList.size();j++)
		{
			if(m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_nKey == 1)
				pComboBox0->AddItem(m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_sName, (void*)m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_nType);
			if(m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_nKey == 2)
				pComboBox1->AddItem(m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_sName, (void*)m_pObjPlayerInfo->m_vSkillList.at(j)->m_pTable->m_nType);
		}	
	}
	
	for(int i=0;i<ARRAY_SKILL_KEY_SIZE;i++)
	{
		int nCode = m_pObjPlayerInfo->GetSkillCode(m_pObjPlayerInfo->m_cSetting.m_nAttackSkillCode[i]);

		CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, nCode ,0));
		if( pSkillTable == NULL ) continue;

		_snprintf( sText, MAX_PATH, "Combo_Skill_%d0" , i );
		pComboBox0 = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pComboBox0);
		pComboBox0->SetSelectedByData((void*)pSkillTable->m_nType);
	}
    
	for(int i=0;i<ARRAY_SKILL_KEY_SIZE;i++)
	{
		int nCode = m_pObjPlayerInfo->GetSkillCode(m_pObjPlayerInfo->m_cSetting.m_nDefenceSkillCode[i]);

		CResSkillTableFormat *pSkillTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, nCode ,0));
		if( pSkillTable == NULL ) continue;

		_snprintf( sText, MAX_PATH, "Combo_Skill_%d1" , i );
		pComboBox1 = dynamic_cast<CUIComboBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pComboBox1);
		pComboBox1->SetSelectedByData((void*)pSkillTable->m_nType);
	}
}

void CGameOptionDialog::FUNCTION_KeyInit()
{
	for(int i = 0; i < ARRAY_KEY_SIZE; i++)
	{
		char szbuff[MAX_PATH]="";
		_snprintf(szbuff, MAX_PATH, "KeySelect%d", i);
		
		CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, m_nDefineKey[i] ,0));
		assert(temp);
		UIFunction::Set_ControlText( szbuff, temp->m_sName );
	}

	FUNCTION_KeyLoading();

	FUNCTION_ControlConfSet(m_bAdd);
}

void CGameOptionDialog::SetKeySelect(int keyselect)
{
	char szbuff[MAX_PATH]="";

	static bool bSpace = false;

	if( keyselect != m_nKeySelect && m_nDefineKey[keyselect] != KEY_SPACE )
		bSpace = false;

	if( bSpace ) 
	{
		bSpace = false;
		return;
	}

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900047 ,0));
	assert(pMTable);

	CUIButton* pBtn = NULL;
  	for(int i=0;i<ARRAY_KEY_SIZE;i++)
	{
		_snprintf(szbuff, MAX_PATH, "KeySelect%d", i);
		pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(szbuff));
		assert(pBtn);

		if(!strcmp(pBtn->GetTextEx().c_str(), pMTable->m_sMessage))
		{
			CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, m_pObjPlayerInfo->m_cSetting.m_nDefineKey[i], 0));
			assert(temp);
			pBtn->SetText(temp->m_sName);
		}
	}

	m_nKeySelect = keyselect;

	if( m_nDefineKey[m_nKeySelect] == KEY_SPACE)
		bSpace = true;

	_snprintf(szbuff, MAX_PATH, "KeySelect%d", m_nKeySelect);
	pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl(szbuff));
	assert(pBtn);

	pBtn->SetText(pMTable->m_sMessage, true);
	pBtn->SetPressed(true);
}

bool CGameOptionDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if(m_bOption) {
		SN3UIUserFunctionParameter cParam;
		switch( uMsg )
		{
		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				cParam.AddParam( "Window", "GameOption" );
				CClientUIManager::GetPtr()->ProcUserFunction( "HideWindow",  &cParam );
				m_bKeyConfig = false;
				return true;
			/*case VK_DELETE:
				FUNCTION_GameOptionEffectSoundDown();
				return true;
			case VK_INSERT:
				FUNCTION_GameOptionEffectSoundUp();
				return true;
			case VK_END:
				FUNCTION_GameOptionBgmDown();
				return true;
			case VK_HOME:
				FUNCTION_GameOptionBgmUp();
				return true;
			case VK_F11:
				FUNCTION_GameOptionCameraChangeRight();
				return true;
			case VK_PRIOR:
				FUNCTION_GameOptionZoomChangeLeft();
				return true;
			case VK_NEXT:
				FUNCTION_GameOptionZoomChangeRight();
				return true;*/
			}
			return true;
		}
	}

	if(m_nKeySelect < 0) return true;

	CUIButton* pBtn = NULL;

	switch( uMsg )
	{
	case WM_KEYUP:
		{
			if(!m_bKeyConfig)
			{
				char szbuff[MAX_PATH]="";

				for(int i=0;i<ARRAY_KEY_SIZE;i++)
				{
					_snprintf(szbuff, MAX_PATH, "KeySelect%d", i);
					pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( szbuff ));
					assert(pBtn);

					CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900047 ,0));
					assert(pMTable);

					if(!strcmp(pBtn->GetTextEx().c_str(), pMTable->m_sMessage))
					{
						CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, m_pObjPlayerInfo->m_cSetting.m_nDefineKey[i], 0));
						assert(temp);
						UIFunction::Set_ControlText( szbuff, temp->m_sName );
					}
				}
				break;
			}

			BYTE byActKey = ((BYTE)(((lParam)&0x00ff0000)>>16));
			if(byActKey == 0) return true;

			switch(wParam)
			{
			case VK_HOME:
			case VK_UP:
			case VK_PRIOR:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_END:
			case VK_DOWN:
			case VK_NEXT:
			case VK_INSERT:
			case VK_DELETE:
				byActKey += 150;
			}

			char szbuff[MAX_PATH];

			for(int i=0;i<ARRAY_KEY_SIZE;i++)
			{
				if(m_nDefineKey[i] == byActKey)
				{
					_snprintf(szbuff, MAX_PATH, "KeySelect%d", i);
					UIFunction::Set_ControlText( szbuff, "NONE" );
					m_nDefineKey[i] = 0;
				}
			}

			m_nDefineKey[m_nKeySelect] = byActKey;

			_snprintf(szbuff, MAX_PATH, "KeySelect%d", m_nKeySelect);
			CResInputTableFormat *temp = dynamic_cast<CResInputTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_INPUTTABLE, byActKey ,0));
			assert(temp);
			pBtn = dynamic_cast<CUIButton*>(CClientUIManager::GetPtr()->FindEventControl( szbuff ));
			assert(pBtn);
			pBtn->SetPressed(false);
			UIFunction::Set_ControlText( szbuff, temp->m_sName );
			m_nKeySelect = -1;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//배경음악 옵션 텝 파트
void CGameOptionDialog::BGM_Choice(bool bUser)
{
	char sText[STRLEN_64]="";

	m_bUserBGM = bUser;

	CUICheckBox* pChkB = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Rad_BasicBGM"));
	assert(pChkB);
	CUICheckBox* pChkU = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Rad_UserBGM"));
	assert(pChkU);
	
	if( m_bUserBGM )
	{
		pChkB->SetCheckValue(false);
		pChkU->SetCheckValue(true);
		
		FUNCTION_UserMusicInit();

		UIFunction::Set_UIVisibility( "FindFileNum", N3UI_VISIBLE_TRUE );
	}
	else
	{
		pChkB->SetCheckValue(true);
		pChkU->SetCheckValue(false);

		FUNCTION_DefaultMusicInit();
		
		UIFunction::Set_UIVisibility( "FindFileNum", N3UI_VISIBLE_FALSE );
	}
}

void CGameOptionDialog::FUNCTION_DefaultMusicInit()
{
	char sText[STRLEN_64]="";

	CResOggTableStack *temp = dynamic_cast<CResOggTableStack*>(CResourceManager::GetPtr()->GetResStack(RESOURCE_OGGTABLE));
	assert(temp);
	
	m_nBGMTotalPage = (temp->GetResourceSize()/LIST10_SIZE)+1;

	if( 1<m_nBGMTotalPage && temp->GetResourceSize()%LIST10_SIZE != 0)
		m_nBGMTotalPage++;

	if( temp->GetResourceSize() < (m_nBGMPage-1) * LIST10_SIZE )
		m_nBGMPage--;

	for(int i=0;i<LIST10_SIZE;i++)
	{
		CResOggTableFormat *temp = dynamic_cast<CResOggTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OGGTABLE, i+(LIST10_SIZE*(m_nBGMPage-1)), 0));

		_snprintf(sText, STRLEN_64, "Chk_BGM_0%d", i);
		UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_FALSE );
			
		_snprintf(sText, STRLEN_64, "BGMList0%d", i);
		if(temp == NULL)
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
		else
		{
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_TRUE);
			_snprintf(sText, STRLEN_64, "BGM_ListName0%d", i);
			UIFunction::Set_ControlText( sText, temp->m_sDataFile );
		}
	}

	FUNCTION_BGMPageUpdate();
}

void CGameOptionDialog::FUNCTION_UserMusicInit()
{
	char sText[STRLEN_64]="";

	int i = Mp3OggLoad(1, m_nBGMPage-1);		// OGG 파일 검색
	//i += Mp3OggLoad(0, m_nBGMPage-1);		// MP3 파일 검색

	CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900074 ,0));
	assert(pMTable);

	_snprintf(sText, STRLEN_64, pMTable->m_sMessage, i);
	UIFunction::Set_ControlText( "FindFileNum", sText );
	
	m_nBGMTotalPage = (i/LIST10_SIZE)+1;

	if( 1<m_nBGMTotalPage && i%LIST10_SIZE != 0)
		m_nBGMTotalPage++;

	for(int i=0;i<LIST10_SIZE;i++)
	{
		_snprintf(sText, STRLEN_64, "BGMList0%d", i);
		if( strlen(m_sFileList[i]) )
		{
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_TRUE);
			_snprintf(sText, STRLEN_64, "Chk_BGM_0%d", i);
			CUICheckBox* pChk = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
			assert(pChk);
			pChk->SetCheckValue(false);

			BGM_VECTORLIST::iterator iter;

			for( iter=m_vBGMList.begin(); iter != m_vBGMList.end();iter++ )
			{
				if( (*iter) == (i+1)+((m_nBGMPage-1)*LIST10_SIZE) )
				{
					pChk->SetCheckValue(true);
					break;
				}
			}
			
			UIFunction::Set_UIVisibility( sText, N3UI_VISIBLE_TRUE );
			_snprintf(sText, STRLEN_64, "BGM_ListName0%d", i);
			UIFunction::Set_ControlText( sText, m_sFileList[i] );
		}
		else
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_FALSE);
	}

	FUNCTION_BGMPageUpdate();
}

void CGameOptionDialog::FUNCTION_UserMusicConf(char idx)
{
	char sText[STRLEN_64]="";

	_snprintf(sText, STRLEN_64, "Chk_BGM_0%d", idx-1);
	CUICheckBox* pChk = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
	assert(pChk);

	if(!m_vBGMList.size())
	{
		pChk->SetCheckValue(true);
		m_vBGMList.push_back(((m_nBGMPage-1)*LIST10_SIZE)+idx);
	}
	else
	{
		BGM_VECTORLIST::iterator iter;

		for( iter=m_vBGMList.begin(); iter != m_vBGMList.end();iter++ )
		{
			if( (*iter) == idx+((m_nBGMPage-1)*LIST10_SIZE) )
			{
				pChk->SetCheckValue(false);
				m_vBGMList.erase(iter);
				return;
			}
		}

		pChk->SetCheckValue(true);
		m_vBGMList.push_back(((m_nBGMPage-1)*LIST10_SIZE)+idx);
	}
}

void CGameOptionDialog::FUNCTION_BGMPageUpdate()
{
	char sText[STRLEN_64]="";

	if( m_nBGMPage < 1 )
		m_nBGMPage = 1;

	_snprintf(sText, STRLEN_64, "%d/%d", m_nBGMPage, m_nBGMTotalPage);
	UIFunction::Set_ControlText( "BGMPage", sText );
}

char CGameOptionDialog::Mp3OggLoad(char nKind, char nPage)
{
	char sText[STRLEN_64]="";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = NULL;

	memset(m_sFileList, 0, sizeof(m_sFileList));

	if(nKind)
		hFile = FindFirstFile(".\\Music\\*.ogg", &FindFileData);
	else
		hFile = FindFirstFile(".\\Music\\*.mp3", &FindFileData);

	if(hFile == INVALID_HANDLE_VALUE) return 0;

	int i = 0;

	while( hFile != INVALID_HANDLE_VALUE )
	{
		if( i != 0 )
		{
			if(!FindNextFile(hFile, &FindFileData))
				break;
		}

		if( LIST10_SIZE*nPage <= i && i < LIST10_SIZE*(nPage+1) )
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if( (int)strlen(FindFileData.cFileName) != 0 )
					strcpy(m_sFileList[i%LIST10_SIZE], FindFileData.cFileName);	
			}
		}
		else
			continue;
		i++;
	}
	FindClose(hFile);

	return i;
}

void CGameOptionDialog::FUNCTION_BGMScriptClear()
{	
	m_vBGMList.clear();

	m_pGameBGMXml->SetAttribute("UserConf", "false");

	TiXmlNode* pChild = m_pGameBGMXml->FirstChild( "Music" );

	for(int i=0;i<LIST10_SIZE;i++) 
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		pElement->SetAttribute("ConfIdx", "0");
		
		pChild = pChild->NextSibling( "Music" );
	}

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

void CGameOptionDialog::FUNCTION_BGMScriptSettingInit()
{
	char sText[MAX_PATH] = "";

	if( !strcmp(m_pGameBGMXml->Attribute("UserConf"), "true") )
		m_bUserBGM = true;
	else
		m_bUserBGM = false;

	TiXmlNode* pChild = m_pGameBGMXml->FirstChild( "Music" );

	m_vBGMList.clear();

	for(int i=0;i<LIST10_SIZE;i++) 
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if( m_bUserBGM )
		{
			if(atoi(pElement->Attribute( "ConfIdx" )))
				m_vBGMList.push_back(atoi(pElement->Attribute( "ConfIdx" )));
		}
		else
			break;
        
		pChild = pChild->NextSibling( "Music" );
	}
}

void CGameOptionDialog::FUNCTION_BGMScriptSettingSave()
{
	char sText[MAX_PATH]="";
	
	if(m_bUserBGM)
		m_pGameBGMXml->SetAttribute("UserConf", "true");
	else
	{
		m_pGameBGMXml->SetAttribute("UserConf", "false");
		m_vBGMList.clear();
	}

	TiXmlNode* pChild = m_pGameBGMXml->FirstChild( "Music" );

	for(int i=0;i<LIST10_SIZE;i++) 
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if( m_bUserBGM && i<(int)m_vBGMList.size() )
		{
			_snprintf( sText, MAX_PATH, "%d", m_vBGMList.at(i) );
			pElement->SetAttribute("ConfIdx", sText);
		}
		else
			pElement->SetAttribute("ConfIdx", "0");
		
		pChild = pChild->NextSibling( "Music" );
	}

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

char* CGameOptionDialog::LoadUserBGM(char nCode)
{
	srand(time(NULL));

	if( m_vBGMList.size() )
	{
		int nRand = m_vBGMList.at(rand()%m_vBGMList.size());

		Mp3OggLoad( 1, nRand/LIST10_SIZE );
		return m_sFileList[nRand-1];
	}
	else
		return "";
}

///////////////////////////////////////////////////////////////////////////////////////////
//퀵채팅 옵션 텝 파트

void CGameOptionDialog::FUNCTION_QuickChoice(bool bUser)
{
	m_bUserQC = bUser;

	CUICheckBox* pChkD = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Rad_BasicQ"));
	assert(pChkD);
	CUICheckBox* pChkU = dynamic_cast<CUICheckBox*>(CClientUIManager::GetPtr()->FindEventControl("OP_Rad_UserQ"));
	assert(pChkU);

	if(bUser)
	{
		pChkD->SetCheckValue(false);
		pChkU->SetCheckValue(true);
		UIFunction::Set_UIVisibility( "QuickChat0", N3UI_VISIBLE_FALSE );
		UIFunction::Set_UIVisibility( "QuickChat1", N3UI_VISIBLE_TRUE );
	}
	else
	{
		pChkD->SetCheckValue(true);
		pChkU->SetCheckValue(false);
		UIFunction::Set_UIVisibility( "QuickChat0", N3UI_VISIBLE_TRUE );
		UIFunction::Set_UIVisibility( "QuickChat1", N3UI_VISIBLE_FALSE );
	}
}

void CGameOptionDialog::FUNCTION_QuickChattingEmpty()
{
	char sText[MAX_PATH]="";
	CUIIMEEditBox* pEdit = NULL;

	FUNCTION_QuickChoice(false);

	for(int i=0;i<8;i++) 
	{
		_snprintf(sText, MAX_PATH, "QuickChatUserList0%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pEdit);
		pEdit->SetText("");
		_snprintf(sText, MAX_PATH, "QuickChatUserList1%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pEdit);
		pEdit->SetText("");
	}
}

void CGameOptionDialog::FUNCTION_QuickChattingInit()
{
	char sText[MAX_PATH] = "";

	if( !strcmp( m_pGameQuickChattingXml->Attribute( "UserConf" ), "true" ) )
        m_bUserQC = true;
	else
		m_bUserQC = false;

	FUNCTION_QuickChoice(m_bUserQC);

	TiXmlNode* pChild = m_pGameQuickChattingXml->FirstChild( "Quick" );
	CUIIMEEditBox* pEdit = NULL;

	for(int i=0;i<8;i++) 
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		_snprintf(sText, MAX_PATH, "QuickChatUserList0%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pEdit);
		pEdit->SetText(pElement->Attribute( "Str1" ));
		_snprintf(sText, MAX_PATH, "QuickChatUserList1%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(sText));
		assert(pEdit);
		pEdit->SetText(pElement->Attribute( "Str2" ));

		pChild = pChild->NextSibling( "Quick" );
	}
}

void CGameOptionDialog::FUNCTION_QuickChattingSave()
{
	char temp[MAX_PATH];
	std::string sText;

	if( m_bUserQC )
		m_pGameQuickChattingXml->SetAttribute( "UserConf", "true" );
	else
		m_pGameQuickChattingXml->SetAttribute( "UserConf", "false" );

	TiXmlNode* pChild = m_pGameQuickChattingXml->FirstChild( "Quick" );
	CUIIMEEditBox* pEdit = NULL;

	for(int i=0;i<8;i++) 
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		sprintf(temp, "QuickChatUserList0%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(temp));
		assert(pEdit);
		sText = pEdit->GetTextEx();
		pElement->SetAttribute("Str1", sText.c_str());
		sprintf(temp, "QuickChatUserList1%d", i);
		pEdit = dynamic_cast<CUIIMEEditBox*>(CClientUIManager::GetPtr()->FindEventControl(temp));
		assert(pEdit);
		sText = pEdit->GetTextEx();
		pElement->SetAttribute("Str2", sText.c_str());

		if((int)sText.length())
			pElement->SetAttribute("IsConf", "1");
		else
			pElement->SetAttribute("IsConf", "0");

		pChild = pChild->NextSibling( "Quick" );
	}

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}

///////////////////////////////////////////////////////////////////////////////////////////
//언어 옵션 텝 파트

void CGameOptionDialog::FUNCTION_LanguageLoading()
{
	m_nLangMax = CResourceManager::GetPtr()->m_vColumn.size()-2;

	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(CGameProject::s_pGameProject->m_cTiXmlDocument.FirstChild( "Language" ));
	assert(pElement);

	std::string sLangKind = pElement->Attribute("Kind");

	CUIControl* pLangStatic = CClientUIManager::GetPtr()->FindEventControl( "LG_Stc_Language" );
	assert(pLangStatic);
	pLangStatic->SetText(sLangKind.c_str());

	for(int i=2;i<(int)CResourceManager::GetPtr()->m_vColumn.size();i++)
	{
		if(!sLangKind.compare(CResourceManager::GetPtr()->m_vColumn.at(i).c_str()))
            m_nLang = i-2;			
	}
}

void CGameOptionDialog::FUNCTION_LanguageSelect(char bPlus)
{
		m_nLang+=bPlus;

		if(m_nLang==m_nLangMax)
			m_nLang=0;

		if(m_nLang<0)
			m_nLang=m_nLangMax-1;
	
	UIFunction::Set_ControlText( "LG_Stc_Language", CResourceManager::GetPtr()->m_vColumn.at(m_nLang+2).c_str() );
}

void CGameOptionDialog::FUNCTION_LanguageAdjust()
{
	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(CGameProject::s_pGameProject->m_cTiXmlDocument.FirstChild( "Language" ));
	assert(pElement);

	pElement->SetAttribute("Kind", CResourceManager::GetPtr()->m_vColumn.at(m_nLang+2).c_str());

	CGameProject::s_pGameProject->m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");
}