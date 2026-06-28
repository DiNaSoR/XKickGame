<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."xkick_socket.inc" );
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$hLayout->TITLE(IDS_MENU_SETTING_BROADCAST );
	$nSendType = 1;
	
	if( $_POST["sendtype"] != "" ) $nSendType = $_POST["sendtype"];

	if( $_POST["mode" ] == 1 )
	{
		$hSocket    = new CXKickSocket;
		$strResult  = $hSocket->Connect( STS_IP, STS_PORT );
		$strMessage = iconv( "utf-8", GAMESERVER_CHARSET, $_POST["message"] );
		$nMode = 0;
		switch( $nSendType )
		{
			case 1:
				$nMode = CHAT_KIND_NOTICE;
				break;
			case 2:
				$nMode = CHAT_KIND_ANNOUNCE;
				break;
			case 3:
				$nMode = CHAT_KIND_OPERATOR;
				break;
		}
		$hSocket->SendBroadcast( $nMode, $strMessage );
	}
?>
<FORM name="FORM_SENDER" method="post" target="_self">
<TABLE>
<TD WIDTH="27">
</TD>
<TD WIDTH="90">
<SELECT NAME="sendtype">
	<OPTION VALUE="1"><?=IDS_MENU_BROADCAST1?>
	<OPTION VALUE="2"><?=IDS_MENU_BROADCAST2?>
	<OPTION VALUE="3"><?=IDS_MENU_BROADCAST3?>
</SELECT>
</TD>
<TD>
	<INPUT TYPE="hidden" NAME="mode" VALUE="1" SIZE="60">
	<INPUT TYPE="TEXT" NAME="message" VALUE="" SIZE="60">
</TD>
</TABLE>
</FORM>

<SCRIPT LANGUAGE="JavaScript">
<!--
	document.getElementById( "FORM_SENDER" ).message.focus();
	document.getElementById( "FORM_SENDER" ).sendtype.value = 0<?=$nSendType?> ;
//-->
</SCRIPT>
