#include "SignBoard.h"
#include "ObjPlayerInfo.h"
#include "ObjectManager.h"
#include "ClientUIManager.h"
#include "UICommonMethod.h"
#include "UIFunction.h"

SignBoard::SignBoard(void)
{
	m_nTickCount = 0;
	m_bXKickOnline = false;
}

SignBoard::~SignBoard(void)
{

}

void SignBoard::InitSignBoard( CSCScheduleList Schedules )
{
	m_cSchedules = Schedules;

	if( m_cSchedules.m_nPacketPosition == PACKET_POSITION_START )
		m_bSignBoard.clear();
	
	char sCur_Time[STRLEN_32]="";

	if(m_nTickCount == 0)
		m_nTickCount = GetTickCount();

	tm now;
	ZeroMemory(&now, sizeof(tm));
	
	now = *localtime( &m_cSchedules.m_nCurrentTime );

	CObjPlayerInfo* pObjPlayerInfo = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));

	for(int i=0;i<m_cSchedules.m_nCount;i++)
	{
		if( m_cSchedules.m_cScheduleList[i].m_nServerCode != pObjPlayerInfo->m_nServerCode && 
			m_cSchedules.m_cScheduleList[i].m_nServerCode != 0 )
			continue;

		if( (m_cSchedules.m_cScheduleList[i].m_nDate == DAY_ALL) || (m_cSchedules.m_cScheduleList[i].m_nDate == DayToCode(now.tm_wday)) )
		{
			if(0 == m_bSignBoard.size())
				m_bSignBoard.push_back(m_cSchedules.m_cScheduleList[i]);
			else
			{
				for(int j=0;j<(int)m_bSignBoard.size();j++)
				{
					if( m_bSignBoard.at(j).m_nTimeType == m_cSchedules.m_cScheduleList[i].m_nTimeType )
					{
						if( m_bSignBoard.at(j).m_nStart > m_cSchedules.m_cScheduleList[i].m_nStart )
							m_bSignBoard.at(j).m_nStart = m_cSchedules.m_cScheduleList[i].m_nStart;
						if( m_bSignBoard.at(j).m_nEnd < m_cSchedules.m_cScheduleList[i].m_nEnd )
							m_bSignBoard.at(j).m_nEnd = m_cSchedules.m_cScheduleList[i].m_nEnd;
						break;
					}					
					
					if( (m_bSignBoard.size()-1) == j )
						m_bSignBoard.push_back(m_cSchedules.m_cScheduleList[i]);
				}
			}
		}
	}

	m_nSignBC = 0;
}

char SignBoard::DayToCode(char nDay)
{
	switch(nDay)
	{
	case 0:	return DAY_SUN;	break;
	case 1:	return DAY_MON;	break;
	case 2:	return DAY_TUE;	break;
	case 3:	return DAY_WED;	break;
	case 4: return DAY_THU; break;
	case 5:	return DAY_FRI;	break;
	case 6:	return DAY_SAT;	break;
	}
	return 0;
}

void SignBoard::DisplaySignBoardTime()
{
	char sText[STRLEN_64]="";
	char sCur_Time[STRLEN_32]="";

	time_t c_time;
	time( &c_time );

	tm now;
	ZeroMemory(&now, sizeof(tm));

	CResMessageTableFormat* pMTable = NULL;

	if(7<(c_time%16))
	{
		c_time = (GetTickCount() - m_nTickCount)/CLK_TCK;
		
		now = *localtime( &c_time );

		now.tm_hour -=9;

		strftime(sCur_Time, 24, "%H:%M:%S", &now);

		pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900040 ,0));
		assert(pMTable);

		sprintf(sText, pMTable->m_sMessage, sCur_Time);
	}
	else
	{
		int nPos=0;

		c_time = m_cSchedules.m_nCurrentTime + (GetTickCount() - m_nTickCount)/CLK_TCK;
		now = *localtime( &c_time );
		int nCurrentTime = (now.tm_hour*3600) + (now.tm_min*60) + now.tm_sec;

		int nCurrentDisplayCount = 0;
		for(int i=0;i<(int)m_bSignBoard.size();i++)
		{
			if( (m_bSignBoard.at(i).m_nStart < nCurrentTime) && (nCurrentTime < m_bSignBoard.at(i).m_nEnd) )
				nCurrentDisplayCount++;
		}

		if(nCurrentDisplayCount == 0)
		{
			strftime(sCur_Time, 24, "%H:%M:%S", &now);

			pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900041 ,0));
			assert(pMTable);
			sprintf(sText, pMTable->m_sMessage, sCur_Time);
		}
		else
		{
			for(int i=0;i<(int)m_bSignBoard.size();i++)
			{
				if( (m_bSignBoard.at(i).m_nStart < nCurrentTime) && 
				(nCurrentTime < m_bSignBoard.at(i).m_nEnd) )
				{
					if(nPos == m_nSignBC-1)
					{
						c_time = m_bSignBoard.at(i).m_nEnd-nCurrentTime;
						now = *localtime( &c_time );
						now.tm_hour-=9;
						if(now.tm_hour<0)
							now.tm_hour = 24 - now.tm_hour;
						strftime(sCur_Time, 24, "%H:%M:%S", &now);
								
						switch( m_bSignBoard.at(i).m_nTimeType )
						{
						case TIME_KIND_TOURNAMENT:
							pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900042 ,0));
							assert(pMTable);
							sprintf(sText, pMTable->m_sMessage, sCur_Time);
							break;
						case TIME_KIND_CLUB:
							pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900043 ,0));
							assert(pMTable);
							sprintf(sText, pMTable->m_sMessage, sCur_Time);
							break;
						case TIME_KIND_GOLDEN:
							pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900044 ,0));
							assert(pMTable);
							sprintf(sText, pMTable->m_sMessage, sCur_Time);
							break;
						}
					}
					nPos++;
				}
			}
		}
	}

	UIFunction::Set_ControlText( "Common_Message", sText );
}

