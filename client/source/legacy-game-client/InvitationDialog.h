#ifndef _INVITATION_DIALOG_H_
#define _INVITATION_DIALOG_H_

#include "Singleton.h"

#include "Control.h"


class CInvitationDialog : public CSingleton<CInvitationDialog>
{
public:
	CObjPlayerInfo* m_pObjPlayerInfo;
	CLobbyData		m_LobbyList[10];
	int				m_nPage;

protected:
	DWORD							m_nUpdateTime;
	bool							m_bIsUpdate;
	char							m_nTab;

public:
	CInvitationDialog(void);
	~CInvitationDialog(void);

	void Terminate();
	void Init();
	void Update();
	void ListCheck(int idx);
	void LobbyListUpdate();
	void InvitationOk();
	void InvitationCancel();
	void LoadLobbyList(CSCLobbyList* pList);
};

#endif