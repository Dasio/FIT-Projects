<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/@layout.latte

class Templatecc8a9dfbef667d7260f1d67b786a56a5 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('181bc05e99', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block head
//
if (!function_exists($_b->blocks['head'][] = '_lb855d558965_head')) { function _lb855d558965_head($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
;
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
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">

    <title><?php if (isset($_b->blocks["title"])) { ob_start(); Latte\Macros\BlockMacrosRuntime::callBlock($_b, 'title', $template->getParameters()); echo $template->striptags(ob_get_clean()) ?>
 | <?php } ?> Internetovy obchod </title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>

    <link rel="stylesheet" href="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($basePath), ENT_COMPAT) ?>/css/style.css">
    <link rel="stylesheet" href="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($basePath), ENT_COMPAT) ?>/css/bootstap.min.css">
    <link rel="shortcut icon" href="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($basePath), ENT_COMPAT) ?>/favicon.ico">
    <meta name="viewport" content="width=device-width">


<?php if ($_l->extends) { ob_end_clean(); return $template->renderChildTemplate($_l->extends, get_defined_vars()); }
call_user_func(reset($_b->blocks['head']), $_b, get_defined_vars())  ?>
</head>

<body>

<nav class="navbar navbar-default">
    <div class="container-fluid">
        <!-- Brand and toggle get grouped for better mobile display -->
        <div class="navbar-header">

            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse"
                    data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>

            <a class="navbar-brand" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Homepage:"), ENT_COMPAT) ?>
">E-Shop</a>
        </div>


        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav">

<?php if ($user->isAllowed('Orders','default')) { ?>
                    <li class="nav navbar-nav navbar-right"><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Orders:default"), ENT_COMPAT) ?>
"> Objednavky </a></li>
<?php } ?>

            </ul>
            <ul class=" nav navbar-nav navbar-right">
<?php if ($user->isAllowed('Admin','show')) { ?>
                <li class="dropdown">
                    <a href="#" data-toggle="dropdown" class="dropdown-toggle">Administracia <b class="caret"></b></a>
                    <ul class="dropdown-menu">
                        <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:products"), ENT_COMPAT) ?>
">Produkty</a></li>
                        <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:users"), ENT_COMPAT) ?>
">Uzivatelia</a></li>
                        <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:suppliers"), ENT_COMPAT) ?>
">Dodavatelia</a></li>
                        <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:supplierOrders"), ENT_COMPAT) ?>
">Objednavky na sklad </a></li>
                    </ul>
                </li>
<?php } if ($user->isAllowed('Cart','default')) { ?>
                    <li class="active"><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Cart:default"), ENT_COMPAT) ?>
">Kosik - <?php echo Latte\Runtime\Filters::escapeHtml($template->price($cart->total), ENT_NOQUOTES) ?> </a></li>
<?php } if ($user->isLoggedIn()) { ?>
                    <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Sign:out"), ENT_COMPAT) ?>
">Odhlasit</a></li>
<?php } else { ?>
                    <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Sign:in"), ENT_COMPAT) ?>
">Přihlásit</a></li>
<?php } ?>
            </ul>


        </div>
        <!-- /.navbar-collapse -->
    </div>
    <!-- /.container-fluid -->
</nav>
<script src="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($basePath), ENT_COMPAT) ?>/js/bootstap.min.js"></script>


<script> document.body.className += ' js' </script>


</div>
<?php Latte\Macros\BlockMacrosRuntime::callBlock($_b, 'content', $template->getParameters()) ?>
</body>
</html>
<?php
}}