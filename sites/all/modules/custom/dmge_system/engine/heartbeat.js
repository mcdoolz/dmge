/**
 * Loaded by the player view.
 */
(function ($, Drupal, window, document, undefined) {
  var mainMap, mainFOW, mainFOW_size;
  var playerMap = $('#player_map')[0];
  // var pMapCtx = playerMap.getContext('2d');
  // pMapCtx.drawImage(mainMap, 0, 0);
  var playerFOW = $('#player_fow')[0];
  var __playerFOW = new Image();
  const pFOWCtx = playerFOW.getContext('2d');

  function player_heartbeat() {
    mainFOW_size = JSON.parse(localStorage.getItem('mainFOW_size'));
    if (__playerFOW.height !== mainFOW_size.height) {
      console.log(mainFOW_size);
      $('canvas').height(mainFOW_size.height).width(mainFOW_size.width);
    }

    if (__playerFOW.src !== localStorage.getItem('mainFOW')) {
      // __mainMap = localStorage.getItem("mainMap");
      __playerFOW.onload = function() {
        pFOWCtx.clearRect(0, 0, playerFOW.width, playerFOW.height);
        pFOWCtx.drawImage(__playerFOW, 0, 0);
      }
      __playerFOW.src = localStorage.getItem("mainFOW");
    }
  }
  const playerHeartbeat = setInterval(player_heartbeat, 1000);

})(jQuery, Drupal, this, this.document);
