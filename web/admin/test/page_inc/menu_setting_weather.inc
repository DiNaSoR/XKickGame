<?
	require_once( CONFIG_CLASS_PATH."htmlform.inc" );
	require_once( CONFIG_CLASS_PATH."pager.inc" );
	require_once( CONFIG_CLASS_PATH."xkick_socket.inc" );
	require_once( CONFIG_CLASS_PATH."xkick_class.inc" );
	
	define( 'EDIT_MODE'		, '1' );
	define( 'DELETE_MODE'	, '2' );
	define( 'ADD_MODE'		, '3' );
	define( 'FLUSH_MODE'	, '4' );
	define( 'ENABLE_MODE'	, '5' );
	
?>
<FORM name="FORM_SENDER" method="post" target="_self">
		<INPUT TYPE="hidden" NAME="mode" VALUE="">
		<INPUT TYPE="hidden" NAME="seq"  VALUE="">
		<INPUT TYPE="hidden" NAME="weather" VALUE="">
		<INPUT TYPE="hidden" NAME="date" VALUE="">
		<INPUT TYPE="hidden" NAME="start" VALUE="">
		<INPUT TYPE="hidden" NAME="end" VALUE="">
</FORM>
<SCRIPT LANGUAGE="JavaScript">
<!--
	var hForm = document.getElementById( "FORM_SENDER" );

	function ShowSelector( nSeq, strText) 
	{	 
		document.write( "<SELECT NAME=\"Weather"+nSeq+"\"><OPTION VALUE=\"1\"><?=IDS_LABEL_FINE?></OPTION><OPTION VALUE=\"2\"><?=IDS_LABEL_CLOUD?></OPTION><OPTION VALUE=\"3\"><?=IDS_LABEL_RAIN?></OPTION><OPTION VALUE=\"4\"><?=IDS_LABEL_SNOW?></OPTION></SELECT>" ); 	
		hSelect = document.getElementById( "Weather"+nSeq);
		switch( strText )
		{
			case "WEATHER_FINE":   hSelect.value=1; break;
			case "WEATHER_CLOUDY": hSelect.value=2; break;
			case "WEATHER_RAIN":   hSelect.value=3; break;
			case "WEATHER_SNOW":   hSelect.value=4; break;
		}
	}

	function ShowDateSelector( strName, nSeq, strText) 
	{	 
		document.write( "<SELECT NAME=\""+strName+"Date"+nSeq+"\"><OPTION VALUE=\"1\"><?=IDS_LABEL_ALL?></OPTION><OPTION VALUE=\"2\"><?=IDS_LABEL_MON?></OPTION><OPTION VALUE=\"3\"><?=IDS_LABEL_TUE?></OPTION><OPTION VALUE=\"4\"><?=IDS_LABEL_WED?></OPTION><OPTION VALUE=\"5\"><?=IDS_LABEL_THU?></OPTION><OPTION VALUE=\"6\"><?=IDS_LABEL_FRI?></OPTION><OPTION VALUE=\"7\"><?=IDS_LABEL_SAT?></OPTION><OPTION VALUE=\"8\"><?=IDS_LABEL_SUN?></OPTION></SELECT>" ); 	
		hSelect = document.getElementById( strName+"Date"+nSeq);
		switch( strText )
		{
			case "ALL":	hSelect.value=1; break;
			case "MON":	hSelect.value=2; break;
			case "TUE":	hSelect.value=3; break;
			case "WED":	hSelect.value=4; break;
			case "THU":	hSelect.value=5; break;
			case "FRI":	hSelect.value=6; break;
			case "SAT":	hSelect.value=7; break;
			case "SUN":	hSelect.value=8; break;
		}
	}

	function Edit( nSeq  ) 
	{
		var nWeather = document.getElementById( "Weather"+ nSeq).value;
		var nDate    = document.getElementById( "Date"+ nSeq).value;
		var nStart	 = document.getElementById( "weathers"+ nSeq).value;
		var nEnd	 = document.getElementById( "weathere"+ nSeq).value;
		hForm.mode.value = 1;		hForm.seq.value = nSeq;		hForm.weather.value = nWeather;
		hForm.date.value = nDate;	hForm.start.value = nStart;	hForm.end.value = nEnd;
		hForm.submit();
	}

	function Delete( nSeq ) 
	{
		hForm.mode.value = 2;		hForm.seq.value = nSeq;
		hForm.submit();
	}

	function Add()
	{
		var nWeather = document.getElementById( "Weather0").value;
		var nDate    = document.getElementById( "Date0").value;	
		var nStart	 = document.getElementById( "weathers").value;
		var nEnd	 = document.getElementById( "weathere").value;
	
		hForm.mode.value = 3;		hForm.seq.value = 0;		hForm.weather.value = nWeather;
		hForm.date.value = nDate;	hForm.start.value = nStart;	hForm.end.value = nEnd;
		hForm.submit();
	}

	function FLUSH()
	{
		hForm.mode.value = 4;
		hForm.submit();
	}

	function Enable(nSeq) { hForm.mode.value = 5;	hForm.seq.value = nSeq;	 hForm.text.value = 1; hForm.submit(); 	}
	function Disable(nSeq)
	{
		hForm.mode.value = 5;		
		hForm.seq.value = nSeq;	
		hForm.text.value = 0;
		hForm.submit();
	}
