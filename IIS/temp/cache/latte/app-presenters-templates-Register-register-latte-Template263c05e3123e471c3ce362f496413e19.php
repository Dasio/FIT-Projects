<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Register/register.latte

class Template263c05e3123e471c3ce362f496413e19 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('f61099cad5', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lb7c7be6bf36_content')) { function _lb7c7be6bf36_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><!DOCTYPE html>
<html lang="en" xmlns:n="http://www.w3.org/1999/xhtml">
<head>

    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
    <script src="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($basePath), ENT_COMPAT) ?>/js/string-length.js"></script>
    <script>
        $(document).ready(function ()
        {
            $("#osobniUdaje").hide();
                $("#frm-registrationForm-agree").click(function ()
                {
                    if(document.getElementById('frm-registrationForm-agree').checked) {
                        $("#osobniUdaje").show();}
                    else {
                        $("#osobniUdaje").hide();}
                });
        });
    </script>
</head>
<body>



<div class="container">
    <div class="row">
        <div class="col-sm-6 col-md-4 col-md-offset-4">
            <h1 class="text-center login-title">Registration</h1>
            <div class="account-wall">
                <form class="form-signin"<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormBegin($form = $_form = $_control["registrationForm"], array (
  'class' => NULL,
), FALSE) ?>>
                        <p align="center">Registrovaci udaje</p>
                    <input type="text" class="form-control" placeholder="Vase jmeno" required autofocus<?php $_input = $_form["jmeno"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder="Prijmeni" required autofocus<?php $_input = $_form["prijmeni"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                    <input type="text" class="form-control" placeholder= "e-mail" required autofocus<?php $_input = $_form["email"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
<?php if ($form['email']->hasErrors()) { ?>
                            <?php echo Latte\Runtime\Filters::escapeHtml($_form["email"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        <input type="password" class="form-control" placeholder="Heslo" required<?php $_input = $_form["heslo"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
))->attributes() ?>>
<?php if ($form['heslo']->hasErrors()) { ?>
                        <?php echo Latte\Runtime\Filters::escapeHtml($_form["heslo"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        <input type="password" class="form-control" placeholder="Potvrzeni hesla" required<?php $_input = $_form["heslo2"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
))->attributes() ?>>
<?php if ($form['heslo2']->hasErrors()) { ?>
                            <?php echo Latte\Runtime\Filters::escapeHtml($_form["heslo2"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>

                    <p align="center">Osobni udaje</p>
                    <div<?php if ($_l->tmp = array_filter(array('form-signin'))) echo ' class="', Latte\Runtime\Filters::escapeHtml(implode(" ", array_unique($_l->tmp)), ENT_COMPAT), '"' ?>>
                        <?php echo $_form["agree"]->getControl()->addAttributes(array('class' => 'checkbox')) ?>

                    </div>
                    <div id="osobniUdaje">
                        <input type="text" class="form-control"  placeholder="Telefonni cislo(bez mezer)"maxlength="9"<?php $_input = $_form["telefon"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'maxlength' => NULL,
))->attributes() ?>>
<?php if ($form['telefon']->hasErrors()) { ?>
                        <?php echo Latte\Runtime\Filters::escapeHtml($_form["telefon"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        <input type="text" class="form-control" placeholder="Mesto"<?php $_input = $_form["mesto"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
))->attributes() ?>>
<?php if ($form['mesto']->hasErrors()) { ?>
                            <?php echo Latte\Runtime\Filters::escapeHtml($_form["mesto"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        <input type="text" class="form-control" placeholder="Adresa"<?php $_input = $_form["ulice"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
))->attributes() ?>>
<?php if ($form['ulice']->hasErrors()) { ?>
                            <?php echo Latte\Runtime\Filters::escapeHtml($_form["ulice"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        <input type="text" class="form-control" placeholder="PSC(bez mezer)" maxlength="5"<?php $_input = $_form["psc"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'maxlength' => NULL,
))->attributes() ?>>
<?php if ($form['psc']->hasErrors()) { ?>
                        <?php echo Latte\Runtime\Filters::escapeHtml($_form["psc"]->getError(), ENT_NOQUOTES) ?>

<?php } ?>
                        </div>
                    <button class="btn btn-lg btn-primary btn-block" type="submit"<?php $_input = $_form["login"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'class' => NULL,
  'type' => NULL,
))->attributes() ?>>
                       Register</button>

                <?php echo Nette\Bridges\FormsLatte\Runtime::renderFormEnd($_form, FALSE) ?></form>
            </div>
        </div>
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