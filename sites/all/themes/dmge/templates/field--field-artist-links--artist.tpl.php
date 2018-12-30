<?php
// dmge artist links
?>

<div class="artist-links">
<?php foreach ($items as $delta => $item): ?>
  <div class="<?php print $classes; ?>"<?php print $attributes; ?>>
    <a href="<?php print $item['field_artist_link'][0]['#element']['url']; ?>" target="_blank">
      <i class="<?php print $item['field_artist_link_type'][0]['#options']['entity']->field_fontawesome_classname[LANGUAGE_NONE][0]['value']; ?>">&nbsp;</i>
      <?php print (!empty($item['field_artist_link_type'])) ? $item['field_artist_link_type'][0]['#title']; ?>
    </a>
  </div>
<?php endforeach; ?>
</div>
