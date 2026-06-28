<?
include "../include/login.php";
include "../include/db_class.php";

$DBC = new MyDB("",FALSE);
if($_POST['save'] == "yes")
{
	$update_version = $_POST['update_version'];
	$svrcode_code = $_POST['svrcode_code'];

	if($_FILES['httpfile']['name'])
	{
		 $ver = explode(".",$update_version); 
		$path = "../../" . $svrcode_code . "/";
		$update_filename = $_FILES['httpfile']['name'];
		if(move_uploaded_file($_FILES['httpfile']['tmp_name'],$path.$update_filename))
		{
			$update_httppath .= "/patch/" . $svrcode_code . "/" . $update_filename; 
		}
		echo "<script> top.location.href='tb_lst_update_list.php'; </script>";
		exit;
	}
}

?>
