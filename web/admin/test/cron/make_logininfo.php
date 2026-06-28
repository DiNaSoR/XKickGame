#!/usr/bin/php -q
<?
//로그인 기록이 있는 고객의 수치를 tb_log_logininfo 테이블에 기록 (10분마다 실행)
//로그인 기록이 있는 고객의 레벨별 수치를 tb_log_logininfo_level 테이블에 기록 (10분마다 실행)

function getmicrotime() 
{ 
    list($usec, $sec) = explode(" ", microtime()); 
    return ((float)$usec + (float)$sec); 
}

include "/home/www/admintest.xkick.gamepower7.com/class_inc/config.inc";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/database.inc";
$dBase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
$dBase->DBSele = "xkick";
$dBase->Conn();

function MakeLoginInfo()
{
	GLOBAL $dBase;

	$cnt = 0;
	$failFlag = 0;

	$time_start = getmicrotime();

	/*
	$today = Date("Y-m-d");
	$Sql = "SELECT DATE_ADD('".$today."', INTERVAL-1 DAY)";
	$result = $dBase->Query($Sql);
	$rows = mysql_fetch_row($result);
	$datestr = $rows[0];
	*/
	$datestr = Date("Y-m-d");

	$startdate = $datestr . " 00:00:00";
	$enddate= $datestr . " 23:59:59";

	//cust_seq별 하루 접속자 로깅
	$Sql = "
				SELECT COUNT(*) 
					FROM tb_mst_member 
						WHERE member_createdate >= '$startdate' 
						AND member_createdate <= '$enddate' 
	";
	$result = $dBase->Query($Sql);
	if(mysql_num_rows($result))
	{
		$dBase->Close();
		$dBase->DBSele = "xkick_log";
		$dBase->Conn();

		$Sql = "
					DELETE FROM tb_log_logininfo WHERE logininfo_date = '$datestr' 
		";
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("데이터베이스 오류입니다1\n");
			exit;
		}

		while($rows = mysql_fetch_row($result))
		{
			$Sql = "
						INSERT INTO tb_log_logininfo(logininfo_date, logininfo_cnt, cust_seq) 
							VALUES('$datestr', $rows[0], 1)
			";
			printf("$Sql\n");
			if(!mysql_query($Sql, $dBase->DB))
			{
				printf("데이터베이스 오류입니다2\n");
				exit;
			}
			//printf("$datestr($rows[0]) : $rows[1]\n");
		}
		$time_end = getmicrotime();
		$time = $time_end - $time_start;
		printf("query_time : ".sprintf("%2.2f", $time)." second\n");
	}

	//레벨별 하루 접속자 로깅
	$Sql = "DELETE FROM tb_log_logininfo_level  WHERE logininfo_date = '".DATE("Y-m-d")."'";
	if(!mysql_query($Sql, $dBase->DB))
	{
		printf("데이터베이스 오류입니다2\n");
		exit;
	}

	$dBase->Close();
	$dBase->DBSele = "xkick";
	$dBase->Conn();

	$Sql = "
				SELECT MAX(b.player_level) 
					FROM tb_mst_member a, tb_game_player b 
						WHERE a.member_seq = b.member_seq 
						AND a.member_logindate >= '$startdate' 
						AND a.member_logindate <= '$enddate' 
	";
	$result = $dBase->Query($Sql);
	$rows = mysql_fetch_row($result);
	$max_level = $rows[0];
	$max_level  = $max_level + (5 - ($max_level % 5));

	$prev_level = 1;
	for($i=5; $i<= $max_level; $i+=5)
	{
		$dBase->Close();
		$dBase->DBSele = "xkick";
		$dBase->Conn();

		$time_start = getmicrotime();

		$Sql = "
					SELECT COUNT(DISTINCT a.member_seq)
						FROM tb_mst_member a, tb_game_player b 
							WHERE a.member_seq = b.member_seq 
							AND a.member_logindate >= '$startdate' 
							AND a.member_logindate <= '$enddate'
							AND b.player_level >= $prev_level 
							AND b.player_level <= $i 
					ORDER BY a.member_seq ASC 
		";
		printf("$Sql\n");
		$result = $dBase->Query($Sql);
		$rows = mysql_fetch_row($result);
		$logincnt = $rows[0];

		$dBase->Close();
		$dBase->DBSele = "xkick_log";
		$dBase->Conn();

		$Sql = "
					INSERT INTO tb_log_logininfo_level(logininfo_date, logininfo_level, logininfo_cnt) 
						VALUES('$datestr', $i, $rows[0])
		";
		printf("$Sql\n");
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("데이터베이스 오류입니다3\n");
			exit;
		}
		$prev_level = $i;

		$time_end = getmicrotime();
		$time = $time_end - $time_start;
		printf("query_time : ".sprintf("%2.2f", $time)." second\n");
	}
}

MakeLoginInfo();
$dBase->Close();
?>