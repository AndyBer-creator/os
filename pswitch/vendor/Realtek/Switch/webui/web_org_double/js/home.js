var isSafari = (navigator.vendor && (navigator.vendor.toLowerCase().indexOf('apple') != -1) ) ? true : false;
var isFF = (navigator.userAgent && (navigator.userAgent.toLowerCase().indexOf('firefox') != -1) ) ? true : false;
var isChrome = (navigator.userAgent && (navigator.userAgent.toLowerCase().indexOf('chrome') != -1) ) ? true : false;

_top = self;
_top.STYLING = {
  afterPOST: "",
  successMsg: "Success.",
  successMsgNS: {
  msgType: 1,
  msgText: "Success. To permanently save the configuration, go to <a href=\"javascript:NavigateFromSuccessMessage()\" class=\"onLight\"></a> page or click Save icon.",
  linkText: "Copy/Save Configuration",
  linkFunc: function(){}
  },
  edtBtnNotSelect: "Edit: only selected entry could be processed.",
  edtBtnNotSingle: "Edit: only single entry could be processed.",
  delBtnNotSelect: "Delete: only selected entries could be processed.",
  separator: ":",
  reqEmptyMsg: "The field cannot be empty.",
  reqTooltip: "* = Required fields.",
  emptyTbl: "0 results found.",
  sessionId: Math.round(Math.random()*10000000000000000).toString(10),
  alertBox:
  {
  title: "",
  msg: "",
  func: function(){},
  severity: 0,
  closeDialog: false,
  hasCancel:true,
  isNotify:false,
  noConfirmFuncHash: {}

  },
  buttons:
  {
  getMinWidth: function() {return 18;},
  getShortWidth: function() {return 80;},
  getLongWidth: function() {return 160;}
  }
};

navigator.sayswho= (function(){
    var ua= navigator.userAgent,
    N= navigator.appName, tem,
    M= ua.match(/(opera|chrome|safari|firefox|msie|trident)\/?\s*([\d\.]+)/i) || [];
    M= M[2]? [M[1], M[2]]:[N, navigator.appVersion, '-?'];
    if(M && (tem= ua.match(/version\/([\.\d]+)/i))!= null) M[2]= tem[1];
    return M.join(' ');
})();

function isBrowserOlderIE10() {
  if (isIE() > 0 && isIE() < 10) {
    return true;
  }
  return false;
}

function setParameters()
{
  var relTd=document.getElementById("tdTree");
  var height=document.getElementById("mainDiv").clientHeight;
  var top=document.getElementById("trTitle").clientHeight;
  document.getElementById("imT").style.height=height;
}
function setPageArea()
{
  var pageArea=document.getElementById("pageAreaDiv").clientHeight;
  var sbExtra=0;
  if(document.body.clientWidth<780)
  {
    document.getElementById("pageAreaDiv").style.width=780;
    if(isIE())
    {
      document.getElementById("bottomTD").style.height=51;
      document.getElementById("bottomTD").style.paddingBottom=30;
    }
  }
  else
  {
    document.getElementById("pageAreaDiv").style.width=document.body.clientWidth;
    if(isIE() && document.getElementById("bottomTD").clientHeight==51)
    {
      document.getElementById("bottomTD").style.height=34;
      document.getElementById("bottomTD").style.paddingBottom=13;
    }
  }
  if(!isBrowserOlderIE10())
    AdjustToFF();
  if (11 != isIE())
    setParameters();
  if(pageArea!=document.getElementById("pageAreaDiv").clientHeight && !isBrowserOlderIE10())
  {
    AdjustToFF();
  }
}
function AdjustToFF()
{
  var extraHeight=document.getElementById("trTitle").clientHeight+document.getElementById("trAction").clientHeight+document.getElementById("trNavi").clientHeight+document.getElementById("bottomTR").clientHeight;
  document.getElementById("divTree").style.height = document.getElementById("pageAreaDiv").clientHeight-extraHeight;
  document.getElementById("mainDiv").style.height = document.getElementById("pageAreaDiv").clientHeight-extraHeight;
  document.getElementById("mainFrame_gw").style.margin = "0px -2px";
}


