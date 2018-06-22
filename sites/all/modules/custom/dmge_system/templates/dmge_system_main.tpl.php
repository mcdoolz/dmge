<?php
/**
 * Sidebar inclusion.
 */
$incept_date = new \DateTime('2018-02-13');
$today = new \DateTime;
$age = $incept_date->diff($today)->days;
?>
<div id="dmge">
<div id="about" class="dialog" title="How to use the DMGE">
  <h4>How to Use</h4>
  <ol>
    <li>Click Choose File and select an image or video to use as a map, or select Online Video and paste the URL of a YouTube video.</li>
    <li>Click and drag the corner of the map to scale it to your liking.  Zoom out using your browsers control if you need more space.</li>
    <li>If the Fog of War is enabled, left click drag to remove the fog, hold shift to repaint it.  Use the slider to change the opacity.</li>
  </ol>
  <h4>Tips &amp; Tricks</h4>
  <ol>
    <li>Press Ctrl-Z to reposition the grid.  Shift-Z clears dragging mode.</li>
    <li>Press Ctrl-X to mark the grid before scrolling it, so you can track where your miniatures were.  Shift-X clears marking mode.</li>
    <li>Press Backspace to clear marking or dragging mode.</li>
    <li>Press Ctrl-Backspace to clear all marks.</li>
    <li>Zoom out and in by holding Ctrl and rolling your scroll wheel.  You can scale your map larger then the available space this way.</li>
    <li>If you lose the ability to interact with the board in some way, click the wrench and your mouse should start reacting.</li>
  </ol>
  <p>Crafted with <i class="fa fa-heart"></i> by Dooley P.<br>
  This is Alphaware (<?php print $age;?> days old).<br>
  Report problems to <a href="mailto:icanfly@digitalforge.ca">icanfly@digitalforge.ca</a><br>
  Code was lifted from various sources, or written personally.</p>
</div>
<div id="file_preview_dialog" title="Preview">
  <img id="file_preview" />
  <button id="file_dialog_add_to_canvas" class="file_add_to_canvas" type="button">
    <i class="fa fa-puzzle-piece" aria-hidden="true"></i> Add
  </button>
