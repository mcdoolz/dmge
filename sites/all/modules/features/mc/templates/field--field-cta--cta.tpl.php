<?php foreach ($items as $delta => $item):
  if (!empty($item['field_link'])) {
    $title = $item['field_link']['#items'][0]['title'];
    if (empty($title)) {
      $title = t('Read More');
    }
    $url = $item['field_link']['#items'][0]['url'];
  }
  ?>
  <div class="cta">
    <?php print render($item['field_image']); ?>
    <?php print render($item['field_title']); ?>
    <?php print render($item['field_copy']); ?>
    <?php if (!empty($url)) : ?>
      <a class="cta-link" href="<?php print $url; ?>"><?php print $title; ?></a>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
