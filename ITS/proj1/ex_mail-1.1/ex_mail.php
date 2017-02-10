<?php

/**
 * @version 1.1
 */

require_once 'Mail.php';
require_once 'Mail_mime.php';

class MyMail
{
    public static function pear_mail($to, $subject, $nadpis, $body, $html = 1, $from = "")
    {
        global $CONFIG, $ERRORMSG;

        $html_start = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
          <html><body><h1>' . $nadpis . '</h1><div id="ObsahMailu">';

        $html_end   = '</div></body></html>';

        if (strncmp($nadpis, "Objednávka č.", 13) == 0)
        {
            $diky = "<p>Děkujeme za Vaši objednávku v našem eshopu. Objednávka byla přijata, o jejím potvrzení Vás budeme co nejdříve informovat.</p>";
            $body = $body . $diky;
        }

        $body = $html_start . $body . $html_end;

        $zjisti_cislo_obj = explode(" ", $nadpis);
        $id_obj           = $zjisti_cislo_obj[2] * 1;

        $prof_ano_ne = Db::DbQuery("SELECT d.proforma FROM kosik_obj o LEFT JOIN dopravne d ON (o.id_dopravy = d.id_dopravy) WHERE o.id_obj = $id_obj");

        if ($prof_ano_ne['proforma'] == 1) {
            $proforma = "<fieldset><legend>Údaje o platbě</legend><br />Číslo účtu: XXSANITIZEDXX" . "<br />" . str_replace("Objednávka č.", "Variabilní symbol:", $nadpis) . "<br />Splatnost: " . date("d.m.Y", strtotime("+$CONFIG[proforma_splatnost] days")) . "</fieldset>";
            $body     = str_replace("Objednávka č.", "Proforma faktura č.", $body);
            $body     = str_replace('id="obsahMailu">', 'id="obsahMailu">' . $proforma, $body);
        }

        $message = new Mail_mime();
        $message->setHTMLBody($body);

        $body = $message->get();

        $sender   = $CONFIG['mail_odesilatel'];
        $server   = $CONFIG['mail_server'];
        $username = $CONFIG['mail_server_name'];
        $password = $CONFIG['mail_server_pass'];
        $port     = $CONFIG['mail_server_port'];
        if ($from == "")
            $from = $sender;

        if ($html == 1) {
            $headers = array(
                "From" => $sender,
                "Reply-to" => $from,
                "To" => $to,
                "MIME-Version" => '1.0',
                "Content-Type" => 'text/html; charset=utf-8',
                "Content-Transfer-Encoding" => 'quoted-printable',
                "Subject" => $subject
            );
        } else {
            $headers = array(
                "From" => $sender,
                "To" => $to,
                "Reply-to" => $from,
                "Content-Type" => 'text/plain; charset=utf-8',
                "Content-Transfer-Encoding" => 'quoted-printable',
                "Subject" => $subject
            );
        }
        $headers = $message->headers($headers);

        $smtp = Mail::factory("smtp", array(
            "host" => $server,
            "port" => $port,
            "auth" => true,
            "username" => $username,
            "password" => $password,
            "timeout" => 5
        ));

        $mail = $smtp->send($to, $headers, $body);

        if (PEAR::isError($mail)) {
            $ERRORMSG = ($mail->getMessage()) . "\n";
            return false;
        } else {
            return true;
        }

    }
}
?>

