<?
	header( "content-type:text/html;charset=utf-8");

if (!function_exists('xkick_env')) {
	function xkick_env($name, $default)
	{
		$value = getenv($name);
		return ($value === false || $value === '') ? $default : $value;
	}
}

// �⺻����
define('CONFIG_SITE_NAME'	, 'XKICK');
define('CONFIG_SITE_URL'	, xkick_env('XKICK_ADMIN_SITE_URL', 'localhost'));
define('CONFIG_AREA'		, 'ENTERPLAY');
define('CONFIG_DB_HOST'     , xkick_env('XKICK_DB_HOST', '127.0.0.1'));
define('CONFIG_DB_USER'     , xkick_env('XKICK_DB_USER', 'xkick'));
define('CONFIG_DB_PASS'		, xkick_env('XKICK_DB_PASS', 'xkick_dev_password'));
define('CONFIG_DB_NAME'		, xkick_env('XKICK_DB_NAME', 'xkick'));
define('CONFIG_DB_LOG_NAME'	, xkick_env('XKICK_DB_LOG_NAME', 'xkick_log'));
define('CONFIG_CLASS_PATH'	, dirname(__FILE__)."/");
define('CONFIG_DOC_ROOT'	, '/xkickadmin/');

define('STS_IP'				, xkick_env('XKICK_STS_HOST', '127.0.0.1'));
define('STS_PORT'			, xkick_env('XKICK_STS_PORT', '3001'));

// ��� ����
$DB = mysql_connect(CONFIG_DB_HOST,CONFIG_DB_USER,CONFIG_DB_PASS);

mysql_select_db(CONFIG_DB_NAME,$DB);
mysql_query("SET NAMES utf8;" );
$Result = mysql_query("SELECT * FROM tb_game_player" );


$rows = mysql_fetch_array($Result, MYSQL_BOTH );
echo $rows[0];
echo ",";
echo $rows[1];
echo ",";
echo $rows[2];
echo ",";
echo $rows[3];
echo ",";
echo $rows[4];
echo ",";
echo $rows[5];

$Result = mysql_query("show session variables like 'char%'");
while($rows = mysql_fetch_array($Result, MYSQL_BOTH ))
{
	echo $rows[0];
	echo ",";
	echo $rows[1];
	echo ",";
	echo "<BR>";
}


?>
