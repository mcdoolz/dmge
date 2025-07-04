<?php
/**
 * @file field--fences-del.tpl.php
 * Wrap each field value in the <del> element.
 *
 * @see http://developers.whatwg.org/edits.html#the-del-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <del class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </del>
<?php endforeach; ?>
