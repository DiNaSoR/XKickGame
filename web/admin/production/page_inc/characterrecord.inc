<?
//##############################################################################
// 캐릭터 기록 설정 페이지
//##############################################################################
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	define( 'EDIT_MODE', '1' );
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

	if( $_POST["mode"] == EDIT_MODE )
	{
		$strQuery = "
							UPDATE tb_game_record".$_POST['table']."
								SET record_match=".$_POST['record_match'].", record_win=".$_POST['record_win'].", record_draw=".$_POST['record_draw'].", record_mvp=".$_POST['record_mvp'].", record_goal=".$_POST['record_goal'].", record_assist=".$_POST['record_assist'].", record_cut=".$_POST['record_cut'].", record_shoot=".$_POST['record_shoot'].", record_steal=".$_POST['record_steal'].", record_tackle=".$_POST['record_tackle'].", record_pass=".$_POST['record_pass'].", record_tryshoot =".$_POST['record_tryshoot'].", record_trysteal =".$_POST['record_trysteal'].", record_trytackle=".$_POST['record_trytackle'].", record_trypass=".$_POST['record_trypass'].", record_mark=".$_POST['record_mark']."
									WHERE player_seq = ".$_GET["seq"]."
						";
		$Result = $dBase->Query($strQuery);					
		$hLayout->ReplacePage();
		return;
	}
	$strQuery = "
				SELECT record_match, record_win, record_draw, record_mvp, record_goal, record_assist, record_cut, record_shoot, record_steal, 
							record_tackle, record_pass, record_tryshoot, record_trysteal, record_trytackle, record_trypass, record_mark 
					FROM tb_game_record 
						WHERE player_seq = ".$_GET["seq"];
	$Result = $dBase->Query($strQuery);			
	$hLayout->TITLE(IDS_CHARACTER_RECORD_TITLE);
//##############################################################################
?>

<INPUT TYPE="hidden" NAME="mode" VALUE="1">
<INPUT type="hidden" name="seq" value="<?=$seq?>">
<table>
<tr>
<td width="25"></td>
<td>
<?
	$nMenu = 1;
	include "./page_inc/chracter_menu.inc";
