<?php
$image_bg = render($content['field_image']);
$image_copy = render($content['field_copy']);
?>
<div class="image-wrapper" style="background: url(<?php print $image_bg; ?>) no-repeat; background-size: cover">
  <?php if (!empty($image_copy)) : ?>
    <div class="image-copy-wrapper"><?php print $image_copy; ?></div>
  <?php endif; ?>
</div>
