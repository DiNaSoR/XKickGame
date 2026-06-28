#!/usr/bin/php -q
<?
///////////////////////////////////////////////////////////////////////////////////////
//작성일 : 2007.06.21
//작성자 : taba
//내용 : 5분마다 데이타를 수집하여 그래프 이미지를 생성
///////////////////////////////////////////////////////////////////////////////////////
header("Content-Type: text/html; charset=UTF-8"); 

include "/home/www/admintest.xkick.gamepower7.com/class_inc/config.inc";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/database.inc";
$dBase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
$dBase->DBSele = "xkick";
$dBase->Conn();

$Sql = "
			SELECT server_code, server_name FROM tb_game_server WHERE server_code > 1 ORDER BY server_code ASC
";
$result = $dBase->Query($Sql);
while($rows = mysql_fetch_row($result))
{
	//$rows[1] = iconv("UTF-8", "euc-kr", $rows[1]);

	$rows[1] = str_replace("서버", "Server", $rows[1]);
	$rows[1] = str_replace("게임", "Game", $rows[1]);
	$rows[1] = str_replace("섭", "Server", $rows[1]);
	$rows[1] = str_replace("레벨", "Lv.", $rows[1]);
	$rows[1] = str_replace("연습", "Practice", $rows[1]);

	$serv_info[$rows[0]] = $rows[1];
}

mysql_query("USE xkick_log", $dBase->DB);

function MakeChartImg($serv_code, $serv_name)
{
	GLOBAL $dBase;

	include "/home/www/admintest.xkick.gamepower7.com/libchart/libchart/libchart.php";

	$chart = new LineChart();
	$tname = "tb_log_connect_" . sprintf("%02d", date("y")) . "_" . sprintf("%02d", date("m"));
	$cnt = 0;

	$Sql = "
				SELECT DATE_FORMAT(connect_date, '%H:%i'), connect_count 
					FROM $tname 
						WHERE connect_date >= (SELECT DATE_ADD(NOW(), INTERVAL -24 HOUR)) 
						AND connect_server = $serv_code 
						AND connect_date <= NOW() 
				ORDER BY connect_date ASC 
	";
	$result = $dBase->Query($Sql);
	while($rows = mysql_fetch_row($result))
	{
		if($cnt % 6 == 0)
		{
			$tmpVar = $rows[0];
		}
		else
		{
			$tmpVar = "";
		}
		$chart->addPoint(new Point($tmpVar, $rows[1]));
		$cnt++;
	}
	printf("$serv_name\n");
	$chart->setTitle("$serv_name");
	$chart->render("/home/www/admintest.xkick.gamepower7.com/chart_img/server_".$serv_code.".png");
}

//5분단위 동접그래프 생성
foreach($serv_info As $key => $mem)
{
	MakeChartImg($key, $mem);
}

$dBase->Close();
?>