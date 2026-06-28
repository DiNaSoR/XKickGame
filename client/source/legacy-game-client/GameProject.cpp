#include "stdafx.h"
#include ".\GameProject.h"
#include "ShaderHelper.h"
#include <NiMetricsLayer.h>
#include <NiDX9Select.h>
#include <NiInputSystem.h>
#include <NiDI8InputSystem.h>

#include "UtlHack.h"

#include "resource.h"

#ifdef USERFUNCTION
#undef USERFUNCTION
#endif
#define USERFUNCTION( functionname ) \
	void N3UI_##functionname( SN3UIUserFunctionParameter* pParam );

#include "UICommonFunction.ini"
#include "UILoginFunction.ini"
#include "UIChoiceTrioFunction.ini"

#include "UIChoiceServerFunction.ini"
#include "UIChoiceRoomFunction.ini"
#include "UIReadyRoomFunction.ini"
#include "UIGamePlayFunction.ini"
#include "UIGameResultFunction.ini"
#include "UICreateCharacterFunction.ini"
#include "UIItemShopFunction.ini"
#include "UISkillShopFunction.ini"
#include "UIItemBagFunction.ini"
#include "UISkillBagFunction.ini"

#include "UIChoiceClassProcess.ini"
#include "UIGuideTutorialProcess.ini"
#include "UIReplayMainProcess.ini"
#include "UITutorialMainProcess.ini"

#include "ExceptionReport.h"
#include "TaskKeyHook.h"
#include "ComInclude.h"

CExceptionReport cExceptionReport;

void TestKey( LPVOID )
{
    DisableTaskKeys( TRUE, TRUE );

    while(1)
    {
        Sleep( 10 );
    }

    assert(!"Error!" );
}

#define FRAME_CHECK

bool LostDeviceObserver( void * )
{
	OutputDebugString("LostDeviceObserver \n");

	CGameProject::s_pGameProject->OnLostDevice();
	return true;
}

bool ResetDeviceObserver( bool b, void * )
{
	OutputDebugString("ResetDeviceObserver \n");

	CGameProject::s_pGameProject->OnResetDevice(b);
	return true;
}

CGameProject *CGameProject::s_pGameProject = NULL;

float nsGameConfig::s_fStateSpeed = 1.f;
float nsGameConfig::s_fSlowSpeed = 1.f;
float nsGameConfig::s_fSlowStep = 1.f;
float nsGameConfig::s_fOriginSpeed = 0.f;

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
NiApplication* NiApplication::Create()
{
	char strName[MAX_PATH];
	char strPass[MAX_PATH];

	memset(strName, 0 , sizeof(strName));
	memset(strPass, 0 , sizeof(strPass));

	CGameProject::s_pGameProject = NiNew CGameProject;

	NiApplication::ms_pkCommand->String("id", strName, MAX_PATH);
	NiApplication::ms_pkCommand->String("pass", strPass, MAX_PATH);

	_snprintf(CGameProject::s_pGameProject->m_strName, MAX_PATH, "%s", strName );
	_snprintf(CGameProject::s_pGameProject->m_strPass, MAX_PATH, "%s", strPass );

    NiAppWindow * pkAppWindow = CGameProject::s_pGameProject->GetAppWindow();
    pkAppWindow->SetWindowStyle( ( WS_CAPTION | WS_BORDER ) );
	
	return CGameProject::s_pGameProject;
}

CGameProject::CGameProject(void) : NiApplication("GameProject Application", 1024, 768)
{
#ifdef _DEBUG
//	N3DEBUGSTART("error.log");
#endif
    
#ifndef _DEBUG
    NiFragmentMaterial::SetDefaultAutoCreateProgramCache( false );
    NiFragmentMaterial::SetDefaultAutoSaveProgramCache( false );
    NiFragmentMaterial::SetDefaultLoadProgramCacheOnCreation( false );
    NiFragmentMaterial::SetDefaultLockProgramCache( false );
    NiFragmentMaterial::SetDefaultWriteDebugProgramData( false );    
#endif
//    cExceptionReport.SetProgramName( "XKick" );

	ZeroMemory(&s_Notice, sizeof(s_Notice));
    m_bMoveWindow = true;

	s_nBGM=0;

#ifdef FRAME_CHECK
	SetMaxFrameRate(1000.f);
#else
	SetMaxFrameRate(40.f);
#endif
	SetMediaPath("");
	m_pkShaderHelper = NULL;

	m_bLostDevice = false;
	m_bRestoreDevice = false;

	m_bMultiThread = true;

	m_fFramePerSecond	= 0.f;
/*
#ifdef PROFILER
	m_pkProfiler = NiNew CProfiler;
	NiMetricsLayer::AddOutputModule( m_pkProfiler );
#endif
*/
	m_hCursor = 0;

	m_bPlayingMusic = false;

	m_bFullscreen = false;//?
	m_bWideScreen = false;

	m_bNeedRecreateRenderer = false;

	m_nCaptionSize.x = 0;
	m_nCaptionSize.y = 0;

	m_spAlphaAccumulator = NiNew NiAlphaAccumulator;

    m_bSaveScreenShot = false;

	m_bCursor		 = true;

	m_fGlobalAniSpeed	= 1.0f;

	m_cReg.Init("Enterplay", "XKick");

	if(m_cReg.GetStr( "Version", 0) == NULL)
		_snprintf(m_sClientVersion, STRLEN_32, "5.0.0.0");
	else
		_snprintf(m_sClientVersion, STRLEN_32, m_cReg.GetStr( "Version", 0));
}

CGameProject::~CGameProject(void)
{
}

void CGameProject::SetMoveWindow( bool bEnable )
{
    m_bMoveWindow = bEnable;
}

bool CGameProject::CreateFontAndLabel()
{
	NiFontPtr spFont;
	NiString2DPtr spLabel;

	spFont = NiFont::Create(m_spRenderer, "CourierNew10.NFF" );
	if (!spFont)
		return false;

	NiColorA kColor(1.0f, 0.0f, 0.0f, 1.0f);
	spLabel = NiNew NiString2D(spFont, NiFontString::COLORED,	64, "Draw Test", kColor, 640, 480);

	spLabel->sprintf("Draw Test B \n");
	if (!spLabel)
		return false;

	m_cUtlLabel.SetFont(spFont, m_spRenderer);
	m_cUtlLabel.SetLabel(spLabel);

	return true;
}

