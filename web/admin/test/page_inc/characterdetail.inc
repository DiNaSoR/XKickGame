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
							SET player_name='".$_POST["player_name"]."', player_state='".$_POST["player_state"]."',player_level = ".$_POST["player_level"].",player_exp = ".$_POST["player_exp"].",player_gender ='".$_POST["player_gender"]."',player_skin = ".$_POST["player_skin"].",player_hair = ".$_POST["player_hair"].",player_shirts = ".$_POST["player_shirts"].",player_pants = ".$_POST["player_pants"].",player_shoes = ".$_POST["player_shoes"].",player_position = ".$_POST["player_position"].", player_skill= ".$_POST["player_skill"].", player_faculty= ".$_POST["player_faculty"].", player_power='".$_POST["player_power"]."', player_ment= '".$_POST["player_ment"]."'
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
<form name="fm" method="post" action="" target="_self" onsubmit= "return CheckFaculty();">
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
					<td align="center"><input type="text" name="player_state"		value="<?=$hChrRow['player_state']?>"	size="1" maxlength="1" style="font-size:9pt"></td>
					<td align="center"><input type="text" id="player_level" name="player_level"		value="<?=$hChrRow['player_level']?>"	size="2" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_exp"	id="player_exp"		value="<?=$hChrRow['player_exp']?>"		size="9" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_gender"		value="<?=$hChrRow['player_gender']?>"	size="2" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_skin"		value="<?=$hChrRow['player_skin']?>"	size="3" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_hair"		value="<?=$hChrRow['player_hair']?>"	size="9" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_shirts"		value="<?=$hChrRow['player_shirts']?>"	size="9" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_pants"		value="<?=$hChrRow['player_pants']?>"	size="9" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" name="player_shoes"		value="<?=$hChrRow['player_shoes']?>"	size="9" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="player_position"  name="player_position"	value="<?=$hChrRow['player_position']?>" size="1" maxlength="10" style="font-size:9pt;"></td>
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
					<td align="center"><input type="text" id="faculty_run" name="faculty_run" value="<?=$hChrRow['faculty_run']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_stamina" name="faculty_stamina" value="<?=$hChrRow['faculty_stamina']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_quick" name="faculty_quick" value="<?=$hChrRow['faculty_quick']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_keeping" name="faculty_keeping" value="<?=$hChrRow['faculty_keeping']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_dribble" name="faculty_dribble" value="<?=$hChrRow['faculty_dribble']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_steal" name="faculty_steal" value="<?=$hChrRow['faculty_steal']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_tackle" name="faculty_tackle" value="<?=$hChrRow['faculty_tackle']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_headshoot" name="faculty_headshoot" value="<?=$hChrRow['faculty_headshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_shortshoot" name="faculty_shortshoot" value="<?=$hChrRow['faculty_shortshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
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
					<td align="center"><input type="text" id="faculty_longshoot" name="faculty_longshoot" value="<?=$hChrRow['faculty_longshoot']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_elasticity" name="faculty_elasticity" value="<?=$hChrRow['faculty_elasticity']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_cross" name="faculty_cross" value="<?=$hChrRow['faculty_cross']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_shortpass" name="faculty_shortpass" value="<?=$hChrRow['faculty_shortpass']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_longpass" name="faculty_longpass" value="<?=$hChrRow['faculty_longpass']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_bodycheck" name="faculty_bodycheck" value="<?=$hChrRow['faculty_bodycheck']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_catch" name="faculty_catch" value="<?=$hChrRow['faculty_catch']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_punch" name="faculty_punch" value="<?=$hChrRow['faculty_punch']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="faculty_defense" name="faculty_defense" value="<?=$hChrRow['faculty_defense']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
				</tr>
				<tr bgcolor="#282828">
					<td align="center"><?=IDS_PLAYER_SKILL?></td>
					<td align="center"><?=IDS_PLAYER_FACULTY?></td>
					<td align="center">Operation</td>
					<td align="center" colspan="6"><?=IDS_PLAYER_MENT?></td>
				</tr>
				<tr bgcolor="#353535">
					<td align="center"><input type="text" name="player_skill" value="<?=$hChrRow['player_skill']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center"><input type="text" id="player_faculty" name="player_faculty" value="<?=$hChrRow['player_faculty']?>" size="5" maxlength="10" style="font-size:9pt;"></td>
					<td align="center">
					<SELECT NAME="player_power" name= id="player_power" >
						<OPTION VALUE="PLAYER_POWER_NORMAL" <? if( $hChrRow['player_power'] =="PLAYER_POWER_NORMAL" ) echo "SELECTED"; ?> >NORMAL
						<OPTION VALUE="PLAYER_POWER_MANAGER" <? if( $hChrRow['player_power'] =="PLAYER_POWER_MANAGER" ) echo "SELECTED"; ?>>MANAGER
					</SELECT> 
					<td align="center" colspan="6"><input type="text" name="player_ment" value="<?=$hChrRow['player_ment']?>" size="80" maxlength="40" style="font-size:9pt;"></td>
				</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td align="left"><?=IDS_PLAYER_LEVEL?> : <INPUT ID="input_level" TYPE="text" SIZE="3" NAME=""> <?=IDS_PLAYER_POSITION?> : <SELECT NAME="" id="input_position" >
				<OPTION VALUE="50" SELECTED>NB
				<OPTION VALUE="10">FW
				<OPTION VALUE="11">ST
				<OPTION VALUE="12">CF
				<OPTION VALUE="13">WF
				<OPTION VALUE="14">SS
				<OPTION VALUE="20">MF
				<OPTION VALUE="21">AM
				<OPTION VALUE="22">CM
				<OPTION VALUE="23">SM
				<OPTION VALUE="24">DM
				<OPTION VALUE="30">DF
				<OPTION VALUE="31">SW
				<OPTION VALUE="32">CB
				<OPTION VALUE="33">SP
				<OPTION VALUE="40">GK
			</SELECT> <input type="button" value="Apply" onclick="CalcStat();">
