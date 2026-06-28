<?
//##############################################################################
// 캐릭터 상세정보 페이지
//##############################################################################
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	define( 'EDIT_MODE', '1' );
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

	if( $_GET["mode"] == "changestatus" )
	{
		$strQuery = "
						UPDATE tb_mst_member SET member_state=".$_GET["status"].", member_blockdate=now() WHERE member_seq=".$_GET["seq"].";
					";

		$Result = $dBase->Query($strQuery);			
	}
	if( $_POST["mode"] == EDIT_MODE )
	{
		$strQuery = "
							UPDATE tb_mst_meminfo 
								SET meminfo_email='".$_POST["email"]."', meminfo_telcode='".$_POST["telcode"]."',  meminfo_hpcode='".$_POST["hpcode"]."', meminfo_telnum1='".$_POST["telnum1"]."', meminfo_telnum2='".$_POST["telnum2"]."', meminfo_hpnum1='".$_POST["hpnum1"]."', meminfo_hpnum2='".$_POST["hpnum2"]."', meminfo_memo='".$_POST["memo"]."' , meminfo_memodate=now() 
									WHERE member_register=".$_POST["register"];
		$Result = $dBase->Query($strQuery);			

		if( $_POST["password"] != "" ) 
		{
			$strQuery = "
							UPDATE tb_mst_member
								SET member_pass='".$_POST["password"]."'
									WHERE member_register=".$_POST["register"];
		}
		$Result = $dBase->Query($strQuery);		
		$hLayout->ReplacePage();
		return;
	}

	if( $_POST['mode'] == "cash_edit" )
	{
		$cashtype = $_POST['cashtype'];
		$cashsize = $_POST['cashsize'];
		$chargetype = $_POST['chargetype'];

		if($cashtype == 1) $object_kind = 11;
		else if($cashtype == 2) $object_kind = 12;
		else $object_kind = 13;

		if($chargetype == 1) $sale_kind = 6;
		else $sale_kind = 7;
		
		$mon = date("m");
		if($mon >= 1 && $mon <= 3) $part = 1;
		else if($mon >= 4 && $mon <= 6) $part = 2;
		else if($mon >= 7 && $mon <= 9) $part = 3;
		else $part = 4;

		$member_seq = $_GET["seq"];
		$table = "tb_log_sale_" . date("y") . "_" . sprintf("%02d", $part);

		$strQuery = "USE xkick_log";
		$Result = $dBase->Query($strQuery);
		
		$strQuery = "
						INSERT INTO $table(sale_seq, member_seq, player_seq, object_seq, object_code, object_kind, buy_kind, sale_kind, sale_price, sale_amount, store_kind, sale_buydate) 
							VALUES(0, $member_seq, 0, 0, 0, '$object_kind', '9', '$sale_kind', $cashsize, 1, 1, NOW())
		";
		if(!mysql_query($strQuery, $dBase->DB)) {
			echo "<script>alert('".IDS_DB_ERROR_QUERY."');</script>";
			exit;
		}

		$strQuery = "USE xkick";
		$Result = $dBase->Query($strQuery);

		$strQuery = "UPDATE tb_game_trio SET ";
		if($cashtype == 1) $strQuery .= " trio_cash = trio_cash";
		else if($cashtype == 2) $strQuery .= " trio_point = trio_point";
		else $strQuery .= "trio_credit = trio_credit";

		if($chargetype == 1) $strQuery .= " + ";
		else $strQuery .= " - ";

		$strQuery .= $cashsize . " WHERE member_seq = ".$member_seq;

		if(!mysql_query($strQuery, $dBase->DB)) {
			echo "<script>alert('".IDS_DB_ERROR_QUERY."');</script>";
		}
		$hLayout->ReplacePage_Parent();
		exit;
	}

	$hLayout->TITLE(IDS_MEMBER_DETAIL_TITLE);
	$strQuery = "SELECT A.member_seq, A.member_id, A.member_pass, A.member_state, B.meminfo_email, D.status_name, A.member_name, A.member_register, A.member_ip, A.member_createdate , A.member_deletedate, A.member_logindate, A.member_blockdate, 
						B.meminfo_postcode, B.meminfo_addr1, B.meminfo_addr2, B.meminfo_telcode, B.meminfo_telnum1, B.meminfo_telnum2, B.meminfo_hpcode, B.meminfo_hpnum1, B.meminfo_hpnum2, B.meminfo_memo, B.meminfo_memodate, 
							C.trio_cash, C.trio_point, C.trio_credit 
					FROM  tb_mst_member as A JOIN tb_mst_meminfo as B on A.member_register=B.member_register 
							JOIN tb_game_trio as C on A.member_seq=C.member_seq 
								JOIN tb_cod_status as D on A.member_state=D.status_code WHERE A.member_seq=".$_GET["seq"];
	$Result = $dBase->Query($strQuery);			
	$hMemberRow = mysql_fetch_array($Result, MYSQL_ASSOC );

	$strQuery = "
						SELECT A.player_seq, A.player_name, A.player_level, A.player_position , B.position_initial 
							FROM tb_game_player as A INNER JOIN tb_cod_position as B ON A.player_position=B.position_code 
								WHERE member_seq=".$_GET["seq"]."
					";
	$Result = $dBase->Query($strQuery);			
