function fileVerGet()
{
  var ver;
  if (undefined != top.fileVer)
  {
    if (top.DebugMode)
      ver = (new Date().getTime());
    else
      ver = top.fileVer;
  }
  else if (undefined != top.parent.fileVer)
  {
    if (top.parent.DebugMode)
      ver = (new Date().getTime());
    else
      ver = top.parent.fileVer;
  }
  else if (undefined != top.opener)
  {
    if (undefined != top.opener.top.parent.fileVer)
    {
      if (top.opener.top.parent.DebugMode)
        ver = (new Date().getTime());
      else
        ver = top.opener.top.parent;
    }
  }

  return ver;
}

function _fileLoad(filename, filetype){
 if (filetype=="js"){ //if filename is a external JavaScript file
  var fileref=document.createElement('script');
  fileref.setAttribute("type","text/javascript");
  fileref.setAttribute("src", filename);
 }
 else if (filetype=="css"){ //if filename is an external CSS file
  var fileref=document.createElement("link");
  fileref.setAttribute("rel", "stylesheet");
  fileref.setAttribute("type", "text/css");
  fileref.setAttribute("href", filename);
 }
 if (typeof fileref!="undefined")
  document.getElementsByTagName("head")[0].appendChild(fileref);
}

function fileLoad(filename, filetype, version){
  var file = filename;
  var ver = fileVerGet();

  if (!(typeof(version) == "undefined" || version == null))
    file += '?ver='+version;
  else if (!(typeof(ver) == "undefined" || ver == null))
    file += '?ver='+ver;

  _fileLoad(file,filetype);
}

function browserVer(){
    var ua= navigator.userAgent,
    N= navigator.appName, tem,
    M= ua.match(/(opera|chrome|safari|firefox|msie|trident)\/?\s*([\d\.]+)/i) || [];
    M= M[2]? [M[1], M[2]]:[N, navigator.appVersion, '-?'];
    if(M && (tem= ua.match(/version\/([\.\d]+)/i))!= null) M[2]= tem[1];
    return M.join(' ');
}

function browserOlderIE10() {
  if (navigator.appName.indexOf("Microsoft") != -1) {
    var ver = browserVer().split(" ")[1];
    var ver_major = ver.split(".")[0];
    if (parseInt(ver_major) < 10)
      return true;
    else
      return false;
  }
  return false;
}

function sleep(delay) {
  var start = new Date().getTime();
  while (new Date().getTime() < start + delay);
}

function fileLoginLoadWait()
{
  if (top.MockupMode)
    return;
  if (browserOlderIE10())
  {
    fileLoad("/js/json2.js", "js");
    if ((typeof(RootFrame) != "undefined")) {
      while(1)
      {
        if ((typeof(ready_js_utility) == "undefined"))
          fileLoad("/js/utility.js", "js");
        if ((typeof(ready_js_url) == "undefined"))
          fileLoad("/js/url.js", "js");
        if ((typeof(ready_js_utility) != "undefined") && (typeof(ready_js_url) != "undefined")) {
          break;
        }
        sleep(500);
      }
    }
  }
}

function fileLoadWait()
{
  if (top.MockupMode)
  {
    if (browserOlderIE10()) {
      fileLoad("../js/json2.js", "js");
    }
    return;
  }
  if (browserOlderIE10())
  {
    fileLoad("/js/json2.js", "js");
    if ((typeof(RootFrame) != "undefined")) {
      while(1)
      {
        if ((typeof(ready_js_styling) == "undefined"))
          fileLoad("/js/styling.js", "js");
        if ((typeof(ready_js_utility) == "undefined"))
          fileLoad("/js/utility.js", "js");
        if ((typeof(ready_js_url) == "undefined"))
          fileLoad("/js/url.js", "js");
        if ((typeof(ready_js_styling) != "undefined") && (typeof(ready_js_utility) != "undefined") && (typeof(ready_js_url) != "undefined")) {
          break;
        }
        sleep(500);
      }
    } else {
      while(1)
      {
        if ((typeof(ready_js_styling) == "undefined"))
          fileLoad("/js/styling.js", "js");
        if ((typeof(ready_js_utility) == "undefined"))
          fileLoad("/js/utility.js", "js");
        if ((typeof(ready_js_styling) != "undefined") && (typeof(ready_js_utility) != "undefined")) {
          break;
        }
        sleep(500);
      }
    }
  }
}

function fileLoadMainFrame()
{
  fileLoad("../css/styling.css", "css");
  fileLoad("../js/styling.js", "js");
  fileLoad("../js/utility.js", "js");

  _fileLoad("../js/jquery.dataTables.min.js", "js");
  _fileLoad("../css/jquery.dataTables.css", "css");
  _fileLoad("../js/jquery.toastmessage-min.js", "js");
  _fileLoad("../css/jquery.toastmessage.css", "css");
  _fileLoad("../js/jquery.qtip.min.js", "js");
  _fileLoad("../css/jquery.qtip.css", "css");
}

function fileLoadDialog()
{
  fileLoad("../css/styling.css", "css");
  fileLoad("../js/styling.js", "js");
  fileLoad("../js/utility.js", "js");

  _fileLoad("../js/jquery.toastmessage-min.js", "js");
  _fileLoad("../css/jquery.toastmessage.css", "css");
  _fileLoad("../js/jquery.qtip.min.js", "js");
  _fileLoad("../css/jquery.qtip.css", "css");
}
