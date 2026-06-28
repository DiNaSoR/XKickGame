<?
require_once( CONFIG_CLASS_PATH."htmlform.inc" );
$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

// 금일 해지회원수
$Result = $dBase->Query("SELECT count(*) as tday_del FROM tb_mst_member WHERE member_deletedate >curdate(); ");		
$hRow = mysql_fetch_array($Result, MYSQL_ASSOC );
$nTodayDelete = $hRow["tday_del"];

// 총 회원수
$Result = $dBase->Query("SELECT count(*) as member_cnt FROM tb_mst_member WHERE member_state = '1'; ");		
$hRow = mysql_fetch_array($Result, MYSQL_ASSOC );
$nMemberCnt = $hRow["member_cnt"];


// 가입자 통계 
$Result = $dBase->Query("SELECT DATE_FORMAT( date_value, '%m/%d') as date_value, ifnull(regcnt, 0 ) as rcnt FROM (SELECT DATE_SUB(curdate(), interval 0 day) AS date_value
UNION ALL SELECT DATE_SUB(curdate(), interval 1 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 2 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 3 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 4 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 5 day )
UNION ALL SELECT DATE_SUB(curdate(), interval 6 day )) as datelist LEFT JOIN (SELECT DATE(member_createdate) AS cdate, count(*) as regcnt FROM tb_mst_member WHERE member_createdate>DATE_SUB(curdate(), interval 7 day ) GROUP BY cdate ) AS member
ON datelist.date_value=member.cdate");		
$hMember = Array(); $i = 0;
while( $hRow = mysql_fetch_array($Result, MYSQL_ASSOC ) ){	$hMember[$i] = $hRow;	$i++; }

// 캐릭터 생성 통계 
$Result = $dBase->Query("SELECT DATE_FORMAT( date_value, '%m/%d') as date_value, ifnull(regcnt, 0 ) as rcnt FROM (SELECT DATE_SUB(curdate(), interval 0 day) AS date_value
UNION ALL SELECT DATE_SUB(curdate(), interval 1 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 2 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 3 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 4 day )
UNION ALL SELECT DATE_SUB(curdate(), interval 5 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 6 day )) as datelist LEFT JOIN (SELECT DATE(player_createdate) AS cdate, count(*) as regcnt FROM tb_game_player WHERE player_createdate>DATE_SUB(curdate(), interval 7 day ) GROUP BY cdate ) AS member
ON datelist.date_value=member.cdate");		
$hPlayer = Array(); $i = 0;
while( $hRow = mysql_fetch_array($Result, MYSQL_ASSOC ) ){	$hPlayer[$i] = $hRow;	$i++; }

// 접속자 통계 
$nYear   = DATE("y");
$nMonth  = DATE("m");
if(mysql_query("SELECT COUNT(*) FROM xkick_log.tb_mst_playlog_".$nYear."_".$nMonth, $dBase->DB)) {
	$Result = $dBase->Query("SELECT DATE_FORMAT( date_value, '%m/%d') as date_value, ifnull(regcnt, 0 ) as rcnt FROM (SELECT DATE_SUB(curdate(), interval 0 day) AS date_value UNION ALL SELECT DATE_SUB(curdate(), interval 1 day )
	UNION ALL SELECT DATE_SUB(curdate(), interval 2 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 3 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 4 day ) UNION ALL SELECT DATE_SUB(curdate(), interval 5 day )
	UNION ALL SELECT DATE_SUB(curdate(), interval 6 day )) as datelist inner JOIN ( SELECT cdate, count(*) as regcnt FROM ( SELECT player_seq, DATE(playlog_date_in) AS cdate, count(*) as regcnt 
		   FROM xkick_log.tb_mst_playlog_".$nYear."_".$nMonth." WHERE playlog_date_in>DATE_SUB(curdate(), interval 7 day ) GROUP BY member_seq , cdate ) AS A GROUP BY cdate ) AS member ON datelist.date_value=member.cdate");		
	$hConnector  = Array(); $i = 0;
	while( $hRow = mysql_fetch_array($Result, MYSQL_ASSOC ) ){	$hConnector[$i] = $hRow;	$i++; }
}

// 서버 현황 통계
$Result = $dBase->Query("SELECT cod.server_name as server_name, cod.server_code, ifnull(data.cnt,0) as cnt FROM tb_game_server as cod LEFT JOIN  (SELECT trio_server, count(*) as cnt FROM tb_game_trio as a GROUP BY trio_server HAVING trio_server > 0) as data ON cod.server_code=data.trio_server where cod.server_code <>0"  );
$hServer = Array(); $i = 0;
while( $hRow = mysql_fetch_array($Result, MYSQL_ASSOC ) ){	$hServer[$i] = $hRow;	$i++; }

// 캐릭터 랭킹
$Result = $dBase->Query("SELECT player_name, player_exp  from tb_game_player ORDER BY player_exp DESC LIMIT 5;" );
$hRanking = Array(); $i = 0;
while( $hRow = mysql_fetch_array($Result, MYSQL_ASSOC ) ){	$hRanking[$i] = $hRow;	$i++; }


