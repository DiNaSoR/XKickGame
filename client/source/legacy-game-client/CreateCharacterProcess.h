/*********************************************************************************************



*			ChoiceTrioProcess.h

*			캐릭터 선택을 위한 프로세스

*			설명 : 트리오중에서 원하는 캐릭터를 선택하는 프로세스


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "ResTableStack.h"
#include "3DRenderedTexBoard.h"

typedef vector<CResItemTableFormat>		ITEM_VECTORLIST;

class CCreateCharacterProcess : public CBaseProcess
{
public:

	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_UPDATE,
		STATE_END
	};

	char							m_nSelectSex;
	bool							m_bDuplication;
	char							m_sCharacterName[PLAYER_NAME_SIZE];

	ITEM_VECTORLIST					m_vItemList[6];
	int								m_nCurrentEquipIndex[6];
	
	char							m_nSkin;
	char							m_nSelSkin;
	char							m_nGender;

	CObjPlayerInfo*					m_pPlayerInfo;
	POINT							m_pDNDPoint;

	CObjPlayerInfo*					m_pCharacterInfo;

	void InitItemTable();

public:
	CCreateCharacterProcess();
	virtual ~CCreateCharacterProcess(void);
    
	virtual bool					OnDefault(NiEventRef pEventRecord);
	void							InputNewCharacterInfo();
	void							LoadGround();
	void							LoadCharacter();
	void							CreateCharacter();
	virtual void					Terminate();

	//패킷 관련 함수
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	virtual void					RenderFrame();

	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();
	CBaseProcess*					GetSameProcess();

	void							UpdateProcess(float fFrameTime,float fAccumTime);
	virtual void					InitProcess();
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void 							Change_Style(int index);
	void 							Create_Character();
	bool 							InputCharacterName();
	void 							InitEquip();
	char 							NameCheck();
	void 							DuplicationCheck();
	void							Change_Gender(int index);
	void							Change_Skin(int index);

	void 							FUNCTION_CreateTutoInit();
	void 							FUNCTION_ShowFacultTuto();
	void 							FUNCTION_InitCreateCharacter();
};