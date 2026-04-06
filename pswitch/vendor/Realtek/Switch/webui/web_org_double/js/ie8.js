function IsUndefOrNull(obj) {
	if (typeof obj == "undefined" || obj == null || obj == "") {
		return true;
	} else {
		return false;
	}
}
function trimBoth(str) { 
return str.replace(/(^\s*)|(\s*$)/g, "");
}


function ltrim(str) {
return str.replace(/(^\s*)/g, "");
}


function rtrim(str) { 
return str.replace(/(\s*$)/g, "");
}