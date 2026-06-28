#include "stdafx.h"
#include "ProcessManager.h"

#include "BaseProcess.h"

#include "FrameLoadingProcess.h"
#include "GameLoginProcess.h"
#include "ChoiceTrioProcess.h"
#include "CreateCharacterProcess.h"
#include "ChoiceServerProcess.h"
#include "ChoiceRoomProcess.h"
#include "ReadyRoomProcess.h"
#include "GamePlayProcess.h"
#include "GameResultProcess.h"

#include "ItemShopProcess.h"
#include "SkillShopProcess.h"
#include "ItemBagProcess.h"
#include "SkillBagProcess.h"
#include "TutorialMainProcess.h"

#include "TutorialPlay1Process.h"
#include "TutorialPlay2Process.h"
#include "TutorialPlay3Process.h"
#include "TutorialPlay4Process.h"

#include "TutorialLoadingProcess.h"

#include "UtlLog.h"
#include "UtlLabel.h"
#include "CoreApi.h"
#include "Common.h"

#include "PutPacket.h"

CProcessManager::CProcessManager(void)
{
	m_nNextCmdType		= GS_CMD_NONE;
	m_pNextProcess		= NULL;
	m_pCurrentProcess	= NULL;
	m_nBeforeProcessCmd	= PROCESS_NONE;
	m_pBeforeProcess	= NULL;
	m_bFpsRender		= false;
//#ifdef _DEBUG
#ifdef _PACKING
#else
	SetConsolePrint();
#endif
//#endif
}

CProcessManager::~CProcessManager(void)
{
}

void CProcessManager::Terminate()
{
	m_pCurrentProcess = NULL;
	while (!m_ProcessStack.empty())
	{
		CBaseProcess* pProcess = m_ProcessStack.top();
		pProcess->Terminate();
		delete pProcess;
		m_ProcessStack.pop();
	}
}

bool CProcessManager::CreateScene()
{
	SetNextProcess(new CFrameLoadingProcess);

	return true;
}

bool CProcessManager::OnLostDevice()
{
	if (m_pCurrentProcess != NULL)
	{
		m_pCurrentProcess->BaseOnLostDevice();
	}
	return true;
}

bool CProcessManager::OnResetDevice()
{
	if (m_pCurrentProcess != NULL)
	{
		m_pCurrentProcess->BaseOnResetDevice();
	}
	return true;
}

void CProcessManager::SetNextProcess(CBaseProcess* pProcess, unsigned int nCmdType)
{
	if (m_pCurrentProcess != NULL)
	{
		m_nBeforeProcessCmd = m_pCurrentProcess->GetProcessCmd();
	}
	m_pNextProcess	= pProcess;
	m_nNextCmdType	= nCmdType;
	
	CUIControl::Set_MouseOveredControl(NULL);
	CUIControl::Set_FocusedControl(NULL);
}

void CProcessManager::SetChangeProcess(CBaseProcess* pProcess, unsigned int nCmdType)
{
	if (m_pCurrentProcess != NULL)
	{
		m_nBeforeProcessCmd = m_pCurrentProcess->GetProcessCmd();
	}
	m_pNextProcess = pProcess;
	m_nNextCmdType = GS_CMD_NONE;

	CUIControl::Set_MouseOveredControl(NULL);
	CUIControl::Set_FocusedControl(NULL);

	DeleteCurrentProcess();
	CreateCurrentProcess();
}

void CProcessManager::CreateCurrentProcess()
{
	assert(!m_pCurrentProcess);
	m_pCurrentProcess = m_pNextProcess;
	if (m_pCurrentProcess != NULL)
	{
		m_ProcessStack.push(m_pCurrentProcess);
		if (m_pCurrentProcess != NULL)
		{
			m_pCurrentProcess->CreateScene();

			m_pCurrentProcess->InitToolTipList();
		}
	}
};

void CProcessManager::DeleteCurrentProcess()
{
	m_pCurrentProcess = NULL;
	if (!m_ProcessStack.empty())
	{
		CBaseProcess* pProcess = m_ProcessStack.top();
		pProcess->Terminate();
		delete pProcess;
		m_ProcessStack.pop();
	}
}

