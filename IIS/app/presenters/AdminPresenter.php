<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form;



class AdminPresenter extends BasePresenter
{
    /** @var Nette\Database\Context */
    private $database;
    private $id;
    private $idp;
    private $supplier;
    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
    // Produkty
    public function renderProducts()
    {
         $this->template->products = $this->database->table('product');
    }

    public function actionDeleteProduct($id)
    {
        $product = $this->database->table('product')->get($id);
        if (!$product) {
            $this->error('Product not found');
        }
        $product->delete();
        $this->redirect('Admin:products');
    }

    public function actionEditProduct($productID)
    {
        $this->id = $productID;
        $product = $this->database->table('product')->get($productID);
        if (!$product) {
            $this->error('Product not found');
        }
        $this['editProductForm']->setDefaults($product->toArray());
    }
    protected function createComponentEditProductForm()
    {
        $form = new Form;
        $form->addText('nazev', 'Nazev:')
            ->setRequired();
        $form->addTextArea('popis', 'Obsah:')
            ->setRequired();
        $form->addText('cena', 'Cena:')
            ->setRequired();
        $form->addText('obrazek', 'Obrazek:')
            ->setRequired();
        $kategorie = array(
                'skicak' => 'Skicak',
                'pastelka' => 'Pastelka',
            );
        $form->addSelect('kategorie', 'Kategorie:', $kategorie)
            ->setPrompt('Zvolte');
        $form->addText('sklad', 'Sklad:')
        ->setRequired();
        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'editProductFormSucceeded');

