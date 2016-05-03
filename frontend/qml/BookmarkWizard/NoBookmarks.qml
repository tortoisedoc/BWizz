// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import com.nokia.meego 1.0
import "globals.js" as Globals

Page{
    Rectangle{ //No bookmarks
        id : colEmptyList;
        visible:true;
        height:parent.height * 0.5;
        width: parent.width;
        anchors.verticalCenter: parent.verticalCenter;
        radius:22;
        smooth: true;
        color:"black";
        //NAME
        Column{
            anchors.fill: parent;
            width: parent.width;
            spacing: 10;
            Text{
                id: noBookMarkNoticeTxt;
                horizontalAlignment: Text.AlignHCenter;
                font.family: Globals.fontFamily;
                //anchors.verticalCenter: parent.Center;
                text:"No user-defined bookmarks to edit.\nAdd some to your home screen with help of the built-in browser!";
                font.bold: true;
                font.pointSize: Globals.pointFontSize;
                anchors.horizontalCenter: parent.Center;
                color: "white";
                width:parent.width;
                wrapMode: TextEdit.Wrap;
            }
            Button{
                id: btnClose;
                width: parent.width*0.9;
                font.family: Globals.fontFamily;
                anchors.horizontalCenter: parent.horizontalCenter;
                font.bold: true;
                anchors.centerIn: parent.Center;
                text: "Close me.";
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }
}