//####################################################################
?>
<!--		HTML 시작		-->			
<script language="javascript">
<!--
function editCash(ctype,type)
{
	var s = document.MembInfoForm;
	var cSize, Msg, cname;
	var msgStr = "";
	if(ctype == 1) {
		cname = "Cash";
		cSize = s.csize1;
		s.cashtype.value = 1;
	} else if(ctype == 2) {
		cname = "POINT";
		cSize = s.csize2;
		s.cashtype.value = 2;
	} else {
		cname = "Credit";
		cSize = s.csize3;
		s.cashtype.value = 3;
	}

	if(cSize.value.search(/^[\d]*$/) < 0) {
		alert('<?=IDS_NUMERIC_CHK?>');
		cSize.value = '';
		cSize.focus();
		return false;
	}
	s.cashsize.value = cSize.value;

	if(cSize.value <= 0) 	{
		alert('<?=IDS_MEMBER_CASHINPUT_CHK?>');
		cSize.focus();
		return false;
	}

	s.mode.value = 'cash_edit';
	s.target = 'hidden_frame';

	msgStr = '<?=IDS_EDIT_CONFIRM?>';
	if(type == "1") s.chargetype.value = 1;
	else s.chargetype.value = 2;
	
	if(confirm(msgStr) == false) {
		return false;
	} else {
		s.submit();
	}
}
//-->
</script>
<FORM NAME="MembInfoForm" METHOD="POST" action="<?=$_SERVER['PHP_SELF']?>?menu=memberdetail&seq=<?=$_GET["seq"]?>">
<INPUT TYPE="hidden" NAME="mode" VALUE="1">
<INPUT TYPE="hidden" NAME="register" VALUE="<?=$hMemberRow["member_register"]?>">
<INPUT TYPE="hidden" NAME="cashtype" VALUE="">
<INPUT TYPE="hidden" NAME="cashsize" VALUE="">
<INPUT TYPE="hidden" NAME="chargetype" VALUE="">
<iframe name="hidden_frame" width=0 height=0></iframe>
        <table width="893" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="26">&nbsp;</td>
            <td width="867">