</div>

  <div id="wrench"><div id="wrench_innerwrapper"><i class="fa fa-wrench"></i></div></div>
  <div id="sidebar">
    <div id="sidebar_innerwrapper">
      <div id="menu">
        <ul>
          <li><a href="#map_settings" title="Map"><i class="fas fa-map"></i></a></li>
          <li><a href="#files_settings" title="Files"><i class="fas fa-file"></i></a></li>
          <li><a href="#fow_settings" title="Fog of War"><i class="fas fa-eye"></i></a></li>
          <li><a href="#grid_settings" title="Grid"><i class="fas fa-th"></i></a></li>
          <li><a href="#paint_box" title="Painters Box"><i class="fas fa-paint-brush"></i></a></li>
          <li><a href="#rulers" title="Rulers &amp; Templates"><i class="fas fa-pencil-ruler"></i></a></li>
        </ul>
      </div>

      <div id="sidebar_sections">

        <div id="files_settings" class="sidebar_section">
          <h2>Files</h2>
          <div id="files_wrapper">
            <h4>Files</h4>
            <div id="files"></div>
            <form method="post">
              <input id="file" type="file" multiple />
              <input id="file_load" type="button" value="Load File" />
              <select id="file_functions">
                <options>
                  <option>--Bulk Ops--</option>
                  <option>Remove</option>
                </options>
              </select>
              <input id="file_bulkop" type="button" value="Do Bulk Op" />
            </form>
            <label for="map_embed">Paste the URL to a Video</label>
            <form><input type="text" id="map_embed"><input type="submit" id="map_embed_submit"></form>
            <p><sup>Currently recognizes YouTube videos only.</sup></p>
          </div>
          <div id="files_storage_path">
            <div id="files_storage_path"><label for="files_storage_path_configure">Select Resource Folder</label>
            <input id="files_storage_path_configure" type="file" webkitdirectory directory multiple/></div>
            <div id="files_storage_path_description">Select the folder in which you wish to save your compiled maps.</div>
          </div>
        </div>

        <div id="map_settings" class="sidebar_section">
          <div class="map_setting">
            <h3>Map Settings</h3>
            <label for="map_name">Map Name</label>
            <input type="text" id="map_name" placeholder="Untitled" />
          </div>
          <div class="map_setting">
            <h4>Map Size</h4>
            <p>Defaults to your screen resolution.<br />Press F11 for Full Screen.</p>
            <label for="map_width">Width</label>
            <input type="number" id="map_width" />
            <label for="map_height">Height</label>
            <input type="number" id="map_height" />
          </div>
        </div>

        <div id="fow_settings" class="sidebar_section">
          <h3>Fog of War Settings</h3>
          <div class="fow_brush_property">
            <label for="fow_toggle">Fog of War</label>
            <input type="checkbox" id="fow_toggle">
          </div>
          <div class="fow_brush_property">
            <label for="fow_opacity">FoW Opacity %</label>
            <input type="number" id="fow_opacity" value="100">
          </div>
          <div class="fow_brush_property">
            <label for="fow_brush_size">FoW Brush</label>
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
          <input class="painting_control" type="checkbox" id="painting_toggle" />
          <div id="painting_brush_settings">
            <input class="painting_control" type="range" id="painting_brushsize" />
            <input class="painting_control" type="color" id="painting_colour" />
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
            <input type="color" id="template_colour" />
          </div>
        </div>

        <div id="grid_settings" class="sidebar_section">
          <h3>Grid Settings</h3>

          <div id="map_grid_type">
            <h3>Grid Type</h3>

            <div class="map_grid_type">
              <label for="quad_grid">Quad</label>
              <input type="radio" id="quad_grid" name="map_grid_type" value="Quad" />
            </div>
            <div class="map_grid_type">
              <label for="h_hex_grid">Horizontal Hex</label>
              <input type="radio" id="h_hex_grid" name="map_grid_type" value="H_Hex" />
            </div>
            <div class="map_grid_type">
              <label for="v_hex_grid">Vertical Hex</label>
              <input type="radio" id="v_hex_grid" name="map_grid_type" value="V_Hex" />
            </div>
            <div class="map_grid_type">
              <label for="no_grid">None</label>
              <input type="radio" id="no_grid" name="map_grid_type" value="None" checked="checked">
            </div>
          </div>
          <div class="map_grid_property">
            <label for="map_grid_size">Grid Size</label>
            <input type="number" id="map_grid_size" value="20" />
            <p><sup>Lower sizes take longer to process.</sup></p>
            <div class="map_grid_property">
              <label for="map_grid_display_size">Grid Size at Scale</label>
              <input type="number" disabled id="map_grid_display_size" value="" />
              <p><sup>This is the grid in pixels with your zoom level considered (Reset Zoom with Ctrl-Z or Ctrl-R).</sup></p>
            </div>
          </div>
          <div class="map_grid_property">
            <div id="map_grid_auto">
              <label for="screen_x">Width Pixels</label>
              <input placeholder="1920" type="text" id="screen_x"/><br />
              <label for="screen_y">Height Pixels</label>
              <input placeholder="1080" type="text" id="screen_y" /><br />
              <label for="screen_inch">Screen Inches</label>
              <input type="text" placeholder="40" id="screen_inch" /><br />
              <label for="screen_result" title="Pixels Per Inch">PPI</label>
              <input type="text" disabled id="screen_result" value="" /><br />
              <button id="screen_calculate">Calculate</button>
            </div>
          </div>

          <div class="map_grid_property">
            <label for="map_grid_opacity">Grid Opacity</label>
            <input type="number" id="map_grid_opacity" value="50" />
          </div>

        </div>

      </div>

        <div id="sidebar_tray">
          <div class="sidebar_tray_category" id="map_tray">
            <div class="sidebar_tray_section" id="map_tray_zoom">
              <button id="save_map"class="sidebar_tray_button"><i class="fas fa-save"></i> Save</button>
              <button id="fullscreener" class="sidebar_tray_button"><i class="fas fa-desktop"></i> Fullscreen</button>
              <button id="map_reset_zoom" class="sidebar_tray_button"><i class="fas fa-bullseye"></i> Reset Zoom</button>
              <button id="map_clear" class="sidebar_tray_button"><i class="fas fa-eraser"></i> Clear Map</button>
              <button id="player_view_open" class="sidebar_tray_button"><i class="fas fa-chess-knight"></i> Open Player View</button>
            </div>
          </div>
        </div>
    </div>
  </div>
  <div id="grid_wrapper">
    <canvas width="1280" height="720" id="grid" class="grid_canvas"></canvas>
  </div>
  <div id="fow_wrapper">
    <canvas width="1280" height="720" id="fow" class="fow_canvas"></canvas>
  </div>
  <div id="map_wrapper">
    <canvas width="1280" height="720" id="map" class="map_canvas"></canvas>
    <div id="map_video_wrapper">
    </div>
  </div>



</div>
