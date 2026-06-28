<?
	$strPage = $_GET["menu"];
	if(  $hLogin->GetCookie( "admin_level" ) == 1 ) 
	{
		switch(  $strPage )
		{
			case "":							include "./page_inc/main.inc"; break;
			case "member":						include "./page_inc/member.inc"; break;
			case "memberdetail":				include "./page_inc/memberdetail.inc"; break;
			case "characterdetail":				include "./page_inc/characterdetail.inc"; break;
			case "characterrecord":				include "./page_inc/characterrecord.inc"; break;
			case "characteritem":				include "./page_inc/characteritem.inc"; break;
			case "charactertraining":			include "./page_inc/charactertraining.inc"; break;
			case "characterskill":				include "./page_inc/characterskill.inc"; break;
			case "characterceremoney":			include "./page_inc/characterceremoney.inc"; break;
			case "member_connectlist":			include "./page_inc/member_connectlist.inc"; break;
			case "member_create":				include "./page_inc/member_create.inc"; break;
			case "menu_setting_notice":			include "./page_inc/menu_setting_notice.inc"; break;
			case "menu_setting_weather":		include "./page_inc/menu_setting_weather.inc"; break;
			case "menu_setting_schedule":		include "./page_inc/menu_setting_schedule.inc"; break;
			case "menu_setting_broadcast":		include "./page_inc/menu_setting_broadcast.inc"; break;
			case "member_connectlist":			include "./page_inc/member_connectlist.inc"; break;
			case "member_conhistory":			include "./page_inc/member_conhistory.inc"; break;
			case "menu_setting_gamestart":			include "./page_inc/menu_setting_gamestart.inc"; break;
			case "menu_con_five_minute":			include "./page_inc/menu_con_five_minute.inc"; break;
			case "menu_join_by_rank":					include "./page_inc/menu_join_by_rank.inc"; break;
			default: echo "<br><br><br><br><center>".IDS_PAGE_NOT_FOUND."<br><br><br><br><center>";
		}
	} else
	{
		switch(  $strPage )
		{
			case "":  
	//		case "member":						include "./page_inc/member.inc"; break;
			case "member_connectlist":			include "./page_inc/member_connectlist.inc"; break;
			case "member_conhistory":			include "./page_inc/member_conhistory.inc"; break;
			
	/*
			case "memberdetail":				include "./page_inc/memberdetail.inc"; break;
			case "characterdetail":				include "./page_inc/characterdetail.inc"; break;
			case "characterrecord":				include "./page_inc/characterrecord.inc"; break;
			case "characteritem":				include "./page_inc/characteritem.inc"; break;
			case "charactertraining":			include "./page_inc/charactertraining.inc"; break;
			case "characterskill":				include "./page_inc/characterskill.inc"; break;
			case "characterceremoney":			include "./page_inc/characterceremoney.inc"; break;
	//		case "member_create":				include "./page_inc/member_create.inc"; break;
			case "menu_setting_notice":			include "./page_inc/menu_setting_notice.inc"; break;
			case "menu_setting_weather":		include "./page_inc/menu_setting_weather.inc"; break;
			case "menu_setting_schedule":		include "./page_inc/menu_setting_schedule.inc"; break;
			
		*/
		case "menu_setting_broadcast":		include "./page_inc/menu_setting_broadcast.inc"; break;
			default: echo "<br><br><br><br><center>".IDS_PAGE_NOT_FOUND."<br><br><br><br><center>";
		}
	}
?>