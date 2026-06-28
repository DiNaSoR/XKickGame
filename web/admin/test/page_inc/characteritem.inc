<?
//##############################################################################
// 캐릭터 기록 설정 페이지
//##############################################################################
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	define( 'DELETE_MODE', '2' );

	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );

	if( $_POST["mode"] == DELETE_MODE )
	{
		$nItemCnt = sizeof($_POST["item_seq"]);
		$strWhere = "";
		for( $i = 0; $i < $nItemCnt; $i++ ) 
		{
			if( $i != 0 ) $strWhere .= " OR ";
			$strWhere .= "item_seq =".$_POST["item_seq"][$i];
		}

		$strQuery = "
						UPDATE tb_game_item 
							SET item_state='0', item_deletedate=now()
								WHERE ".$strWhere;
		
		$Result = $dBase->Query($strQuery);					
	}

	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=characteritem&seq=".$_GET["seq"]."&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "A.item_code", PAGER_SEARCH_STRING, IDS_LABEL_CODE ),array(  "B.item_name", PAGER_SEARCH_BOTH, IDS_LABEL_NAME), array(  "A.item_option0", PAGER_SEARCH_NUMBER, IDS_LABEL_OPTION."1" ), array(  "A.item_option1", PAGER_SEARCH_NUMBER, IDS_LABEL_OPTION."2" ), array(  "A.item_option2", PAGER_SEARCH_NUMBER, IDS_LABEL_OPTION."3" )) );

	// 검색 세팅
	$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );
	if( $strWhere != "" )	$strWhere = " WHERE player_seq=".$_GET["seq"]." and item_state='1' and ".$strWhere;
	else					$strWhere = " WHERE player_seq=".$_GET["seq"]." and item_state='1' ";
	if( $_REQUEST["stype"] == "player_name" || $_REQUEST["stype"] == "player_seq" ) $strFrom = " tb_game_item as A JOIN tb_cod_item as B ON A.item_code=B.item_code"; 
	else	$strFrom = " tb_game_item as A JOIN tb_cod_item as B ON A.item_code=B.item_code";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			
	$strQuery = "SELECT concat(\"<input type='checkbox' name='item_seq[]' value='\", A.item_seq, \"'>\"), A.item_seq, A.item_code as item_code, B.item_type, B.item_name, CASE A.item_equip WHEN '1' THEN 'O' WHEN '0' THEN 'X' END, A.item_amount, item_option0,item_option1,item_option2, item_deletedate FROM ".$strFrom.$strWhere." ORDER BY member_seq DESC LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TITLE(IDS_CHARACTER_ITEM);
//##############################################################################
?>
<table><tr><td width="25"></td><td>
<?
	$nMenu = 2;
	include "./page_inc/chracter_menu.inc";
?>
<table width="100%" border=0 cellpadding=1 cellspacing=1 align="center" bgcolor="#646464">
<tr bgcolor="#353535">
	<td>
	<form name="FORM_ITEM_LIST" method="post" target="_self">
	<INPUT TYPE="hidden" NAME="mode" VALUE="2">
<?
	$hLayout->TABLE_SetTitleArray( array( array( "", 20, "center" ), array(IDS_LABEL_SEQ, 80, "center"), array(IDS_LABEL_CODE, 80, "center"), array(IDS_LABEL_TYPE, 80, "center"), array(IDS_LABEL_NAME, 200, "center", "#484848"), array(IDS_LABEL_EQUIP, 60, "center"), array(IDS_LABEL_AMOUNT, 65, "center"), array(IDS_LABEL_OPTION."1", 60, "center"), array(IDS_LABEL_OPTION."2", 60, "center"), array(IDS_LABEL_OPTION."3", 60, "center"), array(IDS_LABEL_EXPIREDATE, 140, "center")	) );
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
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=characteritem&seq=".$_GET["seq"], CONFIG_DOC_ROOT."image/");
?>
	</td>
</tr>
</table>
</td></tr></table>
