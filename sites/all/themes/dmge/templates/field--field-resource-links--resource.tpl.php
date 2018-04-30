<?php
// dmge resource links
?>
<div class="resource-links">
<?php
  foreach ($items as $delta => $item):
  $url = $item['field_resource_link'][0]['#element']['url'];
  ?>
  <div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <a href="<?php print $item['field_resource_link'][0]['#element']['url']; ?>" target="_blank">
      <i class="<?php print $item['field_resource_link_type'][0]['#options']['entity']->field_fontawesome_classname[LANGUAGE_NONE][0]['value']; ?>">&nbsp;</i>
      <?php print $item['field_resource_link_type'][0]['#title']; ?>
    </a>
    <?php if (_dmge_is_patreon($url)) : ?>
      <a href="<?php print _dmge_rewrite_url($url); ?>" target="_blank">
        <i class="fas fa-external-link-alt" target="_blank">&nbsp;</i>
        Patreon Post
      </a>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
</div>
