var fontFamily = "Verdana";
var pointFontSize = 18;
var itemBackGroundColor = "#4000FF00";
var selectedIndex = 0;

var TYPE_BM_BROWSER=0;
var TYPE_BM_FF = 1;
var TYPE_BM_OPERA = 2;

function deleteCurrentItem(){
    pageStack.pop();
    bmModel.deleteCurrent(idElement);
    bmModel.currIndex = -1;
    bmModel.idElement = -1;
}