var spinner = null;
function homeShowProg(doShow, msg, btnToken)
{
  if (doShow) {
    var clHeight = document.body.clientHeight;
    var clWidth = document.body.clientWidth;
    var posTop = (clHeight - 45) / 2;
    var posLeft = (clWidth - 290) / 2;
    document.getElementById("dvProgBar").style.top = posTop;
    document.getElementById("dvProgBar").style.left = posLeft;
    document.getElementById("dvProgVeil").style.display="";
    document.getElementById("dvProgBar").style.display="";
    if (null == spinner) {
      var opts = {
        lines: 13, // The number of lines to draw
        length: 10, // The length of each line
        width: 3, // The line thickness
        radius: 10, // The radius of the inner circle
        corners: 1, // Corner roundness (0..1)
        rotate: 20, // The rotation offset
        direction: 1, // 1: clockwise, -1: counterclockwise
        color: '#000', // #rgb or #rrggbb or array of colors
        speed: 1, // Rounds per second
        trail: 62, // Afterglow percentage
        shadow: true, // Whether to render a shadow
        hwaccel: false, // Whether to use hardware acceleration
        className: 'spinner', // The CSS class to assign to the spinner
        zIndex: 999997, // The z-index (defaults to 2000000000)
        top: 'auto', // Top position relative to parent in px
        left: 'auto' // Left position relative to parent in px
      };
      var target = document.getElementById('progSpinner');
      spinner = new Spinner(opts).spin(target);
    }
  } else {
    document.getElementById("dvProgBar").style.display="none";
    document.getElementById("dvProgVeil").style.display="none";
  }
}

function createNewWindow(urlString,windowName,windowWidth,windowHeight,windowLeft,windowTop,enableScroll,enableResize)
{
var flagEnableScroll=(enableScroll=='yes'||enableScroll==true)?"yes":"no";
var flagEnableResize=(enableResize=='yes'||enableResize==true)?"yes":"no";
return window.open(urlString, windowName, 'width='+windowWidth+', height='+windowHeight+', left='+windowLeft+', top='+windowTop+', scrollbars='+flagEnableScroll+', resizable='+flagEnableResize);
}
function aboutPopupOpener()
{
  if (typeof(aboutPopupWin) != "object")
  aboutPopupWin = createNewWindow('html/About.html','','480px','425px','50','50','no','no');
  else if (aboutPopupWin.closed)
  aboutPopupWin = createNewWindow('html/About.html','','480px','425px','50','50','no','no');
  aboutPopupWin.focus();
}

function helpPopupOpener()
{
  var finalUrl;
  if (IsUndefOrNull(_top.defHelp))
    finalUrl = 'help/index.html';
  else
    finalUrl = _top.defHelp;
  _top.helpPopupWin = createNewWindow(finalUrl,'help','920px','750px','50','50','no','no');
  _top.helpPopupWin.focus();
}

function SetDrwNormalClass(node)
{
  node.className = "drw_normal";
}
function SetDrwHoverClass(node)
{
  node.className = "drw_hover";
}

