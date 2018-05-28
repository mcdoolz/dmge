
<div id="dmge">
  <div id="grid_wrapper"></div>
  <div id="wrench"><div id="wrench_innerwrapper"><i class="fa fa-wrench"></i></div></div>
  <div id="sidebar">
    <?php
    /**
     * Sidebar inclusion.
     */
    $incept_date = new \DateTime('2018-02-13');
    $today = new \DateTime;
    $age = $incept_date->diff($today)->days;
    ?>
    <div id="about" class="dialog" title="How to use the DMGE">
      <p>Crafted with <i class="fa fa-heart"></i> by Dooley P.<br>
        This is Alphaware (<?php print $age;?> days old).<br>
        Report problems to <a href="mailto:icanfly@digitalforge.ca">icanfly@digitalforge.ca</a><br>
        Code was lifted from various sources, or written personally.</p>
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
      </div>
    <div id="sidebar_innerwrapper">
      <div id="interface">
        <h2>Configuration</h2>
        <div id="map_settings">
          <div class="map_property">
            <div class="map_grid_type">
              <label for="map_embed_option">Online Video</label>
              <input type="radio" id="map_embed_option" name="map_type_options" value="embed" />
            </div>
            <div class="map_grid_type">
              <label for="map_file_option">Local File</label>
              <input type="radio" id="map_file_option" name="map_type_options" value="file" checked="checked">
            </div>
          </div>
          <div id="map_embed_wrapper" class="map_type_property">
            <label for="map_embed">Paste the URL to a Video</label>
            <form><input type="text" id="map_embed"><input type="submit" id="map_embed_submit"></form>
            <p><sup>Currently recognizes YouTube videos only.  Back the Kickstarter to make the full tool happen!</sup></p>
          </div>
          <div id="map_filename_wrapper" class="map_type_property">
            <label for="map_filename">Select a file</label>
            <input type="file" id="map_filename" title="Select a video, or image from your computer">
            <p><sup>Browsers typically top out at 200 megabytes for videos.<br>
              jpg, gif, png, bmp, pdf, m4v, mp4, mpg :: Email for more formats.</sup></p>
          </div>
        </div>
        <div id="fow_settings">
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
        <div id="grid_settings">
          <h3>Grid Settings</h3>

          <div id="map_grid_type">
            <!-- <h4>Auto Grid</h4> -->
            <?php // require_once('./conversion.php'); ?>
            <h4>Grid Type</h4>

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
          </div>

          <div class="map_grid_property">
            <label for="map_grid_opacity">Grid Opacity</label>
            <input type="number" id="map_grid_opacity" value="50" />
          </div>

        </div>
      </div>
    </div>
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
