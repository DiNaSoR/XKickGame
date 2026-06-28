<?
define("OK", 0 );
define("MESSAGE_SIZE", (40 * 2)	+ 1 );
define("CHAT_KIND_ANNOUNCE", 6 );
define("CHAT_KIND_NOTICE",	 7 );
define("CHAT_KIND_OPERATOR", 8 );

define ("CM_STS_LOGIN",			101 );
define ("CM_STS_DRAWFORCE",		102 ); // 접속중인 사용자 강제퇴장 (CYG)
define ("CM_UPDATE_WEATHER",	103 ); // 전서버 날씨 갱신 
define ("CM_UPDATE_SCHEDULE",	104 ); // 전서버 스케쥴 갱신 
define ("CM_UPDATE_NOTICE",		105 ); // 전 서버 공지 갱신 
define ("CM_SEND_BROADCAST",	106 ); // 전 서버 공지 채팅 전송 

 // 헤더 프로토콜
class CHeadPacket
{ 
	var $m_nCommand  = 0;
	var $m_nBodySize = 0;
	var $m_nSequence = 0;
	var $m_strHeaderPack = "iii";
	var $m_nHeadSize = 12;

	function GetHeader()
	{
		$strPack = pack( $this->m_strHeaderPack, $this->m_nCommand, $this->m_nBodySize, $this->m_nSequence );
		return $strPack;
	}
	function SetHeader( $hBinary )
	{
		$arrPack = unpack( $this->m_strHeaderPack, $hBinary  );
		$m_nCommand  = array_shift($arrPack);
		$m_nBodySize = array_shift($arrPack);
		$m_nSequence = array_shift($arrPack);
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 로그인
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCSStsLogin extends CHeadPacket 
{
	var $m_nServerCode = 0;

	function CCSStsLogin()
	{
		$this->m_nCommand  = CM_STS_LOGIN;
		$this->m_nBodySize = 2;
	}

	function GetBinary()
	{
		$strPack = pack( "s", $this->m_nServerCode );
		$strPack = $this->GetHeader().$strPack;
		return $strPack;
	}

}

class CSCStsLogin extends CHeadPacket
{ 
	var $m_nResponse = 1;

	function CSCStsLogin()
	{
		$this->m_nCommand  = CM_STS_LOGIN;
		$this->m_nBodySize = 1;
	}

	function SetBinary( $hBinary )
	{
		$arrPack = unpack( "c", $hBinary  );
		$this->m_nResponse = array_shift($arrPack);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  날씨 업데이트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCSUpdateWeather extends CHeadPacket
{
	function CCSUpdateWeather()
	{
		$this->m_nCommand  = CM_UPDATE_WEATHER;
		$this->m_nBodySize = 0;
	}
	function GetBinary()
	{
		$strPack = $this->GetHeader();
		return $strPack;
	}
};

class CSCUpdateWeather extends CHeadPacket
{
	var $m_nResponse = 1;
	function CSCUpdateWeather()
	{
		$this->m_nCommand  = CM_UPDATE_WEATHER;
		$this->m_nBodySize = 1;
	}

	function SetBinary( $hBinary )
	{
		$arrPack = unpack( "c", $hBinary  );
		$this->m_nResponse = array_shift($arrPack);
	}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  스케쥴 업데이트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCSUpdateSchedule  extends CHeadPacket
{
	function CCSUpdateSchedule()
	{
		$this->m_nCommand  = CM_UPDATE_SCHEDULE;
		$this->m_nBodySize = 0;
	}
	function GetBinary()
	{
		$strPack = $this->GetHeader();
		return $strPack;
	}
};

class CSCUpdateSchedule  extends CHeadPacket
{
	var $m_nResponse = 1;
	function CSCUpdateSchedule()
	{
		$this->m_nCommand  = CM_UPDATE_SCHEDULE;
		$this->m_nBodySize = 1;
	}

	function SetBinary( $hBinary )
	{
		$arrPack = unpack( "c", $hBinary  );
		$this->m_nResponse = array_shift($arrPack);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  공지사항 업데이트
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCSUpdateNotice  extends CHeadPacket
{
	function CCSUpdateNotice()
	{
		$this->m_nCommand  = CM_UPDATE_NOTICE;
		$this->m_nBodySize = 0;
	}
	function GetBinary()
	{
		$strPack = $this->GetHeader();
		return $strPack;
	}
};

class CSCUpdateNotice  extends CHeadPacket
{
	var $m_nResponse = 1;
	function CSCUpdateNotice()
	{
		$this->m_nCommand  = CM_UPDATE_NOTICE;
		$this->m_nBodySize = 1;
	}

	function SetBinary( $hBinary )
	{
		$arrPack = unpack( "c", $hBinary  );
		$this->m_nResponse = array_shift($arrPack);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전체 채팅
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCSSendBroadcast extends CHeadPacket
{
	var $m_nChatKind = CHAT_KIND_NOTICE;
//	var $m_nChatKind = CHAT_KIND_ANNOUNCE;
//	var $m_nChatKind = CHAT_KIND_OPERATOR;
	var $m_sMessage = "";

	function CCSSendBroadcast()
	{
		$this->m_nCommand  = CM_SEND_BROADCAST;
		$this->m_nBodySize = MESSAGE_SIZE+1;
	}
	function GetBinary()
	{
		$strPack = pack( "c", $this->m_nChatKind );
		if( strlen($this->m_sMessage) > MESSAGE_SIZE )  $this->m_sMessage  = substr( $this->m_sMessage, 0, MESSAGE_SIZE );
		$this->m_sMessage = str_pad($this->m_sMessage, MESSAGE_SIZE, "\0" );
		$strPack.= $this->m_sMessage; 

		$strPack = $this->GetHeader().$strPack;
		return $strPack;
	}

};

class CSCSendBroadcast extends CHeadPacket
{
	var $m_nResponse;
	function CSCSendBroadcast()
	{
		$this->m_nCommand  = CM_SEND_BROADCAST;
		$this->m_nBodySize = 1;
	}
	function SetBinary( $hBinary )
	{
		$arrPack = unpack( "c", $hBinary  );
		$this->m_nResponse = array_shift($arrPack);
	}
};


?>