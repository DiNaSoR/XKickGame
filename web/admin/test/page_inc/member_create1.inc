<?
header("Content-Type: text/html; charset=UTF-8"); 

// 급하게 날림코드로 만든 소스 by 머털

?>
<html xmlns="http://www.w3.org/1999/xhtml">
<title>계정관리</title>
<style>
body, td, select, textarea, input{
	font-family:Tahoma;
	font-size: 12px;
	color: #000000;
}

</style>

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" >
<LINK REL=STYLESHEET TYPE="text/css" HREF="index.css">
<head>
<?
include "../class_inc/database.inc";
$dBase = new Database_Class;
$dBase->DBSele = CONFIG_DB_NAME;
$dBase->Conn();
?>
<SCRIPT LANGUAGE="JavaScript">
<!--
	function ERROR( str )
	{
		alert( str );
	}
//-->
</SCRIPT>
<?
function GetPosition( $strPosition)
{
	$strPosition = trim($strPosition);
	if( $strPosition == "BG" ) return 50;
	if( $strPosition == "FW" ) return 10;
	if( $strPosition == "ST" ) return 11;
	if( $strPosition == "CF" ) return 12;
	if( $strPosition == "WF" ) return 13;
	if( $strPosition == "SS" ) return 14;
	if( $strPosition == "MF" ) return 20;
	if( $strPosition == "AM" ) return 21;
	if( $strPosition == "CM" ) return 22;
	if( $strPosition == "SM" ) return 23;
	if( $strPosition == "DM" ) return 24;
	if( $strPosition == "DF" ) return 30;
	if( $strPosition == "SW" ) return 31;
	if( $strPosition == "CB" ) return 32;
	if( $strPosition == "SP" ) return 33;
	if( $strPosition == "GK" ) return 40;

	return 50;
	
}



