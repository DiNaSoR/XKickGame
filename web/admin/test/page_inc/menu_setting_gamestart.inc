<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	require_once( CONFIG_CLASS_PATH."xkick_socket.inc" );
	require_once( CONFIG_CLASS_PATH."xkick_class.inc" );
	require_once( CONFIG_CLASS_PATH."/login.inc" );
	$hLogin		= new CLogin();

	if($hLogin->GetCookie( "admin_level" ) > 1) {
		echo "<script>alert('".IDS_PAGE_NOT_FOUND."')</script>";
		exit;
	}
	
	define( 'EDIT_MODE'		, '1' );
	define( 'ENABLE_MODE'	, '5' );
	
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=menu_setting_notice&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "notice_text", PAGER_SEARCH_BOTH, IDS_LABEL_DATA ),array(  "notice_date", PAGER_SEARCH_LEFT, IDS_LABEL_CREATEDATE) ) );
	$hLayout->TITLE(IDS_MENU_SETTING_GAME_START);

	if( $_POST["mode"] == EDIT_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetGamestartMgr()->EditGamestart( $_POST["text"] );
		$hLayout->ReplacePage();
	} else if( $_POST["mode"] == ENABLE_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetGamestartMgr()->EnableGamestart( $_POST["text"] );
		$hLayout->ReplacePage();
	}

	// 검색 세팅
	$strWhere = "";
	$strFrom = " tb_mst_start ";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			
	$strQuery = "SELECT concat(\"<input type='TEXT' SIZE='70' name='start_msg' value='\", start_msg, \"'>\"), concat('<a href=\"javascript:Edit();\">".IDS_LABEL_MODIFY_SHORT."</a>'), CASE start_yn WHEN 'Y' THEN  concat('<a href=\"javascript:Disable();\"><b>".IDS_LABEL_STARTDATE."</b></a>') WHEN 'N' THEN concat('<a href=\"javascript:Enable();\"><font color=\"red\">".IDS_LABEL_STOP."</font></a>')  END FROM ".$strFrom.$strWhere.";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TABLE_SetTitleArray( array(  array(IDS_LABEL_PERMITMSG, 400, "center"), array(IDS_LABEL_MANAGE_MENU, 100, "center"),array(IDS_SERVER_STATUS, 100, "center")	) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start( 27 );
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
?>
<FORM name="FORM_ITEM_LIST" method="post" target="_self">
		<INPUT TYPE="hidden" NAME="mode" VALUE="">
		<INPUT TYPE="hidden" NAME="seq"  VALUE="">
		<INPUT TYPE="hidden" NAME="text" VALUE="">
</FORM>
<SCRIPT LANGUAGE="JavaScript">
<!--
	var hForm = document.getElementById( "FORM_ITEM_LIST" );
	function Edit() 
	{
		var strText = document.getElementById( "start_msg" ).value;
		hForm.mode.value = 1;		hForm.text.value = strText;
		hForm.submit();
	}
	function Enable()
	{
		hForm.mode.value = 5;		
		hForm.text.value = 'Y';
		hForm.submit();
	}
	function Disable()
	{
		hForm.mode.value = 5;		
		hForm.text.value = 'N';
		hForm.submit();
	}
//-->
</SCRIPT>
