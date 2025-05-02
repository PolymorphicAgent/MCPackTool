import QtQuick 2.15
import Qt.labs.lottieqt 1.0

Item {
    width: 20
    height: 20

    LottieAnimation {
        id: loader
        source: "qrc:/icons/BlueSpinner.json"
        autoPlay: true
        loops: Animation.Infinite
        quality: LottieAnimation.HighQuality

        anchors.centerIn: parent

        //manual uniform scale
        transform: Scale {
            origin.x: loader.width / 2
            origin.y: loader.height / 2
            xScale: 0.5
            yScale: 0.5
        }
    }
}
