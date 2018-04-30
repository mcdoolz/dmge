<?php
// dmge resource type
?>

<?php foreach ($items as $delta => $item): ?>
<div class="<?php print drupal_html_class($item['#markup']); ?> <?php print $classes; ?>"<?php print $attributes; ?>>
    <?php print render($item); ?>
</div>
<?php endforeach; ?>
