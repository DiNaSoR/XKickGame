<?
////////////////////////////////////////////////////////////////////////
// 클래스 : 데이터베이스 접속
// 제작자 : 조용길
// 날  짜 : 2009.08
////////////////////////////////////////////////////////////////////////
require_once( dirname(__FILE__)."/config.inc"	);
Class CDatabase // DB에 관련된 클래스 함수
{
	var $DBHost = "";
	var $DBUser = "";
	var $DBPass = "";
	var $DBSele = "";
	var $DB = null;
	var $m_strQuery = "";

	function CDatabase( $Host, $User, $Pass, $Sele, $bAutoConnect = false )
	{
		$this->DBHost = $Host;
		$this->DBUser = $User;
		$this->DBPass = $Pass;
		$this->DBSele = $Sele;
		if( $bAutoConnect ) $this->Conn();
	}

	function ShowLastQuery()
	{
		echo "<hr>";
		echo "<b>Last Query</b>";
		echo "<hr>";
		echo $this->m_strQuery;
		echo "<hr>";
	}

	function __destruct() 
    { 
       @mysql_close($this->DB);
    } 

	function Conn() // DB 접속
	{
		
		for($i=0;$i<3;$i++) 
		{
			$DB = mysql_connect($this->DBHost,$this->DBUser,$this->DBPass);
			if($DB)
			{
				break;
			}
		}
		if($i == 3) 
		{
			$this->PrintM(IDS_DB_ERROR_CONNECT);
			return false;
		} 
		else 
		{
			if(!mysql_select_db($this->DBSele,$DB))
			{
					$this->PrintM($this->DBSele.IDS_DB_ERROR_SELECT);
					return false;
			}
			$this->DB = $DB; 
		}
		mysql_query("SET NAMES utf8;" );
		return true;
	}

	function Close() // DB 종료
	{
		mysql_close($this->DB);
	}
	
	function Trace( $strData )
	{
		$strQuery =   "
						INSERT INTO tb_test SET test_data=\"$strData\"
					  ";
						
		$Result    = $this->Query($strQuery);	
	}

	function Query($Query) // 쿼리문 전송
	{

		$this->m_strQuery = $Query;
		if(empty($this->DB)) 
		{
			$this->PrintM(IDS_DB_ERROR_CONNECT." - QUERY ()");
			exit;
		}
		$Result = mysql_query($Query,$this->DB) or die(mysql_error());
		return $Result;
	}

	function GetOne($Query)
	{
		$Result = $this->Query($Query);			
		$rows = mysql_fetch_array($Result, MYSQL_NUM );
		return $rows[0];
	}

	function PrintM($msg)
	{
		echo "<script> alert('$msg'); </script>";
	}
}
?>
