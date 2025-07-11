<?php
/**
 * @file field--fences-h5.tpl.php
 * Wrap each field value in the <h5> element.
 *
 * @see http://developers.whatwg.org/sections.html#the-h1,-h2,-h3,-h4,-h5,-and-h6-elements
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <h5 class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </h5>
<?php endforeach; ?>
