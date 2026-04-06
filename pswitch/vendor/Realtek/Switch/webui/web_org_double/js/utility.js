function str2var(str){if(IsUndefOrNull(str)||('' == str)){return '';}return eval('('+str+')');}
function str2int(str){return parseInt(str,10);}
function formDataGet(formId){var _data=$("#"+formId).serialize(); var data = '_ds=1&' + ((IsUndefOrNull(_data)||""==_data)?"empty=1":_data) + '&_de=1';return data;}
function urlDataGen(data){var __data = '_ds=1&' + ((IsUndefOrNull(data)||""==data)?"empty=1":data) + '&_de=1';return __data;}
function dataAppend(data, key, value){var _data = data; _data += '&'+key+'='+value; return _data;}
function convertLineBreaksToHTML(i_str){var res; res = i_str.split(/\r\n|\r|\n/g).join("<br>"); return res;}
function strncmp(a, b, n){return a.substring(0, n) == b.substring(0, n);}
function numberFormatted(num) {return num;} /* Do nothing now. Just Warrper API for later use */
var isSafari = (navigator.vendor && (navigator.vendor.toLowerCase().indexOf('apple') != -1) ) ? true : false;
var isFF = (navigator.userAgent && (navigator.userAgent.toLowerCase().indexOf('firefox') != -1) ) ? true : false;
var isChrome = (navigator.userAgent && (navigator.userAgent.toLowerCase().indexOf('chrome') != -1) ) ? true : false;
var isHTTP = (top.location.protocol.indexOf('http:') != -1) ? true : false;
var isHTTPS = (top.location.protocol.indexOf('https:') != -1) ? true : false;

function isIE()
{
  var rv = -1;
  if (navigator.appName == 'Microsoft Internet Explorer')
  {
    var ua = navigator.userAgent;
    var re  = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
    if (re.exec(ua) != null)
      rv = parseFloat( RegExp.$1 );
  }
  else if (navigator.appName == 'Netscape')
  {
    var ua = navigator.userAgent;
    var re  = new RegExp("Trident/.*rv:([0-9]{1,}[\.0-9]{0,})");
    if (re.exec(ua) != null)
      rv = parseFloat( RegExp.$1 );
  }
  if (-1 == rv)
    return false;
  else
    return rv;
}

/* IE8 or below version do not support indexOf on Array type, implement it here */
if (!Array.prototype.indexOf)
{
  Array.prototype.indexOf = function(elt /*, from*/)
  {
    var len = this.length >>> 0;
    var from = Number(arguments[1]) || 0;
    from = (from < 0) ? Math.ceil(from) : Math.floor(from);
    if (from < 0)
      from += len;

    for (; from < len; from++)
    {
      if (from in this && this[from] === elt)
        return from;
    }
    return -1;
  };
}

function get_cookie(name_to_get)
{
  var cookie_pair
  var cookie_name
  var cookie_value
  var cookie_array = document.cookie.split("; ")
  for (var counter = 0; counter < cookie_array.length; counter++)
  {
    cookie_pair = cookie_array[counter].split("=")
    cookie_name = cookie_pair[0]
    cookie_value = cookie_pair[1]
    if (cookie_name == name_to_get) {
      return unescape(cookie_value)
    }
  }
  return null
}
function delete_cookie(cookie_name, cookie_path, cookie_domain, cookie_secure)
{
  if(cookie_path == null)
  cookie_path = "/";
  var delCookieDate = new Date();
  delCookieDate.setDate(delCookieDate.getDate() - 1);
  set_cookie(cookie_name, "", delCookieDate, cookie_domain, cookie_secure);
}
function set_cookie(cookie_name, cookie_value, cookie_expire, cookie_path, cookie_domain, cookie_secure)
{
  var expire_string;
  var cookie_string = cookie_name + "=" + cookie_value;
  if (cookie_expire)
  {
    if( typeof(cookie_expire)!="string")
    {
      expire_string=cookie_expire.toGMTString();
    }else{
    var expire_date = new Date();
    var ms_from_now = cookie_expire * 24 * 60 * 60 * 1000;
    expire_date.setTime(expire_date.getTime() + ms_from_now)
    expire_string = expire_date.toGMTString();
    }
    cookie_string += "; expires=" + expire_string;
  }
  if (cookie_path) {
    cookie_string += "; path=" + cookie_path;
  }
  if (cookie_domain) {
    cookie_string += "; domain=" + cookie_domain;
  }
  if (cookie_secure) {
    cookie_string += "; true"
  }
  top.document.cookie = cookie_string;
}

function getCookiePaging()
{
  var id = document.location.href.substring(document.location.href.lastIndexOf('/') + 1);

  if (-1 != id.indexOf('?'))
    id = id.substring(0, id.indexOf('?'));

  if(IsUndefOrNull(get_cookie(id))) return 10; else return get_cookie(id);
}

function getCookieIntfPaging()
{
  var id = document.location.href.substring(document.location.href.lastIndexOf('/') + 1);

  if (-1 != id.indexOf('?'))
    id = id.substring(0, id.indexOf('?'));

  if(IsUndefOrNull(get_cookie(id))) return 0; else return get_cookie(id);
}

function setCookiePaging(val)
{
  var id = document.location.href.substring(document.location.href.lastIndexOf('/') + 1);

  if (-1 != id.indexOf('?'))
    id = id.substring(0, id.indexOf('?'));

  set_cookie(id, val);
}

function lang(category, item, args1, args2)
{
  /* Final format will be "_top.defLang.lang_category.item" */
  var langKey = '_top.';
  var selectedLang = get_cookie("cookie_language");
  var msg;

  //if (!IsUndefOrNull(args))
  //  console.log(args);

  if ((IsUndefOrNull(selectedLang)) || (selectedLang == "defLang_en"))
  {
	selectedLang = "defLang_en";
  }
  else
  {
	selectedLang = "defLang_ch";
  }

  langKey += selectedLang;
  langKey += ".lang_";
  langKey += category+'.';
  langKey += item;

  msg = str2var(langKey);

  /* Re-read default lang again, read default message. */
  if (IsUndefOrNull(msg))
  {
	langKey = '_top.';
	langKey += selectedLang;
	langKey += ".lang_";
    langKey += category+'.';
    langKey += item;
    msg = str2var(langKey);
  }

  //console.log(langKey);

  /* null again, popup warning message */
  if (IsUndefOrNull(msg))
  {
      alert(langKey + " is not defined.");
      return msg;
  }

  var cnt = 1;
  if (!IsUndefOrNull(args1))
  {
    _.each(args1, function(t)
    {
      var dollar = msg.indexOf('$'+cnt);
      if (-1 != dollar)
        msg = msg.replace('$'+cnt, t);
      else
        alert("message " + msg + " parameter number not match.");

      cnt++;
    });
  }

  if (!IsUndefOrNull(args2))
  {
    _.each(args2, function(t)
    {
      var dollar = msg.indexOf('$'+cnt);
      if (-1 != dollar)
        msg = msg.replace('$'+cnt, t);
      else
        alert("message " + msg + " parameter number not match.");

      cnt++;
    });
  }

  if (-1 != msg.indexOf('$'+cnt))
    alert("message " + msg + " parameter number not match.");

  //if (item[0] == 't' && item[1] =='i' && item[2] == 'p')
  //  msg = '"'+msg+'"';
  //console.log(msg);

  return msg;
}

