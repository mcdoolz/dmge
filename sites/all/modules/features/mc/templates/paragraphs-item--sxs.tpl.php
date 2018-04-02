<?php
  $sxs = $content['field_sxs_flip']['#items'][0]['value'];
  $sxs_bg = render($content['field_image']);
  $sxs_title = render($content['field_title']);
  $sxs_block = render($content['field_block']);
  $sxs_image = render($content['field_image_style']);
  $sxs_colour = render($content['field_colour']);
  if (!empty($sxs_colour)) { $sxs_colour = HEX_to_RGB(trim(render($content['field_colour'])), 0.66); }
  $sxs_copy = render($content['field_copy']);
  if (empty($sxs)) { $sxs = 'left'; } else { $sxs = 'right'; }
?>
<div class="<?php print 'sxs sxs-'.$sxs; ?>" <?php if (!empty($sxs_bg)) : ?>style="background: url(<?php print $sxs_bg; ?>) no-repeat; background-size: cover"<?php endif; ?>>
  <div class="sxs-inner-wrapper">
    <?php print $sxs_title; ?>
    <?php print $sxs_block; ?>
    <?php print $sxs_image; ?>
    <?php if (!empty($sxs_copy)) : ?>
    <div class="sxs-copy-wrapper" <?php if (!empty($sxs_colour)) : ?>style="background: <?php print $sxs_colour; ?>; padding: 20px"<?php endif; ?>>
      <?php print $sxs_copy; ?>
    </div>
    <?php endif; ?>
  </div>
</div>
