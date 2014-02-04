import QtQuick 1.0

Item {
	id: segment
	property alias segColor: rectangle.color
	width: 50; height: 25
	
	Rectangle {
		id: rectangle
		width: 10; height: 10
		border.color: "white"
		anchors.left: segment.left
		anchors.verticalCenter: segment.verticalCenter
	}
}