if( $_GET["MODE"] == "OK" )
{
	$strID = $_GET["id"];
	$strQuery = "
						SELECT count(*) as cnt
							FROM tb_mst_member 
								WHERE member_id='".$strID."'
					";
	$Result = $dBase->Query($strQuery);			
	$rows = mysql_fetch_array($Result, MYSQL_ASSOC );
	
	$nPosition1 = GetPosition( $_GET["position1"] );
	$nPosition2 = GetPosition( $_GET["position2"] );
	$nPosition3 = GetPosition( $_GET["position3"] );

	$strError = "";
	do
	{
		if( $rows["cnt"] > 0 ) { $strError =  "이미 존재하는 계정입니다.";  break; }

		$strQuery = "
							SELECT member_seq
								FROM tb_mst_member 
									ORDER BY member_seq DESC
										LIMIT 1
						";
		$Result = $dBase->Query($strQuery);			
		$rows = mysql_fetch_array($Result, MYSQL_ASSOC );
 
		$nRegNumber = 1000001+$rows["member_seq"];
		$nRegNumber = "111111".$nRegNumber ;


		$strQuery = "
						INSERT INTO tb_mst_member SET 
							member_seq=0, member_id='".$strID."' , member_pass='1234', member_name='테스터', member_register='".$nRegNumber."', member_state='1', member_partner='enterplay', member_ip='', 
								member_speed=0, member_createdate=now(), member_logindate='0000-00-00 00:00:00', member_deletedate='0000-00-00 00:00:00', member_blockdate='0000-00-00 00:00:00', member_releasedate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			

		$nSeq = mysql_insert_id();
		
		$nQuest = 0;
		$nTutorial = 0;
		if( $_GET["TUTORIAL"] == "on" )
		{
		$nQuest = 7;
		$nTutorial = 63;
		}

		$strQuery = "
						INSERT INTO tb_mst_meminfo SET member_register='".$nRegNumber."',meminfo_postcode='111-111',meminfo_addr1='테스트 생성 계정',meminfo_addr2='테스트 생성 계정',meminfo_email='TEST@ENTERPLAY.CO.KR',meminfo_telcode='02',meminfo_telnum1='1234',meminfo_telnum2='1234',meminfo_hpcode='011',meminfo_hpnum1='1234',meminfo_hpnum2='1234',meminfo_memo='운영자 생성 캐릭터',meminfo_mail_agree='N',meminfo_sms_agree='N',meminfo_memodate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			

		$strQuery = "
						INSERT INTO tb_game_trio SET member_seq=".$nSeq.", trio_cash=10000000, trio_point=10000000, trio_credit=1000, trio_allpoint=0, trio_allcredit=0, trio_lastseq=0, trio_server=0, 
								trio_count=3, trio_tutorial=".$nTutorial.", trio_quest=".$nQuest.", trio_logindate=now(), trio_deletedate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			
			
		$nGender = $_GET["SEX1"];
		if( $nGender == 1 )
		{
			$strQuery = "																																																																															 	  
			INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 0, '[".$_GET["name1"]."]".$_GET["id"]."', '1', 11, 101101111, 102101101, 201001101, 202001101, 204001101, now(), ".$_GET["level1"].", ".(($_GET["level1"]*2)+(floor($_GET["level1"]/10))).", $nPosition1, 100 )
						";
		} else
		{
			$strQuery = "																																																																															 	  
			INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 0, '[".$_GET["name1"]."]".$_GET["id"]."', '2', 21, 101101121, 102104101, 201002101, 202002101, 204002101, now(), ".$_GET["level1"].", ".(($_GET["level1"]*2)+(floor($_GET["level1"]/10))).", $nPosition1, 100 )
						";
		}
					
		$Result = $dBase->Query($strQuery);			

		$nPlayerSeq = mysql_insert_id();
		$strQuery = "
		INSERT INTO tb_game_faculty (player_seq) VALUES (".$nPlayerSeq.")
					";
		$Result = $dBase->Query($strQuery);			
		$strQuery = "
		INSERT INTO tb_game_setting (player_seq) VALUES (".$nPlayerSeq.")
					";
		$Result = $dBase->Query($strQuery);			
		if( $_GET["TRAINNING"] == "on" )
		{
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1009,10,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1109,11,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1209,12,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1309,13,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1409,14,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1509,15,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1609,16,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1709,17,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1809,18,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1909,19,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2009,20,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2109,21,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
		}



		if(  $_GET["name2"] != "" )
		{
			$nGender = $_GET["SEX2"];
		if( $nGender == 1 )
		{
			$strQuery = "																																																																															 	  
			INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 1, '[".$_GET["name2"]."]".$_GET["id"]."', '1', 11, 101101111, 102101101, 201001101, 202001101, 204001101, now(), ".$_GET["level2"].", ".(($_GET["level2"]*2)+(floor($_GET["level2"]/10))).", $nPosition2, 100 )
						";
		} else
		{
			$strQuery = "																																																																															 	  
			INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 1, '[".$_GET["name2"]."]".$_GET["id"]."', '2', 21, 101101121, 102104101, 201002101, 202002101, 204002101, now(), ".$_GET["level2"].", ".(($_GET["level2"]*2)+(floor($_GET["level2"]/10))).", $nPosition2, 100 )
						";
		}


			$Result = $dBase->Query($strQuery);			
			$nPlayerSeq = mysql_insert_id();
			$strQuery = "
				INSERT INTO tb_game_faculty (player_seq) VALUES (".$nPlayerSeq.")
					";
			$Result = $dBase->Query($strQuery);			
			$strQuery = "
				INSERT INTO tb_game_setting (player_seq) VALUES (".$nPlayerSeq.")
					";
			$Result = $dBase->Query($strQuery);			


		if( $_GET["TRAINNING"] == "on" )
		{
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1009,10,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1109,11,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1209,12,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1309,13,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1409,14,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1509,15,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1609,16,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1709,17,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1809,18,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1909,19,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2009,20,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2109,21,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
		}


		}

		if(  $_GET["name3"] != "" )
		{
			$nGender = $_GET["SEX3"];
			if( $nGender == 1 )
			{
				$strQuery = "																																																																															 	  
				INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 2, '[".$_GET["name3"]."]".$_GET["id"]."', '1', 11, 101101111, 102101101, 201001101, 202001101, 204001101, now(), ".$_GET["level3"].", ".(($_GET["level3"]*2)+(floor($_GET["level3"]/10))).", $nPosition3, 100 )
							";
			} else
			{
				$strQuery = "																																																																															 	  
				INSERT INTO tb_game_player  (player_seq, member_seq, player_order, player_name, player_gender, player_skin,  player_face, player_hair, player_shirts, player_pants, player_shoes, player_createdate, player_level,  player_faculty, player_position, player_skill)  VALUES ('', ".$nSeq.", 2, '[".$_GET["name3"]."]".$_GET["id"]."', '2', 21, 101101121, 102104101, 201002101, 202002101, 204002101, now(), ".$_GET["level3"].", ".(($_GET["level3"]*2)+(floor($_GET["level3"]/10))).", $nPosition3, 100 )
							";
			}

				
				
				$Result = $dBase->Query($strQuery);			
			$nPlayerSeq = mysql_insert_id();
			$strQuery = "
				INSERT INTO tb_game_faculty (player_seq) VALUES (".$nPlayerSeq.")
						";
				$Result = $dBase->Query($strQuery);			
			$strQuery = "
				INSERT INTO tb_game_setting (player_seq) VALUES (".$nPlayerSeq.")
						";
				$Result = $dBase->Query($strQuery);			

		if( $_GET["TRAINNING"] == "on" )
		{
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1009,10,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1109,11,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1209,12,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1309,13,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1409,14,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1509,15,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1609,16,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1709,17,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1809,18,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',1909,19,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2009,20,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
			$strQuery = "
				INSERT INTO tb_game_training (member_seq,player_seq,training_state,training_code,training_type,training_level,training_kind,training_buydate) VALUES (".$nSeq .",".$nPlayerSeq.",'1',2109,21,9,'',now());
				";
			$Result	= $dBase->Query($strQuery);
		}
		}
	}
	while(false);

	if( $strError == "" ) $strError = "정상적으로 등록되었습니다.";

	echo "<SCRIPT LANGUAGE='JavaScript'> ERROR( '".$strError."' );	</SCRIPT>";
	
}


