<?
require_once( dirname(__FILE__)."/crypt.inc"	);
require_once( dirname(__FILE__)."/database.inc" );
// 로그인 클래스
class CLogin
{
	private $m_hDatabase   = null;
	private $m_strTableName= "";
	private $m_strIDField  = "";
	private $m_strPassField= "";
	private $m_arrCookField= array();
	private $m_arrAllowedIP= array();
	
	// 다중 파라미터 : TableName, 아이디필드, 패스워드필드, 기타 쿠키로 제작할 필드
	// 로그인을 시도할 때에만 사용하는 생성자
	function CLogin( /* TABLE_NAME, ID_FIELD, PASSWORD_FIELD, ... */ ) 
	{
		if( func_num_args() == 0 ) return;
		$nCnt = 0;
		$args=func_get_args();
		foreach($args as $arg)
		{
			if( $nCnt == 0 ) $this->m_strTableName = $arg;
			if( $this->m_strTableName == "" ) return;
			if( $nCnt == 1 ) $this->m_strIDField   = $arg;
			if( $nCnt == 2 ) $this->m_strPassField = $arg;
			if( $nCnt > 2 ) 
			{
				array_push( $this->m_arrCookField, $arg );
			}	
			$nCnt++;
		}
	}

	function SetAllowIP( /* ... */ )
	{
		if( func_num_args() == 0 ) return;
		$args=func_get_args();
		foreach($args as $arg)
		{
			$arg;
			array_push( $this->m_arrAllowedIP, $arg );
		}
	}

	function CheckAllowIP()
	{
		if( sizeof($this->m_arrAllowedIP) < 1 ) return true;	// 목록이 없으면 모든 아이피 허용
		$strMyIP = $_SERVER['REMOTE_ADDR'];
		$bAllowed= false;

		foreach( $this->m_arrAllowedIP As $strIP )
		{
			if(strpos($strIP, ".*") !==FALSE) 
			{
				$strIP = str_replace("*","",$strIP);
				if( strpos( $strMyIP , $strIP) !== FALSE ) $bAllowed = true;
			} else
			{
				if( $strMyIP == $strIP ) $bAllowed = true;
			}
		}
		
		return $bAllowed;
	}

	function Login( $strID, $strPass, $bIDSave = false, $bMD5 = false )
	{
		if( !$this->CheckAllowIP() ) return -3;	// 허용되지 않은 아이피입니다.
		$this->m_hDatabase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
		$this->m_hDatabase->Conn();
		$strSelectField = "";
		$nSelectField = sizeof($this->m_arrCookField);
		for( $i = 0; $i < $nSelectField; $i++ )
		{
			if( $i > 0 ) $strSelectField .= ",";
			$strSelectField .= $this->m_arrCookField[$i];
		}
		$strQuery = "
							SELECT *
								FROM ".$this->m_strTableName."
									WHERE ".$this->m_strIDField."='".$strID."' 
						";
		$Result = $this->m_hDatabase->Query($strQuery);
		$rows = mysql_fetch_array($Result, MYSQL_ASSOC );

		$nIDExpire = 0;
		if( $bIDSave ) $nIDExpire =  time()+2592000;
		

		if( $bMD5 == true  ) $strPass = md5($strPass);
		if( $rows == false ) return -1;	// 존재하지 않는 아이디입니다.
		if( $rows[$this->m_strPassField] != $strPass ) return -2; // 패스워드가 일치하지 않습니다.
		if( $bIDSave == true ) $this->SetCookie( $this->m_strIDField , $strID, $nIDExpire );
		else				   $this->SetCookie( $this->m_strIDField , "" , $nIDExpire); 
		
		$nArraySize = sizeof($this->m_arrCookField);
		for( $i = 0; $i < $nArraySize; $i++ )
		{
			$this->SetCookie( $this->m_arrCookField[$i], $rows[$this->m_arrCookField[$i]], 0 );
		}
		return 0;
	}

	function Logout()
	{
		$nArraySize = sizeof($this->m_arrCookField);
		for( $i = 0; $i < $nArraySize; $i++ )
		{
			$this->DelCookie( $this->m_arrCookField[$i] );
		}
	}

	function DelCookie( $strName )
	{
		$strCookieName = $this->EncryptCookieName( $strName );
		setcookie ($strCookieName, "", time() - 3600, "/" );
	}

	function SetCookie( $strName, $strData, $nExpire=-1 )
	{
		if( $nExpire == -1 ) $nExpire = time()+2592000;
		$strCookieName = $this->EncryptCookieName( $strName );
		$strCookieData = $this->EncryptCookieData( $strData );
		setcookie(  $strCookieName, $strCookieData, $nExpire, "/"); // 한달
	}

	function GetCookie( $strName )
	{
		$strCookieName = $this->EncryptCookieName( $strName );
		return $this->DecryptCookieData($_COOKIE[$strCookieName]);
	}

	function EncryptCookieName( $strName )
	{
		$strCookieName = CONFIG_SITE_NAME."_".$strName;
		return md5($strCookieName);
	}

	function EncryptCookieData( $strData )
	{
		$hCrypt = new CCryptAPI();
		$strData  = $hCrypt->Encrypt( $strData ); 
		return $strData;
	}

	function DecryptCookieData( $strData )
	{
		$hCrypt = new CCryptAPI();
		$strData  = $hCrypt->Decrypt( $strData ); 
		return $strData;
	}

}
?>