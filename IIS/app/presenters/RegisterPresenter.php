<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form as Form;


class RegisterPresenter extends BasePresenter
{
    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    protected function createComponentRegistrationForm()
    {
        $form = new Form;
        $form->addGroup('Registrovaci udaje');
        $form->addText('jmeno', 'Jmeno');
        $form->addText('prijmeni', 'Prijmeni');
        $form->addText('email', 'E-mail: *', 35)
                ->addRule(Form::FILLED, 'Vyplňte Váš email')
                ->addCondition(Form::FILLED)
                ->addRule(Form::EMAIL, 'Neplatná emailová adresa')
                ->addRule(function ($control) {
                    return !$this->database->query('SELECT email FROM users WHERE email=? LIMIT 1', $control->value)->fetch();
                }, 'Existuje');
        $form->addPassword('heslo', 'Heslo: *', 20)
            ->setOption('description', 'Alespoň 6 znaků')
            ->addRule(Form::FILLED, 'Vyplňte Vaše heslo')
            ->addRule(Form::MIN_LENGTH, 'Heslo musí mít alespoň %d znaků.', 6);
        $form->addPassword('heslo2', 'Heslo znovu: *', 20)
            ->addConditionOn($form['heslo'], Form::VALID)
            ->addRule(Form::FILLED, 'Heslo znovu')
            ->addRule(Form::EQUAL, 'Hesla se neshodují.', $form['heslo']);


        $form->addCheckbox('agree', 'Chci vyplnit osobni udaje')->setOmitted(TRUE);
        $form->addGroup('Osobni udaje');
        $form->addText('telefon', 'Telefon', 6)
            ->addConditionOn($form['agree'],Form::EQUAL,TRUE)
            ->setRequired('Musi byt vyplnene')
            ->addRule(Form::PATTERN, '9 znaku', '[0-9]{9}');
       // ->addRule(Form::MAX_LENGTH, 'Cislo musí mít  %d znaků.', 9);
        $form->addText('mesto', 'Mesto')
            ->addConditionOn($form['agree'], Form::EQUAL, TRUE)
                ->setRequired('Musi byt vyplnene');


        $form->addText('ulice', 'Ulice')
            ->addConditionOn($form['agree'], Form::EQUAL, TRUE)
            ->setRequired('Musi byt vyplnene');
        $form->addText('psc', 'PSC', 5)
            ->setOption('description', 'Alespoň 5 znaků')
            ->addConditionOn($form['agree'], Form::EQUAL, TRUE)
            ->setRequired('Musi byt vyplnene')
            ->addRule(Form::PATTERN, '5 znaku', '[0-9]{5}');
        $form->addSubmit('login', 'Registrovat');
        $form->onSuccess[] = array($this, 'registerFormSubmitted');
        return $form;
    }

    /**
     * @param Form $form
     */
    public function registerFormSubmitted(Form $form) {
        $data = $form->getValues();
        $data['role'] = 'registered';
        $data['heslo'] = sha1($data['heslo']);
        unset($data['heslo2']);
        $this->database->table('users')->insert($data);

        $this->flashMessage('Uspesne jste byli zaregistrovani');
        $this->redirect('Homepage:');



    }
}