?>
<table width="95%" border=0 cellpadding=1 cellspacing=1 align="center" bgcolor="#646464">
<tr bgcolor="#353535">
	<td>
		<table width="750" border=0 cellpadding=3 cellspacing=1 align="center">
	<?

	if(!mysql_num_rows($Result))
	{
		$recordFlag = "N";
		?>
		<tr align="left" bgcolor="#EEEEEE">
		<td bgcolor="#353535">
			<table width="750" border=0 cellpadding=3 cellspacing=1 bgcolor="#565656" align="center">
			<tr>
				<td align="center" colspan="30"><?=IDS_NO_RECORD?></td>
			</tr>
			</table>
		</td>
		</tr>
		<?
	}
	else
	{
		$rows = mysql_fetch_array($Result);
		?>
		<form name="FORM_RECORD" method="post" target="_self">
		<INPUT TYPE="hidden" NAME="mode" VALUE="1">
		<INPUT TYPE="hidden" NAME="table" VALUE="">
		<tr align="left" bgcolor="#EEEEEE">
			<td bgcolor="#353535">
				<table width="750" border=0 cellpadding=3 cellspacing=1 bgcolor="#565656" align="center">
				<tr>
					<td align="center" colspan="30"><?=IDS_CHARACTER_RECORD_TITLE?> (TOTAL)</td>
				</tr>
				<tr bgcolor="#282828">
					<td align="center">match</td>
					<td align="center">win</td>
					<td align="center">draw</td>
					<td align="center">mvp</td>
					<td align="center">goal</td>
					<td align="center">assist</td>
					<td align="center">cut</td>
					<td align="center">shoot</td>
					<td align="center">steal</td>
					<td align="center">tackle</td>
					<td align="center">pass</td>
					<td align="center">tryshoot</td>
					<td align="center">trysteal</td>
					<td align="center">trytackle</td>
					<td align="center">trypass</td>
					<td align="center">mark</td>
				</tr>
				<tr bgcolor="#353535">
					<td align="center"><input type="text" name="record_match" value="<?=$rows['record_match']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_win" value="<?=$rows['record_win']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_draw" value="<?=$rows['record_draw']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_mvp" value="<?=$rows['record_mvp']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_goal" value="<?=$rows['record_goal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_assist" value="<?=$rows['record_assist']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_cut" value="<?=$rows['record_cut']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_shoot" value="<?=$rows['record_shoot']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_steal" value="<?=$rows['record_steal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_tackle" value="<?=$rows['record_tackle']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_pass" value="<?=$rows['record_pass']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_tryshoot" value="<?=$rows['record_tryshoot']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_trysteal" value="<?=$rows['record_trysteal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_trytackle" value="<?=$rows['record_trytackle']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_trypass" value="<?=$rows['record_trypass']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="record_mark" value="<?=$rows['record_mark']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
				</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td align="center"><input TYPE="image" SRC="<?=CONFIG_DOC_ROOT?>/image/B_big_edit.gif" border="0"></a></td>
		</tr>
			</form>
		<?
	}
		$nYear   = (int)DATE("y");
		$nMonth  = (int)DATE("m");
		$strQuery = "
						SHOW TABLES LIKE 'tb_game_record_%'
						";
		$hTableResult = $dBase->Query($strQuery);			
		while( $hTableRows = mysql_fetch_array($hTableResult) )
		{
			$strTableName = $hTableRows[0];
			$strTableParam= str_replace( "tb_game_record", "", $strTableName );
			$strQuery = "
						SELECT record_match, record_win, record_draw, record_mvp, record_goal, record_assist, record_cut, record_shoot, record_steal, 
									record_tackle, record_pass, record_tryshoot, record_trysteal, record_trytackle, record_trypass, record_mark 
							FROM $strTableName 
								WHERE player_seq =".$_GET["seq"]."
			";
			$Result = $dBase->Query($strQuery);
			if(mysql_num_rows($Result))
			{
				$rows = mysql_fetch_array($Result);
			?>
			<tr>
				<td height="6"></td>
			</tr>
			<tr align="left" bgcolor="#EEEEEE">
				<td bgcolor="#353535">
					<table width="750" border=0 cellpadding=3 cellspacing=1 bgcolor="#565656" align="center">
					<tr>
						<td align="center" colspan="30"><?=IDS_CHARACTER_RECORD_TITLE?> (<?=$strTableName?>)</td>
					</tr>
					<FORM name="FORM_RECORD_PART" method="post" target="_self">
					<INPUT TYPE="hidden" NAME="mode" VALUE="1">
					<INPUT TYPE="hidden" NAME="table" VALUE="<?=$strTableParam?>">
					<tr bgcolor="#282828">
						<td align="center">match</td>
						<td align="center">win</td>
						<td align="center">draw</td>
						<td align="center">mvp</td>
						<td align="center">goal</td>
						<td align="center">assist</td>
						<td align="center">cut</td>
						<td align="center">shoot</td>
						<td align="center">steal</td>
						<td align="center">tackle</td>
						<td align="center">pass</td>
						<td align="center">tryshoot</td>
						<td align="center">trysteal</td>
						<td align="center">trytackle</td>
						<td align="center">trypass</td>
						<td align="center">mark</td>
					</tr>
					<tr bgcolor="#353535">
						<td align="center"><input type="text" name="record_match" value="<?=$rows['record_match']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_win" value="<?=$rows['record_win']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_draw" value="<?=$rows['record_draw']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_mvp" value="<?=$rows['record_mvp']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_goal" value="<?=$rows['record_goal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_assist" value="<?=$rows['record_assist']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_cut" value="<?=$rows['record_cut']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_shoot" value="<?=$rows['record_shoot']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_steal" value="<?=$rows['record_steal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_tackle" value="<?=$rows['record_tackle']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_pass" value="<?=$rows['record_pass']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_tryshoot" value="<?=$rows['record_tryshoot']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_trysteal" value="<?=$rows['record_trysteal']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_trytackle" value="<?=$rows['record_trytackle']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_trypass" value="<?=$rows['record_trypass']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
						<td align="center"><input type="text" name="record_mark" value="<?=$rows['record_mark']?>" size="4" maxlength="10" style="font-size:9pt;"></td>
					</tr>
					</table>
				</td>
			</tr>
			<tr>
				<td align="center"><INPUT TYPE="image" SRC="<?=CONFIG_DOC_ROOT?>/image/B_big_edit.gif" border="0"></a></td>
			</tr>
			</FORM>
			<?
			}
			if($nMonth == 1)
			{
				$nYear--;
				$nMonth = 12;
			}
			else
			{
				$nMonth--;
			}
		}
	?>
		<tr>
			<td height="20"></td>
		</tr>
		</table>
	</td>
</tr>
</table>
<br><br>
<iframe name="hidden_frame" width=0 height=0></iframe>

</body>
</html>
