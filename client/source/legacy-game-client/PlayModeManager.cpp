#include "stdafx.h"
#include "PlayModeManager.h"
#include "ObjNpcInfo.h"
#include "ResBallFormat.h"
#include "EffectManager.h"
#include "GameOptionDialog.h"
#include "../RenderingEngine/CoreApi.h"
#include "UICommonMethod.h"

CPlayModeManager::CPlayModeManager(void)
{
	m_nRobotCount = 0;
	m_pTiXmlNode = NULL;
	LoadFile(".\\Script\\GameScript.xml");

	//m_nBallTeam = PLAYER_TEAM_NONE;
	//m_nBallForm = 0;
}

CPlayModeManager::~CPlayModeManager(void)
{
}

void CPlayModeManager::Init(CObjGroundInfo* pObjGroundInfo, int nPlayType)
{
	m_nRobotCount = -1;

	m_pObjGroundInfo = pObjGroundInfo;

	m_pTiXmlNode = SetupWindow(nPlayType);

	SetRoomInfo();

	SetupUI();

	SetupMapLoading();

	SetupMapObjectLoading();

	SetupBallLoading();

	SetupCameraLoading();

	SetupNpcLoading();

	SetupKeyLoading();

	SetupRobotLoading();
}

void CPlayModeManager::Init(CObjGroundInfo* pObjGroundInfo, const char* filename, int nPlayType)
{
	m_nRobotCount = -1;

	m_pObjGroundInfo = pObjGroundInfo;

	LoadFile(filename);

	m_pTiXmlNode = SetupWindow(nPlayType);

	SetRoomInfo();

	SetupUI();

	SetupMapLoading();

	SetupMapObjectLoading();

	SetupBallLoading();

	SetupCameraLoading();

	SetupNpcLoading();

	SetupKeyLoading();

	SetupRobotLoading();
}

void CPlayModeManager::InitPlayerPointSetting()
{
	CObjPlayerInfo*		pAnyPlayer;
	VectorAnyoneList*	pAnyoneList		= &(CObjPlayerManager::GetPtr()->m_vTotalList);
	CObjBallInfo*		pBall		= CObjBallManager::GetPtr()->GetBallPointer();

	for(VectorAnyoneList::iterator iAnyone=pAnyoneList->begin();iAnyone != pAnyoneList->end();++iAnyone)
	{
		for(VectorPlayerList::iterator iPlayer=(*iAnyone)->begin();iPlayer != (*iAnyone)->end();++iPlayer)
		{
			pAnyPlayer = (*iPlayer);
			if(pAnyPlayer == NULL)		continue;

			SetupCharacterPoint(pAnyPlayer, pAnyPlayer->m_nTeam*100+pAnyPlayer->m_nForm, pBall->m_nGoalTeam - 1);

			pAnyPlayer->m_cCurrentAction.m_nAniCode = STAND_CODE;
		}
	}
}

void CPlayModeManager::Update()
{

}

void CPlayModeManager::Rander()
{

}

void CPlayModeManager::Terminate()
{

}

void CPlayModeManager::ReSetupWindow(int nPlayType)
{
	m_pTiXmlNode = SetupWindow(nPlayType);
}

bool CPlayModeManager::LoadFile(const char* filename)
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

TiXmlNode* CPlayModeManager::SetupWindow( int nProcessCmd )
{
	TiXmlNode* pChild = m_cTiXmlDocument.FirstChild( "Window" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if ( atoi(pElement->Attribute( "Type" )) == nProcessCmd  )
		{
			return pElement;
		}
		pChild = pChild->NextSibling( "Window" );
	}
	return NULL;
}

