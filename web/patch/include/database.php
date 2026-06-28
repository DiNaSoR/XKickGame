<?
// MySQL Unsigned Limit
define('MyBIGINT', '18446744073709551615');
define('MyINT', 4294967295);
define('MyMEDIUMINT', 16777215);
define('MySMALLINT', 65535);
define('MyTINYINT', 255);

class MyDB
{
    var $conn, $transaction, $result, $err_report, $query_count;

    function MyDB($db_setting, $transaction = false)
    {
        $this->err_report = false;
        $this->transaction = $transaction;
        $this->query_count = 0;
        $db_host = getenv('XKICK_PATCH_DB_HOST') ? getenv('XKICK_PATCH_DB_HOST') : '127.0.0.1';
        $db_user = getenv('XKICK_PATCH_DB_USER') ? getenv('XKICK_PATCH_DB_USER') : 'xkick';
        $db_pass = getenv('XKICK_PATCH_DB_PASS') ? getenv('XKICK_PATCH_DB_PASS') : 'xkick_dev_password';
        $this->conn = mysql_connect($db_host, $db_user, $db_pass) or exit('mysql_connect error');
        mysql_select_db($db_setting, $this->conn) or exit('mysql_select_db error');
        if($transaction) {
            mysql_query('SET AUTOCOMMIT=0', $this->conn) or exit('mysql_transaction_autocommit error');
            mysql_query('BEGIN', $this->conn) or exit('mysql_transaction_begin error');
            register_shutdown_function(array(&$this, 'close_error'));
        }
        return true;
    }

    function close()
    {
        if($this->conn === false)
            return false;
        if($this->transaction)
            mysql_query('COMMIT', $this->conn) or exit('mysql_transaction_commit error');
        mysql_close($this->conn) or exit('mysql_close error');
        $this->conn = false;
        return true;
    }

    function close_error($message = 'mysql_error exit')
    {
        if($this->conn === false)
            return false;
        if($this->transaction)
            mysql_query('ROLLBACK', $this->conn) or exit('mysql_transaction_rollback error');
        mysql_close($this->conn) or exit('mysql_close_error error');
        exit($message);
        return true;
    }

    function query($query)
    {
        $this->query_count ++;
        $this->result = mysql_query($query, $this->conn) or $this->close_error($this->err_report?'query < '.$query.' > error (count : '.$this->query_count.' ) : '.mysql_error($this->conn):'query error (count : '.$this->query_count.' )');
        return true;
    }
}
?>