function treeRootClick(ths, url)
{
  if (ths == selFolder1)
    return;

  var td = $('#'+ths).find('td');
  var span = $('#'+ths).find('span');
  td.addClass('selectedDrw');
  td.removeClass('drwTD2');
  span.css('color', treeSelColor);
  span.css('font-weight', 'bold');

  if ('' != selFolder1)
  {
    td = $('#'+selFolder1).find('td');
    span = $('#'+selFolder1).find('span');

    td.addClass('drwTD2');
    td.removeClass('selectedDrw');
    span.css('color', '');
    span.css('font-weight', '');

    var oldselimg = $('#'+selFolder1+'img');
      if (-1 != oldselimg.prop('src').indexOf('drawerTriangleOpenSelected'))
        oldselimg.prop('src', 'images/drawerTriangleOpenT.gif');
      else
        oldselimg.prop('src', 'images/drawerTriangleCollapsedT.gif');
  }
  if ('' != selDoc)
  {
    var a = $('#'+selDoc);
    a.css('color', '');
    a.css('font-weight', '');
    a.css('background-color', '');
  }

  selFolder1 = ths;
  selDoc = '';
  $('#mainFrame').attr('src', url);

  _top.defPage = url;
  _top.defHelp = $('#roothelpurl').val();
}
function treeFolder1Click(ths)
{
  if (treeImgClick)
  {
    treeImgClick = false;
    return;
  }

  var img = $('#'+ths+'img');
  var ths_tr = $('#'+ths+'tr');
  var tr;
  /* open and selected */
  if (-1 != img.prop('src').indexOf('drawerTriangleOpenSelected'))
  {
    /* collapsed all childs */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
    }
    /* collapsed all layer 2 folders and documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
      var folder2img = $('#'+ths+'_f'+i+'img');
      folder2img.prop('src', 'images/drawerTriangleCollapsedT.gif');

      for (j = 1; j < maxTreeEntry; j++)
      {
        tr = $('#'+ths+'_f'+i+'_'+j+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', 'none');
      }
    }
	
	/* collapsed all layer 3 folders and documents */
	for (i = 1; i < maxTreeEntry; i++)
	{
		for (k = 1; k < maxTreeEntry; k++)
		{
			tr = $('#'+ths+'_f'+i+'_sf'+k+'tr');
			if (undefined == tr.val())
			  break;
			tr.css('display', 'none');
			var folder3img = $('#'+ths+'_f'+i+'sf_'+k+'img');
			folder3img.prop('src', 'images/drawerTriangleCollapsedT.gif');
			 
			for (j = 1; j < maxTreeEntry; j++)
			{
			  tr = $('#'+ths+'_f'+i+'_sf'+k+'_'+j+'tr');
			  if (undefined == tr.val())
			    break;
			  tr.css('display', 'none');
			}
		}
	}

    img.prop('src', 'images/drawerTriangleCollapsed_Selected.gif');
  }
  /* open and not selected */
  else if (-1 != img.prop('src').indexOf('drawerTriangleOpenT'))
  {
    /* collapsed all childs */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');

      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
    }
    /* collapsed all layer 2 folders and documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
      var folder2img = $('#'+ths+'_f'+i+'img');
      folder2img.prop('src', 'images/drawerTriangleCollapsedT.gif');

	  for (j = 1; j < maxTreeEntry; j++)
	  {
		  /*打开三级目录*/
		  tr = $('#'+ths+'_f'+i+'_sf'+j+'tr');
		  if (undefined == tr.val())
		    break;
		  tr.css('display', 'none');
		  var folder3img = $('#'+ths+'_f'+i+'_sf'+j+'img');
		  folder3img.prop('src', 'images/drawerTriangleCollapsedT.gif');
	  }
	  
      for (j = 1; j < maxTreeEntry; j++)
      {
        tr = $('#'+ths+'_f'+i+'_'+j+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', 'none');
      }
    }

    img.prop('src', 'images/drawerTriangleCollapsedT.gif');
  }
  /* collapsed and selected */
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsed_Selected'))
  {
    /* show all Layer 1 documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');

      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    /* show all layer 2 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }

    /* show all layer 3 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
      for (j = 1; j < maxTreeEntry; j++)
      {
      	tr = $('#'+ths+'_f'+i+'_sf'+j+'tr');
      	if (undefined == tr.val())
      	  break;
      	tr.css('display', '');
      }
    }
	
    img.prop('src', 'images/drawerTriangleOpenSelected.gif');

    var oldSelId = $('#'+ths+'sel').prop('value');
    var strTok = oldSelId.split("_");

    if (strTok.length > 2)
    {
      var folder2img = $('#'+strTok[0]+'_'+strTok[1]+'img');
      folder2img.prop('src', 'images/drawerTriangleOpenT.gif');

      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = $('#'+strTok[0]+'_'+strTok[1]+'_'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
      }
    }
  }
  /* collapsed and not selected */
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
  {
    /* show all Layer 1 documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');

      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    /* show all layer 2 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');

      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
	/* show all layer 3 folders */
	for (i = 1; i < maxTreeEntry; i++)
	{
	  for (j = 1; j < maxTreeEntry; j++)
	  {
		  tr = $('#'+ths+'_f'+i+'_sf'+j+'tr');
		  if (undefined == tr.val())
		    break;
		  tr.css('display', '');
	  }
	}
    /* show selected attr */
    var td = $('#'+ths).find('td');
    var span = $('#'+ths).find('span');
    td.addClass('selectedDrw');
    td.removeClass('drwTD2');
    span.css('color', treeSelColor);
    span.css('font-weight', 'bold');

    if ('' != selFolder1)
    {
      td = $('#'+selFolder1).find('td');
      span = $('#'+selFolder1).find('span');

      td.addClass('drwTD2');
      td.removeClass('selectedDrw');
      span.css('color', '');
      span.css('font-weight', '');

      var oldselimg = $('#'+selFolder1+'img');
      if (undefined != oldselimg.val())
      {
        if (-1 != oldselimg.prop('src').indexOf('drawerTriangleOpenSelected'))
          oldselimg.prop('src', 'images/drawerTriangleOpenT.gif');
        else
          oldselimg.prop('src', 'images/drawerTriangleCollapsedT.gif');
      }
    }
    img.prop('src', 'images/drawerTriangleOpenSelected.gif');

    selFolder1 = ths;

    /* Find old selected document and open it */
    var oldSelId = $('#'+ths+'sel').prop('value');

    if ('' != selDoc)
    {
      a = $('#'+selDoc);
      a.css('color', '');
      a.css('font-weight', '');
      a.css('background-color', '');
    }

    var strTok = oldSelId.split("_");

    if (strTok.length > 2)
    {
      var folder2img = $('#'+strTok[0]+'_'+strTok[1]+'img');
      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = $('#'+strTok[0]+'_'+strTok[1]+'_'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
        span = $('#'+strTok[0]+'_'+strTok[1]).find('span');
        folder2img.prop('src', 'images/drawerTriangleOpenT.gif');
      }
    }

    a = $('#'+oldSelId);
    a.css('color', treeSelColor);
    a.css('font-weight', treeSelDocFontWeight);
    a.css('background-color', treeSelBackground);
    a[0].onclick();
  }
  refreshNavigator();
}
function treeFolder1ImgClick(ths)
{
  treeImgClick = true;

  var img = $('#'+ths+'img')

  if (-1 != img.prop('src').indexOf('drawerTriangleOpenSelected') || -1 != img.prop('src').indexOf('drawerTriangleOpenT'))
  {
    /* collapsed all childs */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
    }
    /* collapsed all layer 2 folders and documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
      var folder2img = $('#'+ths+'_f'+i+'img');
      folder2img.prop('src', 'images/drawerTriangleCollapsedT.gif');

      for (j = 1; j < maxTreeEntry; j++)
      {
        tr = $('#'+ths+'_f'+i+'_'+j+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', 'none');
      }
    }
	/* collapsed all layer 3 folders and documents*/
	for (i = 1; i < maxTreeEntry; i++)
	{
		for (j = 1; j < maxTreeEntry; j++)
		{
			tr = $('#'+ths+'_f'+i+'_sf'+j+'tr');
			if (undefined == tr.val())
			  break;
			tr.css('display', 'none');
			var folder3img = $('#'+ths+'_f'+i+'sf'+j+'img');
			folder3img.prop('src', 'images/drawerTriangleCollapsedT.gif');	
			
			for (k = 1; k < maxTreeEntry; k++)
			{
			  tr = $('#'+ths+'_f'+i+'_sf'+j+'_'+k+'tr');
			  if (undefined == tr.val())
			    break;
			  tr.css('display', 'none');
			}
		}
	}

    if (-1 != img.prop('src').indexOf('drawerTriangleOpenSelected'))
    {
      img.prop('src', 'images/drawerTriangleCollapsed_Selected.gif');
    }
    else
    {
      img.prop('src', 'images/drawerTriangleCollapsedT.gif');
    }
  }
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
  {
    /* show all Layer 1 documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    /* show all layer 2 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }

    img.prop('src', 'images/drawerTriangleOpenT.gif');
  }
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsed_Selected'))
  {
    /* show all Layer 1 documents */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    /* show all layer 2 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_f'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    /* show all layer 3 folders */
    for (i = 1; i < maxTreeEntry; i++)
    {
	  for (j = 1; j < maxTreeEntry; j++)
	  {
		  tr = $('#'+ths+'_f'+i+'_sf'+j+'tr');
		  if (undefined == tr.val())
		    break;
		  tr.css('display', '');
	  }
    }
    img.prop('src', 'images/drawerTriangleOpenSelected.gif');

    var oldSelId = $('#'+ths+'sel').prop('value');
    var strTok = oldSelId.split("_");

    if (strTok.length > 2)
    {
      var folder2img = $('#'+strTok[0]+'_'+strTok[1]+'img');
      folder2img.prop('src', 'images/drawerTriangleOpenT.gif');
      for (i = 1; i < maxTreeEntry; i++)
      {
        tr = $('#'+strTok[0]+'_'+strTok[1]+'_'+i+'tr');
        if (undefined == tr.val())
          break;
        tr.css('display', '');
      }
    }
  }
  refreshNavigator();
}
function treeFolder2Click(ths)
{
  var img = $('#'+ths+'img');
  if (-1 != img.prop('src').indexOf('drawerTriangleOpenT'))
  {
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');

      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
    }
	for (i = 1; i < maxTreeEntry; i++)
	{
	  tr = $('#'+ths+'_sf'+i+'tr');
	  if (undefined == tr.val())
	    break;
	  tr.css('display', 'none');
	}
	
    img.prop('src', 'images/drawerTriangleCollapsedT.gif');
  }
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
  {
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
	
	for (i = 1; i < maxTreeEntry; i++)
	{
	   tr = $('#'+ths+'_sf'+i+'tr');
	   if (undefined == tr.val())
		 break;
	   tr.css('display', '');
	}
    img.prop('src', 'images/drawerTriangleOpenT.gif');
  }
}

