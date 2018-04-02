(function ($, Drupal, window, document, undefined) {

  Drupal.behaviors.mc_init = {
    attach: function(context, settings) {
      $('.cta-title').matchHeight('.cta-title');
      $('.cta-copy').matchHeight('.cta-copy');
    }
  }


})(jQuery, Drupal, this, this.document);
