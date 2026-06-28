<?
	require_once( "./class_inc/login.inc" );
	$hLogin		= new CLogin();
	$strUserID  = $hLogin->GetCookie( "admin_name" );
?>
<html>


<head>
<LINK REL=STYLESHEET TYPE="text/css" HREF="<?=CONFIG_DOC_ROOT?>/css/index.css">
<title><?=IDS_SITE_NAME?></title>

<SCRIPT LANGUAGE="JavaScript">
<!--
	function Logout()	{ var hForm = document.getElementById( "FORM_LOGOUT" ); hForm.submit(); }
//-->
</SCRIPT>
</head>
<table border="0" cellpadding="0" cellspacing="0" width="100%">
	<tr>
		<td width="178"><a href="<?=CONFIG_DOC_ROOT?>"><img src="./image/top_logo.gif" border="0" ></A></td>
		<td background="./image/top_line.gif" align="right"><?=sprintf( IDS_MAIN_WELCOME, $strUserID );?> (<?=IDS_SITE_LANGUAGE?>)</td>
		<td width="70" background="./image/top_line.gif" align="right"><img src="./image/B_logout.gif" style="cursor:hand;"  border="0" align="absmiddle" onClick='javascript:Logout();'></td>
		<td width="30" background="./image/top_line.gif" align="right"></td>
	</tr>
</table>
<iframe name="noshow" width="1" height="0"></iframe>
<STYLE>
<!--
.menuBar
{
    POSITION: relative;
    BACKGROUND-COLOR: transparent;
    TEXT-ALIGN: left
}
.Bar
{
    FLOAT: left;
    WIDTH: 100px;
    CURSOR: hand;
    TEXT-INDENT: 5px;
    POSITION: relative;
    TEXT-ALIGN: left
}
.menu
{
	COLOR:#C4C4C4;
	FONT-FAMILY:Tahoma;
    FONT-SIZE: 9pt; 
	filter:progid:DXImageTransform.Microsoft.Shadow(color='#353535', Direction=125, Strength=5);
    VISIBILITY: hidden;
    WIDTH: 150px;
    LINE-HEIGHT: 140%;
    POSITION: absolute;
	BORDER-RIGHT:#222222 1px solid;
	BORDER-BOTTOM:#222222 1px solid;
	BORDER-TOP:#777777 1px solid;
	BORDER-LEFT:#777777 1px solid;

	BACKGROUND-COLOR:#535153
}
.menuItem
{
    PADDING-RIGHT: 15px;
    PADDING-LEFT: 10px;
    PADDING-TOP: 2px;
    WIDTH: 180%;
    CURSOR: hand;
    LINE-HEIGHT: 22px;
    BORDER-BOTTOM:#535153 1px solid;
    BORDER-TOP:#535153 1px solid;
    BORDER-RIGHT:#535153 1px solid;
    BORDER-LEFT:#535153 1px solid;
}


.ItemMouseOver
{
    PADDING-RIGHT: 15px;
    PADDING-LEFT: 10px;
    PADDING-TOP: 2px;
    WIDTH: 180%;
    CURSOR: hand;
	COLOR: white;
    LINE-HEIGHT: 17px;
    BORDER-BOTTOM:#99CCFF 1px solid;
    BORDER-TOP:#99CCFF 1px solid;
    BORDER-RIGHT:#99CCFF 1px solid;
    BORDER-LEFT:#99CCFF 1px solid;
	filter:progid:DXImageTransform.Microsoft.Shadow(color='#353535', Direction=125, Strength=5);
    BACKGROUND-COLOR: #272727
}
.ItemMouseOut
{
    WIDTH: 100%
}
.Arrow
{
    FLOAT: right;
    FONT-FAMILY: Webdings;
    POSITION: absolute;
    TEXT-ALIGN: left;
}
.barOver
{
    FLOAT: left;
    WIDTH: 100px;
    CURSOR: hand;
    TEXT-INDENT: 5px;
    POSITION: relative;
    TEXT-ALIGN: left
}
-->
</STYLE>

