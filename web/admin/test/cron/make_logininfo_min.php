#!/usr/bin/php -q
<?
//현재 서버별 접속자 수치를 tb_log_logininfo_min 테이블에 기록 (5분마다 실행)
//금일의 동접 평균을 tb_log_logininfo_ever 테이블에 기록

function getmicrotime() 
{ 
    list($usec, $sec) = explode(" ", microtime()); 
    return ((float)$usec + (float)$sec); 
}

include "/home/www/admintest.xkick.gamepower7.com/class_inc/config.inc";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/database.inc";
$dBase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
$dBase->DBSele = "xkick_log";
$dBase->Conn();

$dateStr = Date("Y-m-d");
$datetimeStr = Date("Y-m-d H:i:s");
$tname = "tb_log_connect_" . date("y") . "_" . date("m");

/*function MakeLoginInfo()
{
	GLOBAL $dBase;
	GLOBAL $datetimeStr;

	$cnt = 0;
	$failFlag = 0;

	$time_start = getmicrotime();

	$dBase->Close();
	$dBase->DBSele = "xkick";
	$dBase->Conn();

	//서버별 현재 접속자 수집
	$Sql = "
				SELECT cod.server_name as server_name, cod.server_code, ifnull(data.cnt,0) as cnt 
					FROM tb_game_server as cod LEFT JOIN  (SELECT trio_server, COUNT(*) as cnt FROM tb_game_trio as a GROUP BY trio_server HAVING trio_server > 0) as data ON cod.server_code=data.trio_server 
				where cod.server_code <> 0;
	";
	$result = $dBase->Query($Sql);
	while($rows = mysql_fetch_row($result))
	{
		$dBase->Close();
		$dBase->DBSele = "xkick_log";
		$dBase->Conn();

		$Sql = "
					INSERT INTO tb_log_logininfo_min(logininfo_date, serv_code, logininfo_cnt) 
						VALUES('$datetimeStr', $rows[1], $rows[2])
		";
		printf("$Sql\n");
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("데이터베이스 오류입니다2\n");
			exit;
		}
		printf("$datetimeStr($rows[0]) : $rows[1]\n");	
	}

	$time_end = getmicrotime();
	$time = $time_end - $time_start;
	printf("query_time : ".sprintf("%2.2f", $time)." second\n");
}*/

function MakeConEverage()
{
	GLOBAL $dBase;
	GLOBAL $dateStr;
	GLOBAL $tname;

	$start_str = $dateStr . " 00:00:00";
	$end_str = $dateStr . " 23:59:59";

	$Sql = "
				SELECT connect_date AS DAT, COUNT(*) 
					FROM $tname 
						WHERE connect_date >= '$start_str' 
						AND connect_date <= '$end_str' 
				GROUP BY DAT 
				ORDER BY DAT 
				LIMIT 1 
	";
	$result = $dBase->Query($Sql);
	$rows = mysql_fetch_row($result);
	$serverCnt = $rows[1];

	$Sql = "
				SELECT DATE_FORMAT(connect_date, '%Y-%m-%d') AS DAT, COUNT(*), SUM(connect_count) 
					FROM $tname 
						WHERE connect_date >= '$start_str' 
						AND connect_date <= '$end_str' 
				GROUP BY DAT
	";
	$result = $dBase->Query($Sql);
	$rows = mysql_fetch_row($result);
	if($rows[0])
	{
		$Sql = "
					DELETE FROM tb_log_logininfo_ever WHERE logininfo_date = '$dateStr'
		";
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("DB ERROR3\n");
			exit;
		}

		$everCnt = $rows[2] / ($rows[1] / $serverCnt);
		$everCnt = round($everCnt);

		printf("$rows[2] / $rows[1] / $serverCnt\n");

		$Sql = "
					INSERT INTO tb_log_logininfo_ever(logininfo_date, logininfo_cnt) VALUES('$dateStr', $everCnt)
		";
		printf("$Sql\n");
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("DB ERROR4\n");
			exit;
		}
	}
}

function MakeConMax()
{
	GLOBAL $dBase;
	GLOBAL $dateStr;
	GLOBAL $tname;

	$start_str = $dateStr . " 00:00:00";
	$end_str = $dateStr . " 23:59:59";

	$Sql = "
				SELECT connect_date AS DAT, SUM(connect_count) AS P 
					FROM $tname 
						WHERE connect_date >= '$start_str' 
						AND connect_date <= '$end_str' 
				GROUP BY DAT
				ORDER BY P DESC 
				LIMIT 1 
	";
	$result = $dBase->Query($Sql);
	$rows = mysql_fetch_row($result);
	if($rows[0])
	{
		$Sql = "
					DELETE FROM tb_log_logininfo_max WHERE logininfo_date = '$dateStr'
		";
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("DB ERROR3\n");
			exit;
		}

		$maxCnt = round($rows[1]);

		$Sql = "
					INSERT INTO tb_log_logininfo_max(logininfo_date, logininfo_cnt) VALUES('$dateStr', $maxCnt)
		";
		printf("$Sql\n");
		if(!mysql_query($Sql, $dBase->DB))
		{
			printf("DB ERROR4\n");
			exit;
		}
	}
}

//MakeLoginInfo();
MakeConEverage();
MakeConMax();

$dBase->Close();
?>