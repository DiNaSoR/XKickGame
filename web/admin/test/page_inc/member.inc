<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=member&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "member_id", PAGER_SEARCH_LEFT, IDS_MEMBER_ID ),array(  "member_name", PAGER_SEARCH_LEFT, IDS_MEMBER_NAME ), array(  "member_seq", PAGER_SEARCH_NUMBER, IDS_MEMBER_SEQ ), array(  "player_name", PAGER_SEARCH_LEFT, IDS_MEMBER_CHR_NAME ) , array(  "player_seq", PAGER_SEARCH_NUMBER, IDS_MEMBER_CHR_SEQ ) ) );

	// 검색 세팅
	$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );
	if( $strWhere != "" )	$strWhere = " WHERE ".$strWhere;
	if( $_REQUEST["stype"] == "player_name" || $_REQUEST["stype"] == "player_seq" ) $strFrom = " tb_mst_member as A JOIN tb_game_player as B on A.member_seq=B.member_seq JOIN tb_cod_status as C on A.member_state=C.status_code "; 
	else	$strFrom = " tb_mst_member as A JOIN tb_cod_status as C on A.member_state=C.status_code ";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			
	$strQuery = "SELECT A.member_seq, C.status_name , concat( \"<a href='".CONFIG_DOC_ROOT."?menu=memberdetail&seq=\", A.member_seq , \"'>\", member_id, \"</a>\"),member_name,member_ip,member_createdate FROM ".$strFrom.$strWhere." ORDER BY member_seq DESC LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TITLE(IDS_MEMBER_TITLE);
	$hLayout->TABLE_SetTitleArray( array( array( IDS_MEMBER_SEQ, 80, "center" ), array(IDS_MEMBER_STATUS, 60, "center"), array(IDS_MEMBER_ID, 162, "center", "#484848"), array(IDS_MEMBER_NAME, 82, "center"), array(IDS_MEMBER_JOIN_IP, 125, "center"), array(IDS_MEMBER_JOIN_DATE, 140, "center")	) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start( 27 );
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=member", CONFIG_DOC_ROOT."image/");
?>
