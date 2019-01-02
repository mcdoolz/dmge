<?php
/**
 * Sidebar inclusion.
 */
$incept_date = new \DateTime('2018-02-13');
$today = new \DateTime;
$age = $incept_date->diff($today)->days;
?>
<div id="dmge">
  <div id="questions" class="dialog" title=""></div>
  <div id="library" class="dialog" title="Map Library"></div>

  <div id="about" class="dialog" title="How to use the DMGE">
    <?php print variable_get('dmge_sidebar_about', '
    <h4>How to Use</h4>
    <ol>
      <li>Click the File tab, then Choose File and select an image or video to use as a map.</li>
      <li>You can import any online MP4 video by inputting the address in the provided field, then clicking Import.</li>
      <li>Click and drag the corner of the map to scale it to your liking.  Zoom out using your browsers control if you need more space.  You will need to increase the canvas size in the map settings.</li>
      <li>If the Fog of War is enabled, left click drag to remove the fog, hold shift to repaint it.  Use the slider to change the opacity.</li>
    </ol>
    <h4>Tips &amp; Tricks</h4>
    <ol>
      <!-- <li>Press Ctrl-X to mark the grid before scrolling it, so you can track where your miniatures were.  Shift-X clears marking mode.</li> -->
      <!-- <li>Press Backspace to clear marking or dragging mode.</li> -->
      <!-- <li>Press Ctrl-Backspace to clear all marks.</li> -->
      <!-- <li>Press Alt to pan around.</li> -->
      <li>Press Escape to toggle the sidebar.</li>
      <li>Use the numeric keys to quickly switch between tool sets.</li>
      <li>Press Ctrl-P to open the player view window.</li>
      <li>Press Ctrl-Alt-C to open the player view window.</li>
      <li>Hold Ctrl when clicking on a map element to open its layer options.</li>
      <li>Press Delete or Backspace to remove selected elements from the map area.</li>
      <li>Zoom out and in by holding Ctrl and rolling your scroll wheel.  You can scale your map larger then the available space this way.</li>
      <li>If you lose the ability to interact with the board in some way, click the wrench and your mouse should start reacting.</li>
    </ol>
    <p>Crafted with <i class="fa fa-heart"></i> by Dooley P.<br>
    This is Alphaware (<?php print $age;?> days old).<br>
    Report problems to <a href="mailto:contact@dmge.net">contact@dmge.net</a><br>
    Code was lifted from various sources, or written personally.</p>
  </div>
  '); ?>

  <div id="file_preview_dialog" title="Preview">
    <img id="file_preview">
    <button id="file_dialog_add_to_canvas" class="file_add_to_canvas" type="button">
      <i class="fa fa-puzzle-piece" aria-hidden="true"></i> Add
    </button>
  </div>

  <div id="map_element_options" title="Map Element Options">
    <label for="map_element_opacity">Opacity</label>
    <input id="map_element_opacity" type="range" value="1" step="0.1" min="0.1" max="1">
  </div>

  <div id="wrench"><div id="wrench_innerwrapper"><i class="fa fa-wrench"></i></div></div>

  <div id="sidebar">
    <div id="sidebar_innerwrapper">

      <div id="menu">
        <ul>
          <li><a href="#map_settings" title="Map"><i class="fas fa-map"></i></a></li>
          <li><a href="#files_settings" title="Files"><i class="fas fa-file"></i></a></li>
          <li><a href="#layers" title="Layers"><i class="fas fa-layer-group"></i></a></li>
          <li><a href="#fow_settings" title="Fog of War"><i class="fas fa-eye"></i></a></li>
          <li><a href="#grid_settings" title="Grids"><i class="fas fa-th"></i></a></li>
          <li><a href="#paint_box" title="Painters Box"><i class="fas fa-paint-brush"></i></a></li>
          <li><a href="#rulers" title="Rulers &amp; Templates"><i class="fas fa-pencil-ruler"></i></a></li>
        </ul>
      </div>

      <div id="sidebar_sections">

        <div id="files_settings" class="sidebar_section">
          <h2>Files</h2>
          <div id="file_status"></div>
          <form id="file_local_load">
            <input id="file" type="file" multiple />
          </form>
          <form id="file_remote_load">
            <label for="map_embed">Paste the URL to any publically available file.</label>
            <input type="text" id="map_embed"><input type="button" id="map_embed_submit" value="Import">
            <p><sup>Recognizes YouTube URLs, mp4, mkv, png and jpg.</sup></p>
          </form>
          <div id="files_storage_path">
            <div id="files_storage_path"><label for="files_storage_path_configure">Load campaign folder</label>
            <input id="files_storage_path_configure" type="file" webkitdirectory mozdirectory msdirectory odirectory directory multiple />
          </div>
            <div id="files_storage_path_description">Select the folder in which you wish to save your campaign.</div>
          </div>

          <div id="files_wrapper">
            <div id="files"></div>
          </div>

        </div>

        <div id="map_settings" class="sidebar_section">
          <div class="map_setting">
            <h3>Map Settings</h3>
            <label for="map_name">Map Name</label>
            <input type="text" id="map_name" placeholder="Untitled" class="ui-corner-all">
          </div>
          <div class="map_setting">
            <h4>Map Size</h4>
            <p>Defaults to your screen resolution.<br />Press F11 for Full Screen.</p>
            <label for="map_width">Width</label>
            <input type="number" id="map_width" class="ui-corner-all">
            <label for="map_height">Height</label>
            <input type="number" id="map_height" class="ui-corner-all">
            <br />
            <button id="map_dim_submit" class="ui-button ui-widget ui-corner-all">Set Map Dimensions</button>
          </div>

          <div class="map_setting">
            <h4>Map Controls</h4>
            <label for="map_scroll_synch">Map Scroll Synchronize</label>
            <input type="checkbox" id="map_scroll_synch" name="map_scroll_synch">
          </div>

          <div class="map_setting">
            <label for="map_master_volume">Map Master Volume</label>
            <input type="range" min="0" max="1" step="0.01" value="0" id="map_master_volume" name="map_master_volume">
          </div>

        </div>

        <div id="layers" class="sidebar_section">
          <h3>Layers</h3>
          <p>Click and drag the entries to sort them on the map.</p>
          <div id="layering"></div>
        </div>

        <div id="fow_settings" class="sidebar_section">
          <h3>Fog of War Settings</h3>
          <div class="fow_control">
            <label for="fow_toggle">Fog of War</label>
            <input type="checkbox" id="fow_toggle">
          </div>
          <div id="fow_storage" class="fow_control">
            <label for="fow_storage">FoW Storage / Recall</label>
            <input type="button" id="fow_store" value="Store FoW (Ctrl-Shift-F)">
            <input type="button" id="fow_recall" value="Recall FoW (Ctrl-Alt-F)">
            <input type="button" id="fow_reset" value="Reset FoW">
          </div>
          <div class="fow_brush_property">
            <label for="fow_opacity">FoW Opacity %</label>
            <input type="number" id="fow_opacity" value="100">
          </div>
          <div class="fow_brush_property">
            <label for="fow_brush_toggle">FoW Brush</label>
            <input type="checkbox" id="fow_brush_toggle">
          </div>
          <div class="fow_brush_property">
            <label for="fow_brush_size">FoW Brush Size</label>
            <input type="number" id="fow_brush_size" value="100">
          </div>
          <div class="fow_brush_property">
            <label for="fow_brush_feather_size">FoW Feather</label>
            <input type="number" id="fow_brush_feather_size" value="300">
          </div>
        </div>

        <div id="paint_box" class="sidebar_section">
          <h3>Painters Box</h3>
          <label for="painting_toggle">Painting Mode</label>
          <input class="painting_control" type="checkbox" id="painting_toggle">
          <div id="painting_brush_settings">
            <input class="painting_control" type="range" id="painting_brushsize">
            <input class="painting_control" type="color" id="painting_colour">
          </div>
          <select class="painting_control" id="painting_toolselect">
            <option value="select">Select</option>
            <option value="brush">Brush</option>
            <option value="circle">Circle</option>
            <option value="square">Square</option>
            <option value="line">Line</option>
          </select>
        </div>

        <div id="rulers" class="sidebar_section">
          <h3>Rulers &amp; Templates</h3>
          <label for="template_types">Template Type</label>
          <div id="template_types">
            <button class="template_type" id="template_circle"><i class="fas fa-circle"></i> Radius</button>
            <button class="template_type" id="template_square"><i class="fas fa-square-full"></i> Cube</button>
            <button class="template_type" id="template_triangle"><i class="css-triangle"></i> Cone</button>
          </div>
          <div class="template_controls" id="template_controls">
            <label for="template_colour">Template Size</label>
            <input type="color" id="template_colour">
          </div>
          <h3>Labels</h3>
          <div class="text_controls" id="text_controls">
            <label for="text_create">Text Labeler</label>
            <input type="integer" id="text_size" placeholder="Size" size="4">
            <input type="color" id="text_colour">
            <input type="text" id="text_create_text" />
            <input type="button" id="text_create_button" value="Create Text Label" />
          </div>
        </div>

        <div id="grid_settings" class="sidebar_section">
          <h3>Grid Settings</h3>

          <div id="grid_snap_wrapper" class="grid_setting">
            <label for="grid_snap">Grid Snap</label>
            <input type="checkbox" id="grid_snap" class="grid_snap_checkbox">
          </div>

          <div id="map_grid_type" class="grid_setting">

            <div class="map_grid_type">
              <label for="quad_grid">Quad</label>
              <input type="radio" id="quad_grid" name="map_grid_type" value="Quad">
            </div>
            <div class="map_grid_type">
              <label for="h_hex_grid">Horizontal Hex</label>
              <input type="radio" id="h_hex_grid" name="map_grid_type" value="H_Hex">
            </div>
            <div class="map_grid_type">
              <label for="v_hex_grid">Vertical Hex</label>
              <input type="radio" id="v_hex_grid" name="map_grid_type" value="V_Hex">
            </div>
            <div class="map_grid_type">
              <label for="no_grid">None</label>
              <input type="radio" id="no_grid" name="map_grid_type" value="None" checked="checked">
            </div>
          </div>

          <div id="map_grid_properties" class="grid_setting">
            <div class="map_grid_property">
              <label for="map_grid_size">Grid Size</label>
              <input type="number" id="map_grid_size" value="60">
              <p><sup>Lower sizes take longer to process.</sup></p>
            </div>
            <div class="map_grid_property">
              <label for="map_grid_display_size">Grid Size at Scale</label>
              <input type="number" disabled id="map_grid_display_size" value="">
              <p><sup>This is the grid in pixels with your zoom level considered.</sup></p>
            </div>
          </div>

          <div class="map_grid_property">
            <label for="map_grid_auto">Grid 1 inch auto calculator</label>
            <div id="map_grid_auto">
              <div class="grid_setting">
                <label for="screen_x">Width Pixels</label>
                <input placeholder="1920" type="text" id="screen_x"/><br />
                <label for="screen_y">Height Pixels</label>
                <input placeholder="1080" type="text" id="screen_y"><br />
              </div>
              <div class="grid_setting">
                <label for="screen_inch">Screen Inches</label>
                <input type="text" placeholder="40" id="screen_inch"><br />
                <label for="screen_result" title="Pixels Per Inch">PPI</label>
                <input type="text" disabled id="screen_result" value=""><br />
              </div>
              <button id="screen_calculate">Calculate</button>
            </div>
          </div>

          <div class="map_grid_property">
            <label for="map_grid_opacity">Grid Opacity</label>
            <input type="number" id="map_grid_opacity" value="50">
          </div>

        </div>

      </div>

      <div id="sidebar_tray">
        <button id="save_map"class="sidebar_tray_button" title="Save"><i class="fas fa-save"></i></button>
        <button id="fullscreener" class="sidebar_tray_button" title="Fullscreen"><i class="fas fa-desktop"></i></button>
        <button id="map_reset_zoom" class="sidebar_tray_button" title="Reset Zoom"><i class="fas fa-bullseye"></i></button>
        <button id="map_clear" class="sidebar_tray_button" title="Clear Map"><i class="fas fa-eraser"></i></button>
        <button id="player_view_open" class="sidebar_tray_button" title="Player View (Ctrl-P)"><i class="fas fa-chess-knight"></i></button>
        <button id="map_catalog" class="sidebar_tray_button" title="Map Catalogue (Ctrl-Alt-C)"><i class="fas fa-database"></i></button>
      </div>

    </div>

  </div>

  <div id="canvases_wrapper">
    <div id="tokens_wrapper">
      <canvas id="tokens" class="tokens_canvas"></canvas>
    </div>
    <div id="particles_wrapper">
      <canvas id="particles" class="particles_canvas"></canvas>
    </div>
    <div id="grid_wrapper">
      <canvas id="grid" class="grid_canvas"></canvas>
    </div>
    <div id="fow_wrapper">
      <canvas id="fow" class="fow_canvas"></canvas>
    </div>
    <div id="map_wrapper">
      <canvas id="map" class="map_canvas"></canvas>
      <div id="map_video_wrapper">
      </div>
    </div>
  </div>

</div>
