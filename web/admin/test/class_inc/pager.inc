<? 
////////////////////////////////////////////////////////////////////////
// 클래스 : 페이징 클래스
// 제작자 : 조용길
// 날  짜 : 2009.08
////////////////////////////////////////////////////////////////////////

define('PAGER_SEARCH_NUMBER'	,  1 ); 
define('PAGER_SEARCH_STRING'	,  2 ); 
define('PAGER_SEARCH_LEFT'		,  3 ); 
define('PAGER_SEARCH_RIGHT'		,  4 ); 
define('PAGER_SEARCH_BOTH'		,  5 ); 

class CPager 
{ 
	private $m_nTotal		= 0; 
	private $m_nTotalPage	= 0; 
	private $m_nListLimit	= 0; 
	private $m_nPageLimit	= 0; 
	private $m_nStart		= 0; 
	private $m_nEnd			= 0; 
	private $m_nStartPage	= 0; 
	private $m_nEndPage		= 0; 
	private $m_strLink		= ""; 
	private $m_nCurrentPage = 0;
	private $m_arrQuery		= null;
	 
	function CPager( $nCurrentPage, $nListLimit,$nPageLimit,$link) 
	{ 
			if( $nCurrentPage == "" || $nCurrentPage == 0 ) $nCurrentPage = 1;
			$this->m_nCurrentPage = $nCurrentPage; 
			$this->m_nListLimit	  = $nListLimit; 
			$this->m_nPageLimit   = $nPageLimit; 
			$this->m_nStart		  = ($this->m_nCurrentPage-1)*$nListLimit; 
			$this->m_strLink	  = $link; 
	} 

	function GetStart()		{ return $this->m_nStart; }
	function GetEnd()		{ return $this->m_nEnd;   }
	function GetListLimit() { return $this->m_nListLimit; }
	function GetPageLimit()	{ return $this->m_nPageLimit;   }

	function ShowPageNumber( $nDataCount, $strImagePath )  
	{ 
			echo "<table height='50'><tr><td width=\"765\"> ";
			if($nDataCount <= 0) return;

			$this->m_nTotal = $nDataCount; 
			$this->m_nTotalPage = ceil($this->m_nTotal/$this->m_nListLimit); 

			if($this->m_nTotal < $this->m_nListLimit)			$this->m_nEnd = $this->m_nTotal; 
			else if($this->m_nCurrentPage == $this->m_nTotalPage) $this->m_nEnd = $this->m_nTotal - ($this->m_nListLimit * ($this->m_nTotalPage - 1)); 
			else $this->m_nEnd = $this->m_nListLimit; 

			if($this->m_nCurrentPage>$this->m_nTotalPage) exit; 
			$co = intval($this->m_nCurrentPage/$this->m_nPageLimit); 
			if($this->m_nCurrentPage%$this->m_nPageLimit) { $co = $co + 1; } 

			$this->m_nStartPage = (($co - 1) * $this->m_nPageLimit) + 1; 
			$this->m_nEndPage = $this->m_nStartPage + $this->m_nPageLimit - 1; 

			if($this->m_nCurrentPage > $this->m_nPageLimit) { 
				  $pre = $this->m_nEndPage - $this->m_nPageLimit; 
				  echo("<table align=\"center\" width='400'><tr><td width='50' align='center'><a href='$PHP_SELF?$this->m_strLink&page=$pre'><img src='".$strImagePath."b_prev.gif' width='50' height='24' border='0'></a></td>"); 
			} else { 
				  echo("<table align=\"center\"  width='400'><tr><td width='50' align='center'><img src='".$strImagePath."b_prev.gif' width='50' height='24' border='0'></td>"); 
			} 

			if($this->m_nTotalPage < $this->m_nEndPage) { $this->m_nEndPage = $this->m_nTotalPage; } 
			echo "<td align='center'>";
			for($i=$this->m_nStartPage; $i<=$this->m_nEndPage; $i++) 
			{
				if($i != $this->m_nStartPage) echo " | ";
				if($i==$this->m_nCurrentPage) { 
					echo(" <font color='#FF9428'><b>$i</b></font>"); 
				} else { 
					echo(" <a href='$PHP_SELF?$this->m_strLink&page=$i' class='list'> $i </a>"); 
				} 
			} 
			echo "</td>";

			if($this->m_nEndPage < $this->m_nTotalPage) { 
				  $next = $this->m_nEndPage + 1; 
				  echo("<td width='31' align='center'><a href='$PHP_SELF?$this->m_strLink&page=$next'><img src='".$strImagePath."b_next.gif' width='50' height='24' border='0'></a></td></tr></table>"); 
			} else { 
				  echo("<td width='31' align='center'><img src='".$strImagePath."b_next.gif' width='50' height='24' border='0'></td></tr></table>"); 
			} 
			echo "</td></tr></table>";
	} 

	function SetQueryFormArray( $arrQuery )
	{
		$this->m_arrQuery = $arrQuery;
	}

	function GetQueryString(  $nSearchField, $strSearch )
	{
		$nArrSize = sizeof($this->m_arrQuery);
		$strWhere  = "";
		for( $i = 0; $i < $nArrSize; $i++ )
		{
			$nType  = $this->m_arrQuery[$i][1];
			$strField = $this->m_arrQuery[$i][0];

			if( $nSearchField == $strField && $strSearch != "" )
			{
				switch( $nType ) 
				{
					case PAGER_SEARCH_NUMBER:	$strWhere = " ".$strField." = ".$strSearch." "; break;
					case PAGER_SEARCH_STRING:	$strWhere = " ".$strField." = '".$strSearch."' "; break;
					case PAGER_SEARCH_LEFT:		$strWhere = " ".$strField." LIKE '".$strSearch."%' "; break;
					case PAGER_SEARCH_RIGHT:	$strWhere = " ".$strField." LIKE '%".$strSearch."' "; break;
					case PAGER_SEARCH_BOTH:		$strWhere = " ".$strField." LIKE '%".$strSearch."%' "; break;
				}
			}
		}
		return $strWhere;	
	}

	function ShowQueryForm( $strSearch, $nSearchField, $strAction, $strImagePath )
	{
		$nArrSize = sizeof($this->m_arrQuery);

		echo	"		<table height='50'><tr><td width='765'>";
		echo	"							<table width='312' border='0' align='center' cellpadding='0' cellspacing='0'>";
		echo	"<form name=\"fm\" method=\"post\" action=".$strAction.">";
		echo	"							  <tr>";
		echo	"								<td width='67'><select name='stype'>";
		for( $i = 0; $i < $nArrSize; $i++ )
		{
			echo	"<option value='".$this->m_arrQuery[$i][0]."' ";
			if($nSearchField == $this->m_arrQuery[$i][0]) echo "selected"; 
			echo ">";
			echo $this->m_arrQuery[$i][2];
			echo "</option>";
		}

		echo	"								</select></td>";
		echo	"								<td width='195' align='center'><input name='query' type='text' value='".$strSearch."' size='25' maxlength='25' style='height:15pt'></td>";
		echo	"								<td width='50'><input type='image' src='". $strImagePath."b_search.gif' width='50' height='24' border='0'></td>";
		echo	"							  </tr>";
		echo	"							  </form>";
		echo	"							</table>";
		echo	"		</td></tr></table>";
	}

} 

?> 