function show(txt)
{
  if (IsUndefOrNull(txt))
    return '';

  if (strncmp(txt,'lang(',5))
    return str2var(txt);
  else
  {
    /* repalce & to ? first. To avoid matching error */
    while (-1 != txt.indexOf("&"))
      txt = txt.replace("&", "???");

    while (-1 != txt.indexOf(">"))
      txt = txt.replace(">", "&gt;");

    while (-1 != txt.indexOf("<"))
      txt = txt.replace("<", "&lt;");

    while (-1 != txt.indexOf(" "))
      txt = txt.replace(" ", "&nbsp;");

    while (-1 != txt.indexOf("???"))
      txt = txt.replace("???", "&amp;");

    //console.log(txt);
    return txt;
  }
}

function helplink(_link)
{
  var link = '';
  if (isExtLangSelected()) {
    link = 'extHelp/';
  } else {
    link = 'help/';
  }
  link += 'index.html?'+_link;
  return link;
}

function url(action)
{
  var txtUrl = '_top.url_';
  txtUrl += action;
  return str2var(txtUrl);
}

function dataGet(_url)
{
  var __url;
  if(IsUndefOrNull(_url.split('?')[1]))
    __url = _url+'?dummy='+(new Date().getTime());
  else
    __url = _url+'&dummy='+(new Date().getTime());

  var Datas = Backbone.Model.extend({url: __url});
  return new Datas();
}

function logoutCheck(resp, dialogWindow)
{
  if (!IsUndefOrNull(resp.logout))
  {
    var _url = "../login.html"

    if (!IsUndefOrNull(resp.reason))
      _url = _url + "?reason=" + resp.reason;

    _top.location.href = _url;

    if (!IsUndefOrNull(dialogWindow))
    {
      dialogWindow.close();
    }
    return true;
  }

  return false;
}

function _resp_status_print(status, isOkKeepDialog)
{
  var txt = '';
  var img = '';
  var msg = '';
  var msgClass = '';

  switch(status.msgType)
  {
      case "save_success":
          msg = lang('common','msgNotifySave');
          break;
      case "success":
          msg = lang('common','msgSuccess');
          break;
      case "user_org":
      case "user_lang":
      case "sys_org":
		  //console.log(status.msg);
		  msg = lang('sysError', status.msg);	
	  	  //msg += '<br>';
		  //console.log(msg);
          msg = show(msg);
          break;
	  case "err_msgs":
		 //console.log('send err string');
      default:
          break;
  }

  switch(status.status)
  {
      case "ok":
          if (!(!IsUndefOrNull(isOkKeepDialog) && isOkKeepDialog)) {
            if (isIE() && isIE() > 10 && !IsUndefOrNull(_opener))
              _opener.ShowStatusSuccess(msg);
            CloseDialog();
          }
          ShowStatusSuccess(msg);
          break;
      case "error":
          if (!IsUndefOrNull(status.errMsgs)) {
            var _msg = '';
            for (var i = 0; i < status.errMsgs.length; i++) {
              if (strncmp(txt,'lang(',5))
			  {
                _msg += str2var(status.errMsgs[i]);
				//console.log(_msg);
			  }
              else
			  {
				 msg ='';
                msg += status.errMsgs[i];
				//console.log(msg);
				 var tok = msg.split(':');
				msg = lang('sysError', tok[1]);
				_msg += tok[0] + ':' + msg;	
					//console.log(msg);
			  }
              _msg += '<br>';
            }
			//console.log(_msg);
            ShowStatusError(_msg);
          } else {
            ShowStatusError(msg);
          }
          break;
      case "warning":
          ShowStatusWarning(msg);
          break;
      case "info":
          ShowStatusNotice(msg);
          break;
      case "none":
          return;
      default:
          break;
  }

  return;
}

var mainFrame;
var mainFrameGetUrl;
function dataSet(_url, data, isOkKeepDialog, action)
{
  var __url;
  _top.progressEnable();
  if (_top.MockupMode)
  {
    __url = _url;
    if (isIE() != 8)
      console.log(data);
    var Datas = Backbone.Model.extend({url: __url});
    dataGet(__url).fetch({
      success: function(model, resp){
        if (!IsUndefOrNull(mainFrame))
        {
            _top.progressDisable();
            _resp_status_print(resp, isOkKeepDialog);

            if(IsUndefOrNull(mainFrameGetUrl))
              mainFrame.render();
            else
              mainFrame.render(mainFrameGetUrl);

            if (!IsUndefOrNull(action))
              action(resp);
        }
      },
      error: function(model, resp){
      //console.log(resp);
        _top.progressDisable();
      }
    });
  }
  else
  {
    __url = _url+'&dummy='+(new Date().getTime());
    //if (!isIE())
    //  console.log(__url);
    var Datas = Backbone.Model.extend({
      url: __url,
      parse: function(resp, xhr){
        _top.progressDisable();

        if (!IsUndefOrNull(_opener)) {
          if(IsUndefOrNull(_opener.mainFrameGetUrl))
            _opener.mainFrame.render();
          else
            _opener.mainFrame.render(_opener.mainFrameGetUrl);
        }

        _resp_status_print(resp, isOkKeepDialog);

        if (!IsUndefOrNull(mainFrame))
        {
            if(IsUndefOrNull(mainFrameGetUrl))
              mainFrame.render();
            else
              mainFrame.render(mainFrameGetUrl);
        }

        if (!IsUndefOrNull(action))
          action(resp);
        return;
      },
      error: function(resp, xhr) {
        _top.progressDisable();
      }
    });
    var dataModel = new Datas();
    dataModel.save(data, {});
  }
}

function system_dataSet(_url, data, isOkKeepDialog, action)
{
  var __url;
  //_top.progressEnable();
  if (_top.MockupMode)
  {
    __url = _url;
    if (isIE() != 8)
      console.log(data);
    var Datas = Backbone.Model.extend({url: __url});
    dataGet(__url).fetch({
      success: function(model, resp){
        if (!IsUndefOrNull(mainFrame))
        {
            //_top.progressDisable();
            _resp_status_print(resp, isOkKeepDialog);

            if(IsUndefOrNull(mainFrameGetUrl))
              mainFrame.render();
            else
              mainFrame.render(mainFrameGetUrl);

            if (!IsUndefOrNull(action))
              action(resp);
        }
      },
      error: function(model, resp){
      //console.log(resp);
        //_top.progressDisable();
      }
    });
  }
  else
  {
    __url = _url+'&dummy='+(new Date().getTime());
    //if (!isIE())
    //  console.log(__url);
    var Datas = Backbone.Model.extend({
      url: __url,
      parse: function(resp, xhr){
        //_top.progressDisable();

        if (!IsUndefOrNull(_opener)) {
          if(IsUndefOrNull(_opener.mainFrameGetUrl))
            _opener.mainFrame.render();
          else
            _opener.mainFrame.render(_opener.mainFrameGetUrl);
        }

        _resp_status_print(resp, isOkKeepDialog);

        if (!IsUndefOrNull(mainFrame))
        {
            if(IsUndefOrNull(mainFrameGetUrl))
              mainFrame.render();
            else
              mainFrame.render(mainFrameGetUrl);
        }

        if (!IsUndefOrNull(action))
          action(resp);
        return;
      },
      error: function(resp, xhr) {
        //_top.progressDisable();
      }
    });
    var dataModel = new Datas();
    dataModel.save(data, {});
  }
}

