<?php
/**
 * Created by PhpStorm.
 * User: nimik
 * Date: 11/29/2015
 * Time: 4:14 PM
 */

namespace App\Presenters;
use Nette;
use App\Model;
use Nette\Application\UI\Form as Form;
use Dasio\ShoppingCart\ICartControlFactory;
use Nette\Database\Context;

use App\Presenters\CartPresenter;



class OrderPresenter extends BasePresenter
{



    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;

    }

    public function renderDefault()
    {
        if($this->user->isLoggedIn())
        {
            $osobni_udaje = $this->database->table('users')->get($this->user->id);
            $this['orderForm']->setDefaults([
                'telefon'=> $osobni_udaje->telefon,
                'mesto' => $osobni_udaje->mesto,
                'ulice' => $osobni_udaje->ulice,
                'psc' => $osobni_udaje->psc
            ]);
        }


    }

    protected function createComponentOrderForm()
    {




        $form = new Form;
        $form->addGroup('Osobni udaje');
        $form->addText('telefon', 'Telefon', 6)
            ->addRule(Form::MAX_LENGTH, 9)
            ->addRule(Form::FILLED, 'nutne');
        $form->addText('mesto', 'Mesto')
        ->addRule(Form::FILLED, 'nutne');
        $form->addText('ulice', 'Ulice')
            ->addRule(Form::FILLED, 'nutne');
        $form->addText('psc', 'PSC', 5)
            ->setOption('poznamka','Description')
            ->addRule(Form::MIN_LENGTH, 5)
            ->addRule(Form::MAX_LENGTH, 5);
        $form->addRadioList('sposob_dopravy', 'Zpusob dopravy:', array(
            'osobni_odber' => 'osobni odber',
            'doruceni' => 'doruceni',
            'posta' => 'posta',));
        $form->addRadioList('sposob_platby', 'Zpusob platby:', array(
                 'hotove' => 'Hotove',
                 'kartou' => 'kartou',));
          //  ->getSeparatorPrototype()->setName(NULL);
        $form->addTextArea('poznamka', 'Description', NULL, 5);
        $form->addSubmit('login', 'Objednat');
        $form->onSuccess[] = array($this, 'OrderSubmitted');
        return $form;
    }

    public function OrderSubmitted(Form $form, $item) {
        $data = $form->getValues();
        $data['id_zakaznik'] = $this->cart->id;
        $data['cena'] = $this->cart->getTotal();
        // Objednavka
        $insert = $this->database->table('objednavka')->insert($data);
        $id_obj = $insert['id'];
        // Obsah objednavky
        foreach($this->cart->getItems() as $key => $item)
        {
            $this->database->query(
            'UPDATE product
            SET sklad=sklad-?
            WHERE id=?;',$item->quantity,$item->id);
            $data = array('id_objednavky'=>$id_obj,'id_produkt' =>$item->id,
                'mnozstvo'=>$item->quantity,'cena'=>$item->price);
            $this->database->table('obsah_objednavky')->insert($data);
        }
        // Zmazanie kosika
        $this->cart->clear(0);
        $this->database->table('kosik')
            ->where('id_zakaznik',$this->cart->id)
            ->delete();


        $this->flashMessage('Dekuji za objednavku', 'success');
        $this->redirect('Homepage:default');
        return $form;
    }
}
