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
#include "PacketForm.h"

class CChoiceRoomProcess : public CBaseProcess
{
public:

	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_PLAY,
		STATE_SEND_INFO,
		STATE_END,
	};

	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포
	POINT							m_pDNDPoint;

	CSCRoomList						m_cRoomList;
	CSCLobbyList					m_cLobbyList;

	typedef	vector<int>				VECTOR_QUESTCODE;
	VECTOR_QUESTCODE				m_vQuestCodeList;

	char							m_nPopupIndex;

	DWORD							m_nSendTime;

	char							m_nRoomArrange;

	//int								m_nQuestCode;
	//int								m_nSelectQuestCode;

	char							m_nLobbyTab;

	int								m_nRoomPage;
	int								m_nLobbyPage;

public:

	void							EraseLobbyoneList();

	void							SetRoomList(CSCRoomList* pList);
	void							SetLobbyList(CSCLobbyList* pList);

	//UI관련 함수
	void							FUNCTION_ChoiceRoomListUI();
	void							FUNCTION_ChoiceUserListUI();
	void							FUNCTION_ChoiceUserListUIPopup();

public:
	void							FUNCTION_RoomPageStart();
	void							FUNCTION_RoomPagePrv();
	void							FUNCTION_RoomPageNext();
	void							FUNCTION_RoomPage3Next();
	void							FUNCTION_LobbyPageStart();
	void							FUNCTION_LobbyPagePrv();
	void							FUNCTION_LobbyPageNext();
	void							FUNCTION_LobbyPage3Next();
	
	void							FUNCTION_Init();

	void							FUNCTION_ChangeTab(char idx);
	
	//void							FUNCTION_CreateQuestInfo();
	//void							FUNCTION_QuestListOverCheck();
	//void							FUNCTION_SetQuestList();
	void							FUNCTION_RoomBasicInfo(CSCRoomList* pList, int i);
	void							FUNCTION_CreateQuest();
	//void							InitQuestCode();

	bool							FUNCTION_ChoiceRoomIsSecret(const DWORD& nIndex);
	void							FUNCTION_ChoiceRoomConfirmPass(const DWORD& nIndex);
	void							EnterRoom(const DWORD& nIndex );
	void							EnterLadderRoom();

	/*void							FUNCTION_IsChoiceClass();*/

public:
	CChoiceRoomProcess();
	virtual ~CChoiceRoomProcess(void);

    virtual void					Terminate();

	void							LoadGround();

	virtual void					UpdateState(float fFrameTime, float fAccumTime);

	virtual void					RenderFrame();

	virtual bool					OnDefault(NiEventRef pEventRecord);

	CBaseProcess*					GetSameProcess();

	void							UpdateProcess(float fFrameTime,float fAccumTime);

	virtual void					InitProcess();
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

};