import QtQuick 1.0
import com.nokia.meego 1.0
import QtMobility.gallery 1.1
import "globals.js" as Globals

Page{      
    property alias gridImage: gridImage;
    tools:galleryTB;

    Image{
        visible:false;
        id:gridImage;
    }

    anchors.fill: parent;
    Rectangle {
        anchors.fill:parent;
        color:"transparent";

        Rectangle{
            id:rootRectangle;
            color:"transparent";
            anchors.fill: parent;
            GridView {
                id:imageGrid;
                property int galleryPage:0;
                property int galleryPageCount:6;

                anchors.fill: parent;
                anchors.horizontalCenter: parent.horizontalCenter;
                cellWidth: parent.width / 3 - 10;
                cellHeight: parent.width / 3 - 10;
                model:  DocumentGalleryModel {
                    id : galleryModel;
                    autoUpdate: false;
                    rootType: DocumentGallery.Image;
                    properties: ["fileName", "filePath"]
                    sortProperties: ["fileName"]
                }

                delegate:
                    Item {
                        id: name
                        //! Thumbnail display of images found in the Document Gallery
                        Item {
                            width: 158;
                            height: 158;

                            //! Loading smaller images not to use more memory than needed.
                            //! Image is scaled to fill cell and cropped if necessary.
                            Image {
                                id : selectedImage;
                                anchors.fill: parent;
                                source: model.filePath;
                                fillMode: Image.PreserveAspectCrop;
                                sourceSize.width: 158;
                                clip: true;
                                asynchronous: true;
                                onStatusChanged: {
                                    if (status === Image.Error)
                                        errormsg.text = "Unable to load image";
                                }
                                MouseArea{
                                    enabled: true;
                                    anchors.fill :parent;
                                    onDoubleClicked: {
                                        editBMPage.bmIcon.source = selectedImage.source;
                                        pageStack.pop();
                                    }
                                }
                                Label {
                                    id: errormsg;
                                    wrapMode: Text.WordWrap;
                                    font.pixelSize: Globals.pointFontSize;
                                    color: "white";
                                }
                            }
                        }
                    }
            }
        }
        ToolBarLayout {
                // add the standard menu button to the toolbar
            id:galleryTB;
            ToolIcon {
                width:parent.width/3;
                id:btnClose;
                anchors.left: parent.left;
                anchors.bottom: parent.bottom;
                iconSource: "image://theme/icon-m-toolbar-close-white"
                onClicked: {
                    pageStack.pop();
                }
            }
        }
    }
}