function treeFolder3Click(ths)
{
  var img = $('#'+ths+'img');

  if (-1 != img.prop('src').indexOf('drawerTriangleOpenT'))
  {
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', 'none');
    }
    img.prop('src', 'images/drawerTriangleCollapsedT.gif');
  }
  else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsedT'))
  {
    for (i = 1; i < maxTreeEntry; i++)
    {
      tr = $('#'+ths+'_'+i+'tr');
      if (undefined == tr.val())
        break;
      tr.css('display', '');
    }
    img.prop('src', 'images/drawerTriangleOpenT.gif');
  }
}
function treeDocClick(ths, url)
{
  if (ths == selDoc)
  {
    openLink(url);
    return;
  }

  var a = $('#'+ths);
  a.css('color', treeSelColor);
  a.css('font-weight', treeSelDocFontWeight);
  a.css('background-color', treeSelBackground);

  if ('' != selDoc)
  {
    a = $('#'+selDoc);
    a.css('color', '');
    a.css('font-weight', '');
    a.css('background-color', '');
  }
  selDoc = ths;

  var strTok = ths.split("_");

  if (strTok[0] != selFolder1)
  {
    var td = $('#'+strTok[0]).find('td');
    var span = $('#'+strTok[0]).find('span');
    var img = $('#'+strTok[0]+'img');
    td.addClass('selectedDrw');
    td.removeClass('drwTD2');
    span.css('color', treeSelColor);
    span.css('font-weight', 'bold');
    img.prop('src', 'images/drawerTriangleOpenSelected.gif');

    if ('' != selFolder1)
    {
      td = $('#'+selFolder1).find('td');
      span = $('#'+selFolder1).find('span');
      img = $('#'+selFolder1+'img');

      td.addClass('drwTD2');
      td.removeClass('selectedDrw');
      span.css('color', '');
      span.css('font-weight', '');

      if (undefined != img.val())
      {
        if (-1 != img.prop('src').indexOf('drawerTriangleOpenSelected'))
          img.prop('src', 'images/drawerTriangleOpenT.gif');
        else if (-1 != img.prop('src').indexOf('drawerTriangleCollapsed_Selected'))
          img.prop('src', 'images/drawerTriangleCollapsedT.gif');
      }
    }

    selFolder1 = strTok[0];
  }
  /* Record last select for folder1 */
  $('#'+selFolder1+'sel').prop('value', ths);

  openLink(url);
  refreshNavigator();
}