function urlAppend(url, str)
{
  var _url = url;

  if(IsUndefOrNull(url))
    return "";

  if(IsUndefOrNull(url.split('?')[1])) {
    if (isIE())
      _url += '?_ds=1&'+str;
    else
      _url += '?'+str;
  } else {
    _url += '&'+str;
  }
  return _url;
}

function urlParamAppend(url, key, value)
{
  var _url = url;

  if(IsUndefOrNull(url))
    return "";

  if(IsUndefOrNull(url.split('?')[1]))
  {
    _url += '?'+key+'='+encodeURIComponent(value);
  }
  else
  {
    _url += '&'+key+'='+encodeURIComponent(value)
  }

  return _url;
}

function urlParamGetByName(name, _url)
{
  name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
  var regex = new RegExp("[\\?&]" + name + "=([^&#]*)");
  var results;
  if (IsUndefOrNull(_url))
    results = regex.exec(decodeURIComponent(location.search));
  else
    results = regex.exec(decodeURIComponent(_url));

  return results == null ? "" : results[1].replace(/\+/g, " ");
}

function urlGetSelSouce(url)
{
  var val = '_src='
  val += urlParamGetByName('_src',url);
  return val;
}

function urlParamGetVals(key, url)
{
  var _url = (IsUndefOrNull(url)) ? location.search : url;
  var vals = _url.split('&');
  var strTok;
  var first = true;
  var results = '';

  if ('?' == _url.charAt(0))
    _url = _url.substring(1, _url.length);

  vals = _url.split('&');

  if (!IsUndefOrNull(vals) && vals.length > 0)
  {
    for (i = 0; i < vals.length; i++) {
      strTok = decodeURIComponent(vals[i]).split('=');
      if (strTok[0] == key) {
        if (first)
          first = false;
        else
          results += ',';
        results += strTok[1];
      }
    }
  }
  return results;
}

function urlGetPortList(key, url, isLagOnly)
{
  var vals = urlParamGetVals(key, url);
  var strTok = vals.split(',');
  var results = '';
  var start = -1;
  var curr = -1;
  var end = -1;
  var isRange = false;
  var portNum = (isLagOnly) ? _top.devLagNum : (_top.devPortNum + _top.devLagNum);
  if (!(!IsUndefOrNull(strTok) && strTok.length > 0))
    return results;

  function _getPortId(idx) {
    return (isLagOnly) ? getLagIdStr(idx) : getPortId(idx)
  }

  for (var i = 0; i < strTok.length; i++) {
    curr = str2int(strTok[i]);
    next = (strTok.length - 1 == i) ? -1 : str2int(strTok[i + 1]);

    if (curr < portNum && next < portNum) {
      start = curr;
      while (next > 0 && (next == curr + 1) && curr < portNum && next < portNum){
        curr = next;
        i++;
        next = (strTok.length - 1 <= i) ? -1 : str2int(strTok[i + 1]);
        if (next >= portNum)
          break;
        if (next > 0 && (_getPortId(curr).charAt(0) != _getPortId(next).charAt(0)))
          break;
      }
      end = curr;
      if (start == end) {
        results += _getPortId(curr) + ',';
      } else if (_getPortId(start).charAt(0) != _getPortId(start + 1).charAt(0)) {
        if (start + 1 == end)
          results += _getPortId(start) + ',' + _getPortId(end) + ',';
        else
          results += _getPortId(start) + ',' + _getPortId(start+1) + '-' + _getPortId(end) + ',';
      } else {
        results += _getPortId(start) + '-' + _getPortId(end) + ',';
      }
    } else {
      start = curr;
      while (next > 0 && (next == curr + 1)){
        curr = next;
        i++;
        next = (strTok.length - 1 <= i) ? -1 : str2int(strTok[i + 1]);
      }
      end = curr;
      if (start == end)
        results += _getPortId(curr) + ',';
      else
        results += _getPortId(start) + '-' + _getPortId(end) + ',';
    }
  }
  if (results.charAt(results.length - 1) == ',')
  {
    results = results.substring(0,results.length - 1);
  }
  return results;
}

function urlGetVlanList(key, url)
{
  var vals = urlParamGetVals(key, url);
  var strTok = vals.split(',');
  var results = '';
  var start = -1;
  var curr = -1;
  var end = -1;
  var isRange = false;

  if (!(!IsUndefOrNull(strTok) && strTok.length > 0))
    return results;

  for (var i = 0; i < strTok.length; i++)
  {
    curr = str2int(strTok[i]);
    next = (strTok.length - 1 == i) ? -1 : str2int(strTok[i + 1]);

    if (start == -1)
      start = curr;

    if (next == -1)
    {
      if (start == curr)
        results += curr;
      else
        results += start + '-' + curr;

      break;
    }

    if (next == curr +1)
      continue;
    else if (start == curr)
      results += curr + ',';
    else
      results += start + '-' + curr + ',';

    start = -1;
  }

  //console.log(results);
  return results;
}

function dataVlanList(key, value)
{
  return "vlanList=" + urlGetVlanList(key, value);
}

function dataVlanListSrc(key, value)
{
  return "vlanList=" + urlGetVlanList(key, value) +'&' + urlGetSelSouce(value);
}

function dataMultiSelectVlanList(key, hiddenFormId)
{
    MultiSelectApplySelect(key);
    //console.log($('#' + key).serialize());

    if (IsUndefOrNull(hiddenFormId))
      $('#vlanList').val(urlGetVlanList(key, $('#' + key).serialize()));
    else
      $('#'+ hiddenFormId).val(urlGetVlanList(key, $('#' + key).serialize()));

    MultiSelectDeselectAll(key);
}