?>

<br>

<FORM ID="hForm">
<TABLE border=0 cellpadding=1 cellspacing=1 bgcolor="#646464">
<TR bgcolor="#353535">
	<TD><B>아이디</B> :</TD>
	<TD><INPUT TYPE="text" NAME="id" value="<?=$_GET["id"]?>">   </TD>
</TR>
<TR bgcolor="#353535">
	<TD><B>트레이닝</B> :</TD>
	<TD>  <INPUT TYPE="checkbox" NAME="TRAINNING" <? echo $_GET["TRAINNING"]=="on" ? "checked":""?> > Level 9 </TD>
</TR>
<TR bgcolor="#353535">
	<TD><B>튜토리얼</B> :</TD>
	<TD>  <INPUT TYPE="checkbox" NAME="TUTORIAL" <? echo $_GET["TUTORIAL"]=="on" ? "checked":""?>>  모든 튜토리얼</TD>
</TR>

<TR height="20"><TD align="right"></TD><TD>포지션 1</TD></TR>
<TR bgcolor="#353535">
	<TD>이름1 </TD>
	<TD><INPUT TYPE="text" NAME="name1" value="<?=$_GET["name1"];?>"></TD>
</TR>
<TR bgcolor="#353535">
	<TD>포지션1 </TD>
	<TD>
	<SELECT NAME="position1" style="width:127">
		<OPTION VALUE="BG">BG</OPTION>
		<OPTION VALUE="FW">FW</OPTION>
		<OPTION VALUE="ST">ST</OPTION>
		<OPTION VALUE="CF">CF</OPTION>
		<OPTION VALUE="WF">WF</OPTION>
		<OPTION VALUE="SS">SS</OPTION>
		<OPTION VALUE="MF">MF</OPTION>
		<OPTION VALUE="AM">AM</OPTION>
		<OPTION VALUE="CM">CM</OPTION>
		<OPTION VALUE="SM">SM</OPTION>
		<OPTION VALUE="DM">DM</OPTION>
		<OPTION VALUE="DF">DF</OPTION>
		<OPTION VALUE="SW">SW</OPTION>
		<OPTION VALUE="CB">CB</OPTION>
		<OPTION VALUE="SP">SP</OPTION>
		<OPTION VALUE="GK">GK</OPTION>
		<OPTION VALUE="<?=$_GET["position1"];?>" SELECTED><?=$_GET["position1"];?></OPTION>
	</SELECT>
