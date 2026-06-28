<?
include "../include/login.php";
include "../include/db_class.php";
include "../include/paging.php";

if(!$svrcode_code)	$svrcode_code = "kicksonline";
$DBC = new MyDB("",FALSE);
$sql_select = "select count(*) from tb_lst_update where svrcode_code = '$svrcode_code'";
$DBC->query($sql_select);
$rows = mysql_fetch_array($DBC->result);
$total_num = $rows[0];
if(!$page) $page = 1; 
$Pages = new Page(15,10,"svrcode_code=$svrcode_code"); 

$sql_select = "select update_version, svrcode_code, update_filename, update_httppath, update_inputdate,update_useyn,update_etc from tb_lst_update where svrcode_code = '$svrcode_code' order by update_inputdate desc LIMIT $Pages->Start,$Pages->Llimit";
$DBC->query($sql_select);

?>
<script>
function CheckChg(ver,parm)
{
	fm.action="tb_lst_update_modify.php?ver=" + ver + "&parm=" + parm;
	fm.submit();
}
function DoModify(v,s)
{
	
	var length = document.fm.etc.length;
	if(!length) length = 1;
	for(i=0;i<length;i++)
	{
		
		if(i==s)
		{			
			if(length == 1)
			{
				fm.action="tb_lst_update_modify.php?version=" + v + "&parm=" + document.fm.etc.value;
				fm.submit();
				break;
			}
			else
			{
				fm.action="tb_lst_update_modify.php?version=" + v + "&parm=" + document.fm.etc[i].value;
				fm.submit();
				break;
			}
		}
	}

}
function DoDelete()
{
	fm.action="tb_lst_update_delete.php";
	fm.submit();
}

function DoInsert()
{
	fm.action="tb_lst_update_insert.php";
	fm.submit();
}

</script>
<LINK REL=STYLESHEET TYPE="text/css" HREF="style.css">
	
	<table border="0" cellpadding="2" cellspacing="1" width="782">
		<tr>
			<td>
			<form name="fm" method="post" action="<?=$PHP_SELF?>">
	<input type="hidden" name="svrcode_code" value="<?=$svrcode_code?>">
			  <table width="775" border="0" cellspacing="0" cellpadding="0" background="_img/img_dsearch01.gif" >
			  <tr>
				<td bgcolor="#EEEEEE" height="25"  align=left width=190>&nbsp; <img src="_img/top_kdaq.gif" align="absmiddle"> <b>Kicks Patch</b> </td>
				<td bgcolor=white>&nbsp;<U></td>
			  </tr>
			  </table>
			
			
			<table width="775" border=0 cellpadding=0 cellspacing=1 bgcolor="#CCCCCC" align="center">
			<tr height="25" align="left" bgcolor="#EEEEEE">
				<td align="center" width=2%>C</td>
				<td align="center" width=8%>Version</td>
				<td align="center" width=12%>Filename</td>
				<td align="center" width=12%>Date</td>
				<td align="center" width=5%>Use</td>
				<td align="center" width=18%>Etc</td>
				<td align="center" width=5%>Edit</td>
			</tr>
			<?
			$count = 0;
			
			while($rows = mysql_fetch_array($DBC->result))
			{ 
				$filename = "/www/patch/" . $svrcode_code . "/" . $rows[2]; 

			?>
				<tr  bgcolor="#FFFFFF" align="center">
					<td><input type="checkbox" name="list[]" value="<?=$rows[0]?>"</td>
					<td><B><?=$rows[0]?></B></td>
					<td><a href="<? echo $rows[3]?>"><?=$rows[2]?></a></td>					
					<td><?=$rows[4]?></td>
					<td style="background:#FFF8F2;"><u><a href="javascript:CheckChg('<?=$rows[0]?>','<?=$rows[5]?>')" ><?=$rows[5]?></u></a>
					</select>
					</td>
					<td align=left>&nbsp;<input type="text" name="etc" size=45 maxlength=50 value="<?=$rows[6]?>"></td>
					<td><a href="javascript:DoModify('<?=$rows[0]?>',<?=$count++?>)"><img  src="_img/modify.gif" border=0 align="absmiddle"></a></td>
				</tr>
			<?
			}
			?>
			</table>
			</form>
		</tr>
	
		<tr>
			<td align=center>
				<?  $Pages->Print_Page($total_num);	?>
			</td>
		</tr>
		<tr>
			<td align=right>
			<a href="javascript:DoInsert()"><img  src="_img/write.gif" border=0 align="absmiddle"></a><a href="javascript:DoDelete()"><img  src="_img/delete.gif" border=0 align="absmiddle"></a>
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
