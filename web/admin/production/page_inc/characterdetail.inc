<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	define( 'EDIT_MODE', '1' );

	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

	if( $_POST["mode"] == EDIT_MODE )
	{
		$strQuery = "
						UPDATE tb_game_player
							SET player_name='".$_POST["player_name"]."', player_state='".$_POST["player_state"]."',player_level = ".$_POST["player_level"].",player_exp = ".$_POST["player_exp"].",player_gender ='".$_POST["player_gender"]."',player_skin = ".$_POST["player_skin"].",player_face = ".$_POST["player_face"].",player_shirts = ".$_POST["player_shirts"].",player_pants = ".$_POST["player_pants"].",player_shoes = ".$_POST["player_shoes"].",player_position = ".$_POST["player_position"].", player_skill= ".$_POST["player_skill"].", player_faculty= ".$_POST["player_faculty"].", player_ment= '".$_POST["player_ment"]."'
								WHERE player_seq=".$_GET["seq"];
		$Result = $dBase->Query($strQuery);			
		$strQuery = "
						UPDATE tb_game_faculty
							SET faculty_run=".$_POST["faculty_run"].",faculty_stamina=".$_POST["faculty_stamina"].",faculty_quick=".$_POST["faculty_quick"].",faculty_keeping=".$_POST["faculty_keeping"].",faculty_dribble=".$_POST["faculty_dribble"].",faculty_steal=".$_POST["faculty_steal"].",faculty_tackle=".$_POST["faculty_tackle"].",faculty_headshoot=".$_POST["faculty_headshoot"].",faculty_shortshoot=".$_POST["faculty_shortshoot"].", faculty_longshoot=".$_POST["faculty_longshoot"].",faculty_elasticity=".$_POST["faculty_elasticity"].",faculty_cross=".$_POST["faculty_cross"].",faculty_shortpass=".$_POST["faculty_shortpass"].",faculty_longpass=".$_POST["faculty_longpass"].",faculty_bodycheck=".$_POST["faculty_bodycheck"].",faculty_catch=".$_POST["faculty_catch"].",faculty_punch=".$_POST["faculty_punch"].",faculty_defense=".$_POST["faculty_defense"]."
								WHERE player_seq=".$_GET["seq"];
		$Result = $dBase->Query($strQuery);			
		$hLayout->ReplacePage();
		return;
	}


	$strQuery = "
						SELECT * 
							FROM tb_game_player as A INNER JOIN tb_game_faculty as B on A.player_seq=B.player_seq 
								WHERE A.player_seq=".$_GET["seq"]."
					";
	$Result = $dBase->Query($strQuery);			
	$hChrRow = mysql_fetch_array($Result, MYSQL_ASSOC );
	
	// 출력
	$hLayout->TITLE(IDS_CHARACTER_DETAIL_TITLE);

?>
<form name="fm" method="post" action="" target="_self">
<INPUT TYPE="hidden" NAME="mode" VALUE="1">
<INPUT type="hidden" name="seq" value="<?=$seq?>">
<table>
<tr>
<td width="25"></td>
<td>
<?
	include "./page_inc/chracter_menu.inc";