NiInputSystem::CreateParams* CGameProject::GetInputCreationParameters()
{
    // Create and initialize parameters for the input system
    NiDI8InputSystem::DI8CreateParams* pkParams = 
        NiNew NiDI8InputSystem::DI8CreateParams;
    assert(pkParams);

    pkParams->SetOwnerInstance(GetInstanceReference());
    pkParams->SetOwnerWindow(GetWindowReference());

    // Set the names to show for the controls in the DI config screen
    pkParams->SetControlMapName(NiDI8InputSystem::AXIS_0_H,
        "TEST: Stick 1 Horz");
    pkParams->SetControlMapName(NiDI8InputSystem::AXIS_0_V,
        "TEST: Stick 1 Vert");
    pkParams->SetControlMapName(NiDI8InputSystem::AXIS_1_H,
        "TEST: Stick 2 Horz");
    pkParams->SetControlMapName(NiDI8InputSystem::AXIS_1_V,
        "TEST: Stick 2 Vert");
    pkParams->SetControlMapName(NiDI8InputSystem::POV_DPAD,
        "TEST: D-Pad (LUP/DOWN/LEFT/RIGHT)");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_RUP,
        "TEST: R UP");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_RDOWN,
        "TEST: R DOWN");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_RLEFT,
        "TEST: R LEFT");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_RRIGHT,
        "TEST: R RIGHT");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_L1,
        "TEST: L1");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_R1,
        "TEST: R1");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_START,
        "TEST: START");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_SELECT,
        "TEST: SELECT");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_L2,
        "TEST: L2");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_R2,
        "TEST: R2");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_A,
        "TEST: A");
    pkParams->SetControlMapName(NiDI8InputSystem::BUTTON_B,
        "TEST: B");

    return pkParams;
}

//NiActionMap* CGameProject::CreateActionMap(bool bEmpty)
//{
//    char* pacName;
//
//    if (bEmpty)
//        pacName = "EmptyMap";
//    else
//        pacName = "InputDemo";
//
//    NiActionMap* pkActionMap = CreateNewActionMap(pacName);
//    if (!pkActionMap)
//        return 0;
//
//    // Nothing to do in the empty one. It's a stub to effectively turn off
//    // action mapping.
//    if (bEmpty)
//        return pkActionMap;
//
//    // Fill in the actions...
//    // This function is pretty long, but it is intended to demonstrate the
//    // wide variety of scenarios action mapping supports.
//    // This makes things a little difficult, as there are only so many
//    // buttons/axes that actions can be assigned to.
//
//    // GamePad-specific actions
//    // As the first action being demonstrated, we will include all the
//    // parameters possible. For most actions, this is not necessary.
//    pkActionMap->AddAction(
//        "StartButton",                      // Name
//        ActionEnum::ACTION_GP_BTN_START,    // AppData
//        NiAction::GP_BUTTON_START,          // Semantic
//        NiAction::RETURN_BOOLEAN,           // Flags
//        0 /* Not device-specific */,        // DeviceID
//        0,                                  // RangeLow
//        0,                                  // RangeHigh
//        0,                                  // Context
//        0,                                  // KeyModifiers
//        0,                                  // MouseModifiers
//        0                                   // PadModifiers
//        );
//
//    // This action uses a modifier...
//    pkActionMap->AddAction("SelectWithR1",
//        ActionEnum::ACTION_GP_BTN_SELECT_WITH_R1,
//        NiAction::GP_BUTTON_SELECT,
//        NiAction::RETURN_BOOLEAN | NiAction::USE_MODIFIERS,
//        0, 0, 0, 0, 0, 0, NiInputGamePad::NIGP_MASK_R1);
//
//    // The following actions map ranges on a given axis 
//    // For the left horizontal, we will map range 0 to -30..-10,
//    // and range 1 to 10..30. (Remeber, the demo sets the analog
//    // stick range to -30..30).
//    pkActionMap->AddAction("LeftHRange0", 
//        ActionEnum::ACTION_GP_LEFT_H_RANGE_0, 
//        NiAction::GP_AXIS_LEFT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -30, -10);
//    pkActionMap->AddAction("LeftHRange1", 
//        ActionEnum::ACTION_GP_LEFT_H_RANGE_1, 
//        NiAction::GP_AXIS_LEFT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 10, 30);
//
//    // For the left vertical, we will map the same ranges at the horizontal
//    pkActionMap->AddAction("LeftVRange0", 
//        ActionEnum::ACTION_GP_LEFT_V_RANGE_0, 
//        NiAction::GP_AXIS_LEFT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -30, -10);
//    pkActionMap->AddAction("LeftVRange1", 
//        ActionEnum::ACTION_GP_LEFT_V_RANGE_1, 
//        NiAction::GP_AXIS_LEFT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 10, 30);
//
//    // For the right horizontal, we will map range 0 to both -20..-10 AND 
//    // 20..10. Range 1 will be mapped to both -30..-21 AND 21..30. This will 
//    // require setting the given action enumeration 2 separate times.
//    pkActionMap->AddAction("RightHRange0_Neg", 
//        ActionEnum::ACTION_GP_RIGHT_H_RANGE_0, 
//        NiAction::GP_AXIS_RIGHT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -20, -10);
//    pkActionMap->AddAction("RightHRange0_Pos", 
//        ActionEnum::ACTION_GP_RIGHT_H_RANGE_0, 
//        NiAction::GP_AXIS_RIGHT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 10, 20);
//    pkActionMap->AddAction("RightHRange1_Neg", 
//        ActionEnum::ACTION_GP_RIGHT_H_RANGE_1, 
//        NiAction::GP_AXIS_RIGHT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -30, -21);
//    pkActionMap->AddAction("RightHRange1_Pos", 
//        ActionEnum::ACTION_GP_RIGHT_H_RANGE_1, 
//        NiAction::GP_AXIS_RIGHT_H, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 21, 30);
//
//    // Ditto for the right vertical.
//    pkActionMap->AddAction("RightVRange0_Neg", 
//        ActionEnum::ACTION_GP_RIGHT_V_RANGE_0, 
//        NiAction::GP_AXIS_RIGHT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -20, -10);
//    pkActionMap->AddAction("RightVRange0_Pos", 
//        ActionEnum::ACTION_GP_RIGHT_V_RANGE_0, 
//        NiAction::GP_AXIS_RIGHT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 10, 20);
//    pkActionMap->AddAction("RightVRange1_Neg", 
//        ActionEnum::ACTION_GP_RIGHT_V_RANGE_1, 
//        NiAction::GP_AXIS_RIGHT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, -30, -21);
//    pkActionMap->AddAction("RightVRange1_Pos", 
//        ActionEnum::ACTION_GP_RIGHT_V_RANGE_1, 
//        NiAction::GP_AXIS_RIGHT_V, 
//        NiAction::RETURN_BOOLEAN | NiAction::GAMEBRYORANGE,
//        0, 21, 30);
//
//    // Device-specific actions
//    // These will be mapped just to the device in their respective ports 
//    // ACTION_DEVSPECIFIC_<PORT>. We will have to add multiple entries,
//    // however, as the device could be a keyboard, mouse, or gamepad.
//    // At this point, we are pretty much out of buttons on the mouse,
//    // so we will use the Left ALT key as the modifier on some of them,
//    // and the Left WIN key on others.
//    //
//    // Note the use of NiInputSystem::MakeDeviceID(Port,Slot) to mux the
//    // proper device ID.
//    //
//    pkActionMap->AddAction("DeviceSpecific0_GamePad", 
//        ActionEnum::ACTION_DEVICE_SPECIFIC_0,
//        NiAction::GP_BUTTON_LLEFT, 
//        NiAction::RETURN_BOOLEAN | NiAction::DEVICE_SPECIFIC,
//        NiInputSystem::MakeDeviceID(0,0));
//
//    pkActionMap->AddAction("DeviceSpecific1_GamePad", 
//        ActionEnum::ACTION_DEVICE_SPECIFIC_1,
//        NiAction::GP_BUTTON_LRIGHT, 
//        NiAction::RETURN_BOOLEAN | NiAction::DEVICE_SPECIFIC,
//        NiInputSystem::MakeDeviceID(1,0));
//
//    pkActionMap->AddAction("DeviceSpecific2_GamePad", 
//        ActionEnum::ACTION_DEVICE_SPECIFIC_2,
//        NiAction::GP_BUTTON_LUP, 
//        NiAction::RETURN_BOOLEAN | NiAction::DEVICE_SPECIFIC,
//        NiInputSystem::MakeDeviceID(2,0));
//    pkActionMap->AddAction("DeviceSpecific3_GamePad", 
//        ActionEnum::ACTION_DEVICE_SPECIFIC_3,
//        NiAction::GP_BUTTON_LDOWN, 
//        NiAction::RETURN_BOOLEAN | NiAction::DEVICE_SPECIFIC,
//        NiInputSystem::MakeDeviceID(3,0));
//
//    return pkActionMap;
//}

