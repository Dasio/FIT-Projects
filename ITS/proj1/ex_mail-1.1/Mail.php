<?php
/* Pripraveno pro test doubles. */
/* Primo vyzadovano testovanym souborem. */
class Mail
{
    private function __construct($protocol , $params = array())
    {
        $this->protocol = $protocol;
        $this->params = $params;
    }
    public static function factory($protocol , $params = array())
    {
        return new Mail($protocol,$params);
    }
    public function send($to, $header, $body)
    {
        SmtpObject::$to = $to;
        SmtpObject::$header = $header;
        SmtpObject::$body = $body;
        if(Mail::$error)
            return new PEAR_Error("Failed to send");
        else
            return true;
    }
    private $protocol,$params;
    public static $error = false;
}
?>
