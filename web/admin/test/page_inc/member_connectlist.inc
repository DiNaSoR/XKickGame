<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=member_connectlist&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "member_id", PAGER_SEARCH_LEFT, IDS_MEMBER_ID ),array(  "player_name", PAGER_SEARCH_LEFT, IDS_MEMBER_CHR_NAME )) );

	// 검색 세팅
	$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );
	if( $strWhere != "" )	$strWhere = " WHERE (B.trio_server <> 0) and ".$strWhere;
	else					$strWhere = " WHERE (B.trio_server <> 0) ";
	$strFrom = " (tb_game_player as A join tb_game_trio AS B on((B.trio_lastseq = player_seq))) join tb_mst_member as C ON A.member_seq=C.member_seq JOIN tb_game_server AS D ON B.trio_server=D.server_code JOIN tb_cod_position as E ON A.player_position=E.position_code";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			

	$strQuery = "SELECT player_seq AS player_seq, C.member_id,concat( \"<a href='".CONFIG_DOC_ROOT."?menu=memberdetail&seq=\", A.member_seq , \"'>\", player_name, \"</a>\"),player_level AS player_level, E.position_initial, D.server_name , B.trio_logindate FROM ".$strFrom.$strWhere." ORDER BY A.member_seq DESC LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TITLE(IDS_MEMBER_CURRENT_TITLE);
	$hLayout->TABLE_SetTitleArray( array( array( IDS_MEMBER_SEQ, 80, "center" ), array(IDS_MEMBER_ID, 100, "center"), array(IDS_MEMBER_CHR_NAME, 160, "center", "#484848"), array(IDS_LABEL_LEVEL, 75, "center"), array(IDS_PLAYER_POSITION, 75, "center"),  array(IDS_SERVER_NAME, 130, "center"), array(IDS_PLAYER_LOGINDATE, 155, "center")	) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start( 27 );
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=member_connectlist", CONFIG_DOC_ROOT."image/");
?>