function refreshNavigator()
{
  var strTok = selDoc.split("_");
  var span = '';
  var a = '';
  var txt = '';
  var rightTriangle = '<span style="padding-left:5px;padding-right:7px;"><img src="images/right_triangle.png"></span>';

  span = $('#'+strTok[0]).find('span');
  txt += '<span style="cursor:pointer;color:'+naviColorLevel1+';" onclick="treeFolder1Click(\''+strTok[0]+'\');">'+span.text()+'</span>';
  txt += rightTriangle;

  if (strTok.length > 2 && strTok.length < 4) {
    a = $('#'+strTok[0]+'_'+strTok[1]+'tr').find('a');
    txt += '<span style="cursor:pointer;color:'+naviColorLevel2+';" onclick="treeFolder2Click(\''+strTok[0]+'_'+strTok[1]+'\');">'+a.text()+'</span>';
    txt += rightTriangle;
  }

  a = $('#'+selDoc+'tr').find('a');
  strTok = a.attr('onclick').split("treeDocClick(this.id, '");
  strTok = strTok[1].split("'");
  txt += '<span style="cursor:pointer;color:'+naviColorLevel3+';" onclick="treeDocClick(\''+selDoc+'\', \''+strTok[0]+'\');">'+a.text()+'</span>';

  $('#tdNavi').html(txt);
}