function tableGetPortList(portstring, isLagOnly)
{
  var vals = portstring;
  var strTok = vals.split(',');
  var results = '';
  var start = -1;
  var curr = -1;
  var end = -1;
  var isRange = false;
  var portNum = (isLagOnly) ? _top.devLagNum : _top.devPortNum;
  if (!(!IsUndefOrNull(strTok) && strTok.length > 0))
    return results;

  function _getPortId(idx) {
    return (isLagOnly) ? getLagIdStr(idx) : getPortId(idx)
  }

  for (var i = 0; i < strTok.length; i++) {
    curr = str2int(strTok[i]);
    next = (strTok.length - 1 == i) ? -1 : str2int(strTok[i + 1]);

    if (curr < portNum && next < portNum) {
      start = curr;
      while (next > 0 && (next == curr + 1) && curr < portNum && next < portNum){
        curr = next;
        i++;
        next = (strTok.length - 1 <= i) ? -1 : str2int(strTok[i + 1]);
        if (next >= portNum)
          break;
      }
      end = curr;
      if (start == end)
        results += _getPortId(curr) + ',';
      else
        results += _getPortId(start) + '-' + _getPortId(end) + ',';
    } else {
      start = curr;
      while (next > 0 && (next == curr + 1)){
        curr = next;
        i++;
        next = (strTok.length - 1 <= i) ? -1 : str2int(strTok[i + 1]);
      }
      end = curr;
      if (start == end)
        results += _getPortId(curr) + ',';
      else
        results += _getPortId(start) + '-' + _getPortId(end) + ',';
    }
  }
  if (results.charAt(results.length - 1) == ',')
  {
    results = results.substring(0,results.length - 1);
  }
  return results;
}

function showList(list, groupNum)
{
  var tok;
  var cnt = 0;
  var result = '';
  var group = (IsUndefOrNull(groupNum) ? 10 : groupNum)

  if (IsUndefOrNull(list))
    return result;

  tok = list.split(',');

  for (var i = 0; i < tok.length; i++) {
    result += tok[i];
    if (i != (tok.length - 1))
      result += ',';
    cnt++;

    if (cnt == group) {
      result += '<br>';
      cnt = 0;
    }
  }
  return result;
}

function writeFormattedNumber (value)
{
  /* Do formated by language number format later */
  return value;
}

function selectOption(selectId, entries, selectVal, selwidth)
{
  var txt='';
  var msg;

  
  if(selwidth)
  {
	  txt += '<select id="' + selectId + '" name="' + selectId + '"' + 'style="width:' + selwidth + ';">';
  }
  else
  {
	  txt += '<select id="' + selectId + '" name="' + selectId + '">';
  }

  _.each(entries, function(entry){
    if (IsUndefOrNull(entry.name))
    {
      msg = entry.val;
    }
    else
      msg = show(entry.name);

    txt += '<option value="'+entry.val+'"'+((entry.val == selectVal) ? "selected" : "")+">"+msg+'</option>'});
  txt += '</select>';

  return txt;
}

function printPortSelectOption (selectId, value)
{
  var txt = "";

  txt += '<select id="' + selectId + '" name="' + selectId + '">';

  _.each(_top.data.ports, function(n){
    txt += "<option value="+n.port;
    if (!IsUndefOrNull(value) && n.port==value)
      txt += " selected";
    txt += ">"+n.port +"</option>";
  });

  txt += '</select>';

  return txt;
}

function printNormalPortSelectOption (selectId, value)
{
  var txt = "";
  var p = _top.data.ports.length - _top.data.lags.length;

  txt += '<select id="' + selectId + '" name="' + selectId + '">';

  _.each(_top.data.ports.slice(0, p), function(n){
    txt += "<option value="+n.port;
    if (!IsUndefOrNull(value) && n.port==value)
      txt += " selected";
    txt += ">"+n.port +"</option>";
  });

  txt += '</select>';

  return txt;
}

function printLAGSelectOption (value)
{
  var txt = "";

  _.each(_top.data.lags, function(n){
    txt += "<option value="+n.lag;
    if (!IsUndefOrNull(value) && n.lag==value)
      txt += " selected";
    txt += ">"+n.lag +"</option>";
  });

  return txt;
}

function printLAGPortSelectOption (selectId, value)
{
  var txt = "";

  txt += '<select id="' + selectId + '" name="' + selectId + '">';

  _.each(_top.data.lags, function(n){
    txt += "<option value="+n.lag;
    if (!IsUndefOrNull(value) && n.lag==value)
      txt += " selected";
    txt += ">"+"LAG"+n.lag+"</option>";
  });

  txt += '</select>';

  return txt;
}

function getPortId(index)
{
  if (IsUndefOrNull(_top.data.ports[index]))
    return "";
  return _top.data.ports[index].port;
}

function getPortIdArray()
{
  var arr = {};
  for (i = 0; i < _top.data.ports.length; i++) {
    arr[i] = _top.data.ports[i].port;
  }

  return arr;
}

function getPortEntry(index)
{
  return index+1;
}

function getPortEntryById(id)
{
  for(i=0;i<_top.data.ports.length;i++)
  {
    if(id==_top.data.ports[i].port)
      return i;
  }
}

function isPortGE(id)
{
  if(-1 == id.indexOf('GE'))
    return false;
  else
    return true;
}

function getPortIdByEntry(entry)
{
  return _top.data.ports[entry - 1].port;
}

function getLagId(index)
{
  return _top.data.lags[index].lag;
}

function getLagIdStr(index)
{
  return 'LAG' + _top.data.lags[index].lag;
}

function removeSpacesFromHeadAndTail(val)
{
  val = val + "";
  while (val.length > 0)
  {
    if (val.charAt(0) == ' ' || val.charAt(0) == '\t')
      val = val.substr(1);
    else
      break;
  }
  while (val.length > 0)
  {
    if (val.charAt(val.length - 1) == ' ' || val.charAt(val.length - 1) == '\t')
      val = val.substr(0, val.length - 1);
    else
      break;
  }
  return val;
}

function sliceString(stringValue,token)
{
  var arr = new Array();
  arr = stringValue.split(token);
  return arr;
}

function autoNewLine(str, numPerLine)
{
  var result ='';
  var currNum = 0;
  for (var i = 0; i < str.length; i++) {
    result += str.charAt(i);
    currNum++;
    if (currNum == numPerLine) {
      currNum = 0;
      result += '<br>';
    }
  }
  return result;
}

/*  */
function isInputDisable(inputId){if($('#'+inputId).prop('disabled') == true){return true;}else{return false;}}
function updateValid(oldStatus, status)
{
  if(true==oldStatus&&true==status)
    return true;
  else
    return false;
}

function clearAllErrMsg(){$('.inputTextErrMsg').qtip('destroy', true);}
function errMsgUpdate(blockId, msg, isTop)
{
  var _my = 'left center';
  var _at = 'right center';

  if(!IsUndefOrNull(isTop) && isTop == true) {
    _my = 'bottom center';
    _at = 'top center';
  }

  if (isIE() && isIE() < 10)
  {
    $("#"+blockId).qtip({
      position:{my: _my,
                at: _at,
                adjust:
                {
                   x: document.body.scrollLeft,
                   y: document.body.scrollTop,
                   scroll: false,
                   resize: false
                }
             },
      style: { classes: "qtip-red qtip-rounded qtip-shadow" },
      content: {text: msg},
      hide: {event: false},
      show: {ready: true}
    });
  }
  else
  {
    $("#"+blockId).qtip({
      position:{my: _my, at: _at},
      style: { classes: "qtip-red qtip-rounded qtip-shadow" },
      content: {text: msg},
      hide: {event: false},
      show: {ready: true}
    });
  }
}

function checkNotEmpty(string)
{
  if ( string.length == 0 )
    return false;
  else
    return true;
}

