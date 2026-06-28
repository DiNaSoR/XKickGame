<? 
class Page 
{ 
        var $Total = ""; 
        var $Page_Total = ""; 
        var $Llimit = ""; 
        var $Plimit = ""; 
        var $Start = ""; 
        var $End = ""; 
        var $Start_page = ""; 
        var $End_page = ""; 
        var $Links = ""; 
         
        function Page($list_limit,$page_limit,$link) 
        { 
                GLOBAL $page; 
     
                $this->Llimit = $list_limit; 
                $this->Plimit = $page_limit; 
                $this->Start = ($page-1)*$list_limit; 
                $this->Links = $link; 
        } 

        function Print_Page($total) 
        { 
                GLOBAL $page; 
				if($total <= 0) return;

                $this->Total = $total; 
                $this->Page_Total = ceil($this->Total/$this->Llimit); 
                if($this->Total < $this->Llimit) { 
                        $this->End = $this->Total; 
                } 
                else if($page == $this->Page_Total) { 
                        $this->End = $this->Total - ($this->Llimit * ($this->Page_Total - 1)); 
                } 
                else { 
                        $this->End = $this->Llimit; 
                } 
                if($page>$this->Page_Total) { 
                        //echo("<script>location.replace(\"$PHP_SELF?$this->Links&page=1\");</script>"); 
                        exit; 
                } 
                 
                $co = intval($page/$this->Plimit); 
                if($page%$this->Plimit) { $co = $co + 1; } 

                $this->Start_page = (($co - 1) * $this->Plimit) + 1; 
                $this->End_page = $this->Start_page + $this->Plimit - 1; 

                if($page > $this->Plimit) { 
                      $pre = $this->End_page - $this->Plimit; 
                      echo("<a href='$PHP_SELF?$this->Links&page=$pre'>[이전]</a>   "); 
                } else { 
                      echo("<font color='708090'>[이전]</font>   "); 
                } 

                if($this->Page_Total < $this->End_page) { $this->End_page = $this->Page_Total; } 

                if($this->Total > 0) { echo(" <font color='4b4b4b'></font>"); } else { echo(""); } 

                for($i=$this->Start_page; $i<=$this->End_page; $i++) { 
                      if($i==$page) { 
                              echo(" <b><font color='blue'>[ $i ]</font></b>"); 
                      } else { 
                              echo(" <a href='$PHP_SELF?$this->Links&page=$i' class='list'>[ $i ]</a> "); 
                      } 
                } 

                if($this->End_page < $this->Page_Total) { 
                      $next = $this->End_page + 1; 
                      echo("   <a href='$PHP_SELF?$this->Links&page=$next'>[다음]</a>"); 
                } else { 
                      echo("   <font color='708090'>[ 다음 ]</font>"); 
                } 

        } 

} 

?> 
