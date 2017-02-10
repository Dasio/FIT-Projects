<?php
/* Pripraveno pro test doubles. */
/* Primo vyzadovano testovanym souborem. */
class Mail_mime
{
    public function setHTMLBody($body)
    {
        $this->body = $body;
    }
    public function get()
    {
        return $this->body;
    }
    public function headers($headers)
    {
        return $headers;
    }
    private $body;
}
?>
