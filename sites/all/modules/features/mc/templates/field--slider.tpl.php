<div class="content-slider-wrapper">
  <div class="content-slider cycle-slideshow"
    data-cycle-fx="fade"
    data-cycle-timeout="5000"
    data-cycle-slides="> .content-slide"
    >
    <?php foreach ($items as $delta => $item): ?>
    <div class="content-slide" style="background-image: url('<?php print trim(render($item['field_image'])); ?>')">
      <div class="content-slide-wrapper">
        <div class="content-slide-inner-wrapper">
          <?php print render($item['field_copy']); ?>
          <?php print render($item['field_link']); ?>
        </div>
      </div>
    </div>
    <?php endforeach; ?>
  </div>
</div>
