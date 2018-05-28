(function ($, Drupal, window, document, undefined) {

  // Check for the various File API support.
  if (window.File && window.FileReader && window.FileList && window.Blob) {
    // Great success! All the File APIs are supported.
  } else {
    alert("This browser does not support local file interactions.  You can't read files.  Switch or upgrade your browser.");
  }

  if (typeof(Storage) !== "undefined") {
      var locker = window.localStorage;

  } else {
      alert("This browser is old and does not support local storage.  You can't save data.  Upgrade your browser.");
  }

  Drupal.behaviors.dmge_init = {
    attach: function(context, settings) {
      $('body').prepend('<div id="memory"><div id="memory_available"></div><div id="memory_used"></div></div>');
    }
  }

  Drupal.howto = $('#about');
  Drupal.howto.dialog({
    modal: true,
    draggable: true,
    hide: { effect: "fade", duration: 250 },
    show: { effect: "fade", duration: 250 },
    width: ($(window).width())/2
  });

})(jQuery, Drupal, this, this.document);
