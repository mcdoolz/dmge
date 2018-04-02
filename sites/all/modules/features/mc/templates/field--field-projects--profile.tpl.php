
<?php if ($label):?>

  <h3 class="<?php print $classes; ?>-label field-label<?php if ($element['#label_display'] == 'inline') { print " inline-sibling"; } ?>"<?php print $title_attributes; ?>>
    <?php print $label; ?>
  </h3>
<?php endif; ?>

<?php
  foreach ($items as $delta => $item):
    $_node = array_shift($item['node']);
  ?>
  <div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <div class="profile-project-title"><?php print $_node['#node']->title; ?></div>
    <div class="profile-project-copy">
      <?php
      $string = strip_tags(render($_node['field_content']));
      print (strlen($string) > 13) ? substr($string, 0, 300) . '...' : $string;
      ?>
    </div>
  <?php
    $path = drupal_get_path_alias('node/' . $_node['#node']->nid);
    print l(t('Read More'), $path, array('attributes' => array('class' => 'profile-project-link')));
    ?>
  </div>
<?php endforeach; ?>
