<?php
/**
 * @file field--fences-div.tpl.php
 * Wrap each field value in the <div> element.
 *
 * @see http://developers.whatwg.org/grouping-content.html#the-div-element
 */
?>
<?php if ($label):?>

  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php foreach ($items as $delta => $item): ?>
  <div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <?php
    print render($item);
    $temp = $item['entity']['field_collection_item'];
    $temp = array_shift($temp);
    $path = entity_uri('node', $temp['field_funder']['#items'][0]['entity']);
    $path = $path['path'];
    if (!empty($path)) : ?>
    <div class="funding-funder-link">
      <?php print l(t('Read More'), $path); ?>
    </div>
    <?php endif;?>
  </div>
<?php endforeach; ?>