//---------------------------------------------------------------------------
bool CGameProject::CreateInputSystem()
{
    // Create and initialize parameters for the input system
    NiInputSystem::CreateParams* pkParams = GetInputCreationParameters();
    assert(pkParams);

    pkParams->SetRenderer(m_spRenderer);
    pkParams->SetGamePadCount(4);
    // Set the range to -30..+30
    pkParams->SetAxisRange(-30, +30);

    // Create the ActionMap for the ActionMapMode part fo the demo.
    //m_spActionMap = CreateActionMap(false);
    //if (!m_spActionMap)
    //    return false;
    //// Also, create an empty ActionMap to swap in when we are NOT 
    //// doing the ActionMapMode part of the demo. This will work because
    //// the input system will update the immediate mode data while it
    //// updates the action map. We will set the empty ActionMap active 
    //// so that the system doesn't collect/report actions having had 
    //// occured.
    //m_spEmptyActionMap = CreateActionMap(true);
    //if (!m_spEmptyActionMap)
    //    return false;

    //// Set the empty action map in the params to have the system setup
    //// for using action maps, but to avoid mapping any actions at this
    //// time. Since the first screen is the Immediate mode screen, we
    //// will not need actions.
    //pkParams->SetActionMap(m_spActionMap);

    // Create the input system
    m_spInputSystem = NiInputSystem::Create(pkParams);
    NiDelete pkParams;
    if (!m_spInputSystem)
        return false;

    // Sleep to give the system time to init.
    NiSleep(750);

    // We are just going to stall here...
    bool bGotDevices = false;
    while (!bGotDevices)
    {
        NiInputErr eErr = m_spInputSystem->CheckEnumerationStatus();
        switch (eErr)
        {
        case NIIERR_ENUM_NOTRUNNING:
            assert(!"EnumerateDevices failed");
            return false;
        case NIIERR_ENUM_FAILED:
            assert(!"CheckEnumerationStatus> FAILED");
            return false;
        case NIIERR_ENUM_COMPLETE:
            bGotDevices = true;
            break;
        case NIIERR_ENUM_NOTCOMPLETE:
        default:
            break;
        }
    }

    // Dump the available devices
    char acTemp[256];
    NiOutputDebugString("Devices found:\n");

    NiInputDevice::Description* pkNiDesc = 
        m_spInputSystem->GetFirstDeviceDesc();
    while (pkNiDesc)
    {
        NiSprintf(acTemp, 256, "    Port %d - %s\n", pkNiDesc->GetPort(),
            pkNiDesc->GetName());
        NiOutputDebugString(acTemp);

        pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
    }

    // Not all game pads are guarenteed to be there...
    unsigned int ui;
    for (ui = 0; ui < NiInputSystem::MAX_GAMEPADS; ui++)
        m_aspGamePad[ui] = m_spInputSystem->OpenGamePad(ui, 0);

    return true;
}

