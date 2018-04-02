<?php
/**
 * @file field--fences-precode.tpl.php
 * Wrap each field value in the <pre> and <code> elements.
 *
 * @see http://developers.whatwg.org/grouping-content.html#the-pre-element
 * @see http://developers.whatwg.org/text-level-semantics.html#the-code-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <pre class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <code>
      <?php print render($item); ?>
    </code>
  </pre>
<?php endforeach; ?>
