import QtQuick 1.0

Row {
	id: rrow
	property int rowIndex
	
	Repeater {
		model: rowIndex % 2 ? 6:5
		Segment { segColor: rowIndex % 2 ? "white" : "red" }
	}
}