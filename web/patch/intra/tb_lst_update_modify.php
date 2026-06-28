<?
include "../include/login.php";
include "../include/db_class.php";
$DBC = new MyDB("",FALSE);
$ver = $_GET['ver'];
$parm = $_GET['parm'];
$svrcode_code = $_POST['svrcode_code'];
$version = $_GET['version'];
if($ver && $parm)
{
	if($parm == "Y") $useyn = "N";
	else $useyn = "Y";
	$sql_select = "select UNIX_TIMESTAMP(update_inputdate),UNIX_TIMESTAMP(NOW()) from tb_lst_update where svrcode_code = '$svrcode_code' and  update_version='$ver'";
	$DBC->query($sql_select);
	$row = mysql_fetch_array($DBC->result);
	$sql_update = "update tb_lst_update set update_useyn = '$useyn' where svrcode_code = '$svrcode_code' and  update_version='$ver'";
	$DBC->query($sql_update);
}
else
{
	$sql_update = "update tb_lst_update set update_etc = '$parm' where svrcode_code = '$svrcode_code' and update_version='$version'";
	$DBC->query($sql_update);
}
echo "<script> top.location.href='tb_lst_update_list.php?svrcode_code=$svrcode_code'; </script>";
?>