bool CProcessManager::UpdateFrame(float fFrameTime, float fAccumTime)
{
	char temp[STRLEN_128];
	unsigned int uiTrianglesDrawn;

	//uiTrianglesDrawn = m_kDemo.m_uiNumMeshes * GetCurrentModel()->m_uiTriangleCount;

	//NiSprintf(temp, 512, "Triangles drawn: %d (%d / sec)", uiTrianglesDrawn, uiTrianglesDrawn * UtlFps::GetPtr()->GetFps());

	sprintf( temp, "FPS : %d", CUtlFps::GetPtr()->GetFps() );

	if(m_bFpsRender) CUtlLabel::GetPtr()->AddLabelString(temp);

//	if (m_nNextProcess != PROCESS_NONE)
	if (m_nNextCmdType != GS_CMD_NONE)
	{
		switch(m_nNextCmdType)
		{
		case GS_CMD_CHANGE:
			{
				DeleteCurrentProcess();

				CreateCurrentProcess();
			}
			break;
		case GS_CMD_PUSH:
			{
				CreateCurrentProcess();
			}
			break;
		case GS_CMD_POP:
			{
				assert(!m_ProcessStack.empty());
				DeleteCurrentProcess();

				if (!m_ProcessStack.empty())
				{
					m_pCurrentProcess = m_ProcessStack.top();
				}
			}
			break;
		case GS_CMD_DELETE:
			{
				DeleteCurrentProcess();
			}
			break;
		}
		m_pNextProcess = NULL;
		m_nNextCmdType = GS_CMD_NONE;
	}

	if (m_pCurrentProcess != NULL)
	{
		m_pCurrentProcess->UpdateFrame(fFrameTime, fAccumTime);
	}

	return true;
}

void CProcessManager::RenderFrame()
{
	if (m_pCurrentProcess != NULL)
	{
		m_pCurrentProcess->RenderBefore();
		m_pCurrentProcess->RenderFrame();
	}
	else
	{

	}
}

void CProcessManager::ProcessInput()
{
	if (m_pCurrentProcess != NULL)
		m_pCurrentProcess->ProcessInput();
}

bool CProcessManager::OnDefault(NiEventRef pEventRecord)
{
	switch( pEventRecord->uiMsg )
	{
		case WM_KEYDOWN:
		{
			if(pEventRecord->wParam == 80)
			{
				m_bFpsRender = !m_bFpsRender;
			}
		}
		break;
	}

	if (m_pCurrentProcess != NULL)
		return m_pCurrentProcess->OnDefault(pEventRecord);
	return false;
}

