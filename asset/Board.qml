import QtQuick 1.0

Rectangle {
	id: board
	width: 800; height: 600
	color: "lightgray"
	
	Column {
		id: col
		anchors.top: board.top; 
		anchors.left: board.left;
		anchors.topMargin: 4; anchors.leftMargin: 4;
		spacing: 0
		
		Repeater {
			model: 11
			BoardRow {
				anchors.left: parent.left;
				anchors.leftMargin: index % 2 ? 0 : 25;
				rowIndex: index
			}
		}
	}
 }