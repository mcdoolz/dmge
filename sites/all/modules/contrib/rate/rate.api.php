<?php

/**
 * @file
 * Defines module hooks.
 */

/**
 * Define templates for rate widgets.
 *
 * @return array
 */
function hook_rate_widgets() {
  $templates = array();

  $templates['thumbs_up_down'] = new stdClass();
  $templates['thumbs_up_down']->title = t('Thumbs up / down');
  $templates['thumbs_up_down']->class = 'RateWidget';
  $templates['thumbs_up_down']->file = 'rate.widget.inc';

  $templates['fivestar'] = new stdClass();
  $templates['fivestar']->title = t('Fivestar');
  $templates['fivestar']->class = 'RateWidget';
  $templates['fivestar']->file = 'rate.widget.inc';

  return $templates;
}