bool CGameProject::RecreateInputSystem()
{
	if(GetInputGamePad()) return true;

	// Create and initialize parameters for the input system
	NiInputSystem::CreateParams* pkParams = GetInputCreationParameters();
	assert(pkParams);

	pkParams->SetRenderer(m_spRenderer);
	pkParams->SetGamePadCount(4);
	// Set the range to -30..+30
	pkParams->SetAxisRange(-30, +30);

	// Create the input system
	m_spInputSystem = NiInputSystem::Create(pkParams);
	NiDelete pkParams;
	if (!m_spInputSystem)
		return false;

	// Sleep to give the system time to init.
	NiSleep(750);

	// We are just going to stall here...
	bool bGotDevices = false;
	while (!bGotDevices)
	{
		NiInputErr eErr = m_spInputSystem->CheckEnumerationStatus();
		switch (eErr)
		{
		case NIIERR_ENUM_NOTRUNNING:
			assert(!"EnumerateDevices failed");
			return false;
		case NIIERR_ENUM_FAILED:
			assert(!"CheckEnumerationStatus> FAILED");
			return false;
		case NIIERR_ENUM_COMPLETE:
			bGotDevices = true;
			break;
		case NIIERR_ENUM_NOTCOMPLETE:
		default:
			break;
		}
	}

	// Dump the available devices
	char acTemp[256];
	NiOutputDebugString("Devices found:\n");

	NiInputDevice::Description* pkNiDesc = 
		m_spInputSystem->GetFirstDeviceDesc();
	while (pkNiDesc)
	{
		NiSprintf(acTemp, 256, "    Port %d - %s\n", pkNiDesc->GetPort(),
			pkNiDesc->GetName());
		NiOutputDebugString(acTemp);

		pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
	}

	// Not all game pads are guarenteed to be there...
	unsigned int ui;
	for (ui = 0; ui < NiInputSystem::MAX_GAMEPADS; ui++)
		m_aspGamePad[ui] = m_spInputSystem->OpenGamePad(ui, 0);

	return true;
}

//---------------------------------------------------------------------------
bool CGameProject::Initialize()
{
    DisableTaskKeys( TRUE, TRUE );   
    
	m_hTopLevelHwnd = GetWindowReference();

	LoadFileConf(".\\GameConfig.conf");
    LoadVideoInfomation();

	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( "GameMode" ));
	assert(pElement);
	if( !NiDX9Select::IsSelectResolution(1280, 720) && !strcmp(pElement->Attribute("Wide"), "true") && !strcmp(pElement->Attribute("Full"), "true") )
		pElement->SetAttribute("Wide", "false");

	m_cTiXmlDocument.SaveFile(".\\GameConfig.conf");

	if (!CreateRenderer())
		return false;

	CreateFontAndLabel();

	if (!CreateInputSystem())
		return false;

	m_pkShaderHelper = NiNew CShaderHelper();
	if (m_pkShaderHelper)
	{
		char* pacProgramDirectory = "";
		m_pkShaderHelper->SetupShaderSystem(pacProgramDirectory);
	}
	printf(";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");

	if (!CreateScene())
	{
		return false;
	}

	m_cObjBallInfoManager.SetRenderer(NiSmartPointerCast(NiDX9Renderer, m_spRenderer));

	m_ObjectModelManager.SetRootNode(m_spScene);

	m_cAIDomainManager.DomainLoading();

	return true;
}

bool CGameProject::CreateScene()
{
	NiApplication::CreateScene();

//	ShowCursor(FALSE);

	m_hCursor = LoadCursor(GetInstanceReference(), MAKEINTRESOURCE(IDC_NORMAL));

	ShowPointer();

	m_spRenderer->SetBackgroundColor(NiColorA(0.7f, 0.7f, 0.7f, 1.f));

//	m_Route.Create();

	NiDX9Renderer::GetRenderer()->AddLostDeviceNotificationFunc( LostDeviceObserver, NULL );
	NiDX9Renderer::GetRenderer()->AddResetNotificationFunc( ResetDeviceObserver, NULL );

	m_DialogManager.CreateScene();
	m_DialogManager.Init(GetWindowReference(), m_uiScreenWidth, m_uiScreenHeight);
	m_DialogManager.SetResourcePath(L"./UI/Source/");
	m_DialogManager.OnCreateDevice( NiDX9Renderer::GetRenderer()->GetD3DDevice() );
	m_DialogManager.OnResetDevice( NiDX9Renderer::GetRenderer()->GetD3DDevice() );
	InitialUserFunction();

	m_AudioManager.Initialize(GetWindowReference());

	m_AudioManager.SetAudioPath(".\\Sound\\", ".\\BGM\\");

	m_ProcessManager.CreateScene();

	//m_cGameOptionDialog.Init();
//?	m_InvitatonDialog.CreateScene();

	if (!m_CameraController.Initialize(m_bWideScreen))
	{
		return false;
	}

	return true;
}

