<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Dasio\ShoppingCart\Cart;

/**
 * Base presenter for all application presenters.
 */
abstract class BasePresenter extends Nette\Application\UI\Presenter
{

    /** @var Cart @inject */
    public $cart;
    protected function startup()
    {
        parent::startup();
        if (!$this->user->isAllowed($this->name, $this->action))
        {
            $this->flashMessage('Pristup zamietnuty');
            $this->redirect('Homepage:');
        }
        if($this->getUser()->isLoggedIn())
            $this->cart->id = $this->getUser()->getId();
        else $this->cart->id = 0;
    }
    protected function createTemplate()
    {
        $template = parent::createTemplate();
        $template->cart = $this->cart;
        return $template;
    }

}
