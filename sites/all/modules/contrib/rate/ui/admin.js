(function ($) {
  Drupal.behaviors.rateAdmin = {
    attach: function(context) {
      if ($('input#edit-images-0').length) {
        $('input').click(function() {
          Drupal.rateAdminChangeOptions();
        });
        Drupal.rateAdminChangeOptions();
      }
    }
  };
  
  Drupal.rateAdminChangeOptions = function() {
    if ($('input#edit-images-0').attr('checked')) {
      $('.form-item-sprites').hide();
      $('.form-item-separate-images').hide();
      $('.form-item-upload').hide();
      $('#edit-imagesource div:eq(2)').hide();
    }
    else {
      $('.form-item-sprites').show();
      $('.form-item-separate-images').show();
      $('.form-item-upload').show();
    }
    if ($('#edit-sprites div:eq(0) input').attr('checked')) {
      $('#edit-imagesource div:eq(2)').show();
    }
    else {
      $('#edit-imagesource div:eq(2)').hide();
    }
  }
})(jQuery);
