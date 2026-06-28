<?
require_once( CONFIG_CLASS_PATH."config.inc" );
require_once( CONFIG_CLASS_PATH."database.inc" );
require_once( CONFIG_CLASS_PATH."htmlform.inc" );
require_once( CONFIG_CLASS_PATH."pager.inc" );

$period = $_REQUEST['period'];
$pos_code = $_REQUEST['pos_code'];
$rank_code = $_REQUEST['rank_code'];

if(!$rank_code) $rank_code = 104;

if(!$period) {
	$tname = "tb_game_rank";
} else {
	$tname = "tb_game_rank_".$period;
}

// 페이지 세팅
$hLayout= new CHtmlForm;
$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, "xkick", true );
$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=menu_join_by_rank&period=".$period."&pos_code=".$pos_code."&rank_code=".$rank_code."&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
$hPager->SetQueryFormArray( array( array(  "b.player_name", PAGER_SEARCH_LEFT, IDS_MEMBER_CHR_NAME ), array(  "b.player_seq", PAGER_SEARCH_LEFT, IDS_MEMBER_CHR_SEQ ) ) );

// 검색 세팅
$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );
if( $strWhere != "" )	$strWhere = " AND ".$strWhere;
$strFrom = $tname." a, tb_game_player b WHERE a.player_seq = b.player_seq AND b.player_state = '1' AND a.rank_code = ".$rank_code;
$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			

$sql = "
		SELECT a.rank_num, a.rank_code, b.player_name, a.rank_point, b.player_level, b.player_seq 
			FROM $tname a, tb_game_player b 
				WHERE a.player_seq = b.player_seq 
				AND b.player_state = '1' 
				AND a.rank_code = $rank_code 
";
if($pos_code) $sql .= " AND b.player_position = ".$pos_code;
$sql .= $strWhere;
$sql .= "
		ORDER BY a.rank_num ASC 
		LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit();
$result = mysql_query($sql, $dBase->DB);

/*$rcode = array("101","102","103","104","105","106","107","108","109","110","111","112","201","202","203","204","205","206","207","208","301","302","303","304","305");
$rname = array("경기수","승리수","승점","종합점수","MVP","득점","어시스트","커팅","유효슈팅","유효스틸","유효태클","유효패스","평점","평균득점","평균어시스트","평균커팅",
						"평균슈팅","평균스틸","평균태클","평균패스","승률","슈팅성공률","스틸성공률","태클성공률","패스성공률");*/
$rcode = array("101","102","103","104","105","106","107","108","110");
$rname = array(IDS_PLAYER_MATCHCNT, IDS_PLAYER_WINCNT, IDS_PLAYER_WINPOINT, IDS_PLAYER_MARKPOINT, IDS_PLAYER_MVP, IDS_PLAYER_GOALPOINT, IDS_PLAYER_ASSIST, IDS_PLAYER_CUT, IDS_PLAYER_STEAL);

$Sql = "
			SELECT position_code, position_initial FROM tb_cod_position WHERE position_code >= 10 ORDER BY position_code ASC 
";
$result2 = $dBase->Query($Sql);

$hLayout->TITLE(IDS_MENU_JOIN_BY_RANK);
?>

<form name="fm" method="POST" action="<?=$PHP_SELF?>" target="_self">
<input type="hidden" name="ret" value="Yes">
<input type="hidden" name="mode" value="">
  <table width="100%"  border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td height="30">
        <table width="765" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="27">&nbsp;</td>
            <td width="100">&nbsp;</td>
            <td width="638" align="right">
			<select name="period" style="font-size:12">
			<option value=''><?=IDS_TOTAL?></option>
			<?
			$mon = DATE("m");
			if($mon >= 1 && $mon <= 3) $part = 1;
			else if($mon >= 4 && $mon <= 6) $part = 2;
			else if($mon >= 7 && $mon <= 9) $part = 3;
			else $part = 4;

			$qStr[1] = IDS_LABEL_FIRSTQUARTER;
			$qStr[2] = IDS_LABEL_SECONDQUARTER;
			$qStr[3] = IDS_LABEL_THIRDQUARTER;
			$qStr[4] = IDS_LABEL_FOURTHQUARTER;

			for($i=2010; $i<=date("Y"); $i++) {
				// 2010년은 2/4분기부터 표시
				if($i == 2010) $s = 2;
				else $s = 1;

				if($i == date("Y")) $e = $part;
				else $e = 4;

				for($j=$s; $j<=$e; $j++) {
					$v = substr($i, 2) . "_" . sprintf("%02d",$j);
					if($period == $v) $sel = " selected";
					else $sel = "";
					echo "<option value='".$v."'".$sel.">".$i." - ".$qStr[$j]."</option>";
				}
			}
			?>
			</select> / 
			<select name="pos_code" style="font-size:12">
			<option value=""><?=IDS_TOTAL?></option>
			<?
			while($rows = @mysql_fetch_array($result2)) 
			{
				if($rows[0] % 10 == 0) continue;
				if($pos_code == $rows[0]) $selStr = " selected";
				else $selStr = "";
				echo "<option value='".$rows[0]."'".$selStr.">".$rows[1]."</option>".chr(13);
			}
			?>
			</select> / 
			<select name="rank_code" style="font-size:12">
			<?
			foreach($rcode as $key => $mem)
			{
				if(!$rank_code && $mem == 104) $sel = " selected";
				else if($rank_code == $mem) $sel = " selected";
				else $sel = "";
				echo "<option value='".$mem."'".$sel.">".$rname[$key]."</option>";
			}
			?>
			</select>
			<input type='image' src='<?=CONFIG_DOC_ROOT?>image/b_search.gif' width='50' height='24' border='0' align="absmiddle">
			</td>
          </tr>
      </table>	  </td>
    </tr>
    <tr>
      <td valign="top">
        <table width="765" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="26">&nbsp;</td>
            <td width="739">
<!-- 가맹점현황리스트 시작 -->			
              <table width="739" border="0" cellpadding="0" cellspacing="1" bgcolor="#565656">
                <tr bgcolor="#282828">
                  <td width="139" height="24" align="center"><span class="style2">&nbsp;</span></td>
				  <td width="100" align="center"><span class="style2"><?=IDS_MEMBER_CHR_SEQ?></span></td>
                  <td width="300" align="center"><span class="style2"><?=IDS_MEMBER_CHR_NAME?></span></td>
				  <td width="200" align="center"><span class="style2"><?=IDS_MEMBER_POINT?></span></td>
                </tr>
			<?
			if(!mysql_num_rows($result))
			{
				?>
				<tr bgcolor="#353535">
				  <td height="100" align="center" colspan='3'><?=IDS_NO_DATA?></td>
				</tr>
				<?
			}
			else
			{
				if(!$_REQUEST['page']) {
					$page = 1;
				} else {
					$page = $_REQUEST['page'];
				}
				$no = ($page-1)*20+1;
				while($rows = mysql_fetch_row($result))
				{
					?>
                <tr bgcolor="#353535">
                  <td height="20" align="center"><?=$rows[0]?> </td>
				  <td align="center"><?=$rows[5]?></td>
                  <td align="center"><?=$rows[2]?> (Lv.<?=$rows[4]?>)</td>
				  <td align="center"><?=$rows[3]?></td>
                </tr>
				<?
					$no++;
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
      </table>
	  </td>
    </tr>
</form>
  </table>
    <?
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=menu_join_by_rank", CONFIG_DOC_ROOT."image/");
	?>
<?
$dBase->Close();
?>