void CGameProject::UpdateInput()
{
    if (!m_spInputSystem)
        return;

    // If there are action maps defined the update with action maps
    // if not then just update all of the devices
    NiInputErr eErr;

    if (m_spInputSystem->GetMode() == NiInputSystem::MAPPED)
        eErr = m_spInputSystem->UpdateActionMap();
    else
        eErr = m_spInputSystem->UpdateAllDevices();

    if (eErr == NIIERR_DEVICELOST)
    {
        // A device has been lost. This typically means that one type of
        // device has been removed, and a different one has been inserted.
        NiOutputDebugString("UpdateInput: A device has been lost.\n");
    }
    else
        if (eErr == NIIERR_DEVICECHANGE)
        {
            // A device change has occured. This typically means that a device
            // has either been removed or inserted.
            NiOutputDebugString("UpdateInput: A device change has occured.\n");
        }

        char acTemp[256];
        NiInputDevice::Description* pkNiDesc;

        // If there was an error, dump what each device is out to the
        // debug output window.
        if (eErr != NIIERR_OK)
        {
            pkNiDesc = m_spInputSystem->GetFirstDeviceDesc();
            while (pkNiDesc)
            {
                NiSprintf(acTemp, 256, "Device in Port %d is a ", 
                    pkNiDesc->GetPort());
                NiInputDevice* pkDevice = 0;

                switch (pkNiDesc->GetType())
                {
                case NiInputDevice::NIID_GAMEPAD:
                    NiStrcat(acTemp, 256, "GAMEPAD");
                    pkDevice = m_spInputSystem->OpenGamePad(
                        pkNiDesc->GetPort(), 0);
                    break;

                default:
                    NiStrcat(acTemp, 256, "UNKNOWN");
                    break;
                }

                NiStrcat(acTemp, 256, ", Status is ");

                if (pkDevice)
                {
                    switch (pkDevice->GetStatus())
                    {
                    case NiInputDevice::READY:
                        NiStrcat(acTemp, 256, "READY");
                        break;
                    case NiInputDevice::BUSY:
                        NiStrcat(acTemp, 256, "BUSY");
                        break;
                    case NiInputDevice::REMOVED:
                        NiStrcat(acTemp, 256, "REMOVED");
                        break;
                    case NiInputDevice::LOST:
                        NiStrcat(acTemp, 256, "LOST");
                        break;
                    default:
                        NiStrcat(acTemp, 256, "UNKNOWN");
                        break;
                    }
                }
                else
                {
                    NiStrcat(acTemp, 256, "DEVICE NOT FOUND");
                }

                NiStrcat(acTemp, 256, ".\n");
                NiOutputDebugString(acTemp);

                pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
            }
        }

        // 
        unsigned int uiChangedPortMask = 0;
        if (eErr != NIIERR_OK)
        {
            pkNiDesc = m_spInputSystem->GetFirstDeviceDesc();
            while (pkNiDesc)
            {
                // Grab the port this description is associated with
                unsigned int uiPort = pkNiDesc->GetPort();
                // 
                switch (pkNiDesc->GetType())
                {
                    // If the description is that of a gamepad, then we do 
                    // the following:
                    // 1. If we think there is a keyboard in that port, we 
                    //    release the keyboard and flag the port as changed.
                    // 2. Ditto for a mouse in that port.
                    // 3. If there is no gamepad at that port, we also flag the
                    //    port as changed.
                case NiInputDevice::NIID_GAMEPAD:
                     if (m_aspGamePad[uiPort] == 0)
                        uiChangedPortMask |= (1 << uiPort);
                    break;

                    // If the description is that of a keyboard, then we do 
                    // the following:
                    // 1. If we have a keyboard, and it is not in that port, we 
                    //    release the keyboard and flag the port as changed.
                    // 2. If there is no keyboard, we flag the port as changed.
                    // 3. If we think there is a mouse in that port, we 
                    //    release the mouse and flag the port as changed.
                    // 4. Ditto for a gamepad in that port.
                default:
                    break;
                }
                pkNiDesc = m_spInputSystem->GetNextDeviceDesc();
            }
        }

        // If the uiChangedPortMask is not zero, we have had at least one
        // device change.
        if (uiChangedPortMask != 0)
        {
            NiSprintf(acTemp, 256, "Port changed mask = 0x%08x.\n", 
                uiChangedPortMask);
            NiOutputDebugString(acTemp);

            // For each port
            for (unsigned int uiPort = 0; uiPort < 4; uiPort++)
            {
                // Was there a change on this one?
                if (uiChangedPortMask & (1 << uiPort))
                {
                    NiInputDevice* pkNewDevice = 0;

                    // Grab the description at the changed port
                    pkNiDesc = m_spInputSystem->GetDeviceDesc(uiPort, 0);

                    // Attempt to open the new device at that port.
                    switch (pkNiDesc->GetType())
                    {
                    case NiInputDevice::NIID_GAMEPAD:
                        // We need to release the gamepad before acquiring a
                        // new one...
                        m_aspGamePad[uiPort] = 0;
                        m_aspGamePad[uiPort] = m_spInputSystem->OpenGamePad(
                            uiPort, 0);
                        if (m_aspGamePad[uiPort])
                            pkNewDevice = m_aspGamePad[uiPort];
                        break;
                    default:
                        NiStrcat(acTemp, 256, "UNKNOWN\n");
                        break;
                    }

                    // If we have gotten a new device, update all the 
                    // info screens.
                    if (pkNewDevice)
                    {
                        //HandleNewDevice(pkNewDevice);
                    }

                    // Output the information to the debug spew
                    NiSprintf(acTemp, 256, "Port %d --> ", uiPort);
                    switch (pkNiDesc->GetType())
                    {
                    case NiInputDevice::NIID_GAMEPAD:
                        NiStrcat(acTemp, 256, "GAMEPAD\n");
                        break;
                    default:
                        NiStrcat(acTemp, 256, "UNKNOWN\n");
                        break;
                    }
                    NiOutputDebugString(acTemp);
                }
            }
        }
}

bool CGameProject::LoadFile(const char* filename)
{
	if( filename[strlen(filename)-1] == 'l' )
	{
		if ( !m_cTiXmlDocument.LoadFileXml( filename ) )
			return false;
	}
	else
	{
		if ( !m_cTiXmlDocument.LoadFile( filename ) )
			return false;
	}

	return true;
}

bool CGameProject::LoadFileConf(const char* filename)
{
	if ( !m_cTiXmlDocument.LoadFileConf( filename ) )
		return false;

	return true;
}

TiXmlElement* CGameProject::GetTiXml(char *pName)
{
	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( pName ));
	assert(pElement);
	return pElement;
}

bool CGameProject::GetWideScreenFromFile()
{
	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( "GameMode" ));
	assert(pElement);
	if(!strcmp(pElement->Attribute("Wide"), "true"))
		return true;
	else
		return false;
}

bool CGameProject::GetFullScreenFromFile()
{
	TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( "GameMode" ));
	assert(pElement);
	if(!strcmp(pElement->Attribute("Full"), "true"))
		return true;
	else
		return false;
}

void CGameProject::SetupGameMode()
{
	m_bFullscreen = GetFullScreenFromFile();
	m_bWideScreen = GetWideScreenFromFile();
}

