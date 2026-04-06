function IsUndefOrNull(arg) {return (typeof(arg) == "undefined" || arg == null);}
function HasWord(str,word) {return (str.search(new RegExp("\\b"+word+"\\b")) != -1);}
function Consists(str,word) {return (str.search(new RegExp("^"+word+"$", "i")) != -1);}
function EncodeWindowName(str) {return (str + _top.STYLING.sessionId);}
function DecodeWindowName(str) {return (str.replace(_top.STYLING.sessionId, ""));}
function SetNormalClass(node) {if (node.className != "btn_disabled") node.className = "btn_normal";}
function SetHoverClass(node) {if (node.className != "btn_disabled") node.className = "btn_hover";}
function SetPressedClass(node) {if (node.className != "btn_disabled") node.className = "btn_pressed";}
function SetDefaultClass(node) {if (node.className != "btn_disabled") node.className = "btn_default";}
function isASCII(str) {return /^[\x00-\x7F]*$/.test(str);}

var _top;
var _opener;

function _set_opener()
{
  if (!document.getElementById('dialog_container')) {
    if (!IsUndefOrNull(self.parent) && !self.parent.RootFrame)
      _opener = self.parent;
    else
      _opener = null;
  } else {
    _opener = null;
  }
}

if (!IsUndefOrNull(self.RootFrame) && self.RootFrame == true)
{
  _top = self;
}
else
{
  if (!IsUndefOrNull(top.opener))
  {
    if (!IsUndefOrNull(top.opener.top.opener)) {
      _opener = top.opener;
      _top = top.opener.top.opener.top.parent;
    } else {
      _opener = top.opener;
      //_top = top.opener.top.parent;
		_top =top.parent;
    }
  }
  else
  {
    _top = top.parent;
    _set_opener();
  }
}

var treeFolder1Cnt = 0;
var treeDoc1Cnt = 0;
var treeFolder2Cnt = 0;
var treeDoc2Cnt = 0;
var treeFolder3Cnt = 0;
var treeDoc3Cnt = 0;
var selFolder1 ='';
var selDoc = '';
var treeImgClick = false;
var maxTreeEntry = 100;
function openLink(_url, _param)
{
  var __url = _url;
  var helpurl;
  if(!IsUndefOrNull(_url.split('?')[1]))
    helpurl = _top.$('a[name="'+_url.split('?')[0]+'"]').attr('helpurl');
  else
    helpurl = _top.$('a[name="'+_url+'"]').attr('helpurl');
  if (-1 == __url.indexOf('_ver')) {
    if(IsUndefOrNull(__url.split('?')[1]))
      __url += '?'+'_ver'+'='+_top.fileVer;
    else
      __url += '&'+'_ver'+'='+_top.fileVer;
  }
  _top.defPage = __url;
  if (!IsUndefOrNull(_param)) {
      __url += '&'+_param;
    _top.defPage = __url;
  }
  _top.$('#mainFrame').attr('src', __url);

  if (!IsUndefOrNull(helpurl))
    _top.defHelp = helpurl;

  _top.progressEnable();
}

function openTreeLink(_url, _param)
{
  var treeId;
  if(!IsUndefOrNull(_url.split('?')[1]))
    treeId = _top.$('a[name="'+_url.split('?')[0]+'"]').prop('id');
  else
    treeId = _top.$('a[name="'+_url+'"]').prop('id');
  if (IsUndefOrNull(treeId))
  {
    _top.defPage = _url;
    openLink(_url);
    return;
  }

  var strTok = treeId.split("_");
  var folder1Str = strTok[0];
  var folder2Str ='';
  var doc1Str ='';
  var doc2Str ='';

  if (-1 == strTok[1].indexOf('f'))
  {
    doc1Str = treeId;
  }
  else
  {
    folder2Str = strTok[0] + '_' + strTok[1];
    doc2Str = treeId;
  }

  if (folder1Str != _top.selFolder1)
  {
    if ('' != _top.selFolder1)
    {
      td = _top.$('#'+_top.selFolder1).find('td');
      span = _top.$('#'+_top.selFolder1).find('span');
      img = _top.$('#'+_top.selFolder1+'img');

      td.addClass('drwTD2');
      td.removeClass('selectedDrw');
      span.css('color', '');
      span.css('font-weight', '');

      img.prop('src', 'images/drawerTriangleOpenT.gif');
    }

    img = _top.$('#'+folder1Str+'img');

    if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
    {
      /* show all Layer 1 documents */
      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = _top.$('#'+folder1Str+'_'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
      }
      /* show all layer 2 folders */
      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = _top.$('#'+folder1Str+'_f'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
      }
    }

    td = _top.$('#'+folder1Str).find('td');
    span = _top.$('#'+folder1Str).find('span');

    td.addClass('selectedDrw');
    td.removeClass('drwTD2');
    span.css('color', treeSelColor);
    span.css('font-weight', 'bold');
    img.prop('src', 'images/drawerTriangleOpenSelected.gif');

    _top.selFolder1 = folder1Str;
  }

  if ('' != _top.selDoc)
  {
    a = _top.$('#'+_top.selDoc);
    a.css('color', '');
    a.css('font-weight', '');
    a.css('background-color', '');
  }

  if ('' != doc1Str)
  {
    a = _top.$('#'+doc1Str);
    a.css('color', treeSelColor);
    a.css('font-weight', treeSelDocFontWeight);
    a.css('background-color', treeSelBackground);
    _top.selDoc = doc1Str;
  }
  else
  {
    a = _top.$('#'+doc2Str);
    a.css('color', treeSelColor);
    a.css('font-weight', treeSelDocFontWeight);
    a.css('background-color', treeSelBackground);
    _top.selDoc = doc2Str;

    img = _top.$('#'+folder2Str+'img');

    if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
    {
      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = _top.$('#'+folder2Str+'_'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
      }
      img.prop('src', 'images/drawerTriangleOpenT.gif');
    }
  }

  openLink(_url, _param);
  _top.refreshNavigator();
}

function bodyResize(){
  var tblWidth = $('#main_table').width();
  var mainTableWidth = _top.$('#mainTable').width();
  var body = $('body');
  if (tblWidth > mainTableWidth) {
    body.css('width',tblWidth+'px');
    body.css('padding-right','10px');
  } else {
    body.css('width','');
    body.css('padding-right','0px');
  }
}

var enterKeyEventStr = null;
function EnterKeyEvent(funcStr, frameStr)
{
  if(IsUndefOrNull(enterKeyEventStr)) {
    EnterKeyEventRegister();
  }
  enterKeyEventStr = '';
  enterKeyEventStr += (IsUndefOrNull(frameStr) ? "mainFrame" : frameStr);
  enterKeyEventStr += '.' + funcStr + '()';
}
function EnterKeyEventRegister()
{
  $(document).keypress(function(event){
    var keycode = (event.keyCode ? event.keyCode : event.which);
    if(keycode == '13'){
      if (enterKeyEventStr) {
        str2var(enterKeyEventStr);
      }
    }
  });
  // prevent enter key refresh page
  $(function() {
    $("form").submit(function() { return false; });
  });
}
function EnterKeyEventUnregister()
{
  $(document).unbind('keypress');
}
function PagePostInit()
{
  if (isIE() < 9) {
    _set_opener();
  }

  HandleTables();
  HandleControls();

  if (!IsUndefOrNull($('#main_table'))) {
    bodyResize();
    $('body')[0].onresize = bodyResize;
  }

  _top.progressDisable();
}

