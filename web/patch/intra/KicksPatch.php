<?
include "include/database.php";
include "include/mcrypt_class.php";

if($_GET['prc'] == "traffic")
{
	$DBC = new MyDB("kicks",FALSE);
	$memid = $_GET['memid'];
	$cps = $_GET['cps'];
	if($memid && $cps > 0)
	{
		$sql_select = "select cpcust_seq from tb_mst_cpcust where cpcust_memid = '$memid'";
		$DBC->query($sql_select);
		$rows = mysql_fetch_array($DBC->result);
		$cpcust_seq = $rows[0];
		if($cpcust_seq)
		{
			$sql_update = "update tb_gam_whole set whole_netspeed = '$cps' where cpcust_seq = $cpcust_seq";
			$DBC->query($sql_update);
		}
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
	$Localip = array("218.153.79.222","218.145.57.42","218.145.57.51","");

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

	// 최신 버전 목록을 가져온다.
	$split = explode(".",$version);
	$versplit = sprintf("%03d.%03d.%03d.%03d",$split[0],$split[1],$split[2],$split[3]);

	$sql_select = "
	 select update_version, update_filename, update_httppath , update_filesize
		from tb_lst_update 
		where svrcode_code = '$svrcode' ";
	 if(in_array($_SERVER[REMOTE_ADDR], $Localip))
			 $sel_select .= "and update_useyn = 'Y' ";
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