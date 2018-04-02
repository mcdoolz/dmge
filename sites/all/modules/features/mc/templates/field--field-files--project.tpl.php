<?php
/**
 * @file field--fences-ul.tpl.php
 * Wrap each field value in the <li> element and all of them in the <ul> element.
 *
 * @see http://developers.whatwg.org/grouping-content.html#the-ul-element
 */
?>
<?php if ($label): ?>
  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<ul class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php foreach ($items as $delta => $item): ?>
    <li<?php print $item_attributes[$delta]; ?>>
      <?php print render($item); ?>
      <span class="project-file-download-link">
        <?php print l('Download', file_create_url($item['#file']->uri)); ?>
      </span>
    </li>
  <?php endforeach; ?>

</ul>
