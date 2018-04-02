(function ($, Drupal, window, document, undefined) {
  console.log('here');
  Drupal.behaviors.mc_register = {
    attach: function(context, settings) {
      $("form#user-login").submit(function(e) {
        fields = $('form#user-login input');
        fields.each(function(e)) {
          console.log(e);
        }
    });
    }
  }
})(jQuery, Drupal, this, this.document);
