<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form;
use Nette\Utils\DateTime;

use OndrejBrejla\Eciovni\Eciovni;
use OndrejBrejla\Eciovni\ParticipantBuilder;
use OndrejBrejla\Eciovni\ItemImpl;
use OndrejBrejla\Eciovni\DataBuilder;
use OndrejBrejla\Eciovni\TaxImpl;


class OrdersPresenter extends BasePresenter
{
    /** @var Nette\Database\Context */
    private $database;
    private $produkty;
    private $obsah;
    private $zakaznik;
    private $objednavka;
    private $objednavkaID;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
    public function renderDefault($page = 1)
    {
        $this->template->page = $page;
        if($this->user->isAllowed('Orders', 'showAll'))
            $this->template->objednavky = $this->database->table('objednavka')
                ->order('datum_prijatia DESC');
        else
            $this->template->objednavky = $this->database->table('objednavka')
                ->where('id_zakaznik',$this->user->id)
                ->order('datum_prijatia DESC');

    }
    public function actionShow($id)
    {
        $this->objednavka = $this->database->table('objednavka')->get($id);
        $this->zakaznik = $this->database->table('users')->get($this->objednavka->id_zakaznik);
        $this->obsah = $this->database->table('obsah_objednavky')
            ->where('id_objednavky',$id)->fetchAll();
        foreach($this->obsah as $ob)
        {
            $this->produkty[$ob['id_produkt']] = $this->database->table('product')->get($ob['id_produkt']);
        }
        $this->template->obsah = $this->obsah;
        $this->template->objednavka = $this->objednavka;
        $this->template->produkty = $this->produkty;
    }

    public function actionEdit($id)
    {
        $this->template->orderID = $this->objednavkaID = $id;
    }

    protected function createComponentDelete()
    {
        $form = new Form();
        $form->addSubmit('delete','Zmazat');
        $form->onSuccess[] = array($this,'DeleteOrder');
        return $form;
    }
    public function DeleteOrder(Form $form)
    {
        $order = $this->database->table('objednavka')->get($this->objednavkaID);
        if (!$order) {
            $this->error('Order not found');
        }
        $order->delete();
        $this->flashMessage("zmazane",'success');
        $this->redirect('default');
    }

    protected function createComponentOrdersForm() {
    $form = new Form();
        $form->addRadioList('stav', 'Stav objednavky:', array(
            'Prijata' => 'Prijata',
            'Expedicia' => 'V expedicii',
            'Dorucene' => 'Dorucene'
        ));
        $form->addSubmit('login', 'Zmenit');
        $form->onSuccess[] = array($this, 'OrdersSubmitted');
        return $form;
    }

    public function OrdersSubmitted(Form $form)
    {
        $data = $form->getValues();
        $data['datum_zmeny_stavu'] = new DateTime();
        $this->database->table('objednavka')
            ->where('id',$this->objednavkaID)
            ->update($data);
        $this->redirect('Orders:');





    }

     protected function createComponentFaktura() {
        $dateNow = new DateTime($this->objednavka->datum_prijatia);
        $dateExp = new DateTime($this->objednavka->datum_prijatia);
        $dateExp->modify('+14 days');
        $variableSymbol = '1234';
        $supplierBuilder = new ParticipantBuilder('David Mikus', 'Bozetechova', '42', 'Brno', '1337');
        $supplier = $supplierBuilder->setIn('12345678')->setTin('CZ12345678')->setAccountNumber('123456789 / 1111')->build();
        $meno = $this->zakaznik->jmeno . " " . $this->zakaznik->prijmeni;
        $customerBuilder = new ParticipantBuilder($meno,$this->zakaznik->ulice, '', $this->zakaznik->mesto, $this->zakaznik->psc);
        $customer = $customerBuilder->setAccountNumber('123456789 / 1111')->build();
        $items = array();

        foreach($this->obsah as $ob)
        {
           $items[] = new ItemImpl($this->produkty[$ob['id_produkt']]->nazev,$ob->mnozstvo,$ob->cena,TaxImpl::fromPercent(20));
        }
        $dataBuilder = new DataBuilder($this->objednavka->id, 'Faktura - cislo', $supplier, $customer, $dateExp, $dateNow, $items);
        $dataBuilder->setVariableSymbol($variableSymbol)->setDateOfVatRevenueRecognition($dateNow);
        $data = $dataBuilder->build();

        return new Eciovni($data);
    }

    public function actionGenerate($id,$print=false) {
        $this->objednavka = $this->database->table('objednavka')->get($id);
        if(!$this->user->isAllowed('Orders', 'showAll') && $this->objednavka['id_zakaznik'] != $this->user->id)
        {
            return;
        }
       /* if($this->objednavka->stav !== 'Dorucene')
            return;*/
        if(!isset($this->zakaznik))
            $this->zakaznik = $this->database->table('users')->get($this->objednavka['id_zakaznik']);
        if(!isset($this->obsah))
            $this->obsah = $this->database->table('obsah_objednavky')
                ->where('id_objednavky',$id)->fetchAll();
        foreach($this->obsah as $ob)
        {
            $this->produkty[$ob['id_produkt']] = $this->database->table('product')->get($ob['id_produkt']);
        }
        // in case you downloaded mPDF separately
        // include_once(LIBS_DIR . '/MPDF54/mpdf.php');
        $mpdf = new \mPDF('utf-8');
        if($print)
            $mpdf->SetJS('this.print();');
        // Exporting prepared invoice to PDF.
        // To save the invoice into a file just use the second and the third parameter, equally as it's described in the documentation of mPDF->Output().
        $this['faktura']->exportToPdf($mpdf);
        //file_put_contents( "faktura.pdf", $this['eciovni']->exportToPdf($mpdf));
    }
}
