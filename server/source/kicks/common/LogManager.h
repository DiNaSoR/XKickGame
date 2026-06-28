///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DESC : 로그 관리를 위한 기본 클래스
// DATE : 2009.06
// AUTH : MUTTER(CYG)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

#include "Include.h"
#include "Define.h"

#define MAX_LOG_BUFFER 	2048

#define LOG_ERROR_PATH		"../Log/ERROR"
#define LOG_DATABASE_PATH	"../Log/DATABASE"
#define LOG_OPERATE_PATH	"../Log/OPERATE"
#define LOG_PACKET_PATH		"../Log/PACKET"
#define LOG_ETC_PATH		"../Log/ETC"


enum LOG_LEVEL_DEFINE
{
	LOG_LEVEL_ERROR		= 0,	// 에러로그 함수/라인번호/적색
	LOG_LEVEL_DATABASE,			// 디비로그 함수/라인번호/적색
	LOG_LEVEL_OPERATE,			// 운영로그
	LOG_LEVEL_PACKET,			// 패킷로그
	LOG_LEVEL_ECT,				// 기타로그
	LOG_LEVEL_SCREEN,			// 화면으로만 표시
	LOG_LEVEL_FILE,				// 파일로만 표시
	LOG_LEVEL_ALL
};

#define LOGOUT_ERROR   		CLogManager(__FILE__, __LINE__, LOG_LEVEL_ERROR ).LogOut
#define LOGOUT_DATABASE 	CLogManager(__FILE__, __LINE__, LOG_LEVEL_DATABASE).LogOut
#define LOGOUT_OPERATE 		CLogManager( LOG_LEVEL_OPERATE ).LogOut
#define LOGOUT_PACKET 		CLogManager( LOG_LEVEL_PACKET ).LogOut
#define LOGOUT_ECT	 		CLogManager( LOG_LEVEL_ECT ).LogOut
#define LOGOUT_SCREEN 		CLogManager( LOG_LEVEL_SCREEN).LogOut
#define LOGOUT_TEST			CLogManager( LOG_LEVEL_SCREEN).LogOut
#define LOGOUT_FILE			CLogManager( LOG_LEVEL_FILE).LogOut

class CLogManager
{
public:
	CLogManager(char *strFileName, int nLineNo, int nLogLevel );
	CLogManager( int nLogLevel );
	~CLogManager();
	void		LogOut(const char *strFormat, ...);
	static int	LOG_LEVEL_FILE[LOG_LEVEL_ALL];
	static int	LOG_LEVEL_SCREEN[LOG_LEVEL_ALL];
	static char*LOG_SYSTEM_NAME;	// 사용전에 머신/시스템 명을 지정하고 사용할것
protected:
	char*		m_strFileName;
	int			m_nLineNo;
	int			m_nLogLevel;
};
#endif
