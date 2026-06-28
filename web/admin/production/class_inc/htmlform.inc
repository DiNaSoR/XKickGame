<?
////////////////////////////////////////////////////////////////////////
// 클래스 : 테이블 위주의 페이지를 구성할때 사용하는 HTML Generator
// 제작자 : 조용길
// 날  짜 : 2009.08
////////////////////////////////////////////////////////////////////////
class CHtmlForm
{
	private $m_nTitleCnt    = 0;
	private $m_nTitleArr    = null;
	private $m_nTitleColor  = "#282828";
	private $m_nContensColor= "#353535";
	private $m_nBorderColor = "#565656";
	// 제목 출력
	function TITLE( $strTitle )
	{
		echo "
				<table class =\"HTMLFORM_TITLE_TABLE\" >
				  <tr>
					<td width=\"27\">&nbsp;</td>
					<td width=\"765\"><img src=\"".CONFIG_DOC_ROOT."image/icon.gif\" width=\"11\" height=\"11\" align=\"absmiddle\"> <span class=\"title_text\">".$strTitle."</span></td>
				  </tr>
				</table>	
			";
	}

	function TABLE_ColorSetting( $nTitleColor, $nContensColor, $nBorderColor )
	{
		$this->m_nTitleColor  = $nTitleColor;
		$this->m_nContensColor= $nContensColor;
		$this->m_nBorderColor = $nBorderColor;
	}

	function TABLE_Start( $nLeft = 0)
	{
		$nTableSize = 0;
		for( $i = 0; $i < $this->m_nTitleCnt; $i++ ) $nTableSize+=$this->m_nTitleArr[$i][1];

		echo "
				<table width=\"".$nTableSize."\" height=\"30\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">
				  <tr>";
		if( $nLeft > 0 ) echo "<td width=\"".$nLeft."\">&nbsp;</td>";
		echo "
						</td>
					<td>
					<table border=\"0\" cellpadding=\"3\" cellspacing=\"1\" bgcolor=\"".$this->m_nBorderColor."\">
			";

		echo "<tr bgcolor=\"".$this->m_nTitleColor."\">";
		for( $i = 0; $i < $this->m_nTitleCnt; $i++ )
		{
                  echo "<td width=\"".$this->m_nTitleArr[$i][1]."\" height=\"24\" align=\"".$this->m_nTitleArr[$i][2]."\"><span class=\"table_title\">".$this->m_nTitleArr[$i][0]."</span></td>";
		}
		echo "</tr>";

	}

	// 타이틀 정의
	function TABLE_SetTitleArray( $strArr )
	{
		$this->m_nTitleCnt = sizeof($strArr);
		$this->m_nTitleArr = $strArr;
	}

	// 값 추가 (바로 출력됨)
	function TABLE_PrintData( $strArr )
	{
		$nBackground = $this->m_nContensColor;
        echo "<tr bgcolor=\"".$nBackground ."\">";
		for( $i = 0; $i < $this->m_nTitleCnt; $i++ )
		{
			if( $this->m_nTitleArr[$i][3] ) $nBackground =$this->m_nTitleArr[$i][3];
			else							$nBackground = $this->m_nContensColor;
			echo "<td style=\"background:".$nBackground .";\" width=\"".$this->m_nTitleArr[$i][1]."\" height=\"20\" align=\"".$this->m_nTitleArr[$i][2]."\">".$strArr[$i]."</span></td>";
		}
		echo "</tr>";

	}

	// MYSQL 리소스를 출력
	function TABLE_PrintRes( $hResource )
	{
		while( $rows = mysql_fetch_array($hResource ) ) $this->TABLE_PrintData( $rows );
	}

	function TABLE_End()
	{
		echo "	</table>
				</td>
			  </tr>
			</table>
		";
	}

	function ReplacePage()
	{
		echo "<script>document.location.replace('".$_SERVER["REQUEST_URI"]."');</script>";
		exit;
	}

	function ShowDateForm( $nYear, $nMonth, $strAction )
	{
		echo "
				<table class =\"HTMLFORM_TITLE_TABLE\" >
				  <tr>
					<td width=\"27\">&nbsp;</td>
					<td width=\"765\">
					";
		
		ECHO "<FORM name=\"datefm\" method=\"post\"  action='".$strAction."'><SELECT id='FM_YEAR' name='FM_YEAR'>";
		for( $i = 2009; $i < 2013; $i++ )
		{
			ECHO "<OPTION VALUE='".$i."' ";
			IF( $i == $nYear ) echo "SELECTED";	
			ECHO ">".$i;
		}
		ECHO "</SELECT>년 ";
		ECHO "<SELECT id='FM_MONTH' name='FM_MONTH'>";
		for( $i = 1; $i < 13; $i++ )
		{
			ECHO "<OPTION VALUE='".$i."' ";
			IF( $i == $nMonth ) echo "SELECTED";	
			ECHO ">".$i;
		}
		ECHO "</SELECT>월 <input type='submit' value='검색' ></FORM>";

		ECHO  "					</td> 
				  </tr>
				</table>	
			";

	}
}
?>