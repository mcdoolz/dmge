<?php
// dmge resource tags
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
<?php foreach ($items as $delta => $item): ?>
    <?php print render($item); ?>
<?php endforeach; ?>
</div>