<!-- 내용테이블 시작 -->			<PRE></PRE>
              <table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#000000" align="center">
				<tr height="20" align="left" bgcolor="#EEEEEE">
				<td height=120 bgcolor="#353535" >
						<table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#353535" align="center">
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_SEQ?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_seq"]?>
						</td>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_STATUS?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td align=left>
						<? 
						$nStatus = $hMemberRow["member_state"];
							$nChangeValue = 1;
							if( $nStatus == 1 ) $nChangeValue = 4;
							else				$nChangeValue = 1;
						?>
						
							&nbsp;<?
							
							$strStatus = $hMemberRow["status_name"];
							if($nStatus == 2 ) $strStatus = "<font color='red'><b>".$strStatus."</b></font>";
							else if($nStatus == 4 ) $strStatus = "<font color='red'><b>".$strStatus."</b></font>";
							echo $strStatus;
							?>
							<a href="<?=$_SERVER['PHP_SELF']?>?menu=memberdetail&seq=<?=$_GET["seq"]?>&mode=changestatus&status=<?=$nChangeValue?>">
							 [<u><FONT COLOR="#CCFFFF">CHANGE</FONT></u>]
						</a>
						</td>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_ID?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td width="290">
							&nbsp;<?=$hMemberRow["member_id"]?>
						</td>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_PASS?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td >
							&nbsp;<input type="text" name="password" value="" size=30 style="font-size:12"><BR>KEY : <?=$hMemberRow["member_pass"]?>
						</td>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_NAME?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_name"]?>
						</td>
						<td width="135" style="display:none">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr > 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_REG?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td style="display:none">&nbsp;<?=substr($hMemberRow["member_register"],0,6)?> - <?=substr($hMemberRow["member_register"],6,13)?></td>
						<tr>
						<td width="135" >
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_MAIL?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<input type="text" name="email" value="<?=$hMemberRow["meminfo_email"]?>" size=30 style="font-size:12">
						</td>
						<td width="135" >
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000">IP ADDRESS</font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_ip"]?>
						</td>
						</tr>
						</table>
				</td>
			</tr>
			<tr height="20" align="left" bgcolor="#EEEEEE"  style="display:none">
				<td height="130" bgcolor="#353535" >
						<table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#353535" align="center">
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_ADRESS?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["meminfo_addr1"]?> <?=$hMemberRow["meminfo_addr2"]?>
						</td>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_POST?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["meminfo_postcode"]?>
						</td>
						<tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_TELNUMBER?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<input type="text" name="telcode" value="<?=$hMemberRow["meminfo_telcode"]?>" size=4 style="font-size:12"> - <input type="text" name="telnum1" value="<?=$hMemberRow["meminfo_telnum1"]?>" size=4 style="font-size:12"> - <input type="text" name="telnum2" value="<?=$hMemberRow["meminfo_telnum2"]?>" size=4 style="font-size:12">
						</td>
						
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_MOBILE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<input type="text" name="hpcode" value="<?=$hMemberRow["meminfo_hpcode"]?>" size=4 style="font-size:12"> - <input type="text" name="hpnum1" value="<?=$hMemberRow["meminfo_hpnum1"]?>" size=4 style="font-size:12"> - <input type="text" name="hpnum2" value="<?=$hMemberRow["meminfo_hpnum2"]?>" size=4 style="font-size:12">
						</td>
						</table>
				</td>
			</tr>

			<tr height="20" align="left" bgcolor="#EEEEEE">
				<td height=100 bgcolor="#353535" >
						<table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#353535" align="center">
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_CYBERMONEY?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td WIDTH="300">
							&nbsp;<?=number_format($hMemberRow["trio_cash"])?> Cash
							<input type="text" name="csize1" value="" size="5" maxlength="7"> <input type="button" value="+" onclick="javascript:editCash('1','1');"><input type="button" value="-" onclick="javascript:editCash('1','2');">
						</td>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_POINT?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td WIDTH="300">
							&nbsp;<?=number_format($hMemberRow["trio_point"])?> POINT
							<input type="text" name="csize2" value="" size="5" maxlength="7"> <input type="button" value="+" onclick="javascript:editCash('2','1');"><input type="button" value="-" onclick="javascript:editCash('2','2');">
						</td>
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_CREDIT?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td WIDTH="300">
							&nbsp;<?=number_format($hMemberRow["trio_credit"])?> Credit
							<input type="text" name="csize3" value="" size="5" maxlength="7"> <input type="button" value="+" onclick="javascript:editCash('3','1');"><input type="button" value="-" onclick="javascript:editCash('3','2');">
						</td>
						<td width="135">
							  &nbsp; <!-- ... //-->
							</td>
						</td>
						<td WIDTH="300">
							&nbsp; 
						</td>
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_JOIN_DATE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_createdate"]?>
						</td>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_DELETE_DATE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_deletedate"]?>
						</td>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_SUSPEND_DATE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_blockdate"]?>
						</td>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_LAST_CONDATE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["member_logindate"]?>
						</td>
						</table>
				</td>
			</tr>
			<tr height="20" align="left" bgcolor="#EEEEEE">
				<td height=80 bgcolor="#353535" >
					<table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#353535" align="center">
					<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_CHAR_LIST?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
						<table>
							
								<?
									while( $hCharacterRow = mysql_fetch_array($Result, MYSQL_ASSOC ) )
									{
										echo "<tr height='21'>";
										echo "<td align='left' bgcolor='#4E4E4E'><FONT COLOR='#ffffff'>&nbsp;";
										echo $hCharacterRow["position_initial"];
										echo "&nbsp;</font></td>";
										echo "<td></td>";
										echo "<td><a href='".CONFIG_DOC_ROOT."?menu=characterdetail&seq=".$hCharacterRow["player_seq"]."'>";
										echo $hCharacterRow["player_name"];
										echo "</a> (LV.".$hCharacterRow["player_level"].")";
										echo "</td>";

										echo "</tr>";
									}
								?>
						</table>
						</td>
					</tr>
					</table>
				</td>
			</tr>
			<tr height="20" align="left" bgcolor="#EEEEEE">
				<td bgcolor="#353535" >
					<table width="100%" border=0 cellpadding=1 cellspacing=1 bgcolor="#353535" align="center">
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_MEMO?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<textarea name="memo" rows=8 cols=100 style="font-size:9pt"><?=$hMemberRow["meminfo_memo"]?></textarea>
						</td>
						<tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								  <td>&nbsp;</td>
								  <td height="25" width="130" background="<?=CONFIG_DOC_ROOT?>image/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><font color="#000000"><?=IDS_MEMBER_MEMO_DATE?></font></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$hMemberRow["meminfo_memodate"]?>
						</td>
						<tr>
					</table>
				</td>
			</tr>
			</table>
			<table border="0" width="98%">
				<tr height="80" align="left" bgcolor="#EEEEEE">
					<td bgcolor="#353535" width="100%" align="right">
						<input type="image" src="<?=CONFIG_DOC_ROOT?>image/B_big_edit.gif">
						<A href='./?menu=member' > <img border='0' src="<?=CONFIG_DOC_ROOT?>image/B_big_list.gif"></a>
					</td>
				</td>
				
			</table>
</td>
</tr></table>
</FORM>