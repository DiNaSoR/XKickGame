/*********************************************************************************************



*			SkillShopProcess.h

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
#include "MySkillWindow.h"

class CSkillShopProcess : public CBaseProcess
{
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_END
	};

	CObjPlayerInfo*					m_pSelectPlayerInfo;//캐릭터 인포
	CObjRoomInfo*					m_pRoom;

	NiCameraPtr						m_spCeremonyCamera;
	NiNodePtr						m_spGameCameraNode; // a fixed-transform node to convert camera
	NiNodePtr						m_spCeremonyCameraNode; // a fixed-transform node to convert camera

	NiControllerManagerPtr			m_pkControllerManager;
	POINT							m_pDNDPoint;

public:

	// 분류 관련
	int								m_nSmallFractionStart;
	int								m_nBigFractionIndex;//대분류 번호
	int								m_nSmallFractionIndex;//소분류 번호

	// 페이지 관련
	int								m_nCurrentPage;
	int								m_nTotalPage;

	// 정보저장 관련
	int								m_nShopEquipWear[MAX_EQUIP];//착용 의상	
    int								m_nCode[LIST32_SIZE];//코드
	int								m_nSkillCode[LIST512_SIZE];//스킬 총코드
	int								m_nSkillType[LIST512_SIZE];//스킬 타입
	int								m_nTrainingCode[LIST512_SIZE];//트레이닝 총코드
	int								m_nCeremonyCode[LIST512_SIZE];//세레모니 총코드
	CResBaseFormat*					m_pEquipWearModelID[MAX_EQUIP];	//착용 의상 아이디

	int                             m_nSelectIndex;
	int								m_nDetailTabIndex;
	char							m_nSelectArea;
	int								m_nTableSell;
	
	bool                            m_bSkillView;
    NiNodePtr                       m_spChar;
    bool                            m_bBeforeStateMachineUpdate;

	// 분류 관련 벡터 리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	typedef vector<int>				SMALLFRACTIONTYPE_VECTORLIST;

	FRACTION_VECTORLIST				m_vFractionList[5];
	SMALLFRACTIONTYPE_VECTORLIST	m_vSmallFractionTypeList[5];


public:
	CSkillShopProcess();
	virtual ~CSkillShopProcess(void);

    virtual void					Terminate();

	virtual void					InitProcess();

	virtual void					RenderFrame();
	virtual void					Update(float fFrameTime, float fAccumTime);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	virtual bool					OnDefault(NiEventRef pEventRecord);
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool					OnLostDevice();
	virtual bool					OnResetDevice();

	CBaseProcess*					GetSameProcess();

	void							LoadGround();
	void							LoadCharacter();
	void							LoadCameraType(const char *sCameraFileName);
	void							FUNCTION_3DRotation();

	// 스킬샵 뷰 관련
	virtual void					FUNCTION_InitShop();//아이템 샾 들어갈때 초기화
	virtual void					FUNCTION_UpdateShop();//아이템 샾 업데이트
	virtual void					FUNCTION_ItemRender(int ItemIndex, int nSkillCode, int nPointPrice, int nCoinPrice, char *sName = NULL, char *sIconFile = NULL, int nSell = BUY_KIND_POINT);
	
	// 스킬 코드 관련
	void							InitSkillCode( char nPosition );
	void							InitTrainingCode();
	void							InitCeremonyCode();

	void							UpdateSkillCode( char nPage );
	void							UpdateTrainingCode( char nPage );
	void							UpdateCeremonyCode( char nPage );

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							FUNCTION_BigFraction(int nBigFraction);
	void							FUNCTION_SmallFractionInit(int nBigFraction);
	void							FUNCTION_SmallFraction(int nSmallFraction);
	void							FUNCTION_DetailTabChange(char idx);
	void							FUNCTION_DetailTabUpdate( int ItemIndex );

	// 페이지 관련
	char							FUNCTION_PageUpdate(char nPage);
	void							FUNCTION_GoPage(int nPage);
    
	
	// 기타 UI관련 함수
	void							UpdatePositionArrange();
	void							FUNCTION_Selection();
	void							FUNCTION_Buy(char idx);
	
	// 스킬보기 관련
    void                            FUNCTION_Replay();
	void                            FUNCTION_SkillView();
	void							FUNCTION_CeremonyView();	
	void							FUNCTION_View();

	// 툴팁
	//void							FUNCTION_SkillShopToolTipViewUpdate(LPARAM lParam);
};