void CPlayModeManager::SetRoomInfo()
{
	CObjBallInfo* pBall = CObjBallManager::GetPtr()->GetBallPointer();

	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "RoomInfo" );
	while( pChild )
	{
		CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if     (!strcmp(pElement->Attribute("State"), "ROOM_STATE_EMPTY"))		pRoom->m_nState = ROOM_STATE_EMPTY;
		else if(!strcmp(pElement->Attribute("State"), "ROOM_STATE_NORMAL"))		pRoom->m_nState = ROOM_STATE_NORMAL;
		else if(!strcmp(pElement->Attribute("State"), "ROOM_STATE_SECRET"))		pRoom->m_nState = ROOM_STATE_SECRET;
		
		if     (!strcmp(pElement->Attribute("Mode"), "ROOM_MODE_NORMAL"))		pRoom->m_nMode = ROOM_MODE_NORMAL;
		else if(!strcmp(pElement->Attribute("Mode"), "ROOM_MODE_TRAINING"))		pRoom->m_nMode = ROOM_MODE_TRAINING;
		else if(!strcmp(pElement->Attribute("Mode"), "ROOM_MODE_QUEST"))		pRoom->m_nMode = ROOM_MODE_QUEST;
		else if(!strcmp(pElement->Attribute("Mode"), "ROOM_MODE_TUTORIAL"))		pRoom->m_nMode = ROOM_MODE_TUTORIAL;
		else if(!strcmp(pElement->Attribute("Mode"), "ROOM_MODE_TOURNAMENT"))	pRoom->m_nMode = ROOM_MODE_TOURNAMENT;

		if     (!strcmp(pElement->Attribute("ScaleCode"), "ROOM_SCALE_5"))		pRoom->m_nScaleCode = ROOM_SCALE_5;
		else if(!strcmp(pElement->Attribute("ScaleCode"), "ROOM_SCALE_6"))		pRoom->m_nScaleCode = ROOM_SCALE_6;

		if     (!strcmp(pElement->Attribute("TimeCode"), "ROOM_TIME_SUNRISE"))	pRoom->m_nTimeCode = ROOM_TIME_SUNRISE;
		else if(!strcmp(pElement->Attribute("TimeCode"), "ROOM_TIME_DAYLIGHT"))	pRoom->m_nTimeCode = ROOM_TIME_DAYLIGHT;
		else if(!strcmp(pElement->Attribute("TimeCode"), "ROOM_TIME_SUNSET"))	pRoom->m_nTimeCode = ROOM_TIME_SUNSET;
		else if(!strcmp(pElement->Attribute("TimeCode"), "ROOM_TIME_NIGHT"))	pRoom->m_nTimeCode = ROOM_TIME_NIGHT;
		else if(!strcmp(pElement->Attribute("TimeCode"), "ROOM_TIME_AUTO"))		pRoom->m_nTimeCode = ROOM_TIME_AUTO;

		if     (!strcmp(pElement->Attribute("WeatherCode"), "ROOM_WEATHER_FINE"))	pRoom->m_nWeatherCode = ROOM_WEATHER_FINE;
		else if(!strcmp(pElement->Attribute("WeatherCode"), "ROOM_WEATHER_CLOUDY"))	pRoom->m_nWeatherCode = ROOM_WEATHER_CLOUDY;
		else if(!strcmp(pElement->Attribute("WeatherCode"), "ROOM_WEATHER_RAIN"))	pRoom->m_nWeatherCode = ROOM_WEATHER_RAIN;
		else if(!strcmp(pElement->Attribute("WeatherCode"), "ROOM_WEATHER_SNOW"))	pRoom->m_nWeatherCode = ROOM_WEATHER_SNOW;
		else if(!strcmp(pElement->Attribute("WeatherCode"), "ROOM_WEATHER_AUTO"))	pRoom->m_nWeatherCode = ROOM_WEATHER_AUTO;

		if     (!strcmp(pElement->Attribute("AttackCode"), "ROOM_ATTACK_ALTERNATE"))	pRoom->m_nAttackCode = ROOM_ATTACK_ALTERNATE;
		else if(!strcmp(pElement->Attribute("AttackCode"), "ROOM_ATTACK_RANDOM"))		pRoom->m_nAttackCode = ROOM_ATTACK_RANDOM;
		else if(!strcmp(pElement->Attribute("AttackCode"), "ROOM_ATTACK_HANDICAP"))		pRoom->m_nAttackCode = ROOM_ATTACK_HANDICAP;
		else if(!strcmp(pElement->Attribute("AttackCode"), "ROOM_ATTACK_HOME"))			
		{
			pRoom->m_nAttackCode = ROOM_ATTACK_HOME;
			pRoom->m_nAttackTeam = PLAYER_TEAM_HOME;
		}
		else if(!strcmp(pElement->Attribute("AttackCode"), "ROOM_ATTACK_AWAY"))			
		{
			pRoom->m_nAttackCode = ROOM_ATTACK_AWAY;
			pRoom->m_nAttackTeam = PLAYER_TEAM_AWAY;
		}

		pRoom->m_nStartLevel = atoi(pElement->Attribute( "StartLevel" ));
		pRoom->m_nEndLevel = atoi(pElement->Attribute( "EndLevel" ));
		pRoom->m_fPlayTime = static_cast<float>( atof(pElement->Attribute( "PlayTime" )) );

		pBall->m_nBallForm= atoi(pElement->Attribute( "BallForm" ));

		m_nRobotCount = atoi(pElement->Attribute( "RobotCount" ));

		pRoom->m_cHomeResult.m_nResult[ARRAY_RESULT_GOAL] = atoi(pElement->Attribute( "HomeStartScore" ));
		pRoom->m_cAwayResult.m_nResult[ARRAY_RESULT_GOAL] = atoi(pElement->Attribute( "AwayStartScore" ));

		pChild = pChild->NextSibling( "RoomInfo" );
	}
}

