<?php
/**
 * @file field--fences-footer.tpl.php
 * Wrap each field value in the <footer> element.
 *
 * @see http://developers.whatwg.org/sections.html#the-footer-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <footer class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
  </footer>
<?php endforeach; ?>
