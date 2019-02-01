(function ($, Drupal, window, document, undefined) {

  window.moveTo(0, 0);
  window.resizeTo(screen.availWidth, screen.availHeight);

  const todays_date = new Date();
  const f = fabric.Image.filters;

  fabric.filterBackend = fabric.initFilterBackend();
  fabric.isWebglSupported(fabric.textureSize);

  // Our filter constants.  These will be important later on.
  const FILTER_REMOVE_COLOR = 2;

  // We set these now for assignment later.
  var player_view, map_scroll_synch,
  r1 = $('#fow_brush_size').val(),
  r2 = $('#fow_brush_feather_size').val(),
  dragging = false;

  fabric.Object.prototype.originX = fabric.Object.prototype.originY = 'center';
  fabric.Canvas.prototype.getAbsoluteCoords = function(object) {
    return {
      left: object.left + this._offset.left,
      top: object.top + this._offset.top
    };
  }

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

  const canvases = ['map', 'grid', 'fow', 'tokens', 'templates', 'particles', 'painting'];
  const canvas_canvases = new Array();

  // zIndexes for Canvases
  const ACTIVE_CANVAS_ZINDEX = 700;
  const TEMPLATES_CANVAS_ZINDEX = 500;
  const TOKENS_CANVAS_ZINDEX = 475;
  const PARTICLES_CANVAS_ZINDEX = 450;
  const PAINTING_CANVAS_ZINDEX = 425;
  const GRID_CANVAS_ZINDEX = 400;
  const FOW_CANVAS_ZINDEX = 350;
  const MAP_CANVAS_ZINDEX = 300;

  canvases.forEach(function(e) {
    let _e = e + '_canvas';
    canvas_canvases.push(_e);
  });

  if (_canvases = $('#canvases_wrapper')) {
    canvas_canvases.forEach(function(_e) {
      let e = _e.replace('_canvas', '');
      let _wrapper = e + '_wrapper';
      if (e !== 'map') {
        window[e] = `<div id="${_wrapper}" class="notouchie"><canvas id="${e}" class="${_e}"> </canvas></div>`;
      }
      if (e === 'map') {
        window[e] = `<div id="${_wrapper}"><canvas id="${e}" class="${_e}"> </canvas><div id="map_video_wrapper"></div></div>`;
      }
      _canvases.append(window[e]);
      // Make TYPE of canvas.
      switch (e) {
        case 'fow':
        case 'grid':
          window[e] = new fabric.StaticCanvas(document.getElementById(e), _canvas_props);
          break;
        default:
          window[e] = new fabric.Canvas(document.getElementById(e), _canvas_props);
      }
      // Make events for 'toggling' canvases that can be interacted with.
      switch (e) {
        case 'map':
        // Do nothing.
          break;
        default:
          let _toggle = $('#' + e + '_toggle');
          $(_toggle).on('change', function() {
            toggle_canvas(e);
          });
      }
      window['map'].selection = true;
      $(_wrapper).css('z-index', eval(_e.toUpperCase() + '_ZINDEX'));
    });
  }
  const fow_ctx = $('#fow').get(0).getContext('2d');

  /**
   * Helper to set dimensions on all canvases.
   * Defines the canvases to fabricjs.
   */
  function set_canvas_dimensions(_screensize = window.screen) {
    canvases.forEach(function(e) {
      window[e].setWidth(_screensize.width);
      window[e].setHeight(_screensize.height);
      if (player_view) {
        let width = player_view.screen.width;
        let height = player_view.screen.height;
        let _e = 'player_' + e;
        if (window[_e]) {
          window[_e].width = width;
          window[_e].height = height;
        }
      }
    });
  }

  set_canvas_dimensions(screensize);

  /**
   * Helper renders canvases on command.
   */
  function update_canvases() {
    canvases.forEach(function(e) {
      let _canvas = e + '_canvas';
      window[_canvas].renderAll();
      if (player_view) {
        let _e = 'player_' + e;
        let width = player_view.width;
        let height = player_view.height;
        let box = window[_e].getBoundingClientRect();
        if (window[_e]) {
          let ctx = window[_e].getContext('2d');
          ctx.clearRect(0, 0, box.width, box.height);
          ctx.drawImage(window[e].getElement(), 0, 0, box.width, box.height);
        }
      }
    });
  }

  /**
   * Helper checks object being imported.
   * If map size is smaller than item, map is adjusted.
   */
  function check_object_vs_map(e) {
    let _width = (screensize.width < e.target.width);
    let _height = (screensize.height < e.target.height);
    if ((_width) || (_height)) {
      let _screensize = {
        width: (_width) ? e.target.width : screen.availWidth,
        height: (_height) ? e.target.height : screen.availHeight
      }
      set_canvas_dimensions(_screensize);
    }
  }

  /**
   * Helper function activates and deactivates canvases for interaction.
   */
  function toggle_canvas(selected_canvas = null) {
    let _selected_toggle = $('#' + selected_canvas + '_toggle');
    let _selected_wrapper = $('#' + selected_canvas + '_wrapper');
    let _selected_link = $('#' + selected_canvas + '_link');

    canvases.forEach(function(_canvas) {
      $('#' + _canvas + '_toggle').not(_selected_toggle).prop('checked', false);
      $('#' + _canvas + '_wrapper').not('#map_wrapper').addClass('notouchie');
      $('#' + _canvas + '_link').removeClass('selected');

      // Ditch selections when switching canvases.
      if (window[_canvas].discardActiveObject) {
        window[_canvas].discardActiveObject().renderAll();
      }
    });

    // If the checkbox is turned on, we activate the selected canvas.
    if ((_selected_toggle).prop('checked')) {
      _selected_wrapper.removeClass('notouchie');
      _selected_link.addClass('selected');
    }
    $('#map_wrapper').removeClass('notouchie');
  }

  /**
   * Helper generates html for the options window.
   */
  function object_options(obj) {
    let html;
    let blending_mode, color, remove_color, distance, opacity,
    blending_mode_content ='', remove_color_content = '', opacity_content = '';

    opacity = obj.opacity ? obj.opacity : 1;

    blending_mode_content = `
      <div id="element_blending_mode_wrapper">
        <label for="element_blending_modes">Blending Mode</label>
        <select id="element_blending_modes">
          <options>
            <option selected="selected" value="source-over">Normal</option>
            <option value="multiply">Multiply</option>
            <option value="screen">Screen</option>
            <option value="overlay">Overlay</option>
            <option value="darken">Darken</option>
            <option value="lighten">Lighten</option>
            <option value="color-dodge">Color Dodge</option>
            <option value="color-burn">Color Burn</option>
            <option value="hard-light">Hard Light</option>
            <option value="soft-light">Soft Light</option>
            <option value="difference">Difference</option>
            <option value="hue">Hue</option>
            <option value="exclusion">Exclusion</option>
            <option value="saturation">Saturation</option>
            <option value="color">Colour</option>
            <option value="luminosity">Luminosity</option>
          </options>
        </select>
      </div>
    `;

    opacity_content = `
      <div id="element_opacity_wrapper">
        <label for="element_opacity">Opacity</label>
        <input id="element_opacity" type="range" value="${opacity}" min="0" max="1" step="0.01" />
      </div>
    `;

    // If we are not looking at a videos properties, we will print the filter controls.
    if (!obj.video) {
      remove_color = obj.filters[FILTER_REMOVE_COLOR] ? obj.filters[FILTER_REMOVE_COLOR] : null;
      remove_color_color = obj.filters[FILTER_REMOVE_COLOR] ? obj.filters[FILTER_REMOVE_COLOR].color : '';

      remove_color_content = `
        <div id="element_remove_color_wrapper">
          <label for="element_remove_color">Remove Colour</label>
          <input id="element_remove_color" type="checkbox" value="${remove_color}" />
          <input id="element_remove_color_color" type="color" value="${remove_color_color}" />
          <label for"element_remove_color_distance">Color Range</label>
          <input id="element_remove_color_distance" type="range" value="${distance}" min="0.01" max="1" step="0.01" />
        </div>
      `;
    }

    html = blending_mode_content + remove_color_content + opacity_content;

    return html;
  }

  /**
   * Helper generates events for the options menu opened.
   */
  function object_options_events(obj) {
    $('#element_opacity').on('change, input', function() {
      if (obj) {
        obj.set({
          opacity: this.value
        });
      }
    }).change();

    $('#element_blending_modes').on('change, input', function() {
      let mode = $(this).find(":selected").attr('value');
      if (obj) {
        obj.set({'globalCompositeOperation': mode});
      }
    }).change();

    $('#element_remove_color').on('click', function() {
      if (this.checked) {
        if (obj.video) {
          alert('Cannot remove colour from video yet.');
          this.checked = false;
          return;
        }
        // Turn on FILTER_REMOVE_COLOR.
        applyFilter(obj, FILTER_REMOVE_COLOR, new f.RemoveColor({
          distance: $('#element_remove_color_distance').val(),
          color: $('#element_remove_color_color').val()
        }));
      }
      else {
        // Turn off FILTER_REMOVE_COLOR.
        if (getFilter(obj, FILTER_REMOVE_COLOR)) {
          obj.filters.splice([FILTER_REMOVE_COLOR]);
          obj.applyFilters();
        }
      }
    });
    $('#element_remove_color_color').on('change, input',function() {
      let color = $('#element_remove_color_color').val();
      if (obj) {
        if (getFilter(obj, FILTER_REMOVE_COLOR)) {
          applyFilterValue(obj, FILTER_REMOVE_COLOR, 'color', color);
        }
      }
    });
    $('#element_remove_color_distance').on('change, input',function() {
      let distance = $('#element_remove_color_distance').val();
      if (obj) {
        if (getFilter(obj, FILTER_REMOVE_COLOR)) {
          applyFilterValue(obj, FILTER_REMOVE_COLOR, 'distance', distance);
        }
      }
    });
  }

  /**
   * Gets the filter asked for by index.
   * Index should be a constant.
   */
  function getFilter(obj = NULL, index) {
    if (!obj) {
      obj = window['map'].getActiveObject();
    }
    if (obj) {
      if (obj.filters[index]) {
        return obj.filters[index];
      }
    }
    return false;
  }

  function applyFilter(obj = NULL, index, filter) {
    if (!obj) {
      obj = window['map'].getActiveObject();
    }
    // if (obj)
    obj.filters[index] = filter;
    // var timeStart = +new Date();
    obj.applyFilters();
    // var timeEnd = +new Date();
    // var dimString = window['map'].getActiveObject().width + ' x ' +
      // window['map'].getActiveObject().height;
    // console.log(dimString + 'px ' +
    //   parseFloat(timeEnd-timeStart) + 'ms');
  }

  function applyFilterValue(obj = null, index, prop, value) {
    if (!obj) {
      obj = window['map'].getActiveObject();
    }
    if (obj.filters[index]) {
      obj.filters[index][prop] = value;
      // var timeStart = +new Date();
      obj.applyFilters();
      // var timeEnd = +new Date();
      // var dimString = window['map'].getActiveObject().width + ' x ' +
      //   window['map'].getActiveObject().height;
      // console.log(dimString + 'px ' +
      //   parseFloat(timeEnd-timeStart) + 'ms');
    }
  }

  function open_layer_options(obj) {
    $('#questions').html(object_options(obj));
    object_options_events(obj);

    let blending_mode = obj.globalCompositeOperation ? obj.globalCompositeOperation : 'source-over';
    document.getElementById('element_blending_modes').value = blending_mode;

    if (!obj.video) {
      let remove_color = obj.filters.FILTER_REMOVE_COLOR;
      if (remove_color) {
        document.getElementById('element_remove_color').value = true;
        document.getElementById('element_remove_color_color').value = remove_color.color;
        document.getElementById('element_remove_color_distance').value = remove_color.distance;
      }
    }

    $('#questions').dialog({
      resizable: false,
      height: "auto",
      width: 400,
      modal: true,
      buttons: {
        'Close': function() {
          $(this).dialog('close');
        }
      }
    });
  }

  /**
   * Checks incoming for id, or target.id for either invoking function.
   */
  function selected(e) {
    let id = -1;
    id = e.id;
    if (!id) {
      id = e.target.id;
    }
    if (!id) {
      console.log('Nothing selected; nothing gained.');
      return;
    }
    let row = get_row(id, $("#layering").jsGrid('option', 'data')),
    selected_layer = $("#layering").find('table tr.highlight');
    if (selected_layer.length) {
        selected_layer.removeClass('highlight');
    };
    if (row) {
      $(row).addClass("highlight");
    }
    if (e.e && e.e.ctrlKey) {
      let obj = -1
      if (e.target) {
        obj = e.target;
        if (!obj && e.selected) {
          obj = e.selected[0];
        }
      }
      if (obj) {
        open_layer_options(obj);
        return;
      }
      console.log('No object found.');
    }
  }

  window['map'].on('object:selected', function(e) {selected(e);});
  window['map'].on('selection:updated', function(e) {selected(e);});

  window['map'].on('object:added', function(e) {
    check_object_vs_map(e);
    let id, from_id, row, new_row;

    if (e.target && e.target.id) {
      id = e.target.id;
      if (e.target.from_id) {
        from_id = e.target.from_id;
      }
    }

    if (id) {
      row = get_row(id);
      if (!row && from_id) {
        row = get_row(from_id);
      }
    }
    else {
      console.log(id + " not found.");
    }

    if (row) {
      $('#layering').jsGrid('insertItem', {
        'id': id,
        'Filename': row.Filename,
        'Thumbnail': row.Thumbnail
      });
    }
    else {
      if (e.target && e.target.text) {
        $('#layering').jsGrid('insertItem', {
          'id': id,
          'Filename': e.target.text,
          'Thumbnail': 'data:image/svg+xml;base64,PHN2ZyBhcmlhLWhpZGRlbj0idHJ1ZSIgZGF0YS1wcmVmaXg9ImZhcyIgZGF0YS1pY29uPSJ0YWciIGNsYXNzPSJzdmctaW5saW5lLS1mYSBmYS10YWcgZmEtdy0xNiIgcm9sZT0iaW1nIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA1MTIgNTEyIj48cGF0aCBmaWxsPSJjdXJyZW50Q29sb3IiIGQ9Ik0wIDI1Mi4xMThWNDhDMCAyMS40OSAyMS40OSAwIDQ4IDBoMjA0LjExOGE0OCA0OCAwIDAgMSAzMy45NDEgMTQuMDU5bDIxMS44ODIgMjExLjg4MmMxOC43NDUgMTguNzQ1IDE4Ljc0NSA0OS4xMzcgMCA2Ny44ODJMMjkzLjgyMyA0OTcuOTQxYy0xOC43NDUgMTguNzQ1LTQ5LjEzNyAxOC43NDUtNjcuODgyIDBMMTQuMDU5IDI4Ni4wNTlBNDggNDggMCAwIDEgMCAyNTIuMTE4ek0xMTIgNjRjLTI2LjUxIDAtNDggMjEuNDktNDggNDhzMjEuNDkgNDggNDggNDggNDgtMjEuNDkgNDgtNDgtMjEuNDktNDgtNDgtNDh6Ij48L3BhdGg+PC9zdmc+'
        });
      }
    }
  });

  /**
   * Token time.  Here's our settings html.
   */
  function token_settings_html() {
    let html = `
    <div id="token_settings_wrapper">
      <div id="token_settings_title">Token settings</div>
      <div id="token_settings_close"><i class="fas fa-window-close"></i></div>
      <label for="token_opacity">Opacity</label>
      <input id="token_opacity" type="range" min="0.1" max="1" step="0.1" />
      <label for="token_opacity">Colour</label>
      <input id="token_color" type="color" />
    </div>
    `;
    return html;
  }

  function open_token_settings(e) {
    let _settings_wrapper = $('#token_settings_wrapper').get(0);
    $('#token_opacity').target = window['tokens'].getActiveObject();
    if (!_settings_wrapper) {
      $(document.body).append(token_settings_html());
    }
    if (_opacity = $('#token_opacity').get(0)) {
      _settings_wrapper = $('#token_settings_wrapper');
      _settings_wrapper.fadeIn(250, function() {
        _settings_wrapper.tid = e.target.id;
      });
      let _color = $('#token_color').get(0);

      _opacity.value = e.target.opacity;
      _color.value = e.target.fill;

      $('#token_opacity').on('change, input', function(o) {
        if (window['tokens'].getActiveObject()) {
          window['tokens'].getActiveObject().set({
            opacity: this.value
          });
          window['tokens'].renderAll();
        }
      }).change();

      $('#token_color').on('change, input', function(o) {
        console.log(this.value);
        if (window['tokens'].getActiveObject()) {
          window['tokens'].getActiveObject().set({
            fill: this.value
          });
          window['tokens'].renderAll();
        }
      }).change();

      $('#token_settings_close').on('click', function() {
        _settings_wrapper.fadeOut(250, function() {
          this.remove();
        });
      });
      let absCoords = window['tokens'].getAbsoluteCoords(e.target);
      _settings_wrapper.css({
        'left': (absCoords.left - (_settings_wrapper.width() / 2)) + 'px',
        'top': (absCoords.top +  (_settings_wrapper.height() / 2)) + 'px'
      });
    }
  }

  /**
   * Token canvas events.
   */
  window['tokens'].observe('selection:cleared', function() {
    $('#token_settings_wrapper').fadeOut(250, function() {
      this.remove();
    });
  });

  window['tokens'].observe('object:moving', function(e) {
    let _settings_wrapper = $('#token_settings_wrapper');
    if (_settings_wrapper.get(0)) {
      let absCoords = window['tokens'].getAbsoluteCoords(e.target);
      _settings_wrapper.css({
        'left': (absCoords.left - (_settings_wrapper.width() / 2)) + 'px',
        'top': (absCoords.top +  (_settings_wrapper.height() / 2)) + 'px'
      });
    }
  });

  window['tokens'].observe('mouse:down', function(e) {
    if (!e.target) {
      let color = $('#tokens_colour').val();
      let pointer = window['tokens'].getPointer(e.e);
      let radius = $('#tokens_size').val();
      origX = pointer.x;
      origY = pointer.y;
      circle = new fabric.Circle({
        left: pointer.x,
        top: pointer.y,
        radius: parseInt(radius),
        // strokeWidth: 1,
        // stroke: 'black',
        fill: color,
        selectable: true,
        originX: 'center', originY: 'center'
      });
      window['tokens'].add(circle);
    }
    if (e.target) {
      open_token_settings(e);
    }
    if (e.e.shiftKey) {
      window['tokens'].remove(window['tokens'].getActiveObject());
      $('#token_settings_wrapper').fadeOut(250, function() {
        this.remove();
      });
    }
    window['tokens'].renderAll();
  });

  window['tokens'].observe('after:render', function() {
    update_player_tokens();
  })

  function update_player_tokens() {
    if (player_view) {
      if (window['player_tokens']) {
        let box = window['player_tokens'].getBoundingClientRect();
        let ctx = window['player_tokens'].getContext('2d');
        ctx.clearRect(0, 0, box.width, box.height)
        ctx.drawImage(window['tokens'].getElement(), 0, 0, box.width, box.height);
      }
    }
  }

  /**
  * Get a row from provided jsgrid data.
   */
  function get_row(id, d = false) {
    if (!d) {
      d = $('#files').jsGrid('option', 'data');
    }
    idx = d.findIndex(function(e) {
      return e.id == id;
    });
    return d[idx];
  }

  /**
   * Rendering function.
   * Rendering canvas elements one millisecond at a time.
   */
  fabric.util.requestAnimFrame(function render() {
    // Pre filter filtered images before rendering the canvas.
    // let result = canvas.item.find(obj => {
    //   return obj.b === 6;
    // })
    window['map'].getObjects().forEach(function(obj) {
      let backend = fabric.filterBackend;
      if (obj.video) {
        if (0 < obj.filters.length) {
          if (backend && backend.evictCachesForKey) {
            backend.evictCachesForKey(obj.cacheKey);
            backend.evictCachesForKey(obj.cacheKey + '_filtered');
            obj.applyFilters();
          }
        }
      }
    });

    window['map'].renderAll();

    /**
     * Update the player view on frame.
     */
    if (player_view) {
      if (window['player_map']) {
        let box = window['player_map'].getBoundingClientRect();
        let ctx = window['player_map'].getContext('2d');
        ctx.clearRect(0, 0, box.width, box.height);
        ctx.drawImage(window['map'].getElement(), 0, 0, box.width, box.height);
      }
    }

    fabric.util.requestAnimFrame(render);
  });

  // Using jQuery with fabricjs to call the canvas function
  function load_canvas_data(data = null) {
    canvases.forEach(function(canvas) {
      let _canvas = canvas + '_canvas';
      let _canvas_content = canvas + '_canvas_content';
      if (Drupal.settings.use_local_storage) {
        window[_canvas_content] = localStorage.getItem(canvas + '_canvas_content');
        if (window[_canvas_content]) {
          window[_canvas].loadFromJSON(window[_canvas_content]);
        }
        return;
      }
      if (!data) {
        console.log('No canvas data found in .dmge file.')
        return;
      }
      if (data) {
        console.log(data);
        window[_canvas].loadFromJSON(JSON.stringify(data[_canvas]), function() {
        }, function(o, object) {
          console.log(o, object);
        });
        return;
      }
    });

  }

  /**
   * Helper assigns json to txt and link, then clicks for download.
   */
  function file_storage(content, filename, content_type = 'text/json') {
    var a = document.createElement("a");
    var file = new Blob([content], {type: content_type});
    a.href = URL.createObjectURL(file);
    a.download = filename;
    a.click();
    a.remove();
  }

  function save_show_save_icon() {
    if (!$('#save_icon')) {
      let div = document.createElement('div');
      div.setAttribute('id', 'save_icon');
      $('#save_icon').hide().html('<i class="far fa-save"></i>');
    }
    $('#save_icon').fadeIn();
  }

  function save_show_error() {
    if (!$('#error')) {
      let div = document.createElement('div');
      div.setAttribute('id', 'error');
    }
    $('#error').hide().html('<i class="fas fa-exclamation-triangle"></i>').append('Error saving map.');
  }

  function save_hide_save_icon() {
    $('#save_icon').fadeOut();
  }

  /**
   * Helper to run the saving of canvases.
   */
  function save_canvases() {
    let map_name = document.getElementById('map_name').value;
    if (!map_name) {
      map_name = 'untitled';
    }
    let storage = {};
    storage.grid = {
      size: parseInt(document.getElementById('map_grid_size').value),
      type: $('input[name=map_grid_type]:checked').val(),
    }
    canvases.forEach(function(canvas) {
      let _canvas = canvas + '_canvas';
      let $_canvas = $('#' + canvas).get(0);
      let _canvas_content = _canvas + '_content';
      let _json = -1;

      if (['grid', 'map', 'particles', 'tokens'].includes(canvas)) {
        if (['grid'].includes(canvas)) {
          return;
        }
        _json = window[_canvas].toJSON(['rotation', 'x', 'y', 'width', 'height']);
      } else {
        _json = $_canvas.toDataURL();
      }
      if (_json) {
        // Set content variable for local storage.
        if (Drupal.settings.use_local_storage) {
          localStorage.setItem(_canvas_content, _json);
        }

        // Add to storage object for json export.
        storage[_canvas] = _json;
      }
    });
    if (storage) {
      storage = JSON.stringify(storage);
      let file = file_storage(storage, map_name + '.dmge');
      if (file) {
        return true;
      }
    }
    // If we failed to generate content, return false.
    return false;
  }

  $('#save_map').click(function(e) {
    save_show_save_icon();
    if (save_canvases()) {
      save_hide_save_icon();
    }
    else {
      save_show_error();
    }
  });

  function dragging_initialize() {
    window['map'].selection = false;
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
        // If holding shift, scroll the screen.
        // TODO: This will be changed for viewport.
        if (e.shiftKey) {
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
      // 'mouseup': function() {
      //   $('#map_wrapper').removeClass('notouchie');
      //   clicked = false;
      //   document.getElementById('grid_wrapper').style.pointerEvents = false;
      //   window['map'].selection = true;
      //   $('html').css('cursor', 'auto');
      //   if ($('#error')) {
      //     $('#error').fadeOut();
      //   }
      // }
  });

  /**
   * Got it from stack overflow.
   * Snaps to grid.
   */
  window['map'].on('object:moving', function(options) {
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

  window['map'].on('object:scaling', function(options) {
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
  function getObjectFromCanvasById(id, canvas = window['map']) {
    let objs = -1;
    objs = canvas.getObjects().filter(function (item) {
      return item.id == parseInt(id);
    });
    if (objs) {
      return objs[0];
    }
    return false;
  }

  /**
   * Gets all items by from id.
   */
  function getObjectsFromCanvasByFromId(from_id, canvas = window['map']) {
    let objs = -1;
    objs = canvas.getObjects().filter(function (item) {
      return item.from_id == parseInt(from_id);
    });
    if (objs) {
      return objs;
    }
    return false;
  }

  /**
   * Helper to remove a single object from canvas.
   */
  function removeObjectFromCanvasById(id, canvas = window['map']) {
    let obj = -1;
    obj = getObjectFromCanvasById(id, canvas);
    if (obj) {
      canvas.remove(obj);
      return obj;
    }
    return false;
  }

  /**
   * Helper to remove multiple objects from canvas.
   */
  function removeObjectsFromCanvas(objs, canvas = window['map']) {
    objs.forEach(function(item) {
      canvas.remove(item);
    });
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
    _size = _size * window['map'].getZoom();
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

  // window['map'].on('mouse:wheel', function(opt) {
  //   var delta = opt.e.deltaY;
  //   var zoom = window['map'].getZoom();
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
  //     window['grid'].viewportTransform[4] = this.viewportTransform[4] = 0;
  //   } else if (vpt[4] < window['map'].getWidth() * zoom) {
  //     window['grid'].viewportTransform[4] = this.viewportTransform[4] = window['map'].getWidth() * zoom;
  //   }
  //   if (vpt[5] >= 0) {
  //     window['grid'].viewportTransform[5] = this.viewportTransform[5] = 0;
  //   } else if (vpt[5] < window['map'].getHeight() * zoom) {
  //     window['grid'].viewportTransform[5] = this.viewportTransform[5] = window['map'].getHeight() * zoom;
  //   }
  // });


  // General click event to close the sidebar.
  $(document).on('click', function(e) {
    if ($(e.target).closest('#sidebar').length === 0) {
      $('#sidebar').hide('slide', {direction:'right'});
    }
  });

  /**
   * No right click in the Fog of war, it complicates matters.
   */
  $('#fow').contextmenu(function() {
      return false;
  });

  /**
   * When we press a key, the world changes.
   */
  $(document).on('keydown', function(e) {

    // If we're in an input, don't do shit.
    if ($('input').is(':focus')) {
      return;
    }
    // If it's one of the number keys at the top then count off and activate a sidebar menu item.
    if ((e.which >= 48) && (e.which <= 57)) {
      $('#sidebar_sections .sidebar_section').hide();
      try {
        var hash = $('#menu a').get(e.which - 56).hash.substr(1);
      } catch (e) {
        console.log('Nothing there.');
      } finally {
        $('#' + hash).toggle('slide', {direction:'up'});
      }
    }

    switch (e.which) {
      // Escape
      case 27:
        $('#sidebar').toggle('slide', {direction:'right'});
        break;

      // f1
      case 112:
      // Qqqqqqquestions need answers
      case 81:
        __toggle = Drupal.howto.dialog('isOpen') ? 'close' : 'open';
        Drupal.howto.dialog(__toggle);
        break;
      // rrrrrefresh killer
      case 81:
        // Disable ctrl r refresh.
        if (e.ctrlKey) {
          e.preventDefault();
          e.cancelBubble = true;
        }
        break;
      // cccccatalogue
      case 67:
        e.cancelBubble = true;
        e.preventDefault();
        open_map_catalog();
        break;
      // Fffffog of war
      case 70:
        e.cancelBubble = true;
        e.preventDefault();
        if (e.shiftKey) {
          if (e.altKey) {
            $('#questions').html('Restore this FOW?');
            $('#questions').dialog({
              resizable: false,
              height: 'auto',
              width: 400,
              modal: true,
              buttons: {
                'Yes': function() {
                  __fow = localStorage.getItem('fow_content');
                  $(this).dialog('close');
                },
                Cancel: function() {
                  $(this).dialog('close');
                }
              }
            });
          }
          if (e.ctrlKey) {
            $('#questions').html('Save this FOW?');
            $('#questions').dialog({
              resizable: false,
              height: 'auto',
              width: 400,
              modal: true,
              buttons: {
                'Yes': function() {
                  __fow = localStorage.setItem('fow_content', window['fow'].toDataURL());
                  $(this).dialog('close');
                },
                Cancel: function() {
                  $(this).dialog('close');
                }
              }
            });
          }
          return;
        }
        toggle_canvas('fow');
        break;
      // gggggrid toggle
      case 71:
        e.cancelBubble = true;
        e.preventDefault();
        $('#grid_toggle').click();
        toggle_canvas('grid');
        break;
      // Pppplayer view
      case 80:
        e.cancelBubble = true;
        e.preventDefault();
        player_view_open();
        break;

      // ttttt
      case 84:
      // mmmmm
      case 77:
        e.cancelBubble = true;
        e.preventDefault();
        $('#tokens_toggle').click();
        toggle_canvas('tokens');
        break;

      // Zzzz
      case 90:

        break;

      // XXX
      case 88:

        break;

      // Backspace
      case 8:
      // Delete
      case 46:
        e.preventDefault();
        e.cancelBubble = true;
        delete_objects();
        break;

      // F5
      case 116:
        // Prevent accidental refresh.
        e.preventDefault();
        e.cancelBubble = true;
        break;

      default:

    }
  });

  /**
   * Someones got to delete objects.
   */
  function delete_objects() {
    canvases.forEach(function(canvas) {
      switch (canvas) {
        case 'fow':
        case 'grid':
        //
          break;
        default:
          let _obj;
          if (_obj = window[canvas].getActiveObject()) {
            switch (canvas) {
              case 'map':
                if (confirm('Are you sure?')) {
                  window[canvas].remove(_obj);
                }
              break;
              default:
                window[canvas].remove(_obj);
            }
          }
        }
    });
  }

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
   * Grid drag a-go.
   */
  $('#grid_toggle').change(function() {
    toggle_canvas('grid');
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
      window['player_grid_wrapper'].css('opacity', get_opacity($('#map_grid_opacity')));
    }
  });

  $('#fow_opacity').on('input', function() {
    $('#fow_wrapper').css('opacity', get_opacity($('#fow_opacity')) / 2);
    if (player_view) {
      window['player_fow_wrapper'].css('opacity', get_opacity($('#fow_opacity')));
    }
  });

  /**
   * Helper stores FoW content to window variable.
   */
  function fow_store_content() {
    // Get image data from canvas and store it to window variable.
    window.fow_content = fow_ctx.getImageData(0, 0, window['fow'].width, window['fow'].height);
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
    fow_reset();
    // window.fow_content = JSON.parse(localStorage.getItem('current_fow_content'));
    if (window.fow_content) {
      fow_ctx.putImageData(window.fow_content, 0, 0);
      render_fow_canvas();
    }
  }
  $('#fow_recall').click(function(e) {
    fow_recall_content();
  });

  /**
   * Render the Fog of War data in variable to player canvas
   */
  function render_fow_canvas() {
    if (window['player_fow']) {
      let box = window['player_fow'].getBoundingClientRect();
      let ctx = window['player_fow'].getContext('2d');
      ctx.clearRect(0, 0, box.width, box.height);
      ctx.drawImage(window['fow'].getElement(), 0, 0, box.width, box.height);
    }
  }

  /**
   * Helper initializes fog of war.
   */
  function fow_reset() {
    fow_ctx.globalCompositeOperation = 'source-over';
    fow_ctx.fillStyle = 'rgba( 0, 0, 0, 1 )';
    fow_ctx.fillRect(0, 0, window['fow'].width, window['fow'].height);
    if (window['player_fow']) {
      window['player_fow'].width = window['fow'].width;
      window['player_fow'].height = window['fow'].height;
      render_fow_canvas();
    }
  }
  $('#fow_reset').click(function(e) {
    fow_reset();
  });
  fow_reset();

  /**
   * Fog On!
   */
  $("#fow_enabler").change(function() {
    fow_store_content();
    if (this.checked) {
      $('label[for="fow_enabler"]').html('Fog of War Visible');
    }
    if (!this.checked) {
      $('label[for="fow_enabler"]').html('Fog of War Hidden');
    }
    $('canvas#fow').toggleClass('active', this.checked);
    if ($(this).hasClass('active')) {
      fow_recall_content();
    }
  }).change();

  $('#fow_toggle').change(function() {
    $('#cursor').toggleClass('active', this.checked);
    $('#fow').toggleClass('brush-active', this.checked);
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

  /**
   * Set gif.
   * url is actually a file.
   */
  function do_gif(_id, _file) {
    var data = new FormData();
    data.append('dmge_gif2mp4', _file);
    $.ajax({
      url: Drupal.settings.basePath + 'engine/gif2mp4',
      processData: false,
      method: 'POST',
      type: 'POST',
      contentType: false,
      dataType: 'json',
      data: data,
      beforeSend: function() {
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-cog fa-spin"></i> Loading ' + _file.name + ' to Cloudinary</div>');
      },
      success: function(data) {
        do_video(_id, data[0], 'mp4');
        $('#file_status').html('<div id="file_remote_load_progress" class="success"><i class="fas fa-check"></i> Loaded ' + _file.name + '</div>');
        $('#files').jsGrid('insertItem', {'id': _id, 'Filename': _file.name, 'Blob': data[0], 'Type': 'GIF', 'Thumbnail': window.URL.createObjectURL(_file) });
      },
      error: function(data) {
        console.error(data);
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-exclamation-triangle"></i> Something has tragically failed.</div>');
      },
      complete: function() {
        $('#file_remote_load_progress').fadeOut(2000);
      }
    });
    return true;
  }

  /**
   * Set video.
   */
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
    vtag.prop('volume', $('#map_master_volume').val());
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
    if (code = get_youtube_code($('#map_embed').val())) {
      // Fire our backend script with our id code.
      do_youtube(code);
      return;
    };
    // If we got bupkiss, let's look for an extension?  Maybe it's an mp4.
    if (!code) {
      console.log('Not a youtube.  Load a file.');

      let url = $('#map_embed').val();
      let ext = getExtension(url);

      if (['mp4', 'mkv'].includes(ext)) {
        let id = make_file_id(url);
        let name = url.split(/(\\|\/)/g).pop();
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-cog fa-spin"></i> Reaching out for ' + name + '</div>');
        let vtag = do_video(id, url, 'mp4');
        vtag.on('loadeddata', function() {
          $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-check"></i> Loaded ' + name + '</div>');
          $('#file_remote_load_progress').fadeOut(2000);
        });
        vtag.on('error', function(error) {
          console.error(error);
          $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-exclamation-triangle"></i> Something failed.</div>');
          $('#file_remote_load_progress').fadeOut(2000);
        });
        $('#files').jsGrid('insertItem', {'id': id, 'Filename': name, 'Blob': url, 'Type': 'Remote'});
      }

      if (['jpg', 'png'].includes(ext)) {
        let id = make_file_id(url);
        let name = url.split(/(\\|\/)/g).pop();
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-cog fa-spin"></i> Reaching out for ' + name + '</div>');
        do_image(id, url);
        $('#files').jsGrid('insertItem', {'id': id, 'Filename': name, 'Blob': url, 'Type': 'Remote'});
      }

      if (url.includes('https://www.patreon.com/file')) {
        $.ajax({
          url: url,
          // type: 'get',
          dataType: 'script',
          success: function(response) {
            console.log(response);
            $('#file_status').html('<div id="file_remote_load_progress"><i class="fab fa-patreon"></i> loaded from Patreon.</div>');
            $('#file_remote_load_progress').fadeOut(2000);
          },
          complete: function(response) {
            console.log(response);
            $('#file_status').html('<div id="file_remote_load_progress"><i class="fab fa-patreon"></i> loaded from Patreon.</div>');
            $('#file_remote_load_progress').fadeOut(2000);
          },
          beforeSend: function(data) {
            $('#file_status').html('<div id="file_remote_load_progress"><i class="fab fa-patreon"></i> reaching out to Patreon.</div>');
          }
        });
      }

    }
  });

  /**
   * Helper fires GET and then sets off code and video processing.
   */
  function do_youtube(code) {
    $.ajax({
      url: '/engine/youtube?v=' + code,
      type: 'get',
      dataType: 'json',
      success: function(response) {
        if (response[0] && response[0].url) {
          let _url = response[0].url;
          let _id = make_file_id(_url);
          do_video(_id, _url, 'mp4');
          $('#files').jsGrid('insertItem', {'id': _id, 'Filename': response['title'], 'Blob': _url, 'Type': 'YouTube', 'Thumbnail': response['thumbnail'] });
          $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-check"></i> Loaded  ' + response['title'] + '</div>');
          return true;
        }
        console.log(response);
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-exclamation-triangle"></i> YouTube has failed us.</div>');
      },
      beforeSend: function() {
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-cog fa-spin"></i> Reaching out to YouTube for  ' + code + '</div>');
      },
      error: function(response) {
        console.error(response);
        $('#file_status').html('<div id="file_remote_load_progress"><i class="fas fa-exclamation-triangle"></i> Something has tragically failed.</div>');
      },
      complete: function() {
        $('#file_remote_load_progress').fadeOut(2000);
      }
    });
  }

  /**
   * Adds image to canvas from provided url, with provided id.
   */
  function do_image(_id, _url) {
    window[_id] = fabric.Image.fromURL(_url, function(img) {
      img.set({
        id: _id
      })
      window['map'].add(img);
    });
  }

  $('#map_reset_zoom').click(function() {
    window.parent.document.body.style.zoom = 1;
  });

  $('#map_clear').click(function() {
    window['map'].clear();
    window['fow'].clear();
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
  function getQueryParams(url = '') {
    try {
      if (!url) {
        let url = window.location.href;
      }
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

  $(document).ready(function() {
    setupTimers();
    fow_reset();
    let params = getQueryParams();
    if (params.v) {
      do_youtube(params.v);
    }
    $('#map_width').val(screensize.width);
    $('#map_height').val(screensize.height);
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


  $('#template_circle, #template_square, #template_triangle').click(function(e) {
    switch (e.id) {
      case 'template_circle':

        break;
      case 'template_square':

        break;
      case 'template_triangle':

        break;
      default:

    }
  });

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

  $('#fow').on('mouseup', function() {
    if (window['player_fow']) {
      window['player_fow'].width = window['fow'].width;
      window['player_fow'].height = window['fow'].height;
      render_fow_canvas();
    }
  });

  $('#fow').on('mousemove', function(ev, ev2) {
    if (ev.buttons == 1) {
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
    window['grid'].clear();
    if (window['player_grid']) {
      let box = window['player_grid'].getBoundingClientRect();
      window['player_grid'].getContext('2d').clearRect(0, 0, box.width, box.height);
    }
    var _type = $('input[name=map_grid_type]:checked').val();
    if (_type == 'None') {
      return;
    }

    $('#grid_wrapper').css('opacity', get_opacity($('#map_grid_opacity')));

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
    window['grid'].Grid = Grid;

    _cols = (_width / _size);
    _rows = (_height / _size);

    const __grid = Grid.rectangle({width: _cols, height: _rows});
    window['grid'].__grid = __grid;
    if ((window['grid'].width !== window['map'].width) || (window['grid'].height !== window['map'].height)) {
      window['grid'].width = window['map'].width;
      window['grid'].height = window['map'].height;
    }


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

          hasBorders: false,
          hasControls: false,

          lockMovementX: true,
          lockMovementY: true,
          objectCaching: true
        };

        let hexSymbol = new fabric.Polygon(corners, _props, false);

        window['grid'].add(hexSymbol);

      });
    }

    // The grid is now just lines.
    if (_type === 'Quad') {
      for (var x = 1; x < (window['grid'].width / _size); x++) {
        window['grid'].add(new fabric.Line([_size * x, 0, _size * x, window['grid'].height], {
          stroke: "#ffffff",
          strokeWidth: 1,
          selectable: false
        }));
      }
      for (var x = 1; x < (window['grid'].height / _size); x++) {
        window['grid'].add(new fabric.Line([0, _size * x, window['grid'].width, _size * x], {
          stroke: "#ffffff",
          strokeWidth: 1,
          selectable: false
        }));
      }
    }

    window['grid'].renderAll();

    if (window['player_grid']) {
      let box = window['player_grid'].getBoundingClientRect();
      let ctx = window['player_grid'].getContext('2d');
      ctx.clearRect(0, 0, box.width, box.height);
      ctx.drawImage(window['grid'].getElement(), 0, 0, box.width, box.height);
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

  $('#map_master_volume').on('change, input', function(e) {
    $('video').prop('volume', $(this).val());
  }).change();

  /**
   * Load a campaign folder.
   */
  $('#files_storage_path_configure').change(function() {
    let files = this.files;
    if (!files) {
      return;
    }
    let len = files.length,
        relative_path = files[0].webkitRelativePath,
        folder = relative_path.split("/");

    loadFiles(files);
  });

  /**
   * Load files from the file dialogue.
   */
  $('#file').change(function() {
    loadFiles($(this));
  });

  if (!Array.isArray) {
    Array.isArray = function(arg) {
      return Object.prototype.toString.call(arg) === '[object Array]';
    };
  }

  /**
   * Helper loads files from input or dialog.
   */
  function loadFiles(files) {
    if (typeof files.prop === "function") {
      files = files.prop('files');
    }
    let data;
    if (!files) {
      console.error('No files selected?');
      return;
    }
    $.each(files, function(key, _file) {
      let _url = window.URL.createObjectURL(_file),
      _id = make_file_id(_url),
      _thumbnail = _url,
      _type = 'Static',
      ext = getExtension(_file.name);

      if (ext == 'gif') {
        _url = _file;
      }

      if (loaded = loadFile(_url, ext, _id)) {
        if (!['gif', 'dmge'].includes(ext)) {
          $('#files').jsGrid('insertItem', {'id': _id, 'Filename': _file.name, 'Blob': _url, 'Type': loaded.type, 'Thumbnail': _thumbnail });
        }
      }
    });
  }

  function loadFile(_url, ext, _id = NULL) {
    let file = {
      type: 'Static',
      url: _url,
      ext: ext,
      id: _id
    };
    switch (ext) {
      case 'pdf':
        do_pdf(_url);
        break;

      case 'jpg':
      case 'jpeg':
      case 'bmp':
      case 'png':
        do_image(_id, _url, ext);
        break;
      case 'gif':
        file.type = 'Animated';
        do_gif(_id, _url);
        break;

      case 'm4v':
        ext = 'x-m4v';
      case 'mpg':
      case 'mp4':
        file.type = 'Animated';
        do_video(_id, _url, ext);
        break;

      case 'txt':
      case 'dmge':
        $.ajax({
          url : _url,
          dataType: "text",
          success : function (data) {
            load_canvas_data(data);
          }
        });
        break;
    default:
     items = ['You look great, by the way :)', 'You look very nice today.  I hope you\'re well.', 'That tickled.', 'I wish all my friends looked as good as you :)'];
     var item = items[Math.floor(Math.random()*items.length)];
     alert('Sorry, I don\'t know what you are trying to load.\n\n' + item);
     break;
    }
    return file;
  }

  /**
   * Helper makes unique ID from hash of current time and url.
   */
  function make_file_id(filename) {
    filename = filename + '';
    let t = todays_date.getTime();
    let random = Math.round(Math.random() * 1000);
    t = filename.hashCode() + (t + random);
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

    onItemInserted: function(e) {
      // The closest to a central function for video loading we have.
      let item = e.item;
      let _id = $('#' + item.id);
      let tag = document.getElementById(item.id);
      // 'Types' of files we can make a thumbnail from.
      const types = new Array('YouTube', 'Remote', 'GIF');
      if (_id.is('video.map_video')) {
        $(_id).on('play', function(e) {
          if (types.indexOf(item.Type) == -1) {
            item.Thumbnail = make_video_thumbnail(item.id);
          }
          $('#files').jsGrid("updateItem", item, {'Thumbnail': item.Thumbnail});
          window[item.id] = new fabric.Image(tag, {
            id: item.id,
            from_id: item.id,
            originX: 'left',
            originY: 'top',
            height: tag.videoHeight,
            width: tag.videoWidth,
            video: true,
            left: 0,
            top: 0
          });
          window['map'].add(window[item.id]);
          $(_id).off('play');
        });
      }
    },

    fields: [
      // { name: 'Bulk Op', type: 'checkbox' },
      { name: 'id', type: 'number', visible: false },
      { name: 'from_id', type: 'number', visible: false },
      { name: 'Filename', type: 'text', width: '50%' },
      { name: 'Blob', type: 'text', width: '25%', visible: false },
      { name: 'Type', type: 'select', items: ['Static', 'Animated', 'YouTube', 'Remote', 'GIF'] },
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
            add_clone(val, item);
          });
        },
        align: 'center',
        width: 120
      },
      { name: 'Delete',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-trash" aria-hidden="true"></i> Delete').attr({'class': 'file_delete_from_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            let id = item.id;
            let from_id = -1;
            let obj = -1;
            obj = getObjectFromCanvasById(id, window['map']);
            if (obj) {
              removeObjectFromCanvasById(id, window['map']);
              if (objs = getObjectsFromCanvasByFromId(obj.from_id, window['map'])) {
                removeObjectsFromCanvas(objs, window['map']);
              }
            }
            $('#files').jsGrid('deleteItem', $(item));
          });
        },
        align: 'center',
        width: 120
      }
    ]
  });

  function add_clone(val, item) {
    let obj, clone;
    if (obj = getObjectFromCanvasById(item.id, window['map'])) {
      clone = fabric.util.object.clone(obj);
      clone.id = make_file_id(item.id);
      clone.from_id = obj.id;
      // Keep the original from_id if one exists.
      if (obj.from_id) {
        clone.from_id = obj.from_id;
      }
      if (!$.isEmptyObject(clone)) {
        window['map'].add(clone);
      }
    }
    else {
      let data;
      if (item.url) {
        data = item.url
      }
      if (item.Blob) {
        data = item.Blob;
      }
      loadFile(data, getExtension(item.Filename), item.id);
    }
  }

  /**
   * Layering grid.
   */
  $('#layering').jsGrid({
    height: '400px',
    width: '100%',

    editing: false,
    pageSize: 999,
    confirmDeleting: false,

    onItemDeleted: function(e) {
      removeObjectFromCanvasById(e.item.id, window['map']);
    },

    fields: [
      { name: 'Thumbnail',
        itemTemplate: function(val, item) {
          return $('<img>').attr('src', item.Thumbnail).css({ height: 50, width: 50 });
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
      { name: 'id', type: 'number', visible: false },
      { name: 'Filename', type: 'text', width: '25%' },
      { name: 'Add',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-puzzle-piece" aria-hidden="true"></i> Add').attr({'class': 'file_add_to_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            let obj;
            if (obj = getObjectFromCanvasById(item.id, window['map'])) {
              add_clone(val, item);
            }
          });
        },
        align: 'center',
        width: 120
      },
      { name: 'Options',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fas fa-wrench"></i> Options').attr({'class': 'layer_options'}).css({ 'display': 'block' }).on('click', function(e) {
            let obj = getObjectFromCanvasById(item.id, window['map']);
            window['map'].setActiveObject(obj);
            open_layer_options(obj);
          });
        },
        align: 'center',
        width: 120
      },
      { name: 'Delete',
        itemTemplate: function(val, item) {
          return $('<button>').html('<i class="fa fa-trash" aria-hidden="true"></i> Delete').attr({'class': 'file_delete_from_canvas'}).css({ 'display': 'block' }).on('click', function(e) {
            $('#layering').jsGrid('deleteItem', item);
          });
        },
        align: 'center',
        width: 120
      },
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
            // console.log(clientIndexRegExp.exec(classes));
            return clientIndexRegExp.exec(classes)[1];
          });

          // arrays of items
          var items = $.map($gridData.find("tr"), function(row) {
            return $(row).data("JSGridItem");
          });
          items.forEach(function(e) {
            window['map'].moveTo(getObjectFromCanvasById(e.id, window['map']), e.row);
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
   * Helper for setting angle snap.
   */
  fabric.Object.prototype.set({
    snapThreshold: 15,
    snapAngle: 45
  });

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
   * Helper function for searching an array.
   * https://stackoverflow.com/a/7178381/4942292
   */
  function find_attr(array, attr, value) {
    for(var i = 0; i < array.length; i += 1) {
      if(array[i][attr] === value) {
        return i;
      }
    }
    return -1;
  }

  /**
   * Helper fires when object is deleted.
   */
  window['map'].on('object:removed', function (e) {
    // We are only worried about removing layers.
    let data = $('#layering').jsGrid('option', 'data');
    let row = -1;
    if (e.target && e.target.id) {
      // row = data[find_attr(data, 'id', e.target.id)];
      row = get_row(e.target.id, data);
      if (row) {
        $('#layering').jsGrid('deleteItem', row);
      }
    }

    clean_videos();
  });

  function clean_videos() {
    let videos = $('video.map_video').toArray(),
    data = $('#files').jsGrid('option', 'data'),
    row = null;

    videos.forEach(function(video) {
      if (!getObjectFromCanvasById(video.id, window['map'])) {
        if ((getObjectsFromCanvasByFromId(video.id, window['map'])).length > 0) {
          return false;
        }
        row = get_row(video.id);
        if (row) {
          $('#files').jsGrid('deleteItem', row);
        }
        $('#' + video.id).remove();
      }
    });

  }

  /**
   * Selection function opens element dialog and should set controls to match properties of object.
   */
  window['map'].on('selection:created', function (e) {
    if (e.shiftKey) {
      if (!$('#map_element_options').dialog('isOpen')) {
        $('#map_element_options').dialog('open');
      }
      let obj = window['map'].getActiveObject();
      $('#map_element_opacity').val(obj.opacity);
    }
  });

  /**
   * ELement Opacity.
   */
  $('#map_element_opacity').on('input', function(e) {
    let obj = window['map'].getActiveObject();
    obj.set({
      opacity: this.value
    });
  })

  /**
   * Grab video tag by id and make a screen shot.
   */
  function make_video_thumbnail(vid) {
    let video = document.getElementById(vid);

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
      window[vid] = _shot;
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

  $('#library').dialog({
    autoOpen: false,
    modal: true,
    width: 80+'%',
    height: screen.availHeight * 0.8,
    position: {
      my: "center",
      at: "top",
      of: $('#map_wrapper')
    }
  });

  $('#map_catalog').click(function() {
    open_map_catalog();
  });

  function open_map_catalog() {
    let library = $('#library');
    if (library.dialog('isOpen')) {
      library.dialog('close');
    }
    let map_catalog_button = $('#map_catalog');
    if (map_catalog_button.prop('disabled') == true) {
      console.log('Already opening map catalogue.');
      return;
    }
    // map_catalog_button.append('<div class="ajax-progress"><div class="throbber">&nbsp;</div></div>');
    map_catalog_button.prop('disabled', true);
    $.ajax({
      url: '/views/ajax',
      type: 'post',
      data: {
        view_name: 'library',
        view_display_id: 'editor_library',
        view_args: '',
      },
      dataType: 'json',
      success: function(response) {
        if (response[1] !== undefined) {
          // we have data!
          let data = response[1].data;
          library.html(data);
          library.dialog('open');

          $('.library.resources .resource-link-type').each(function(e) {
            $(this).remove();
          });
          $('.library.resources .resource-link-link a').each(function(e) {
            let url = this.href;
            if (!get_youtube_code(url)) {
              // $(this).parent('.resource').remove();
              this.disabled = true;
              $(this).remove();
            }
          })
          $('.library.resources .resource-link-link a').on('click', function(e) {
            e.preventDefault();
            let url = this.href;
            library.dialog('close');
            if (!get_youtube_code(url)) {
              console.log('Could not find a YouTube code in ' + url);
              return;
            }
            $('#questions').html('');
            $('#questions').dialog({
              title: 'Load from catalog',
              resizable: false,
              height: 'auto',
              width: 400,
              modal: true,
              buttons: {
                'Load this item': function() {
                  $(this).dialog('close');
                  $('library.resources .resource-link-link a').prop('disabled', true);
                  if (do_youtube(get_youtube_code(url))) {
                    $('library.resources .resource-link-link a').prop('disabled', false);
                  }
                },
                Cancel: function() {
                  $(this).dialog('close');
                }
              }
            });
          });
        }
      },
      error: function(response) {
        console.log('There was an error retrieving maps.');
      },
      complete: function(response) {
        // $('#map_catalog .ajax-progress').remove();
        map_catalog_button.prop('disabled', false);
        $('library.resources .resource-link-link a').prop('disabled', false);
      }
    });
  }

  $('#text_create_button').click(function() {
    window['map'].off('mouse:down');
    $('.temp_text').remove();
    let text = new fabric.Text($('#text_create_text').val());
    let textSize = ($('#text_size').val()) ? $('#text_size').val() : 15;
    let ttag = $('<div />', {
      class: 'temp_text',
    }).html(text.text).css({
      color: $('#text_colour').val(),
      fontSize: textSize + 'px'
    });

    $('body').append(ttag[0]);
    window['map'].on('mouse:down', function(e) {
      placeText(text, {'x': e.e.clientX, 'y': e.e.clientY});
    });
    $(document).on('mousemove', function(e) {
      ttag.css({
        color: $('#text_colour').val(),
        fontSize: textSize + 'px',
        left:  e.pageX,
        top:   e.pageY
      });
    });
  });

  function placeText(text, coords) {
    text.left = coords.x;
    text.top = coords.y;
    text.id = make_file_id(text.text);
    text.fontSize = $('#text_size').val();
    text.setColor($('#text_colour').val());
    window['map'].add(text);
    window['map'].off('mouse:down');
    $('.temp_text').remove();
  }

  /**
   * Helper opens the player window and sets the reference to local storage.
   */
  function player_view_open() {
    player_view = window.player_view = window['player_view'] = window.open('/engine/players', 'player_window', 'toolbar=0, location=0, menubar=0');
    player_view_initialize();
    player_view.onbeforeunload = function() {
      player_view = undefined;
    };
  }

  /**
   * Sets event handler for onload and assigns player canvases to variables in local window.
   */
  function player_view_initialize() {
    if (!player_view) {
      setTimeout(player_view_initialize, 1000);
      return;
    }
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
      canvases.forEach(function(e) {
        let _wrapper = 'player_' + e + '_wrapper';
        let _e = 'player_' + e;
        window[_wrapper] = $player_view_content.find('#' + _wrapper);
        window[_e] = $player_view_content.find('#' + _e).get(0);
        if (window[_e]) {
          $(window[_e]).css('z-index', eval(e.toUpperCase() + '_CANVAS_ZINDEX'));
        }
      });
      set_canvas_dimensions();
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