</td>

		</tr>
		<tr>
			<td align="center"><input TYPE="image" src="<?=CONFIG_DOC_ROOT?>image/B_big_edit.gif" border="0"></td>
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
<SCRIPT LANGUAGE="JavaScript">
<!--
	var arrLevelData = new Array();
	arrLevelData[1]= 0;
	arrLevelData[2]= 400;
	arrLevelData[3]= 800;
	arrLevelData[4]= 1200;
	arrLevelData[5]= 1600;
	arrLevelData[6]= 2000;
	arrLevelData[7]= 2200;
	arrLevelData[8]= 3064;
	arrLevelData[9]= 4792;
	arrLevelData[10]= 6952;
	arrLevelData[11]= 11272;
	arrLevelData[12]= 15678;
	arrLevelData[13]= 20261;
	arrLevelData[14]= 25119;
	arrLevelData[15]= 30365;
	arrLevelData[16]= 36136;
	arrLevelData[17]= 42657;
	arrLevelData[18]= 50221;
	arrLevelData[19]= 59223;
	arrLevelData[20]= 70115;
	arrLevelData[21]= 83730;
	arrLevelData[22]= 97617;
	arrLevelData[23]= 111782;
	arrLevelData[24]= 126230;
	arrLevelData[25]= 140967;
	arrLevelData[26]= 155999;
	arrLevelData[27]= 174038;
	arrLevelData[28]= 195684;
	arrLevelData[29]= 221659;
	arrLevelData[30]= 252830;
	arrLevelData[31]= 290235;
	arrLevelData[32]= 328388;
	arrLevelData[33]= 367304;
	arrLevelData[34]= 406998;
	arrLevelData[35]= 447486;
	arrLevelData[36]= 488784;
	arrLevelData[37]= 534212;
	arrLevelData[38]= 584182;
	arrLevelData[39]= 639149;
	arrLevelData[40]= 699613;
	arrLevelData[41]= 766123;
	arrLevelData[42]= 833299;
	arrLevelData[43]= 901146;
	arrLevelData[44]= 969672;
	arrLevelData[45]= 1038883;
	arrLevelData[46]= 1108786;
	arrLevelData[47]= 1192670;
	arrLevelData[48]= 1293330;
	arrLevelData[49]= 1414123;
	arrLevelData[50]= 1559074;

	function CalcStat()
	{
		var nLevel = document.getElementById( "input_level" ).value;
		document.getElementById( "player_faculty" ).value = (nLevel*2)+Math.floor(nLevel/10) ;

		document.getElementById( "faculty_run" ).value = 0;
		document.getElementById( "faculty_stamina" ).value = 0;
		document.getElementById( "faculty_quick" ).value = 0;
		document.getElementById( "faculty_keeping" ).value = 0;
		document.getElementById( "faculty_dribble" ).value = 0;
		document.getElementById( "faculty_steal" ).value = 0;
		document.getElementById( "faculty_tackle" ).value = 0;
		document.getElementById( "faculty_headshoot" ).value = 0;
		document.getElementById( "faculty_shortshoot" ).value = 0;
		document.getElementById( "faculty_longshoot" ).value = 0;
		document.getElementById( "faculty_elasticity" ).value = 0;
		document.getElementById( "faculty_cross" ).value = 0;
		document.getElementById( "faculty_shortpass" ).value = 0;
		document.getElementById( "faculty_longpass" ).value = 0;
		document.getElementById( "faculty_bodycheck" ).value = 0;
		document.getElementById( "faculty_catch" ).value = 0;
		document.getElementById( "faculty_punch" ).value = 0;
		document.getElementById( "faculty_defense" ).value = 0;

		document.getElementById( "player_position" ).value = document.getElementById( "input_position" ).value;
		document.getElementById( "player_level" ).value = document.getElementById( "input_level" ).value;

		document.getElementById( "player_exp" ).value = arrLevelData[document.getElementById( "input_level" ).value];

	}

	function CheckFaculty()
	{
		var nPosition	= document.getElementById( "player_position" ).value;
		var nLevel		= document.getElementById( "player_level" ).value;

		if(  nLevel > 4 && nPosition == 50 )  
		{
			alert( "You have entered invalid [position] data. Please try again." );
			return false;
		} else if(  nLevel < 5 && nPosition != 50 )  
		{
			alert( "You have entered invalid [position] data. Please try again." );
			return false;
		} else if(  nLevel > 19 && nPosition%10 == 0 )  
		{
			alert( "You have entered invalid [position] data. Please try again." );
			return false;
		} else if(  nLevel < 20 && nPosition%10 != 0 )  
		{
			alert( "You have entered invalid [position] data. Please try again." );
			return false;
		}

		return true;
	}

	document.getElementById( "input_level" ).value = 	document.getElementById( "player_level" ).value;


	for(i=0;i<document.getElementById( "input_position" ).length;i++)
	{
		var nInput    = document.getElementById( "input_position" )[i].value;
		var nPosition = document.getElementById( "player_position" ).value;
		if( nInput == nPosition ) 
		{
			document.getElementById( "input_position" ).options[i].selected=true;
			break;
		}
	}


//-->
</SCRIPT>
</body>
</html>

