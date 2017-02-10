<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Sign/in.latte

class Template89eaad5ac7770acac578365d36d9d04e extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('4a1dccee11', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lb00f49e7751_content')) { function _lb00f49e7751_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><!DOCTYPE html>
<html lang="en">
<head>
  <title>Bootstrap Example</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
  <script src="http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
</head>
<body>

<div class="container"> 
     <div class="row"> 
         <div class="col-sm-6 col-md-4 col-md-offset-4"> 
             <h1 class="text-center login-title">Prihlasovani</h1>
             <div class="account-wall">  
                 <form class="form-signin"<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormBegin($form = $_form = $_control["signInForm"], array (
  'class' => NULL,
), FALSE) ?>> 
                 <input type="text" class="form-control" placeholder="E-mail" required autofocus<?php $_input = $_form["jmeno"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
  'autofocus' => NULL,
))->attributes() ?>>
                 <input type="password" class="form-control" placeholder="Heslo" required<?php $_input = $_form["heslo"]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'type' => NULL,
  'class' => NULL,
  'placeholder' => NULL,
  'required' => NULL,
))->attributes() ?>>
                 <button class="btn btn-lg btn-primary btn-block" type="submit"> 
                     Sign in</button> 
                 <label class="checkbox pull-left"> 


                 </label>
                 <a href="#" class="pull-right need-help"> </a><span class="clearfix"></span>
                 <?php echo Nette\Bridges\FormsLatte\Runtime::renderFormEnd($_form, FALSE) ?></form> 
             </div> 
             <a class="text-center new-account" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Register:register"), ENT_COMPAT) ?>
">Vytvorit ucet </a>
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