        return $form;
    }

    public function editProductFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $this->database->table('product')
            ->where('id',$this->id)
            ->update($data);

        $this->flashMessage("Produkt bol zmeneny", 'success');
        $this->redirect('Admin:products');
    }

    protected function createComponentAddProductForm()
    {
        $form = new Form;
        $form->addText('nazev', 'Nazev:')
            ->setRequired();
        $form->addTextArea('popis', 'Obsah:')
            ->setRequired();
        $form->addText('cena', 'Cena:')
            ->setRequired();
        $form->addText('obrazek', 'Obrazek:')
            ->setRequired();
        $kategorie = array(
            'skicak' => 'Skicak',
            'pastelka' => 'Pastelka',
        );
        $form->addSelect('kategorie', 'Kategorie:', $kategorie)
            ->setPrompt('Zvolte');
        $form->addText('sklad', 'Sklad:')
            ->setRequired();

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'addProductFormSucceeded');

        return $form;
    }
    public function addProductFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $this->database->table('product')
            ->insert($data);

        $this->flashMessage("Produkt bol pridany", 'success');
        $this->redirect('Admin:products');
    }
    public function renderUsers()
    {
        $this->template->users = $this->database->table('users');
    }

    public function actionEditUser($id)
    {
        $this->id = $id;
    }

    protected function createComponentEditUserRoleForm() {
        $form = new Form();
        $form->addRadioList('role', 'Role:', array(
            'registered' => 'Registrovany',
            'staff' => 'Obsluha obchodu'
        ));
        $form->addSubmit('login', 'Zmenit');
        $form->onSuccess[] = array($this, 'editRoleFormSucceeded');
        return $form;
    }
    public function editRoleFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $this->database->table('users')
            ->where('id', $this->id)
            ->update($data);
        $this->redirect('Admin:users');
    }

    public function renderSuppliers()
    {
        $this->template->suppliers = $this->database->table('dodavatel');
    }
    public function createComponentAddSupplierForm(){

        $form = new Form;
        $form->addText('nazev', 'Nazev:')
            ->setRequired();
        $form->addText('www', 'WWW:')
            ->setRequired();
        $form->addText('email', 'E-mail:')
            ->setRequired();
        $form->addText('telefon', 'Telefon:')
            ->setRequired();
        $form->addText('mesto', 'Mesto:')
            ->setRequired();
        $form->addText('ulica', 'Ulica:')
            ->setRequired();
        $form->addText('psc', 'PSC:')
            ->setRequired();

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'addSupplierFormSucceeded');

        return $form;
    }
    public function addSupplierFormSucceeded(Form $form){
        $data = $form->getValues();
        $this->database->table('dodavatel')
            ->insert($data);

        $this->flashMessage("Dodavatel bol pridany", 'success');
        $this->redirect('Admin:suppliers');
    }
    public function actionEditSupplier($id)
    {
        $this->id = $id;
        $this->template->supplier = $supplier = $this->database->table('dodavatel')->get($id);
        if (!$supplier) {
            $this->error('Supplier not found');
        }
        $this['editSupplierForm']->setDefaults($supplier->toArray());

        $this->template->products = $this->database->query('
            SELECT dp.id_dodavatel,dp.id_produkt,p.nazev,dp.doba,p.cena FROM dodavatel d
            INNER JOIN dodavatel_produkt dp ON d.id = dp.id_dodavatel
            INNER JOIN product p ON p.id = dp.id_produkt
            WHERE d.id = ?;',$id);
    }

    public function actionDeleteSupplier($id)
    {
        $product = $this->database->table('dodavatel')->get($id);
        if (!$product) {
            $this->error('Supplier not found');
        }
        $product->delete();
        $this->redirect('Admin:suppliers');
    }

    protected function createComponentEditSupplierForm()
    {
        $form = new Form;
        $form->addText('nazev', 'Nazev:')
            ->setRequired();
        $form->addText('www', 'WWW:')
            ->setRequired();
        $form->addText('email', 'E-mail:')
            ->setRequired();
        $form->addText('telefon', 'Telefon:')
            ->setRequired();
        $form->addText('mesto', 'Mesto:')
            ->setRequired();
        $form->addText('ulica', 'Ulica:')
            ->setRequired();
        $form->addText('psc', 'PSC:')
            ->setRequired();

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'editSupplierFormSucceeded');

        return $form;
    }

    public function editSupplierFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $this->database->table('dodavatel')
            ->where('id',$this->id)
            ->update($data);

        $this->flashMessage("Dodavatel bol zmeneny", 'success');
        $this->redirect('Admin:suppliers');
    }

    public function actionDeleteSupplierProduct($id_supplier, $id_product)
    {
        $product = $this->database->table('dodavatel_produkt')
            ->where('id_dodavatel',$id_supplier)
            ->where('id_produkt',$id_product);
        if (!$product) {
            $this->error('Supplier-product not found');
        }
        $product->delete();
        $this->redirect('Admin:editSupplier',$id_supplier);
    }

    public function actionEditSupplierProduct($id_supplier, $id_product)
    {
        $this->id = $id_supplier;
        $this->idp = $id_product;
        $supplier = $this->database->table('dodavatel_produkt')
            ->where('id_dodavatel',$id_supplier)
            ->where('id_produkt',$id_product)
            ->fetch();
        if (!$supplier) {
            $this->error('Supplier-product not found');
        }
        $this['editSupplierProductForm']->setDefaults($supplier->toArray());
    }

     protected function createComponentEditSupplierProductForm()
    {
        $form = new Form;
        $form->addText('doba', 'Doba:')
            ->setRequired();
        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'editSupplierProductFormSucceeded');

        return $form;
    }

    public function editSupplierProductFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $this->database->table('dodavatel_produkt')
            ->where('id_dodavatel',$this->id)
            ->where('id_produkt',$this->idp)
            ->update($data);

        $this->flashMessage("Doba bola zmenenena", 'success');
        $this->redirect('Admin:editSupplier',$this->id);
    }

    public function actionAddSupplierProduct($id)
    {
        $this->id = $id;
    }
    protected function createComponentAddSupplierProductForm()
    {
        $form = new Form;
        $form->addText('id_produkt', 'ID Produktu:')
            ->setRequired();;
        $form->addText('doba', 'Doba:')
            ->setRequired();

        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'addSupplierProductFormSucceeded');

        return $form;
    }
    public function addSupplierProductFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $data['id_dodavatel'] = $this->id;
        $this->database->table('dodavatel_produkt')
            ->insert($data);

        $this->flashMessage("Produkt bol pridany", 'success');
        $this->redirect('Admin:editSupplier',$this->id);
    }


    public function actionOrderSupplierProduct($id_supplier, $id_product)
    {
        $this->id = $id_supplier;
        $this->idp = $id_product;
        $this->supplier = $this->database->table('dodavatel_produkt')
            ->where('id_dodavatel',$id_supplier)
            ->where('id_produkt',$id_product)
            ->fetch();
        if (!$this->supplier) {
            $this->error('Supplier-product not found');
        }
    }

     protected function createComponentOrderSupplierProductForm()
    {
        $form = new Form;
        $form->addText('mnozstvo', 'Mnozstvo:')
            ->setRequired();
        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'orderSupplierProductFormSucceeded');

        return $form;
    }

    public function orderSupplierProductFormSucceeded(Form $form)
    {
        $data = $form->getValues();
        $data['id_dodavatel'] = $this->id;
        $data['id_produkt'] = $this->idp;
        $data['id_predajca'] = $this->user->id;
        $data['doba'] = $this->supplier->doba;
        $this->database->table('objednavka_produkt')
            ->insert($data);
        $this->flashMessage("Objednavka bola uskutocenna", 'success');
        $this->redirect('Admin:supplierOrders');
    }

    public function renderSupplierOrders()
    {
        $this->template->orders = $this->database->query(
            'SELECT op.id,u.email, d.nazev AS firma,p.nazev AS produkt,op.mnozstvo,op.doba,op.stav,op.datum
            FROM objednavka_produkt op
            INNER JOIN dodavatel d ON op.id_dodavatel=d.id
            INNER JOIN product p ON op.id_produkt=p.id
            INNER JOIN users u ON u.id = op.id_predajca
            ORDER BY op.datum DESC;');
    }

    public function actionAcceptSupplierOrder($id)
    {
        $order = $this->database->table('objednavka_produkt')->get($id);
        $order->update(array('stav' => 'Dokoncene'));
        $this->database->query(
            'UPDATE product
            SET sklad=sklad+?
            WHERE id=?;',$order['mnozstvo'],$order['id_produkt']);
        $this->redirect('Admin:supplierOrders');

    }

}
