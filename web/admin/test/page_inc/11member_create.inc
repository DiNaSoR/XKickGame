<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	define( 'CREATE_MODE', '3' );
	
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hLayout->TITLE(IDS_MENU_MEMBER_CREATE);

	if( $_POST["mode"] == CREATE_MODE )
	{
		$strID   = $_POST["userid"];
		$strPass = MD5($_POST["passwd"]);
		$strName = $_POST["name"];
		if( $strID == "" || $strPass == "" || $strName == "" ) 
		{
			$strError =  IDS_NO_INPUT; 
			echo "<SCRIPT LANGUAGE='JavaScript'> alert( '".$strError."' );	</SCRIPT>";
			exit;
		}

		$strQuery = "
							SELECT member_seq
								FROM tb_mst_member 
									WHERE member_id='".$strID."'
						";
		$Result = $dBase->Query($strQuery);			
		$rows = mysql_fetch_array($Result, MYSQL_ASSOC );
		$strError = "";
		if( $rows["member_seq"] != FALSE ) { $strError =  IDS_MEMBER_EXIST;  echo "<SCRIPT LANGUAGE='JavaScript'> alert( '".$strError."' );	</SCRIPT>"; }
		else
		{
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
							member_id='".$strID."' , member_pass='".$strPass."', member_name='".$strName."', member_register='".$nRegNumber."', member_state='1', member_partner='enterplay', member_ip='".$_SERVER["REMOTE_ADDR"]."', 
								member_speed=0, member_createdate=now(), member_logindate='0000-00-00 00:00:00', member_deletedate='0000-00-00 00:00:00', member_blockdate='0000-00-00 00:00:00', member_releasedate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			

		$nSeq = mysql_insert_id();
		
		$nQuest = 0;
		$nTutorial = 0;
		if( $_POST["TUTORIAL"] == "on" )
		{
			$nQuest = 7;
			$nTutorial = 63;
		}

		$strQuery = "
						INSERT INTO tb_game_trio SET member_seq=".$nSeq.", trio_cash=10000000, trio_point=10000000, trio_credit=0, trio_allpoint=0, trio_allcredit=0, trio_lastseq=0, trio_server=0, 
								trio_count=3, trio_tutorial=".$nTutorial.", trio_quest=".$nQuest.", trio_camera=12, trio_camerateam=1, trio_radian=0, trio_zoom=2, trio_target=10, trio_shadow=0, trio_label=0, trio_sound=8, trio_music=16, trio_invite=1, 
									trio_whisper=1, trio_friend=1, trio_up=222, trio_down=230, trio_left=225, trio_right=227, trio_leftshoot=32, trio_rightshoot=33, trio_longpass=30, trio_shortpass=31, trio_screen=17, trio_tackle=16, trio_steal=18, trio_quick=42, trio_skill1=46, trio_skill2=45, trio_skill3=47, trio_skill4=57, trio_skillcode1=0, trio_skillcode2=0, trio_skillcode3=0, trio_skillcode4=0, trio_logindate=now(), trio_deletedate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			
			
		$strQuery = "
						INSERT INTO tb_mst_meminfo SET member_register='".$nRegNumber."',meminfo_postcode='111-111',meminfo_addr1='테스트 생성 계정',meminfo_addr2='테스트 생성 계정',meminfo_email='TEST@ENTERPLAY.CO.KR',meminfo_telcode='02',meminfo_telnum1='1234',meminfo_telnum2='1234',meminfo_hpcode='011',meminfo_hpnum1='1234',meminfo_hpnum2='1234',meminfo_memo='',meminfo_mail_agree='N',meminfo_sms_agree='N',meminfo_memodate='0000-00-00 00:00:00';
					";
		$Result = $dBase->Query($strQuery);			


		if( $strError == "" ) $strError = "Create success.";
		echo "<SCRIPT LANGUAGE='JavaScript'> alert( '".$strError."' );	</SCRIPT>";
		}
	}

?>
<FORM name="FORM_CREATE" method="post" target="_self">
<INPUT TYPE="hidden" NAME="mode" VALUE="3">
<table><tr><td width="25"></td><td>
	 <table border="0" cellpadding="0" cellspacing="0" >
				<tr>
					<td align="left" height="25" width="70"><?=IDS_MEMBER_ID?> </td><td width="40"><input type="text" name="userid" size="12" maxlength="12" style="font-size:9pt;" value="<?= $_POST["userid"]?>"></td>
				</tr>
				<tr>
					<td align="left" height="25"><?=IDS_MEMBER_PASS?>  </td><td><input type="text" name="passwd" size="12" maxlength="12" style="font-size:9pt;" value=""></td>
				</tr>
				<tr>
					<td align="left" height="25"><?=IDS_MEMBER_NAME?>  </td><td><input type="text" name="name" size="12" maxlength="12" style="font-size:9pt;" value="<?= $_POST["name"]?>"></td>
				</tr>
				<TR>
					<TD>Tutorial </TD>
					<TD>  <INPUT TYPE="checkbox" NAME="TUTORIAL" <? echo $_POST["TUTORIAL"]=="on" ? "checked":""?>>  </TD>
				</TR>

				<tr>
					<td align="left" height="50"></td><td align="right"><input type="image" src="<?=CONFIG_DOC_ROOT?>image/B_big_ok.gif" border="0"></td>
				</tr>

	</table>
</tr></td></table>
</FORM>