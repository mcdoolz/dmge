<?php
/**
 * @file field--fences-i.tpl.php
 * Wrap each field value in the <i> element.
 *
 * @see http://developers.whatwg.org/text-level-semantics.html#the-i-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <i class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </i>
<?php endforeach; ?>