//-->
</SCRIPT>

<?
	// 페이지 세팅
	$hLayout= new CHtmlForm;
	$dBase	= new CDatabase( CONFIG_DB_HOST, CONFIG_DB_USER, CONFIG_DB_PASS, CONFIG_DB_NAME, true );
	$hPager = new CPager( $_REQUEST["page"], 20,10,"menu=menu_setting_weather&stype=".$_REQUEST["stype"]."&query=".$_REQUEST["query"]); 
	$hLayout->TITLE(IDS_MENU_SETTING_WEATHER);

	if( $_POST["mode"] == EDIT_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetWeatherMgr()->EditWeather( $_POST["seq"], $_POST["weather"], $_POST["start"], $_POST["end"], $_POST["date"] );
		$hLayout->ReplacePage();
	} else if( $_POST["mode"] == DELETE_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetWeatherMgr()->DeleteWeather( $_POST["seq"] );
		$hLayout->ReplacePage();
	} else if( $_POST["mode"] == ADD_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetWeatherMgr()->InsertWeather($_POST["weather"], $_POST["start"], $_POST["end"], $_POST["date"] );
		$hLayout->ReplacePage();
	} else if( $_POST["mode"] == FLUSH_MODE )
	{
		
		$hSocket = new CXKickSocket;
		echo "Connect....";		flush(); 	usleep(200000);  // 잦은 클릭 방지를 위한 Sleep처리
		$strResult = $hSocket->Connect( STS_IP, STS_PORT );
		echo $strResult."!<br>";		flush(); 	usleep(200000);
		if( $strResult  != "OK" ) exit;
		echo "Write(UpdateWeather)...";		flush(); 	usleep(200000);
		$hSocket->UpdateWeather();
		echo "OK!";		flush(); 	usleep(200000);
		$hLayout->ReplacePage();
	} else if( $_POST["mode"] == ENABLE_MODE )
	{
		$hXKick = new CXKickManager;
		$hXKick->GetWeatherMgr()->EnableWeather( $_POST["seq"], $_POST["text"] );
		$hLayout->ReplacePage();
	}

	$strFrom = " tb_game_weather ";
	$nDataCnt = $dBase->GetOne("SELECT count(*) as cnt FROM ".$strFrom.$strWhere);			
	$strQuery = "SELECT weather_seq, ".
				"concat( '<script> ShowSelector(', weather_seq, ',\"' , weather_type, '\") </script>' ), ".
				"concat( '<script> ShowDateSelector( \"\",', weather_seq, ',\"' , weather_date, '\") </script>' ) , ".
				" concat(\"<input type='TEXT' SIZE='8' name='weathers\", weather_seq, \"' value='\", weather_start, \"'>\"),".
				" concat(\"<input type='TEXT' SIZE='8' name='weathere\", weather_seq, \"' value='\", weather_end, \"'>\"), ".
				" concat('<a href=\"javascript:Edit(' , weather_seq,');\">".IDS_LABEL_MODIFY_SHORT."</a>'), concat('<a href=\"javascript:Delete(' , weather_seq,');\">".IDS_LABEL_DEL_SHORT."</a>') ".
					"FROM ".$strFrom." ORDER BY weather_seq DESC LIMIT ".$hPager->GetStart().",".$hPager->GetListLimit().";";
	$Result = $dBase->Query($strQuery);			

	// 출력
	$hLayout->TABLE_SetTitleArray( array(  array( IDS_LABEL_SEQ, 60, "center" ), array(IDS_LABEL_WEATHER, 100, "center"), array(IDS_LABEL_DAY, 100, "center"), array(IDS_LABEL_STARTDATE, 140, "center"), array(IDS_LABEL_ENDDATE, 140, "center"),array(IDS_LABEL_MANAGE_MENU, 40, "center"),array(IDS_LABEL_MANAGE_MENU, 40, "center")	) );
	$hLayout->TABLE_ColorSetting("#282828", "#353535", "#565656" );
	$hLayout->TABLE_Start( 27 );
	$hLayout->TABLE_PrintRes( $Result );
	$hLayout->TABLE_End();
?>
<table width="600">
	<tr>
		<td align="right" height="80">
		<SCRIPT LANGUAGE="JavaScript">
		 <!--
			ShowSelector(0,"WEATHER_FINE");
			document.write( " " );
			ShowDateSelector( "",0,"ALL") ;
		 //-->
		 </SCRIPT> <input type='TEXT' SIZE='8' name='weathers'> <input type='TEXT' SIZE='8' name='weathere'> [<A HREF="#" onclick="Add()" ><?=IDS_LABEL_ADD?></A>] [<A HREF="#" onclick="FLUSH()" ><?=IDS_SERVER_REALTIME_SET?></A>] 
		<br><br>* <?=IDS_SCHEDULE_SETTING_DESC1?>
		</td>
	</tr>
</table>
<?
	$hPager->ShowPageNumber($nDataCnt, CONFIG_DOC_ROOT."image/" );	
?>