void SignBoard::DisplaySignBoard()
{
	char sText[STRLEN_64]="";

	CUIControl* pT	= CClientUIManager::GetPtr()->FindEventControl("Tournament_Icon");
	assert(pT);
	CUIControl* pC	= CClientUIManager::GetPtr()->FindEventControl("Club_Icon");
	assert(pC);
	CUIControl*	pG	= CClientUIManager::GetPtr()->FindEventControl("Golden_Icon");
	assert(pG);
	
	int nCount = 0;
	int nCurrentTime = 0;

	for(int i=0;i < (int)m_bSignBoard.size();i++)
	{
		time_t c_time;
		time( &c_time );

		c_time = m_cSchedules.m_nCurrentTime + (GetTickCount() - m_nTickCount)/CLK_TCK;

		tm now;
		ZeroMemory(&now, sizeof(tm));

		now = *localtime( &c_time );

		if(2<nCount)
			break;

		nCurrentTime = (now.tm_hour*3600) + (now.tm_min*60) + now.tm_sec;

		if( (m_bSignBoard.at(i).m_nStart < nCurrentTime) && 
			(nCurrentTime < m_bSignBoard.at(i).m_nEnd) ) 
		{
			sprintf(sText, "Static_Icon0%d", nCount++);
			CUIControl* pLight = CClientUIManager::GetPtr()->FindEventControl(sText);
			UIFunction::Set_UIVisibility(sText, N3UI_VISIBLE_TRUE);	
		
			switch(m_bSignBoard.at(i).m_nTimeType)
			{
			case TIME_KIND_TOURNAMENT:
				pLight->SetElement(pT->m_pElement->Clone());
				break;
			case TIME_KIND_CLUB:
				pLight->SetElement(pC->m_pElement->Clone());
				break;
			case TIME_KIND_GOLDEN:
				pLight->SetElement(pG->m_pElement->Clone());
				break;
			}
		}
	}

	if(nCount == 0)
		m_bXKickOnline = true;
	else
		m_bXKickOnline = false;
	
	DisplaySignBoardTime();

	CUIControl* pXKickBoard = CClientUIManager::GetPtr()->FindEventControl("Static_box");
	assert(pXKickBoard);
	CUIControl* pRemBoard = CClientUIManager::GetPtr()->FindEventControl("Static_box01");
	assert(pRemBoard);

	CUIControl* pXKick_Online = CClientUIManager::GetPtr()->FindEventControl("XKick_Online_Ani");
	assert(pXKick_Online);
	CUIControl* pTournament = CClientUIManager::GetPtr()->FindEventControl("Tournament_Ani");
	assert(pTournament);
	CUIControl* pClub_Match = CClientUIManager::GetPtr()->FindEventControl("Club_Match_Ani");
	assert(pClub_Match);
	CUIControl* pGolden_Time = CClientUIManager::GetPtr()->FindEventControl("Golden_Time_Ani");
	assert(pGolden_Time);

	if(m_bXKickOnline)
	{
		if( !pXKick_Online->m_pParent->IsPlay() )
		{
			pXKick_Online->m_pParent->SetVisible(N3UI_VISIBLE_TRUE);
			pXKick_Online->m_pParent->SetPlay(true);
			pXKickBoard->SetVisible(N3UI_VISIBLE_TRUE);
			pRemBoard->SetVisible(N3UI_VISIBLE_FALSE);
		}
		else
			return;
	}
	else
	{
		pXKickBoard->SetVisible(N3UI_VISIBLE_FALSE);
		pRemBoard->SetVisible(N3UI_VISIBLE_TRUE);

		if( !pTournament->m_pParent->IsPlay() && !pClub_Match->m_pParent->IsPlay() && !pGolden_Time->m_pParent->IsPlay() )
		{
			if(m_nSignBC == nCount)
				m_nSignBC=0;

			int nPos=0;

			for(int i=0;i<(int)m_bSignBoard.size();i++)
			{
				if( (m_bSignBoard.at(i).m_nStart < nCurrentTime) && 
				(nCurrentTime < m_bSignBoard.at(i).m_nEnd) )
				{
					if(nPos == m_nSignBC)
					{
						switch( m_bSignBoard.at(i).m_nTimeType )
						{
						case TIME_KIND_TOURNAMENT:
							pTournament->m_pParent->SetVisible(N3UI_VISIBLE_TRUE);
							pTournament->m_pParent->SetPlay(true);
							break;
						case TIME_KIND_CLUB:
							pClub_Match->m_pParent->SetVisible(N3UI_VISIBLE_TRUE);
							pClub_Match->m_pParent->SetPlay(true);
							break;
						case TIME_KIND_GOLDEN:
							pGolden_Time->m_pParent->SetVisible(N3UI_VISIBLE_TRUE);
							pGolden_Time->m_pParent->SetPlay(true);
							break;
						}
						m_nSignBC++;
						break;
					}
					nPos++;
				}
			}
		}
	}
}
