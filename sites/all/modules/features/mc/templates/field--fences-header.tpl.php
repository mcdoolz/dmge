<?php
/**
 * @file field--fences-header.tpl.php
 * Wrap each field value in the <header> element.
 *
 * @see http://developers.whatwg.org/sections.html#the-header-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <header class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </header>
<?php endforeach; ?>
