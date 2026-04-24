_top = self;
_opener = _top;
var isDialogOpen = false;

function str2var(str){if(IsUndefOrNull(str)||('' == str)){return '';}return eval('('+str+')');}
function IsUndefOrNull(arg) {return (typeof(arg) == "undefined" || arg == null);}
function SetNormalClass(node) {if (node.className != "btn_disabled") node.className = "btn_normal";}
function SetHoverClass(node) {if (node.className != "btn_disabled") node.className = "btn_hover";}
function SetPressedClass(node) {if (node.className != "btn_disabled") node.className = "btn_pressed";}
function setUserNameFocus(){document.forms[0].username.focus();}
function setPasswordFocus(){document.forms[0].password.focus();}
function showInfo(){if(infoMsg != ""){ShowStatusNotice(infoMsg,true);}}
function DisableButton(id){$('#'+id).prop('className', 'btn_disabled');}
function EnableButton(id, isDefault){if (IsUndefOrNull(isDefault)) $('#'+id).prop('className', 'btn_normal'); else $('#'+id).prop('className', 'btn_default');}

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

function _showStatusDialog(type, msg, isStick)
{
  var _isStick = (IsUndefOrNull(isStick) ? false : true);
  $().toastmessage('showToast', {
    text     : msg,
    sticky   : _isStick,
    position : 'top-right',
    type     : type,
    closeText: ''
  });
}

function ShowStatusSuccess(msg, isStick)
{
  if (IsUndefOrNull(_opener) || (!IsUndefOrNull(_opener) && _opener.isDialogOpen))
    _showStatusDialog("success", msg, isStick);
  else
    _opener._showStatusDialog("success", msg, isStick);
}

function ShowStatusNotice(msg, isStick)
{
  if (IsUndefOrNull(_opener) || (!IsUndefOrNull(_opener) && _opener.isDialogOpen))
    _showStatusDialog("notice", msg, isStick);
  else
    _opener._showStatusDialog("notice", msg, isStick);
}

function ShowStatusWarning(msg, isStick)
{
  if (IsUndefOrNull(_opener) || (!IsUndefOrNull(_opener) && _opener.isDialogOpen))
    _showStatusDialog("warning", msg, isStick);
  else
    _opener._showStatusDialog("warning", msg, isStick);
}

function ShowStatusError(msg, isStick)
{
  var _isStick = (IsUndefOrNull(isStick) ? true : isStick);

  if (IsUndefOrNull(_opener) || (!IsUndefOrNull(_opener) && _opener.isDialogOpen))
    _showStatusDialog("error", msg, _isStick);
  else
    _opener._showStatusDialog("error", msg, _isStick);
}

function languageSelect()
{
  $(document).ready(function(){
    $('#language_select').change(function(){
      var p1 = $(this).children('option:selected').val();
      if (p1 == "defLang_en" || p1 == "defLang_ru")
        set_cookie("cookie_language", p1);
      else
        set_cookie("cookie_language", "defLang_ru");
      window.location.reload();
    })
  })
}

function showLoginlogofunction(logo_name)
{
  var txt = '';
  
  txt += '<img style="padding-left:10px;"'
  txt += 'src="./images/logo/' + logo_name + '"'
  txt += '>'
  return txt;
}

function languageInitShow(langMode)
{
  var txt = '';
  var labels = {
    "defLang_ru": "Русский",
    "defLang_en": "English"
  };
  var order = ["defLang_ru", "defLang_en"];

  if (labels[langMode]) {
    txt += '<option value ="' + langMode + '">' + labels[langMode] + '</option>';
  }

  for (var i = 0; i < order.length; i++) {
    if (order[i] == langMode)
      continue;
    txt += '<option value ="' + order[i] + '">' + labels[order[i]] + '</option>';
  }

  return txt;
}