function checkRange(val, validMin, validMax)
{
  if ( val < validMin || val > validMax)
    return false;
  else
    return true;
}

function checkMulti(val, validMulti)
{
  if ( (val % validMulti) != 0)
    return false;
  else
    return true;
}

function checkMacAddr(macStr)
{
  var mac = /^([0-9A-F]{1,2}:){5}[0-9A-F]{1,2}$/i;
  return mac.test(macStr);
}

function checkIPv4Addr(ipStr, isMulticast, isMask, isPrefix)
{
  var octet = '(?:25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9][0-9]|[0-9])';
  var ip    = '(?:' + octet + '\\.){3}' + octet;
  var ipRE  = new RegExp( '^' + ip + '$' );

  if (!ipRE.test(ipStr))
    return false;

  if (!IsUndefOrNull(isMulticast) && isMulticast == true) {
    var tok = ipStr.split('.');
    var firstGroup = str2int(tok[0]);
    if (firstGroup >= 224 && firstGroup <= 239)
      return true;
    else
      return false;
  } else if (!IsUndefOrNull(isMask) && isMask == true) {
    return true;
  } else if (!IsUndefOrNull(isPrefix) && isPrefix == true) {
    return true;
  } else {
    var tok = ipStr.split('.');
    var groups = new Array();
    groups[0] = str2int(tok[0]);
    groups[1] = str2int(tok[1]);
    groups[2] = str2int(tok[2]);
    groups[3] = str2int(tok[3]);
    if (groups[0] >= 1 && groups[0] <=126) {
      if (groups[1] == 0 && groups[2] == 0 && groups[3] == 0)
        return false;
      else if (groups[1] == 255 && groups[2] == 255 && groups[3] == 255)
        return false;
      else
        return true;
    } else if (groups[0] >= 128 && groups[0] <=191) {
      if (groups[2] == 0 && groups[3] == 0)
        return false;
      else if (groups[2] == 255 && groups[3] == 255)
        return false;
      else
        return true;
    } else if (groups[0] >= 192 && groups[0] <=223) {
      if (groups[3] == 0)
        return false;
      else if (groups[3] == 255)
        return false;
      else
        return true;
    }
    return false;
  }
  return true;
}

function checkIPv6Addr(ipStr)
{
  var ipRE  = new RegExp("^\s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])(.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])){3}))|:)))(%.+)?\s*$");
  if(ipStr[0]==':')
    return false;
  return ipRE.test(ipStr);
}

function checkIPv6McastAddr(ipStr)
{
  if (checkIPv6Addr(ipStr)) {
    if ((ipStr.charAt(0)=='f' ||ipStr.charAt(0)=='F') && (ipStr.charAt(1)=='f' ||ipStr.charAt(1)=='F') )
      return true;
    else
      return false;
  }
  return false;
}

function checkHost(hostStr)
{
  var label_array;
  var pattern = /^[a-zA-Z0-9\-.]*$/;

  return hostStr.match(pattern);
}

function checkDns(hostStr)
{
  var label_array;
  var hasChar = false;

  if (!checkHost(hostStr))
    return false;

  label_array = hostStr.split(".");
  for (var i =0; i< label_array.length;i++) {
    if (label_array[i].length > 63 || label_array[i].length == 0)
      return false;
    if (label_array[i].charAt(0) == "-" || label_array[i].charAt(label_array[i].length - 1) == "-")
      return false;
  }

  for (var i =0; i< hostStr.length;i++)
  {
    if ("." == hostStr[i])
      continue;

    hasChar |= isNaN(hostStr[i]);
  }

  //if (!hasChar)
  //  return false;

  return true;
}

function checkHexPair(hexPairStr, minPair, maxPair)
{
  var maxPairStr = IsUndefOrNull(maxPair) ? "" : (","+maxPair);
  var pairRE = new RegExp("^([0-9A-F]{2,2}){"+minPair+maxPairStr+"}$","i");
  return pairRE.test(hexPairStr);
}

function checkSnmpEngineId(engineStr)
{
  /* Need regular expression */
  if(engineStr.length < 10)
    return false;
  return true;
}

function checkMibOid(oidStr)
{
  oidStr=removeSpacesFromHeadAndTail(oidStr);
  var anum=/(^\d+$)/;
  var OIDArr=sliceString(oidStr,".");
  for (var i=0; i<OIDArr.length; i++)
  {
    if (0 == i)
    {
      if ("" != OIDArr[i])
        return false;
    }
    else
    {
      if (!anum.test(OIDArr[i]))
        return false;
    }
  }
  return true;
}

function checkPort(portStr, withPrefix, isPoe)
{
  var _isPoe = (!IsUndefOrNull(isPoe)&&isPoe);
  if(portStr.length == 0)
    return false;
  if (withPrefix) {
    if (isDevSupportFE(_top.deviceId)) {
      if (portStr.charAt(0) == 'F' && portStr.charAt(1) == 'E') {
        var port = portStr.split("FE");
        var portId = str2int(port[1]);
        if (_isPoe && (portId < 1 || portId > (_top.devPoePortNum)))
          return false;
        if (portId < 1 || portId > (_top.devPortNum - 2))
          return false;
      } else if (portStr.charAt(0) == 'G' && portStr.charAt(1) == 'E') {
        var port = portStr.split("GE");
        var portId = str2int(port[1]);
        if (_isPoe)
          return false;
        if (portId < 1 || portId > 2)
          return false;
      } else {
        return false;
      }
    } else {
      if (portStr.charAt(0) == 'G' && portStr.charAt(1) == 'E') {
        var port = portStr.split("GE");
        var portId = str2int(port[1]);
        if (_isPoe && (portId < 1 || portId > (_top.devPoePortNum)))
          return false;
        if (portId < 1 || portId > _top.devPortNum)
          return false;
      } else {
        return false;
      }
    }
  } else {
    var portId = str2int(portStr);
    if (_isPoe && (portId < 1 || portId > (_top.devPoePortNum)))
      return false;
    if (portId < 1 || portId > _top.devPortNum)
      return false;
  }
  return true;
}

function checkLag(lagStr, withPrefix)
{
  if (withPrefix) {
    if (lagStr.charAt(0) == 'L' && lagStr.charAt(1) == 'A' && lagStr.charAt(2) == 'G') {
      var lag = lagStr.split("LAG");
      var lagId = str2int(lag[1]);
      if (lagId < 1 || lagId > 8)
        return false;
    } else {
      return false;
    }
  } else {
    var lagId = str2int(lagStr);
    if (lagId < 1 || lagId > 8)
      return false;
  }
  return true;
}

function checkVlanList(str, vlanObj, withPrefix)
{
  if (str.length == 0)
    return false;
  if (withPrefix) {
    if (str.charAt(0) == 'V' && str.charAt(1) == 'L' && str.charAt(2) == 'A' && str.charAt(3) == 'N') {
      var vlan = str.split("VLAN");
      var vlanId = str2int(vlan[1]);
      if (vlanId < vlanObj.minVid || vlanId > vlanObj.maxVid)
        return false;
    } else {
      return false;
    }
  } else {
    var vlanId = str2int(str);
    if (vlanId < vlanObj.minEntry || vlanId > vlanObj.maxEntry)
      return false;
  }
  return true;
}

