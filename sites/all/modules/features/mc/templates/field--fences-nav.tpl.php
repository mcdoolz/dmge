<?php
/**
 * @file field--fences-nav.tpl.php
 * Wrap each field value in the <nav> element.
 *
 * @see http://developers.whatwg.org/sections.html#the-nav-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <nav class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </nav>
<?php endforeach; ?>