</TR>
<TR bgcolor="#353535">
	<TD>성별1 </TD>
	<TD>  <INPUT TYPE="radio" NAME="SEX1" VALUE="1" <? echo $_GET["SEX1"]=="1" ? "checked":""?> >  남   <INPUT TYPE="radio" NAME="SEX1" VALUE="2" <? echo $_GET["SEX1"]=="2" ? "checked":""?>>  여</TD>
</TR>
<TR bgcolor="#353535">
	<TD>레벨1  </TD>
	<TD><INPUT TYPE="text" NAME="level1" value="<?=$_GET["level1"];?>"></TD>
</TR>
<TR height="20"><TD></TD><TD>포지션 2</TD></TR>
<TR bgcolor="#353535">
	<TD>이름2 </TD>
	<TD><INPUT TYPE="text" NAME="name2" value="<?=$_GET["name2"];?>"></TD>
</TR>
<TR bgcolor="#353535">
	<TD>포지션2  </TD>
	<TD>	<SELECT NAME="position2" style="width:127">
		<OPTION VALUE="BG">BG</OPTION>
		<OPTION VALUE="FW">FW</OPTION>
		<OPTION VALUE="ST">ST</OPTION>
		<OPTION VALUE="CF">CF</OPTION>
		<OPTION VALUE="WF">WF</OPTION>
		<OPTION VALUE="SS">SS</OPTION>
		<OPTION VALUE="MF">MF</OPTION>
		<OPTION VALUE="AM">AM</OPTION>
		<OPTION VALUE="CM">CM</OPTION>
		<OPTION VALUE="SM">SM</OPTION>
		<OPTION VALUE="DM">DM</OPTION>
		<OPTION VALUE="DF">DF</OPTION>
		<OPTION VALUE="SW">SW</OPTION>
		<OPTION VALUE="CB">CB</OPTION>
		<OPTION VALUE="SP">SP</OPTION>
		<OPTION VALUE="GK">GK</OPTION>
		<OPTION VALUE="<?=$_GET["position2"];?>" SELECTED><?=$_GET["position2"];?></OPTION>
	</SELECT></TD>
</TR>
<TR bgcolor="#353535">
	<TD>성별2 </TD>
	<TD>  <INPUT TYPE="radio" NAME="SEX2" VALUE="1" <? echo $_GET["SEX2"]=="1" ? "checked":""?> >  남   <INPUT TYPE="radio" NAME="SEX2" VALUE="2" <? echo $_GET["SEX2"]=="2" ? "checked":""?>>  여</TD>
</TR>

<TR bgcolor="#353535">
	<TD>레벨2  </TD>
	<TD><INPUT TYPE="text" NAME="level2" value="<?=$_GET["level2"];?>"></TD>
</TR>
<TR height="20"><TD></TD><TD>포지션 3</TD></TR>
<TR bgcolor="#353535">
	<TD>이름3 </TD>
	<TD><INPUT TYPE="text" NAME="name3" value="<?=$_GET["name3"];?>"></TD>
