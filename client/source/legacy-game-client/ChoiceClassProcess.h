/*********************************************************************************************



*			TutorialProcess.h

*			서버 선택 프로세스

*			설명 : 서버를 선택할수 있는 프로세스 이다


*			본소스는 (주)EnterPlay의 허가없이 수정될 수 없습니다. 본소스를 수정

*			하기 위해서는 반드시 CCP(change Conrtol Process)룰에 의거 (주)EnterPlay에

*			수정에 관련한 요청을 통하여 정식으로 수정될수 있도록 해야합니다.


*				All Copyrights Reserved (주)EnterPlay corporation - Since 2008

**********************************************************************************************/

#pragma once

#include "BaseProcess.h"
#include "PolygonDraw.h"
#include "3DRenderedTexBoard.h"

class CChoiceClassProcess : public CBaseProcess
{
public:
	enum STATE
	{
		STATE_START = 0x00,
		STATE_INIT_LOAD,
		STATE_GROUND_LOAD,
		STATE_CHARACTER_LOAD,
		STATE_PLAY,
		STATE_END
	};

	CObjPlayerInfo*					m_pObjPlayerInfo;	//캐릭터 인포
	char							m_nPosition;
	char							m_n1stClassStep;
	char							m_nClassIndex;
	POINT							m_pDNDPoint;

	// 통계치 그리기 관련
	float							m_fScale[LIST32_SIZE];		//포지션 통계 능력치
	D3DXVECTOR3						m_vSink;
	D3DXCOLOR						m_nColor[LIST32_SIZE];
	

	LPDIRECT3DDEVICE9				m_pd3dDevice;
	CPolygonDraw					m_pPolyGonDraw;		// 다각형 그리기
	
public:
	CChoiceClassProcess();
	CChoiceClassProcess(char nClassIndex);
	virtual ~CChoiceClassProcess(void);

    virtual void					InitProcess();
	virtual void					Update(float fFrameTime, float fAccumTime);
	virtual void					UpdateState(float fFrameTime, float fAccumTime);
	virtual void					RenderFrame();
	virtual bool					OnDefault(NiEventRef pEventRecord);
	void							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void							UpdateProcess(float fFrameTime,float fAccumTime);
	
	CBaseProcess*					GetSameProcess();
	
	virtual void					Terminate();
	
	void							LoadGround();
	void							LoadCharacter();

	
	struct Vertex
    { 
    public:
        Vertex()
        {
        }
		
        Vertex( float x, float y, float z, float w, D3DCOLOR color )
        {
            this->m_vP.z		= x;
            this->m_vP.y		= y;
			this->m_vP.z		= z;
			this->m_fW			= w;
            this->m_nColor		= color;
        }

        D3DXVECTOR3     m_vP;
		float			m_fW;
        D3DCOLOR        m_nColor;
    };

	void 							PositionDecision();
	void 							InitPolygonDraw();

	void							FUNCTION_InitChoiceClass();
	void 							FUNCTION_ClassNextExplain();
	void 							FUNCTION_PositionExplain(char nPosition);

	//void FUNCTION_SelectPositionVeiw(char idx);
	//void FUNCTION_InitChoiceClass();
	//void FUNCTION_Init2ndChoiceClass();
	//void FUNCTION_Explain();

	void FUNCTION_SkillNameToolTipViewUpdate();

};
