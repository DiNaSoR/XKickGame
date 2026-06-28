<?
require_once( dirname(__FILE__)."/xkick_protocol.inc"	);

class CXKickSocket 
{
	var $m_strIP  = "192.168.1.101";
	var $m_nPort  = "3001";
	var $m_nReadTimeOut = 10000; // 10msec
	var $m_hSocket;

	function Connect( $strIP = "", $nPort = 0)
	{
		if( $strIP != "" ) $this->m_strIP = $strIP;
		if( $nPort != 0 )  $this->m_nPort = $nPort;
		$this->m_hSocket = @fsockopen($this->m_strIP, $this->m_nPort, $strErrorNo, $strErrStr, 5); 
		if( $this->m_hSocket != FALSE ) $strErrStr = "OK";
		return $strErrStr;
	}

	function Close()
	{
		fclose($this->m_hSocket);
	}
	
	function Send( $hPacket )
	{
		$nLen = fwrite ($this->m_hSocket , $hPacket->GetBinary(), $hPacket->m_nBodySize + $hPacket->m_nHeadSize );
		return $nLen;
	}

	function Read( $hPacket )
	{
		$hBinary = fread($this->m_hSocket , $hPacket->m_nHeadSize );
		$hPacket->SetHeader( $hBinary );
		$hBinary = fread($this->m_hSocket , $hPacket->m_nBodySize );
		$hPacket->SetBinary( $hBinary );
		return $hPacket;
	}

	function Wait()
	{
		usleep($this->m_nReadTimeOut);
	}

	// 로그인
	function Login()
	{
		$hPacket =  new CCSStsLogin;
		$hPacket->m_nServerCode = 99;
		$this->Send( $hPacket );
		$this->Wait();
		$hPacket =  new CSCStsLogin;
		$hPacket = $this->Read(  $hPacket );
		return $hPacket->m_nResponse;
	}

	// 날씨
	function UpdateWeather()
	{
		$hPacket =  new CCSUpdateWeather;
		$this->Send( $hPacket );
		$this->Wait();
		$hPacket =  new CSCUpdateWeather;
		$hPacket = $this->Read(  $hPacket );
		return $hPacket->m_nResponse;
	}

	// 스케쥴
	function UpdateSchedule()
	{
		$hPacket =  new CCSUpdateSchedule;
		$this->Send( $hPacket );
		$this->Wait();
		$hPacket =  new CSCUpdateSchedule;
		$hPacket = $this->Read(  $hPacket );
		return $hPacket->m_nResponse;
	}

	// 공지
	function UpdateNotice()
	{
		$hPacket =  new CCSUpdateNotice;
		$this->Send( $hPacket );
		$this->Wait();
		$hPacket =  new CSCUpdateNotice;
		$hPacket = $this->Read(  $hPacket );
		return $hPacket->m_nResponse;
	}

	// 전체 채팅 공지
	function SendBroadcast( $nType, $strMsg )
	{
		$hPacket =  new CCSSendBroadcast;
		$hPacket->m_nChatKind = $nType;
		$hPacket->m_sMessage = $strMsg;
		$this->Send( $hPacket );
		$this->Wait();
		$hPacket =  new CSCSendBroadcast;
		$hPacket = $this->Read(  $hPacket );
		return $hPacket->m_nResponse;
	}
}
?>