function genTreeRoot(descp, url, helpurl)
{
  var txt = '';
  txt += '<tr height="21px" id="treeF0tr">';
  txt += '<td align="left" nowrap>';
  txt += '<table id="treeF0" width="100%" class="drw_normal" onclick="treeRootClick(this.id, \''+url+'\');" onmouseover="SetDrwHoverClass(this)" onmouseout="SetDrwNormalClass(this)" >';
  txt += '<tr><td noWrap class="drwTD2" style="padding-left: 12px"><span class="FolderText-" align="left" >'+descp+'</span></td></tr>';
  if (!IsUndefOrNull(helpurl))
    txt += '<input type="hidden" id="roothelpurl" value="'+helpurl+'">';
  txt += '</table></td></tr>';
  $('#tfootTree').append(txt);
}
function genTreeFolder1(descp)
{
  var treeId = '';
  var txt = '';

  treeFolder1Cnt++;
  treeFolder2Cnt = 0;
  treeFolder3Cnt = 0;
  treeDoc1Cnt = 0;
  treeDoc2Cnt = 0;
  treeDoc3Cnt = 0;
  
  treeId = 'treeF' + treeFolder1Cnt;

  txt += '<tr height="21px" id="'+treeId+'tr'+'">';
  txt += '<td noWrap align="left">';
  txt += '<table id="'+treeId+'" width="100%" class="drw_normal" onclick="treeFolder1Click(this.id);" onmouseover="SetDrwHoverClass(this)" onmouseout="SetDrwNormalClass(this)" >';
  txt += '<tr><td noWrap class="drwTD2" ><img id="'+treeId+'img'+'" height="12px" onclick="treeFolder1ImgClick(\''+treeId+'\');" width="12px" vspace="2" hspace="0" border="0" align="left" src="./images/drawerTriangleCollapsedT.gif" ><span class="FolderText-" align="left" >'+descp+'</span></td><input type="hidden" id="'+treeId+'sel'+'"></tr>';
  txt += '</table></td></tr>';
  $('#tfootTree').append(txt);
}
function genTreeDoc1(descp, url, helpurl)
{
  var treeId = '';
  var txt = '';

  treeDoc1Cnt++;
  treeId = 'treeF' + treeFolder1Cnt + '_' + treeDoc1Cnt;

  var folder1Sel = $('#treeF' + treeFolder1Cnt + 'sel');
  if ('' == folder1Sel.prop('value'))
    folder1Sel.prop('value', treeId);

  txt += '<tr height=18 id="'+treeId+'tr'+'" style="display:none">';
  txt += '<td valign="top" nowrap="" align="left" '+((treeDoc1Cnt==1 && treeFolder2Cnt ==0) ? "style='padding-top: 3px;'": "")+'>';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="28" height="18">';
  txt += '<a !style="width:100%" id="'+treeId+'"';
  if (!IsUndefOrNull(helpurl))
    txt += ' helpurl="'+helpurl+'"';
  txt += ' class="FolderText" name="'+url+'" onclick="treeDocClick(this.id, \''+url+'\');" href="javascript:void(0);">'+descp+'</a>';
  txt += '</td></tr>';
  $('#tfootTree').append(txt);
}
function genTreeFolder2(descp)
{
  var treeId = '';
  var txt = '';

  treeFolder2Cnt++;
  treeFolder3Cnt = 0;
  treeDoc2Cnt = 0;
  treeId = 'treeF' + treeFolder1Cnt + '_f' + treeFolder2Cnt;

  txt += '<tr height=18 id="'+treeId+'tr'+'" style="display:none">';
  txt += '<td valign="top" nowrap="" align="left" '+((treeDoc1Cnt==0 && treeFolder2Cnt ==1) ? "style='padding-top: 3px;'": "")+' '+((treeFolder2Cnt >= 1) ? "" : "")+'>';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="10" height="18">';
  txt += '<img id="'+treeId+'img'+'" height="12px" width="12px" vspace="2" hspace="0" border="0" align="left" src="./images/drawerTriangleCollapsedT.gif" onclick="treeFolder2Click(\''+treeId+'\');">';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="6" height="12">';
  txt += '<a !style="width:100%" id="'+treeId+'" class="FolderText" onclick="treeFolder2Click(this.id);" onmouseover="return true;" href="javascript:void(0);">'+descp+'</a>';
  txt += '</td></tr>';
  $('#tfootTree').append(txt);
}
function genTreeDoc2(descp, url, helpurl)
{
  var treeId = '';
  var txt = '';

  treeDoc2Cnt++;
  treeId = 'treeF' + treeFolder1Cnt + '_f' + treeFolder2Cnt + '_' + treeDoc2Cnt;

  var folder1Sel = $('#treeF' + treeFolder1Cnt + 'sel');
  if ('' == folder1Sel.prop('value'))
    folder1Sel.prop('value', treeId);

  txt += '<tr height=18 id="'+treeId+'tr'+'" style="display:none">';
  txt += '<td valign="top" nowrap="" align="left">';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="38" height="18">';
  txt += '<a !style="width:100%" id="'+treeId+'"';
  if (!IsUndefOrNull(helpurl))
    txt += ' helpurl="'+helpurl+'"';
  txt += ' class="FolderText" name="'+url+'" onclick="treeDocClick(this.id, \''+url+'\');" href="javascript:void(0);">'+descp+'</a>';
  txt += '</td></tr>';
  $('#tfootTree').append(txt);
}

