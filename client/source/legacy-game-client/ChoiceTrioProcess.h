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
#include "CharacterSelectAnimator.h"
#include "3DRenderedTexBoard.h"

class CChoiceTrioProcess : public CBaseProcess
{
public:

	CharacterSelectAnimator         m_cRotateCharacter;

	CObjPlayerInfo*					m_pObjPlayerInfo;

	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,

		STATE_END
	};


	int								m_nSelectCharacterIndex;
protected:
	bool							m_bRecvSettingInfo;

public:
	CChoiceTrioProcess();
	virtual ~CChoiceTrioProcess(void);

	void							LoadGround();

    // 캐릭터를 로드하고나서 각각의 캐릭터들에 대해 애니메이션을 로딩한다.
	void							LoadCharacter();

    //유저가 가지고 있는 캐릭터 정보를 셋팅한다.
	void							SetCharacterInfo(CSCCharacterInfo* pPacket);
    
//패킷 관련 함수
	bool Quit();
    
    // nIndex 슬롯에 있는 캐릭터를 선택하고, 캐릭터별 옵션 설정정보를 서버에게 요구한다.
    void                            SelectCharacter( int nIndex );

    // 현재 선택된 캐릭터를 선택하고, 선택된 캐릭터의 옵션정보를 서버에게 요구하고, 캐릭터가 선택되었음을 서버에게 알린다.
	void							Select_Character();

	void							FUNCTION_DateUpdate();
	void							SoundSetting();
	void							Select_Delete();

	void							Update_CharacterInfo();

	virtual void					UpdateState(float fFrameTime, float fAccumTime);

	virtual bool					OnDefault(NiEventRef pEventRecord);
	
	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();
	CBaseProcess*					GetSameProcess();

	void							UpdateProcess(float fFrameTime,float fAccumTime);

	virtual void					InitProcess();

    virtual void					Terminate();

    // 각각의 캐릭터에 관한 애니메이션 정보를 설정한다.
    void                            InitCharacterSelectAnimator(); 

	void							FUNCTION_Init();
};