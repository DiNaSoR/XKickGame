<?
////////////////////////////////////////////////////////////////////////
// 클래스 : 암호화 클래스
// 제작자 : 조용길
// 날  짜 : 2009.08
////////////////////////////////////////////////////////////////////////
class CCryptAPI
{
	private $m_strPassword = "local-dev-crypt-key-change-me";

	// 암호화
	function Encrypt( $strText )
	{
		$nLen = 16;
		$strText .= "\x13";
		$n = strlen($strText);
		if ($n % 16) $strText .= str_repeat("\0", 16 - ($n % 16));
		$i = 0;
		while ($nLen-- >0) $strEncrypt .= chr(mt_rand() & 0xff);
		$strKey = substr($m_strPassword ^ $strEncrypt, 0, 512);
		while($i <$n)
		{
			$strBuf = substr($strText, $i, 16) ^ pack('H*', md5($strKey));
			$strEncrypt .= $strBuf;
			$strKey = substr($strBuf . $strKey, 0, 512) ^ $m_strPassword;
			$i += 16;
		}
		return base64_encode($strEncrypt);
	}

	// 복호화
	function Decrypt($strEncrypt)
	{
		$nLen = 16;
		$strEncrypt = base64_decode($strEncrypt);
		$n = strlen($strEncrypt);
		$i = $nLen;
		$strText = '';
		$strKey = substr($m_strPassword ^ substr($strEncrypt, 0, $nLen), 0, 512);
		while($i <$n)
		{
			$strBuf = substr($strEncrypt, $i, 16);
			$strText .= $strBuf ^ pack('H*', md5($strKey));
			$strKey = substr($strBuf . $strKey, 0, 512) ^ $m_strPassword;
			$i += 16;
		}
		return preg_replace('/\x13\x00*$/', '', $strText);
	}
}
?>