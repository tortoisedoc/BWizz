import QtQuick 1.0
import com.nokia.meego 1.0 //1.1 -> 1.0 Should fix PR 1.1?
import com.giulietta.bookmarktools 1.0
import "globals.js" as Globals

PageStackWindow {

    id: appWindow    
    initialPage: mainPage;

    EditBMPage{
        id:editBMPage;
    }
    NoBookmarks{
        id:noBookmarksPage;
    }
    Page{
        id : mainPage;
        property alias bmList:bmList;

        Flickable {
            id: bmListFlickable;
            anchors.fill:parent;
            contentHeight: bmList.height;
            FocusScope{
                id:focusScope;
                //FocusScope needs to bind to visual properties of the children
                anchors.fill:parent;


                ListView{ //Bookmarklist
                    id : bmList;
                    visible:false;
                    focus:true;
                    highlightFollowsCurrentItem: true;                    
                    highlightMoveDuration: 0;
                    highlightMoveSpeed:3200;
                    highlight:Rectangle {
                          id:highlightObj;
                          radius:22;
                          width : parent.width;
                          smooth: true;
                          color: "lightsteelblue";                          
                          Text{
                              id:textHoldToEdit;
                              height:highlightObj.height / 2;
                              width:60;
                              verticalAlignment: Text.AlignBottom;
                              anchors.right: highlightObj.right;
                              anchors.rightMargin: 60;
                              anchors.bottom: highlightObj.bottom;
                              text:"Hold to Edit..";
                              font.family: Globals.fontFamily;
                              font.bold: true;
                              font.pointSize: Globals.pointFontSize - 7;
                              color:"red";
                              visible:true;
                          }
                    }


                    model:Bookmarkmodel{
                        id:bmModel;
                    }

                    anchors.fill: parent;
                    anchors.margins: 20;
                    delegate:
                    Item {
                        property variant myData: model;
                        property variant iconImage: iconImg;
                        id : itemObj;

                        clip:true;
                        width:parent.width;
                        height: 80;

                        Rectangle{
                            id : textBackGround;
                            anchors.fill:parent;
                            radius:22;
                            smooth: true;
                            color:"transparent";
                            Item{
                                id:iconImgItem;
                                height: itemObj.height;
                                width : itemObj.height;
                                Image {
                                     id : iconImg;
                                     anchors.fill: parent;
                                     fillMode: Image.PreserveAspectCrop;
                                     sourceSize.width: 80;
                                     sourceSize.height: 80;
                                     anchors.verticalCenter: parent.verticalCenter;
                                     anchors.left:parent.left;
                                     source: iconurl;
                                     clip: true;
                                     asynchronous: true;
                                }
                            }
                            Text {
                                 id:itemText;
                                 height:itemObj.height;
                                 width:parent.width*0.7;
                                 anchors.left : iconImgItem.right;
                                 anchors.topMargin: 3;
                                 horizontalAlignment :Text.AlignHCenter;
                                 verticalAlignment: Text.AlignVCenter;
                                 text: name;
                                 font.family: Globals.fontFamily;
                                 font.bold: true;
                                 font.pointSize: Globals.pointFontSize;
                                 color: "white";
                                 wrapMode: Text.WordWrap;
                            }
                            Rectangle{
                                 id:catIconPanel;
                                 height:itemObj.height;
                                 anchors.left: itemText.right;
                                 anchors.right:parent.right;
                                 color:"transparent";
                                 Image {
                                     anchors.right: parent.right;
                                     anchors.bottom: parent.Bottom;
                                     anchors.verticalCenter:  parent.verticalCenter;
                                     id : iconCatImg;
                                     height: parent.height / 2;
                                     width : parent.height / 2;
                                     source: cat_icon;
                                 }
                            }
                            MouseArea{
                                enabled: true;
                                anchors.fill :parent;
                                hoverEnabled: false;
                                onPressed: {
                                    bmList.currentIndex = index;
                                }
                                onPressAndHold:{
                                    Globals.selectedIndex = index;
                                    editBMPage.idElement = index;
                                    bmList.currentIndex = index;
                                    editBMPage.bmName.text = bmList.currentItem.myData.name;
                                    editBMPage.bmUrl.text = bmList.currentItem.myData.url;
                                    console.debug(bmList.currentItem.myData.iconurl);
                                    editBMPage.bmIcon.source = bmList.currentItem.myData.iconurl;
                                    pageStack.push(editBMPage);
                                }
                           }
                        }
                    }
                }
            }
        }
        
        ConfirmationBox{
            id:confBox;
        }

        ContextMenu {
            id: browsersMenu;
            MenuLayout {
                MenuItem{
                    text:"Home Screen";
                    enabled:(bmList.currentItem != null) && (bmList.currentItem.myData.type 
                            != Globals.TYPE_BM_BROWSER);
                    id:exportBrowser;
                    onClicked: {
                        bmModel.exportCurrentBookMarkToHomeScreen(bmList.currentIndex);
                    }
                }
                MenuItem{
                    text:"Firefox";
                    enabled:(bmList.currentItem != null) 
                        && (bmList.currentItem.myData.type != Globals.TYPE_BM_FF);
                    id:exportFF;
                    onClicked: {
                        bmModel.exportCurrentBookMarkToFirefox(bmList.currentIndex);
                    }
                }
                MenuItem{
                    text:"Opera";
                    enabled:(bmList.currentItem != null) 
                        && (bmList.currentItem.myData.type != Globals.TYPE_BM_OPERA);
                    id:exportOpera;
                    onClicked: {
                        bmModel.exportCurrentBookMarkToOpera(bmList.currentIndex);
                    }
                }
            }

        }
        Menu {
             id: mainMenu
             // define the items in the menu and corresponding actions
             content: MenuLayout {
                 MenuItem {
                     text: "Remove";
                     enabled: bmList.currentItem != null;
                     onClicked: {
                         confBox.conftext.text = "Remove?";
                         connConfBoxRemove.target = confBox.yesbtn;
                         pageStack.push(confBox);
                     }
                 }
                 MenuItem{
                     text: "Export";
                     enabled: bmList.currentItem != null;
                     onClicked:{
                         browsersMenu.open();
                     }
                 }

     /*                Button{
                         id:btnIcon;
                         iconSource:"image://theme/icon-m-toolbar-image-edit-white";
                         width: parent.width /3;
     //                            height:parent.height;
                         onClicked: {
                             //pageStack.push(galleryPage);
                         }
                     }
                 }*/
             }
        }

        Connections {
            id:connConfBoxRemove;
            onClicked:{
                if (bmList.currentIndex != -1)
                    bmModel.deleteCurrent(bmList.currentIndex);
                bmList.currentIndex = -1;
                pageStack.pop();
            }
        }

        ToolBar {
            id: toolbar
            anchors.bottom: parent.bottom
            width:parent.width;
            height:80;
            tools: ToolBarLayout {
                // add the standard menu button to the toolbar
                ToolButton {
                    anchors.right: parent.right;
                    iconSource: "image://theme/icon-m-toolbar-view-menu-white"
                    onClicked: mainMenu.open()
                }
            }
        }
    }


    Component.onCompleted:
    {
        console.log( "application ready." );
        theme.inverted = true;
        if (bmModel.count > 0){
            bmList.visible = true;
            bmList.currentIndex = -1;
        }else{
            pageStack.push(noBookmarksPage);
        }
    }
}



