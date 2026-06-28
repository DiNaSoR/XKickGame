<?
define ("WEATHER_FINE",	    1 ); // 맑음
define ("WEATHER_CLOUDY",	2 ); // 흐림
define ("WEATHER_RAIN",	    3 ); // 비
define ("WEATHER_SNOW",	    4 ); // 눈

define ("ALL", 1 ); // 모든 요일
define ("MON", 2 ); // 월요일
define ("TUE", 3 ); // 화요일
define ("WED", 4 ); // 수요일
define ("THU", 5 ); // 목요일
define ("FRI", 6 ); // 금요일
define ("SAT", 7 ); // 토요일
define ("SUN", 8 ); // 일요일

define("GOLDEN_TIME", 1 );		// 골든타임
define("TOURNAMENT_TIME", 2 );	// 토너먼트
define("CLUB_TIME", 3 );		// 클럽타임

class CXKickManager
{
	private $m_hDatabase = null;
	private $m_hWeather  = null;
	private $m_hNotice   = null;
	private $m_hSchedule = null;
	private $m_hMember   = null;

	function CXKickManager()
	{
		$this->m_hDatabase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
		$this->m_hDatabase->Conn();
	}

	function GetWeatherMgr()
	{
		if( $this->m_hWeather == null ) $this->m_hWeather = new CXKickWeather;
		$this->m_hWeather->m_hDatabase = $this->m_hDatabase;
		return $this->m_hWeather;
	}

	function GetNoticeMgr()
	{
		if( $this->m_hNotice == null ) $this->m_hNotice = new CXKickNotice;
		$this->m_hNotice->m_hDatabase = $this->m_hDatabase;
		return $this->m_hNotice;
	}

	function GetScheduleMgr()
	{
		if( $this->m_hSchedule == null ) $this->m_hSchedule = new CXKickSchedule;
		$this->m_hSchedule->m_hDatabase = $this->m_hDatabase;
		return $this->m_hSchedule;
	}

	function GetMemberMgr()
	{
		if( $this->m_hMember == null ) $this->m_hMember = new CXKickMember;
		$this->m_hMember->m_hDatabase = $this->m_hDatabase;
		return $this->m_hMember;
	}

	function GetDatabase()
	{
		return $this->m_hDatabase;
	}
}


class CXKickWeather
{
	public $m_hDatabase = null;
	// 날씨 목록
	function GetWeatherList()
	{
		$strQuery = "
					SELECT weather_seq,weather_type,weather_start,weather_end,weather_date FROM  tb_game_weather 
						ORDER BY weather_seq
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}
	

	function GetWeather( $nSeq )
	{
		$strQuery = "
					SELECT weather_seq,weather_type,weather_start,weather_end,weather_date FROM  tb_game_weather 
						WHERE weather_seq=".$nSeq."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return mysql_fetch_array($hResult, MYSQL_ASSOC );
	}

	function InsertWeather( $nWeatherType, $strStartTime, $strEndTime, $strDate )
	{
		$strQuery = "
						INSERT INTO tb_game_weather 
							SET weather_type=".$nWeatherType.", weather_start='".$strStartTime."', weather_end='".$strEndTime."', weather_date='".$strDate."'
					";
		$this->m_hDatabase->Query($strQuery);			
	}

	function EditWeather( $nWeatherSeq, $nWeatherType, $strStartTime, $strEndTime, $strDate )
	{
		$strQuery = "
							UPDATE tb_game_weather 
								SET weather_type=".$nWeatherType.", weather_start='".$strStartTime."', weather_end='".$strEndTime."', weather_date='".$strDate."'
									WHERE weather_seq=".$nWeatherSeq."
					";
		$this->m_hDatabase->Query($strQuery);			
	}

	function DeleteWeather( $nWeatherSeq )
	{
		$strQuery = "
							DELETE FROM tb_game_weather 
									WHERE weather_seq=".$nWeatherSeq."
					";
		$this->m_hDatabase->Query($strQuery);			
	}
}

class CXKickNotice
{
	public $m_hDatabase = null;
	function GetNoticeList()
	{
		$strQuery = "
					SELECT notice_seq,notice_text,notice_enable,notice_date 
						FROM  tb_game_notice 
							ORDER BY notice_seq
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}
	
	function GetNotice( $nSeq )
	{
		$strQuery = "
					SELECT notice_seq,notice_text,notice_enable,notice_date 
						FROM  tb_game_notice 
							WHERE notice_seq=".$nSeq."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return mysql_fetch_array($hResult, MYSQL_ASSOC );
	}

	function InsertNotice( $strNoticeText, $nNoticeEnable )
	{
		$strQuery = "
						INSERT INTO tb_game_notice 
							SET notice_text='".$strNoticeText."', notice_enable=".$nNoticeEnable.", notice_date=now()  
					";
					echo $strQuery;
		$this->m_hDatabase->Query($strQuery);
	}

	function EditNotice( $nNoticeSeq, $strNoticeText )
	{
		$strQuery = "
							UPDATE tb_game_notice 
								SET notice_text='".$strNoticeText."', notice_date=now() 
									WHERE notice_seq=".$nNoticeSeq."
					";
		$this->m_hDatabase->Query($strQuery);	
	}