function validRangeStr(inputId, validMin, validMax)
{
  var inputIp = 0;
  var minIp = 0;
  var maxIp = 0;

  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  var arr = val.split(".");
  var minArr = validMin.split(".");
  var maxArr = validMax.split(".");

  for( var i=0; i< arr.length; i++)
  {
    inputIp += (str2int(arr[i]) << (24 - i * 8));
    minIp += (str2int(minArr[i]) << (24 - i * 8));
    maxIp += (str2int(maxArr[i]) << (24 - i * 8));
  }

  if (inputIp < minIp || inputIp > maxIp)
  {
    errMsgUpdate(inputId, lang('error','errOutOfRangeArg',[validMin, validMax]));
    return false;
  }

  return true;
}

function validRange(inputId, validMin, validMax, isTop, allowEmpty, isHex)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  var finalIsTop = (!IsUndefOrNull(isTop)) ? isTop : false;
  var finalallowEmpty = (!IsUndefOrNull(allowEmpty)) ? allowEmpty : false;

  if (!checkNotEmpty(val))
  {
    if (finalallowEmpty)
      return true;
    else
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), finalIsTop);
      return false;
    }
  }

  if (!IsUndefOrNull(isHex) && isHex)
  {
    /* a possible is that first char is hex follows are not. */
    if(val.match(/[0-9A-F]*/i) == val)
      val =  parseInt(val,16);
    else
      val = "";
  }
  else
  {
    /* a possible is that first char is digit follows are not. */
    if (val.match(/\d*/i)  == val)
      val =  parseInt(val);
    else
      val = "";
  }

  if (isNaN(val) || !checkRange(val, validMin, validMax))
  {
    if (isHex)
    {
      var min = "0x" + validMin.toString(16).toUpperCase();
      var max = "0x" + validMax.toString(16).toUpperCase();
      errMsgUpdate(inputId, lang('error','errOutOfRangeArg',[min, max]), finalIsTop);
    }
    else
      errMsgUpdate(inputId, lang('error','errOutOfRangeArg',[validMin, validMax]), finalIsTop);

    return false;
  }

  return true;
}

function validVlanRange(inputId, isTop, allowEmpty, allowDefault)
{
  var finalallowEmpty = (!IsUndefOrNull(allowEmpty)) ? allowEmpty : false;
  var finalallowDefault = (!IsUndefOrNull(allowDefault)) ? allowDefault : true;

  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (!checkNotEmpty(val))
  {
    if (finalallowEmpty)
      return true;
    else
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  }

  if (!finalallowDefault && (_top.defaultVid == val))
  {
    errMsgUpdate(inputId, lang('error','errOutOfRange'), isTop);
    return false;
  }

  if (isNaN(parseInt(val)) || !checkRange(val, 1, 4094))
  {
    errMsgUpdate(inputId, lang('error','errOutOfRange'), isTop);
    return false;
  }

  return true;
}

function validRequireName(inputId)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (checkNotEmpty(val))
  {

    return true;
  }
  else
  {
    errMsgUpdate(inputId, lang('error','errEmptyValue'));
    return false;
  }
}

function validSelString(inputId)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (null == val)
  {
    errMsgUpdate(inputId, lang('error','errEmptyValue'));
    return false
  }
  else
    return true;
}

function validString(inputId, isFilename, isSNMP, allowEmpty)
{
  var finalallowEmpty = (!IsUndefOrNull(allowEmpty)) ? allowEmpty : false;
  var finalisFilename = (!IsUndefOrNull(isFilename)) ? isFilename : false;
  var finalisSNMP = (!IsUndefOrNull(isSNMP)) ? isSNMP : false;

  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (!checkNotEmpty(val))
  {
    if (finalallowEmpty)
      return true;
    else
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'));
      return false;
    }
  }

  for (var i = 0; i < val.length ; i++)
  {
    var a = val.charCodeAt(i);

    if (a < 0x20 || a > 0x7E) // not in allow char range
    {
      errMsgUpdate(inputId, lang('error','errInvalidChar'));
      return false;
    }

    if (a == 0x3f || a == 0x22) // ? "
    {
      errMsgUpdate(inputId, lang('error','errInvalidChar'));
      return false;
    }
    else if (finalisSNMP)
    {
       //       \         ,
      if (a == 0x5c || a == 0x2c)
      {
        errMsgUpdate(inputId, lang('error','errInvalidChar'));
        return false;
      }
    }
    else if (finalisFilename)
    {
       //       \             /            :            *            <            >           |
      if (a == 0x5c || a == 0x2f || a == 0x3a || a == 0x2a || a == 0x3c || a == 0x3e || a == 0x7c)
      {
        errMsgUpdate(inputId, lang('error','errInvalidChar'));
        return false;
      }
    }
  }

  return true;
}

