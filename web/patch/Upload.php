<?
if($_FILES['httpfile'])
{
        unlink($_FILES['httpfile']['tmp_name']);
}
?>
