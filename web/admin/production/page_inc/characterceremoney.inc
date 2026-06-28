<?
//##############################################################################
// 캐릭터 세레모니 설정 페이지
//##############################################################################
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	define( 'DELETE_MODE', '2' );

	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

	if( $_POST["mode"] == DELETE_MODE )
	{
		$nItemCnt = sizeof($_POST["ceremony_seq"]);
		$strWhere = "";
		for( $i = 0; $i < $nItemCnt; $i++ ) 
		{
			if( $i != 0 ) $strWhere .= " OR ";
			$strWhere .= "ceremony_seq =".$_POST["ceremony_seq"][$i];
		}

		$strQuery = "
						UPDATE tb_game_ceremony 
							SET ceremony_state='0', ceremony_deletedate=now()
								WHERE ".$strWhere;
		
		$Result = $dBase->Query($strQuery);					
	}

	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=characterceremony&seq=".$_GET["seq"]."&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "A.ceremony_code", PAGER_SEARCH_STRING, IDS_LABEL_CODE),array(  "B.ceremony_name", PAGER_SEARCH_BOTH, IDS_LABEL_NAME) ) );

	// 검색 세팅
	$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );
	if( $strWhere != "" )	$strWhere = " WHERE player_seq=".$_GET["seq"]." and ceremony_state='1' and ".$strWhere;
	else					$strWhere = " WHERE player_seq=".$_GET["seq"]." and ceremony_state='1' ";
	if( $_REQUEST["stype"] == "player_name" || $_REQUEST["stype"] == "player_seq" ) $strFrom = " tb_game_ceremony as A JOIN tb_cod_ceremony as B ON A.ceremony_code=B.ceremony_code"; 
	else	$strFrom = " tb_game_ceremony as A JOIN tb_cod_ceremony as B ON A.ceremony_code=B.ceremony_code";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			
	$strQuery = "SELECT concat(\"<input type='checkbox' name='ceremony_seq[]' value='\", A.ceremony_seq, \"'>\"), A.ceremony_seq, A.ceremony_code as ceremony_code, B.ceremony_name, A.ceremony_buydate FROM ".$strFrom.$strWhere." ORDER BY member_seq DESC LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TITLE(IDS_CHARACTER_CEREMONY);
//##############################################################################
?>
<table><tr><td width="25"></td><td>
<?
	$nMenu = 5;
	include "./page_inc/chracter_menu.inc";
?>
<table width="650" border=0 cellpadding=1 cellspacing=1 bgcolor="#646464">
<tr bgcolor="#353535">
	<td>
	<form name="FORM_CEREMONY_LIST" method="post" target="_self">
	<INPUT TYPE="hidden" NAME="mode" VALUE="2">
<?
	$hLayout->TABLE_SetTitleArray( array( array( "", 20, "center" ), array(IDS_LABEL_SEQ, 50, "center"), array(IDS_LABEL_CODE, 80, "center"),  array(IDS_LABEL_NAME, 207, "center", "#484848"), array(IDS_LABEL_BUYDATE, 150, "center")) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start(0);
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
?>
<table width="90%">
	<tr>
		<td align="right" height="40">
		<INPUT TYPE="IMAGE" SRC="<?=CONFIG_DOC_ROOT?>/image/B_big_del.gif" border="0">
		</td>
	</tr>
</table>
</form>
<?
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=characterceremony&seq=".$_GET["seq"], CONFIG_DOC_ROOT."image/");
?>
	</td>
</tr>
</table>
</td></tr></table>