void CPlayModeManager::SetupUI()
{
	CClientUIManager::GetPtr()->PageHide();

	CClientUIManager::GetPtr()->m_vCurrentPageList.clear();

	TiXmlNode* pNode = m_pTiXmlNode;
	TiXmlNode* pChild = pNode->FirstChild( "LoadUI" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(CGameProject::s_pGameProject->GetWideScreen() && !strcmp(pElement->Attribute("Wide"), "true"))
		{
			CClientUIManager::GetPtr()->m_vCurrentPageList.push_back(pElement->Attribute( "File" ));
			CClientUIManager::GetPtr()->LoadFile(pElement->Attribute( "File" ), !strcmp(pElement->Attribute("Visible"), "true"));
		}
		else if(!CGameProject::s_pGameProject->GetWideScreen() && !strcmp(pElement->Attribute("Wide"), "false"))
		{
			CClientUIManager::GetPtr()->m_vCurrentPageList.push_back(pElement->Attribute( "File" ));
			CClientUIManager::GetPtr()->LoadFile(pElement->Attribute( "File" ), !strcmp(pElement->Attribute("Visible"), "true"));
		}
		else if(!strcmp(pElement->Attribute("Wide"), "all"))
		{
			CClientUIManager::GetPtr()->m_vCurrentPageList.push_back(pElement->Attribute( "File" ));
			CClientUIManager::GetPtr()->LoadFile(pElement->Attribute( "File" ), !strcmp(pElement->Attribute("Visible"), "true"));
		}

		pChild = pChild->NextSibling( "LoadUI" );
	}
}

void CPlayModeManager::GroundOptionApply( CResGroundFormat * pkGroundFormat )
{
    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();

    if( pkVideoInfomation->GetCharacterLight() == 0 ) //CGameOptionDialog::GetPtr()->GetCharacterLightOption() == 1 )
    {
        pkGroundFormat->EraseAllDetailMap();
    }

    if( pkVideoInfomation->GetGroundEffect() == 0 ) //CGameOptionDialog::GetPtr()->GetGroundEffectOption() == 1 )
    {
        NiNode * pkGroundRoot = pkGroundFormat->GetRootNode();

        std::vector<std::string> kExceptList;
        kExceptList.push_back( "Pannel_0" );//전광판은 이팩트는 건드리지 않는다...

        CoreApi_RecursiveControllerEnable( pkGroundRoot, false, &kExceptList );    
    }
}

void CPlayModeManager::SetupMapLoading()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "MapLoading" );
	if( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		CResGroundFormat *pGround = dynamic_cast<CResGroundFormat *>(CObjectModelManager::GetPtr()->LoadGround(atoi(pElement->Attribute("ID"))));
		assert(pGround);
		m_pObjGroundInfo->Init(pGround, NiPoint3::ZERO, NiPoint3::ZERO, 1.f);

		/*pRoom->m_nGroundCode = 0;*/
        
        //this->GroundOptionApply( pGround );
	}
	else if(pRoom->m_nGroundCode > -1)
	{
		CResGroundTableFormat* ResourceTable = dynamic_cast<CResGroundTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_GROUNDTABLE, pRoom->m_nGroundCode, 0));
		assert(ResourceTable);

		//? 맵 변화 코드 임시 코드
		if(ResourceTable->m_nGroundCode/10 == 900 || ResourceTable->m_nGroundCode/10 == 910)
		{
			ResourceTable->m_nGroundCode = (ResourceTable->m_nGroundCode/10)*10 + pRoom->m_nBallCode%5 + 1;
		}
        
        CResGroundFormat * pGround = dynamic_cast<CResGroundFormat *>(CObjectModelManager::GetPtr()->LoadGround( ResourceTable->m_nGroundCode) );

        //int nGroundCode = (ResourceTable->m_nGroundCode/100 * 100) + pRoom->m_nTimeCode;
        //if( pRoom->m_nTimeCode == ROOM_TIME_DAYLIGHT && pRoom->m_nWeatherCode == ROOM_WEATHER_RAIN )
        //{
        //    nGroundCode = (ResourceTable->m_nGroundCode/100 * 100) + 5;
        //}

        //CResGroundFormat *pGround = dynamic_cast<CResGroundFormat *>(CObjectModelManager::GetPtr()->LoadGround( nGroundCode ));
        //if( pGround == 0 )
        //{        
		    //CResGroundFormat * pGround = dynamic_cast<CResGroundFormat *>(CObjectModelManager::GetPtr()->LoadGround(nGroundCode)); //ResourceTable->m_nGroundCode));
		    //assert(pGround);
        //}

		m_pObjGroundInfo->Init(pGround, NiPoint3::ZERO, NiPoint3::ZERO, 1.f);
        //this->GroundOptionApply( pGround );
        CEffectManager::GetPtr()->PrecreateEffect();
	}
}