<SCRIPT LANGUAGE="JavaScript">
<!--
function OpenWindow(url){	window.open(url,'','menubar=no, scrollbars=yes, status=yes, toolbar=no, resizable=yes, width=800, height=600'); }
 
   function InitMenu()
   {
     var bar = menuBar.children     

     for(var i=0;i < bar.length;i++) 
        {
          var menu=eval(bar[i].menu)
          menu.style.visibility = "hidden"
          bar[i].onmouseover = new Function("ShowMenu("+bar[i].id+")")
          var Items = menu.children
          for(var j=0; j<Items.length; j++)
            {
              var menuItem = eval(Items[j].id)
                
                if(menuItem.menu != null)
                   { 
                    menuItem.innerHTML += "<FONT FACE='Webdings' SIZE='2' >4</FONT>"
                    //var tmp = eval(menuItem.id+"_Arrow")
                    // tmp.style.pixelLeft = menu.getBoundingClientRect().Right //- tmp.offsetWidth - 15
                    FindSubMenu(menuItem.menu)}
               
                 if(menuItem.cmd != null) 
                   {
                   menuItem.onclick = new Function("Do("+menuItem.id+")") } 
                   
              menuItem.onmouseover = new Function("highlight("+Items[j].id+")")
            
            }    
        
        }  
   }
   function FindSubMenu(subMenu)
   {
         var menu=eval(subMenu)
         var Items = menu.children
          for(var j=0; j<Items.length; j++)
            {
              menu.style.visibility = "hidden"
              var menuItem = eval(Items[j].id)
              
              
              if(menuItem.menu!= null)
                 {
                    menuItem.innerHTML += "<FONT FACE='Webdings' SIZE='2' >4</FONT>"
                    // var tmp = eval(menuItem.id+"_Arrow")
                    //tmp.style.pixelLeft = 35 //menuItem.getBoundingClientRect().right - tmp.offsetWidth - 15
                    FindSubMenu(menuItem.menu)
                 }

                 if(menuItem.cmd != null) 
                   {
                   menuItem.onclick = new Function("Do("+menuItem.id+")") } 
                  
              menuItem.onmouseover = new Function("highlight("+Items[j].id+")")
                 
            }  
   } 
   function ShowMenu(obj)
   {
     HideMenu(menuBar)
     var menu = eval(obj.menu)
     var bar = eval(obj.id)
     bar.className="barOver"
     menu.style.visibility = "visible"
     menu.style.pixelTop =  obj.getBoundingClientRect().top + obj.offsetHeight + Bdy.scrollTop
     menu.style.pixelLeft = obj.getBoundingClientRect().left + Bdy.scrollLeft
   }
   
   function highlight(obj)
   {
     var PElement = eval(obj.parentElement.id)
     if(PElement.hasChildNodes() == true)
     {  var Elements = PElement.children
       for(var i=0;i<Elements.length;i++)
       {
          TE = eval(Elements[i].id)
          TE.className = "menuItem"
       }
      } 
     obj.className="ItemMouseOver"
     window.defaultStatus = obj.title
     ShowSubMenu(obj)
   }
   
   function Do(obj)
	{
    var cmd = eval(obj).cmd	
    window.navigate(cmd)
	}
   
   function HideMenu(obj)
   {
          if(obj.hasChildNodes()==true)
		  {  
			 var child = obj.children     
			 
			 for(var j =0;j<child.length;j++)
			  {
				 if (child[j].className=="barOver")
				 {var bar = eval(child[j].id)
				  bar.className="Bar"}
				  
				 if(child[j].menu != null)
				   {
					  var childMenu = eval(child[j].menu)
					  if(childMenu.hasChildNodes()==true) 
						   HideMenu(childMenu)
					  
					   childMenu.style.visibility = "hidden" 
					 }
			  }
		  
		  }
   }
  function ShowSubMenu(obj)
  {
     PMenu = eval(obj.parentElement.id)
      HideMenu(PMenu)
     if(obj.menu != null)
     {
     var menu = eval(obj.menu)
     menu.style.visibility = "visible"
     menu.style.pixelTop =  obj.getBoundingClientRect().top + Bdy.scrollTop
     menu.style.pixelLeft = obj.getBoundingClientRect().right + Bdy.scrollLeft
     if(menu.getBoundingClientRect().right > window.screen.availWidth )
       menu.style.pixelLeft = obj.getBoundingClientRect().left - menu.offsetWidth
    }
  } 