?>
<table width="95%" border=0 cellpadding=1 cellspacing=1 bgcolor="#646464">
<tr bgcolor="#353535">
	<td>
		<table width="750" border=0 cellpadding=3 cellspacing=1 >
		<tr align="left" bgcolor="#EEEEEE">
			<td bgcolor="#353535">
				<table width="100%" border=0 cellpadding=3 cellspacing=1 bgcolor="#565656" align="left">
				<tr>
					<td align="left" colspan="30">
					<?=IDS_MEMBER_CHR_NAME?> : &nbsp;<input type="text" name="player_name" value="<?=$hChrRow["player_name"]?>" size="20" maxlength="20"> 
					&nbsp;&nbsp;&nbsp;<font color="#FFFFFF"><?=IDS_LABEL_CREATEDATE?>:<?=$hChrRow['player_createdate']?></font><br>
					</td>
				</tr>
				<tr bgcolor="#282828">
					<td align="center"><span class="style2"><?=IDS_PLAYER_STATE?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_LEVEL?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_EXP?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_GENDER?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_SKIN?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_HEAD?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_SHIRTS?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_PANTS?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_SHOES?></span></td>
					<td align="center"><span class="style2"><?=IDS_PLAYER_POSITION?></span></td>
				</tr>
				<tr bgcolor="#353535">
				</tr>
				</table>
			</td>
		</tr>
		<tr align="left" bgcolor="#EEEEEE">
			<td bgcolor="#353535">
				<table width="100%" border="0" cellpadding="3" cellspacing="1" bgcolor="#565656" align="left">
				<tr bgcolor="#282828">
					<td align="center"><?=IDS_PLAYER_RUN?></td>
					<td align="center"><?=IDS_PLAYER_STAMINA?></td>
					<td align="center"><?=IDS_PLAYER_QUICK?></td>
					<td align="center"><?=IDS_PLAYER_BALLCONT?></td>
					<td align="center"><?=IDS_PLAYER_DRIBBLE?></td>
					<td align="center"><?=IDS_PLAYER_STEAL?></td>
					<td align="center"><?=IDS_PLAYER_TACKLE?></td>
					<td align="center"><?=IDS_PLAYER_HEADING?></td>
					<td align="center"><?=IDS_PLAYER_SHORTSHOTS?></td>
				</tr>
				<tr bgcolor="#353535">
					<td align="center"><input type="text" name="faculty_run" value="<?=$hChrRow['faculty_run']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_stamina" value="<?=$hChrRow['faculty_stamina']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_quick" value="<?=$hChrRow['faculty_quick']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_keeping" value="<?=$hChrRow['faculty_keeping']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_dribble" value="<?=$hChrRow['faculty_dribble']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_steal" value="<?=$hChrRow['faculty_steal']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_tackle" value="<?=$hChrRow['faculty_tackle']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_headshoot" value="<?=$hChrRow['faculty_headshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_shortshoot" value="<?=$hChrRow['faculty_shortshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
				</tr>
				<tr bgcolor="#282828">
					<td align="center"><?=IDS_PLAYER_LONGSHOTS?></td>
					<td align="center"><?=IDS_PLAYER_ELASTIC?></td>
					<td align="center"><?=IDS_PLAYER_CROSS?></td>
					<td align="center"><?=IDS_PLAYER_SHORTPASS?></td>
					<td align="center"><?=IDS_PLAYER_LONGPASS?></td>
					<td align="center"><?=IDS_PLAYER_MARK?></td>
					<td align="center"><?=IDS_PLAYER_CATCH?></td>
					<td align="center"><?=IDS_PLAYER_PUNCH?></td>
					<td align="center"><?=IDS_PLAYER_DEFEND?></td>
				</tr>
				<tr bgcolor="#353535">
					<td align="center"><input type="text" name="faculty_longshoot" value="<?=$hChrRow['faculty_longshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_elasticity" value="<?=$hChrRow['faculty_elasticity']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_cross" value="<?=$hChrRow['faculty_cross']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_shortpass" value="<?=$hChrRow['faculty_shortpass']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_longpass" value="<?=$hChrRow['faculty_longpass']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_bodycheck" value="<?=$hChrRow['faculty_bodycheck']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_catch" value="<?=$hChrRow['faculty_catch']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_punch" value="<?=$hChrRow['faculty_punch']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="faculty_defense" value="<?=$hChrRow['faculty_defense']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
				</tr>
				<tr bgcolor="#282828">
					<td align="center"><?=IDS_PLAYER_SKILL?></td>
					<td align="center"><?=IDS_PLAYER_FACULTY?></td>
					<td align="center" colspan="7"><?=IDS_PLAYER_MENT?></td>
				</tr>
				<tr bgcolor="#353535">
					<td align="center"><input type="text" name="player_skill" value="<?=$hChrRow['player_skill']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_faculty" value="<?=$hChrRow['player_faculty']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center" colspan="7"><input type="text" name="player_ment" value="<?=$hChrRow['player_ment']?>" size="80" maxlength="40" style="font-size:9pt;"></td>
				</tr>
				</table>
			</td>
		</tr>
	
		<tr>
			<td align="center"><input TYPE="image" src="<?=CONFIG_DOC_ROOT?>image/B_big_edit.gif" border="0"></a></td>
		</tr>
		<tr>
			<td height="6"></td>
		</tr>
		</table>
	</td>
</tr>
</table>
</td></tr></table>
<iframe name="hidden_frame" width=0 height=0></iframe>
</form>
</body>
</html>

