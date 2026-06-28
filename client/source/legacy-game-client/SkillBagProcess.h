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
#include "MySkillWindow.h"

class CSkillBagProcess : public CBaseProcess
{
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_END
	};

public:

	CObjPlayerInfo*					m_pObjPlayerInfo;//캐릭터 인포
	POINT							m_pDNDPoint;
	int								m_nResourceType;

	// 분류 관련
	int								m_nSmallFractionStart;
	int								m_nBigFractionIndex;//대분류 번호
	int								m_nSmallFractionIndex;//소분류 번호
	int								m_nDetailTabIndex;
	bool							m_bListShape;//아이템 리스트 형태 기본 스몰

	// 페이징 관련
	int								m_nCurrentPage;
	int								m_nTotalPage;

	// 아이템 관련
	int								m_nShopEquipWear[MAX_EQUIP];//착용 의상	
	CResBaseFormat*					m_pEquipWearModelID[MAX_EQUIP];	//착용 의상 아이디
	int								m_nSkin;
	int								m_nCode[LIST32_SIZE];//코드
	char							m_sName[OBJECT_NAME_SIZE];
	char							m_sIconFile[FILE_NAME_SIZE];
    MySkillWindow                   m_cMySkillWindow;
	char							m_nSelectArea;
    int                             m_nSelectIndex;

	// 분류 관련 벡터 리스트
	typedef vector<std::string>		FRACTION_VECTORLIST;
	typedef vector<int>				SMALLFRACTIONTYPE_VECTORLIST;

	FRACTION_VECTORLIST				m_vFractionList[5];
	SMALLFRACTIONTYPE_VECTORLIST	m_vSmallFractionTypeList[5];

	// 백 관련 벡터 리스트
    CBagSkillList                   m_cBagSkillList;
    CBagTrainingList                m_cBagTrainingList;
	CBagCeremonyList				m_cBagCeremonyList;

public:
	CSkillBagProcess();
	virtual ~CSkillBagProcess(void);

	virtual void					InitProcess();
	CBaseProcess*					GetSameProcess();

	virtual void					RenderFrame();
	virtual bool					OnDefault(NiEventRef pEventRecord);
	virtual void					Update(float fFrameTime, float fAccumTime);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void					Terminate();
	
	void							LoadGround();
	void							LoadCharacter();
	void							UpdateCamera(int Index);
	void							FUNCTION_3DRotation();


	void							FUNCTION_InitSkillBag();	//스킬 인벤 초기화
	void							FUNCTION_InitTrainingBag();	//트레이닝 인벤 초기화
	void							FUNCTION_InitCeremonyBag();	//세레모니 인벤 초기화

	virtual void					FUNCTION_InitShop();//아이템 샾 들어갈때 초기화
	virtual void					FUNCTION_UpdateShop();//아이템 샾 업데이트
	virtual void					FUNCTION_TerminateShop();//아이템 샾 종료할때
	virtual void					FUNCTION_ItemRender(int ItemIndex, int nSkillCode, int nLimitLevel = -1, int nEquip = 0, char *sName = NULL, char *sIconFile = NULL, int nSell = BUY_KIND_POINT);

    void                            FUNCTION_MySkillPageChange(char idx);
	void							FUNCTION_GoPage(int nPage);

	// 분류 관련
	void							FUNCTION_InitFraction();
	void							FUNCTION_BigFraction(int nBigFractionIndex);
	void							FUNCTION_SmallFractionInit(int nBigFraction);
	void							FUNCTION_SmallFraction(int nSmallFractionIndex);
	void							FUNCTION_DetailTabChange(char idx);
	void							FUNCTION_DetailTabUpdate(int ItemIndex);
	
	// 백 관련
	virtual void					FUNCTION_GetBagList();
	virtual void					FUNCTION_GetSkillBagList(int nPage);
	virtual void					FUNCTION_GetTrainingBagList(int nPage);
	virtual void					FUNCTION_GetCeremonyBagList(int nPage);

	// 기타 UI관련
	void							FUNCTION_Selection();
	void							FUNCTION_SkillEquip(char idx);
	void							FUNCTION_SkillDivest(char idx);
	void							FUNCTION_SkillUpgrade(char idx);
    void                            FUNCTION_AllDivest();
	void							FUNCTION_ItemListExchange(bool bItemListShape);

	// 툴팁
	void							FUNCTION_SkillBagToolTipViewUpdate();
};