//-->
</SCRIPT>
</HEAD>
<BODY OnLoad="InitMenu()" Onclick="HideMenu(menuBar)" ID="Bdy" topmargin="0" leftmargin="0">
<iframe name="noshow_login" width="0" height="0"></iframe>
<!-- 탑메뉴 시작 -->	  
        <table width="100%" border="0" cellspacing="0" cellpadding="0" bgcolor="#535153">
          <tr>
            <td width="3%">&nbsp;</td>
            <td width="95%" align="left">
			<DIV Id="menuBar" class="menuBar" >
			<DIV Id="Bar1" class="Bar" menu="menu1" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=member';"><img src="<?=CONFIG_DOC_ROOT?>image/menuline.gif" width="13" height="30" align="absmiddle"> &nbsp;<font color="#FFFFFF"><?=IDS_MENU_MEMBER?></font> </DIV>
			<DIV Id="Bar4" class="Bar" menu="menu4"><img src="<?=CONFIG_DOC_ROOT?>image/menuline.gif" width="13" height="30" align="absmiddle"> &nbsp;<font color="#888888"><?=IDS_MENU_STATISTICS?></font> </DIV>
			<DIV Id="Bar5" class="Bar" menu="menu5"><img src="<?=CONFIG_DOC_ROOT?>image/menuline.gif" width="13" height="30" align="absmiddle"> &nbsp;<font color="#888888"><?=IDS_MENU_CLUB?></font> </DIV>
			<DIV Id="Bar6" class="Bar" menu="menu6"><img src="<?=CONFIG_DOC_ROOT?>image/menuline.gif" width="13" height="30" align="absmiddle"> &nbsp;<font color="#ffffff"><?=IDS_MENU_SETTING?></font> </DIV>
			</DIV>
			</td>
            <td width="2%"><!--<img src="/image/menuline.gif" width="13" height="30">//--></td>
          </tr>
      </table>
<!-- 탑메뉴끝 -->	  

<!--메뉴항목을 설정 하세요 -->
<div id="menu1" class="menu">
	<div id="menuItem1_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=member';">
		<?=IDS_MENU_MEMBER_SEARCH?>
	</div>
	<div id="menuItem1_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=member_connectlist';">
		<?=IDS_MENU_MEMBER_CURRENT?>
	</div>
	<div id="menuItem1_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=member_conhistory';">
		<?=IDS_MENU_MEMBER_HISTORY?>
	</div>
	<div id="menuItem1_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=member_create';">
		<?=IDS_MENU_MEMBER_CREATE?>
	</div>
	
</div>



<div id="menu2" class="menu">

</div>

<div id="menu20" class="menu">
<div id="menuItem20_1" class="menuItem"  onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_cs_answer_1'">
		<?=IDS_MENU_CS_ANSWER_1?>
	</div>
<div id="menuItem20_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_cs_answer_2'">
		<?=IDS_MENU_CS_ANSWER_2?>
</div>
<div id="menuItem20_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_cs_answer_3'">
		<?=IDS_MENU_CS_ANSWER_3?>
</div>
<div id="menuItem20_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_cs_answer_4'">
		<?=IDS_MENU_CS_ANSWER_4?>
</div>
<div id="menuItem20_5" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_cs_answer_5'">
		<?=IDS_MENU_CS_ANSWER_5?>
</div>
</div>


<div id="menu3" class="menu">

</div>

<div id="menu94" class="menu">
<div id="menuItem94_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_sale_by_case'">
	<?=IDS_MENU_SALE_BY_CASE?>
</div>
<div id="menuItem94_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_sale_by_day'">
	<?=IDS_MENU_SALE_BY_DAY?>
</div>
<div id="menuItem94_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_sale_by_pg'">
	<?=IDS_MENU_SALE_BY_PG?>
</div>
<div id="menuItem94_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_sale_statistics'">
	<?=IDS_MENU_SALE_STATISTICS?>
</div>
</div>

<div id="menu4" class="menu">
	<div id="menuItem4_1" class="menuItem" menu="menu41">
		<?=IDS_MENU_STATISTICS_B2B?>
	</div>
	<div id="menuItem4_2" class="menuItem" menu="menu92">
		<?=IDS_MENU_STATISTICS_JOIN?>
	</div>
	<div id="menuItem4_3" class="menuItem" menu="menu93">
		<?=IDS_MENU_STATISTICS_CREATE?>
	</div>
	<div id="menuItem4_4" class="menuItem" menu="menu91">
		<?=IDS_MENU_STATISTICS_STU?>
	</div>
	<div id="menuItem4_5" class="menuItem" menu="menu90">
		<?=IDS_MENU_STATISTICS_CON?>
	</div>
	<div id="menuItem4_6" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_statistics_sale'">
		<?=IDS_MENU_STATISTICS_SALE?>
	</div>
</div>


<div id="menu41" class="menu">
<div id="menuItem41_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_statistics_b2b_hit'">
		<?=IDS_MENU_STATISTICS_B2B_HIT?>
</div>
</div>


<div id="menu92" class="menu">
<div id="menuItem92_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_statistics_join1'">
	<?=IDS_MENU_STATISTICS_JOIN1?>
</div>
<div id="menuItem92_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_statistics_join2'">
	<?=IDS_MENU_STATISTICS_JOIN2?>
</div>
<div id="menuItem92_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_statistics_join3'">
	<?=IDS_MENU_STATISTICS_JOIN3?>
</div>
</div>

<div id="menu93" class="menu">
<div id="menuItem93_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_join_by_level'">
	<?=IDS_MENU_JOIN_BY_LEVEL?>
