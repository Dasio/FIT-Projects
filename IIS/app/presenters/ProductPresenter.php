<?php


namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form as Form;
use Nette\Forms\Controls\SubmitButton as SubmitButton;

class ProductPresenter extends BasePresenter
{

    private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

    public function renderShow($productId)
    {
        $product = $this->database->table('product')->get($productId);
        $this->template->product = $product;
        if($product->sklad == 0)
            $this->template->doba = $this->database->table('dodavatel_produkt')
                ->where('id_produkt',$productId)
                ->min('doba');
        $this->template->comments = $product->related('comment')->order('created_at');
        $this->template->users = array();
        foreach($this->template->comments as $comment)
        {
            $this->template->users[$comment->id] = $this->database->table('users')->get($comment->zakaznik_id);
        }
        $avg = $this->database->query(
            'SELECT AVG(body) as hodnotenie FROM comments WHERE product_id=?;',$productId)
            ->fetchField('hodnotenie');

        if($avg === NULL)
            $this->template->avg = "Nehodnotene";
        else
            $this->template->avg =round($avg,1);


    }

    protected function createComponentCommentForm()
    {
        $form = new Form;
        $form->addTextArea('content', 'Komentár:')
            ->setRequired();
        $body = array(

            '1' => '1',
            '2' => '2',
            '3'  => '3',
            '4'  => '4',
            '5'  => '5',
        );
        $form->addSelect('body', 'Body:', $body)
            ->setDefaultValue(1)
            ->setPrompt('Hodnoceni produktu');
        $form->addSubmit('send', 'Publikovat');
        $form->onSuccess[] = array($this, 'commentFormSucceeded');
        return $form;
    }

    public function commentFormSucceeded($form, $values)
    {

        $this->database->table('comments')->insert(array(
            'product_id' => $this->getParameter('productId'),
            'zakaznik_id' => $this->getUser()->getId(),
            'content' => $values->content,
            'body' => $values->body
        ));

        $this->flashMessage('D?kuji za komentá?', 'success');
        $this->redirect('this');
    }


}
