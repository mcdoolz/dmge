/**
 * @file
 * Handles the JS for the droogle file browser.
 */


(function ($) {

  Drupal.behaviors.droogleBrowser = {
    attach: function (context, settings) {

      // Container for the files that get passed back to the browser
      var files = {};

      // Disable the links on media items list
      var $links = $('.view-droogle-browser a');
      $links.click(function() {
        $links.removeClass('selected');
        var $this = $(this);
        $this.addClass('selected');
        $("input[name='submitted-document']").val($this.attr('href'));
        return false;
      });
    }
  }

}(jQuery));