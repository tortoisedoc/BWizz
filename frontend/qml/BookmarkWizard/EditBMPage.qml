// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import com.nokia.meego 1.0
import "globals.js" as Globals

Page{
//    id : editBMPage;
    property alias bmName: bmName;
    property alias bmUrl: bmUrl;
    property alias bmIcon: bmIcon;
    property int idElement;

    Gallery{
        id:galleryPage;
    }

    FocusScope{
        id:focusScope;
        //FocusScope needs to bind to visual properties of the children
        anchors.fill:parent;

        Column{
            id:maincolumn;
            spacing: 14;
            width:parent.width;
            Row{
                width:parent.width;
                height:editBMPage.height*0.10;
                Rectangle{
                    color:"black";
                    width: parent.width/2.0;
                    height: parent.height;
                    Button{
                        width: parent.width*0.9;
                        font.family: Globals.fontFamily;
                        anchors.centerIn: parent.Center;
                        text: "Back";
                        onClicked: {
                            bmName.closeSoftwareInputPanel();
                            bmUrl.closeSoftwareInputPanel();
                            bmName.focus = false;
                            bmUrl.focus = false;
                            pageStack.pop();
                        }
                    }
                }
                Rectangle{
                    color:"black";
                    width: parent.width/2.0;
                    height: parent.height;
                    Button{
                        width: parent.width*0.9;
                        font.family: Globals.fontFamily;
                        anchors.centerIn: parent.Center;
                        text: "Save";
                        onClicked: {
                            bmName.closeSoftwareInputPanel();
                            bmUrl.closeSoftwareInputPanel();
                            bmName.focus = false;
                            bmUrl.focus = false;
                            bmModel.setData(idElement, bmName.text, 33);
                            bmModel.setData(idElement, bmIcon.source, 34);
                            bmModel.setData(idElement, bmUrl.text, 36);
                            bmModel.saveCurrentEdit(idElement);
                            pageStack.pop();
                        }
                    }
                }
            }

            //NAME
            Text{ //caption
                height:editBMPage.height*0.06;
                width:parent.width;
                font.family: Globals.fontFamily;
                text: " Name :";
                font.bold: true;
                font.pointSize: Globals.pointFontSize;
                color: "white";
            }
            Rectangle{
                color:Globals.itemBackGroundColor;
                anchors.horizontalCenter: parent.horizontalCenter;
                radius:20;
                width:parent.width*0.9;
                height:editBMPage.height*0.12;
                border.color: "white";
                border.width: 3;
                TextInput{
                    id:bmName;
                    anchors.fill: parent;
                    anchors.leftMargin: 25;
                    anchors.rightMargin: 25;
                    anchors.topMargin:parent.height * 0.09;
                    font.family:Globals.fontFamily;
                    font.pointSize: Globals.pointFontSize;
                    font.bold: true;
                    color:"white";
                    horizontalAlignment: TextInput.AlignLeft;
                    maximumLength: 255;
                    selectByMouse: true;
                    autoScroll: true;
                    onFocusChanged: {
                        if (!bmName.focus){
                            bmName.closeSoftwareInputPanel();
                        }
                    }
                }
            }
            //URL
            Text{ //Caption
                height: editBMPage.height*0.06;
                anchors.horizontalCenter: parent.Center;
                text: " URL :";
                font.bold: true;
                font.family:Globals.fontFamily;
                font.pointSize: Globals.pointFontSize;
                color: "white";
            }

            Rectangle{
                color: Globals.itemBackGroundColor;
                anchors.horizontalCenter: parent.horizontalCenter;
                radius:20;
                smooth: true;
                border.color: "white";
                border.width: 3;
                width:parent.width*0.9;
                height:editBMPage.height*0.36;
                TextEdit{
                    id:bmUrl;
                    width:parent.width;
                    wrapMode: TextEdit.Wrap;
                    height:parent.height;
                    textMargin:15;
                    font.family: Globals.fontFamily;
                    font.bold:true;
                    font.pointSize: Globals.pointFontSize;
                    color:"white";
                }
            }

            //Icona
            Rectangle{
                color:"transparent";
                anchors.horizontalCenter: parent.horizontalCenter;
                radius:20;
                width:parent.width;
                height:80;
                Image{
                    id:bmIcon;
                    fillMode: Image.PreserveAspectCrop;
                    width:parent.height;
                    height:parent.height;
                    clip:true;
                    asynchronous: true;
                    sourceSize.width: parent.height;
                    sourceSize.height: parent.height;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           bmName.closeSoftwareInputPanel();
                           bmUrl.closeSoftwareInputPanel();
                           galleryPage.gridImage.source = bmIcon.source;
                           pageStack.push(galleryPage);
                       }
                    }
                }
                Rectangle { // rounded corners for img
                    anchors.fill: bmIcon;
                    color: "transparent";
                    border.color: "white"; // color of background
                    border.width: 4;
                    radius: 10;
                }
            }

        }
    }
}
