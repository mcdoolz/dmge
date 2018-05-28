/**
 * Loaded by the player view.
 */
(function ($, Drupal, window, document, undefined) {
  var mainMap, mainFOW;
  var playerMap = $('#player_map')[0];
  // var pMapCtx = playerMap.getContext('2d');
  // pMapCtx.drawImage(mainMap, 0, 0);
  var playerFOW = $('#player_fow')[0];
  var __playerFOW = new Image();
  const pFOWCtx = playerFOW.getContext('2d');

  function player_heartbeat() {
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
