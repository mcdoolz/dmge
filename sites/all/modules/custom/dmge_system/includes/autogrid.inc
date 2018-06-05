<h1>Screen Size Calculator</h1>
	<p>Trying to figure out the required size for your 16:9 HDTV projection screen?<br>
		This tool calculates 2 of 3 dimensions for you in cm and inches.<br>
    Simply enter any known screen dimension and the others will be calculated.</p>

	<div id="autogrid_form">
		<form>
			<div class="autogrid_subform">
				<fieldset>
					<legend>1. Choose Aspect Ratio</legend>

					<div class="table">
						<div class="table-row">
							<div class="table-col"><label for="width">Aspect ratio</label></div>
							<div class="table-col"><input type="number" id="aspectRatioWidth" name="aspectRatioWidth" class="inputValue" value="16"/> : <input type="number" id="aspectRatioHeight" name="aspectRatioHeight" class="inputValue" value="9"/></div>
						</div>
						<div class="table-row">
							<div class="table-col"></div>
							<div class="table-col">
								Select predefined format:<br/>
								<ul class="formValueList">
									<li><a id="setAspectRatio_3840_2160">4K Ultra HD (3840&times;2160) <span class="arbox ar177">16:9</span></a></li>
									<li><a id="setAspectRatio_1920_1080">Full HD (1920&times;1080) <span class="arbox ar177">16:9</span></a></li>
									<li><a id="setAspectRatio_1280_720">HD-Ready (1280&times;720) <span class="arbox ar177">16:9</span></a></li>
									<li><a id="setAspectRatio_1920_1200">WUXGA (1920&times;1200) <span class="arbox ar239">16:10</span></a></li>

									<li><a id="setAspectRatio_4_3">NTSC/PAL <span class="arbox ar133">4:3</span></a></li>
									<li><a id="setAspectRatio_239_100">Anamorphic <span class="arbox ar239">2.39:1</span></a></li>
								</ul>
							</div>
						</div>

					</div>
				</fieldset>
			</div>

			<div class="subform">
				<fieldset>
					<legend>2. Calculate</legend>
					<p>Enter the measurement that you know (diagonal, width or height) and the other two will be calculated.</p>

					<div class="table">
						<div class="table-row">
							<div class="table-col"><label for="diagonalCm">Diagonal</label></div>
							<div class="table-col">
								<input type="number" step="0.1" id="diagonalCm" name="diagonalCm" data-key="d" data-unit="cm" class="inputValue"/> cm
								/ <input type="number" step="0.1" id="diagonalIn" name="diagonalIn" data-key="d" data-unit="in" class="inputValue"/> inches
							</div>
							<div class="table-col">
								<button id="calculate_d" type="button">Calculate</button>
							</div>
						</div>
						<div class="table-row">
							<div class="table-col"><label for="widthCm">Width</label></div>
							<div class="table-col">
								<input type="number" step="0.1" id="widthCm" name="widthCm" data-key="w" data-unit="cm" class="inputValue"/> cm
								/ <input type="number" step="0.1" id="widthIn" name="widthIn" data-key="w" data-unit="in" class="inputValue"/> inches
							</div>
							<div class="table-col">
								<button id="calculate_w" type="button">Calculate</button>
							</div>
						</div>
						<div class="table-row">
							<div class="table-col"><label for="heightCm">Height</label></div>
							<div class="table-col">
								<input type="number" step="0.1" id="heightCm" name="heightCm" data-key="h" data-unit="cm" class="inputValue"/> cm
								/ <input type="number" step="0.1" id="heightIn" name="heightIn" data-key="h" data-unit="in" class="inputValue"/> inches
							</div>
							<div class="table-col">
								<button id="calculate_h" type="button">Calculate</button>
							</div>
						</div>
					</div>
				</fieldset>
			</div>
		</form>
	</div>