void CPlayModeManager::SetupMapLoading(int nGroundCode)
{
	CResGroundFormat *pGround = dynamic_cast<CResGroundFormat *>(CObjectModelManager::GetPtr()->LoadGround(nGroundCode));
	assert(pGround);
	m_pObjGroundInfo->Init(pGround, NiPoint3::ZERO, NiPoint3::ZERO, 1.f);

    //this->GroundOptionApply( pGround );
}

void CPlayModeManager::SetupMapObjectLoading()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "MapObject" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(!strcmp(pElement->Attribute("AttachType"), "NULL"))
		{
			CResObjectFormat *pObject = dynamic_cast<CResObjectFormat *>(CObjectModelManager::GetPtr()->ObjectLoadStickGround(atoi(pElement->Attribute("Type"))));
			assert(pObject);
			pObject->Init(atoi(pElement->Attribute("ID")), NiPoint3::ZERO, NiPoint3::ZERO);
			m_pObjGroundInfo->InsertObject(pObject);
		}
		else if(!strcmp(pElement->Attribute("AttachType"), "MAP"))
		{
			CResObjectTableFormat* ResourceTable = dynamic_cast<CResObjectTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OBJECTTABLE, atoi(pElement->Attribute("Type")), 0));
			assert(ResourceTable);
			CResGroundFormat *pGroundFormat = dynamic_cast<CResGroundFormat*>(m_pObjGroundInfo->m_pModel);
			assert(pGroundFormat);
			NiNode* temp = pGroundFormat->GroundObjectAttach("Object", ResourceTable->m_sDataFile, pGroundFormat->GetRootNode()); //"-", pGroundFormat->GetRootNode());
			temp->SetName(ResourceTable->m_sDataFile);
		}
		else if(!strcmp(pElement->Attribute("AttachType"), "Character"))
		{

		}
		pChild = pChild->NextSibling( "MapObject" );
	}
}

void CPlayModeManager::SetupBallLoading()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "BallLoading" );
	if( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		int nBallCode = atoi(pElement->Attribute( "BallIndex" ));

		if(nBallCode == -1) return;

		pRoom->m_nBallCode = nBallCode;

		CResBallFormat *pBall = dynamic_cast<CResBallFormat *>(CObjectModelManager::GetPtr()->BallLoadStickGround(pRoom->m_nBallCode));
		assert(pBall);
		if(pBall == NULL) return;
		pBall->SetPolygonTrailer();

		NiPoint3 pos;

		pos.x = static_cast<float>( atof(pElement->Attribute( "PosX" )) );
		pos.y = static_cast<float>( atof(pElement->Attribute( "PosY" )) );
		pos.z = static_cast<float>( atof(pElement->Attribute( "PosZ" )) );

		CObjBallManager::GetPtr()->GetBallPointer()->Init(pBall, pos);
	}
	else if(pRoom->m_nBallCode > -1)
	{
		CResBallFormat *pBall = dynamic_cast<CResBallFormat *>(CObjectModelManager::GetPtr()->BallLoadStickGround(pRoom->m_nBallCode));
		assert(pBall);
		if(pBall == NULL) return;
		pBall->SetPolygonTrailer();

		CObjBallManager::GetPtr()->GetBallPointer()->Init(pBall, NiPoint3::ZERO);
	}
}

void CPlayModeManager::SetupCameraLoading()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "CameraInit" );
	if( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		int tempteam = PLAYER_TEAM_HOME;

		if(!strcmp(pElement->Attribute("Team"), "PLAYER_TEAM_HOME")) tempteam = PLAYER_TEAM_HOME;
		else tempteam = PLAYER_TEAM_AWAY;

		CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
		assert(pSelectPlayerInfo);

		pSelectPlayerInfo->m_cSetting.m_nCameraType = atoi(pElement->Attribute("CameraType"));
		pSelectPlayerInfo->m_cSetting.m_nCameraTarget = atoi(pElement->Attribute("CameraTarget"));
		pSelectPlayerInfo->m_cSetting.m_nCameraTeam = atoi(pElement->Attribute("CameraTeam"));
		pSelectPlayerInfo->m_cSetting.m_nCameraZoom = atoi(pElement->Attribute("CameraZoom"));
		pSelectPlayerInfo->m_nCameraIndex = atoi(pElement->Attribute("CameraIndex"));
	}
}