bool CGameProject::CreateRenderer()
{
    char szCurrentDirectory[MAX_PATH];
    ::GetCurrentDirectory( sizeof(szCurrentDirectory), szCurrentDirectory );

    NiMaterial::SetDefaultWorkingDirectory( szCurrentDirectory );    

	SetupGameMode();
    m_eCurrentFrameBufferMode = this->SelectFrameBufferMode();
    CObjectModelManager::GetPtr()->SetFrameBufferMode( m_eCurrentFrameBufferMode );

	RECT kRect, kRect2;

	GetWindowRect(m_hTopLevelHwnd, &kRect);
	GetClientRect(GetRenderWindowReference(), &kRect2);

	m_nCaptionSize.x = (kRect.right-kRect.left) - kRect2.right ;
	m_nCaptionSize.y = (kRect.bottom-kRect.top) - kRect2.bottom;

	if(m_bWideScreen)
	{
		m_uiScreenWidth = 1280;
		m_uiScreenHeight = 720;
	}
	else
	{
		m_uiScreenWidth = 1024;
		m_uiScreenHeight = 768;
	}

	if( m_bFullscreen)
	{
        SetKeyLock( TRUE );
		SetWindowLong(GetWindowReference(), GWL_STYLE, WS_POPUP);

		m_spRenderer = NiDX9Renderer::Create( m_uiScreenWidth, m_uiScreenHeight, NiDX9Renderer::USE_FULLSCREEN | NiDX9Renderer::USE_MULTITHREADED, 
			//m_hTopLevelHwnd, m_hTopLevelHwnd, D3DADAPTER_DEFAULT, NiDX9Renderer::DEVDESC_PURE, NiDX9Renderer::FBFMT_R5G6B5,//16비트
			m_hTopLevelHwnd, m_hTopLevelHwnd, D3DADAPTER_DEFAULT, NiDX9Renderer::DEVDESC_PURE, NiDX9Renderer::FBFMT_X8R8G8B8,
			NiDX9Renderer::DSFMT_UNKNOWN,NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE,	NiDX9Renderer::SWAPEFFECT_DEFAULT, m_eCurrentFrameBufferMode, 0	);
	}
	else
	{
		TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( "WindowPosition" ));
		assert(pElement);
		int x = atoi(pElement->Attribute( "X" ));
		int y = atoi(pElement->Attribute( "Y" ));
		
        SetKeyLock( FALSE );
		::MoveWindow(GetWindowReference(), x, y, m_uiScreenWidth + m_nCaptionSize.x, m_uiScreenHeight + m_nCaptionSize.y, true );
		SendMessage(m_hTopLevelHwnd, WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(m_uiScreenWidth, m_uiScreenHeight));//윈도우 사이즈 변경

		UpdateWindow(m_hTopLevelHwnd);

		m_spRenderer = NiDX9Renderer::Create( m_uiScreenWidth, m_uiScreenHeight, NiDX9Renderer::USE_MULTITHREADED, GetRenderWindowReference(), 
			//m_hTopLevelHwnd, D3DADAPTER_DEFAULT, NiDX9Renderer::DEVDESC_PURE, NiDX9Renderer::FBFMT_R5G6B5,16비트
			m_hTopLevelHwnd, D3DADAPTER_DEFAULT, NiDX9Renderer::DEVDESC_PURE, NiDX9Renderer::FBFMT_X8R8G8B8,
			NiDX9Renderer::DSFMT_UNKNOWN, NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE, NiDX9Renderer::SWAPEFFECT_DEFAULT, m_eCurrentFrameBufferMode, 0);
	}
    NiDX9Renderer * pkDX9Renderer = (NiDX9Renderer*) NiRenderer::GetRenderer();

	return true;
}

void CGameProject::SetWideScreen(bool bWideScreen) 
{ 
	m_bWideScreen = bWideScreen;
}

void CGameProject::ReCreateRenderer()
{
	if(m_bWideScreen)
	{
		m_uiScreenWidth = 1280;
		m_uiScreenHeight = 720;
	}
	else
	{
		m_uiScreenWidth = 1024;
		m_uiScreenHeight = 768;
	}
    
    m_eCurrentFrameBufferMode = this->SelectFrameBufferMode();
    CObjectModelManager::GetPtr()->SetFrameBufferMode( m_eCurrentFrameBufferMode );

    if(m_bFullscreen)
	{
        //SetWindowLong(m_hTopLevelHwnd, GWL_STYLE, WS_POPUP);
        SetKeyLock( TRUE );

		NiDX9Renderer* pkDX9Renderer = NiSmartPointerCast(NiDX9Renderer, m_spRenderer);
		assert(pkDX9Renderer);

		pkDX9Renderer->Recreate(m_uiScreenWidth, m_uiScreenHeight,(NiDX9Renderer::FlagType)(NiDX9Renderer::USE_FULLSCREEN | NiDX9Renderer::USE_MULTITHREADED), m_hTopLevelHwnd,
			NiDX9Renderer::FBFMT_UNKNOWN, NiDX9Renderer::DSFMT_UNKNOWN, NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE, NiDX9Renderer::SWAPEFFECT_DEFAULT, m_eCurrentFrameBufferMode, 1, NiDX9Renderer::REFRESHRATE_DEFAULT);
	}
	else
	{        
        SetKeyLock( FALSE );
        SetWindowLong(m_hTopLevelHwnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION );

        ::ShowWindow( m_hTopLevelHwnd, SW_MINIMIZE);
        ::ShowWindow( m_hTopLevelHwnd, SW_RESTORE);

		NiDX9Renderer* pkDX9Renderer = NiSmartPointerCast(NiDX9Renderer, m_spRenderer);
		assert(pkDX9Renderer);
		pkDX9Renderer->Recreate(m_uiScreenWidth, m_uiScreenHeight,(NiDX9Renderer::FlagType)(NiDX9Renderer::USE_MULTITHREADED), GetRenderWindowReference(),
			NiDX9Renderer::FBFMT_UNKNOWN, NiDX9Renderer::DSFMT_UNKNOWN, NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE, NiDX9Renderer::SWAPEFFECT_DEFAULT, m_eCurrentFrameBufferMode, 1, NiDX9Renderer::REFRESHRATE_DEFAULT);
	
		RECT kRect, kRect2;
		GetWindowRect(m_hTopLevelHwnd, &kRect);
		GetClientRect(GetRenderWindowReference(), &kRect2);

		m_nCaptionSize.x = (kRect.right-kRect.left) - kRect2.right ;
		m_nCaptionSize.y = (kRect.bottom-kRect.top) - kRect2.bottom;

		TiXmlElement* pElement = dynamic_cast<TiXmlElement*>(m_cTiXmlDocument.FirstChild( "WindowPosition" ));
		assert(pElement);
		int x = atoi(pElement->Attribute( "X" ));
		int y = atoi(pElement->Attribute( "Y" ));

		::MoveWindow(GetWindowReference(), x, y, m_uiScreenWidth + m_nCaptionSize.x, m_uiScreenHeight + m_nCaptionSize.y, true );
		SendMessage(m_hTopLevelHwnd, WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(m_uiScreenWidth, m_uiScreenHeight));//윈도우 사이즈 변경

		UpdateWindow(m_hTopLevelHwnd);
	}

	CCameraController::GetPtr()->SetViewFrustum(m_bWideScreen);

	m_bNeedRecreateRenderer = false;
}