/* no use
function validDnsHostname(inputId, allowEmpty, isTop)
{
  var finalallowEmpty = (!IsUndefOrNull(allowEmpty)) ? allowEmpty : false;
  var finalisTop = (!IsUndefOrNull(isTop)) ? isTop : false;

  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (!checkNotEmpty(val))
  {
    if (finalallowEmpty)
      return true;
    else
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), finalisTop);
      return false;
    }
  }

  if (!checkHost(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidChar'), finalisTop);
    return false;
  }
  if (!checkDns(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidDnsName', [1, 63]), finalisTop);
    return false;
  }

  return true;
}

function validPortList(inputId, isTop, isPoe)
{
  if(isInputDisable(inputId))
    return true;
  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val))
  {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  val = val.replace(/\s/g, '').toUpperCase();

  var withPrefix = false;
  var i;
  for (i = 0; i < val.length; i++) {
    if((val.charAt(i) < '0' || val.charAt(i) > '9') && val.charAt(i) != ',' && val.charAt(i) != '-')
      withPrefix = true;
  }
  var list = val.split(',');
  var validMaxNum = (!IsUndefOrNull(isPoe)&&isPoe) ? _top.devPoePortNum : _top.devPortNum;
  for (i = 0; i < list.length; i++){
    var range = list[i].split('-');
    if (range.length == 1) {
      if (!checkPort(list[i], withPrefix, isPoe)) {
        if (withPrefix)
          errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        else
          errMsgUpdate(inputId, lang('error','errOutOfRangeArg', [1, validMaxNum]), isTop);
        return false;
      }
    } else if (range.length == 2) {
      if (!checkPort(range[0], withPrefix, isPoe)) {
        if (withPrefix)
          errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        else
          errMsgUpdate(inputId, lang('error','errOutOfRangeArg', [1, validMaxNum]), isTop);
        return false;
      }
      if (!checkPort(range[1], withPrefix, isPoe)) {
        if (withPrefix)
          errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        else
          errMsgUpdate(inputId, lang('error','errOutOfRangeArg', [1, validMaxNum]), isTop);
        return false;
      }
    } else {
      if (withPrefix)
        errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
      else
        errMsgUpdate(inputId, lang('error','errOutOfRangeArg', [1, validMaxNum]), isTop);
      return false;
    }
  }

  return true;
}


function validLagList(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;
  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val))
  {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  val = val.replace(/\s/g, '').toUpperCase();

  var withPrefix = false;
  var i;
  for (i = 0; i < val.length; i++) {
    if((val.charAt(i) < '0' || val.charAt(i) > '9') && val.charAt(i) != ',' && val.charAt(i) != '-')
      withPrefix = true;
  }
  var list = val.split(',');
  for (i = 0; i < list.length; i++){
    var range = list[i].split('-');
    if (range.length == 1) {
      if (!checkLag(list[i], withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        return false;
      }
    } else if (range.length == 2) {
      if (!checkLag(range[0], withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        return false;
      }
      if (!checkLag(range[1], withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
        return false;
      }
    } else {
      errMsgUpdate(inputId, lang('error','errInvalidValue'), isTop);
      return false;
    }
  }

  return true;
}

function validVlanList(inputId, vlanObj, isTop, allowEmpty)
{
  var finalallowEmpty = (!IsUndefOrNull(allowEmpty)) ? allowEmpty : false;

  if(isInputDisable(inputId))
    return true;
  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val))
  {
    if (finalallowEmpty)
    {
      return true;
    }
    else
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  }
  val = val.replace(/\s/g, '').toUpperCase();

  var withPrefix = false;
  var i;
  for (i = 0; i < val.length; i++) {
    if((val.charAt(i) < '0' || val.charAt(i) > '9') && val.charAt(i) != ',' && val.charAt(i) != '-')
      withPrefix = true;
  }

  var list = val.split(',');
  var minVal = (withPrefix)? vlanObj.minVid : vlanObj.minEntry;
  var maxVal = (withPrefix)? vlanObj.maxVid : vlanObj.maxEntry;
  for (i = 0; i < list.length; i++){
    var range = list[i].split('-');
    if (range.length == 1) {
      if (!checkVlanList(list[i], vlanObj, withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidVlanRangeArg',[minVal, maxVal]), isTop);
        return false;
      }
    } else if (range.length == 2) {
      if (!checkVlanList(range[0], vlanObj, withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidVlanRangeArg',[minVal, maxVal]), isTop);
        return false;
      }
      if (!checkVlanList(range[1], vlanObj, withPrefix)) {
        errMsgUpdate(inputId, lang('error','errInvalidVlanRangeArg',[minVal, maxVal]), isTop);
        return false;
      }
    } else {
      errMsgUpdate(inputId, vlanObj, lang('error','errInvalidVlanRangeArg',[minVal, maxVal]), isTop);
      return false;
    }
  }

  return true;
}


function validQosName(inputId)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (checkNotEmpty(val))
  {

    return true;
  }
  else
  {
    errMsgUpdate(inputId, lang('error','errEmptyValue'));
    return false;
  }
}
*/

function validMacAddr(inputId, isUnicast, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  if (!checkMacAddr(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidMacAddr'), isTop);
    return false;
  } else {
    if (!IsUndefOrNull(isUnicast) && isUnicast) {
      var tok = val.split(':');
      var mcastBit = parseInt(tok[0],16) & 1;
      if (mcastBit == 1) {
        errMsgUpdate(inputId, lang('error','errInvalidUnicastMacAddr'), isTop);
        return false;
      }
    }
  }

  return true;
}

function validMacAddrCheck(inputId, isUnicast)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    return false;
  }
  if (!checkMacAddr(val)) {
    return false;
  } else {
    if (!IsUndefOrNull(isUnicast) && isUnicast) {
      var tok = val.split(':');
      var mcastBit = parseInt(tok[0],16) & 1;
      if (mcastBit == 1) {
        return false;
      }
    }
  }
  return true;
}

function validIPv4Addr(inputId, isTop, allowEmpty, isPrefix)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if(inputId == 'adminGw' && val == 0)
  {
	  return true;
  }
  if (IsUndefOrNull(allowEmpty) || (!IsUndefOrNull(allowEmpty)&&allowEmpty==false))
  {
    if (!checkNotEmpty(val)) {
        errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
        return false;
    }
  }
  else
  {
    if (val.length == 0)
    {
      return true;
    }
  }
  if (!IsUndefOrNull(isPrefix) && isPrefix == true)
  {
    if (!checkIPv4Addr(val, false, false, true)) {
      errMsgUpdate(inputId, lang('error','errInvalidIpAddr'), isTop);
      return false;
    }
  } else {
    if (!checkIPv4Addr(val)) {
      errMsgUpdate(inputId, lang('error','errInvalidIpAddr'), isTop);
      return false;
    }
  }

  return true;
}

function validIPv4Mask(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  if (!checkIPv4Addr(val, false, true)) {
    errMsgUpdate(inputId, lang('error','errInvalidIpMask'), isTop);
    return false;
  }

  return true;
}

function validIPv4McastAddr(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  if (!checkIPv4Addr(val, true)) {
    errMsgUpdate(inputId, lang('error','errInvalidMulticastIpv4Addr'), isTop);
    return false;
  }

  return true;
}

function validIPv6Addr(inputId, isTop, allowEmpty)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (IsUndefOrNull(allowEmpty) || (!IsUndefOrNull(allowEmpty)&&allowEmpty==false))
  {
    if (!checkNotEmpty(val))
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  }
  else
  {
    if (val.length == 0)
    {

      return true;
    }
  }

  if (!checkIPv6Addr(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidIpAddr'), isTop);
    return false;
  }

  return true;
}

function validIPv6McastAddr(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  if (!checkIPv6Addr(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidMulticastIpv6Addr'), isTop);
    return false;
  }
  if (!checkIPv6McastAddr(val)) {
    errMsgUpdate(inputId, lang('error','errInvalidMulticastIpv6FirstGroup'), isTop);
    return false;
  }

  return true;
}

function validHexPair(inputId, minPairNum, maxPairNum, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val))
    return true;

  if (!checkHexPair(val, minPairNum, maxPairNum)) {
    if (IsUndefOrNull(maxPairNum))
      errMsgUpdate(inputId, lang('error','errValMustBePairHexChars',[minPairNum]), isTop);
    else
      errMsgUpdate(inputId, lang('error','errValMustBePairToPairHexChars',[minPairNum,maxPairNum]), isTop);
    return false;
  }

  return true;
}

function validSnmpEngineId(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }
  if (!checkSnmpEngineId(val)) {
    errMsgUpdate(inputId, lang('snmp','errNotEngineID'), isTop);
    return false;
  }

  return true;
}

function validMibOid(inputId, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }

  if (!checkMibOid(val)) {
    errMsgUpdate(inputId, lang('snmp','errMIBOIDNotValid'), isTop);
    return false;
  }

  return true;
}

