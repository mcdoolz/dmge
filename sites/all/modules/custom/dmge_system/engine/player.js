/**
 * Loaded by the player view.
 */
(function ($, Drupal, window, document, undefined) {

  if (window.opener) {
    var main_window = window.opener;
    console.log(window.opener);
  }

  var updateScrollPos = function(e) {
    $('html').css('cursor', 'nwse-resize');
    $(window).scrollTop($(window).scrollTop() + (clickY - e.pageY));
    $(window).scrollLeft($(window).scrollLeft() + (clickX - e.pageX));
  }

  var clicked = false, clickY, clickX;
  $(document).on({
    'mousemove': function(e) {
      clicked && updateScrollPos(e);
    },
    'mousedown': function(e) {
      if (e.altKey === true) {
        clicked = true;
        clickY = e.pageY;
        clickX = e.pageX;
      }
    },
    'mouseup': function() {
      clicked = false;
      $('html').css('cursor', 'auto');
    }
  });

})(jQuery, Drupal, this, this.document);
