#ifndef __CREATEROOM_DIALOG_H__
#define __CREATEROOM_DIALOG_H__

#include "Singleton.h"

#include "Control.h"

class CObjPlayerInfo;

class CCreateRoomDialog : public CSingleton<CCreateRoomDialog>
{
public:
	char				m_nPosition[ROOM_SCALE_6];

	CObjPlayerInfo*		m_pObjPlayerInfo;
	CObjRoomInfo		m_pRoom;
	char				m_nTabIndex;

public:
	CCreateRoomDialog(void);
	~CCreateRoomDialog(void);

	void Terminate();
	void CreateScene();

	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void FUNCTION_CreateRoomInit();

	void FUNCTION_CreateRoomUpdate(const char* szwindow);

	void FUNCTION_CreateRoomTerminate(const char* szwindow);

	void FUNCTION_CreateRoomOK();

	void FUNCTION_CreateRoomFW(int index, const char* szwindow);

	void FUNCTION_CreateRoomMF(int index, const char* szwindow);

	void FUNCTION_CreateRoomDF(int index, const char* szwindow);

	void FUNCTION_CreateRoomALL1(int index, const char* szwindow);

	void FUNCTION_CreateRoomALL2(int index, const char* szwindow);

	void FUNCTION_CreateRoomGK(int index, const char* szwindow);

	void FUNCTION_CreateRoomLevelRange(short range);
	
	void FUNCTION_CreateRoomSecret();

	void FUNCTION_TabChange(char idx);

	void FUNCTION_RR_Is_Detail();

	void SetRoomNormalOption();
	void SetRoomDetailOption();
};

#endif //__CREATEROOM_DIALOG_H__