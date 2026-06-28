<?
require_once( CONFIG_CLASS_PATH."config.inc" );
require_once( CONFIG_CLASS_PATH."database.inc" );
require_once( CONFIG_CLASS_PATH."htmlform.inc" );

// 페이지 세팅
$hLayout= new CHtmlForm;
$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, "xkick_log", true );

$Year = $_REQUEST['Year'];
$Month = $_REQUEST['Month'];
$Day = $_REQUEST['Day'];

if(!$Year) 	$Year = date('Y');
if(!$Month) $Month = sprintf("%02d",date('m'));
else $Month = sprintf("%02d",$Month);
if(!$Day) $Day = date('d');
else $Day = sprintf("%02d",$Day);

$start_str = $Year . "-" . $Month . "-" . $Day . " 00:00:00";
$end_str = $Year . "-" . $Month . "-" . $Day . " 23:59:59";

$tname = "tb_log_connect_" . sprintf("%02d", date("y")) . "_" . sprintf("%02d", date("m"));

$serv_code = $_REQUEST['serv_code'];
if(!$serv_code)
{
	$Sql = "
				SELECT DATE_FORMAT(connect_date, '%H:%i:%s') AS DAT, SUM(connect_count) 
					FROM $tname 
						WHERE connect_date >= '$start_str' 
						AND connect_date <= '$end_str' 
						AND connect_server > 1 
				GROUP BY DAT 
				ORDER BY DAT ASC 
	";
}
else
{
	$Sql = "
				SELECT DATE_FORMAT(connect_date, '%H:%i:%s') AS DAT, SUM(connect_count) 
					FROM $tname 
						WHERE connect_date >= '$start_str' 
						AND connect_date <= '$end_str' 
						AND connect_server = $serv_code  
				GROUP BY DAT 
				ORDER BY DAT ASC 
	";
}
$result = $dBase->Query($Sql);
$cnt = 0;
while($rows = @mysql_fetch_array($result)) 
{
	$dateInfo[$cnt] = $rows[0];
	$cntInfo[$cnt] = round($rows[1]);
	$cnt++;

	if($rows[1] > $maxCnt) {
		$maxCnt = $rows[1];
	}
}
$maxCnt = round($maxCnt);
$twidth = 334;

$hLayout->TITLE(IDS_MENU_CON_FIVE_MINUTE);
?>

<form name="fm" method="POST" action="<?=$PHP_SELF?>" target="_self">
<input type="hidden" name="ret" value="Yes">
<input type="hidden" name="mode" value="">
  <table width="100%"  border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td height="30">
        <table width="568" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="27">&nbsp;</td>
            <td width="265">&nbsp;</td>
            <td width="601" align="right">
			<select name="serv_code" style="font-size:12">
			<option value=''><?=IDS_LABEL_ALLSERVER?></option>
			<?
			$dBase->Close();
			$dBase->DBSele = "xkick";
			$dBase->Conn();
			$Sql = "
						SELECT server_code, server_name FROM tb_game_server WHERE server_code > 1 ORDER BY server_code ASC 
			";
			$result = $dBase->Query($Sql);
			while($rows = @mysql_fetch_array($result)) 
			{
				if($serv_code == $rows[0]) $selStr = " selected";
				else $selStr = "";
				echo "<option value='".$rows[0]."'".$selStr.">".$rows[1]."</option>".chr(13);
			}
			?>
			</select> &nbsp;&nbsp;
			<select name="Year" style="font-size:12">
				<?
				for($i=2007;$i<=date('Y');$i++) {
					if($Year == $i) echo "<option value=$i selected>$i</option>";
					else echo "<option value=$i>$i</option>";
				}
				?>
				</select> / 
				<select name="Month" style="font-size:12">
				<?
				for($i=1;$i<13;$i++) {
					if($Month == $i) echo "<option value=$i selected>$i</option>";
					else echo "<option value=$i>$i</option>";
				}
				?>
				</select> / 
				<select name="Day" style="font-size:12">
				<?
				for($i=1;$i<32;$i++) {
					if($Day == $i) echo "<option value=$i selected>$i</option>";
					else echo "<option value=$i>$i</option>";
				}
				?>
				</select> 
				<input type="submit" value="<?=IDS_LABEL_SEARCH?>" style="font-size:12">
			</td>
          </tr>
      </table>	  </td>
    </tr>
    <tr>
      <td valign="top">
        <table width="568" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="26">&nbsp;</td>
            <td width="542">
<!-- 가맹점현황리스트 시작 -->			
              <table width="542" border="0" cellpadding="0" cellspacing="1" bgcolor="#565656">
                <tr bgcolor="#282828">
                  <td width="100" height="24" align="center"><span class="style2"><?=IDS_LABEL_TIME?></span></td>
                  <td width="334" align="center"><span class="style2"><?=IDS_LABEL_GRAPH?></span></td>
				  <td width="100" align="center"><span class="style2"><?=IDS_LABEL_USERCOUNT?></span></td>
                </tr>
			<?
			if(!$cnt)
			{
				?>
				<tr bgcolor="#353535">
				  <td height="100" align="center" colspan='3'><?=IDS_NO_DATA?></td>
				</tr>
				<?
			}
			else
			{
				$twidth = $twidth - 10;	//그래프를 td보다 10픽셀 짧게..
				for($i=0; $i<=count($dateInfo)-1; $i++)
				{
					$total = $total + $cntInfo[$i];
					if($maxCnt) {
						$per_day = ($cntInfo[$i]/$maxCnt);
						$per_day = sprintf("%01.2f", $per_day);
						$width = $twidth * $per_day;
					}
					?>
                <tr bgcolor="#353535">
                  <td height="15" align="center"><?=$dateInfo[$i]?> </td>
                  <td align="left">&nbsp;<img src="./image/yellow_bar.gif" width="<?=$width?>" height="5"></td>
				  <td align="right"><?=number_format($cntInfo[$i])?>&nbsp;</td>
                </tr>
				<?
					if($maxCnt < $cntInfo[$i])
					{
						$maxCnt = $cntInfo[$i];
					}
				}
			}
			?>
				<!--<tr bgcolor="#353535">
                  <td height="20" align="center" colspan="2">전체</td>
				  <td align="right"><?=number_format($total)?> 명&nbsp;</td>
                </tr>//-->
				</table>
<!-- 가맹점현황리스트 끝 -->			
			</td>
          </tr>
		  <tr>
			<td>&nbsp;</td>
		  </tr>
		  <?if($cnt) {?>
		  <tr>
			<td width="26">&nbsp;</td>
            <td width="542" height="20" align="left">
			<font color="#FFFFFF"><?=IDS_LABEL_AVGCCU?></font> : <?=number_format(round($total / $i))?>
			</td>
		  </tr>
		  <tr>
			<td width="26">&nbsp;</td>
            <td width="542" height="20" align="left">
			<font color="#FFFFFF"><?=IDS_LABEL_MAXCCU?></font> : <?=number_format(round($maxCnt))?>
			</td>
		  </tr>
		  <?}?>
		  <tr>
			<td width="26">&nbsp;</td>
            <td width="542" height="40" align="left">&nbsp;</td>
		  </tr>
      </table>
	  </td>
    </tr>
  </table>
</form>
<?
$dBase->Close();
?>