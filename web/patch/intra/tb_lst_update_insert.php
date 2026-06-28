<?
include "../include/login.php";
include "../include/db_class.php";

$DBC = new MyDB("",FALSE);
if($_POST['save'] == "yes")
{
	
	if($_FILES['httpfile']['name'])
	{
		$filesize = $_FILES['httpfile']['size'];
		$update_version = $_POST['update_version'];
		$svrcode_code = $_POST['svrcode_code'];
		$update_httppath = $_POST['update_httppath'];
		$update_etc = $_POST['update_etc'];

		$split = explode(".",$update_version); 
		$ver1 = sprintf("%03d",$split[0]);
		$ver2 = sprintf("%03d",$split[1]);
		$ver3 = sprintf("%03d",$split[2]);
		$ver4 = sprintf("%03d",$split[3]);
		$path = "../_data_/";
		$update_filename = $svrcode_code . $update_version . ".pat";
		
		if(move_uploaded_file($_FILES['httpfile']['tmp_name'],$path.$update_filename))
		{
			chmod($path.$update_filename,0777);
			$update_httppath .= "/" . $update_filename; 
			$sql_insert = "insert into tb_lst_update (update_version,svrcode_code,update_filename,update_httppath,update_inputdate,update_ver_first,
			update_ver_middle,update_ver_last,update_ver_etc,update_etc, update_filesize)
			values('$update_version','$svrcode_code','$update_filename','$update_httppath',now(),
				'$ver1','$ver2','$ver3','$ver4','$update_etc',$filesize)";
			$DBC->query($sql_insert);
		
			echo "<script> top.location.href='tb_lst_update_list.php?svrcode_code=$svrcode_code'; </script>";
			
		exit;
		}
		echo "<script> alert('Upload Failed[" . $path.$update_filename . "]'); top.location.href='tb_lst_update_list.php?svrcode_code=$svrcode_code'; </script>";
		exit;
	}
}
$sql_select = "select update_version from tb_lst_update where svrcode_code = '$svrcode_code' order by update_inputdate desc";
$DBC->query($sql_select);
$row = mysql_fetch_array($DBC->result);
$last_version = $row[0];
?>
<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8">
        <title>KicksOnline Patch</title>
	<LINK REL=STYLESHEET TYPE="text/css" HREF="style.css">
	<script language=javascript src="/default.js"></script>
    </head>
    <body bgcolor="white" text="black" link="blue" vlink="purple" alink="red" leftmargin="3" marginwidth="3" topmargin="0" marginheight="0" id="Bdy">
	<form name="fm" action="<?=$PHP_SELF?>" method="post" ENCTYPE="multipart/form-data" >
<input type="hidden" name="tp" value="insert">
<input type="hidden" name="save" value="yes">
<input type="hidden" name="svrcode_code" value="<?=$svrcode_code?>">
	<table border="0" cellpadding="2" cellspacing="1" width="782">
		<tr>
			<td width="775">
	</td>
		</tr>
		<tr>
			<td>
			  <table width="775" border="0" cellspacing="0" cellpadding="0" background="_img/img_dsearch01.gif" >
			<tr>
				<td bgcolor="#EEEEEE" height="25"  align=left width=110>&nbsp; <img src="_img/top_kdaq.gif" align="absmiddle"> <b>Patch Insert</b> </td>
				<td bgcolor=white>&nbsp;</td>
			  </tr>
			  </table>
			  <table width="100%" border=0 cellpadding=0 cellspacing=1 bgcolor="#CCCCCC" align="center">
	<tr height="20" align="left" bgcolor="#EEEEEE">
				<td height=140 bgcolor=white >
						<table width="100%" border=0 cellpadding=0 cellspacing=1 bgcolor="white" align="center">
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="_img/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><b>Code</b></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
							&nbsp;<?=$svrcode_code?>
						</td>				
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="_img/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><b>Version</b></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
						&nbsp;<input type="text" name="update_version" value="" style='font-size:12' size=16> ex) 1.0.0.1
						</td>
						
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="_img/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><b>Http</b></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
						&nbsp;<input type="text" name="update_httppath" value="http://kickspatch.kicks-online.com/_data_/" style='font-size:12' size=60>
						</td>
						
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="_img/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><b>File</b></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
						&nbsp;<input type="file" name="httpfile" size=60>
						</td>
						
						</tr>
						<tr>
						<td width="135">
							  <table width="135" border="0" cellspacing="0" cellpadding="0">
								<tr> 
								<td>&nbsp;</td>
								  <td height="25" width="130" background="_img/bg_box02.gif" valign="bottom"> 
									<table border="0" cellspacing="0" cellpadding="0">
									  <tr> 
										<td width="15"> </td>
										<td height="24"><b>Etc</b></td>
									  </tr>
									</table>
								  </td>
								</tr>
							  </table>
							</td>
						</td>
						<td>
						&nbsp;<input type="text" name="update_etc" size=70>
						</td>
						
						</tr>
						</table>
				</td>
			</tr>
			</table>
</tr>
		<tr>
		<td align=right>
			<a href="tb_lst_update_list.php?tp=list&svrcode_code=<?=$svrcode_code?>"><img src="_img/list.gif" border=0 ></a>
			
			<input type="image" src="_img/write.gif" border=0 >
		</td>
		</tr>
			<tr>
				<td width="776" height="47" colspan="3">
					<table border="0" cellpadding="0" cellspacing="0" width="775" bgcolor="#99BA09">
						<tr>
							<td width="775">
								<p align="right">
									
								</p>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
		</form>
	</body>
	</html>
