
/**
 * TV size calculator stolen from http://screen-size.info
 */

var UNITS = ["cm", "in"];
var currentKey = null;

function findInput(key, unit) {
	return $("*[data-key="+key+"]").filter("*[data-unit="+unit+"]");
}

function setAspectRatio(width, height) {
	$("#aspectRatioWidth").val(width);
	$("#aspectRatioHeight").val(height);

	calculate(currentKey);
}

function cmToInch(value) {
	return value / 2.54;
}

function inchToCm(value) {
	return value * 2.54;
}

function calculateDiagonal(width, height) {
	return Math.sqrt(Math.pow(width, 2) + Math.pow(height, 2));
}

function transformUnits(key, unit, value) {
	var element, transfomredValue;
	if (unit === "cm") {
		element = findInput(key, "in");
		transfomredValue = cmToInch(value);
	} else if (unit === "in") {
		element = findInput(key, "cm");
		transfomredValue = inchToCm(value);
	}
	element.data("exactValue", transfomredValue);
	element.val(transfomredValue.toFixed(1));
}

function calculate(key) {
	switch (key) {
		case "d":
			calculateByDiagonal();
			break;
		case "w":
			calculateByWidth();
			break;
		case "h":
			calculateByHeight();
			break;
	}
  _resolution_width = $("#aspectRatioWidth").val();
  _screen_width = $('#widthIn').val();
  _size = _resolution_width / _screen_width;
  set_grid(_size);
}

function calculateByDiagonal() {
	var aspectRatioWidth = $("#aspectRatioWidth").val();
	var aspectRatioHeight = $("#aspectRatioHeight").val();
	var aspectRatioDiagonal = calculateDiagonal(aspectRatioWidth, aspectRatioHeight);

	for (var i in UNITS) {
		var unit = UNITS[i];
		var inputElement = findInput("d", unit);
		var diagonal = inputElement.data("exactValue") || inputElement.val();

		if (diagonal > 0) {
			var factor = diagonal / aspectRatioDiagonal;
			var width = aspectRatioWidth * factor;
			var height = aspectRatioHeight * factor;

			findInput("w", unit).val(width.toFixed(1)).data("exactValue", width);
			findInput("h", unit).val(height.toFixed(1)).data("exactValue", height);

		}
	}
}

function calculateByWidth() {
	var aspectRatioWidth = $("#aspectRatioWidth").val();
	var aspectRatioHeight = $("#aspectRatioHeight").val();
	var aspectRatioDiagonal = calculateDiagonal(aspectRatioWidth, aspectRatioHeight);

	for (var i in UNITS) {
		var unit = UNITS[i];
		var inputElement = findInput("w", unit);
		var width = inputElement.data("exactValue") || inputElement.val();

		if (width > 0) {
			var factor = width / aspectRatioWidth;
			var diagonal = aspectRatioDiagonal * factor;
			var height = aspectRatioHeight * factor;

			findInput("d", unit).val(diagonal.toFixed(1)).data("exactValue", diagonal);
			findInput("h", unit).val(height.toFixed(1)).data("exactValue", height);

		}
	}
}

function calculateByHeight() {
	var aspectRatioWidth = $("#aspectRatioWidth").val();
	var aspectRatioHeight = $("#aspectRatioHeight").val();
	var aspectRatioDiagonal = calculateDiagonal(aspectRatioWidth, aspectRatioHeight);

	for (var i in UNITS) {
		var unit = UNITS[i];
		var inputElement = findInput("h", unit);
		var height = inputElement.data("exactValue") || inputElement.val();

		if (height > 0) {
			var factor = height / aspectRatioHeight;
			var diagonal = aspectRatioDiagonal * factor;
			var width = aspectRatioWidth * factor;

			findInput("d", unit).val(diagonal.toFixed(1)).data("exactValue", diagonal);
			findInput("w", unit).val(width.toFixed(1)).data("exactValue", width);

		}
	}
}
