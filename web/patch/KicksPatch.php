<?
include "include/database.php";
include "include/mcrypt_class.php";

if($_GET['prc'] == "traffic")
{
	$DBC = new MyDB("kicks",FALSE);
	$cps = $_GET['cps'];
	$sql_select = "select speed_seq from tb_gam_speed where speed_ipaddr = '" . $_SERVER['REMOTE_ADDR'] . "'";
	$DBC->query($sql_select);
	$rows = mysql_fetch_array($DBC->result);
	$speed_seq = $rows[0];
	if($speed_seq)
	{
		$sql_update = "update tb_gam_speed set speed_bps = '$cps' where speed_seq = " . $speed_seq . "";
		$DBC->query($sql_update);
	}
	else
	{
		$sql_insert = "insert into tb_gam_speed (speed_seq, speed_bps, speed_ipaddr) values (0, " . $cps . ",'" . $_SERVER['REMOTE_ADDR'] . "')";
		$DBC->query($sql_insert);
	}
}

else if($_GET['prc'] == "account")
{
	$DBC = new MyDB("kicks",FALSE);
	if($_GET['Account'])
	{
		echo decrypt($_GET['Account']);
	}
}

else
{
	$DBC = new MyDB("entupdate",FALSE);
	$svrcode = "kicksonline";
	$version = $_GET['version'];
	// Set Admin IP
	$Localip = array("202.130.88.218","218.188.201.43","218.153.79.222","218.145.57.42","218.145.57.51","121.134.66.169","121.134.66.248","202.130.88.218");
	$sql_select = "select update_version from tb_lst_update where svrcode_code = '$svrcode' order by update_ver_first,update_ver_middle,update_ver_last,update_ver_etc desc limit 1";
	$DBC->query($sql_select);
	$rows = mysql_fetch_array($DBC->result);	
	$release_ver = $rows[0];
	if(!$release_ver) {
		echo "ERR|Not found the patch data on the list";
		exit;
	}
	if(strncmp($release_ver,$version,1))
	{
		echo "ERR|Have to download latest version from web page";
		exit;
	}

	if($release_ver == $version) {
		echo "SUCC|None";
		exit;
	}

	$split = explode(".",$version);
	$versplit = sprintf("%03d.%03d.%03d.%03d",$split[0],$split[1],$split[2],$split[3]);

	$sql_select = "
	 select update_version, update_filename, update_httppath , update_filesize
		from tb_lst_update 
		where svrcode_code = '$svrcode' ";
	 if(!in_array($_SERVER[REMOTE_ADDR], $Localip))
	 	$sql_select .= "and update_useyn = 'Y' ";
	 $sql_select .= "
			  and concat(update_ver_first,'.',update_ver_middle,'.',update_ver_last,'.',update_ver_etc) > '$versplit'
			order by update_inputdate asc 
		";
	 $DBC->query($sql_select);
	 $total = mysql_num_rows($DBC->result);
	 if($total <= 0)
	 {
		 echo "SUCC|None";
		 exit;
	 }
	 else
	 {
		 echo  "SUCC" . "|" . $total  . "|";
		 while($rows = mysql_fetch_array($DBC->result))
		{
			 echo $rows[0] . "," .  $rows[1] . "," . $rows[2] . "," . $rows[3] . ";";
		}
		
	 }
}
?>