CObjPlayerInfo* CPlayModeManager::SetupMyPlayerLoading()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "MyPlayerLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		CObjPlayerInfo* pSelectPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
		assert(pSelectPlayerInfo);
		CObjPlayerInfo* pMyPlayerInfo = pSelectPlayerInfo->Clone();

		pMyPlayerInfo->m_nTeam = atoi(pElement->Attribute("Team"));

		pMyPlayerInfo->m_nForm = atoi(pElement->Attribute("Form"));

		pMyPlayerInfo->m_nPosition = atoi(pElement->Attribute("Position"));

		pMyPlayerInfo->m_cLevel.m_nLevel = atoi(pElement->Attribute("Level"));

		_snprintf(pMyPlayerInfo->m_sName, PLAYER_NAME_SIZE, "%s", pElement->Attribute("Name"));

		CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + pMyPlayerInfo->m_nPosition*100 + pMyPlayerInfo->m_cLevel.m_nLevel, 0));
		assert(pTempAbility);
//		CResTempSkillTableFormat* pTempSkill	 = CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPSKILLTABLE, pTempAbility->m_nSkillCode, 0);

		CFaculty cFaculty;
		memset(&cFaculty, 0, sizeof(CFaculty));
		pTempAbility->SetValue(pMyPlayerInfo->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, pMyPlayerInfo->m_cTrainingFaculty.m_nFaculty, 0);

//		pTempSkill->SetValue(pMyPlayerInfo->m_vSkillList);

		return pMyPlayerInfo;
	}
	return NULL;
}

void CPlayModeManager::SetupNpcLoading()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "NpcLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		CObjPlayerInfo* pObjPlayerInfo = new CObjPlayerInfo;

		pObjPlayerInfo->SetObjSeq( atoi(pElement->Attribute("ObjSeq")) );

		pObjPlayerInfo->m_nTeam = atoi(pElement->Attribute("Team"));

		pObjPlayerInfo->m_nForm = atoi(pElement->Attribute("Form"));

		if     (!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_USER"))			pObjPlayerInfo->SetKind(PLAYER_KIND_USER);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_ROBOT"))			pObjPlayerInfo->SetKind(PLAYER_KIND_ROBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_USERBOT"))		pObjPlayerInfo->SetKind(PLAYER_KIND_USERBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_CARDBOT"))		pObjPlayerInfo->SetKind(PLAYER_KIND_CARDBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_TUTORIALROBOT"))	pObjPlayerInfo->SetKind(PLAYER_KIND_TUTORIALROBOT);

		if     (!strcmp(pElement->Attribute("Duty"), "DUTY_NONE"))					pObjPlayerInfo->m_nDuty = DUTY_NONE;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOHEADINGATTACK"))		pObjPlayerInfo->m_nDuty = DUTY_TUTOHEADINGATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOCROSSATTACK"))		pObjPlayerInfo->m_nDuty = DUTY_TUTOCROSSATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOSHORTPASSATTACK"))	pObjPlayerInfo->m_nDuty = DUTY_TUTOSHORTPASSATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_DIRECTSHOOT"))			pObjPlayerInfo->m_nDuty = DUTY_DIRECTSHOOT;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOWAYPOINTATTACK"))	pObjPlayerInfo->m_nDuty = DUTY_TUTOWAYPOINTATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TACKLEMOVE"))			pObjPlayerInfo->m_nDuty = DUTY_TACKLEMOVE;

		pObjPlayerInfo->SetTargetDoman( atoi(pElement->Attribute("TargetDoman")) );

		//if(pElement->Attribute("TargetPlayer"))
		//	pObjPlayerInfo->SetTargetPlayer( atoi(pElement->Attribute("TargetPlayer")) );

		pObjPlayerInfo->m_nPosition = atoi(pElement->Attribute("Position"));

		pObjPlayerInfo->m_cLevel.m_nLevel = atoi(pElement->Attribute("Level"));

		CResTempAbilityTableFormat* pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, atoi(pElement->Attribute("TempAbility")), 0));
		assert(pTempAbility);
		CResTempCostumeTableFormat* pTempCostume = dynamic_cast<CResTempCostumeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPCOSTUMETABLE, atoi(pElement->Attribute("TempCostume")), 0));
		assert(pTempCostume);
//		CResTempSkillTableFormat* pTempSkill	 = CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPSKILLTABLE, pTempAbility->m_nSkillCode, 0);

		CFaculty cFaculty;
		memset(&cFaculty, 0, sizeof(CFaculty));
		pTempAbility->SetValue(pObjPlayerInfo->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, pObjPlayerInfo->m_cTrainingFaculty.m_nFaculty, 0);

		pTempCostume->SetValue(pObjPlayerInfo->m_nEquipWear);

//		pTempSkill->SetValue(pObjPlayerInfo->m_vSkillList);

		pObjPlayerInfo->m_cShape.m_nGender = (pObjPlayerInfo->m_nEquipWear[0]%100/10);

		CObjPlayerManager::GetPtr()->SetPlayerLists(pObjPlayerInfo, PLAYER_TEAM_STAY);

		pChild = pChild->NextSibling( "NpcLoading" );
	}
}

