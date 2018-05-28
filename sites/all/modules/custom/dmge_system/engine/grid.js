/**
 * Make grid.
 */
(function ($, Drupal, window, document, undefined) {
  function set_grid(_size) {

    var __map = $('#map_wrapper');
    var __grid_wrapper = $('#grid_wrapper');
    var _mWidth = __map.width();
    var _mHeight = __map.height();

    $('#grid_settings').addClass('loading');
    console.time();
    __grid_wrapper.empty();
    draw = SVG('grid_wrapper');

    _type = $('input[name=map_grid_type]:checked').val();

    if (_type !== 'None') {

      __grid_wrapper.css('opacity', get_opacity($('#map_grid_opacity')));

      if (!_size) {
        var _size = parseInt($('#map_grid_size').val());
      }

      var _width = _mWidth;
      var _height = _mHeight;

      var _rows = (_height / _size);
      var _cols = (_width / _size);

      var __gridoptions = {size: _size};

      // Single option switch sets an otherwise default.
      switch (_type) {
        case 'H_Hex':
        __gridoptions.orientation = 'Flat';
          break;
      }
      const Hex = Honeycomb.extendHex(__gridoptions);

      Grid = Honeycomb.defineGrid(Hex)

      _rows = (_height / _size);
      _cols = (_width / _size);

    if (_type === 'H_Hex' || _type === 'V_Hex') {
      corners = Hex().corners();
      hexSymbol = draw.symbol()
        .polygon(corners.map(({ x, y }) => `${x}, ${y}`))
        .fill('none')
        .stroke({ width: 1, color: 'white' });

      const __grid = Grid.rectangle({ width: _cols*0.66, height: _rows*0.66 });

      __grid.forEach(hex => {
        const {x, y} = hex.toPoint();

        draw.use(hexSymbol).translate(x, y).click(function(e) {
          if (!$('#grid_wrapper').is('.grid_marking')) {
            return;
          }
          draw.polygon(corners.map(({x, y}) => `${x},${y}`))
          .translate(x, y)
          .stroke({ width: 1, color: 'white' })
          .fill(getRandomColor())
          .attr('class', 'map_token map_mark')
          .click(function(e) {
            $(e.target).remove();
          });
        });

      });

    }

    if (_type === 'Quad') {
      quadSymbol = draw.rect(_size, _size).fill('none')
      .stroke({ width: 1, color: 'white' })
      .fill('none');

      const __grid = Grid.rectangle({ width: _cols, height: _rows });

      __grid.forEach(hex => {
        _x = hex.x*_size;
        _y = hex.y*_size;
        draw.use(quadSymbol).translate(_x, _y).click(function(e) {
          if (!$('#grid_wrapper').is('.grid_marking')) {
            return;
          }
          draw.rect(_size, _size)
          .stroke({ width: 1, color: 'white' })
          .fill(getRandomColor())
          .attr('class', 'map_token map_mark')
          .translate(hex.x*_size, hex.y*_size)
          .click(function(e) {
            $(e.target).remove();
          });
        });
      });
    }
  } else {
      __grid_wrapper.empty();
    }
    console.timeEnd();
    __grid_wrapper.css({
      'top': __map.css('top'),
      'left': __map.css('left'),
      'width': _mWidth,
      'height': _mHeight,
    });
    $('#grid_settings').removeClass('loading');
  }


  function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }
})(jQuery, Drupal, this, this.document);
