<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form as Form;
use Nette\Security as NS;

class SignPresenter extends BasePresenter
{
    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    protected function createComponentSignInForm()
    {
        $form = new Form;
        $form->addText('jmeno', 'Email');
        $form->addPassword('heslo', 'Heslo');
        $form->addSubmit('login', 'Sign in');
        $form->onSuccess[] = array($this, 'authenticate');
        return $form;
    }

    function authenticate(Form $form)
    {
        $values = $form->getValues();
        $user = $this->getUser();
        $authenticator = new \auth\MyAuthenticator($this->database);
        $user->setAuthenticator($authenticator);

        try {

            $identity = $user->login($values['jmeno'], $values['heslo']);
            // Restore cart
            $this->cart->clear(0);
            $rows = $this->database->table('kosik')
                ->where('id_zakaznik',$user->getId())
                ->fetchAll();
            foreach($rows as $row)
            {
                $produkt = $this->database->table('product')
                    ->where('id',$row['id_produkt'])
                    ->fetch();
                $this->cart->addItem($row['id_produkt'],$produkt['cena'],$row['mnozstvo'],[],0)
                    ->setName($produkt['nazev'])
                    ->setImage($produkt['obrazek'])
                    ->setUnit("kus/y")
                    ->setVatRate(20)
                    ->setLink('Product:show')
                    ->setLinkArgs($row['id_produkt']);
            }


        } catch (Nette\Security\AuthenticationException $e) {
            $this->flashMessage('Nespravne jmeno/heslo');
        }
        $user->setExpiration('5 minutes',TRUE);
        $this->redirect('Homepage:default');
    }

    public function actionOut()
    {
        $this->getUser()->logout();
        $this->flashMessage('You have been signed out.');
        $this->redirect('Homepage:');
    }


}