CObjPlayerInfo* CPlayModeManager::SetupPlayerLoading(int Index)
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "PlayerLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(atoi(pElement->Attribute( "ID" )) != Index )
		{
			pChild = pChild->NextSibling( "PlayerLoading" );
			continue;
		}

		CObjPlayerInfo* pObjPlayerInfo = new CObjPlayerInfo;

		pObjPlayerInfo->SetObjSeq( atoi(pElement->Attribute("ObjSeq")) );

		pObjPlayerInfo->m_nTeam = atoi(pElement->Attribute("Team"));

		pObjPlayerInfo->m_nForm = atoi(pElement->Attribute("Form"));

		if     (!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_USER"))			pObjPlayerInfo->SetKind(PLAYER_KIND_USER);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_ROBOT"))			pObjPlayerInfo->SetKind(PLAYER_KIND_ROBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_USERBOT"))		pObjPlayerInfo->SetKind(PLAYER_KIND_USERBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_CARDBOT"))		pObjPlayerInfo->SetKind(PLAYER_KIND_CARDBOT);
		else if(!strcmp(pElement->Attribute("Kind"), "PLAYER_KIND_TUTORIALROBOT"))	pObjPlayerInfo->SetKind(PLAYER_KIND_TUTORIALROBOT);

		if     (!strcmp(pElement->Attribute("Duty"), "DUTY_NONE"))					pObjPlayerInfo->m_nDuty = DUTY_NONE;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOHEADINGATTACK"))		pObjPlayerInfo->m_nDuty = DUTY_TUTOHEADINGATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOCROSSATTACK"))		pObjPlayerInfo->m_nDuty = DUTY_TUTOCROSSATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOSHORTPASSATTACK"))	pObjPlayerInfo->m_nDuty = DUTY_TUTOSHORTPASSATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_DIRECTSHOOT"))			pObjPlayerInfo->m_nDuty = DUTY_DIRECTSHOOT;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TUTOWAYPOINTATTACK"))	pObjPlayerInfo->m_nDuty = DUTY_TUTOWAYPOINTATTACK;
		else if(!strcmp(pElement->Attribute("Duty"), "DUTY_TACKLEMOVE"))			pObjPlayerInfo->m_nDuty = DUTY_TACKLEMOVE;

		pObjPlayerInfo->SetTargetDoman( atoi(pElement->Attribute("TargetDoman")) );

		//if(pElement->Attribute("TargetPlayer"))
		//	pObjPlayerInfo->SetTargetPlayer( atoi(pElement->Attribute("TargetPlayer")) );

		pObjPlayerInfo->m_nPosition = atoi(pElement->Attribute("Position"));

		pObjPlayerInfo->m_cLevel.m_nLevel = atoi(pElement->Attribute("Level"));

		_snprintf(pObjPlayerInfo->m_sName, PLAYER_NAME_SIZE, "%s", pElement->Attribute("Name"));

		pObjPlayerInfo->m_nLoadingStep = 100;//인공지능은 로딩이 무조건 다 된것으로 간주

		CResTempAbilityTableFormat* pTempAbility = NULL;

		if(pElement->Attribute("TempAbility"))
		{
			pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, atoi(pElement->Attribute("TempAbility")), 0));
		}
		else pTempAbility = dynamic_cast<CResTempAbilityTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPABILITYTABLE, 10000 + pObjPlayerInfo->m_nPosition*100 + pObjPlayerInfo->m_cLevel.m_nLevel, 0));
		
		assert(pTempAbility);
		CResTempCostumeTableFormat* pTempCostume = dynamic_cast<CResTempCostumeTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPCOSTUMETABLE, atoi(pElement->Attribute("TempCostume")), 0));
		assert(pTempCostume);
//		CResTempSkillTableFormat* pTempSkill	 = CResourceManager::GetPtr()->FindResource(RESOURCE_TEMPSKILLTABLE, pTempAbility->m_nSkillCode, 0);

		CFaculty cFaculty;
		memset(&cFaculty, 0, sizeof(CFaculty));
		pTempAbility->SetValue(pObjPlayerInfo->m_cBaseFaculty.m_nFaculty, cFaculty.m_nFaculty, pObjPlayerInfo->m_cTrainingFaculty.m_nFaculty, 0);

		pTempCostume->SetValue(pObjPlayerInfo->m_nEquipWear);

//		pTempSkill->SetValue(pObjPlayerInfo->m_vSkillList);

		pObjPlayerInfo->m_cShape.m_nGender = (pObjPlayerInfo->m_nEquipWear[0]%100/10);

		return pObjPlayerInfo;
	}
	return NULL;
}

