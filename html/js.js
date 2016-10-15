function Run(s){
	//alert(window.location.pathname)
	var relpath = window.location.pathname.split('/');
	relpath.shift();
	relpath.pop();
	relpath = relpath.join('/') + '/'
	var Shell = new ActiveXObject("WScript.Shell");
	Shell.Run(relpath+s);
}