function genTreeFolder3(descp)
{
  var treeId = '';
  var txt = '';
  treeFolder3Cnt++;
  treeDoc3Cnt = 0;
  treeId = 'treeF' + treeFolder1Cnt + '_f' + treeFolder2Cnt + '_sf' + treeFolder3Cnt;
  txt += '<tr height=18 id="'+treeId+'tr'+'" style="display:none">';
  txt += '<td valign="top" nowrap="" align="left" '+((treeDoc2Cnt==0 && treeFolder3Cnt ==1) ? "style='padding-top: 3px;'": "")+' '+((treeFolder3Cnt >= 1) ? "" : "")+'>';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="20" height="18">';
  txt += '<img id="'+treeId+'img'+'" height="12px" width="12px" vspace="2" hspace="0" border="0" align="left" src="./images/drawerTriangleCollapsedT.gif" onclick="treeFolder3Click(\''+treeId+'\');">';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="6" height="12">';
  txt += '<a !style="width:100%" id="'+treeId+'" class="FolderText" onclick="treeFolder3Click(this.id);" onmouseover="return true;" href="javascript:void(0);">'+descp+'</a>';
  txt += '</td></tr>'; 
  $('#tfootTree').append(txt);
}

function genTreeDoc3(descp, url, helpurl)
{
  var treeId = '';
  var txt = '';
  
  treeDoc3Cnt++;
  treeId = 'treeF' + treeFolder1Cnt + '_f' + treeFolder2Cnt  + '_sf' + treeFolder3Cnt + '_' + treeDoc3Cnt;

  var folder1Sel = $('#treeF' + treeFolder1Cnt + 'sel');
  if ('' == folder1Sel.prop('value'))
    folder1Sel.prop('value', treeId);

  txt += '<tr height=18 id="'+treeId+'tr'+'" style="display:none">';
  txt += '<td valign="top" nowrap="" align="left">';
  txt += '<img hspace="0" border="0" align="left" src="./images/space.gif" width="58" height="18">';
  txt += '<a !style="width:100%" id="'+treeId+'"';
  if (!IsUndefOrNull(helpurl))
    txt += ' helpurl="'+helpurl+'"';
  txt += ' class="FolderText" name="'+url+'" onclick="treeDocClick(this.id, \''+url+'\');" href="javascript:void(0);">'+descp+'</a>';
  txt += '</td></tr>';
  $('#tfootTree').append(txt);
}

