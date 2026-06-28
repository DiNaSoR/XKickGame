<?
$svrcode_code = $_SERVER['PHP_AUTH_USER'];
if(!$svrcode_code) {
	echo "Login failed\n";
	exit;
}
?>
