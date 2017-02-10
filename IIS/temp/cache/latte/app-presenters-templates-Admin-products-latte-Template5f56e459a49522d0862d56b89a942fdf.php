<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Admin/products.latte

class Template5f56e459a49522d0862d56b89a942fdf extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('52a60d020e', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lb009a594113_content')) { function _lb009a594113_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?>    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <div class="comments">

    </div>
    <div class="container">
        <div class="content">
            <div class="mytitle">Produkty</div>
            <a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:addProduct"), ENT_COMPAT) ?>
">Vytvorit produkt</a></td>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th>id</th>
                    <th>Nazev</th>
                    <th>Popis</th>
                    <th>Cena</th>
                    <th>Datum</th>
                    <th>Obrazek</th>
                </tr>
                </thead>
                <tbody>
<?php $iterations = 0; foreach ($products as $product) { ?>                <div>
                    <tr>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->id, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->nazev, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->popis, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->cena, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->pridani, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($product->obrazek, ENT_NOQUOTES) ?></td>

                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("editProduct", array($product->id)), ENT_COMPAT) ?>
">Uprav produkt</a></td>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("deleteProduct", array($product->id)), ENT_COMPAT) ?>
">Vymaz produkt</a></td>


                    </tr>
                </div>
<?php $iterations++; } ?>
                </tbody>
            </table>
        </div>
    </div>
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