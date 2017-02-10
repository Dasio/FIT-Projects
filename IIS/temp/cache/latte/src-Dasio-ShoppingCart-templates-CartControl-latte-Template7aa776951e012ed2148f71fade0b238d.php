<?php
// source: C:\Program Files (x86)\EasyPHP-DevServer-14.1VC11\data\localweb\github\vendor\dasio\shopping-cart\src\Dasio\ShoppingCart/templates/CartControl.latte

class Template7aa776951e012ed2148f71fade0b238d extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('6a83f6b837', 'html')
;
// prolog Nette\Bridges\ApplicationLatte\UIMacros

// snippets support
if (empty($_l->extends) && !empty($_control->snippetMode)) {
	return Nette\Bridges\ApplicationLatte\UIRuntime::renderSnippets($_control, $_b, get_defined_vars());
}

//
// main template
//
if ($cart->empty) { ?>
<p>
	<?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Váš košík je prázdny.'), ENT_NOQUOTES) ?>

</p>

<?php } else { ?>

<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormBegin($form = $_form = $_control["updateForm"], array('class'=>'form-inline')) ?>

<?php $iterations = 0; foreach ($form->errors as $error) { ?>	<div class="alert alert-error fade in">
		<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
		<?php echo Latte\Runtime\Filters::escapeHtml($template->translate($error), ENT_NOQUOTES) ?>

	</div>
<?php $iterations++; } ?>
	<table class="table table-hover table-striped" id="cart">
		<thead>
			<tr>
<?php if ($showImage+$showName) { ?>				<th class="name" colspan="<?php echo Latte\Runtime\Filters::escapeHtml($showImage+$showName, ENT_COMPAT) ?>
"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Produkt'), ENT_NOQUOTES) ?></th>
<?php } if ($showPrice) { ?>				<th class="price"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Cena'), ENT_NOQUOTES) ?></th>
<?php } if ($showQuantity) { ?>				<th class="quantity"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Množství'), ENT_NOQUOTES) ?></th>
<?php } if ($showTotalWithoutVat) { ?>				<th class="total-without-vat"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Cena bez DPH'), ENT_NOQUOTES) ?></th>
<?php } if ($showTotal) { ?>				<th class="total"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Celkovo'), ENT_NOQUOTES) ?></th>
<?php } if ($showDelete) { ?>				<th class="delete"></th>
<?php } ?>
			</tr>
		</thead>
		<tbody>
<?php $iterations = 0; foreach ($iterator = $_l->its[] = new Latte\Runtime\CachingIterator($cart->getItems()) as $key => $item) { ?>			<tr>
<?php if ($showImage) { ?>				<td class="image">
<?php if ($_l->ifs[] = ($item->link)) { ?>					<a href="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($presenter->link($item->link, $item->linkArgs)), ENT_COMPAT) ?>
" title="<?php echo Latte\Runtime\Filters::escapeHtml($item->name, ENT_COMPAT) ?>" class="thumbnail">
<?php } ?>
						<img src="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($item->image), ENT_COMPAT) ?>">
<?php if (array_pop($_l->ifs)) { ?>					</a>
<?php } ?>
				</td>
<?php } if ($showName) { ?>				<td class="name">
<?php if ($_l->ifs[] = ($item->link)) { ?>					<a href="<?php echo Latte\Runtime\Filters::escapeHtml(Latte\Runtime\Filters::safeUrl($presenter->link($item->link, $item->linkArgs)), ENT_COMPAT) ?>
" title="<?php echo Latte\Runtime\Filters::escapeHtml($item->name, ENT_COMPAT) ?>
"><?php } echo Latte\Runtime\Filters::escapeHtml($item->name, ENT_NOQUOTES) ;if (array_pop($_l->ifs)) { ?>
</a><?php } ?>
<br>
<?php $iterations = 0; foreach ($iterator = $_l->its[] = new Latte\Runtime\CachingIterator($item->options) as $name => $option) { ?>
						<strong><?php echo Latte\Runtime\Filters::escapeHtml($name, ENT_NOQUOTES) ?>
:</strong> <?php echo Latte\Runtime\Filters::escapeHtml($option, ENT_NOQUOTES) ;if (!$iterator->isLast()) { ?>
, <?php } ?>

<?php $iterations++; } array_pop($_l->its); $iterator = end($_l->its) ?>
				</td>
<?php } Tracy\Debugger::barDump(($item), '$item') ;if ($showPrice) { ?>				<td class="price"><?php echo Latte\Runtime\Filters::escapeHtml($template->price($item->price), ENT_NOQUOTES) ?></td>
<?php } if ($showQuantity) { ?>				<td class="quantity"><input class="form-control"<?php $_input = is_object($key) ? $key : $_form[$key]; echo $_input->{method_exists($_input, 'getControlPart')?'getControlPart':'getControl'}()->addAttributes(array (
  'class' => NULL,
))->attributes() ?>> <?php echo Latte\Runtime\Filters::escapeHtml($item->unit, ENT_NOQUOTES) ?></td>
<?php } if ($showTotalWithoutVat) { ?>				<td class="total-without-vat"><strong><?php echo Latte\Runtime\Filters::escapeHtml($template->price($item->totalWithoutVat), ENT_NOQUOTES) ?></strong></td>
<?php } if ($showTotal) { ?>				<td class="total"><strong><?php echo Latte\Runtime\Filters::escapeHtml($template->price($item->total), ENT_NOQUOTES) ?></strong></td>
<?php } if ($showDelete) { ?>				<td class="delete"><a class="btn btn-danger" title="Delete" href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("delete", array($key)), ENT_COMPAT) ?>
"><i class="fa fa-trash-o"></i></a></td>
<?php } ?>
			</tr>
<?php $iterations++; } array_pop($_l->its); $iterator = end($_l->its) ?>		</tbody>
		<tfoot>
			<tr>
<?php if ($showImage+$showName+$showPrice) { ?>				<td colspan="<?php echo Latte\Runtime\Filters::escapeHtml($showImage+$showName+$showPrice, ENT_COMPAT) ?>"></td>
<?php } if ($showQuantity) { ?>				<td><button class="btn btn-warning" type="submit" name="update"><i class="fa fa-refresh"></i> <?php echo Latte\Runtime\Filters::escapeHtml($template->translate($updateName), ENT_NOQUOTES) ?></button></td>
<?php } if ($showTotalWithoutVat+$showTotal+$showDelete) { ?>				<td colspan="<?php echo Latte\Runtime\Filters::escapeHtml($showTotalWithoutVat+$showTotal+$showDelete, ENT_COMPAT) ?>" class="total">
					<strong><?php echo Latte\Runtime\Filters::escapeHtml($template->translate('Celkově'), ENT_NOQUOTES) ?>
:&nbsp;&nbsp;<span><?php echo Latte\Runtime\Filters::escapeHtml($template->price($cart->total), ENT_NOQUOTES) ?></span></strong>
				</td>
<?php } ?>
			</tr>
		</tfoot>
	</table>
	<button class="btn btn-default pull-left" type="submit" name="continue"><i class="fa fa-arrow-circle-left">&nbsp; <?php echo Latte\Runtime\Filters::escapeHtml($template->translate($continueName), ENT_NOQUOTES) ?></i></button>
	<button class="btn btn-success btn-lg pull-right" type="submit" name="next"><?php echo Latte\Runtime\Filters::escapeHtml($template->translate($nextName), ENT_NOQUOTES) ?> &nbsp;<i class="fa fa-arrow-circle-right"></i></button>
<?php echo Nette\Bridges\FormsLatte\Runtime::renderFormEnd($_form) ?>


<?php } 
}}