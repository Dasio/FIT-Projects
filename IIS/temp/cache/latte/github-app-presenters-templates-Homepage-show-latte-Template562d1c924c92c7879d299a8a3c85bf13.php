<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Homepage/show.latte

class Template562d1c924c92c7879d299a8a3c85bf13 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('4bf95119af', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lb1b48e15bae_content')) { function _lb1b48e15bae_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><div class="page-container">
<div class="col-md-1">
    <div class="lead"></div>
    <div class="list-group">
        <a class="list-group-item" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("show", array('skicak')), ENT_COMPAT) ?>
">Skicaky</a>
        <a class="list-group-item" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("show", array('pastelka')), ENT_COMPAT) ?>
">Pastelky</a>

    </div>
</div>
    <!-- bloc-0 -->
    <div class="bloc l-bloc " id="bloc-0">
        <div class="container bloc-lg">
            <div class="row">

<?php $iterations = 0; foreach ($flashes as $flash) { ?>                    <div
                         class="alert alert-<?php echo Latte\Runtime\Filters::escapeHtml($flash->type, ENT_COMPAT) ?>
"><?php echo Latte\Runtime\Filters::escapeHtml($flash->message, ENT_NOQUOTES) ?></div>
<?php $iterations++; } $iterations = 0; foreach ($products as $product) { ?>
                    <div class="col-sm-4">
                        <h3 class="mg-md">
                            <a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Product:show", array($product->id)), ENT_COMPAT) ?>
"><?php echo Latte\Runtime\Filters::escapeHtml($product->nazev, ENT_NOQUOTES) ?></a>
                        </h3>
                        <img src="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($product->obrazek), ENT_COMPAT) ?>" class="img-responsive">
                        <div><?php echo Latte\Runtime\Filters::escapeHtml($product->popis, ENT_NOQUOTES) ?></div>
                        </div>
<?php $iterations++; } ?>

                    </div>

                </div>


        </div>
    </div>
    <!-- bloc-0 END -->
</div>                </div>




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
if ($_l->extends) { ob_end_clean(); return $template->renderChildTemplate($_l->extends, get_defined_vars()); }
call_user_func(reset($_b->blocks['content']), $_b, get_defined_vars()) ; 
}}