void CProcessManager::AttachItem(CObjPlayerInfo* pAnyPlayer, float nAlpha)
{
	int j = 0;

    assert( pAnyPlayer->m_pModel != 0 );
    if( pAnyPlayer->m_pModel == 0 ) return;

    OutputDebugString( "AttachItem Start!\n" );

    pAnyPlayer->m_pModel->m_pkBoneLODController = 0;

	for(int i = 0; i < MAX_EQUIP; i++)
	{
		if(pAnyPlayer->m_nEquipWear[i] > 0)
		{
			if(i == ITEM_EQUIP_FACE)
			{
				pAnyPlayer->m_cShape.m_nSkin = pAnyPlayer->m_nEquipWear[i]%100;
			}

            char buf[128];
            sprintf( buf, "Item Code : %d\n", pAnyPlayer->m_nEquipWear[i] );
            OutputDebugString( buf );

			pAnyPlayer->m_pEquipWearModelID[i] = CObjectModelManager::GetPtr()->ItemLoadStickCharacter(pAnyPlayer->m_nEquipWear[i], pAnyPlayer->m_pModel, pAnyPlayer->m_cShape.m_nSkin);
		}
	}

    OutputDebugString( "AttachItem End\n" );

	AttachObject(pAnyPlayer);
    
    NiBoneLODController * pkBoneLODController = pAnyPlayer->m_pModel->m_pkBoneLODController;
    //assert(  pkBoneLODController != 0 );
    if( pkBoneLODController == 0 )
    {
        OutputDebugString( "Bone LOD Controller Not Found! \n" );
    }

    if( pkBoneLODController ) pkBoneLODController->ChangeBones( pAnyPlayer->m_pModel->m_spNode ); //BoneLOD : pAnyPlayer->m_pModel->m_spNode 에 있는 본으로 교체한다.

    NiNode * pkModelNode = pAnyPlayer->m_pModel->m_spNode;
    assert( pkModelNode != 0 );
    if( pkModelNode == 0 ) return;

    char buf[1024];
    sprintf( buf, "Bone LOD Controller %x \n", pkBoneLODController );
    OutputDebugString( buf );

    if( pkBoneLODController ) 
    {    
        NiBoneLODController::NiTriBasedGeomSet kGeoms;
        NiBoneLODController::NiSkinInstanceSet kSkins;

        pkBoneLODController->GetSkinData( kGeoms, kSkins );

        int nCountLOD = pkBoneLODController->GetNumberOfBoneLODs();
        for( unsigned int i=0; i<nCountLOD; i++ )
        {   
            pkBoneLODController->SetBoneLOD( i );        
            for( unsigned int iGeom = 0; iGeom < kGeoms.GetSize(); iGeom++ )
            {
                NiTriBasedGeom * pkGeom = kGeoms.GetAt(iGeom);           
            
                NiGeometryData * pkGeomData = pkGeom->GetModelData();

                unsigned int nKeepFlag = pkGeomData->GetKeepFlags();

                pkGeomData->SetKeepFlags( NiGeometryData::KEEP_ALL );
            
                CoreApi_RecursiveSetNameToGeometry( pkGeom, "TestBoneLOD" );
                CoreApi_RecursivePrepack( pkGeom );
            }
        }

        pkBoneLODController->SetBoneLOD(0);
    }

    NiNode * pkRoot = (NiNode*) pkModelNode->GetObjectByName( "root" );
    NiNode * pkRootNonAccum = (NiNode*) pkModelNode->GetObjectByName( "root NonAccum" );

    pkRoot->SetSelectiveUpdateRigid( false );
    pkRootNonAccum->SetSelectiveUpdateRigid( false );

    RemoveItemDivest( pkModelNode );

    //CoreApi_RecursiveSpecularEnable( pkModelNode, true );
    CoreApi_RecursivePrepack( pkModelNode );

    if( nAlpha < 1.0f )
    {    
        CoreApi_RecursiveSetVertexColorProperty( pkModelNode, NiVertexColorProperty::SourceVertexMode::SOURCE_IGNORE );
        CoreApi_RecursiveSetAlphaPropertyToMesh( pkModelNode, eTrue, eFalse );
	    CoreApi_RecursiveSetAlphaToMesh( pkModelNode, nAlpha );
    }
    
    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();
    bool bReflectionMap = true;
    if( pkVideoInfomation->GetLightMap() == 0 )
    {
        bReflectionMap = false;
    }
    CoreApi_RecursiveOnOffEnvironmentMap( pkModelNode, bReflectionMap );

    bool bSpecular = true;
    if( pkVideoInfomation->GetSpecular() == 0 )
    {
        bSpecular = false;        
    }
    CoreApi_RecursiveSpecularEnable( pkModelNode, bSpecular );//게임 옵션에 따라 캐릭터의 Specular Light 을 켜거나 끈다 추후에는 게임에만 적용되게 게임로딩부로 옴겨야 된다

    pkModelNode->UpdateProperties();
    pkModelNode->UpdateEffects();
    pkModelNode->Update(0.0f);

    //CoreApi_PrintRigidUpdate( pkModelNode->GetObjectByName( "root" ) );
}

void CProcessManager::AttachObject(CObjPlayerInfo* pAnyPlayer)
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	if(pRoom->m_nBallCode > -1)
	{
		//pAnyPlayer->ObjectPushBack(CObjectModelManager::GetPtr()->BallLoadStickCharacter(pRoom->m_nBallCode, pAnyPlayer->m_pModel, "bone_ball"));
		CObjectModelManager::GetPtr()->BallLoadStickCharacter(pRoom->m_nBallCode, pAnyPlayer->m_pModel, "bone_ball");
	}
}