/*


SELECT player_name, player_exp  from tb_game_player ORDER BY player_exp DESC LIMIT 5;

SELECT b.server_name, trio_server, count(*) FROM tb_game_trio as a INNER JOIN tb_game_server as b ON a.trio_server=server_code GROUP BY trio_server HAVING trio_server > 0;

SELECT DATE(member_createdate) AS cdate, count(*) as regcnt FROM tb_mst_member WHERE member_createdate>DATE_SUB(curdate(), interval 7 day ) GROUP BY cdate ORDER BY cdate;

SELECT DATE(player_createdate) AS cdate, count(*) as regcnt FROM tb_game_player WHERE player_createdate>DATE_SUB(curdate(), interval 7 day ) GROUP BY cdate ORDER BY cdate;

SELECT cdate, count(*) FROM ( SELECT player_seq, DATE(playlog_date_in) AS cdate, count(*) as regcnt 
       FROM xkick_log.tb_mst_playlog_10_06 WHERE playlog_date_in>DATE_SUB(curdate(), interval 7 day ) GROUP BY member_seq , cdate ) AS A GROUP BY cdate ORDER BY cdate;
*/

?>
<style type="text/css"> 
<!--
.style1 {
	color: #C0FF00;
	font-weight: bold;
}
.style3 {color: #FFC600}
-->
</style>
<div style="float:left;">
<div style="padding-left:10px; padding-top:10px">
<table >
	<tr><td >
		<table border="0" cellpadding="0" cellspacing="0" width="100%">
					<tr>
						<td width="260" height="1" bgcolor="black" colspan="2">
						</td>
					</tr>
					<tr>
						<td width="260" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_friends.gif" align="absmiddle" width="15" height="18" border="0"> <span class="style1"><?=IDS_MEMBER_RESISTRATION ?></span> [<?=IDS_TOTAL?>: <?=number_format($nMemberCnt)?> ]</td>
					</tr>
					<tr bgcolor="#4D4D4D">
						<td width="177" height="20">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif" align="absMiddle"> <?=IDS_JOINED_USERS_TODAY ?></td>
						<td width="95" height="20">:&nbsp;<?=$hMember[0]["rcnt"]?> </td>
					</tr>
					<tr bgcolor="#4D4D4D">
						<td width="177" height="20">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif" align="absMiddle">&nbsp;<?=IDS_JOINED_USERS_YESTERDAY ?> </td>
						<td width="95" height="20">:&nbsp;<?=$hMember[1]["rcnt"]?> </td>
					</tr>
					<tr bgcolor="#4D4D4D">
						<td width="177" height="20">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif" align="absMiddle">&nbsp;<?=IDS_RESIGNED_USERS?> &nbsp;&nbsp; </td>
						<td width="95" height="20">:&nbsp;<?=number_format($nTodayDelete);?> </td>
					</tr>
		</table>
		</td></tr>
</table>
</div>

<div style="padding-left:10px; padding-top:0px;display:none">
<table>
	<tr><td >
		<table border="0" cellpadding="0" cellspacing="0" width="100%">
					<tr>
						<td width="260" height="1" bgcolor="black" colspan="2">
						</td>
					</tr>
					<tr>
						<td width="260" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_info_point.gif" align="absmiddle" width="17" height="17" border="0"> <span class="style1"><?=IDS_CHARACTER_RANKING ?></span> </td>
					</tr>
					<?
						$nLen = count($hRanking);
						for( $i = 0; $i < $nLen; $i++ )
						{
							
					?>
					<tr bgcolor="#4D4D4D">
						<td width="177" height="20">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif" align="absMiddle"> <?=$hRanking[$i]["player_name"]?></td>
						<td width="95" height="20">:&nbsp;<?=$hRanking[$i]["player_exp"]?> </td>
					</tr>
					<?
						}
					?>
		</table>
		</td></tr>
</table>
</div>


<div style="padding-left:10px;">
<table>
	<tr><td >
		<table border="0" cellpadding="0" cellspacing="0" width="100%">
					<tr>
						<td width="260" height="1" bgcolor="black" colspan="2">
						</td>
					</tr>
					<tr>
						<td width="260" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_friends.gif" align="absmiddle" width="15" height="18" border="0"> <span class="style1"><?=IDS_SERVER_STATUS?></span> </td>
					</tr>
					<?
						$nLen = count($hServer);
						for( $i = 0; $i < $nLen; $i++ )
						{
							
					?>
					<tr bgcolor="#4D4D4D">
						<td width="177" height="20">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif" align="absMiddle"> <?=$hServer[$i]["server_name"]?></td>
						<td width="95" height="20">:&nbsp;<?=$hServer[$i]["cnt"]?> </td>
					</tr>
					<?
						}
					?>
		</table>
		</td></tr>
</table>
</div>
</div>






<div style="float:left;" >
<div style="padding-left:10px; padding-top:13px; width:300px;">
<table border="0" cellpadding="0" cellspacing="0" width="100%">
							<tr>
								<td height="1" bgcolor="black" colspan="3">
								</td>
							</tr>
							<tr>
								<td width="200" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_money.gif" align="absmiddle" border="0"> <span class="style1"><?=IDS_REGISTERED_USER_STATUS?></span></td>
								<td height="25" bgcolor="#000000" align="right"></td>
							</tr>
							<tr bgcolor="#4D4D4D">
								<td colspan="3">
									<table border="0" cellpadding="1" cellspacing="1" width="100%">
								<?
										$nLen = count($hMember);
										$nAll = 0;
										for( $i = 0; $i < $nLen; $i++ )
										{
											$nAll += $hMember[$i]["rcnt"];
										}

										for( $i = 0; $i < $nLen; $i++ )
										{
									?>
																		   <tr>
											<td width="10">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif"></td>
											<td width="40"><?=$hMember[$i]["date_value"]?></td>
											<td width="299">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/red_bar.gif" width="<?=round($hMember[$i]["rcnt"]/$nAll*100, 1)?>" height="10"> <?=round($hMember[$i]["rcnt"]/$nAll*100, 0)?>%</td>
											<td width="140" align=right>&nbsp;<?=$hMember[$i]["rcnt"]?>&nbsp;</td>
										</tr>
								<?
										}
								?>
																			</table>
		</td>
							</tr>
							</table>


</div>






<div style="padding-left:10px; padding-top:5px; width:300px;">
<table border="0" cellpadding="0" cellspacing="0" width="100%">
							<tr>
								<td height="1" bgcolor="black" colspan="3">
								</td>
							</tr>
							<tr>
								<td width="200" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_money.gif" align="absmiddle" border="0"> <span class="style1"><?=IDS_CHARACTER_CREATION_STATUS?></span></td>
								<td height="25" bgcolor="#000000" align="right"></td>
							</tr>
							<tr bgcolor="#4D4D4D">
								<td colspan="3">
									<table border="0" cellpadding="1" cellspacing="1" width="100%">
								<?
										$nLen = count($hPlayer);
										$nAll = 0;
										for( $i = 0; $i < $nLen; $i++ )
										{
											$nAll += $hPlayer[$i]["rcnt"];
										}
										if( $nAll == 0 ) $nAll = 1;
										for( $i = 0; $i < $nLen; $i++ )
										{
									?>
																		   <tr>
											<td width="10">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif"></td>
											<td width="40"><?=$hPlayer[$i]["date_value"]?></td>
											<td width="299">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/yellow_bar.gif" width="<?=round($hPlayer[$i]["rcnt"]/$nAll*100, 1)?>" height="10"> <?=round($hPlayer[$i]["rcnt"]/$nAll*100, 0)?>%</td>
											<td width="140" align=right>&nbsp;<?=$hPlayer[$i]["rcnt"]?>&nbsp;</td>
										</tr>
								<?
										}
								?>
																			</table>
		</td>
							</tr>
							</table>


</div>




<div style="padding-left:10px;padding-top:5px; width:300px;">
<table border="0" cellpadding="0" cellspacing="0" width="100% ">
							<tr>
								<td height="1" bgcolor="black" colspan="3">
								</td>
							</tr>
							<tr>
								<td width="200" height="25" bgcolor="#000000" colspan="2">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_money.gif" align="absmiddle" border="0"> <span class="style1"><?=IDS_GAME_CONNECTED_USER_STATUS?></span></td>
								<td height="25" bgcolor="#000000" align="right"></td>
							</tr>
							<tr bgcolor="#4D4D4D">
								<td colspan="3">
									<table border="0" cellpadding="1" cellspacing="1" width="100%">
								<?
										$nLen = count($hConnector);
										$nAll = 0;
										for( $i = 0; $i < $nLen; $i++ )
										{
											$nAll += $hConnector[$i]["rcnt"];
										}
										if( $nAll == 0 ) $nAll = 1;
										for( $i = 0; $i < $nLen; $i++ )
										{
									?>
																		   <tr>
											<td width="10">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/ico_dot.gif"></td>
											<td width="40"><?=$hConnector[$i]["date_value"]?></td>
											<td width="299">&nbsp;<img src="<?=CONFIG_DOC_ROOT?>image/blue_bar.gif" width="<?=round($hConnector[$i]["rcnt"]/$nAll*100, 1)?>" height="10"> <?=round($hConnector[$i]["rcnt"]/$nAll*100, 0)?>%</td>
											<td width="140" align=right>&nbsp;<?=$hConnector[$i]["rcnt"]?>&nbsp;</td>
										</tr>
								<?
										}
								?>
																			</table>
		</td>
							</tr>
							</table>


</div>


</div>
<div style="clear:both"></div>