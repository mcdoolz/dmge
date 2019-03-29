/**
 * Loaded by the player view.
 */
(function ($, Drupal, window, document, undefined) {
  $('#player_grid_wrapper').draggable();

  if (!window['opener']) {
    console.log('Where is the parent window?');
  }

  $('canvas').change(function() {
    if (this.width <= 0 || this.height <= 0) {
      console.log('Something failed in the canvas size.  We are altering the canvas size for everyones safety.  Pray we do not alter it further.');
      update_canvases();
    }
  });

  $(window).change(function() {
    update_canvases();
  });

  // Just fixes sizing based on available data.
  const update_canvases = function() {
    let $parent = $(window['opener']),
    $parent_body = $(window['opener'].document.body),
    width = $parent_body.find('#map_width').val(),
    height = $parent_body.find('#map_height').val();

    if (!width || !height) {
      width = window.screen.availWidth;
      height = window.screen.availHeight;
    }

    this.width = width;
    this.height = height;
  }

  const updateScrollPos = function(e) {
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
