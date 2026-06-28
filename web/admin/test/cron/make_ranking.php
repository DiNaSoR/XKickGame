#!/usr/bin/php -q
<?
@set_time_limit(0);

function getmicrotime() 
{ 
    list($usec, $sec) = explode(" ", microtime()); 
    return ((float)$usec + (float)$sec); 
}

//랭킹에 포함되는 케릭터에 대한 제한 경기수를 매일 5경기씩 증가
/*if(!($fp = fopen("/home/kicks/wwwhome/cron/rank_cnt.dat","r")))
{
	printf("참조파일 오픈실패\n");
	exit;
}
if($fp)
{
	$match_limit = trim(fgets($fp, 100));
	$match_limit_month = (int)$match_limit - 100;
}
if(!($fp = fopen("/home/kicks/wwwhome/cron/rank_cnt.dat","w")))
{
	printf("참조파일 오픈실패2\n");
	exit;
}
fputs($fp, $match_limit+5);
*/

/*$startDate = "2006-07-01";
$endDate = date("Y-m-d");

$arrStartDate = explode("-", $startDate);
$arrEndDate = explode("-", $endDate);

$startTime = mktime(0,0,0,$arrStartDate[1],$arrStartDate[2],$arrStartDate[0]);
$endTime = mktime(0,0,0,$arrEndDate[1],$arrEndDate[2],$arrEndDate[0]);

$interval = number_format(intval(($endTime-$startTime)/86400));*/

$start_date = "2010-07-01 00:00:00";
$end_date = date("Y-m-d") . " 00:00:00";
$interval = intval((strtotime($end_date)-strtotime($start_date))/86400);

//$match_limit = 200 + ($interval * 2);
$match_limit = $interval * 2;
$match_limit_month = Date("d") * 3;

/*printf("$match_limit\n");
printf("$match_limit_month\n");
exit;*/

if(!($fp = fopen("/home/www/admintest.xkick.gamepower7.com/cron/logs/".date("Ymd")."_rank.log","a+")))
{
	printf("로그파일 오픈실패\n");
	exit;
}
fputs($fp, "== ". date("Ymd H:i:s") ." ==\n");

include "/home/www/admintest.xkick.gamepower7.com/class_inc/config.inc";
include "/home/www/admintest.xkick.gamepower7.com/class_inc/database.inc";
$dBase = new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME );
$dBase->Conn();

$mon = DATE("m");
if($mon >= 1 && $mon <= 3) $part = 1;
else if($mon >= 4 && $mon <= 6) $part = 2;
else if($mon >= 7 && $mon <= 9) $part = 3;
else $part = 4;

$ymStr = date("y")."_".sprintf("%02d",$part);

if(CheckMonTable($ymStr) == false) {
	fputs($fp, "테이블 생성 실패 $ymStr \n");
	printf("테이블 생성 실패 $ymStr \n");
	fclose($fp);
	exit;
}

//월별 랭킹 테이블 존재 체크
function CheckMonTable($ymStr)
{
	GLOBAL $dBase;
	$Sql = "SELECT COUNT(*) FROM tb_game_rank_".$ymStr;
	if(!@mysql_query($Sql, $dBase->DB))
	{
		$Sql = "
					CREATE TABLE `tb_game_rank_$ymStr` (
					  `rank_num` int(11) unsigned NOT NULL default '0',
					  `rank_code` int(11) unsigned NOT NULL default '0',
					  `player_seq` int(11) unsigned NOT NULL default '0',
					  `rank_point` float(10,3) unsigned NOT NULL default '0.000',
					  PRIMARY KEY  (`player_seq`,`rank_code`),
					  KEY `rank_code` (`rank_code`)
					) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='케릭터 월별 랭킹 정보';
		";
		printf($Sql."\n");
		if(!mysql_query($Sql, $dBase->DB))
		{
			return false;
		}
	}
	return true;
}