void CPlayModeManager::SetupPlayerLoadingN()
{
	for(int i=0;i<m_nRobotCount;i++)
		SetupPlayerLoading(i);
}

void CPlayModeManager::SetupCharacterLoading(CObjPlayerInfo* pObjPlayerInfo, int Index)
{
    bool bLighting = true;
    VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();

    if( pkVideoInfomation->GetCharacterLight() == 0 ) bLighting = false;

	CResGroundFormat *pGroundFormat = dynamic_cast<CResGroundFormat*>(m_pObjGroundInfo->m_pModel);
	assert(pGroundFormat);
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "CharacterLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(atoi(pElement->Attribute( "ID" )) != Index )
		{
			pChild = pChild->NextSibling( "CharacterLoading" );
			continue;
		}

		CResCharacterFormat *pCharacter = dynamic_cast<CResCharacterFormat*>(CObjectModelManager::GetPtr()->CharacterLoadStickGround(pObjPlayerInfo->m_cShape.m_nGender, pObjPlayerInfo->GetKind(),bLighting));
		assert(pCharacter);
		pObjPlayerInfo->Init(pCharacter);

		NiAVObject * pkGround = pGroundFormat->GetRootNode()->GetObjectByName( pElement->Attribute( "PointIndex" ) );
		assert( pkGround != 0 );

		pCharacter->SetNodePoint(NiPoint3::ZERO);

		NiNode* pGround = NiDynamicCast(NiNode, pkGround);

		pGround->RemoveAllChildren();

		pGround->AttachChild( pCharacter->GetRootNode() );

		pGround->Update( 0.0f );
		pGround->UpdateNodeBound();
		pGround->UpdateProperties();
		pGround->UpdateEffects();

		pObjPlayerInfo->m_cCurrentAction.m_nAniCode = atoi(pElement->Attribute( "AniCode" ));
		pObjPlayerInfo->m_cCurrentAction.m_fCurrentTime = 0.0f;

		if(!strcmp(pElement->Attribute("Morpher"), "true")) pCharacter->SetMorpher(true);
		else pCharacter->SetMorpher(false); 		

		return;
	}
}

void CPlayModeManager::SetupCharacterLoading(CObjPlayerInfo* pObjPlayerInfo, int Index, int nKind)
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "CharacterLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(atoi(pElement->Attribute( "ID" )) != Index )
		{
			pChild = pChild->NextSibling( "CharacterLoading" );
			continue;
		}

        bool bLighting = true;
        VideoInfomation * pkVideoInfomation = VideoInfomation::GetPtr();

        if( pkVideoInfomation->GetCharacterLight() == 0 ) bLighting = false;

		CResCharacterFormat *pCharacter = dynamic_cast<CResCharacterFormat*>(CObjectModelManager::GetPtr()->CharacterLoadStickGround(pObjPlayerInfo->m_cShape.m_nGender, nKind, bLighting));
		assert(pCharacter);

		pObjPlayerInfo->Init(pCharacter);

		if(pElement->Attribute( "AniCode" ) == NULL)
			pObjPlayerInfo->m_cCurrentAction.m_nAniCode = STAND_CODE;
		else
		{
			pObjPlayerInfo->m_cCurrentAction.m_nAniCode = atoi(pElement->Attribute( "AniCode" ));
			pObjPlayerInfo->m_cCurrentAction.m_fCurrentTime = 0.0f;
		}

		if(!strcmp(pElement->Attribute("Specular"), "true")) pObjPlayerInfo->RecursiveSpecularEnable(true);
		else pObjPlayerInfo->RecursiveSpecularEnable(false); 

		if(!strcmp(pElement->Attribute("Morpher"), "true")) pCharacter->SetMorpher(true);
		else pCharacter->SetMorpher(false); 		

		return;
	}

	assert(0);//캐릭터를 찾지 못했다
}

void CPlayModeManager::SetupCharacterPoint(CObjPlayerInfo* pObjPlayerInfo, int Index)
{
	if(!pObjPlayerInfo->m_pModel || !m_pObjGroundInfo->m_pModel) return;

	CResGroundFormat *pGroundFormat = dynamic_cast<CResGroundFormat*>(m_pObjGroundInfo->m_pModel);
	assert(pGroundFormat);
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "CharacterLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(atoi(pElement->Attribute( "ID" )) != Index )
		{
			pChild = pChild->NextSibling( "CharacterLoading" );
			continue;
		}

		NiAVObject * pkGround = pGroundFormat->GetRootNode()->GetObjectByName( pElement->Attribute( "PointIndex" ) );
		assert( pkGround != 0 );

		NiNode* pGround = NiDynamicCast(NiNode, pkGround);

		pGround->AttachChild( pObjPlayerInfo->GetRootNode() );

		pObjPlayerInfo->m_cCurrentAction.m_nAniCode = atoi(pElement->Attribute( "AniCode" ));

		return;
	}
}

