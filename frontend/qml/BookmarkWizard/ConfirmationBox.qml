// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import com.nokia.meego 1.0
import "globals.js" as Globals

Page{
    id:confBoxPage;
    property alias conftext :confirmationText;
    property alias yesbtn : btnYes;

    Rectangle{
        width:parent.width;
        height:parent.height * 0.7;
        anchors.centerIn: parent;
        color:"transparent";

        Column{
            anchors.fill:parent;
            anchors.centerIn: parent.Center;
            spacing : 10;
            Text{
                id:confirmationText;
                width:parent.width;
                font.family: Globals.fontFamily;
                font.pointSize: Globals.pointFontSize;
                font.bold: true;
                text:"TBD";
                color:"white";
                horizontalAlignment: Text.AlignHCenter;
            }
            Button{
                id:btnYes;
                width:parent.width;
                anchors.horizontalCenter: parent.horizontalCenter;
                text:"Yes";
/*                onClicked: {
                    pageStack.pop();
                }*/
            }
            Button{
                width:parent.width;
                anchors.horizontalCenter: parent.horizontalCenter;
                id:btnNo;
                text:"No";
                onClicked: {
                    pageStack.pop();
                }
            }
        }
    }
}