function MakeRank($rank_code, $type="")
{
	GLOBAL $fp;
	GLOBAL $dBase;
	GLOBAL $ymStr;
	GLOBAL $match_limit;
	GLOBAL $match_limit_month;

	$cnt = 0;
	$failFlag = 0;
	$whereStr = "";

	$time_start = getmicrotime();

	if($type == "month")
	{
		$record_tname = "tb_game_record_".$ymStr;
		$rank_tname = "tb_game_rank_".$ymStr;
		$limitCnt = $match_limit_month;
	}
	else
	{
		$record_tname = "tb_game_record";
		$rank_tname = "tb_game_rank";
		$limitCnt = $match_limit;
	}

	$Sql = "DELETE FROM ".$rank_tname." WHERE rank_code = ".trim($rank_code);
	
	if(!$dBase->Query($Sql))
	{
		fputs($fp, "초기화실패 $rank_code \n");
		printf("초기화 실패 $rank_code \n");
		return -1;
	}

	switch ($rank_code) 
	{
		case 101 :		//경기수	
			$sortStr = "a.record_match";
			break;
		case 102 :		//승리 횟수
			$sortStr = "a.record_win";
			break;
		case 103 :		//승점
			$sortStr = "(a.record_win * 3) + (a.record_draw * 1)";
			break;
		case 104 :		//종합점수
			$sortStr = "a.record_mark";
			break;
		case 105 :		//MVP 횟수
			$sortStr = "a.record_mvp";
			break;
		case 106 :		//득점
			$sortStr = "a.record_goal";
			break;
		case 107 :		//어시스트 횟수
			$sortStr = "a.record_assist";
			break;
		case 108 :		//커팅 횟수
			$sortStr = "a.record_cut";
			break;
		case 109 :		//유효슈팅 횟수
			$sortStr = "a.record_shoot";
			break;
		case 110 :		//유효스틸 횟수
			$sortStr = "a.record_steal";
			break;
		case 111 :		//유효태클 횟수
			$sortStr = "a.record_tackle";
			break;
		case 112 :		//유효패스 횟수
			$sortStr = "a.record_pass";
			break;
		case 201 :		//평점
			$sortStr = "((a.record_mark / 10) / a.record_match)";
			break;
		case 202 :		//평균 득점
			$sortStr = "(a.record_goal / a.record_match)";
			break;
		case 203 :		//평균 어시스트
			$sortStr = "(a.record_assist / a.record_match)";
			break;
		case 204 :		//평균 커팅
			$sortStr = "(a.record_cut / a.record_match)";
			break;
		case 205 :		//평균 슈팅
			$sortStr = "(a.record_shoot / a.record_match)";
			break;
		case 206 :		//평균 스틸
			$sortStr = "(a.record_steal / a.record_match)";
			break;
		case 207 :		//평균 태클
			$sortStr = "(a.record_tackle / a.record_match)";
			break;
		case 208 :		//평균 패스
			$sortStr = "(a.record_pass / a.record_match)";
			break;
		case 301 :		//승률
			$sortStr = "(a.record_win / a.record_match)";
			break;
		case 302 :		//슈팅 성공률
			//$sortStr = "(a.record_shoot / a.record_tryshoot)";
			$sortStr = "a.record_shoot / (a.record_tryshoot + a.record_shoot)";
			$whereStr = "AND a.record_tryshoot >= 50";
			break;
		case 303 :		//스틸 성공률
			$sortStr = "(a.record_steal / a.record_trysteal)";
			$whereStr = "AND a.record_trysteal >= 50";
			break;
		case 304 :		//태클 성공률
			$sortStr = "(a.record_tackle / a.record_trytackle)";
			$whereStr = "AND a.record_trytackle >= 50";
			break;
		case 305 :		//패스 성공률
			$sortStr = "(a.record_pass / a.record_trypass)";
			$whereStr = "AND a.record_trypass >= 50";
			break;
		default : 
			printf("CODE ERROR\n");
			return -2;
			break;
	}

	$Sql = "
				SELECT a.player_seq, $sortStr 
					FROM $record_tname a, tb_game_player b 
						WHERE a.player_seq = b.player_seq 
						AND a.record_match >= $limitCnt 
						AND b.player_state = '1' $whereStr 
				ORDER BY $sortStr DESC 
				LIMIT 10000
	";
	//printf("$Sql\n");
	$result = $dBase->Query($Sql);

	if(!mysql_num_rows($result))
	{
		printf("$rank_code : NULL\n");
		fputs($fp, $rank_code."|NULL\n");
		return -3;
	}
	else
	{
		while($rows=mysql_fetch_row($result))
		{
			$cnt++;
			if(!$rows[1]) $rows[1] = 0;
			$Sql = "
						INSERT INTO $rank_tname(rank_num, rank_code, player_seq, rank_point) 
							VALUES($cnt, $rank_code, $rows[0], $rows[1])
			";
			if(!$dBase->Query($Sql))
			{
				$failFlag = 1;
			}
		}
		$time_end = getmicrotime();
		$time = $time_end - $time_start;
		//printf("$rank_code : ".sprintf("%2.2f", $time)." second\n");
		printf("$rank_code\n");
		fputs($fp, $rank_code."|".sprintf("%2.2f", $time)."\n");
	}
}

//$rcode = array("101","102","103","104","105","106","107","108","109","110","111","112","201","202","203","204","205","206","207","208","301","302","303","304","305");
$rcode = array("101","102","103","104","105","106","107","108","110");

foreach($rcode as $key => $mem)
{
	MakeRank($mem);
	MakeRank($mem,'month');
}

fclose($fp);
$dBase->Close();
?>