function validSrvAddr(inputId, srvDef, allowEmpty, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (IsUndefOrNull(allowEmpty) || (!IsUndefOrNull(allowEmpty)&&allowEmpty==false)) {
    if (!checkNotEmpty(val)) {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  } else {
    if (val.length == 0) {

      return true;
    }
  }
  if ('ipv4' == srvDef) {
    if (!checkIPv4Addr(val) || checkIPv4Addr(val, true)) {
        errMsgUpdate(inputId, lang('error','errInvalidUnicastIpAddr'), isTop);
        return false;
      }
  } else if ('ipv6' == srvDef) {
    if (!checkIPv6Addr(val) || checkIPv6McastAddr(val)) {
      errMsgUpdate(inputId, lang('error','errInvalidIpAddr'), isTop);
      return false;
    }
  } else {
    if (!checkHost(val)) {
      errMsgUpdate(inputId, lang('error','errInvalidChar'), isTop);
      return false;
    }
    if (!checkDns(val)) {
      errMsgUpdate(inputId, lang('error','errInvalidDnsName', [1, 63]), isTop);
      return false;
    }
  }

  return true;
}

function validStpPri(inputId, validMin, validMax, validMulti, isTop)
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();
  if (!checkNotEmpty(val)) {
    errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
    return false;
  }

  if (!checkRange(val, validMin, validMax)) {
    errMsgUpdate(inputId, lang('error','errOutOfRangeArg',[validMin,validMax]), isTop);
    return false;
  }

  if (!checkMulti(val, validMulti)) {
    errMsgUpdate(inputId, lang('stp','errInvalidPriority',[validMin,validMax,validMulti]), isTop);
    return false;
  }


  return true;
}

function translateTime(val, hasSec)
{
  var time = "24hh:mm:ss";
  var ta;
  var va;
  var is12 = false;
  var isPM = false;
  var hour = null;
  var msg = "";

  ta = time.replace("24", "");

  if (ta.length == time.length)
  {
    ta = time.replace("12", "");
    is12 = true;
  }

  if (true == is12)
  {
    msg = val.replace("AM", "");

    if (msg.length == val.length)
    {
      msg = val.replace("PM", "");
      isPM = true;
    }

    if (msg.length == val.length)
      return "";

    val = msg;
  }
  else
  {
    msg = val.replace("AM", "");
    msg = val.replace("PM", "");

    if (msg.length != val.length)
      return "";
  }

  va = val.split(':');

  if (hasSec && va.length != 3)
    return "";

  if (!hasSec && va.length != 2)
    return "";

  if (is12)
  {
    if (isPM)
    {
      if (parseInt(va[0]) > 12 || parseInt(va[0]) == 0)
        return "";

      if (parseInt(va[0]) != 12)
        hour = parseInt(va[0]) + 12;
      else
        hour = parseInt(va[0]);
    }
    else if (parseInt(va[0]) > 11)
      return "";
    else
      hour = va[0];
  }
  else
    hour = va[0];

  if (hasSec)
    msg = hour + ":" + va[1] + ":" + va[2];
  else
    msg = hour + ":" + va[1];

  return $.trim(msg);
}

function translateDate(val)
{
  var monthNames = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
  var date = 'YYYY-MM-DD';
  var da;
  var va;
  var year = null;
  var month = null;
  var day = null;
  var msg = "";

  if (-1 != date.indexOf('/'))
  {
    da = date.split('/');
    va = val.split('/');
  }
  else
  {
    da = date.split('-');
    va = val.split('-');
  }

  if (da.length != va.length)
    return "";

  for (var i = 0; i < da.length; i++)
  {
    switch(da[i])
    {
      case 'YYYY':
        year = va[i];
        break;

      case 'YY':
        year = "20" + va[i];
        break;

      case 'MM':
        if (isNaN(va[i]))
          return "";

        month = va[i];
        break;

      case 'MMM':
        for (j = 0; j < monthNames.length; j++)
        {
          if (va[i] == monthNames[j])
          {
            month = j+1;
            break;
          }
        }

        if (month == null)
          return "";

        break;

      case "DD":
        day = va[i];
        break;

      default:
        return  "";
    }
  }

  msg = year + '-' + month + '-' + day;

  return $.trim(msg);
}

function validTimeHHMMSS(inputId, hasSec, allowEmpty, isTop) /* HH:MM:SS */
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (IsUndefOrNull(allowEmpty) || (!IsUndefOrNull(allowEmpty)&&allowEmpty==false))
  {
    if (!checkNotEmpty(val))
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  }
  else
  {
    if (val.length == 0)
    {

      return true;
    }
  }

  val = translateTime(val, hasSec);

  if (val == "")
  {
    errMsgUpdate(inputId, lang('time','errMsgTimeFormat'), isTop);
    return false;
  }

  var pattern;

  if (hasSec)
    pattern = /^(?:2[0-3]|[01]?[0-9]):[0-5]?[0-9]:[0-5]?[0-9]$/;
  else
    pattern = /^(?:2[0-3]|[01]?[0-9]):[0-5]?[0-9]$/;

  if (!pattern.test(val))
  {
    errMsgUpdate(inputId, lang('time','errMsgTimeFormat'), isTop);
    return false;
  }

  return true;
}

function validTimeYYYYMMDD(inputId, allowEmpty, isTop) /* YYYY/MM/DD */
{
  if(isInputDisable(inputId))
    return true;

  var val = $('#'+inputId).val();

  if (IsUndefOrNull(allowEmpty) || (!IsUndefOrNull(allowEmpty)&&allowEmpty==false))
  {
    if (!checkNotEmpty(val))
    {
      errMsgUpdate(inputId, lang('error','errEmptyValue'), isTop);
      return false;
    }
  }
  else
  {
    if (val.length == 0)
    {

      return true;
    }
  }

  val = translateDate(val);

  if (val == "")
  {
    errMsgUpdate(inputId, lang('time','errMsgTimeFormat'), isTop);
    return false;
  }

  var re = new RegExp("^([0-9]{4})[.-]{1}([0-9]{1,2})[.-]{1}([0-9]{1,2})$");
  var strDataValue;
  var infoValidation = true;
  var y;
  var m;
  var d;

  if ((strDataValue = re.exec(val)) != null)
  {
      y = parseFloat(strDataValue[1]);
      m = parseFloat(strDataValue[2]);
      d = parseFloat(strDataValue[3]);

      if (y <= 0 || y > 9999) // YYYY
          infoValidation = false;

      if (m <= 0 || m > 12) // mm
          infoValidation = false;

      if (d <= 0) // DD
        infoValidation = false;

      if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 ||  m == 10 || m == 12)
      {
        if (d > 31)
          infoValidation = false;
      }
      else if (m == 4 || m == 6 || m == 9 || m == 11)
      {
        if (d > 30)
          infoValidation = false;
      }
      else if (m == 2)
      {
        if (y % 4 == 0)
        {
          if (d > 29)
            infoValidation = false;
        }
        else if (d > 28)
          infoValidation = false;
      }
  }
  else
    infoValidation = false;

  if (!infoValidation)
  {
    errMsgUpdate(inputId, lang('time','errMsgDateFormat'), isTop);
    return false;
  }

  return true;
}

/* This ready flag should put at file end */
var ready_js_utility = 1;
