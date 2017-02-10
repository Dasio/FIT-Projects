<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\app\presenters/templates/Admin/users.latte

class Templatebf003dff39bcc2334734fac9ca372343 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('0f2a939000', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lbe0ab564e8d_content')) { function _lbe0ab564e8d_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><div class="content">
        <div class="mytitle">Uzivatele</div>
        <table class="table table-striped">
            <thead>
            <tr>
                <th>id</th>
                <th>Jmeno</th>
                <th>Prijmeni</th>
                <th>E-mail</th>
                <th>Role</th>
                <th></th>
            </tr>
            </thead>
<?php $iterations = 0; foreach ($users as $userx) { ?>            <div>
                <tr>
                    <td><?php echo Latte\Runtime\Filters::escapeHtml($userx->id, ENT_NOQUOTES) ?></td>
                    <td><?php echo Latte\Runtime\Filters::escapeHtml($userx->jmeno, ENT_NOQUOTES) ?></td>
                    <td><?php echo Latte\Runtime\Filters::escapeHtml($userx->prijmeni, ENT_NOQUOTES) ?></td>
                    <td><?php echo Latte\Runtime\Filters::escapeHtml($userx->email, ENT_NOQUOTES) ?></td>
                    <td><?php echo Latte\Runtime\Filters::escapeHtml($userx->role, ENT_NOQUOTES) ?></td>

<?php if ($userx->role != 'admin' && $user->isAllowed('Admin','editUser')) { ?>
                        <td><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Admin:editUser", array($user->id)), ENT_COMPAT) ?>
"> Edit </a></td>
<?php } ?>
                </tr>
            </div>
<?php $iterations++; } ?>
        </table>
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
if ($_l->extends) { ob_end_clean(); return $template->renderChildTemplate($_l->extends, get_defined_vars()); }
call_user_func(reset($_b->blocks['content']), $_b, get_defined_vars()) ; 
}}