</TR>
<TR bgcolor="#353535">
	<TD>포지션3  </TD>
	<TD>	<SELECT NAME="position3" style="width:127">
		<OPTION VALUE="BG">BG</OPTION>
		<OPTION VALUE="FW">FW</OPTION>
		<OPTION VALUE="ST">ST</OPTION>
		<OPTION VALUE="CF">CF</OPTION>
		<OPTION VALUE="WF">WF</OPTION>
		<OPTION VALUE="SS">SS</OPTION>
		<OPTION VALUE="MF">MF</OPTION>
		<OPTION VALUE="AM">AM</OPTION>
		<OPTION VALUE="CM">CM</OPTION>
		<OPTION VALUE="SM">SM</OPTION>
		<OPTION VALUE="DM">DM</OPTION>
		<OPTION VALUE="DF">DF</OPTION>
		<OPTION VALUE="SW">SW</OPTION>
		<OPTION VALUE="CB">CB</OPTION>
		<OPTION VALUE="SP">SP</OPTION>
		<OPTION VALUE="GK">GK</OPTION>
		<OPTION VALUE="<?=$_GET["position3"];?>" SELECTED><?=$_GET["position3"];?></OPTION>
	</SELECT></TD>
</TR>
<TR bgcolor="#353535">
	<TD>성별3 </TD>
	<TD>  <INPUT TYPE="radio" NAME="SEX3" VALUE="1" <? echo $_GET["SEX3"]=="1" ? "checked":""?> >  남   <INPUT TYPE="radio" NAME="SEX3" VALUE="2" <? echo $_GET["SEX3"]=="2" ? "checked":""?>>  여</TD>
</TR>

<TR bgcolor="#353535">
	<TD>레벨3  </TD>
	<TD><INPUT TYPE="text" NAME="level3" value="<?=$_GET["level3"];?>"></TD>
</TR>
</TABLE><BR><BR>
<INPUT TYPE="hidden" NAME="MODE">
<INPUT  TYPE="button"  onclick="Submit()" VALUE="캐릭터 등록">
</form> 

<br><br>*  <b>중요</b> ▷ 계정생성 전에 개발상 변경사항이 없는지 꼭 확인 바랍니다. 
<SCRIPT LANGUAGE="JavaScript">
<!--
	var hForm = document.getElementById( "hForm" );	
	function Submit()
	{
		if( hForm.id.value == "" )
		{
			alert( "아이디를 입력하여 주세요." );
			return;
		}
		if( hForm.name1.value == "" )
		{
			alert( "첫 이름은 꼭 입력해야 합니다." );
			return;
		}
		if( hForm.position1.value == "" )
		{
			alert( "첫 포지션은 꼭 입력해야 합니다." );
			return;
		}
		if( hForm.level1.value == "" )
		{
			alert( "첫 레벨은 꼭 입력해야 합니다." );
			return;
		}
		if( hForm.name1.value == hForm.name2.value ||  ( (hForm.name2.value == hForm.name3.value ) && hForm.name3.value!= "" ) )
		{
			alert( "같은 이름으로 캐릭터를 생성할 수 없습니다." );
			return;
		}

		if(  hForm.name2.value  != ""  && ( hForm.position2.value == "" || hForm.level2.value == "" ))
		{
			alert( "캐릭터 정보가 부족합니다." );
			return;
		}
		if(  hForm.name3.value  != ""  && ( hForm.position3.value == "" || hForm.level3.value == "" ))
		{
			alert( "캐릭터 정보가 부족합니다." );
			return;
		}

		if( hForm.name1.value.length > 5 || hForm.name2.value.length > 5  || hForm.name3.value.length > 5 )
		{
			alert( "캐릭터 이름은 5자 이내로 제한합니다." );
			return;
		}
		
		if(  ( hForm.name1.value  != ""  && hForm.SEX1[0].checked == false &&  hForm.SEX1[1].checked == false  ) || ( hForm.name2.value  != ""  && hForm.SEX2[0].checked == false &&  hForm.SEX2[1].checked == false  )  || ( hForm.name3.value  != ""  && hForm.SEX3[0].checked == false &&  hForm.SEX3[1].checked == false  )  )
		{
			alert( "성별을 입력하지 않으셨습니다." );
			return;
		}
	

		hForm.MODE.value = "OK"
		hForm.submit();
		return;
	}
//-->
</SCRIPT>


