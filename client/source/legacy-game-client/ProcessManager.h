/*********************************************************************************************



*			ProcessManager.h

*			프로세스 메니져

*			설명 : 프로세스의 관리 생성 변경을 담당한다


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#ifndef _PROCESS_MANAGER_H_
#define _PROCESS_MANAGER_H_

#include "BaseProcess.h"

#include "Singleton.h"
#include <stack>

class CObjPlayerInfo;

class CProcessManager : public CSingleton < CProcessManager >
{
public:
	enum GS_CMD_TYPE
	{
		GS_CMD_NONE,
		GS_CMD_CHANGE,				//프로세스 변경
		GS_CMD_PUSH,				//프로세스 생성
		GS_CMD_POP,					//프로세스 꺼내기
		GS_CMD_DELETE,				//프로세스 삭제
	};

	typedef std::stack<CBaseProcess*>	ssdef_Process;

	ssdef_Process			m_ProcessStack;

	unsigned int			m_nNextCmdType;

	CBaseProcess*			m_pCurrentProcess;
	CBaseProcess*			m_pNextProcess;

	unsigned int			m_nBeforeProcessCmd;
	CBaseProcess*			m_pBeforeProcess;

	bool					m_bFpsRender;

public:
	CProcessManager(void);
	~CProcessManager(void);

	void Terminate();
	bool CreateScene();

	bool OnLostDevice();
	bool OnResetDevice();

	void SetNextProcess(CBaseProcess* pProcess, unsigned int nCmdType = GS_CMD_CHANGE);
	void SetChangeProcess(CBaseProcess* pProcess, unsigned int nCmdType = GS_CMD_CHANGE);

	void CreateCurrentProcess();
	void DeleteCurrentProcess();

	bool UpdateFrame(float fFrameTime, float fAccumTime);
	void RenderFrame();

	void ProcessInput();

	bool OnDefault(NiEventRef pEventRecord);

	void AttachItem(CObjPlayerInfo* pAnyPlayer, float nAlpha = 1.f);

	void AttachObject(CObjPlayerInfo* pAnyPlayer);

	static void Add_PopUp(const char* sTargetControl);
	static void Add_PopUpHoverArea(const char* sTargetControl, const char* sControlName);

	//패킷 관련 함수
	void						GetCertifyLogin(CHeadPacket* pPacket);
	void						GetCertifyExit(CHeadPacket* pPacket);
	void						GetMemberInfo(CHeadPacket* pPacket);
	void						GetChangeParent(CHeadPacket* pPacket);
	void						GetChangeJang(CHeadPacket* pPacket);
	void						GetRoomInfo(CHeadPacket* pPacket);
	void						GetAthleteInfo(CHeadPacket* pPacket);
	void						GetRaiseFaculty(CHeadPacket* pPacket);
	void						GetChangeSetting(CHeadPacket* pPacket);

	CBaseProcess*				Get_CurrentProcess() const;

	void						ChangeTutorialeMachine(int nTutorialType);
};


#endif