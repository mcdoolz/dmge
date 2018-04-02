<?php
/**
 * @file field--fences-dl.tpl.php
 * Wrap all field values in a single <dl> element.
 *
 * @see http://developers.whatwg.org/grouping-content.html#the-dl-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<dl class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php foreach ($items as $delta => $item): ?>
    <?php print render($item); ?>
  <?php endforeach; ?>

</dl>
