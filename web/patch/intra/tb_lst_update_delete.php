<?
include "../include/login.php";
include "../include/db_class.php";
if(!$_POST['svrcode_code']) exit;
$DBC = new MyDB("",FALSE);
for($i=0;$i<sizeof($_POST['list']);$i++)
{
	$path = "../_data_/" . $_POST['svrcode_code']  . $_POST['list'][$i] . ".pat";
	unlink($path);
	$sql_delete = "delete from tb_lst_update where svrcode_code = '" . $_POST['svrcode_code'] . "' and update_version = '" . $_POST['list'][$i] . "'";
	$DBC->query($sql_delete);
}
echo "<script> location.href='tb_lst_update_list.php?svrcode_code=" . $_POST['svrcode_code'] . "'; </script>";
exit;
?>
