(function ($, Drupal, window, document, undefined) {
  window.moveTo(0, 0);
  window.resizeTo(screen.availWidth, screen.availHeight);

  const todays_date = new Date();

  // We set these now for assignment later.
  var player_view, player_map, player_fow, player_grid, player_paint, map_scroll_synch,
  fow_ctx = document.getElementById('fow').getContext('2d'),
  r1 = $('#fow_brush_size').val(),
  r2 = $('#fow_brush_feather_size').val(),
  dragging = false;

  /**
   * Hash code helper.
   * https://stackoverflow.com/a/7616484/4942292
   */
  String.prototype.hashCode = function() {
    var hash = 0, i, chr;
    if (this.length === 0) return hash;
    for (i = 0; i < this.length; i++) {
      chr   = this.charCodeAt(i);
      hash  = ((hash << 5) - hash) + chr;
      hash |= 0; // Convert to 32bit integer
    }
    return hash;
  };

  /**
   * Helper for screen size.
   */
  const screensize = {
    width: screen.availWidth,
    height: screen.availHeight
  };

  var _canvas_props = {
    preserveObjectStacking: true,
    renderOnAddRemove: false,
    hoverCursor: 'pointer',
    skipTargetFind: false,
    skipOffscreen: false,
    selection: false
  };
  const canvases = ['map', 'grid', 'fow'];
  const canvas_canvases = ['map_canvas', 'grid_canvas', 'fow_canvas'];

  /**
   * Helper to set dimensions on all canvases.
   * Defines the canvases to fabricjs.
   */
  function set_canvas_dimensions(_screensize) {
    if (!_screensize) {
      let _screensize = screensize;
    }
    canvases.forEach(function(e) {
      let _canvas = e + '_canvas';
      if (!window[_canvas]) {
        switch (e) {
          case 'fow':
          case 'grid':
            window[_canvas] = new fabric.StaticCanvas(e, _canvas_props);
            break;
          default:
            window[_canvas] = new fabric.Canvas(e, _canvas_props);
        }
        map_canvas.selection = true;
      }
      window[_canvas].setWidth(_screensize.width);
      window[_canvas].setHeight(_screensize.height);
    });
  }

  set_canvas_dimensions(screensize);

  function update_canvases() {
    canvases.forEach(function(e) {
      let _canvas = e + '_canvas';
      window[_canvas].renderAll();
    });
  }

  /**
   * Helper checks event passed in.
   */
  function check_object_vs_map(e) {
    let _width = (screensize.width < e.target.width);
    let _height = (screensize.height < e.target.height);
    if ((_width) || (_height)) {
      let _screensize = {
        width: (_width) ? e.target.width : screensize.width,
        height: (_width) ? e.target.height : screensize.height
      }
      set_canvas_dimensions(_screensize);
    }
  }

  map_canvas.on('object:added', function(e) {
    check_object_vs_map(e);
    $('#layering').jsGrid('insertItem', {'id': e.target.id, 'index': map_canvas.getObjects().indexOf(getObjectFromCanvasById(e.target.id, map_canvas))});
  });

  fabric.util.requestAnimFrame(function render() {
    map_canvas.renderAll();

    /**
     * Update the player view on frame.
     */
    if (window.player_view) {
      if (player_map) {
        let map_width = map_canvas.width;
        let map_height = map_canvas.height;

        window.player_view.width = map_width;
        window.player_view.height = map_height;

        player_map.width = map_width;
        player_map.height = map_height;

        player_map.getContext('2d').drawImage(map_canvas.getElement(), 0, 0, map_width, map_height);
      }
    }

    fabric.util.requestAnimFrame(render);
  });

  // Using jQuery with fabricjs to call the canvas function
  function load_canvas_data() {
    canvases.forEach(function(e) {
      let _canvas_content = e + '_canvas_content';
      window[_canvas_content] = localStorage.getItem(canvas + '_canvas_content');
      window.e.loadFromJSON(window[_canvas_content]);
      window[e].renderAll();
    });
  }

  /**
   * Helper assigns json to txt and link, then clicks for download.
   */
  function file_storage(content, filename, contentType) {
    var a = document.createElement("a");
    var file = new Blob([content], {type: text/json});
    a.href = URL.createObjectURL(file);
    a.download = filename;
    a.click();
    a.remove();
  }

  function save_canvases() {
    let map_name = document.getElementById('map_name').value;
    let storage = {'fow': NULL, 'grid': NULL, 'map': NULL};
    canvases.forEach(function(e) {
      _e = eval(e + '_canvas');
      _e.toJSON();
      _e_content = eval(_e + '_content');
      _e_json = _e.toJSON();
      localStorage.setItem(_e_content, _e_json);
      storage._e_content = _e_json;
    });
    file_storage(JSON.stringify(storage), map_name + '.txt', 'text/plain');
  }

  $('#save_map').click(function(e) {
    save_canvases();
  });

  function dragging_initialize() {
    map_canvas.selection = false;
    $('#map_wrapper').addClass('notouchie');
    map_scroll_synch = $('map_scroll_synch').val();
  }

  var clicked = false, clickY, clickX;
  $(document).on({
      'mousemove': function(e) {
        if (clicked) {
          updateScrollPos(e, window);
          if ((player_view) && (map_scroll_synch)) {
            updateScrollPos(e, player_view);
          }
        }

      },
      'mousedown': function(e) {
        if (e.altKey === true) {
          dragging_initialize();
          clicked = true;
          clickY = e.pageY;
          clickX = e.pageX;
          if (player_view) {
            player_view.pageY = e.pageY;
            player_view.pageX = e.pageX;
          }
        }
      },
      'mouseup': function() {
        $('#map_wrapper').removeClass('notouchie');
        clicked = false;
        map_canvas.selection = true;
        $('html').css('cursor', 'auto');
      }
  });

  map_canvas.on('object:moving', function(options) {
    let grid_snap = document.getElementById('grid_snap');
    if (!grid_snap.checked) {
      return;
    }
    let grid = parseInt(document.getElementById('map_grid_size').value);
    options.target.set({
      left: Math.round(options.target.left / grid) * grid,
      top: Math.round(options.target.top / grid) * grid
    });
  });

  map_canvas.on('object:scaling', function(options) {
    let grid_snap = document.getElementById('grid_snap');
    if (!grid_snap.checked) {
      return;
    }
    let grid = parseInt(document.getElementById('map_grid_size').value),
    target = options.target,
    w = target.width * target.scaleX,
    h = target.height * target.scaleY,
    snap = {      // Closest snapping points
       top: Math.round(target.top / grid) * grid,
       left: Math.round(target.left / grid) * grid,
       bottom: Math.round((target.top + h) / grid) * grid,
       right: Math.round((target.left + w) / grid) * grid
    },
    threshold = grid,
    dist = {      // Distance from snapping points
       top: Math.abs(snap.top - target.top),
       left: Math.abs(snap.left - target.left),
       bottom: Math.abs(snap.bottom - target.top - h),
       right: Math.abs(snap.right - target.left - w)
    },
    attrs = {
       scaleX: target.scaleX,
       scaleY: target.scaleY,
       top: target.top,
       left: target.left
    };
    switch (target.__corner) {
      case 'tl':
         if (dist.left < dist.top && dist.left < threshold) {
            attrs.scaleX = (w - (snap.left - target.left)) / target.width;
            attrs.scaleY = (attrs.scaleX / target.scaleX) * target.scaleY;
            attrs.top = target.top + (h - target.height * attrs.scaleY);
            attrs.left = snap.left;
         } else if (dist.top < threshold) {
            attrs.scaleY = (h - (snap.top - target.top)) / target.height;
            attrs.scaleX = (attrs.scaleY / target.scaleY) * target.scaleX;
            attrs.left = attrs.left + (w - target.width * attrs.scaleX);
            attrs.top = snap.top;
         }
         break;
      case 'mt':
         if (dist.top < threshold) {
            attrs.scaleY = (h - (snap.top - target.top)) / target.height;
            attrs.top = snap.top;
         }
         break;
      case 'tr':
         if (dist.right < dist.top && dist.right < threshold) {
            attrs.scaleX = (snap.right - target.left) / target.width;
            attrs.scaleY = (attrs.scaleX / target.scaleX) * target.scaleY;
            attrs.top = target.top + (h - target.height * attrs.scaleY);
         } else if (dist.top < threshold) {
            attrs.scaleY = (h - (snap.top - target.top)) / target.height;
            attrs.scaleX = (attrs.scaleY / target.scaleY) * target.scaleX;
            attrs.top = snap.top;
         }
         break;
      case 'ml':
         if (dist.left < threshold) {
            attrs.scaleX = (w - (snap.left - target.left)) / target.width;
            attrs.left = snap.left;
         }
         break;
      case 'mr':
         if (dist.right < threshold) attrs.scaleX = (snap.right - target.left) / target.width;
         break;
      case 'bl':
         if (dist.left < dist.bottom && dist.left < threshold) {
            attrs.scaleX = (w - (snap.left - target.left)) / target.width;
            attrs.scaleY = (attrs.scaleX / target.scaleX) * target.scaleY;
            attrs.left = snap.left;
         } else if (dist.bottom < threshold) {
            attrs.scaleY = (snap.bottom - target.top) / target.height;
            attrs.scaleX = (attrs.scaleY / target.scaleY) * target.scaleX;
            attrs.left = attrs.left + (w - target.width * attrs.scaleX);
         }
         break;
      case 'mb':
         if (dist.bottom < threshold) attrs.scaleY = (snap.bottom - target.top) / target.height;
         break;
      case 'br':
         if (dist.right < dist.bottom && dist.right < threshold) {
            attrs.scaleX = (snap.right - target.left) / target.width;
            attrs.scaleY = (attrs.scaleX / target.scaleX) * target.scaleY;
         } else if (dist.bottom < threshold) {
            attrs.scaleY = (snap.bottom - target.top) / target.height;
            attrs.scaleX = (attrs.scaleY / target.scaleY) * target.scaleX;
         }
         break;
       }
     target.set(attrs);
  });

  /**
   * Helper function get canvas object.
   */
  function getObjectFromCanvasById(id, canvas) {
    let objs = canvas.getObjects().filter((item) => {
      return item.id === parseInt(id);
    });
    return objs[0];
  }

  /**
   * Helper to remove object from canvas.
   */
  function removeObjectFromCanvas(id, canvas) {
    let obj = getObjectFromCanvasById(id, canvas);
    canvas.remove(obj);
  }

  var updateScrollPos = function(e, _window) {
    $('html').css('cursor', 'move');
    $(_window).scrollTop($(_window).scrollTop() + (clickY - e.pageY));
    $(_window).scrollLeft($(_window).scrollLeft() + (clickX - e.pageX));
  }

  /**
   * Helper to set zoom across all canvases.
   */
  function map_zoom(opt, zoom) {
    let _x = opt.e.offsetX;
    let _y = opt.e.offsetY;
    canvases.forEach(function(e) {
      _e = eval(e + "_canvas");
      _e.zoomToPoint({ x: _x, y: _y }, zoom);
    });
    let _size = parseInt($('#map_grid_size').val());
    _size = _size * map_canvas.getZoom();
    $('#map_grid_display_size').val(_size);
    update_canvases();
  }

  $('#map_dim_submit').click(function(e) {
    let _screensize = {
      width: $('#map_width').val(),
      height: $('#map_height').val()
    };
    set_canvas_dimensions(_screensize);
  });

  $('#fullscreener').click(function(e) {
    full_screen();
  });

  // map_canvas.on('mouse:wheel', function(opt) {
  //   var delta = opt.e.deltaY;
  //   var zoom = map_canvas.getZoom();
  //   zoom = zoom + delta/200;
  //   if (zoom > 10) {zoom = 10;}
  //   if (zoom < 1) {zoom = 1;}
  //
  //   map_zoom(opt, zoom);
  //
  //   opt.e.preventDefault();
  //   opt.e.stopPropagation();
  //   var vpt = this.viewportTransform;
  //   if (vpt[4] >= 0) {
  //     grid_canvas.viewportTransform[4] = this.viewportTransform[4] = 0;
  //   } else if (vpt[4] < map_canvas.getWidth() * zoom) {
  //     grid_canvas.viewportTransform[4] = this.viewportTransform[4] = map_canvas.getWidth() * zoom;
  //   }
  //   if (vpt[5] >= 0) {
  //     grid_canvas.viewportTransform[5] = this.viewportTransform[5] = 0;
  //   } else if (vpt[5] < map_canvas.getHeight() * zoom) {
  //     grid_canvas.viewportTransform[5] = this.viewportTransform[5] = map_canvas.getHeight() * zoom;
  //   }
  // });

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
      if (e.altKey) {
        __fow = localStorage.getItem('fow_content');
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
    $('#sidebar_sections .sidebar_section').hide();
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
    if (player_view) {
      player_grid_wrapper.css('opacity', get_opacity($('#map_grid_opacity')));
    }
  });

  $('#fow_opacity').on('input', function() {
    $('#fow_wrapper').css('opacity', get_opacity($('#fow_opacity')) / 2);
    if (player_view) {
      player_fow_wrapper.css('opacity', get_opacity($('#map_grid_opacity')));
    }
  });

  /**
   * Helper stores FoW content to window variable.
   */
  function fow_store_content() {
    // Get image data from canvas and store it to window variable.
    window.fow_content = fow_ctx.getImageData(0, 0, fow_canvas.width, fow_canvas.height);
    // Stringify the window variable for localstorage.
    // window.fow_content = JSON.stringify(window.fow_content);
    // localStorage.setItem('current_fow_content', window.fow_content);
  }
  $('#fow_store').click(function(e) {
    fow_store_content();
  });

  /**
   * Helper recalls FoW content.
   */
  function fow_recall_content() {
    // window.fow_content = JSON.parse(localStorage.getItem('current_fow_content'));
    if (window.fow_content) {
      fow_ctx.putImageData(window.fow_content, 0, 0);
      render_fow_canvas();
    }
    else {
      fow_reset();
    }
  }
  $('#fow_recall').click(function(e) {
    fow_recall_content();
  });

  /**
   * Render the Fog of War data in variable to player canvas
   */
  function render_fow_canvas() {
    if (player_fow) {
      player_fow.getContext('2d').drawImage(fow_canvas.getElement(), 0, 0, fow_canvas.width, fow_canvas.height);
    }
  }

  /**
   * Helper initializes fog of war.
   */
  function fow_reset() {
    fow_ctx.globalCompositeOperation = 'source-over';
    fow_ctx.fillStyle = 'rgba( 0, 0, 0, 1 )';
    fow_ctx.fillRect(0, 0, fow_canvas.width, fow_canvas.height);
  }
  $('#fow_reset').click(function(e) {
    fow_reset();
  });

  /**
   * Fog On!
   */
  $("#fow_toggle").change(function() {
    fow_store_content();
    $('canvas[id^=fow]').toggleClass('active', this.checked);
    $('#cursor').toggleClass('active', this.checked);
    if ($('canvas[id^=fow]').hasClass('active')) {
      fow_recall_content();
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

  function do_video(_id, _url, ext) {
    let vtag;
    vtag = $('<video />', {
      class: 'map_video',
      type: 'video/' + ext,
      src: _url,
      controls: false,
      autoplay: true,
      loop: true,
      id: _id
    });
    $('#map_video_wrapper').append(vtag[0]);
    return vtag;
  }

  /**
   * Don't reinvent the wheel.
   * https://stackoverflow.com/a/8260383/4942292
   */
  function get_youtube_code(url) {
    var regExp = /^.*((youtu.be\/)|(v\/)|(\/u\/\w\/)|(embed\/)|(watch\?))\??v?=?([^#\&\?]*).*/;
    var match = url.match(regExp);
    return (match&&match[7].length==11)? match[7] : false;
  }

  /**
   * Let's make us ah awnliiiiine veedeo.
   */
  $('#map_embed_submit').on('touchend, click', function(e) {
    // A serverside PHP callback fires the URL to YouTube and parses an mp4 url from the response, for us to embed.
    let code = get_youtube_code($('#map_embed').val());
    // Fire our backend script with our id code.
    do_youtube(code);
  });

  /**
   * Helper fires GET and then sets off code and video processing.
   */
  function do_youtube(code) {
    $.get('/engine/youtube?v=' + code, null, function(response) {
      if (response[0]) {
        if (response[0].url) {
          let _url = response[0].url;
          let _id = make_file_id(_url);
          do_video(_id, _url, 'mp4');
          $('#files').jsGrid('insertItem', {'id': _id, 'Blob': _url, 'Type': 'YouTube' });
        }
      }
    });
  }

  /**
   * Returns reference to added canvas entity.
   */
  function do_image(_url) {
    let _id = make_file_id(_url);
    window[_id] = fabric.Image.fromURL(_url, function(img) {
      img.set({
        id: _id
      })
      map_canvas.add(img);
    });
  }

  $('#map_reset_zoom').click(function() {
    map_reset_zoom();
  });

  $('#map_clear').click(function() {
    map_canvas.clear();
    fow_canvas.clear();
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
    document.addEventListener('keypress', resetTimer, false);
    document.addEventListener('mousemove', resetTimer, false);
    document.addEventListener('mousedown', resetTimer, false);
    document.addEventListener('touchmove', resetTimer, false);

    startTimer();
  }

  /**
   * Helper for returning query string params as an object.
   * Thanks to https://stackoverflow.com/a/29546771/4942292
   */
  function getQueryParams() {
    try {
      let url = window.location.href;
      let query_str = url.substr(url.indexOf('?')+1, url.length-1);
      let r_params = query_str.split('&');
      let params = {};
      for (i in r_params) {
          param = r_params[i].split('=');
          params[param[0]] = param[1];
      }
      return params;
    }
    catch(e) {
       return {};
    }
  }

  $(document).ready(function(){
    setupTimers();
    fow_reset();
    let params = getQueryParams();
    if (params.v) {
      console.log(params);
      console.log(params.v);
      do_youtube(params.v);
    }
  });

  /**
   * Quick helper to return the extension.
   */
  function getExtension(filename) {
      let parts = filename.split('.');
      let ext = parts[parts.length - 1]
      ext = ext.toLowerCase();
      return ext;
  }

  /**
   * Make a polygon.
   * Stolen from https://stackoverflow.com/questions/29319677/fabric-js-geometric-shapes
   */
  function regularPolygonPoints(sideCount,radius){
    var sweep = (Math.PI * 2) / sideCount;
    var cx = radius;
    var cy = radius;
    var points = [];
    for(var i=0; i < sideCount; i++){
      var x = cx + radius * Math.cos(i * sweep);
      var y = cy + radius * Math.sin(i * sweep);
      points.push({
        x:x, y:y
      });
    }
    return(points);
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

  $('#fow').on('mousedown', function() {
    dragging = true;
  });

  $('#fow').on('mouseup', function() {
    dragging = false;
    if (player_fow) {
      player_fow.width = fow_canvas.width;
      player_fow.height = fow_canvas.height;
      render_fow_canvas();
    }
  });

  $('#fow').on('mousemove', function(ev, ev2) {
    if (dragging) {
      r1 = $('#fow_brush_size').val() / 2;
      r2 = $('#fow_brush_feather_size').val() / 2;

      ev2 && (ev = ev2);

      var pX = ev.pageX,
      pY = ev.pageY;

      var radGrd = fow_ctx.createRadialGradient(pX, pY, r1, pX, pY, r2);
      radGrd.addColorStop(0, 'rgba( 0, 0, 0,  1 )');
      radGrd.addColorStop(0.25, 'rgba( 0, 0, 0, .1 )');
      radGrd.addColorStop(1, 'rgba( 0, 0, 0, 0 )');

      if (ev.shiftKey !== false) {
        fow_ctx.globalCompositeOperation = 'source-over';
      } else {
        fow_ctx.globalCompositeOperation = 'destination-out';
      }

      fow_ctx.fillStyle = radGrd;
      fow_ctx.fillRect(pX - r2, pY - r2, r2 * 2, r2 * 2);
    };
  });

  /**
   * Helper creates grid.
   */
  function set_grid(_size) {
    grid_canvas.clear();
    var _type = $('input[name=map_grid_type]:checked').val();
    if (_type !== 'None') {
      var __map = $('#map_wrapper');
      var __grid_wrapper = $('#grid_wrapper');

      console.time();

      __grid_wrapper.css('opacity', get_opacity($('#map_grid_opacity')));

      if (!_size) {
        var _size = parseInt($('#map_grid_size').val());
      }

      var _width = $('#map_width').val() ? $('#map_width').val() : screensize.width;
      var _height = $('#map_height').val() ? $('#map_height').val() : screensize.height;

      var _cols = (_width / _size);
      var _rows = (_height / _size);

      var __gridoptions = {size: _size};

      // Single option switch sets an otherwise default.
      switch (_type) {
        case 'H_Hex':
        __gridoptions.orientation = 'Flat';
          break;
      }
      const Hex = Honeycomb.extendHex(__gridoptions);

      var Grid = Honeycomb.defineGrid(Hex)

      _cols = (_width / _size);
      _rows = (_height / _size);

      const __grid = Grid.rectangle({width: _cols, height: _rows});


      if (_type === 'H_Hex' || _type === 'V_Hex') {
        var _points = regularPolygonPoints(6, _size);
        var corners = Hex().corners();

        __grid.forEach(hex => {
          const {x, y} = hex.toPoint();
          var _corners = corners.map(({x, y}) => `${x}, ${y}`);

          let _props = {
            angle: 0,
            left: x,
            top: y,
            width: _size,
            height: _size,
            stroke: 'white',
            strokeWidth: 1,
            fill: '',
            originX: 'left',
            originY: 'top',
            centeredRotation: true,
            hasRotatingPoint: false,
            selectable: false,
            lockMovementX: true,
            lockMovementY: true,
            objectCaching: true
          };

          let hexSymbol = new fabric.Polygon(corners, _props, false);

          grid_canvas.add(hexSymbol);

        });
      }

      if (_type === 'Quad') {
        const __grid = Grid.rectangle({ width: _cols, height: _rows });

        __grid.forEach(quad => {
          let _props = {
            left: quad.x*_size,
            top: quad.y*_size,
            width: _size,
            height: _size,
            stroke: 'white',
            strokeWidth: 1,
            fill: '',
            originX: 'left',
            originY: 'top',
            centeredRotation: true,
            selectable: false,
            lockMovementX: true,
            lockMovementY: true,
            objectCaching: true
          };

          let quadSymbol = new fabric.Rect(_props);
          grid_canvas.add(quadSymbol);
        });

        /**
         * This doesn't work because:
         * We're using Fabric because:
         * We want to interact with the grid because:
         * Grid marking, etc.
         */
        // var canvas = $('#grid').get(0);
        // var context = canvas.getContext("2d");
        // context.beginPath();
        // for (var x = 0; x <= _width; x += _size) {
        //   context.moveTo(0.5 + x, 0);
        //   context.lineTo(0.5 + x, _height);
        // }
        // for (var x = 0; x <= _height; x += _size) {
        //   context.moveTo(p, 0.5 + x);
        //   context.lineTo(bw, 0.5 + x);
        // }
        // context.closePath();
        // context.strokeStyle = "white";
        // context.lineWidth = 1;
        // context.stroke();
      }
      console.timeEnd();
      grid_canvas.renderAll();

      if (player_grid) {
        player_grid.width = grid_canvas.width;
        player_grid.height = grid_canvas.height;
        player_grid.getContext('2d').drawImage(grid_canvas.getElement(), 0, 0, grid_canvas.width, grid_canvas.height);
      }
    }
  }

  function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++) {
      color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
  }

  // File loaders.
  $('#file_load').click(function() {
    loadFile($('#file'));
  })
  $('#file').change(function(){
    loadFile($(this));
  })

  /**
   * Helper loads files.
   */
  function loadFile(file) {
    var files = file.prop("files");
    $.each(files, function () {
      let _file = this;
      let _url = window.URL.createObjectURL(_file);
      let _id = make_file_id(_url);
      let _thumbnail = _url;
      let _type = 'Static';
      let ext = getExtension(_file.name);

      switch (ext) {
        case 'pdf':
          do_pdf(_url);
          break;

        case 'jpg':
        case 'jpeg':
        case 'gif':
        case 'bmp':
        case 'png':
          do_image(_url, ext);
          break;

        case 'm4v':
          ext = 'x-m4v';
        case 'mpg':
        case 'mp4':
          _type = 'Animated';
          _vtag = do_video(_id, _url, ext);
          break;

      default:
       items = ['You look great, by the way :)', 'You look very nice today.  I hope you\'re well.', 'That tickled.', 'I wish all my friends looked as good as you :)'];
       var item = items[Math.floor(Math.random()*items.length)];
       alert('Sorry, I don\'t know what you are trying to load.\n\n' + item);
       break;
      }

      $('#files').jsGrid('insertItem', {'id': _id, 'Filename': _file.name, 'Blob': _url, 'Type': _type, 'Thumbnail': _thumbnail });
    });
  }

  /**
   * Helper makes unique ID from hash of current time and url.
   */
  function make_file_id(filename) {
    let t = todays_date.getTime();
    t = filename.hashCode() + t;
    return t;
  }

  /**
   * Initializing the file preview dialog.
   */
  $("#file_preview_dialog").dialog({
    autoOpen: false,
    modal: true,
    position: {
      my: "center",
      at: "top",
      of: $('#map_wrapper')
    }
  });

  /**
   * Initializing the files table.
   */
  $('#files').jsGrid({
    height: '400px',
    width: '100%',

    filtering: true,
    editing: false,
    sorting: true,

    pageSize: 15,
    pageButtonCount: 5,

    confirmDeleting: true,
    deleteConfirm: 'Remove?',
    onItemDeleted: function(e) {
      let _id = e.row[0].id;
      while (getObjectFromCanvasById(_id, map_canvas)) {
        removeObjectFromCanvas(_id, map_canvas);
        $('#' + _id).remove();
      }
    },

    onItemInserted: function(e) {
      let item = e.item;
      if ((item.Type === 'Animated') || (item.Type === 'YouTube')) {
        let vId = $('#' + item.id);
        let vtag = document.getElementById(item.id);
        $(vId).on('play', function(e) {
          let thumbnail = make_video_thumbnail(item.id, item.Type);
          $('#files').jsGrid("updateItem", item, {'Thumbnail': thumbnail });
          // VIDEO TAGS GET A MAP_VIDEO_ PREFIX
          let item_id = 'map_video_' + item.id;
          window[item_id] = new fabric.Image(vtag, {
            id: item.id,
            originX: 'left',
            originY: 'top',
            height: vtag.videoHeight,
            width: vtag.videoWidth,
            left: 0,
            top: 0
          });
          map_canvas.add(window[item_id]);
        });
      }
    },

    fields: [
      { name: 'id', type: 'number', visible: false },
      { name: 'Bulk Op', type: 'checkbox' },
      { name: 'Filename', type: 'text', width: '25%' },
      { name: 'Blob', type: 'text', width: '25%' },
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
          return $('<button>').html('<i class="fa fa-puzzle-piece" aria-hidden="true"></i> Add').attr({'class': 'file_add_to_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            let obj = fabric.util.object.clone(getObjectFromCanvasById(item.id, map_canvas));
            map_canvas.add(obj);
          });
        },
        align: 'center',
        width: 120
      },
      { name: 'Delete',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-trash" aria-hidden="true"></i> Delete').attr({'class': 'file_delete_from_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            $('#files').jsGrid('deleteItem', $(item));
          });
        },
        align: 'center',
        width: 120
      }
    ]
  });

  /**
   * Layering grid.
   */
  $('#layering').jsGrid({
    height: '400px',
    width: '100%',

    editing: false,

    pageSize: 999,
    // pageButtonCount: 5,

    confirmDeleting: true,
    deleteConfirm: 'Remove?',
    onItemDeleted: function(e) {
      let _id = e.row[0].id;
      while (getObjectFromCanvasById(_id, map_canvas)) {
        removeObjectFromCanvas(_id, map_canvas);
        $('#' + _id).remove();
      }
    },

    fields: [
      { name: 'id', type: 'number', visible: false },
      { name: 'Index', type: 'integer' },
      { name: 'Filename', type: 'text', width: '25%' },
      { name: 'Delete',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-trash" aria-hidden="true"></i> Delete').attr({'class': 'file_delete_from_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            $('#files').jsGrid('deleteItem', $(item));
          });
        },
        align: 'center',
        width: 120
      }
    ],
    rowClass: function(item, itemIndex) {
      return "idx-" + itemIndex;
    },
    onRefreshed: function() {
      var $gridData = $("#layering .jsgrid-grid-body tbody");

      $gridData.sortable({
        update: function(e, ui) {
          // array of indexes
          var clientIndexRegExp = /\s*idx-(\d+)\s*/;
          var indexes = $.map($gridData.sortable('toArray', {
            attribute: 'class'
          }), function(classes) {
            console.log(clientIndexRegExp.exec(classes));
            return clientIndexRegExp.exec(classes)[1];
          });

          // arrays of items
          var items = $.map($gridData.find("tr"), function(row) {
            return $(row).data("JSGridItem");
          });
          console && console.log("Reordered items", items);
          items.forEach(function(e) {
            console.log(e);
            map_canvas.moveTo(getObjectFromCanvasById(e.id, map_canvas), e.row);
          });
        }
      });
    }
  });

  /**
   * Helper function for getting an objects z-index.
   */
  fabric.Object.prototype.getZIndex = function() {
    return this.canvas.getObjects().indexOf(this);
  }

  /**
   * Initialize map element properties dialog.
   */
  $('#map_element_options').dialog({
    autoOpen: false,
    modal: true,
    position: {
      my: "center",
      at: "top",
      of: $('#map_wrapper')
    }
  });

  /**
   * Selection function opens element dialog and should set controls to match properties of object.
   */
  map_canvas.on('selection:created', function (e) {
    console.log(e);
    if (e.shiftKey) {
      if (!$('#map_element_options').dialog('isOpen')) {
        $('#map_element_options').dialog('open');
      }
      let obj = map_canvas.getActiveObject();
      $('#map_element_opacity').val(obj.opacity);
    }
  });

  /**
   * ELement Opacity.
   */
  $('#map_element_opacity').on('input', function(e) {
    let obj = map_canvas.getActiveObject();
    obj.set({
      opacity: this.value
    });
  })

  /**
   * Grab video tag by id and make a screen shot.
   */
  function make_video_thumbnail(video, type) {
    if (type == 'YouTube') {
      return;
    }
    video = document.getElementById(video);

    if ($('#thumb_video_canvas')) {
      $('#thumb_video_canvas').remove();
    }
    let _shot,
    thumb_canvas = $('<canvas />', {
      'class': 'thumb_video_canvas',
      'id': 'thumb_video_canvas',
      'height': 256,
      'width': 256
    });
    $('body').append(thumb_canvas);
    thumb_canvas[0].width = 256;
    thumb_canvas[0].height = 256;
    thumb_canvas[0].getContext('2d').drawImage(video, 0, 0, thumb_canvas[0].width, thumb_canvas[0].height);
    while (!_shot) {
        _shot = thumb_canvas[0].toDataURL();
      // if (type == 'YouTube') {
      //   _shot = thumb_canvas[0].getImageData(0, 0, thumb_canvas[0].width, thumb_canvas[0].height);
      // }
    }

    if (_shot) {
      thumb_canvas.remove();
    }
    return _shot;
  }

  /**
   * Calculate screen.
   * Shout out to screen-size.info for the javascript driving this feature.
   */
  var _screen_x = $("#first").val(screen.width),
    _screen_y = $("#second").val(screen.height);

  $('#screen_calculate').click(function() {
  	_screen_x = $("#screen_x").val();
  	_screen_y = $("#screen_y").val();
  	let inch = $("#screen_inch").val(),
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

  /**
   * Helper opens the player window and sets the reference to local storage.
   */
  function player_view_open() {
    player_view = window.player_view = window.open('/engine/players', 'player_window');
    player_view_initialize();
  }

  /**
   * Sets event handler for onload and assigns player canvases to variables in local window.
   */
  function player_view_initialize() {
    if (player_view) {
      player_view.onload = function() {
        player_view_connect();
      };
    }
  }

  /**
   * Link child window canvases when the view connects.
   */
  function player_view_connect() {
    if (!$(player_view)) {
      return;
    }
    if (!$(player_view.document)) {
      return;
    }
    $player_view_content = $(player_view.document.body);
    if ($player_view_content) {
      player_map_wrapper = $player_view_content.find('#player_map');
      player_map = player_map_wrapper[0];
      player_fow_wrapper = $player_view_content.find('#player_fow');
      player_fow = player_fow_wrapper[0];
      player_grid_wrapper = $player_view_content.find('#player_grid');
      player_grid = player_grid_wrapper[0];
    }
    else {
      alert("Couldn't connect to player window.  Re open the window.");
    }
  }

  /**
   * Function to set full screen DMGE.
   */
  function full_screen() {
    if ("fullscreenEnabled" in document || "webkitFullscreenEnabled" in document || "mozFullScreenEnabled" in document || "msFullscreenEnabled" in document) {
      if (document.fullscreenEnabled || document.webkitFullscreenEnabled || document.mozFullScreenEnabled || document.msFullscreenEnabled) {
        var element = document.getElementById("dmge");
        //requestFullscreen is used to display an element in full screen mode.
        if ("requestFullscreen" in element) {
          element.requestFullscreen();
        } else if ("webkitRequestFullscreen" in element) {
          element.webkitRequestFullscreen();
        } else if ("mozRequestFullScreen" in element) {
          element.mozRequestFullScreen();
        } else if ("msRequestFullscreen" in element) {
          element.msRequestFullscreen();
        }
      }
    } else {
      console.error("You didn't allow full screen, so the DMGE couldn't go full screen.");
    }
  }

}(jQuery, Drupal, this, this.document));