void CPlayModeManager::SetupCharacterPoint(CObjPlayerInfo* pObjPlayerInfo, int Index, int nKind)
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "CharacterLoading" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if(atoi(pElement->Attribute( "ID" )) != Index + nKind*1000)
		{
			pChild = pChild->NextSibling( "CharacterLoading" );
			continue;
		}

		NiPoint3 Pos, Dir;

		Pos.x = static_cast<float>( atof(pElement->Attribute( "PosX" )) );
		Pos.y = static_cast<float>( atof(pElement->Attribute( "PosY" )) );
		Pos.z = static_cast<float>( atof(pElement->Attribute( "PosZ" )) );
		Dir.x = static_cast<float>( atof(pElement->Attribute( "DirX" )) );
		Dir.y = static_cast<float>( atof(pElement->Attribute( "DirY" )) );
		Dir.z = static_cast<float>( atof(pElement->Attribute( "DirZ" )) );

		pObjPlayerInfo->SetPoint(Pos, Dir);
		pObjPlayerInfo->m_cCurrentAction.m_nAniCode = atoi(pElement->Attribute( "AniCode" ));
		return;
	}
}

void CPlayModeManager::DeleteMapObject()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "MapObject" );
	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		CResObjectTableFormat* ResourceTable = dynamic_cast<CResObjectTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_OBJECTTABLE, atoi(pElement->Attribute("Type")), 0));
		assert(ResourceTable);
		CResGroundFormat *pGroundFormat = dynamic_cast<CResGroundFormat*>(m_pObjGroundInfo->m_pModel);
		assert(pGroundFormat);
		if(ResourceTable->GetObjCode() != 10002)
		{
			NiAVObject * pkGroundObject = pGroundFormat->GetRootNode()->GetObjectByName( ResourceTable->m_sDataFile );
			NiNode* pParentNode = pkGroundObject->GetParent();
			pParentNode->DetachChild(pkGroundObject);
			pkGroundObject = 0;
		}

		pChild = pChild->NextSibling( "MapObject" );
	}
}

void CPlayModeManager::SetupKeyLoading()
{
	TiXmlNode* pChild = m_pTiXmlNode->FirstChild( "UseKey" );

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
	if( pObjPlayerInfo == NULL ) return;

	if(pChild) pObjPlayerInfo->InitUseKey(false);
	else pObjPlayerInfo->InitUseKey(true);

	while( pChild )
	{
		TiXmlElement* pElement = (TiXmlElement*)pChild;

		if     (!strcmp(pElement->Attribute("Key"), "UP"))			pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_UP]			= true;
		else if(!strcmp(pElement->Attribute("Key"), "DOWN"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_DOWN]			= true;
		else if(!strcmp(pElement->Attribute("Key"), "LEFT"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_LEFT]			= true;
		else if(!strcmp(pElement->Attribute("Key"), "RIGHT"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_RIGHT]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "LEFTSHOOT"))	pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_LEFTSHOOT]	= true;
		else if(!strcmp(pElement->Attribute("Key"), "RIGHTSHOOT"))	pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_RIGHTSHOOT]	= true;
		else if(!strcmp(pElement->Attribute("Key"), "LONGPASS"))	pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_LONGPASS]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SHORTPASS"))	pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SHORTPASS]	= true;
		else if(!strcmp(pElement->Attribute("Key"), "SCREEN"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SCREEN]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "TACKLE"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_TACKLE]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "STEAL"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_STEAL]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "QUICK"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_QUICK]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "QUICK2"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_QUICK2]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SKILL1"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SKILL1]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SKILL2"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SKILL2]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SKILL3"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SKILL3]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SKILL4"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SKILL4]		= true;
		else if(!strcmp(pElement->Attribute("Key"), "SKILL5"))		pObjPlayerInfo->m_bIsUseKey[ARRAY_KEY_SKILL5]		= true;

		pChild = pChild->NextSibling( "UseKey" );
	}
}

void CPlayModeManager::SetupRobotLoading()
{
	CObjRoomInfo* pRoom = CObjRoomManager::GetPtr()->GetRoomPointer();

	for(int i = 0 ; i < m_nRobotCount; i++)
	{
		if(pRoom->m_nMode == ROOM_MODE_QUEST && 1 < CObjPlayerManager::GetPtr()->GetPlayerCount() && i + 1 < CObjPlayerManager::GetPtr()->GetPlayerCount())
			continue;			

		CObjPlayerInfo*  pObjPlayerInfo = SetupPlayerLoading(i);

		if(pObjPlayerInfo != NULL)
			CObjPlayerManager::GetPtr()->SetPlayerLists(pObjPlayerInfo, pObjPlayerInfo->m_nTeam);
	}
}
