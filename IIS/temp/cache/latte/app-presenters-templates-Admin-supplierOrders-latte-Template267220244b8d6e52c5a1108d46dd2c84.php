<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Admin/supplierOrders.latte

class Template267220244b8d6e52c5a1108d46dd2c84 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('cc3520aff6', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lbf60d46d4b5_content')) { function _lbf60d46d4b5_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?>    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <div class="comments">

    </div>
    <div class="container">
        <div class="content">
            <div class="mytitle">Objednavky na sklad</div>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th>id</th>
                    <th>Email zamestnanca</th>
                    <th>Nazev firmy</th>
                    <th>Nazev produktu</th>
                    <th>Mnozstvo</th>
                    <th>Doba</th>
                    <th>Stav</th>
                </tr>
                </thead>
                <tbody>
<?php $iterations = 0; foreach ($orders as $order) { ?>                <div>
                    <tr>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->id, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->email, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->firma, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->produkt, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->mnozstvo, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->doba, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($order->stav, ENT_NOQUOTES) ?></td>
<?php if ($order->stav == 'Objednane') { ?>
                            <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("acceptSupplierOrder", array($order->id)), ENT_COMPAT) ?>
">Potvrdit</a></td>
<?php } ?>


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