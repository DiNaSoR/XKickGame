<?
$encryptkey = "asklgds2so222dssd387djsha2hsb";
 function encrypt($encrypt) { 
   global $encryptkey; 
   $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND); 
   $passcrypt = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, $encryptkey, $encrypt, MCRYPT_MODE_ECB, $iv); 
   $encode = base64_encode($passcrypt); 
 return $encode; 
 } 
  
 //Decrypt Function 
 function decrypt($decrypt) { 
   global $encryptkey; 
   $decoded = base64_decode($decrypt); 
   $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_ECB), MCRYPT_RAND); 
   $decrypted = mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $encryptkey, $decoded, MCRYPT_MODE_ECB, $iv); 
 return $decrypted; 
 } 
 ?>