bool CGameProject::OnLostDevice()
{
	m_DialogManager.OnLostDevice();

	m_ProcessManager.OnLostDevice();

	m_bLostDevice = true;
	m_bRestoreDevice = false;
    
    m_ObjectModelManager.OnLostDevice();

	return true;
}

bool CGameProject::OnResetDevice( bool b)
{
    if( b == true )
    {
        return this->OnLostDevice();
    }

    if( b == false )
    {
		m_DialogManager.OnResetDevice( NiDX9Renderer::GetRenderer()->GetD3DDevice() );

		m_ProcessManager.OnResetDevice();
        m_ObjectModelManager.OnResetDevice();

		m_bLostDevice = false;
    }

	return true;
}

//---------------------------------------------------------------------------
bool CGameProject::CreateCamera()
{
	if (! NiApplication::CreateCamera())
		return false;

	return true;
}

//---------------------------------------------------------------------------
// idle processing
//---------------------------------------------------------------------------
void CGameProject::OnIdle(void)
{
	//NiApplication::OnIdle();

	if (!MeasureTime())
		return;

	ResetFrameTimings();

	BeginUpdate();
	UpdateFrame();
	EndUpdate();

	if(m_spRenderer && m_bNeedRecreateRenderer)
		ReCreateRenderer();

	if (m_ProcessManager.m_pCurrentProcess != NULL)
	{
		m_spRenderer->SetSorter( m_spAlphaAccumulator );

		BeginFrame();
		RenderFrame();
        SaveScreenShot();

        if( m_bSaveScreenShot == true )
        {
            m_bSaveScreenShot = false;
            this->SaveSurfaceImage();
        }

		EndFrame();

		m_spRenderer->SetSorter( 0 );
		DisplayFrame();

	}

	m_cUtlLabel.EmptyLabel();
	m_cUtlFps.OnIdle();

	UpdateVisualTrackers();
	UpdateMetrics();

	m_iClicks++;
}

void CGameProject::SaveScreenShot()
{
    if( IsPrintScreenKeyPress() == TRUE ) 
    {
        m_bSaveScreenShot = true;
        SetPrintScreenKeyPress( FALSE );
    }    
}

void CGameProject::SetScreenShotFileName( const char * pScreenShotFileName )
{
    m_strScreenShotFileName = pScreenShotFileName;
}

void CGameProject::SaveSurfaceImage()
{
    NiDX9Renderer * pkRenderer = (NiDX9Renderer*) NiRenderer::GetRenderer();
    NiRenderTargetGroup * pkRenderTarget = pkRenderer->GetDefaultRenderTargetGroup();

    NiDX92DBufferData* pkBuffData = NiDynamicCast( NiDX92DBufferData, (NiDX92DBufferData*) pkRenderTarget->GetBufferRendererData(0) );
    assert(pkBuffData);

    LPDIRECT3DSURFACE9 pkShotSurf = pkBuffData->GetSurface();

    struct tm* today;
    time_t ltime;

    time(&ltime);
    today = localtime(&ltime);  // 현재시간을 구하는

    char file_name[MAX_PATH];

    int nYear  = today->tm_year + 1900;
    int nMonth = today->tm_mon + 1;
    int nDay   = today->tm_mday;
    int nHour  = today->tm_hour;
    int nMin   = today->tm_min;
    int nSec   = today->tm_sec;

    sprintf(file_name, ".\\ScreenShot\\%04d%02d%02d%02d%02d%02d.jpg", nYear, nMonth, nDay, nHour, nMin, nSec );
    D3DXSaveSurfaceToFile( file_name, D3DXIFF_JPG, pkShotSurf, 0, 0 );
}

//---------------------------------------------------------------------------
void CGameProject::UpdateFrame()
{
	NiApplication::UpdateFrame();

	m_AudioManager.Update();

	static float s_fAccumTime = m_fAccumTime;

	m_fFrameTime = m_fFrameTime * nsGameConfig::s_fSlowSpeed;
	s_fAccumTime = s_fAccumTime + m_fFrameTime;

	if (!m_ProcessManager.UpdateFrame(m_fFrameTime, s_fAccumTime))
	{
		QuitApplication();
	}

/*	if(timeGetTime() - m_nHackTime > 60000)
	{
		m_nHackTime = timeGetTime();
		if(CheckHack())
		{
			MessageBox(0, "해킹툴 사용하지마삼", "Kicks", MB_OK);
			QuitApplication();
		}
	}
*/
	if(!m_cPacketManager.SavePacketInList())
	{
		QuitApplication();
	}
	m_cPacketManager.PacketProcess(); //저장된 패킷을 적용
 }

void CGameProject::RenderFrame()
{
	if (m_bLostDevice == false)
	{
		m_ProcessManager.RenderFrame();
	}
}

//---------------------------------------------------------------------------
void CGameProject::ProcessInput()
{
    m_ProcessManager.ProcessInput();
    NiInputGamePad * pkGamePad = 0;
    for( int i=0; i<NiInputSystem::MAX_GAMEPADS; i++ )
    {
        if( m_aspGamePad[i] != 0 ) 
        {
            pkGamePad = m_aspGamePad[i];
            break;
        }
    }
    
    if( pkGamePad == 0 ) return;

    int iHorz, iVert;
    pkGamePad->GetStickValue(NiInputGamePad::NIGP_STICK_LEFT, iHorz, iVert);

    char buf[128];
    if( iHorz != 0 )
    {
        sprintf( buf, "Left Stick Horiz Value : %d\n", iHorz );
        printf( buf );
    }

    if( iVert != 0 )
    {
        sprintf( buf, "Left Stick Vert Value : %d\n", iVert );
        printf( buf );
    }

    pkGamePad->GetStickValue(NiInputGamePad::NIGP_STICK_RIGHT, iHorz, iVert);

    if( iHorz != 0 )
    {
        sprintf( buf, "Right Stick Horiz Value : %d\n", iHorz );
        printf( buf );
    }

    if( iVert != 0 )
    {
        sprintf( buf, "Right Stick Vert Value : %d\n", iVert );
        printf( buf );
    }

    for (unsigned int ui = NiInputGamePad::NIGP_LUP;  ui < NiInputGamePad::NIGP_NUMBUTTONS; ui++)
    {
        // Determine if the button is pressed (digital)
        bool bDown = pkGamePad->ButtonIsDown((NiInputGamePad::Button)ui);
        // Get the analog value of the button
        unsigned char ucValue = pkGamePad->ButtonState((NiInputGamePad::Button)ui);

        if( bDown == true )
        {            
            sprintf( buf, "Button : %d Value : %d\n", ui, ucValue );
            printf( buf );
        }
    }
}

