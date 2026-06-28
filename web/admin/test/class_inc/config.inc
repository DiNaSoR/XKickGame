<?
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
define('CONFIG_DOC_ROOT'	, '');

define('STS_IP'				, xkick_env('XKICK_STS_HOST', '127.0.0.1'));
define('STS_PORT'			, xkick_env('XKICK_STS_PORT', '3001'));

// ��� ����
require_once( CONFIG_CLASS_PATH."language/".CONFIG_AREA.".inc" );
?>
