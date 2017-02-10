<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form;



class HomepagePresenter extends BasePresenter
{
    /** @var Nette\Database\Context */
    private $database;
    private $products;
    private $produkty;
    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }
    public function renderDefault($page = 1)
    {
        $this->template->page = $page;
        $this->template->products = $this->database->table('product')
           // ->order('created_at DESC')

            ->page($page, 9);
    }

    public function actionShow($id)
    {
        $this->products = $this->database->table('product')
        ->where('kategorie', $id)->fetchAll();

        $this->template->products = $this->products;


    }
}