var pageSelVal = -1;
var pageVal = 0;
function pageSelChg(val)
{
  pageSelVal = val;
}
function HandleTables()
{
  if (HasWord(navigator.userAgent, "Safari") && !HasWord(navigator.userAgent, "Chrome"))
  {
    var captionArr = document.getElementsByTagName("caption");
    for (var i = 0; i < captionArr.length; i++)
    {
      if (HasWord(captionArr[i].parentNode.className, "dataTable"))
      {
        captionArr[i].className += " safari";
        if (captionArr[i].children[0])
          captionArr[i].children[0].className += " safari";
      }
    }
  }

  if ($('.dataTable').length == 0)
    return;

  if (HasWord($('.dataTable').attr('class'), "func")) {
    $('.dataTable').find('tbody').find('tr').each(function(idx){
      if (idx % 2 == 0) {
        $(this).prop('className','odd');
      } else {
        $(this).prop('className','even');
      }
    });
    return;
  }

  function _is_need_paging() {
    var classStr = $('.dataTable').attr('class');
    if (!IsUndefOrNull(classStr) && HasWord(classStr, "paging")) {
      return true;
    }
    return false;
  }

  function _is_first_column_sort() {
    var classStr = $('.dataTable .selAllCheckbox').attr('class');
    if (!IsUndefOrNull(classStr) && HasWord(classStr, "selAllCheckbox")) {
      return false;
    }
    if ($('.dataTable input[type=radio]').length != 0 ) {
      return false;
    }
    return true;
  }

  $('.dataTable').dataTable({
    "bJQueryUI": false,
    "bPaginate": _is_need_paging(),
    "bInfo":_is_need_paging(),
    "bSort":true,
    "sDom":'<lif>t<p>',
    "sPaginationType": "full_numbers",
    "bStateSave": false,
    "oLanguage": {
      "sEmptyTable": lang('common','lblNoResultFound'),
      "sSearch": "<img style='position:relative;top:6px;' src='../images/search.png'>",
      "oPaginate":{
        "sFirst":"First",
        "sLast":"Last",
        "sNext":"Next",
        "sPrevious":"Previous"
      },
      "sLengthMenu": lang('common','lblPageLeftCombo')+' <select id="pageSel" onchange="pageSelChg(this.value);">'+
        '<option value="-1">All</option>'+
        '<option value="10">10</option>'+
        '<option value="30">30</option>'+
        '<option value="50">50</option>'+
        '<option value="100">100</option>'+
        '</select> '+lang('common','lblPageRightCombo'),
        "sInfo": "Showing _START_ to _END_ of _TOTAL_ entries",
      "sInfoFiltered": " (filtered from _MAX_ total entries)"

    },
    "sEmptyTable": "There are no records",
    "iDisplayLength" : pageSelVal,
    "iDisplayStart" : pageVal,
    "aoColumnDefs" : [{
      "bSortable" : _is_first_column_sort(),
      "bSearchable" : _is_first_column_sort(),
      "aTargets" : [ 0 ]
    }]
  });
  $('.dataTable').on( 'draw.dt', function () {
    var oTable = $('.dataTable').dataTable();
    var oSettings = oTable.fnSettings();
    pageSelVal = oSettings._iDisplayLength;
  } );
  $('.dataTable').on( 'page.dt', function () {
    var oTable = $('.dataTable').dataTable();
    var oSettings = oTable.fnSettings();
    pageVal = oSettings._iDisplayStart;
  } );
  $('.dataTable tbody td').click( function() {
    if (0 != $(this).parent().parent().find('td').find('input[type=checkbox]').length) {
      RefreshTableMultiSelect(this);
    } else if (0 != $(this).parent().parent().find('td').find('input[type=radio]').length) {
      $('.dataTable').find('tbody').find('tr').removeClass('row_selected');
      $('.dataTable').find('tbody').find('tr').find('td').find('input[type=radio]').prop('checked', false);
      $(this).parent().addClass('row_selected');
      $(this).parent().find('input[type=radio]').prop('checked', true);
    }
  });
  $('.dataTable .selAllCheckbox').click( function(){
    RefreshTableMultiSelect(this, true);
  });
  copyTableSelArry = null;
}

