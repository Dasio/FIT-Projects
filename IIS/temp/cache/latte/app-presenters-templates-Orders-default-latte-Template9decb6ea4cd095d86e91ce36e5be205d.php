<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Orders/default.latte

class Template9decb6ea4cd095d86e91ce36e5be205d extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('9b5a4daf04', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lb595e9174e2_content')) { function _lb595e9174e2_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
;if (!$user->isLoggedIn()) { ?>
<p>
Nie ste prihlaseny!
</p>
<?php } else { ?>


    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
 <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <div class="comments">

    </div>
    <div class="container">
        <div class="content">
            <div class="mytitle">Objednavky</div>
            <table class="table table-striped">
                <thead>
                <tr>
                    <th>id</th>
<?php if ($user->isInRole('admin')) { ?>
                    <th>id_zakaznik</th>
<?php } ?>
                    <th>Cena</th>
                    <th>Datum prijatia</th>
                    <th>Datum zmeny</th>
                    <th>Stav</th>
                    <th></th>

                </tr>
                </thead>
                <tbody>
<?php $iterations = 0; foreach ($iterator = $_l->its[] = new Latte\Runtime\CachingIterator($objednavky) as $objednavka) { ?>                <div>
                    <tr>
                        <td><?php if ($user->isInRole('admin')) { echo Latte\Runtime\Filters::escapeHtml($objednavka->id, ENT_NOQUOTES) ?>

<?php } else { ?>
                        <?php echo Latte\Runtime\Filters::escapeHtml($iterator->counter, ENT_NOQUOTES) ?>

                        <?php } ?></td>
<?php if ($user->isInRole('admin')) { ?>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($objednavka->id_zakaznik, ENT_NOQUOTES) ?></td>
<?php } ?>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($objednavka->cena, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($objednavka->datum_prijatia, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($objednavka->datum_zmeny_stavu, ENT_NOQUOTES) ?></td>
                        <td><?php echo Latte\Runtime\Filters::escapeHtml($objednavka->stav, ENT_NOQUOTES) ?></td>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Orders:show", array($objednavka->id)), ENT_COMPAT) ?>
"> Detail </a></td>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Orders:edit", array($objednavka->id)), ENT_COMPAT) ?>
"> Edit </a></td>


                    </tr>
                </div>
<?php $iterations++; } array_pop($_l->its); $iterator = end($_l->its) ?>
                </tbody>
            </table>
        </div>
    </div>
<?php } 
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