///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DESC : 데이터베이스 관리를 위한 기본 클래스
// DATE : 2009.04
// AUTH : MUTTER(CYG)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "Include.h"
#include "Define.h"

#define MAINDBFLAG		0
#define LOGDBFLAG		1
#define SAMPLEDBFLAG	2
#define PCBANGDBFLAG	3

#define QUERY_SIZE		1024
#define QUERY_SIZE2		2048

#define DB_SIZE			6

class CTime;
typedef std::map<string, char *> MY_ROW; // ROW관리 자료구조 (CYG)
typedef std::map<string, int> MY_FIELD; // FIELD관리 자료구조 (CYG)
typedef std::map<int,string>  DB_LIST; // DB관리 자료구조 (CYG)

#define	FREE_RESULT( a ) {if( a != NULL ){	mysql_free_result(a);	a = NULL;} }


class CDatabase
{
private:
	MYSQL				m_hDatabase[DB_SIZE];
	int					m_nDBSel[DB_SIZE];
	bool				m_bUse[DB_SIZE];
	bool				m_bConn[DB_SIZE];
	bool				m_bInit;
	bool				m_bTrace;
	pthread_mutex_t		m_tMutex;
	string				m_strIP;
	string				m_strUser;
	string				m_strPass;
	string				m_strChar;

	int					InitDB(MYSQL *pSql, string strDBName ); // 초기화
public:
	CDatabase();
	~CDatabase();

	DB_LIST				m_hDBList;
	int					m_nBaseDBID;

	void				SetConnectInfo( string strIP, string strUser, string strPass, string strChar); // 기본정보 설정
	void				AddDB( int nDBID, string strDBName );	// DB 추가
	void				SetBaseDB( int nDBID );				// 기본DB 선택
	MYSQL*				GetDB( const int nFlag );				// DB 선택
	void				ReleaseDB(MYSQL * pMysql);				// DB 해제
	void				CloseDB(MYSQL *pSql);					// DB 종료
	int					PingDB(MYSQL *pSql, const char sDB[]  ); // 상태검사
	int					SendQuery(MYSQL *pSql, char sQuery[], int nSize); // 쿼리전송을 위한 기본함수

	MYSQL_RES*			Query( char * strQuery, ... );				// Query   : Select용, 사용후 리턴된 리소스를 해제해야 한다.
	MYSQL*				Execute( char * strQuery, ... );			// Execute : 실행용, 리소스 해제가 필요없으며 결과를 얻기 위해 MYSQL핸들러를 리턴한다.
	bool				FetchRow( MYSQL_RES* pRes, MY_ROW * pRow );// FetchRow: 필드명으로 접근하기 위해 만들어진 함수 / Query와 쌍으로 동작한다.
	int 				GetDBTime( CTime * pTime );				// DB에서 시간 가져오기

	void				TRACE_ENABLE();
	void				TRACE_DISABLE();


};


class CTime
{
public:
	int			m_nYear;	char		m_nQuarter;	char		m_nMonth;	char		m_nDay;
	char		m_nWeek;	char		m_nHour;	char		m_nMinute;	char		m_nSecond;
public:
	CTime()
	{
		time_t	t;
		tm*		pTime;

		time(&t);
		pTime = localtime(&t);

		m_nYear		= pTime->tm_year + 1900;
		m_nMonth	= pTime->tm_mon + 1;
		m_nDay		= pTime->tm_mday;
		m_nWeek		= pTime->tm_wday;

		m_nHour		= pTime->tm_hour;
		m_nMinute	= pTime->tm_min;
		m_nSecond	= pTime->tm_sec;

		switch(m_nMonth)
		{
			case 1:  case 2:  case 3:	m_nQuarter = 1; break;
			case 4:  case 5:  case 6:	m_nQuarter = 2; break;
			case 7:  case 8:  case 9:	m_nQuarter = 3; break;
			case 10: case 11: case 12:	m_nQuarter = 4; break;
		}
	}
	~CTime() {}
};


#endif