function ChangeDecoration(id,addDecor)
{
  if(addDecor) {
    document.getElementById(id).style.textDecoration="underline";
    document.getElementById(id).style.color="red";
  }
  else {
    document.getElementById(id).style.textDecoration="none";
    document.getElementById(id).style.color="#EFEFEF";
  }
}

function genActionTab(id, descp, action)
{
  var txt = '';
  var tabid= 'tab_'+id;
  txt += '<td class="tab"><a href="javascript:'+ (IsUndefOrNull(action) ? 'void(0);' : action) +'" ';
  txt += 'style="text-decoration:none;" ';
  txt += 'onmouseover="ChangeDecoration(\''+tabid+'\',true)" ';
  txt += 'onmouseout="ChangeDecoration(\''+tabid+'\',false)" >  ';
  txt += '<span id="'+tabid+'" style="color:#EFEFEF">'+descp+'</span></a></td>';
  return txt;
}

function genActionSeparator()
{
  var txt = '';
  txt += '<td class="tab"> </td>';
  return txt;
}

function homeOpenModalDialog(url, width, height)
{
  $.modal('<iframe src="html/' + url + '" height="'+height+'" width="'+width+'" style="border:0">', {
    containerCss:{
      height:height,
      width:width
    },
    zIndex:10000000
  });
}

function showlogofunction()
{
  var selectedLang = get_cookie("cookie_language");
  var txt = '';

  txt += '<img style="cursor:pointer" href="javascript:void();"  id="imgLogoHeader" '
  if ((IsUndefOrNull(selectedLang)) || (selectedLang == "defLang_ch"))
  {
	  txt += 'src="./images/logo/logo_ch.png"'
  }
  else
  {
	  txt += 'src="./images/logo/logo_ch.png"'
  }
  txt += 'height="55px">'
  
  return txt;
}