void CProcessManager::Add_PopUp(const char* sTargetControl)
{
	GetPtr()->m_pCurrentProcess->Add_PopUp(sTargetControl);
}
void CProcessManager::Add_PopUpHoverArea(const char* sTargetControl, const char* sControlName)
{
	GetPtr()->m_pCurrentProcess->Add_PopUpHoverArea(sTargetControl,sControlName);
}

CBaseProcess* CProcessManager::Get_CurrentProcess() const
{
	return m_pCurrentProcess;
}

void CProcessManager::ChangeTutorialeMachine(int nTutorialType)
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();
	CTutorialPlayProcess	*pTutorialMachine;

	switch(nTutorialType) 
	{
	case TUTORIAL_1TYPE:
		pTutorialMachine = new CTutorialPlay1Process();
		break;
	case TUTORIAL_2TYPE:
		pTutorialMachine = new CTutorialPlay2Process();
		break;
	case TUTORIAL_3TYPE:
		pTutorialMachine = new CTutorialPlay3Process();
		break;
	case TUTORIAL_4TYPE:
		pTutorialMachine = new CTutorialPlay4Process();
		break;
	case TUTORIAL_5TYPE:
	case TUTORIAL_6TYPE:
	case TUTORIAL_7TYPE:
		{
			char sText[STRLEN_64]="";
			sprintf(sText,".\\Script\\Tutorial%d.xml", nTutorialType);

			CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

			pRoom->SetParentSeq(pSelectPlayerInfo->GetObjSeq());
			pRoom->m_nMode = ROOM_MODE_TUTORIAL;
			pRoom->m_nQuestCode = nTutorialType;

			CCSCreateRoom		cPacket;

			_snprintf(cPacket.m_sTitle, TITLE_NAME_SIZE, "%s", pRoom->m_sTitle );
			_snprintf(cPacket.m_sPass, 5, "%s", pRoom->m_sPass );
			cPacket.m_nState			= pRoom->m_nState;
			cPacket.m_nMode				= pRoom->m_nMode;
			cPacket.m_nAttackCode		= pRoom->m_nAttackCode;
			cPacket.m_nScaleCode		= pRoom->m_nScaleCode;
			cPacket.m_nAICode			= pRoom->m_nAICode;
			cPacket.m_nPointCode		= pRoom->m_nPointCode;
			cPacket.m_nStartLevel		= pRoom->m_nStartLevel;
			cPacket.m_nEndLevel			= pRoom->m_nEndLevel;

			cPacket.m_nCheckClub		= pRoom->m_nCheckClub;
			cPacket.m_nCheckTime		= pRoom->m_nCheckTime;
			cPacket.m_nCheckWeather		= pRoom->m_nCheckWeather;
			cPacket.m_nCheckViewChat	= pRoom->m_nCheckViewChat;
			cPacket.m_nCheckView		= pRoom->m_nCheckView;
			cPacket.m_nMaxCount			= pRoom->m_nScaleCode * 2;

			cPacket.m_nHomePosition[0]	= POSITION_ALL;
			cPacket.m_nAwayPosition[0]	= POSITION_NONE;
			cPacket.m_nHomePosition[1]	= cPacket.m_nAwayPosition[1] = POSITION_NONE;
			cPacket.m_nHomePosition[2]	= cPacket.m_nAwayPosition[2] = POSITION_NONE;
			cPacket.m_nHomePosition[3]	= cPacket.m_nAwayPosition[3] = POSITION_NONE;
			cPacket.m_nHomePosition[4]	= cPacket.m_nAwayPosition[4] = POSITION_NONE;
			cPacket.m_nHomePosition[5]	= cPacket.m_nAwayPosition[5] = POSITION_NONE;

			PutCreateRoom(&cPacket);

			CObjPlayerManager::GetPtr()->SetPlayerLists(pSelectPlayerInfo->Clone(), pSelectPlayerInfo->m_nTeam);

			CProcessManager::GetPtr()->SetNextProcess(new CTutorialLoadingProcess(sText, nTutorialType));
			return;
		}
	}

	assert(pTutorialMachine);

	SetNextProcess(pTutorialMachine);
}