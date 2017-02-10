<?php


namespace App\Presenters;

use Dasio\ShoppingCart\Cart;
use Dasio\ShoppingCart\ICartControlFactory;
use Nette\Database\Context;


class CartPresenter extends BasePresenter
{



    /** @var ICartControlFactory @inject */
    public $cartControlFactory;

    /** @var Context @inject */
    public $database;

    public function actionAdd($id)
    {
        $product = $this->database->table('product')->get($id);

        if (!$product) $this->error();


        $item = $this->cart->addItem($product->id, $product->cena)
            ->setName($product->nazev)
            ->setImage($product->obrazek)
            ->setUnit("kus/y")
            ->setVatRate(20)
            ->setLink('Product:show')
            ->setLinkArgs($product->id);
        $this->redirect('default');
    }


    protected function createComponentCartControl()
    {
        $cartControl = $this->cartControlFactory->create();

        $cartControl->onClickContinue[] = function() {
            $this->redirect('Homepage:default');
        };

        $cartControl->onClickNext[] = function() {
            $this->redirect('Order:default');
        };
        return $cartControl;
    }

}