	function EnableNotice( $nNoticeSeq, $nEnable )
	{
		$strQuery = "
							UPDATE tb_game_notice 
								SET notice_enable=".$nEnable." , notice_date=now() 
									WHERE notice_seq=".$nNoticeSeq."
					";
		
		$this->m_hDatabase->Query($strQuery);	
	}

	function DeleteNotice( $nNoticeSeq )
	{
		$strQuery = "
							DELETE FROM tb_game_notice 
									WHERE notice_seq=".$nNoticeSeq."
					";
		$this->m_hDatabase->Query($strQuery);			
	}
}

class CXKickSchedule
{
	public $m_hDatabase = null;
	function GetScheduleList()
	{
		$strQuery = "
					SELECT schedule_seq,server_code,schedule_type,schedule_start,schedule_end,schedule_date,schedule_desc
						FROM  tb_game_schedule 
							ORDER BY schedule_seq
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}
	
	function GetSchedule( $nSeq )
	{
		$strQuery = "
					SELECT schedule_seq,server_code,schedule_type,schedule_start,schedule_end,schedule_date,schedule_desc
						FROM  tb_game_schedule 
							WHERE schedule_seq=".$nSeq."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return mysql_fetch_array($hResult, MYSQL_ASSOC );
	}

	function InsertSchedule( $nServerCode, $nScheduleType,  $strStartTime, $strEndTime, $nScheduleDate,  $strScheduleDescription )
	{
		$strQuery = "
						INSERT INTO tb_game_schedule 
							SET server_code=".$nServerCode.",schedule_type=".$nScheduleType.",schedule_start='".$strStartTime."',schedule_end='".$strEndTime."',schedule_date='".$nScheduleDate."',schedule_desc='".$strScheduleDescription."'
					";
		$this->m_hDatabase->Query($strQuery);
	}

	function EditSchedule( $nScheduleSeq, $nServerCode, $nScheduleType,  $strStartTime, $strEndTime, $nScheduleDate, $strScheduleDescription )
	{
		$strQuery = "
							UPDATE tb_game_schedule 
								SET server_code=".$nServerCode.",schedule_type=".$nScheduleType.",schedule_start='".$strStartTime."',schedule_end='".$strEndTime."',schedule_date='".$nScheduleDate."',schedule_desc='".$strScheduleDescription."'
									WHERE schedule_seq=".$nScheduleSeq."
					";
		$this->m_hDatabase->Query($strQuery);	
	}

	function DeleteSchedule( $nScheduleSeq )
	{
		$strQuery = "
							DELETE FROM tb_game_schedule 
									WHERE schedule_seq=".$nScheduleSeq."
					";
		$this->m_hDatabase->Query($strQuery);	
	}
}

class CXKickMember
{
	public $m_hDatabase = null;
	function GetMemberList( $nStartPage, $nEndPage )
	{
		$strQuery = "
					SELECT member_seq,member_id,member_pass,member_name,member_register,member_state,member_partner,member_ip,member_speed,member_createdate,member_logindate,member_deletedate,member_blockdate,member_releasedate
						FROM  tb_mst_member 
							ORDER BY member_seq
								LIMIT ".$nStartPage.", ".$nEndPage."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}

	function GetMemberData( $nSeq )
	{
		$strQuery = "
					SELECT member_seq,member_id,member_pass,member_name,member_register,member_state,member_partner,member_ip,member_speed,member_createdate,member_logindate,member_deletedate,member_blockdate,member_releasedate
						FROM  tb_mst_member 
							WHERE member_seq=".$nSeq."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return mysql_fetch_array($hResult, MYSQL_ASSOC );
	}

	function FindMemberByID( $strID, $nStartPage, $nEndPage )
	{
		$strQuery = "
					SELECT member_seq,member_id,member_pass,member_name,member_register,member_state,member_partner,member_ip,member_speed,member_createdate,member_logindate,member_deletedate,member_blockdate,member_releasedate
						FROM  tb_mst_member 
							WHERE member_id like '".$strID."%'
								LIMIT ".$nStartPage.", ".$nEndPage."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}

	function FindMemberByName( $strName, $nStartPage, $nEndPage )
	{
		$strQuery = "
					SELECT member_seq,member_id,member_pass,member_name,member_register,member_state,member_partner,member_ip,member_speed,member_createdate,member_logindate,member_deletedate,member_blockdate,member_releasedate
						FROM  tb_mst_member 
							WHERE member_name like '".$strName."%'
								LIMIT ".$nStartPage.", ".$nEndPage."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}

	function FindMemberByRegist( $strRegist , $nStartPage, $nEndPage )
	{
		$strQuery = "
					SELECT member_seq,member_id,member_pass,member_name,member_register,member_state,member_partner,member_ip,member_speed,member_createdate,member_logindate,member_deletedate,member_blockdate,member_releasedate
						FROM  tb_mst_member 
							WHERE member_register like '".$strRegist."%'
								LIMIT ".$nStartPage.", ".$nEndPage."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}

	function FindMemberByCharacter( $strCharacter , $nStartPage, $nEndPage )
	{
		$strQuery = "
					SELECT member.* 
						   FROM tb_mst_member as member Inner join tb_game_player as player on member.member_seq=player.member_seq
								WHERE player_name like '%".$strCharacter."%'
								LIMIT ".$nStartPage.", ".$nEndPage."
					";
		$hResult = $this->m_hDatabase->Query($strQuery);			
		return $hResult;
	}

};
?>