NiInputGamePad* CGameProject::GetInputGamePad()
{
	NiInputGamePad * pkGamePad = NULL;
	for( int i=0; i<NiInputSystem::MAX_GAMEPADS; i++ )
	{
		if( m_aspGamePad[i] != 0 ) 
		{
			return m_aspGamePad[i];
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------
bool CGameProject::OnDefault(NiEventRef pEventRecord)
{
    if( m_bMoveWindow == false )
    {    
        if( pEventRecord->uiMsg == WM_NCLBUTTONDOWN ) return true;
    }

    if (pEventRecord->uiMsg == WM_SETCURSOR)
    {
        SetCursor(m_hCursor);
        return false;
    }

    if (!m_ProcessManager.OnDefault(pEventRecord))
    {
    }

	return false;
}

//---------------------------------------------------------------------------
void CGameProject::Terminate()
{
	if (m_pkShaderHelper != NULL)
	{
		NiDelete m_pkShaderHelper;
		m_pkShaderHelper = NULL;
	}

	m_ProcessManager.Terminate();

	m_CameraController.Terminate();

	m_cObjPlayerManager.Terminate();

	m_DialogManager.OnLostDevice();
	m_DialogManager.OnDestroyDevice();
	m_DialogManager.Terminate();

	//?	m_PacketManager.Terminate();

	m_AudioManager.Terminate();

	m_cGameOptionDialog.Terminate();
	//?	m_InvitatonDialog.Terminate();
}

void CGameProject::InitialUserFunction()
{
#ifdef USERFUNCTION
#undef USERFUNCTION
#endif

#define USERFUNCTION( functionname ) \
	{ \
	CClientUIManager::GetPtr()->AddUserFunction( #functionname, N3UI_##functionname ); \
	}
#include "UICommonFunction.ini"
#include "UILoginFunction.ini"
#include "UIChoiceTrioFunction.ini"
#include "UIChoiceServerFunction.ini"
#include "UIChoiceRoomFunction.ini"
#include "UICreateCharacterFunction.ini"
#include "UIReadyRoomFunction.ini"
#include "UIGamePlayFunction.ini"
#include "UIGameResultFunction.ini"

#include "UIItemShopFunction.ini"
#include "UISkillShopFunction.ini"
#include "UIItemBagFunction.ini"
#include "UISkillBagFunction.ini"

#include "UIChoiceClassProcess.ini"
#include "UIGuideTutorialProcess.ini"
#include "UIReplayMainProcess.ini"
#include "UITutorialMainProcess.ini"
}

BOOL CGameProject::GetScreenMode()
{
	return m_bWideScreen;
}

void CGameProject::HidePointer()
{
	if(m_bCursor == true)
	{
		m_bCursor = false;
		ShowCursor(false);
	}
}
//---------------------------------------------------------------------------
void CGameProject::ShowPointer()
{
	if(m_bCursor == false)
	{
		m_bCursor = true;
		ShowCursor(true);
	}
}

NiDX9Renderer::FramebufferMode CGameProject::SelectFrameBufferMode()
{
    std::vector<NiDX9Renderer::FramebufferMode> kSupportFrameBufferMode;

    kSupportFrameBufferMode.push_back( NiDX9Renderer::FramebufferMode::FBMODE_DEFAULT );

    D3DFORMAT eFormat = NiDX9Renderer::GetD3DFormat( NiDX9Renderer::FBFMT_R5G6B5 );
    const NiDX9SystemDesc * pkSystemDesc = NiDX9Renderer::GetSystemDesc();
    const NiDX9AdapterDesc * pkAdapter = pkSystemDesc->GetAdapter(0);
    const NiDX9DeviceDesc * pkDeviceDesc = pkAdapter->GetDevice(D3DDEVTYPE_HAL);

    const NiDX9DeviceDesc::DisplayFormatInfo* pkDFI = pkDeviceDesc->GetFormatInfo( eFormat );

    NiDX9Renderer::DepthStencilFormat eNiDSFormat = pkDFI->FindClosestDepthStencil(32, 0);
    D3DFORMAT eDSFormat = NiDX9Renderer::GetD3DFormat(eNiDSFormat);

    for( int i=2; i<17; i++ )
    {
        D3DMULTISAMPLE_TYPE eType = (D3DMULTISAMPLE_TYPE)i;

        if (pkDFI->IsMultiSampleValid(!m_bFullscreen, eType, eFormat, eDSFormat))
        {
            kSupportFrameBufferMode.push_back( (NiDX9Renderer::FramebufferMode) i );
        }
    }
    
    VideoInfomation::eMultiSampleQuality eQuality = VideoInfomation::GetPtr()->GetAntiAliasing();
    unsigned int nIndex;

    switch( eQuality )
    {
    case VideoInfomation::eMultiSampleQuality::eLowestMultiSampleQuality:
        nIndex = 0;
    break;

    case VideoInfomation::eMultiSampleQuality::eMiddleMultiSampleQuality:
        nIndex = kSupportFrameBufferMode.size() / 2;
    break;

    case VideoInfomation::eMultiSampleQuality::eHighestMultiSampleQuality:
        nIndex = kSupportFrameBufferMode.size() - 1;
    break;

    default:
        assert( !"MultiSample Quality Error !" );
        return NiDX9Renderer::FramebufferMode::FBMODE_DEFAULT;
    break;
    }
    
    return kSupportFrameBufferMode[ nIndex ];
}
