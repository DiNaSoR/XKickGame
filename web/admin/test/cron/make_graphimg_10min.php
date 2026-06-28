#!/usr/bin/php -q
<?
///////////////////////////////////////////////////////////////////////////////////////
//작성일 : 2007.06.21
//작성자 : taba
//내용 : 매일 오전 12시 5분에 전일 데이타를 수집하여 그래프 이미지를 생성
///////////////////////////////////////////////////////////////////////////////////////

include "/home/www/admintest.xkick.gamepower7.com/libchart/libchart/libchart.php";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/config.inc";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/database.inc";
$dBase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
$dBase->DBSele = "xkick";
$dBase->Conn();

//서버리스트 수집
$Sql = "
			SELECT server_code, server_name FROM tb_game_server WHERE server_code > 1 ORDER BY server_code ASC
";
$result = $dBase->Query($Sql);
while($rows = mysql_fetch_row($result))
{
	$rows[1] = str_replace("서버", "Server", $rows[1]);
	$rows[1] = str_replace("섭", "Server", $rows[1]);
	$rows[1] = str_replace("레벨", "Lv.", $rows[1]);
	$rows[1] = str_replace("연습", "Practice", $rows[1]);
	$serv_name[$rows[0]] = $rows[1];
}

mysql_query("USE xkick_log", $dBase->DB);

$dateStr = Date("Y-m");

$start_str = $dateStr . "-01 00:00:00";
$end_str = $dateStr . "-31 23:59:59";

//////////////////////////////////////////////////////////////////////////////////////
// 일평균 동접자 그래프 생성
//////////////////////////////////////////////////////////////////////////////////////
$Sql = "
			SELECT DATE_FORMAT(logininfo_date, '%m-%d'), logininfo_cnt 
				FROM tb_log_logininfo_ever 
					WHERE logininfo_date >= '$start_str' 
					AND logininfo_date <= '$end_str' 
			ORDER BY logininfo_date ASC 
";
$result = $dBase->Query($Sql);

$chart = new VerticalChart();
while($rows = mysql_fetch_row($result))
{
	$chart->addPoint(new Point($rows[0], round($rows[1])));
	//$chart->addPoint(new Point($rows[0], $cnt));
}
$chart->setTitle("$start_str ~ $end_str");
$chart->render("/home/www/admintest.xkick.gamepower7.com/chart_img/logininfo_everage/logininfo_everage_".str_replace("-", "", $dateStr).".png");

//////////////////////////////////////////////////////////////////////////////////////
// 일최대 동접자 그래프 생성(서버별)
//////////////////////////////////////////////////////////////////////////////////////
$Sql = "
			SELECT DATE_FORMAT(logininfo_date, '%m-%d'), logininfo_cnt 
				FROM tb_log_logininfo_max 
					WHERE logininfo_date >= '$start_str' 
					AND logininfo_date <= '$end_str' 
			ORDER BY logininfo_date ASC 
";
$result = $dBase->Query($Sql);
if(mysql_num_rows($result))
{
	$chart = new VerticalChart();
	while($rows = mysql_fetch_row($result))
	{
		$chart->addPoint(new Point($rows[0], round($rows[1])));
		//$chart->addPoint(new Point($rows[0], $cnt));
	}
	$chart->setTitle("$start_str ~ $end_str");
	$chart->render("/home/www/admintest.xkick.gamepower7.com/chart_img/logininfo_max/logininfo_max_".str_replace("-", "", $dateStr).".png");
}

$dBase->Close();
?>