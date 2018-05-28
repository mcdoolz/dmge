/**
 * Initialize the fog of war.
 */
(function ($, Drupal, window, document, undefined) {
  function init_fow(__height, __width){
    var fow = $( '#fow' ),
        ctx = fow[0].getContext( '2d' ),
        r1 = $('#fow_brush_size').val(),
        r2 = $('#fow_brush_feather_size').val(),
        dragging = false;

    fow.attr('height', __height).attr('width', __width);
    ctx.fillStyle = 'rgba( 0, 0, 0, 1 )';
    ctx.fillRect( 0, 0, __width, __height );

    $('#fow').on('mousedown', function() {
      dragging = true;
    });
    $('#fow').on('mouseup', function() {
      dragging = false;
    });

    $('#fow').on('mousemove', function(ev, ev2){
      if (dragging) {
        r1 = $('#fow_brush_size').val()/2;
        r2 = $('#fow_brush_feather_size').val()/2;

        ev2 && ( ev = ev2 );

        var pX = ev.pageX,
            pY = ev.pageY;

        var radGrd = ctx.createRadialGradient( pX, pY, r1, pX, pY, r2 );
        radGrd.addColorStop(0, 'rgba( 0, 0, 0,  1 )' );
        radGrd.addColorStop(0.25, 'rgba( 0, 0, 0, .1 )' );
        radGrd.addColorStop(1, 'rgba( 0, 0, 0, 0 )' );

        if (shifted !== false) {
          ctx.globalCompositeOperation="source-over";
        } else {
          ctx.globalCompositeOperation = 'destination-out';
        }

        ctx.fillStyle = radGrd;
        ctx.fillRect( pX - r2, pY - r2, r2*2, r2*2 );
      };
    });
  }
})(jQuery, Drupal, this, this.document);
