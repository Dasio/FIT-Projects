<?php
/* Pripraveno pro test doubles. */
/* Vyzadovano bootstrap.php jako vychozi zavislosti. */

class Db
{
    public static $proforma = 0;
    public static function DbQuery($query)
    {
        if(Db::$proforma == -1)
            return array();
        return array("proforma" => Db::$proforma);
    }
}

class PEAR
{
    public static function isError($mail)
    {
        if($mail === true)
            return false;
        else
            return true;
    }
}
class PEAR_Error
{
    public function __construct($message)
    {
        $this->message = $message;
    }
    public function getMessage()
    {
        return $this->message;
    }
    private $message;
}
class SmtpObject
{
    public static $to,$header,$body;
}
?>
