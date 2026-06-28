/*
Name :  Version Maker
Auth :  Cho Yong Gil
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/vfs.h>
#include "unistd.h"

#define P_MAX_BUF 255
int main(void)
{
	/*
	FILE * fp;

	fp = fopen( "version.h", "r" );

	if( fp == NULL ) return -1;
	const int nSize=256;
	char buf[nSize] = {0};

	fread( buf, sizeof(char), nSize, fp );

	char * strVersion = strstr( buf, "SERVER_VERSION" );
	strVersion = strstr( strVersion, " " );

	int nVerion = 0;
	if( strVersion == NULL ) return -2;

	int nVersion = atoi( strVersion );
	fclose( fp );

	nVersion++;
	memset( buf, 0x00, nSize );
	sprintf( buf, "#define SERVER_VERSION %d\n", nVersion );
	fp = fopen( "version.h", "w" );
	fwrite( buf, sizeof(char), strlen(buf), fp );
	fclose( fp );
	*/

	FILE * fp;

	fp = fopen( "version.h", "r" );

	if( fp == NULL ) return -1;
	const int nSize=256;
	char oldBuf[nSize] = {0};

	fread( oldBuf, sizeof(char), nSize, fp );

	char * strVersion = strstr( oldBuf, "SERVER_VERSION" );
	strVersion = strstr( strVersion, " " );

	int nVersion = 0;
	if( strVersion == NULL ) return -2;

	nVersion = atoi( strVersion );
	fclose( fp );



		char strTop[P_MAX_BUF];
		char buf[P_MAX_BUF];
		memset( strTop, 0x00, P_MAX_BUF );
		memset( buf, 0x00, P_MAX_BUF );
		FILE*cmdp=popen("svn up","r");
		usleep( 50 );
		while (1)
		{
			usleep( 10 );
			if( fgets(strTop,P_MAX_BUF,cmdp) == false )
			{
				break;
			}
			sprintf( buf, "%s%s", buf, strTop );
		}
		
		printf( buf );

		pclose(cmdp);
		int nLen = strlen( buf );
		if( nLen < 3 ) return 0;
		buf[nLen-1] = '\0';
		buf[nLen-2] = '\0';
		for( int i = nLen; i > 0; i-- )
		{
			if( buf[i] == ' ' )
			{
				strcpy( strTop,  &buf[i+1] );
				break;
			}
		}


		printf( "¿¢½ºÅ± ¼­¹ö¼Ò½º ¸®ºñÀü : %d -> %s\n", nVersion, strTop );

		int nNewVersion = atoi( strTop);
		if( nNewVersion > nVersion )
		{
			memset( buf, 0x00, P_MAX_BUF );
			sprintf( buf, "#define SERVER_VERSION %d\n", nNewVersion );
			fp = fopen( "version.h", "w" );
			fwrite( buf, sizeof(char), strlen(buf), fp );
			fclose( fp );
		}


	return 0;
}
