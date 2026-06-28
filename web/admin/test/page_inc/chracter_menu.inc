<STYLE>
.OverMenu
{
    PADDING-RIGHT: 15px;
    PADDING-TOP: 5px;
    PADDING-BOTTOM: 0px;
	COLOR: white;
    LINE-HEIGHT: 20px;
    BORDER-TOP:#878787 1px solid;
    BORDER-RIGHT:#878787 1px solid;
    BORDER-LEFT:#878787 1px solid;
    BACKGROUND-COLOR: #474747
}

.NormalMenu
{
    PADDING-RIGHT: 15px;
    PADDING-TOP: 5px;
    PADDING-BOTTOM: 0px;
	COLOR: white;
    LINE-HEIGHT: 20px;
    BORDER-BOTTOM:#272727 1px solid;
    BORDER-TOP:#272727 1px solid;
    BORDER-RIGHT:#272727 1px solid;
    BORDER-LEFT:#272727 1px solid;
    BACKGROUND-COLOR: #272727
}

</STYLE>
<?
	if( $nMenu == "" ) $nMenu = 0;
	for( $i = 0; $i < 8; $i++ ) $arrMenu[$i] = "NormalMenu";
	$arrMenu[$nMenu] = "OverMenu";
?>
<table width="95%" border=0 cellpadding=0 cellspacing=0 >
<tr>
	<td align="left">
	<table width="640" height="25" border=0 cellpadding=1 cellspacing=1 align="left">
	<tr>
		<td width="80" class="<?=$arrMenu[0]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterdetail&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_DETAIL_TITLE?></a></td>
		<td width="80" class="<?=$arrMenu[1]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterrecord&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_RECORD_TITLE?></a></td>
		<td width="80" class="<?=$arrMenu[2]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characteritem&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_ITEM?></a></td>
		<td width="80" class="<?=$arrMenu[3]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=charactertraining&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_TRAINING?></a></td>
		<td width="80" class="<?=$arrMenu[4]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterskill&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_SKILL?></a></td>
		<td width="80" class="<?=$arrMenu[5]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterceremoney&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_CEREMONY?></a></td>
		<td width="80" class="<?=$arrMenu[6]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterfriend&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_FRIEND?></a></td>
		<td width="80" class="<?=$arrMenu[7]?>" align="center"><a href="<?=CONFIG_DOC_ROOT?>?menu=characterblack&seq=<?=$_GET["seq"]?>"><?=IDS_CHARACTER_BLACKLIST?></a></td>
	</tr>
	</table>
	</td>
</tr>
</table>