</div>
<div id="menuItem93_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_join_by_date'">
	<?=IDS_MENU_JOIN_BY_DATE?>
</div>
<div id="menuItem93_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_join_by_rank'">
	<?=IDS_MENU_JOIN_BY_RANK?>
</div>
<div id="menuItem93_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_join_create'">
	<?=IDS_MENU_JOIN_CREATE?>
</div>
</div>


<div id="menu91" class="menu">
<div id="menuItem91_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_latest'">
	<?=IDS_MENU_CON_LATEST?>
</div>
<div id="menuItem91_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_average'">
	<?=IDS_MENU_CON_AVERAGE?>
</div>
<div id="menuItem91_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_average_server'">
	<?=IDS_MENU_CON_AVERAGE_SERVER?>
</div>
<div id="menuItem91_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_max'">
	<?=IDS_MENU_CON_MAX?>
</div>
<div id="menuItem91_5" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_max_server'">
	<?=IDS_MENU_CON_MAX_SERVER?>
</div>
<div id="menuItem91_6" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_five_minute'">
	<?=IDS_MENU_CON_FIVE_MINUTE?>
</div>
</div>

<div id="menu90" class="menu">
<div id="menuItem90_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_month'">
	<?=IDS_MENU_CON_MONTH?>
</div>
<div id="menuItem90_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_con_by_level'">
	<?=IDS_MENU_CON_BY_LEVEL?>
</div>
</div>


<div id="menu5" class="menu">

</div>

<div id="menu6" class="menu">
	<div id="menuItem6_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_notice'">
		<?=IDS_MENU_SETTING_NOTICE?>
	</div>
	<div id="menuItem6_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_weather'">
		<?=IDS_MENU_SETTING_WEATHER?>
	</div>
	<div id="menuItem6_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_schedule'">
		<?=IDS_MENU_SETTING_SCHEDULE?>
	</div>
	<div id="menuItem6_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_broadcast'">
		<?=IDS_MENU_SETTING_BROADCAST?>
	</div>
	<div id="menuItem6_5" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_gamestart'">
		<?=IDS_MENU_SETTING_GAME_START?>
	</div>
</div>

<div id="menu7" class="menu">

</div>

<div id="menu70" class="menu">
<div id="menuItem70_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_global_sale1'">
	<?=IDS_MENU_GLOBAL_SALE1?>
</div>
<div id="menuItem70_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_global_sale2'">
	<?=IDS_MENU_GLOBAL_SALE2?>
</div>
<div id="menuItem70_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_global_sale3'">
	<?=IDS_MENU_GLOBAL_SALE3?>
</div>
</div>


<div id="menu66" class="menu">
	<div id="menuItem66_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_contest1'">
		<?=IDS_MENU_SETTING_CONTEST1?>
	</div>
	<div id="menuItem66_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_contest2'">
		<?=IDS_MENU_SETTING_CONTEST2?>
	</div>
	<div id="menuItem66_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_contest3'">
		<?=IDS_MENU_SETTING_CONTEST3?>
	</div>
</div>

<div id="menu64" class="menu">
	<div id="menuItem64_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code1'">
		<?=IDS_MENU_SETTING_CODE1?>
	</div>
	<div id="menuItem64_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code2'">
		<?=IDS_MENU_SETTING_CODE2?>
	</div>
	<div id="menuItem64_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code3'">
		<?=IDS_MENU_SETTING_CODE3?>
	</div>
	<div id="menuItem64_4" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code4'">
		<?=IDS_MENU_SETTING_CODE4?>
	</div>
	<div id="menuItem64_5" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code5'">
		<?=IDS_MENU_SETTING_CODE5?>
	</div>
	<div id="menuItem64_6" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_code6'">
		<?=IDS_MENU_SETTING_CODE6?>
	</div>
</div>

<div id="menu68" class="menu">
<div id="menuItem68_1" class="menuItem"  onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_network1'">
	<?=IDS_MENU_SETTING_NETWORK1?>
</div>
<div id="menuItem68_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_network2'">
	<?=IDS_MENU_SETTING_NETWORK2?>
</div>
</div>



<div id="menu63" class="menu">
<div id="menuItem63_1" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_reward1'">
	<?=IDS_MENU_SETTING_REWARD1?>
</div>
<div id="menuItem63_2" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_reward2'">
	<?=IDS_MENU_SETTING_REWARD2?>
</div>
<div id="menuItem63_3" class="menuItem" onclick="location.href='<?=CONFIG_DOC_ROOT?>?menu=menu_setting_reward3'">
	<?=IDS_MENU_SETTING_REWARD3?>
</div>
</div>

<!------------------------- 여기까지 ---------------------------------->
</BODY>
</HTML>
