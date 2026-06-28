/*********************************************************************************************



*			PlayModeManager.h

*			프로세스 메니져

*			설명 : 프로세스의 관리 생성 변경을 담당한다


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"

#include "Singleton.h"

#include <stack>
class CObjPlayerInfo;

class CPlayModeManager : public CSingleton < CPlayModeManager >
{
public:
	CObjGroundInfo*				m_pObjGroundInfo;	//그라운드 인포

	TiXmlDocument				m_cTiXmlDocument;
protected:

	int							m_nRobotCount;//로봇 갯수

	//int							m_nBallTeam;//볼소유팀

	//int							m_nBallForm;//볼소유자셋번호

	TiXmlNode*					m_pTiXmlNode;

public:
	//int							GetBallTeam() { return m_nBallTeam; }
	//int							GetBallForm() { return m_nBallForm; }

public:
	CPlayModeManager(void);
	~CPlayModeManager(void);

	void						Init(CObjGroundInfo* pObjGroundInfo, int nPlayType);

	void						Init(CObjGroundInfo* pObjGroundInfo, const char* filename, int nPlayType);

	void						InitPlayerPointSetting();

	void						Update();

	void						Rander();

	void						Terminate();

	void						ReSetupWindow(int nPlayType);

	bool						LoadFile(const char* filename);

	TiXmlNode*					SetupWindow( int nProcessCmd );

	void						SetRoomInfo();

	void						SetupUI();

	void						SetupMapLoading();

	void						SetupMapLoading(int nGroundCode);

	void						SetupMapObjectLoading();

	void						SetupBallLoading();

	void						SetupCameraLoading();

	CObjPlayerInfo*				SetupMyPlayerLoading();

	void						SetupNpcLoading();

	void						SetupKeyLoading();

	void						SetupRobotLoading();

	CObjPlayerInfo*				SetupPlayerLoading(int Index);
	
	void						SetupPlayerLoadingN();

	void						SetupCharacterLoading(CObjPlayerInfo* pObjPlayerInfo, int Index);

	void						SetupCharacterLoading(CObjPlayerInfo* pObjPlayerInfo, int Index, int nKind);

	void						SetupCharacterPoint(CObjPlayerInfo* pObjPlayerInfo, int Index);

	void						SetupCharacterPoint(CObjPlayerInfo* pObjPlayerInfo, int Index, int nKind);

	void						DeleteMapObject();

	void						SetRobotCount(int nRobotCount) { m_nRobotCount = nRobotCount; }

    void                        GroundOptionApply( CResGroundFormat * pkGroundFormat );
};