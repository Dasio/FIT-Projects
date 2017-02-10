<?php

require "ex_mail.php";
$thx = '<p>Děkujeme za Vaši objednávku v našem eshopu. Objednávka byla přijata, o jejím potvrzení Vás budeme co nejdříve informovat.</p>';
function testBody($obj,$title,$prof = 0)
{
    $proforma = "<fieldset><legend>Údaje o platbě</legend><br />Číslo účtu: XXSANITIZEDXX" . "<br />";
    $html_end = '</div></body></html>';
    $html_start = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
          <html><body><h1>';
    $obj->assertStringStartsWith($html_start,SmtpObject::$body);
    $obj->assertStringEndsWith($html_end,SmtpObject::$body);
    if(strpos($title,"Objednávka č.") !== FALSE)
    {
        if($prof == 1)
        {
            $obj->assertEquals(strpos(SmtpObject::$body,'Proforma faktura č.'),TRUE);
            $obj->assertEquals(strpos(SmtpObject::$body,$proforma),TRUE);
        }
        else
            $obj->assertEquals(strpos(SmtpObject::$body,'Objednávka č.'),TRUE);
    }

}
class MyMailTest extends PHPUnit_Framework_TestCase
{
    // BCC
    public function testBaseBlocks()
    {
        global $thx;
        global $CONFIG;
        Db::$proforma = 0;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $title = "Objednávka č. 10";
        MyMail::pear_mail($to, $subject, $title, "some body");
        $this->assertEquals($to,SmtpObject::$to);
        testBody($this,$title);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $CONFIG['mail_odesilatel'],
            "To" => $to,
            "MIME-Version" => '1.0',
            "Content-Type" => 'text/html; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
        $this->assertEquals(strpos(SmtpObject::$body,$thx),TRUE);
    }
    // Testing non-based blocks
    public function testTextForm()
    {
        global $thx;
        global $CONFIG;
        Db::$proforma = 0;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $title = "Objednávka č. 10";
        MyMail::pear_mail($to, $subject, $title, "some body",0);
        $this->assertEquals($to,SmtpObject::$to);
        testBody($this,$title);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $CONFIG['mail_odesilatel'],
            "To" => $to,
            "Content-Type" => 'text/plain; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
        $this->assertEquals(strpos(SmtpObject::$body,$thx),TRUE);
    }
    public function testFrom()
    {
        global $CONFIG;
        Db::$proforma = 0;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $from = "admin@fit.vutbr.cz";
        $title = "Objednávka č. 10";

        MyMail::pear_mail($to, $subject, $title, "some body",1,$from);
        testBody($this,$title);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $from,
            "To" => $to,
            "MIME-Version" => '1.0',
            "Content-Type" => 'text/html; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
    }
    public function testProforma()
    {
        global $CONFIG;
        $CONFIG['proforma_splatnost'] = "čas vypršel";
        Db::$proforma = 1;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $title = "Objednávka č. 1337";
        $body = "some body";

        MyMail::pear_mail($to, $subject, $title, $body, 1);
        testBody($this,$title,1);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $CONFIG['mail_odesilatel'],
            "To" => $to,
            "MIME-Version" => '1.0',
            "Content-Type" => 'text/html; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
    }
    public function testSendError()
    {
        global $ERRORMSG;
        Db::$proforma = 0;
        Mail::$error = true;
        $this->assertFalse(MyMail::pear_mail("to@example.com", "Subject text", "Nadpis x 10", "some body"));
        $this->assertNotNull($ERRORMSG);
    }
    public function testTitleWithoutNumber()
    {
        Db::$proforma = 0;
        Mail::$error = false;
        $title = "Nadpis x y";
        MyMail::pear_mail("to@example.com", "Subject text", $title, "some body");
        testBody($this,$title);
    }
    public function testTitleWithOneSpace()
    {
        Db::$proforma = 0;
        Mail::$error = false;
        MyMail::pear_mail("to@example.com", "Subject text", "Nadpis x", "some body");
    }
    public function testEmptyResultFromDB()
    {
        Db::$proforma = -1;
        Mail::$error = false;
        MyMail::pear_mail("to@example.com", "Subject text", "Nadpis x 10", "some body");
    }
    // No "objednavka"
    public function testValidTitle()
    {
        global $thx;
        global $CONFIG;
        Db::$proforma = 0;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $title = "Nadpis č. 10";
        MyMail::pear_mail($to, $subject, $title, "some body");
        $this->assertEquals($to,SmtpObject::$to);
        testBody($this,$title);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $CONFIG['mail_odesilatel'],
            "To" => $to,
            "MIME-Version" => '1.0',
            "Content-Type" => 'text/html; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
        $this->assertEquals(strpos(SmtpObject::$body,$thx),FALSE);
    }
    public function testTypoInTitle()
    {
        global $thx;
        global $CONFIG;
        Db::$proforma = 0;
        Mail::$error = false;
        $to = "to@example.com";
        $subject = "Subject text";
        $title = "Objednávka č, 10"; // , instead of .
        MyMail::pear_mail($to, $subject, $title, "some body");
        $this->assertEquals($to,SmtpObject::$to);
        testBody($this,$title);
        $refHeader = array(
            "From" => $CONFIG['mail_odesilatel'],
            "Reply-to" => $CONFIG['mail_odesilatel'],
            "To" => $to,
            "MIME-Version" => '1.0',
            "Content-Type" => 'text/html; charset=utf-8',
            "Content-Transfer-Encoding" => 'quoted-printable',
            "Subject" => $subject
            );
        $this->assertEquals($refHeader,SmtpObject::$header);
        $this->assertEquals(strpos(SmtpObject::$body,$thx),FALSE);
    }

}
?>