function inputText_digitonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90) && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      // Ensure that it is a number and stop the keypress
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_oidonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || event.keyCode == 110 || event.keyCode == 190 ||/*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v, '.'
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90) && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      // Ensure that it is a number and stop the keypress
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_timeonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90) && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* : */
      if ((event.keyCode == 186 && event.shiftKey) || (isFF && event.keyCode == 59 && event.shiftKey) ) {
        return;
      }

      // Ensure that it is a number and stop the keypress
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_dateonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90) && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* - */
      if (event.keyCode == 189 || (isFF && event.keyCode == 173)) {
        return;
      }

      // Ensure that it is a number and stop the keypress
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_hexonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90)  && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* ABCDEF */
      if ((event.keyCode >= 65 && event.keyCode <= 70) && event.shiftKey) {
        return;
      }

      // Ensure that it is a number or abcdef and stop the keypress
      if (event.shiftKey || ((event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 65 || event.keyCode > 70)) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_maconly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90)  && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* : */
      if ((event.keyCode == 186 && event.shiftKey) || (isFF && event.keyCode == 59 && event.shiftKey) ) {
        return;
      }

      /* ABCDEF */
      if ((event.keyCode >= 65 && event.keyCode <= 70) && event.shiftKey) {
        return;
      }

      // Ensure that it is a number or abcdef and stop the keypress
      if (event.shiftKey || ((event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 65 || event.keyCode > 70)) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_ipv4only(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90)  && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* . */
      if (event.keyCode == 110 || event.keyCode == 190) {
        return;
      }
      // Ensure that it is a number and stop the keypress
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_ipv6only(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A, Ctrl+z, Ctrl+x, Ctrl+c, Ctrl+v
      ((event.keyCode == 65 || event.keyCode == 67 || event.keyCode == 86 || event.keyCode == 88 || event.keyCode == 90)  && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
      /* . */
      if (event.keyCode == 110 || event.keyCode == 190) {
        return;
      }
      /* : */
      if ((event.keyCode == 186 && event.shiftKey) || (isFF && event.keyCode == 59 && event.shiftKey) ) {
        return;
      }
      // Ensure that it is a number or abcdef and stop the keypress
      if ((event.shiftKey || event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 65 || event.keyCode > 70) && (event.keyCode < 96 || event.keyCode > 105 )) {
          event.preventDefault();
      }
  }
}

function inputText_hostonly(event) {
  if ( event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || /*event.keyCode == 13 ||*/
       // Allow: Ctrl+A
      (event.keyCode == 65 && event.ctrlKey === true) ||
       // Allow: home, end, left, right
      (event.keyCode >= 35 && event.keyCode <= 39)) {
           // let it happen, don't do anything
           return;
  }
  else {
    /* 0-9 */
    if (((event.keyCode >= 48 && event.keyCode <=57) && !event.shiftKey) || (event.keyCode >= 96 && event.keyCode <=105)) {
      return;
    }
    /* a-zA-Z */
    if (event.keyCode >= 65 && event.keyCode <=90) {
      return;
    }
    /* - */
    if (event.keyCode == 189 || event.keyCode == 109 || (isFF && event.keyCode == 173)) {
      return;
    }
    /* . */
    if (event.keyCode == 110 || event.keyCode == 190) {
      return;
    }
    event.preventDefault();
  }
}

function inputText_common(event) {
  if ( event.keyCode != 13 ) {
    return;
  }
  else {
    event.preventDefault();
  }
}

function HandleControls()
{
  isSomeControlChanged = false;
  selectCtrlTableHash = {};
  ctrlArray = [];
  var ctrlArr = document.getElementsByTagName("input");
  var ctrlNameArr = new Array();
  var ctrlName, ctrlOnclick, isMultiselect;
  var isMSIE = HasWord(navigator.userAgent, "MSIE");
  var isStandardMode = (document.compatMode == "CSS1Compat");
  var chkClass = " checkbox";
  var rdoClass = " radio";
  var txtClass = " text";
  if (isStandardMode)
  {
  document.body.style.overflow = "visible";
  if (document.getElementsByTagName("html").length)
  document.getElementsByTagName("html")[0].style.overflow = "auto";
  chkClass = " checkboxStd";
  rdoClass = " radioStd";
  txtClass = " textStd";
  }
  for (var i = 0; i < ctrlArr.length; i ++)
  {
  if (ctrlArr[i].type == "hidden")
  continue;
  if (IsUndefOrNull(ctrlArr[i].id) || ctrlArr[i].id == "")
  {
  ctrlArray.push(ctrlArr[i]);
  }
  switch (ctrlArr[i].type)
  {
  case "checkbox":
  ctrlArr[i].className += chkClass;
  if (isMSIE & isStandardMode)
  ctrlArr[i].style.marginBottom = "0px";
  break;
  case "radio":
  ctrlArr[i].className += rdoClass;
  if (isMSIE)
  {
  ctrlArr[i].style.width = "13px";
  ctrlArr[i].style.height = "14px";
  ctrlArr[i].style.marginLeft = "-1px";
  if (isStandardMode)
  {
  ctrlArr[i].style.marginBottom = "0px";
  ctrlArr[i].style.marginTop = "1px";
  }
  }
  break;
  case "file":
  case "password":
  case "text":
  if (ctrlArr[i].disabled)
    ctrlArr[i].className += txtClass + " disabled";
  else
    ctrlArr[i].className += txtClass;
  if (isMSIE)
  ctrlArr[i].style.margin = "-1px 0px";
  break;
  }
  }
  var tblHash;
  for (ctrlName in selectCtrlTableHash)
  {
  tblHash = selectCtrlTableHash[ctrlName];
  if (tblHash.selectAllCtrl)
  tblHash.selectAllCtrl.disabled = !(tblHash.selectCtrlHashLength - tblHash.disabledCount);
  }
  var tagNameArr = ["select", "textarea"];
  var corrTextarea;
  for (var i = 0; i < tagNameArr.length; i ++)
  {
  corrTextarea = (isMSIE && tagNameArr[i] == "textarea");
  ctrlArr = document.getElementsByTagName(tagNameArr[i]);
  for (var j = 0; j < ctrlArr.length; j ++)
  {
  if (IsUndefOrNull(ctrlArr[j].id) || ctrlArr[j].id == "")
  {
  ctrlArray.push(ctrlArr[j]);
  }
  if (corrTextarea)
  ctrlArr[j].style.margin = "-1px 0px";
  }
  }

  /* Find input text with digitonly attribute and register keydown event */
  $('input[type=text]').each(function(idx){
    if(!IsUndefOrNull($(this).attr('digitonly')))
    {
      $(this).keydown(inputText_digitonly);
    }
    else if(!IsUndefOrNull($(this).attr('oidonly')))
    {
      $(this).keydown(inputText_oidonly);
    }
    else if(!IsUndefOrNull($(this).attr('timeonly')))
    {
      $(this).keydown(inputText_timeonly);
    }
    else if(!IsUndefOrNull($(this).attr('dateonly')))
    {
      $(this).keydown(inputText_dateonly);
    }
    else if(!IsUndefOrNull($(this).attr('hexonly')))
    {
      $(this).keydown(inputText_hexonly);
    }
    else if(!IsUndefOrNull($(this).attr('maconly')))
    {
      $(this).keydown(inputText_maconly);
    }
    else if(!IsUndefOrNull($(this).attr('hostonly')))
    {
      $(this).keydown(inputText_hostonly);
    }
    else if(!IsUndefOrNull($(this).attr('ipv4only')))
    {
      $(this).keydown(inputText_ipv4only);
    }
    else if(!IsUndefOrNull($(this).attr('ipv6only')))
    {
      $(this).keydown(inputText_ipv6only);
    }
    else if(!IsUndefOrNull($(this).attr('ascii')))
    {
      $(this).keydown(inputText_common);
    }
    $(this).addClass('inputTextErrMsg');
  });
  $('input[type=password]').each(function(idx){
    $(this).addClass('inputTextErrMsg');
  });

  /* IE9 and IE8 may not implement maxlength on textarea tag, implement here */
  if (isIE() && isIE() < 10) {
    var txts = document.getElementsByTagName('TEXTAREA')

    for(var i = 0, l = txts.length; i < l; i++) {
      if(/^[0-9]+$/.test(txts[i].getAttribute("maxlength"))) {
        var input_func = function() {
          var len = parseInt(this.getAttribute("maxlength"), 10);

          if(this.value.length > len) {
            this.value = this.value.substr(0, len);
            return false;
          }
        }
        var keypress_func = function() {
          var len = parseInt(this.getAttribute("maxlength"), 10);
          e = event || window.event;
          code = e.charCode || e.keyCode;

          if((code == 8 || code == 46) || (code >= 37 && code <= 40))
            return true;
          else
            return (len > this.value.length);
        }
        var paste_func = function() {
          var len = parseInt(this.getAttribute("maxlength"), 10);

          if ((this.value.length + window.clipboardData.getData('Text').length) > len) {
            if (document.selection) {
              this.focus();
              sel = document.selection.createRange();
              var sel_len = len + sel.text.length - this.value.length;
              sel.text = window.clipboardData.getData('Text').substr(0, sel_len);
            }
            else if (this.selectionStart || this.selectionStart == 0) {
              var sel_len = len + (this.selectionEnd - this.selectionStart) - this.value.length;
              this.value = this.value.substring(0, this.selectionStart) +
                           window.clipboardData.getData('Text').substr(0, sel_len) +
                           this.value.substring(this.selectionEnd, this.value.length);

            }
            else {
              this.value += window.clipboardData.getData('Text').substr(0, len - this.value.length);
            }
            return false;
          }
        }

        txts[i].onkeypress = keypress_func;
        txts[i].onpaste = paste_func;
        txts[i].onblur = input_func;
      }
    }
  }

  return;
}

function ClearStatusContainer(statusId)
{
  if (IsUndefOrNull(statusId))
    $('#status_container').html('');
  else
    $('#'+statusId).html('');
}

function ChageServerInputType(inputId, type)
{
  var _type = (IsUndefOrNull(type)) ? "host" : type;
  if ('ipv4' == _type) {
    $('#'+inputId).unbind('keydown');
    $('#'+inputId).keydown(inputText_ipv4only);
  } else if ('ipv6' == _type) {
    $('#'+inputId).unbind('keydown');
    $('#'+inputId).keydown(inputText_ipv6only);
  } else if ('host' == _type) {
    $('#'+inputId).unbind('keydown');
    $('#'+inputId).keydown(inputText_hostonly);
  }
}

function _CreateButton(txt, id, width, isDisabled, isDefault, optClass)
{
  /* Do not show button if user privilege is 1 */
  if (!IsUndefOrNull(_top.data.priv) && _top.data.priv == 1)
    return "";

  if (IsUndefOrNull(isDefault))
    isDefault = false;
  var btnLiteral = "<table onmousedown='SetPressedClass(this)' onmouseup='SetHoverClass(this)'";
  if (!IsUndefOrNull(id) && id != "")
    btnLiteral += " id='"+id+"'";

  btnLiteral += " onmouseover='SetHoverClass(this)'";
  if (!isDefault)
  {
    btnLiteral += " onmouseout='SetNormalClass(this)'";
    if (!IsUndefOrNull(isDisabled) && isDisabled)
      btnLiteral += " class='btn_disabled'";
    else
      btnLiteral += " class='btn_normal'";
  }
  else
  {
    btnLiteral += " onmouseout='SetDefaultClass(this)' class='btn_default'";
    id = "defaultButton";
  }
  var minWidth = _top.STYLING.buttons.getMinWidth();
  if (IsUndefOrNull(width) || !Consists(typeof(width), "number") || width < minWidth)
    width = minWidth;
  btnLiteral += "><tbody><tr><td class='btnTD1' width='9'><img alt='' src='../images/empty.gif' width='9' height='1' /></td>";
  btnLiteral += "<td class='btnTD2' width='"+(width-minWidth)+"'";
  if (!IsUndefOrNull(id) && id != "")

  btnLiteral += "'><a href='javascript:void(0)'";
  if (!IsUndefOrNull(optClass))
    btnLiteral += " class='"+optClass+"'";
  btnLiteral += " id='"+id+"_tdNode'";
  //if (!IsUndefOrNull(id) && id != "")
  //btnLiteral += " id='"+id+"'";
  if (IsUndefOrNull(txt))
  txt = "";
  btnLiteral += " title='"+txt.replace(/\.{3}$/, "")+"'>"+txt+"</a></td>";
  btnLiteral += "<td class='btnTD3' width='9'><img alt='' src='../images/empty.gif' width='9' height='1' /></td></tr></tbody></table>";
  return btnLiteral;
}

function ButtonShort(txt, id, isDisabled, isDefault, optClass)
{
  var btnWidth = _top.STYLING.buttons.getShortWidth();
  return _CreateButton(txt, id, btnWidth, isDisabled, isDefault, optClass);
}

function ButtonLong(txt, id, isDisabled, isDefault, optClass)
{
  var btnWidth = _top.STYLING.buttons.getLongWidth();
  return _CreateButton(txt, id, btnWidth, isDisabled, isDefault, optClass);
}

function Button(txt, id, width, isDisabled)
{
	return _CreateButton(txt, id, width, isDisabled);
}
function ButtonApply(id)
{
	return ButtonShort(lang('common','btnApply'), id, false, false);
}

function ButtonCancel(id)
{
	return ButtonShort(lang('common','btnCancel'), id);
}

function ButtonAdd(id, isDisabled)
{
	return ButtonShort(lang('common','btnAdd'), id, isDisabled);
}
function ButtonEdit(id, isDisabled)
{
	return ButtonShort(lang('common','btnEdit'), id, isDisabled, false, 'edit');
}
function ButtonDel(id, isDisabled)
{
  return ButtonShort(lang('common','btnDelete'), id, isDisabled, false, 'del');
}
function ButtonClose(id, isDisabled)
{
  return ButtonShort(lang('common','btnClose'), id, isDisabled);
}
function ButtonClear(id, isDisabled)
{
  return ButtonShort(lang('common','btnClear'), id, isDisabled);
}
function ButtonRefresh(id, isDisabled)
{
  return ButtonShort(lang('common','btnRefresh'), id, isDisabled);
}
function ButtonDetail(id, isDisabled)
{
  return ButtonShort(lang('common','btnDetail'), id, isDisabled);
}
function ButtonActive(id, isDisabled)
{
  return ButtonShort(lang('common','btnActive'), id, isDisabled);
}

function ButtonDeactive(id, isDisabled)
{
  return ButtonShort(lang('common','btnDeactive'), id, isDisabled);
}

function ButtonBind(id, isDisabled)
{
  return ButtonShort(lang('common','btnBind'), id, isDisabled);
}

function ButtonUnbind(id, isDisabled)
{
  return ButtonShort(lang('common','btnUnbind'), id, isDisabled);
}

function ButtonGroupApply(applyId)
{
  var btnGrp = '<table class="formButtons"><tr>'
  btnGrp += '<td class="groupTblSpacing">'+ButtonApply(applyId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupApplyClose(applyId, closeId)
{
  var btnGrp = '<table class="formButtons"><tr>'
  btnGrp += '<td class="groupTblSpacing">'+ButtonApply(applyId)+'</td>';
  btnGrp += '<td class="groupTblSpacing">'+ButtonClose(closeId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupApplyCancel(applyId, cancelId)
{
  var btnGrp = '<table class="formButtons"><tr>'
  btnGrp += '<td class="groupTblSpacing">'+ButtonApply(applyId)+'</td>';
  btnGrp += '<td class="groupTblSpacing">'+ButtonCancel(cancelId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupApplyActive(applyId, activeId)
{
  var btnGrp = '<table class="formButtons"><tr>'
  btnGrp += '<td class="groupTblSpacing">'+ButtonApply(applyId)+'</td>';
  btnGrp += '<td class="groupTblSpacing">'+ButtonActive(activeId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupAddEditDel(addId, editId, delId)
{
  var btnGrp = '<table><tr>'
  btnGrp += '<td>'+ButtonAdd(addId)+'</td>';
  if (!IsUndefOrNull(editId))
    btnGrp += '<td>'+ButtonEdit(editId)+'</td>';
  btnGrp += '<td>'+ButtonDel(delId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupAddDel(addId, delId)
{
  var btnGrp = '<table><tr>'
  btnGrp += '<td>'+ButtonAdd(addId)+'</td>';
  btnGrp += '<td>'+ButtonDel(delId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroupClearRefresh(clearId, refreshId)
{
  var btnGrp = '<table class="formButtons"><tr>'
  btnGrp += '<td class="groupTblSpacing">'+ButtonClear(clearId)+'</td>';
  btnGrp += '<td class="groupTblSpacing">'+ButtonRefresh(refreshId)+'</td>';
  btnGrp += '</tr></table>';

  return btnGrp;
}

function ButtonGroup()
{
  var i = 0;
  var btnGrp = '<table class="formButtons"><tr>'

  while(!IsUndefOrNull(arguments[i]))
  {
    btnGrp += '<td class="groupTblSpacing">'+ arguments[i] +'</td>';
    i++;
  }

  btnGrp += '</tr></table>';

  return btnGrp;
}


function EnableButton(id, isDefault)
{
	if (IsUndefOrNull(isDefault))
      $('#'+id).prop('className', 'btn_normal');
    else
      $('#'+id).prop('className', 'btn_default');
}
function DisableButton(id)
{
  $('#'+id).prop('className', 'btn_disabled');
}

function _CreateButtonNextBackTransfer(id, width, btnClass, title, isDisable, onclick, parentId)
{
  var btnLiteral = "<table id='"+id+"' onmousedown='SetPressedClass(this)' onmouseup='SetHoverClass(this)'";
  btnLiteral += " onmouseover='SetHoverClass(this)'";
  btnLiteral += " onmouseout='SetNormalClass(this)'";
  if (!IsUndefOrNull(isDisable) && isDisable == true)
    btnLiteral += " class='btn_disabled'";
  else
    btnLiteral += " class='btn_normal'";
  if (!IsUndefOrNull(onclick))
  btnLiteral += " onclick=\"if(this.className!='btn_disabled'){"+onclick+"}\"";
  if (IsUndefOrNull(width) || !Consists(typeof(width), "number") || width < 24)
  width = 24;
  btnLiteral += " width='"+width+"'";
  if (!IsUndefOrNull(title))
  btnLiteral += " title="+title;
  btnLiteral += "><tbody><tr>";
  btnLiteral += "<td class="+btnClass+" width='"+width+"' ";
  if (!IsUndefOrNull(id) && id != "")
  btnLiteral += " id='"+id+"_tdNode'>";
  btnLiteral +="</td>";
  btnLiteral += "</td>";
  btnLiteral += "</tr></tbody></table>";
  if (IsUndefOrNull(parentId))
    return btnLiteral;
  //document.write(btnLiteral);
  else
  document.getElementById(parentId).innerHTML = btnLiteral;
  function CheckKeyPress(ev)
  {
  if (GetEventKeyCode(ev) == 13)
  {
  var target = GetEventTarget(ev);
  if (IsUndefOrNull(target.tagName))
  return false;
  switch (target.tagName.toLowerCase())
  {
  case "a":
  case "area":
  case "button":
  case "textarea":
  break;
  case "input":
  if (IsUndefOrNull(target.type))
  return false;
  switch (target.type.toLowerCase())
  {
  case "button":
  case "file":
  case "image":
  case "reset":
  case "submit":
  break;
  default:
  PressDefaultButton();
  }
  break;
  default:
  PressDefaultButton();
  }
  }
  return true;
  }
}

function ButtonTransfer(idAdd, idRemove)
{
  var btnLiteral = "<table><tr><td>";
  btnLiteral += _CreateButtonNextBackTransfer(idAdd, 24, "btnTransformNext", lang('common','tipButtonTransAdd'));
  btnLiteral += "</td></tr><tr><td class=transferBtnsSpacing>";
  btnLiteral += _CreateButtonNextBackTransfer(idRemove, 24, "btnTransformBack", lang('common','tipButtonTransRem'));
  btnLiteral += "</td></tr></table>";
  return btnLiteral;
}

function ButtonTransferDisabled(idAdd, idRemove)
{
  var btnLiteral = "<table><tr><td>";
  btnLiteral += _CreateButtonNextBackTransfer(idAdd, 24, "btnTransformNext", lang('common','tipButtonTransAdd'), true);
  btnLiteral += "</td></tr><tr><td class=transferBtnsSpacing>";
  btnLiteral += _CreateButtonNextBackTransfer(idRemove, 24, "btnTransformBack", lang('common','tipButtonTransRem'), true);
  btnLiteral += "</td></tr></table>";
  return btnLiteral;
}

function ButtonGroupUpDown(idAdd, idRemove, upTip, downTip)
{
  var btnLiteral = "<table><tr><td>";
  btnLiteral += _CreateButtonNextBackTransfer(idAdd, 24, "btnTransformUp", upTip);
  btnLiteral += "</td></tr><tr><td class=transferBtnsSpacing>";
  btnLiteral += _CreateButtonNextBackTransfer(idRemove, 24, "btnTransformDown", downTip);
  btnLiteral += "</td></tr></table>";
  return btnLiteral;
}

function ButtonGroupUpDownDisabled(idAdd, idRemove, upTip, downTip)
{
  var btnLiteral = "<table><tr><td>";
  btnLiteral += _CreateButtonNextBackTransfer(idAdd, 24, "btnTransformUp", upTip, true);
  btnLiteral += "</td></tr><tr><td class=transferBtnsSpacing>";
  btnLiteral += _CreateButtonNextBackTransfer(idRemove, 24, "btnTransformDown", downTip, true);
  btnLiteral += "</td></tr></table>";
  return btnLiteral;
}

function IsButtonDisabled(id)
{
  if('btn_disabled' == $('#'+id).prop('className'))
    return true;
  else
    return false;
}

function changeButtonTitle(id, msg)
{
    $('#'+id+'_tdNode').attr('title', msg);
}

function MultiSelectApplySelect(idRight, idLeft)
{
  if(!IsUndefOrNull(idRight))
  {
    $('#'+idRight).find("option").prop('selected', 'selected');
  }
  if(!IsUndefOrNull(idLeft))
  {
    $('#'+idLeft).find("option").prop('selected', 'selected');
  }
}
function MultiSelectDeselectAll(id1, id2)
{
  if(!IsUndefOrNull(id1))
  {
    $('#'+id1).find("option").prop('selected', '');
  }
  if(!IsUndefOrNull(id2))
  {
    $('#'+id2).find("option").prop('selected', '');
  }
}

function MultiSelectTransferUpdate(idClick, idAdd, idRemove, idLeft, idRight)
{
  if(idClick == idLeft)
  {
    if('btn_disabled' == $('#'+idAdd).prop('className'))
      $('#'+idAdd).prop('className', 'btn_normal');
  }
  else if (idClick == idRight)
  {
    if('btn_disabled' == $('#'+idRemove).prop('className'))
      $('#'+idRemove).prop('className', 'btn_normal');
  }
  else if (idClick == idAdd)
  {
    if (!isIE() || !(isIE() == 9)) // for sahi ie9
    {
      if('btn_disabled' == $('#'+idAdd).prop('className'))
        return;
    }

    var selItems = $('#'+idLeft).find(":selected").prop('selected', '').remove();
    $('#'+idRight).append(selItems);

    $('#'+idAdd).prop('className', 'btn_disabled');
  }
  else if (idClick == idRemove)
  {
    if (!isIE() || !(isIE() == 9)) // for sahi ie9
    {
      if('btn_disabled' == $('#'+idRemove).prop('className'))
        return;
    }

    var selItems = $('#'+idRight).find(":selected").prop('selected', '').remove();
    $('#'+idLeft).append(selItems);

    $('#'+idRemove).prop('className', 'btn_disabled');
  }
}

function _CreateLabel(txt, className, htmlFor, id, isRequired, isSeperator)
{
  var lblLiteral = "<label";
  if (!IsUndefOrNull(className) && className != "")
    lblLiteral += " class='"+className+"'";
  if (!IsUndefOrNull(htmlFor) && htmlFor != "")
    lblLiteral += " for='"+htmlFor+"'";
  if (!IsUndefOrNull(id) && id != "")
    lblLiteral += " id='"+id+"_label'";
  lblLiteral += ">";
  if (!IsUndefOrNull(isRequired) && isRequired == true)
  {
    lblLiteral += "<img alt='' src='../images/empty.gif' width='15' height='10'";
    if (!IsUndefOrNull(_top.STYLING.reqTooltip))
    {
      lblLiteral += " title='" + _top.STYLING.reqTooltip + "'";
      if (!IsUndefOrNull(id) && id != "")
        lblLiteral += " id='"+id+"_img'";
    }
    lblLiteral += " />";
  }
  lblLiteral += "<span";
  if (!IsUndefOrNull(id) && id != "")
    lblLiteral += " id='"+id+"'";
  if (IsUndefOrNull(txt))
    txt = "";
  lblLiteral += ">"+txt+"</span>";
  /*var execStatus = 0;
  if (!IsUndefOrNull(className) && !IsUndefOrNull(_top.STYLING.separator) &&
    (HasWord(className, "left") || HasWord(className, "topSingle") || HasWord(className, "topMulti")))
  {
    lblLiteral += "<span";
    if (!IsUndefOrNull(id))
    lblLiteral += " id='"+id+"_separator'";
    lblLiteral += ">"+_top.STYLING.separator+"</span>";
    execStatus = 1;
  }
  else if (true == isSeperator)
  {
    lblLiteral += "<span";
    if (!IsUndefOrNull(id))
      lblLiteral += " id='"+id+"_separator'";
    lblLiteral += ">"+_top.STYLING.separator+"</span>";
  }
  lblLiteral += "</label>";*/

  return lblLiteral;
}

function Label(txt, id, htmlFor, className)
{
  return _CreateLabel(txt, className, htmlFor, id);
}

function LabelLeft(txt, id, htmlFor)
{
  return _CreateLabel(txt, "left", htmlFor, id,true,false);
}

function LabelLeftDisabled(txt, id)
{
  return _CreateLabel(txt, "left disabled", "", id);
}

function LabelTopSingle(txt, id)
{
  return _CreateLabel(txt, "topSingle", "", id);
}

function LabelTopMulti(txt, id)
{
  return _CreateLabel(txt, "topMulti", "", id);
}

function LabelHint(txt, id)
{
  return _CreateLabel(txt, "disabled", "", id);
}

function LabelDisabled(txt, id)
{
  return _CreateLabel(txt, "disabled", "", id);
}

function LabelRequire(txt, id)
{
  return _CreateLabel(txt, "", "", id, true, true);
}

function LabelRequireDisabled(txt, id)
{
  return _CreateLabel(txt, "disabled", "", id, true, true);
}

function DisableLabel(id)
{
  $('#'+id+'_label').addClass('disabled');
}

function EnableLabel(id)
{
  $('#'+id+'_label').removeClass('disabled');
}

function DisableInputText(id)
{
  $('#'+id).prop('disabled', true);
  $('#'+id).addClass('disabled');
}

function EnableInputText(id)
{
  $('#'+id).prop('disabled', false);
  $('#'+id).removeClass('disabled');
}

function RefreshTableSingleSelect(selId)
{
  $('.dataTable').find('tbody').find('tr').removeClass('row_selected');
  $('#'+selId).parent().parent().addClass('row_selected');
}

function GetTableSelectVal()
{
  var selVal='';
  $('.dataTable').children('tbody').children('tr').each(function(idx){
    if(HasWord($(this).prop('className'), 'row_selected'))
    {
      selVal=$(this).find('input').val();
    }
  });

  return selVal;
}

function TableHead(title)
{
  var txt = '<h1 class="table_head">'+title+'</h1>'
  return txt;
}

function TableFoot(btn)
{
  var txt = '';
  if (!IsUndefOrNull(btn))
    txt += '<div class="tblButtonGroup">'+btn+'</div>';
  txt += '<h1 class="table_foot"></h1>';
  return txt;
}

function TableCheckboxAll()
{
  var txt = '<input type="checkbox" class="selAllCheckbox" name="selAll" id="selAll">';
  return txt;
}

function TableCheckbox(val)
{
  var txt = '<input type="checkbox"  name="selEntry" id="selEntry_'+val+'" value="'+val+'"></input>'
  return txt;
}

function TableCheckboxDisable(val)
{
  var txt = '<input type="checkbox"  name="selEntry" id="selEntry_'+val+'" value="'+val+'" disabled></input>'
  return txt;
}

function TableRadio(val)
{
  var txt = '<input type="radio"  name="selEntry" id="selEntry_'+val+'" value="'+val+'"></input>'
  return txt;
}

function TableRadioDisable(val)
{
  var txt = '<input type="radio"  name="selEntry" id="selEntry_'+val+'" value="'+val+'" disabled></input>'
  return txt;
}

function TableLastTh(rowspan)
{
  var txt = '<th '+(IsUndefOrNull(rowspan) ? '' : ('rowspan='+rowspan))+' class="lastTh"><img src="../images/empty.gif"/></th>';
  return txt;
}

function TableLastTd()
{
  return '<td></td>';
}

function TableRefreshSelect(id, selVal, optAry)
{
  var txt = '';
  txt += '<table><tr><td><b>'+lang('common','lblRefreshRate')+'</b></td>';
  txt += '<td class="right">';
  txt += '<select id="'+id+'" name="'+id+'">';
  if (IsUndefOrNull(optAry)) {
    txt += '<option value=0 '+((selVal == 0)?'selected':'')+'>0</option>';
    txt += '<option value=5 '+((selVal == 5)?'selected':'')+'>5</option>';
    txt += '<option value=10 '+((selVal == 10)?'selected':'')+'>10</option>';
    txt += '<option value=30 '+((selVal == 30)?'selected':'')+'>30</option>';
  } else {
    for (var i = 0; i < optAry.length; i++) {
      txt += '<option value='+optAry[i][0]+' '+((selVal == optAry[i][0])?'selected':'')+'>'+show(optAry[i][1])+'</option>';
      //console.log(txt);
    }
  }
  txt += '</select>';
  txt += '</td><td class="right"><b>'+lang('common','lblRefreshRateSec')+'</b></td>';
  txt += '</tr></table>';
  return txt;
}

var copyTableSelArry = null;
function RefreshTableMultiSelect(ths, isSelAll)
{
  var selAllChecked = false;
  var _isSelAll = IsUndefOrNull(isSelAll) ? false : isSelAll;
  var classStr = $('.dataTable').attr('class');
  var isCopySource = false;

  if (_isSelAll)
    selAllChecked = $(ths).prop('checked');

  if (!IsUndefOrNull(classStr) && HasWord(classStr, "copy"))
    isCopySource = true;

  if (IsUndefOrNull(copyTableSelArry))
    copyTableSelArry = new Array();

  if (!_isSelAll)
  {
    var val = $(ths).parent().find('input[type=checkbox]').val();

    if($(ths).find('input[type=checkbox]').length != 0) {
      var isChecked = $(ths).find('input[type=checkbox]').prop('checked');
      if (isChecked) {
        $(ths).parent().addClass('row_selected');
        if (isCopySource)
          copyTableSelArry.push(val);
      } else {
        $(ths).parent().removeClass('row_selected');
        if (isCopySource) {
          var index = copyTableSelArry.indexOf(val);
          if (index > -1)
            copyTableSelArry.splice(index, 1);
        }
      }
    } else {
      if (!$(ths).parent().find('input[type=checkbox]').prop('disabled')) {
        $(ths).parent().toggleClass('row_selected');
        $(ths).parent().find('input[type=checkbox]').each(function(idx){
          if ($(this).prop('checked')) {
            $(this).prop('checked', false);
            if (isCopySource) {
              var index = copyTableSelArry.indexOf(val);
              if (index > -1)
                copyTableSelArry.splice(index, 1);
            }
          } else {
            $(this).prop('checked', true);
            if (isCopySource)
              copyTableSelArry.push(val);
          }
        });
      }
    }
  } else {
    if (selAllChecked) {
      $('.dataTable tbody tr').addClass('row_selected');
      $('.dataTable tbody input[type=checkbox]').each(function(){
        if (!$(this).prop('disabled')) {
          var val = $(this).val();
          $(this).prop('checked', true);
          if (isCopySource)
            copyTableSelArry.push(val);
        } else {
          $(this).parent().parent().removeClass('row_selected');
        }
      });
    } else {
      $('.dataTable tbody tr').removeClass('row_selected');
      $('.dataTable tbody input[type=checkbox]').each(function(){
        $(this).prop('checked', false);
      });
      if (isCopySource)
        copyTableSelArry = new Array();
    }
  }
  if (isCopySource) {
    $('.dataTable tbody tr').each(function(){
      var first = null;
      if (IsUndefOrNull(first)) {
        if(copyTableSelArry[0] == $(this).find('input[type=checkbox]').val()) {
          $(this).addClass('source');
          first = copyTableSelArry[0];
          return;
        }
      }
      $(this).removeClass('source');
    });
  }
}

function GetTableMultiSelectNum()
{
  var selNum = 0;
  $('.dataTable').children('tbody').children('tr').each(function(idx){
    if(HasWord($(this).prop('className'), 'row_selected'))
      selNum++;
  });

  return selNum;
}

function GetTableMultiSelectVals(selKey, isNeedEncode)
{
  var firstVal = true;
  var selVals = '';

  $('.dataTable').find('tbody').find('tr').each(function(idx){
    if(HasWord($(this).prop('class'), 'row_selected')) {
      if(firstVal)
        firstVal=false;
      else
        selVals+= '&';

      var isSource = false;
      if (HasWord($(this).prop('class'), 'source'))
        isSource = true;

      $(this).find('input[type=checkbox]').each(function(idx){
        var val=$(this).val();
        if (IsUndefOrNull(isNeedEncode) || isNeedEncode)
          selVals += selKey+'='+ encodeURIComponent(val);
        else
          selVals += selKey+'='+ val;

        if (isSource)
          selVals += '&_src='+ val;
      });
    }
  });

  return selVals;
}
function GetTableMultiSelectVals2(selKey, isNeedEncode)
{
  var firstVal = true;
  var selVals = '';
  $('.dataTable').find('tbody').find('tr').each(function(idx){
    if(HasWord($(this).prop('class'), 'row_selected')) {
      if(firstVal)
        firstVal=false;
      else
        selVals+= '&';

      var isSource = false;
      //if (HasWord($(this).prop('class'), 'source'))
        isSource = true;

      $(this).find('input[type=checkbox]').each(function(idx){
		  
        var val=$(this).val();
		if (IsUndefOrNull(isNeedEncode) || isNeedEncode)
          selVals += selKey+'='+ encodeURIComponent(val);
        else
          selVals += selKey+'='+ val;

        //if (isSource)
          selVals += '&_src='+ val;
      });
    }
  });
  return selVals;
}

function CheckTableAddEditDel(btnId)
{
  if ('edit' == $('#'+btnId).find('a').attr('class')) {
    if (GetTableMultiSelectNum() == 0)
      //ShowStatusWarning("No entry selected");
	  ShowStatusWarning(lang('common','tipNoEntrySelected'));
    else if (GetTableMultiSelectNum() > 1)
      //ShowStatusWarning("Please select only one entry to edit");
      ShowStatusWarning(lang('warning','warningSelectEdit'));
    else
      return true;
  } else if ('del' == $('#'+btnId).find('a').attr('class')){
    if (GetTableMultiSelectNum() == 0)
      //ShowStatusWarning("No entry selected");
	  ShowStatusWarning(lang('common','tipNoEntrySelected'));
    else
      return true;
  }
  return false;
}

function CheckTableEdit()
{
  if (GetTableMultiSelectNum() == 0) {
    //ShowStatusWarning("No entry selected");
    ShowStatusWarning(lang('common','tipNoEntrySelected'));
    return false;
  }

  return true;
}

function OpenModalDialog(url, width, height)
{
  var param ='';
  param += 'width='
  if(!IsUndefOrNull(width))
  	param += width;
  else
  	param += '160';

  param += ',height='
  if(!IsUndefOrNull(height))
  	param += height;
  else
  	param += '100';

  param += ',left=100,top=100,scrollbars=yes,resizable=yes,location=no,status=yes,menubar=no,toolbar=no'
  _top.modalWindow = window.open(url, EncodeWindowName("modal"), param);
  _top.document.getElementById('veil').style.display = 'block';
}

function OpenConfirmDialog(msg, title, cb_func, width, height, hasCancel, isNotify)
{
  var url = '';

  if (_top.MockupMode)
  {
    if (_top == self)
      url = 'html/ConfirmBox.html';
    else
      url = '../html/ConfirmBox.html';
  }
  else
    url = '/html/ConfirmBox.html';

  var param ='';
  param += 'width='
  if(!IsUndefOrNull(width))
    param += width;
  else
    param += '400';

  param += ',height='
  if(!IsUndefOrNull(height))
    param += height;
  else
    param += '180';

  param += ',left=250,top=180,scrollbars=yes,resizable=yes,location=no,status=yes,menubar=no,toolbar=no';

  _top.STYLING.alertBox.msg = msg;

  if (!IsUndefOrNull(cb_func))
    _top.STYLING.alertBox.func = function(){cb_func();};
  else
    _top.STYLING.alertBox.func = null;

  _top.STYLING.alertBox.title = title;

  if(!IsUndefOrNull(hasCancel))
    _top.STYLING.alertBox.hasCancel = hasCancel;
  else
    _top.STYLING.alertBox.hasCancel = true;

  if(!IsUndefOrNull(isNotify))
    _top.STYLING.alertBox.isNotify = isNotify;
  else
    _top.STYLING.alertBox.isNotify = false;

  _top.alertBox = window.open(url, EncodeWindowName("alertBox"), param);
}

function OpenNotifyDialog(msg, title, width, height)
{
  OpenConfirmDialog(msg, title, null, width, height, false, true);
}

function DestroyModalDialog()
{
  _top.document.getElementById('veil').style.display = 'none';
}

function CloseModalDialog()
{
  _top.document.getElementById('veil').style.display = 'none';
  top.close();
}

function ReloadDialogOpener()
{
  _opener.location.reload(true);
}

function OpenConfirmBox(msg, cb_func)
{
  _top.$('#confirmContainer').html('<div id="confirmBox"></div>');
  _top.$('#confirmBox').confirmOn({
      questionText: msg,
      textYes: lang('common','btnOK'),
      textNo: lang('common','btnCancel')
  },'click', function(e, confirmed) {
      if(confirmed) {$(this).remove();cb_func();}
  });

  _top.$('#confirmBox').click();
}

function ChangePageTitle(title)
{
  $(document).attr('title', title);
}

function _InputText(name, val, opt, maxlen, size)
{
  var txt = '';

  txt += '<input type="text" name="'+name+'" id="'+name+'"';
  if (!IsUndefOrNull(val))
    txt += ' value="' +val+'"';
  if (!IsUndefOrNull(opt) && opt != 'normal')
    txt += ' ' + opt;
  if (!IsUndefOrNull(maxlen)) {
    txt += ' maxlength="'+maxlen+'"'
  } else {
    if ('ipv4only' == opt)
      txt += ' maxlength="15"'
    else if ('ipv6only' == opt)
      txt += ' maxlength="45"'
    else if ('maconly' == opt)
      txt += ' maxlength="17"'
    else if ('hostonly' == opt)
      txt += ' maxlength="255"'
  }

  if (!IsUndefOrNull(size)) {
    txt += ' size="'+size+'"'
  }

  txt += 'style="ime-mode:disabled">';

  return txt;
}

function InputText(name, val, maxlen, size){return _InputText(name, val,'ascii', maxlen, size);}
function InputTextDigit(name, val, maxlen, size) {return _InputText(name,val,'digitonly', maxlen, size);}
function InputTextOid(name, val, maxlen, size) {return _InputText(name,val,'oidonly', maxlen, size);}
function InputTextHex(name, val, maxlen, size) {return _InputText(name,val,'hexonly', maxlen, size);}
function InputTextMac(name, val, maxlen, size) {return _InputText(name,val,'maconly', maxlen, size);}
function InputTextHost(name, val, maxlen, size) {return _InputText(name,val,'hostonly', maxlen, size);}
function InputTextIPv4(name, val, maxlen, size) {return _InputText(name,val,'ipv4only', maxlen, size);}
function InputTextIPv6(name, val, maxlen, size) {return _InputText(name,val,'ipv6only', maxlen, size);}
function InputHidden(name, val){return '<input type="hidden" id="'+name+'" name="'+name+'" value="'+val+'" >';}
function InputPassword(name, val, maxlen, size)
{
  var txt = '';
  txt += '<input type="password" name="'+name+'" id="'+name+'"';
  if (!IsUndefOrNull(val))
    txt += ' value="' +val+'"';
  if (!IsUndefOrNull(maxlen))
    txt += ' maxlength="'+maxlen+'"'
  if (!IsUndefOrNull(size))
    txt += ' size="'+size+'"'
  txt += '>';
  return txt;
}

function _Radio(name, label, val, isChecked, isDisabled)
{
  var txt ='';
  var id = name+'_'+(IsUndefOrNull(val)?'':val);
  txt += '<table><tr>';

  txt += '<td><input type=radio name="'+name+'" id="'+id+'"';
  if (!IsUndefOrNull(val))
    txt += ' value="'+val+'"';
  if (!IsUndefOrNull(isChecked) && isChecked == true)
    txt += ' checked';
  if (!IsUndefOrNull(isDisabled) && isDisabled == true)
    txt += ' disabled';
  txt += '></td>';
  txt += '<td class="right"><label id="'+id+'_label" '+(isDisabled ? "class=disabled" : "")+' for="'+id+'">'+label+'</label></td>';

  txt += '</tr></table>';

  return txt;
}

function Radio(name, label, val, isChecked)
{
  return _Radio(name, label, val, isChecked, false);
}

function RadioDisabled(name, label, val, isChecked)
{
  return _Radio(name, label, val, isChecked, true);
}

function EnableRadio(name, val)
{
  $('[name='+name+']').each(function(idx)
  {
    var _id = $(this).prop('name') + '_'+$(this).val();
    if(IsUndefOrNull(val))
    {
      $(this).prop('disabled', false);
      $('#'+_id+'_label').prop('className', '');
    }
    else
    {
      var id = name+'_'+val;
      if ($(this).val() == val) {
        $(this).prop('disabled', false);
        $('#'+id+'_label').prop('className', '');
      }
    }
  });
}

function DisableRadio(name, val)
{
  $('[name='+name+']').each(function(idx)
  {
    var _id = $(this).prop('name') + '_'+ $(this).val();
    if(IsUndefOrNull(val))
    {
      $(this).prop('disabled', true);
      $('#'+_id+'_label').prop('className', 'disabled');
    }
    else
    {
      var id = name+'_'+val;
      if ($(this).val() == val)
      {
        $(this).prop('disabled', true);
        $('#'+id+'_label').prop('className', 'disabled');
      }
    }
  });
}

function SelectRadio(name, val)
{
  $('[name='+name+']').each(function(idx)
  {
    var id = name+'_'+val;
    if ($(this).val() == val)
    {
      $(this).prop('checked', true);
    }
    else
    {
      $(this).prop('checked', false);
    }
  });
}

function IsChecked(id)
{
  return $('#'+id).prop('checked');
}

function RadioGroup(name, currSelVal)
{
  var txtRadioGroup ='';
  var spaceClass1='';
  var spaceClass2='';
  var i = 2;
  var id;

  txtRadioGroup += '<table>'
  while(!IsUndefOrNull(arguments[i]))
  {
    if(2 == i)
    {
      spaceClass1 = '';
      spaceClass2 = 'class="right"';
    }
    else
    {
      spaceClass1 = 'class="radioSpacing"';
      spaceClass2 = 'class="radioSpacing right"';
    }
    id = name+'_'+arguments[i][0];

    txtRadioGroup += '<tr><td '+spaceClass1+'><input type="radio" name="'+name+'" id="' + id + '" value="'+arguments[i][0]+'"'
    if (!IsUndefOrNull(currSelVal) && currSelVal == arguments[i][0])
      txtRadioGroup += ' checked';
    if (!IsUndefOrNull(arguments[i][2]) && true == arguments[i][2])
      txtRadioGroup += ' disabled';
    txtRadioGroup += '></td>';
    txtRadioGroup += '<td '+spaceClass2+'><label for="' + id + '" name="'+name+'_label" id="'+ id +'_label"';
    if (!IsUndefOrNull(arguments[i][2]) && true == arguments[i][2])
      txtRadioGroup += 'class="disabled"';
    txtRadioGroup +='>'+arguments[i][1]+'</label></td></tr>';
    i++;
  }
  txtRadioGroup += '</table>';

  return txtRadioGroup;
}

function RadioGroupPlain(name, currSelVal)
{
  var txtRadioGroup ='';
  var spaceClass1='';
  var spaceClass2='';
  var i = 2;
  var id;

  txtRadioGroup += '<table><tr>'
  while(!IsUndefOrNull(arguments[i]))
  {
    if(2 == i)
    {
      spaceClass1 = '';
      spaceClass2 = 'class="right"';
    }
    else
    {
      //spaceClass1 = 'class="radioSpacing"';
      spaceClass2 = 'class="radioSpacing right"';
    }
    id = name+'_'+arguments[i][0];

    txtRadioGroup += '<td '+spaceClass1+'><input type="radio" name="'+name+'" id="' + id + '" value="'+arguments[i][0]+'"'
    if (!IsUndefOrNull(currSelVal) && currSelVal == arguments[i][0])
      txtRadioGroup += ' checked';
    if (!IsUndefOrNull(arguments[i][2]) && true == arguments[i][2])
      txtRadioGroup += ' disabled';
    txtRadioGroup += '></td>';
    txtRadioGroup += '<td '+spaceClass2+'><label for="' + id + '" name="'+name+'_label" id="'+ id +'_label"';
    if (!IsUndefOrNull(arguments[i][2]) && true == arguments[i][2])
      txtRadioGroup += 'class="disabled"';
    txtRadioGroup +='>'+arguments[i][1]+'</label></td>';
    txtRadioGroup += '<td class="right"></td>';
    i++;
  }
  txtRadioGroup += '</tr></table>';

  return txtRadioGroup;
}

function RadioGroupSrvAddrType(hostId, ipv4Id, ipv6Id)
{
  var host = (IsUndefOrNull(hostId) ? 'host' : hostId);
  var ipv4 = (IsUndefOrNull(ipv4Id) ? 'ipv4' : ipv4Id);
  var ipv6 = (IsUndefOrNull(ipv6Id) ? 'ipv6' : ipv6Id);

  if (IsUndefOrNull(ipv6Id))
   return RadioGroup('srvDef',host,[host,lang('common','lblHostname')],[ipv4,lang('common','lblIPv4')]);
  else
   return RadioGroup('srvDef',host,[host,lang('common','lblHostname')],[ipv4,lang('common','lblIPv4')],[ipv6,lang('common','lblIPv6')]);
}

function RadioGroupDisabled(name, currSelVal)
{
  var txtRadioGroup ='';
  var spaceClass1='';
  var spaceClass2='';
  var i = 2;

  txtRadioGroup += '<table>'
  while(!IsUndefOrNull(arguments[i]))
  {
    if(2 == i)
    {
      spaceClass1 = '';
      spaceClass2 = 'class="right"';
    }
    else
    {
      spaceClass1 = 'class="radioSpacing"';
      spaceClass2 = 'class="radioSpacing right"';
    }
    txtRadioGroup += '<tr><td '+spaceClass1+'><input type="radio" name="'+name+'" id="'+name+'_'+arguments[i][0]+'" value="'+arguments[i][0]+'"'
    if (!IsUndefOrNull(currSelVal) && currSelVal == arguments[i][0])
      txtRadioGroup += ' checked';
    txtRadioGroup += ' disabled></td>';
    txtRadioGroup += '<td '+spaceClass2+'><label for="'+name+'_'+arguments[i][0]+'" name="'+name+'_label" class="disabled">'+arguments[i][1]+'</label></td></tr>';
    i++;
  }
  txtRadioGroup += '</table>'

  return txtRadioGroup;
}

function DisableRadioGroup(name)
{
  $('[name='+name+']').each(function(idx){
      $(this).prop('disabled', true);
  });
  $('[name='+name+'_label]').each(function(idx){
      $(this).prop('className', 'disabled');
  });
}

function EnableRadioGroup(name)
{
  $('[name='+name+']').each(function(idx){
      $(this).prop('disabled', false);
  });
  $('[name='+name+'_label]').each(function(idx){
      $(this).prop('className', '');
  });
}

function _Checkbox(id, label, isChecked, isDisabled, value)
{
  var txt ='';
  txt += '<table><tr>';

  txt += '<td><input type=checkbox name="'+id+'" id="'+id+'"';
  if (!IsUndefOrNull(value))
    txt += ' value="'+value+'"';
  if (!IsUndefOrNull(isChecked) && isChecked == true)
    txt += ' checked';
  if (!IsUndefOrNull(isDisabled) && isDisabled == true)
    txt += ' disabled';
  txt += '></td>';
  txt += '<td class="right"><label id="'+id+'_label" '+(isDisabled ? "class=disabled" : "")+' for="'+id+'">'+label+'</label></td>';

  txt += '</tr></table>';

  return txt;
}

function _CheckboxOncheckBoxclick(id, label, isChecked, isDisabled, value)
{
  var txt ='';
  txt += '<table><tr>';

  txt += '<td><input type=checkbox onclick= "onCheckBoxClick(this)" name="'+id+'" id="'+id+'"';
  if (!IsUndefOrNull(value))
    txt += ' value="'+value+'"';
  if (!IsUndefOrNull(isChecked) && isChecked == true)
    txt += ' checked';
  if (!IsUndefOrNull(isDisabled) && isDisabled == true)
    txt += ' disabled';
  txt += '></td>';
  txt += '<td class="right"><label id="'+id+'_label" '+(isDisabled ? "class=disabled" : "")+' for="'+id+'">'+label+'</label></td>';

  txt += '</tr></table>';

  return txt;
}

function CheckboxOncheck(id, label, isChecked, value)
{
  return _CheckboxOncheckBoxclick(id, label, isChecked, false, value);
}

function Checkbox(id, label, isChecked, value)
{
  return _Checkbox(id, label, isChecked, false, value);
}

function CheckboxTxtEnable(id, isChecked, value)
{
  return Checkbox(id, lang('common','txtEnable'), isChecked, value);
}

function CheckboxDisabled(id, label, isChecked, value)
{
  return _Checkbox(id, label, isChecked, true, value);
}

function EnableCheckbox(id)
{
  $('#'+id).prop('disabled', false);
  $('#'+id+'_label').prop('className', '');
}

function DisableCheckbox(id)
{
  $('#'+id).prop('disabled', true);
  $('#'+id+'_label').prop('className', 'disabled');
}

function IsCheckboxChecked(id)
{
  return $('#'+id).prop('checked');
}

function Select(selectId, entries, selectVal)
{
  var txt='';
  var msg;

  txt += '<select class="inputTextErrMsg" id="' + selectId + '" name="' + selectId + '">';

  _.each(entries, function(entry){
    if (IsUndefOrNull(entry.name))
      msg = entry.val;
    else
      msg = show(entry.name);

    txt += '<option value="'+entry.val+'"'+((entry.val == selectVal) ? "selected" : "")+">"+msg+'</option>';
  });
  txt += '</select>';

  return txt;
}


function SelectArry(selectId, entries, selectVal)
{
  var txt='';
  var msg;

  txt += '<select class="inputTextErrMsg" id="' + selectId + '" name="' + selectId + '">';

  _.each(entries, function(entry){
    if (IsUndefOrNull(entries))
    {
      msg = '';
    }
    else
    {
        msg = show(entry.toString());
    }

    txt += '<option value="'+entry+'"'+((entry == selectVal) ? "selected" : "")+">"+msg+'</option>'});
  txt += '</select>';

  return txt;
}

function EnableSelect(id)
{
  $('#'+id)[0].disabled = false;
  $('#'+id)[0].className = "";
}
function DisableSelect(id)
{
  $('#'+id)[0].disabled = true;
  $('#'+id)[0].className = "disabled";
}

function GetSelectVal(selId)
{
  var val;
  $('#'+selId).children('option').each(function(idx){
    if(true == $(this).prop('selected'))
    {
      val = $(this).val();
      return val;
    }
  });
  return val;
}

function SelSelectVal(selId, selVal)
{
  var val;
  $('#'+selId).children('option').each(function(idx){
    if(selVal == $(this).val())
    {
      $(this).prop('selected', true);
      return;
    }
  });
  return;
}

function GetRadioGroupCheckedVal(rdoName)
{
  var val;
  $('[name='+rdoName+']').each(function(idx){
    if(true == $(this).prop('checked'))
    {
      val = $(this).val();
      return val;
    }
  });
  return val;
}

function GetRadioVal(rdoName)
{
  return GetRadioGroupCheckedVal(rdoName);
}

function EnableInput(id)
{
  $('#'+id)[0].disabled = false;
  $('#'+id)[0].className = "";
}
function DisableInput(id)
{
  $('#'+id)[0].disabled = true;
  $('#'+id)[0].className = "disabled";
}

function ShowHr(className, colNum)
{
  var txt = '';
  var i;
  var _colNum = IsUndefOrNull(colNum) ? 1 : (colNum - 1);
  txt += '<td ';
  if (!IsUndefOrNull(className))
    txt += 'class="'+className+'"';
  txt += '><img class="hr" src="../images/hr60.png"></td>';
  for (i = 0; i < _colNum; i++) {
    txt += '<td '+((i == _colNum - 1)?'width=100%':'')+'><img class="hr" src="../images/hr60.png"></td>';
  }
  return txt;
}

function ShowFormContentGap()
{
  var txt = '<tr><td>&nbsp;</td></tr>';
  return txt;
}

function ShowFormTitle(title)
{
  var txt = '<tr><td colspan="2" class="formContentTitle">'+title+'</td></tr>';
  return txt;
}

function DialogHead(title)
{
  /*var txt = '<table><tr><td style="width:650px;';
  console.log(isIE());
  if (isIE() && isIE() >= 10)
    txt += 'height:40px;';
  txt += '"><h1 class="dialog_head">'+title+'</h1></td></tr></table>'*/

  var txt = '<div style="width:650px;height:40px;"><h1 class="dialog_head">'+title+'</h1></div>';
  return txt;
}

function DialogFoot(btn)
{
  var txt = '<table border=1><tr><td style="width:650px;">'+btn+'</td></tr><tr><td><h1 class="dialog_foot"></h1></td></tr></table>'
  return txt;
}

var isDialogOpen = false;
function _OpenDialog(url, isAdd)
{
  $('#dialog_container').html('<iframe width="100%" height="100%" allowtransparency="true" style="background-color: transparent;" frameborder="no" src="'+url+'"></iframe>');
  if (isIE() && isIE() <= 8)
    $('#main_body').scrollTop($('#dialog_container').position().top + $('#main_table').height());
  else
    $('#main_body').scrollTop($('#dialog_container').position().top);

  isDialogOpen = true;
}

function OpenDialog(url){_OpenDialog(url);}

function CloseDialog()
{
  if (IsUndefOrNull(_opener)) {
    $('#dialog_container').html('');
    isDialogOpen = false;
  } else {
    _opener.$('#dialog_container').html('');
    _opener.isDialogOpen = false;
  }
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

/*for mcast function*/
function get_mcast_ipVersion(url)
{
  var ipVer = 0;
  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
    if (urlParamGetByName('ipType', url).length == 0)
      ipVer = 0;
    else
      ipVer = str2int(urlParamGetByName('ipType', url)) % 2;
  <!--#endif-->

  return ipVer;
}

function get_mcastEdit_ipVersion()
{
  var ipVer = 0;
  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  if (urlParamGetByName('ipType').length != 0)
      ipVer = urlParamGetByName('ipType');
  <!--#endif-->
  return ipVer;
}

function mcastUrl_append_ipVer(url,ipVer)
{
  var _url = url;
  <!--#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING-->
  _url = urlParamAppend(_url, 'ipType', ipVer);
  <!--#endif-->
  return _url;
}

/* This ready flag should put at file end */
var ready_js_styling = 1;
