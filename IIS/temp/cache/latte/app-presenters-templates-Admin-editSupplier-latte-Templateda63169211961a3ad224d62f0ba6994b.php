<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Admin/editSupplier.latte

class Templateda63169211961a3ad224d62f0ba6994b extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('9766e66ade', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lba36fd4bf9e_content')) { function _lba36fd4bf9e_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><!DOCTYPE html>
<html lang="en">
<head>

    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
</head>
<body>

<div class="container">
    <div class="row">
        <div class="col-sm-6 col-md-4 col-md-offset-4">
            <h1 class="text-center login-title">Upravit dodavatela</h1>

            <div class="account-wall">
                <form class="form-signin"<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormBegin($form = $_form = $_control["editSupplierForm"], array (
  'class' => NULL,
), FALSE) ?>>
                    <input type="text" class="form-control" placeholder="Nazev" required autofocus<?php $_input = $_form["nazev"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="WWW" required autofocus<?php $_input = $_form["www"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="E-mail" required autofocus<?php $_input = $_form["email"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="Telefon" required autofocus<?php $_input = $_form["telefon"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="Mesto" required autofocus<?php $_input = $_form["mesto"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="Ulica" required autofocus<?php $_input = $_form["ulica"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="PSC" required autofocus<?php $_input = $_form["psc"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>

                    <button class="btn btn-lg btn-primary btn-block" type="submit">
                        Zmenit'
                    </button>

                <?php echo Nette\Bridges\FormsLatte\Runtime::renderFormEnd($_form, FALSE) ?></form>
            </div>
        </div>
    </div>
</div>
 <div class="container">
        <div class="content">
            <div class="mytitle">Dodavane produkty</div>
            <a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:addSupplierProduct", array($supplier->id)), ENT_COMPAT) ?>
">Vytvorit produkt</a></td>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th>id</th>
                    <th>Nazev</th>
                    <th>Doba</th>
                    <th>Cena </th>
                </tr>
                </thead>
                <tbody>
<?php $iterations = 0; foreach ($products as $product) { ?>                <div>
                    <tr>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->id_produkt, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->nazev, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->doba, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->cena, ENT_NOQUOTES) ?></td>

                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("orderSupplierProduct", array($product->id_dodavatel, $product->id_produkt)), ENT_COMPAT) ?>
">Objednat produkt</a></td>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("editSupplierProduct", array($product->id_dodavatel, $product->id_produkt)), ENT_COMPAT) ?>
">Uprav produkt</a></td>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("deleteSupplierProduct", array($product->id_dodavatel, $product->id_produkt)), ENT_COMPAT) ?>
">Vymaz produkt</a></td>



                    </tr>
                </div>
<?php $iterations++; } ?>
                </tbody>
            </table>
        </div>
    </div>

</body>
</html>
<?php
}}

//
// end of blocks
//

// template extending

$_l->extends = empty($_g->extended) && isset($_control) && $_control instanceof Nette\Application\UI\Presenter ? $_control->findLayoutTemplateFile() : NULL; $_g->extended = TRUE;

if ($_l->extends) { ob_start();}

// prolog Nette\Bridges\ApplicationLatte\UIMacros

// snippets support
if (empty($_l->extends) && !empty($_control->snippetMode)) {
	return Nette\Bridges\ApplicationLatte\UIRuntime::renderSnippets($_control, $_b, get_defined_vars());
}

//
// main template
//
?>




<?php if ($_l->extends) { ob_end_clean(); return $template->renderChildTemplate($_l->extends, get_defined_vars()); }
call_user_func(reset($_b->blocks['content']), $_b, get_defined_vars()) ; 
}}