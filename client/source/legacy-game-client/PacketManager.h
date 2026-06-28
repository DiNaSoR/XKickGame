#ifndef __PACKET_MANAGER_H__
#define __PACKET_MANAGER_H__

#include "Singleton.h"
#include "ComStructure.h"
#include "NetSocket.h"
#include "ReplayPacket.h"
#include "Secure.h"
#include <vector>
#include <map>

using namespace std;

typedef vector< CHeadPacket * > VectorHeadPacket;
typedef void (*Function_t)(CHeadPacket*);
typedef map<int, Function_t>		MapFunction;

// 프로토콜 추가 및 검사 - assert는 알수 없는 이유로 NULL이 되는 경우가 있어서 검사 (CYG) 
#define	INSERT_PROTOCOL_FUNCTION( A, B ) { m_mFunctionHash.insert( pair<int, Function_t>(A,	B) ); assert( m_mFunctionHash[A] != NULL ); }

class CPacketManager : public CSingleton < CPacketManager >
{
public:
	VectorHeadPacket				m_vTCPPacketList;	//TCP 패킷 저장 백터
	VectorHeadPacket				m_vUDPPacketList;	//UDP 패킷 저장 백터

	CNetTCPSocket*					m_pTCPSocket;		//TCP 소캣
	CNetUDPSocket*					m_pUDPSocket;		//UDP 소캣

	CNetQue							m_cTCPQue;			//TCP 패킷 저장 큐
	CNetQue							m_cUDPQue;			//UDP 패킷 저장 큐

	MapFunction						m_mFunctionHash;

	CReplay							m_cReplayQue;		//리플레이용 패킷 저장큐

	CSecure 						m_hTCPSecure;	// TCP 보안 클래스 (CYG)


	bool							m_bIsCertifyLoginDone;
	bool							m_bIsMemberInfoDone;
	bool							m_bIsPlayerInfoDone;
	bool							m_bIsChoiceRoomDone;
	bool							m_bIsAthleteInfoDone;
	bool							m_bIsRobotInfoDone;
	bool							m_bIsGameReadyDone;
	bool							m_bIsGameCountDone;
	bool							m_bIsGamePlayDone;
	bool							m_bIsGameEndDone;

	bool							m_bIsRecording;
	bool							m_bIsCertifyToGame;
	bool							m_bIsLeaveRoom;
	bool							m_bIsRaiseFacultyDone;


public:
	CPacketManager();
	virtual ~CPacketManager();

	void							InitProtocol();

	int								ConnectTCPSocket(char *sIP, int nPort);
	void							CloseTCPSocket();

	int								ConnectUDPSocket(char *sIP, int nPort);
	void							CloseUDPSocket();

	bool							SavePacketInList();
	void							PacketProcess();

	void							SendTCP(char* sPacket, int nPacketSize);
	void							SendUDP(CAddress* pTargetAddr, char* sPacket, int nBodySize);
	void							SendReplay(char* sPacket);
	void							SendAllUDP(char* sPacket, int nBodySize);
	void							SendServerUDP(char* sPacket, int nBodySize);
	void							SendParentUDP(char* sPacket, int nBodySize);
	void							SendChildUDP(char* sPacket, int nBodySize);
	void							SendAngerUDP(char* sPacket, int nBodySize);
	void							SendChildTCP(char* sPacket, int nBodySize);
	
	void							SecureLayer( CHeadPacket * pPacket );

	bool							IsSendRelay();
};

#endif