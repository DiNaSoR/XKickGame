<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );

	$nYear = date("Y"); 	$nMonth= date("n");
	if( $_REQUEST["FM_YEAR"] != "" )  $nYear=$_REQUEST["FM_YEAR"];
	if( $_REQUEST["FM_MONTH"] != "" ) $nMonth=$_REQUEST["FM_MONTH"];

	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hPager = new CPager( $_REQUEST["page"], 20,10,"&FM_YEAR=".$nYear."&FM_MONTH=".$nMonth."&menu=member_conhistory&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hPager->SetQueryFormArray( array( array(  "C.member_id", PAGER_SEARCH_LEFT, IDS_MEMBER_ID ),array(  "B.player_name", PAGER_SEARCH_BOTH, IDS_MEMBER_CHR_NAME )) );

	// 검색 세팅
	$strWhere = $hPager->GetQueryString( $_REQUEST["stype"], $_REQUEST["query"] );

	if( $strWhere != "" )	$strWhere = " WHERE B.player_seq>0 and ".$strWhere;
	else					$strWhere = " WHERE B.player_seq>0 ";


	$strTableName = sprintf( "tb_mst_playlog_%02d_%02d", $nYear-2000, $nMonth );
	if( mysql_num_rows( $dBase->Query("SHOW TABLES FROM ".CONFIG_DB_LOG_NAME." WHERE Tables_in_".CONFIG_DB_LOG_NAME."='".$strTableName."';")) == 0 )
	{
		// 출력
		$hLayout->TITLE(IDS_MENU_MEMBER_HISTORY);
		$hLayout->ShowDateForm( $nYear, $nMonth, CONFIG_DOC_ROOT."?menu=member_conhistory" );
		exit;
	}
	$strFrom = " ".CONFIG_DB_LOG_NAME.".".$strTableName." as A INNER JOIN ".CONFIG_DB_NAME.".tb_game_player as B ON  A.player_seq=B.player_seq INNER JOIN ".CONFIG_DB_NAME.".tb_mst_member as C ON B.member_seq=C.member_seq JOIN ".CONFIG_DB_NAME.".tb_cod_position as D ON B.player_position=D.position_code ";

	$Result = $dBase->Query("SELECT count(*) as cnt FROM ".$strFrom.$strWhere."  GROUP BY A.player_seq  ");			
	$nDataCnt = mysql_num_rows( $Result  );

	$strQuery = "SELECT A.player_seq as SEQ, C.member_id, B.player_name as NAME , B.player_level, D.position_initial, count(A.player_seq) as CNT, MAX(playlog_date_in) as DATE_IN,MAX(playlog_date_out) as DATE_OUT, playlog_remoteip as IP , MAX(playlog_seq) as mseq  FROM ".$strFrom.$strWhere." GROUP BY A.player_seq  ORDER BY mseq DESC  LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";

	$Result   = $dBase->Query($strQuery);			
	// 출력
	$hLayout->TITLE(IDS_MENU_MEMBER_HISTORY);
	$hLayout->ShowDateForm( $nYear, $nMonth, CONFIG_DOC_ROOT."?menu=member_conhistory" );

	$hLayout->TABLE_SetTitleArray( array( array( IDS_MEMBER_SEQ, 80, "center" ), array(IDS_MEMBER_ID, 100, "center"), array(IDS_MEMBER_CHR_NAME, 150, "center", "#484848"), array(IDS_LABEL_LEVEL, 75, "center"), array(IDS_PLAYER_POSITION, 75, "center"),  array(IDS_PLAYER_CON_COUNT, 60, "center"),  array(IDS_PLAYER_LATEST_LOGIN, 155, "center"), array(IDS_PLAYER_LATEST_LOGOUT, 155, "center")	) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start( 27 );
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
	$hPager->ShowQueryForm( $_POST["query"], $_REQUEST["stype"], CONFIG_DOC_ROOT."?menu=member_conhistory", CONFIG_DOC_ROOT."image/");
?>
