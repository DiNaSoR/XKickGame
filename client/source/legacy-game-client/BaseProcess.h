#pragma once

#include <d3dx9.h>
#include <NiInputSystem.h>
#include "3DRenderedTexBoard.h"
#include "UIManager.h"
#include "ObjGroundInfo.h"
#include "ObjPlayerInfo.h"
using std::map;

enum PROCESS
{
	PROCESS_NONE = 0x00,
	PROCESS_FRAMELOADING,		//왁구 로딩
	PROCESS_GAMELOGIN,			//로그인
	PROCESS_CHOICETRIO,			//캐릭터 선택
	PROCESS_CREATECHARACTER,	//캐릭터 생성
	PROCESS_CHOICESERVER,		//서버선택
	PROCESS_CHOICEROOM,			//룸선택
	PROCESS_READYROOM,			//대기룸

	PROCESS_GAMELOADING,		//게임 로딩
	PROCESS_GAMEPLAY,			//게임
	PROCESS_GAMERESULT,			//게임종료

	PROCESS_ITEMSHOP,			//아이템 샾
	PROCESS_SKILLSHOP,			//스킬 샾

	PROCESS_ITEMBAG,			//아이템 가방
	PROCESS_SKILLBAG,			//스킬 가방

	PROCESS_GUIDETUTORIAL,		//가이드 튜토리얼
	PROCESS_TUTORIALSTEP1MAIN,	//튜토리얼 플레이
	PROCESS_TUTORIALSTEP1PLAY,	//튜토리얼 플레이
	PROCESS_TUTORIALSTEP2MAIN,	//퀘스트 메인
	PROCESS_TUTORIALSTEP2PLAY,	//퀘스트 플레이
	PROCESS_CHOICECLASS,		//전직창
	PROCESS_CREATEQUEST,		//퀘스트 생성창

	PROCESS_REPLAYMAIN,			//리플레이 메인창

	PROCESS_EXIT
};

class CUIPopUp;

class CBaseProcess
{
protected:
	enum STATE
	{
		STATE_START = 0x00,
	};

	ID3DXFont*						g_pFont;

	int								m_nProcessCmd;

	int								m_nState;

	bool							m_bIsRender;

	C3DRenderedTexBoard				m_cCharacterTexBoard;

protected:
	CBaseProcess();
public:
	virtual ~CBaseProcess(void);

	void							FUNCTION_3DRender(const char* sWindow, bool bAlphaBlend = true);
	void							FUNCTION_3DRender(const char* sWindow, NiCamera* pCamera, bool bAlphaBlend = true);

	virtual void					UpdateState(float fFrameTime, float fAccumTime) {}

	int		GetProcessCmd() { return m_nProcessCmd; }

	virtual CBaseProcess* GetSameProcess();
	bool IsCurrentProcess(int nProcess);

	int				GetProcessState() { return m_nState; }

	virtual void Terminate();
	virtual bool CreateScene();

	virtual bool OnLostDevice();
	virtual bool OnResetDevice();

	void BaseOnLostDevice();
	void BaseOnResetDevice();

	void UpdateFrame(float fFrameTime, float fAccumTime);
	virtual void UpdateProcess(float fFrameTime, float fAccumTime) = 0;
	virtual void RenderBefore()										{ }
	virtual void RenderFrame();

	virtual void ProcessInput();

	virtual bool OnDefault(NiEventRef pEventRecord);
	void Add_PopUp(const char* sTargetControl);
	void Add_PopUpHoverArea(const char* sTargetControl, const char* sControlName);

	void InitToolTipList();

	virtual void InitProcess() {};

public:

private:
	string sKeyWord_ToolTipListNum;
	string sKeyWord_ToolTip;

	map<string, CUIPopUp*> m_mPopUpList;
	map<string, string> m_mToolTipList;

	void UpdatePopUp();
	void UpdateToolTip();
};