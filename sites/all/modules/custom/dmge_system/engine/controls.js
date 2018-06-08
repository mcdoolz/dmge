(function ($, Drupal, window, document, undefined) {

  var mainDocument = Drupal.mainDocument = document;
  var mainView = Drupal.mainView = window;

  var grid_canvas = Drupal.grid = new fabric.Canvas('grid', {
    hoverCursor: 'pointer',
    selection: true
  });
  var map_canvas = Drupal.__map = new fabric.Canvas('map', {
    hoverCursor: 'pointer',
    selection: true
  });
  var fow_canvas = Drupal.fow = new fabric.Canvas('fow', {
    hoverCursor: 'pointer',
    selection: true
  });

  fabric.util.requestAnimFrame(function render() {
    map_canvas.renderAll();
    grid_canvas.renderAll();
    fabric.util.requestAnimFrame(render);
  });

  var mainFOW = Drupal.mainFOW = $('#fow')[0];
  localStorage.setItem("map_canvas", map_canvas);
  localStorage.setItem("mainFOW", mainFOW);

  map_canvas.on('mouse:down', function(opt) {
    var evt = opt.e;
    if (evt.altKey === true) {
      this.isDragging = true;
      this.selection = false;
      grid_canvas.lastPosX = this.lastPosX = evt.clientX;
      grid_canvas.lastPosY = this.lastPosY = evt.clientY;
    }
  });
  map_canvas.on('mouse:move', function(opt) {
    if (this.isDragging) {
      var e = opt.e;
      grid_canvas.viewportTransform[4] = this.viewportTransform[4] += e.clientX - this.lastPosX;
      grid_canvas.viewportTransform[5] = this.viewportTransform[5] += e.clientY - this.lastPosY;
      this.requestRenderAll();
      grid_canvas.requestRenderAll();
      grid_canvas.lastPosX = this.lastPosX = e.clientX;
      grid_canvas.lastPosY = this.lastPosY = e.clientY;
    }
  });
  map_canvas.on('mouse:up', function(opt) {
    this.isDragging = false;
    this.selection = true;
  });


  function map_reset_zoom() {
    map_canvas.zoomToPoint();
  }

  /**
   * Helper to set zoom across all canvases.
   */
  function map_zoom(opt, zoom) {
    let canvases = [map_canvas, grid_canvas, fow_canvas];
    canvases.forEach(function(e){
      e.zoomToPoint({ x: opt.e.offsetX, y: opt.e.offsetY }, zoom);
    });
    let _size = parseInt($('#map_grid_size').val());
    _size = _size * map_canvas.getZoom();
    $('#map_grid_display_size').val(_size);
  }

  map_canvas.on('mouse:wheel', function(opt) {
    var delta = opt.e.deltaY;
    var zoom = map_canvas.getZoom();
    zoom = zoom + delta/200;
    if (zoom > 10) {zoom = 10;}
    if (zoom < 1) {zoom = 1;}
    map_zoom(opt, zoom);

    opt.e.preventDefault();
    opt.e.stopPropagation();
    var vpt = this.viewportTransform;
    if (zoom < 400 / 1000) {
      grid_canvas.viewportTransform[4] = this.viewportTransform[4] = 200 - 1000 * zoom / 2;
      grid_canvas.viewportTransform[5] = this.viewportTransform[5] = 200 - 1000 * zoom / 2;
    } else {
      if (vpt[4] >= 0) {
        grid_canvas.viewportTransform[4] = this.viewportTransform[4] = 0;
      } else if (vpt[4] < map_canvas.getWidth() - 1000 * zoom) {
        grid_canvas.viewportTransform[4] = this.viewportTransform[4] = map_canvas.getWidth() - 1000 * zoom;
      }
      if (vpt[5] >= 0) {
        grid_canvas.viewportTransform[5] = this.viewportTransform[5] = 0;
      } else if (vpt[5] < map_canvas.getHeight() - 1000 * zoom) {
        grid_canvas.viewportTransform[5] = this.viewportTransform[5] = map_canvas.getHeight() - 1000 * zoom;
      }
    }
  });

  $(document).on('click', function(e) {
    if ($(e.target).closest('#sidebar').length === 0) {
      $('#sidebar').hide('slide', {direction:'right'});
    }
  });

  /**
   * No right click in the fow of war, it complicates matters.
   */
  $('#fow').contextmenu(function() {
      return false;
  });

  /**
   * Binding an event to a variable toggle.  Computer science in action.
   */
  var shifted = false;
  $(document).on('keyup keydown', function(e){
    shifted = e.shiftKey;
  });

  /**
   * When we press a key, the world changes.
   */
  $(document).bind('keydown', function(e) {
    if (e.which == 27) {
      $('#sidebar').toggle('slide', {direction:'right'});
    }
    if (e.which == 81) {
      __toggle = Drupal.howto.dialog('isOpen') ? 'close' : 'open';
      Drupal.howto.dialog(__toggle);
    }
    if (e.which == 70) {
      if (e.ctrlKey) {
        __fow = localStorage.getItem('fow');
      }
    }
    if (e.which == 86) {
      if ((e.ctrlKey) && (e.shiftKey)) {
        player_view_open();
      }
    }
    if (e.which == 90) {
      if (e.ctrlKey) {
        // $('#grid_wrapper').removeClass();
        // $('#grid_wrapper').addClass('active grid_canvas_drag');
        // $('#grid_wrapper').draggable({disabled: false});
      }
      if (e.shiftKey) {
        // $('#grid_wrapper').removeClass();
        // $('#grid_wrapper').draggable({disabled: true});
      }
    }
    if (e.which == 88) {
      if (e.ctrlKey) {
        $('#grid_wrapper').removeClass();
        // $('#grid_wrapper').draggable({disabled: true});
        $('#grid_wrapper').addClass('active grid_canvas_marking');
      }
      if (e.shiftKey) {
        $('#grid_wrapper').removeClass();
      }
    }
    if (e.which == 8) {
      e.preventDefault();
      $('#grid_wrapper').removeClass();
      // $('#grid_wrapper').draggable('disable');
      if (e.ctrlKey) {
        $('.map_token').remove();
      }
    }
  });

  /**
   * Helper gets opacity from the grid_canvas and returns percentile.
   */
  function get_opacity(thing) {
    _opacity = thing.val();
    _opacity = _opacity / 100;
    return _opacity;
  }

  /**
   * Get the hash of a link and reveal the appropriate div by id.
   */
  $('#menu a').each(function() {
    var hash = this.hash.substr(1);
    $('#' + hash).hide();
  }).on('touchend click', function(event) {
    event.stopPropagation();
    event.preventDefault();
    var hash = this.hash.substr(1);
    $(this).toggleClass('active');
    $('#' + hash).toggle('slide', {direction:'up'});
  });

  /**
   * WELL IS IT?!
   */
  function MOBILE() {
    if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent) ) {
     return true;
    }
    return false;
  }

  /**
   * Open sesame.
   */
  $('#wrench').on('touchend click', function(event) {
    event.stopPropagation();
    event.preventDefault();
    $('#sidebar').toggle('slide', {direction:'right'});
  }).one('click', function() {
    $('#wrench').removeClass('newbie');
  });

  /**
   * Grid type selected?  Set the grid_canvas per the options.
   */
  $('input[name=map_grid_type]').change(function() {
    set_grid();
  });

  /**
   * Grid resized?  Set the grid_canvas per the options.
   */
  $('#map_grid_size').change(function() {
    set_grid();
  });

  /**
   * Input vs change, change fires after, input fires with each.
   */
  $('#map_grid_opacity').on('input', function() {
    $('#grid_wrapper').css('opacity', get_opacity($('#map_grid_opacity')));
  });

  $('#fow_opacity').on('input', function() {
    $('#fow_wrapper').css('opacity', get_opacity($('#fow_opacity')));
  });

  /**
   * Fog On!
   */
  $("#fow_toggle").change(function() {
    $('canvas[id^=fow]').toggleClass('active', this.checked);
    $('#cursor').toggleClass('active', this.checked);
    if ($('canvas[id^=fow]').hasClass('active')) {
      _width = $('#map_wrapper').width();
      _height = $('#map_wrapper').height();
      init_fow(_height, _width);
    }
  }).change();

  /**
   * Create the cursor div to hold the svgs for the cursor.
   */
  $('body').append('<div id="cursor"></div>');

  // svg maker, make me some svgs.
  fow_brush = SVG('cursor');
  fow_brush_cursor = fow_brush.circle($('#fow_brush_size').val()).fill('white').stroke({ color: 'black', width: 1}).opacity(0.33);
  fow_brush_feather_cursor = fow_brush.circle($('#fow_brush_feather_size').val()).fill('white').stroke({ color: 'black', width: 1}).opacity(0.33);

  // one brush for each.
  __brush = $('#fow_brush_size');
  __feather = $('#fow_brush_feather_size');

  // I feel like jquery probably creates me two event handlers each with a switch that can only result in one direction, butt fuck it, this is a prototype.
  $('#fow_brush_size, #fow_brush_feather_size').on('input', function() {
    switch (this.id) {
      case 'fow_brush_size':
        if (parseInt(__brush.val()) >= parseInt(__feather.val())) {
          __feather.val(parseInt(__brush.val()) + 1);
        }
        break;
      case 'fow_brush_feather_size':
        if (parseInt(__feather.val()) <= parseInt(__brush.val())) {
          __brush.val(parseInt(__feather.val()) - 1);
        }
        break;
      default:

    }
    // Cursor uses Cx and Cy.  I knew this, and still lost ten minutes to it.
    $(fow_brush_cursor)[0].attr({
      'r': __brush.val()/2,
      'cx': __feather.val()/2,
      'cy': __feather.val()/2
    });
    $(fow_brush_feather_cursor)[0].attr({
      'r': __feather.val()/2,
      'cx': __feather.val()/2,
      'cy': __feather.val()/2
    });
    // SVG is in relative space, div is in page space.  We'll affect the div thanks.
    $('#cursor').css({
      'width': $(fow_brush_feather_cursor)[0].attr('r') * 2,
      'height': $(fow_brush_feather_cursor)[0].attr('r') * 2,
    });
  }).trigger('input');

  /**
   * The function of cursor truth.  Now that we have a cursor.
   */
  $(window).mousemove(function(event) {
    _r = $(fow_brush_feather_cursor)[0].attr('r');
    _x = event.pageX - _r;
    _y = event.pageY - _r;
    $('#cursor').css('top', _y);
    $('#cursor').css('left', _x);
  });

  /**
   * Don't reinvent the wheel.
   * https://stackoverflow.com/a/8260383/4942292
   */
  function get_youtube_code(url) {
      var regExp = /^.*((youtu.be\/)|(v\/)|(\/u\/\w\/)|(embed\/)|(watch\?))\??v?=?([^#\&\?]*).*/;
      var match = url.match(regExp);
      return (match&&match[7].length==11)? match[7] : false;
  }
  $("input[name=map_type_options]").change(function(event) {
    _option = event.target;
    $(".map_type_property").hide();
    switch (_option.id) {
      case 'map_embed_option':
        $('#map_embed_wrapper').show();
        break;
      case 'map_file_option':
        $('#map_filename_wrapper').show();
    }
  }).change();

  /**
   * Set iframe tag.
   */
  function do_pdf(_url) {
    $('#map_innerwrapper').html('<iframe src="' + _url + '" frameborder="0" allowfullscreen></iframe>');
  }

  /**
   * Make and inject YouTube embed.
   */
  function do_youtube(_url) {
    var _code;
    _code = get_youtube_code(_url);

    if (!_code) {
      alert("YouTube only please.");
    }
    else {
      $('#map_innerwrapper').html('<iframe src="https://www.youtube.com/embed/' + _code + '?version=3&controls=0&disablekb=1&showinfo=0&rel=0&autoplay=1&loop=1&playlist=' + _code + '" frameborder="0" allowfullscreen></iframe>');
    }
  }

  /**
   * Let's make us ah awnliiiiine veedeo.
   */
  $('#map_embed_submit').on('touchend click', function(event) {
    event.stopPropagation();
    event.preventDefault();
    $.get('/engine/youtube?url=' + $('#map_embed').val(), null, function(response) {
      $('body').html(response[0]);
      console.log(response);
    });
  });

  // $('.map_authors_maps a').on('touchend click', function(event){
  //   event.stopPropagation();
  //   event.preventDefault();
  //   do_youtube(this.href);
  // });

function do_image(img) {
  fabric.Image.fromURL(_url, function(img) {
    map_canvas.add(img);
  });
}

$('#map_clear').click(function() {
  map_canvas.clear();
});

  /**
  * When a file is selected, we check it and add the appropriate tag to the map wrapper.
  */
  $('#map_filename').change(function() {
    var _file = this.files[0];
    _url = window.URL.createObjectURL(_file);
    ext = getExtension(_file.name)
    ext = ext.toLowerCase();

    switch (ext) {
      case 'pdf':
        do_pdf(_url);
        break;

      case 'jpg':
      case 'jpeg':
      case 'gif':
      case 'bmp':
      case 'png':
        do_video();
        do_image(_url, ext);
        break;

      case 'm4v':
        ext = 'x-m4v'
        do_video(_url, ext);
      case 'mpg':
      case 'mp4':
        do_video(_url, ext);
        break;

    default:
     items = ['You look great, by the way :)', 'You look very nice today.  I hope you\'re well.', 'That tickled.', 'I wish all my friends looked as good as you :)'];
     var item = items[Math.floor(Math.random()*items.length)];
     alert('Sorry, I don\'t know what you are trying to load.\n\n' + item);
     break;
    }

    function do_video(_url, ext) {
      if (!_url) {
        $('#map_video').remove();
      }
      $('#map_video_wrapper').html('<video loop autoplay id="map_video"><source src="' + _url + '" type="video/' + ext + '"></video>');
      $('#map_video').on('loadeddata', function() {
        __map_wrapper = $('#map_wrapper');
        __map = $('#map')[0];
        if (__map.videoHeight) {
          __map_wrapper.css('height', __map.videoHeight);
          __map_wrapper.css('width', __map.videoWidth);
        }
      });
      _video = $('#map_video')[0];
      fabric.Image.fromURL(_url, function(_video){
        map_canvas.clear();
        map_canvas.add(_video);
      });
    }

  });

  /**
   * On mouse wheel scroll, roll numbers in number inputs.
   */
  $('input[type="number"]').bind('wheel', function(event) {

    if (event.originalEvent.deltaY < 0) {
      this.value = parseInt(this.value) + 1;
    } else {
      if (parseInt(this.value) > 0) {
        this.value = parseInt(this.value) - 1;
      }
    }
    $(this).change();
    $(this).trigger('input');
    return false;
  });

  /**
   * Inactivity detection.
   */

  var timeoutInMiliseconds = 1000;
  var timeoutId;

  function startTimer() {
    // window.setTimeout returns an Id that can be used to start and stop a timer
    timeoutId = window.setTimeout(doInactive, timeoutInMiliseconds)
    $('#wrench').show('slide', {direction:'right'});
    $('#cursor').removeClass('hideBrush');
  }

  function resetTimer() {
    window.clearTimeout(timeoutId);
    startTimer();
  }

  function doInactive() {
    $('#wrench').hide('slide', {direction:'right'});
    $('#cursor').addClass('hideBrush');
  }

  function setupTimers () {
    document.addEventListener("mousemove", resetTimer, false);
    document.addEventListener("mousedown", resetTimer, false);
    document.addEventListener("keypress", resetTimer, false);
    document.addEventListener("touchmove", resetTimer, false);

    startTimer();
  }

  $(document).ready(function(){
    setupTimers();
  });

  /**
   * Quick helper to return the extension.
   */
  function getExtension(filename) {
      var parts = filename.split('.');
      return parts[parts.length - 1];
  }

  $(".inputValue").change(function(event) {
  	var element = $(event.target);
  	var key = element.attr("data-key");
  	var unit = element.attr("data-unit");
  	var value = element.val();

  	if (key) {
  		currentKey = key;
  		element.data("exactValue", value);
  		transformUnits(key, unit, value);
  	}
  	calculate(currentKey);
  });

  var __dmge_veteran = Cookies.get('DMGE_Veteran');

  if (!__dmge_veteran) {
    $('#wrench').addClass('newbie');
    Cookies.set('DMGE_Veteran', 'true');
  }

  Cookies.remove('DMGE_Veteran');

  function init_fow(__height, __width){
    var fow = $('#fow'),
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
      localStorage.setItem('mainFOW', mainFOW.toDataURL());
      localStorage.setItem('mainFOW_size', JSON.stringify({
        'width': $('#fow').width(),
        'height': $('#fow').height()
      }));
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

  function set_grid(_size) {

    var __map = $('#map_wrapper');
    var __grid_wrapper = $('#grid_wrapper');
    var _mWidth = __map.width();
    var _mHeight = __map.height();

    // $('#grid_settings').addClass('loading');
    console.time();
    // __grid_wrapper.empty();
    // draw = SVG('grid_wrapper');

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
      const __grid = Grid.rectangle({ width: _cols, height: _rows });
      grid_canvas.clear();

      __grid.forEach(hex => {

        let _props = {
          left: hex.x*_size,
          top: hex.y*_size,
          width: _size,
          height: _size,
          stroke: 'white',
          strokeWidth: 1,
          fill: '',
          originX: 'left',
          originY: 'top',
          centeredRotation: true,
          selectable: false,
          objectCaching: false
        };

        let quadSymbol = new fabric.Rect(_props);

        grid_canvas.add(quadSymbol);

      });
      grid_canvas.renderAll();
      grid_canvas.on('mouse:down', function(e) {
        if (!$('#grid_wrapper').is('.grid_marking')) {
          return;
        }
        _props.left = e.target.left;
        _props.top = e.target.top;
        _props.fill = getRandomColor();
        let highlightSymbol = new fabric.Rect(_props);
        highlightSymbol.on('selected', function(e) {
          console.log(e);
          grid_canvas.remove(e);
        })
        grid_canvas.add(highlightSymbol);
      });
    }
  } else {
    grid_canvas.clear();
  }
    console.timeEnd();
  }

  function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }

  // file_load
  $('#file_load').click(function() {
    loadFile($('#file'));
  })
  $('#file').change(function(){
    loadFile($(this));
  })

  function loadFile(file){
    var files = file.prop("files")
    $.each(files, function () {
      _file = this;
      _url = window.URL.createObjectURL(_file);
      $("#files").jsGrid("insertItem", { 'Filename': _file.name, 'Title': _url, 'Type': 'Animated', 'Thumbnail': _url }).done(function() {
        $(this).stop().css("background-color", "green").animate({ backgroundColor: "none"}, 500);
      });
    });
  }

  $("#file_preview_dialog").dialog({
    autoOpen: false,
    modal: true,
    position: {
      my: "center",
      at: "top",
      of: $('#map_wrapper')
    }
  });

  // Fashion the file grid_canvas.
  $('#files').jsGrid({
    height: '400px',
    width: '100%',

    filtering: true,
    editing: false,
    sorting: true,
    paging: true,
    autoload: true,

    pageSize: 15,
    pageButtonCount: 5,

    deleteConfirm: 'Remove?',

    fields: [
      { name: 'Bulk Op', type: 'checkbox' },
      { name: 'Filename', type: 'text', width: '25%' },
      { name: 'Address', type: 'text', width: '25%' },
      { name: 'Type', type: 'select', items: ['Static', 'Animated'] },
      { name: 'Thumbnail',
        itemTemplate: function(val, item) {
          return $('<img>').attr('src', val).css({ height: 50, width: 50 }).on('click', function() {
            $('#file_preview').attr('src', item.Thumbnail);
            $('#file_preview_dialog').dialog('open');
          });
        },
        insertTemplate: function() {
            var insertControl = this.insertControl = $('<input>').prop('type', 'file');
            return insertControl;
        },
        insertValue: function() {
            return this.insertControl[0].files[0];
        },
        align: 'center',
        width: 120
      },
      { name: 'Add',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-puzzle-piece" aria-hidden="true"></i> Add').attr({'class': 'file_add_to_canvas'}).css({ 'display': 'block' }).on('click', function() {
            do_image(val);
          });
        },
        align: 'center',
        width: 120
      },
      { type: 'control',
        editButton: false
      }
    ]
  });

  function make_video_thumbnail(_url) {
    var __video  = document.getElementById(_url);
    var __canvas = document.createElement('canvas');
    __canvas.width  = __video.videoWidth;
    __canvas.height = __video.videoHeight;
    var __ctx = __canvas.getContext('2d');
    __ctx.drawImage(__video, 0, 0);
    return __canvas.toDataURL('image/jpeg');
  }

  /**
   * Calculate screen.
   */
  let _screen_width = screen.width,
    _screen_height = screen.height,
    _screen_x = $("#first").val(_screen_width),
    _screen_y = $("#second").val(_screen_height);

  $('#screen_calculate').click(function() {
  	let _screen_x = $("#screen_x").val(),
  	_screen_y = $("#screen_y").val(),
  	inch = $("#screen_inch").val(),
  	result = $("#screen_result"),
  	sqroot = +(_screen_x * _screen_x) + +(_screen_y * _screen_y);

  	if (_screen_x == 0)
  		result.val("Missing horizontal resolution");
  	else if (_screen_y == 0)
  		result.val("Missing vertical resolution");
  	else if (inch == 0)
  		result.val("Missing screen size")
  	else
      _result = (Math.sqrt(sqroot) / inch).toFixed(2);
      if (!_result) {
        return;
      }
  		result.val(_result);
      $('#map_grid_size').val(_result);
      set_grid(_result);
  });

  $('#player_view_open').click(function() {
    player_view_open();
  });

  function player_view_open() {
    var playerView = Drupal.playerView = window.open('/engine/players');
    var playerViewInterval = setInterval(playerViewTimer, 1000);
    var playerMap = Drupal.playerMap = playerView.document.getElementById('player_map');
    var playerFOW = Drupal.playerFOW = playerView.document.getElementById('player_fow');
    function playerViewTimer() {
      // playerMap.innerHTML += 'works.';
      // playerFOW.innerHTML += 'works.';
    }
  }

}(jQuery, Drupal, this, this.document));
