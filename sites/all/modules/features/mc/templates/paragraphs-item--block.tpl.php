<?php
$bg = render($content['field_image']);
$block = render($content['field_block']);
?>
<div class="block-wrapper" <?php if (!empty($bg)) : ?>style="background: url(<?php print $bg; ?>) no-repeat; background-size: cover"<?php endif; ?>>
  <?php if (!empty($block)) : ?>
    <div class="block-inner-wrapper"><?php print $block; ?></div>
  <?php endif; ?>
</div>
