/*********************************************************************************************



*			ChoiceRoomProcess.h

*			방 선택 프로세스

*			설명 : 게임을 하기위한 방선택 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "3DRenderedTexBoard.h"

class CReadyRoomProcess : public CBaseProcess
{
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_END
	};

private:
	int								m_nPosIndex;
	CObjPlayerInfo*					m_pObjPlayerInfo;
	int								m_nUDPTime;
    CObjGroundInfo *                m_pGroundInfo;

    NiNode *                        m_pkCasterNode;
	POINT							m_pDNDPoint;

public:
	CReadyRoomProcess();
	virtual ~CReadyRoomProcess(void);

	virtual void					Terminate();

	void							LoadCharacter();

	//UI관련 함수

	virtual void					RenderFrame();

	void							FUNCTION_Pass();
	void							FUNCTION_UpdateBallChange();
	void							FUNCTION_UpdateMapChange();
	void							FUNCTION_MapChange(char idx);
	void							FUNCTION_BallChange(char idx);
	void							FUNCTION_PitInUpdate();
	void							FUNCTION_TeamChangeVisible(N3UI_VISIBLE_TYPE eVISIBLE_TYPE);
	void							ViewOverCheck();

	void 							SetUIReadyRoomPositionSetting();
	void 							SetUIReadyRoomAthleteInSetting(CObjPlayerInfo* pPlayer);
	void 							SetUIReadyRoomAthleteSetting();
	void 							Set_ReadyRoomUI();
	void 							Set_RoomNumber(const DWORD& nRoomNumber);
	void 							SetRoom();

private:
	void 							Update_LevelBounds(char* sLevelBounds);
	void 							Update_PersonLimit(char* sPersonLimit);
	void 							Update_ReadyRoomTitle(char* sTitle);
	enum MARK_FLAG
	{
		NONE=0,
		BANGJANG=1,
		TEAMJANG=2,
		PCBANG=4,
		FLAG0=8
	};

	void 							Set_MarkList(char nTeam, DWORD nSeatNum, bool bBangJang, bool bTeamJang);
	void 							EnableSetBallMapCommand(bool bBangJang);
	void 							EnableBanCommand(bool bBangJang,bool bTeamJang, bool bHomeTeam);
	void							EnableStartRoomCommand(bool bBangJang);
	void 							EnableSetRoomCommand(bool bBangJang);
	void 							EnableTeamJangCommand(char nTeam ,bool bTeamJang);
	void 							DisableSelfPopUpCommand(CObjPlayerInfo* pPlayer);
	void 							DisableXPositionButton(CObjRoomInfo* pRoom);

public:
	void 							Send_ReadyRoomOnClickWhisper(DWORD nIndex);
	void 							Send_ReadyRoomOnClickInfo(DWORD nIndex);
	void 							Send_ReadyRoomOnClickAddFriend(DWORD nIndex);
	void							Send_ReadyRoomOnClickAddBlock(DWORD nIndex);
	void							Send_ReadyRoomOnClickBan(DWORD nIndex);
	void							Send_ReadyRoomChangePosition(DWORD nIndex);

	void							InitRoomNormalOption();
	void							InitRoomDetailOption();
	void							RoomConfigInit(bool bView);

	int								PositionToConvertIndex(int idx, int nPosition);

	virtual void					UpdateState(float fFrameTime, float fAccumTime);

	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();

	virtual bool					OnDefault(NiEventRef pEventRecord);

	CBaseProcess*					GetSameProcess();

	void							UpdateProcess(float fFrameTime,float fAccumTime);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void					InitProcess();

	void							FUNCTION_MapNameToolTipUpdate();

private:
	void							TurnOn_RoomJangButton();
	void							TurnOn_TeanJangButton();

    void                            SetGameModeSetting();
};