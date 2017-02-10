<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Product/show.latte

class Template293b762008d4ab246cc8a9a747456cee extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('746bcd2e48', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lbc5d1972268_content')) { function _lbc5d1972268_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><!doctype html>
<html>
<head>

    <title>Produkt </title>
</head>
<body>
<!-- Main container -->
<div class="page-container">

    <!-- bloc-0 -->
    <div class="bloc l-bloc " id="bloc-0">
        <div class="container bloc-lg">
            <div class="row">
                <div class="col-sm-4">
                    <h3 class="mg-md">
                        <?php echo Latte\Runtime\Filters::escapeHtml($product->nazev, ENT_NOQUOTES) ?>

<?php if ($product->sklad != 0) { ?>
                        <p> Skladem </p>
<?php } else { ?>
                        <p> Neni na sklade</p>
<?php if ($doba == '') { ?>
                                <p> Doba dodania nedostupna </p>
<?php } else { ?>
                            <p> Dostupne za <?php echo Latte\Runtime\Filters::escapeHtml($doba, ENT_NOQUOTES) ?> dni </p>
<?php } } ?>
                    </h3>
                    <p><?php echo Latte\Runtime\Filters::escapeHtml($product->popis, ENT_NOQUOTES) ?></p>

                    <div class="col-sm-2">
<?php if ($user->isAllowed('Cart','add') && $product->sklad != 0) { ?>
                        <a class="btn  btn-d  btn-lg" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Cart:add", array($product->id)), ENT_COMPAT) ?>
"> Pridat do kosika</a>
<?php } ?>
                        <p>Prumerne hodnoceni: <?php echo Latte\Runtime\Filters::escapeHtml($avg, ENT_NOQUOTES) ?>

                        </p>
                    </div>

                </div>

                <div class="col-sm-4">
                    <h3 class="mg-md"></h3>
                    <img src="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($product->obrazek), ENT_COMPAT) ?>" class="img-responsive">
            </div>
        </div>
    </div>

    <!-- bloc-0 END -->
    <h4 align="center">Komentare</h4>
    <div class="bloc l-bloc " id="bloc-0">
        <div class="container bloc-lg">
            <div class="row">
                <div class="col-sm-12">
                    <div class="comments">

                    </div>
<?php if ($user->isLoggedIn()) { ?>
                        <form  id="form-1" novalidate<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormBegin($form = $_form = $_control["commentForm"], array (
  'id' => NULL,
  'novalidate' => NULL,
), FALSE) ?>>

                        <div class="form-group">
                                                                                        						<textarea id="message" class="form-control" rows="4" cols="50" required<?php $_input = $_form["content"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'id' => NULL,
  'class' => NULL,
  'rows' => NULL,
  'cols' => NULL,
  'required' => NULL,
))->attributes() ?>><?php echo $_input->getControl()->getHtml() ?></textarea>
                            <div class="content-form"><?php echo $_form["body"]->getControl() ?></div>
                        </div>
                        <button class="bloc-button btn btn-d btn-lg btn-block" type="submit">
                            Publikovat
                        </button>
                        <h3>Recenze uzivatelu</h3>
                    <?php echo Nette\Bridges\FormsLatte\Runtime::renderFormEnd($_form, FALSE) ?></form>
<?php } else { ?>
                        <a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Sign:in"), ENT_COMPAT) ?>
"><h4 align="center">Komentovat muzou jenom registrovani uzivatele</h4></a>
<?php } ?>

<?php $iterations = 0; foreach ($comments as $comment) { ?>
                                        <div class="titleBox">
                        <label><b><?php echo Latte\Runtime\Filters::escapeHtml($users[$comment->id]->jmeno, ENT_NOQUOTES) ?></b></label>


                    </div>
                    <div class="actionBox">
                        <ul class="commentList">
                            <li>
                                <div class="commentText">
                                    <p class=""><?php echo Latte\Runtime\Filters::escapeHtml($comment->content, ENT_NOQUOTES) ?></p> <span class="date sub-text"></span>

                                </div>
                            </li>
                           </ul>
<?php $iterations++; } ?>
                        </form>
                    </div>
                                                    </div>
            </div>
        </div>
    </div>
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