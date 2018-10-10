// base
var prefixForService = "Service";
var activeCallsign = "Controller";
var memoryIntervals = {};
var memoryTimeoutId = null;
var updateDeviceInfoEnabled = true;
var webShellSocket = null;

window.addEventListener('hashchange', function (event) {
	updateDeviceInfoEnabled = (location.hash !== '#off');

	var pause = document.getElementById("pause-button");

	if (updateDeviceInfoEnabled) {
		pause.innerHTML = "pause";
	} else {
		pause.innerHTML = "resume";
	}

}, false);

function handleRequest(method, URL, body, handler) {
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open(method, URL, false);
	if (handler) {
		xmlHttp.onreadystatechange = function () {
			if (xmlHttp.readyState == 4)
				handler(xmlHttp.responseText);
		};
	}
	if (body !== null)
		xmlHttp.send(body);
	else
		xmlHttp.send();
}

function getHostname() {
	var hostName = document.getElementById("hostname") && document.getElementById("hostname").value;
	if ((hostName === null) || (hostName === ""))
		hostName = window.location.hostname;
	return (hostName);
}

function getPort() {
	var port = document.getElementById("port") && document.getElementById("port").value;

	if ((port === null) || (port === "")) {
		if (window.location.host === window.location.hostname)
			port = 80;
		else
			port = window.location.host.substring(window.location.hostname.length + 1);
	}

	return (port);
}

function getURLStart(protocol, addActiveCallsign) {
	var url = protocol + "://" + getHostname();
	var port = getPort();

	if ((port !== "") && (port !== 80))
		url += ":" + port;

	url += '/' + prefixForService + '/';
	if (addActiveCallsign)
		url += activeCallsign + '/';

	return url;
}

function reloadMenu() {
	var nav = document.getElementById('menu');
	while (nav.hasChildNodes())
		nav.removeChild(nav.firstChild);
	var ul = document.createElement("ul");
	var usedMenuItems = ["Controller", "DeviceInfo", "Provisioning", "WebKitBrowser", "Tracing", "Snapshot", "Netflix", "YouTube", "WebDriver", "WebShell", "OpenCDMi", "RemoteControl", "Compositor"];
	handleRequest('GET', getURLStart("http") + 'Controller/Plugins', null, function (items) {
		try {
			items = JSON.parse(items);
		} catch(err) {
			return;
		}
		for (var i = 0; i < items.plugins.length; i++) {
			if ((items.plugins[i].state === "activated" || items.plugins[i].state === "resumed") && usedMenuItems.indexOf(items.plugins[i].callsign) >= 0) {
				var li = document.createElement("li");
				li.id = "item_" + items.plugins[i].callsign;
				//var a = document.createElement("a");
				var name = items.plugins[i].callsign;
				if (name == 'DeviceInfo') name = 'Device';
				li.appendChild(document.createTextNode(name));
				li.setAttribute("onclick", "toggleMenuItem(" + "\"" + items.plugins[i].callsign + "\"" + ")");
				//li.appendChild(a);
				ul.appendChild(li);
				nav.appendChild(ul);
			}
			if (items.plugins[i].callsign == 'RemoteControl') {
				window.removeEventListener('keydown', handleKey);
				if ((items.plugins[i].state == 'activated') || (items.plugins[i].state === 'resumed')) window.addEventListener('keydown', handleKey);
				document.getElementById('keyPressedLabel').style.display = (items.plugins[i].state === 'activated' || items.plugins[i].state === 'resumed') ? 'inline' : 'none';
				document.getElementById('keyPressed').style.display = (items.plugins[i].state == 'activated' || items.plugins[i].state === 'resumed') ? 'inline' : 'none';
			}
		}
	});
}

function createMenu() {
	var navigation = document.getElementsByClassName('navigation')[0];
	var nav = document.createElement('ul');
	nav.id = 'menu';
	activeCallsign = "Controller";
	localStorage.clear();
	navigation.appendChild(nav);
	reloadMenu();
	toggleMenuItem(activeCallsign);
}

function updateCallSignInfo(prefix) {
	try {
		document.getElementById(prefix + '_hostname').innerHTML = getHostname();
		document.getElementById(prefix + '_port').innerHTML = getPort();
		document.getElementById(prefix + '_callsign').innerHTML = activeCallsign;
	} catch(e) {}
}

function toggleMenuItem(item) {
	if (!pluginInfo[item]) return;
	removeActiveClass();
	hideContainers();
	setActive(item);
	updateCallSignInfo(pluginInfo[item].infoPrefix);
	if (pluginInfo[item].method && typeof pluginInfo[item].method == 'function') pluginInfo[item].method();
}

function setActive(item) {
	activeCallsign = item;

	if(isTouchDevice()) {
		document.getElementById("menu").style.top = "-600px";
	}

	document.getElementById('item_' + activeCallsign).className = "active";
	document.getElementById('content_' + activeCallsign).className = "content grid show";
}

function removeActiveClass() {
	var lis = document.getElementsByTagName("li");

	for (var i = 0; i < lis.length; i++) {
		if (lis[i].className === "active") {
			lis[i].className = "";
		}
	}
}

function hideContainers() {
	var containers = document.getElementsByClassName("content");

	for (var i = 0; i < containers.length; i++) {
		if (containers[i].className === "content grid show") {
			containers[i].className = "content grid";
		}
	}
}

// controller
function toggleActivity(callsign) {
	var checkbox = document.getElementById(callsign);
	var div = document.getElementById(callsign + 'suspenddiv');
	var state = null;

	if (checkbox.checked) {
		handleRequest('PUT', getURLStart('http', true) + 'Activate/' + callsign, null);
		state = 'activated';
	}
	else {
		handleRequest('PUT', getURLStart('http', true) + 'Deactivate/' + callsign, null);
		state = 'deactivated';
	}

	//TODO @PIER(startsuspended)
	handleRequest('GET', getURLStart('http', true) + 'Plugin/' + callsign, null, function(data) {
		var data = JSON.parse(data);

		if ( (state === 'suspended') || (state === 'resumed') ) {
			div.className = 'suspend';
		} else {
			div.className = 'suspend hide-suspend';
		}
	});

	reloadMenu();
}
function toggleSuspend(id) {
	var checkbox = document.getElementById(id),
		label = document.getElementById(id + 'label');

	if (checkbox.checked) {
		label.innerHTML = 'resume';
		handleRequest('POST', getURLStart('http', false) + id.replace('suspend', '') + '/Suspend', null);
	} else {
		label.innerHTML = 'suspend';
		handleRequest('POST', getURLStart('http', false) + id.replace('suspend', '') + '/Resume', null);
	}
	reloadMenu();
}
function loadPlugins(pluginInfo, switchData) {
	var plugins = [];
	var switches = [];

	try{
		plugins = JSON.parse(pluginInfo).plugins;
	} catch(e) {}
	try{
		switches = JSON.parse(switchData).callsigns;
	} catch(e) {}
        var switchIndex = 0;
	var controllerPluginsCtr = document.getElementById("controllerPlugins");
	var controllerSwitchCtr = document.getElementById("controllerSwitchSelect");

	while (controllerPluginsCtr.hasChildNodes()) controllerPluginsCtr.removeChild(controllerPluginsCtr.firstChild);
	for (var i = 0; i < plugins.length; i++) {
		var callsign = plugins[i].callsign;
		if (callsign == 'Monitor' && plugins[i].configuration) updateMemoryIntervals(plugins[i].configuration);
		if (callsign != "Controller") {
			var controlAllowed = true;

			if (switches.find( function (label) { return (label === callsign) }) !== undefined) {

				switchIndex++;
				if (switchIndex > controllerSwitchCtr.options.length) {

					var newChild = controllerSwitchCtr.appendChild(document.createElement("option"));
					newChild.innerHTML = callsign;
				} else {
					controllerSwitchCtr.options[switchIndex - 1].innerHTML = callsign;
				}

				if ( (plugins[i].state === 'activated') || (plugins[i].state === 'resumed') ) {
					controllerSwitchCtr.value = callsign;
				}
				controlAllowed = false;

			}

			var labelDiv = document.createElement("div");
			labelDiv.className = "label grid__col grid__col--2-of-8";
			controllerPluginsCtr.appendChild(labelDiv);

			var label = document.createElement("label");
			label.innerHTML = callsign;
			label.setAttribute("for", callsign);
			labelDiv.appendChild(label);

			var div = document.createElement("div");
			div.className = "grid__col grid__col--6-of-8 ";
			div.id =  callsign + "MainDiv";
			controllerPluginsCtr.appendChild(div);

			var checkboxDiv = document.createElement("div");
			checkboxDiv.className = "checkbox";
			div.appendChild(checkboxDiv);

			var checkbox = document.createElement("input");
			checkbox.type = "checkbox";
			checkbox.id = callsign;
			if (plugins[i].state == "activated" || plugins[i].state == "resumed" || plugins[i].state == "suspended") {
				checkbox.checked = true;
			}
			if (controlAllowed === true) {
				checkbox.onclick = toggleActivity.bind(null, callsign);
			} else {
				checkbox.readOnly = true;
				checkbox.disabled = true;
				checkbox.disabled = true;
				checkbox.style = "cursor:default";
			}
			checkboxDiv.appendChild(checkbox);

			var checkboxLabel = document.createElement("label");
			checkboxLabel.setAttribute("for", callsign);
			checkboxDiv.appendChild(checkboxLabel);

			if ( (plugins[i].state === 'suspended') || (plugins[i].state === 'resumed') ) {
				var suspend = document.createElement("div");
				suspend.id = callsign + "suspenddiv";
				suspend.className = "suspend";
				var suspendCheckbox = document.createElement("input");
				suspendCheckbox.type = "checkbox";
				suspendCheckbox.id = callsign + "suspend";
				if (plugins[i].state == "suspended") {
					suspendCheckbox.checked = true;
				}
				if (controlAllowed === true) {
					suspendCheckbox.onclick = toggleSuspend.bind(null, callsign + "suspend");
				} else {
					suspendCheckbox.readOnly = true;
					suspendCheckbox.disabled = true;
					suspendCheckbox.style = "cursor:default";
				}
				suspend.appendChild(suspendCheckbox);

				var suspendLabel = document.createElement("label");
				suspendLabel.setAttribute("for", callsign + "suspend");
				suspendLabel.id = callsign + "suspendlabel";
				if (plugins[i].state == "suspended") {
					suspendLabel.innerHTML = "resume";
				} else {
					suspendLabel.innerHTML = "suspend";
				}
				suspend.appendChild(suspendLabel);

				div.appendChild(suspend);
			}
		}
	}
        if ((switches === undefined)  || (switches.length == 0) ) {
	    document.getElementById("controllerSwitch").style.display = "none";
	} else {
	    document.getElementById("controllerSwitch").style.display = "";
	}

	while (switchIndex < controllerSwitchCtr.options.length) {
		controllerSwitchCtr.Remove(controllerSwitchCtr.options.length - 1);
	}
}

function loadSwitch(pluginInfo) {
	handleRequest('GET', getURLStart('http') + 'Controller/Switch', null, function(data) {
		loadPlugins(pluginInfo, data);
	});
}

function switchChange(switchLabel) {
	handleRequest('PUT', getURLStart('http') + 'Controller/Switch/' + switchLabel, null, function(data) {
		handleRequest('GET', getURLStart("http") + "Controller/Plugins", null, loadSwitch);
	});
}

function updateMemoryIntervals(config) {
	if (!config || !config.observables || !Array.isArray(config.observables)) return;
	for (var i in config.observables)
		memoryIntervals[config.observables[i].callsign] = config.observables[i].interval;
}

function updateMemoryInfoInDiv(divId, callsign) {
	if (activeCallsign != callsign) return;
	if (memoryIntervals[callsign] && memoryIntervals[callsign] > 0) {
		if (memoryTimeoutId) clearTimeout(memoryTimeoutId);
		memoryTimeoutId = setTimeout(updateMemoryInfoInDiv.bind(null, divId, callsign), memoryIntervals[callsign]);
	}

	handleRequest('GET', getURLStart('http') + 'Monitor/' + callsign, null, function (data) {
		var div = document.getElementById(divId);
		while (div.hasChildNodes()) div.removeChild(div.firstChild);
		if (!data) return;
		try {
			data = JSON.parse(data);
		} catch(e) {
			return;
		}
		if (!data['resident']) return;

		var titleDiv = document.createElement('div');
		titleDiv.className = "title grid__col grid__col--8-of-8";
		titleDiv.innerHTML = "Memory";
		div.appendChild(titleDiv);

		for (var i in data['resident']) {
			var labelDiv = document.createElement('div');
			labelDiv.className = "label grid__col grid__col--2-of-8";
			div.appendChild(labelDiv);

			var label = document.createElement('label');
			label.innerHTML = i;
			labelDiv.appendChild(label);

			var text = document.createElement('div');
			text.className = "text grid__col grid__col--6-of-8";
			text.innerHTML = bytesToMbString(data['resident'][i]);
			div.appendChild(text);
		}

		var labelDiv = document.createElement('div');
		labelDiv.className = "label grid__col grid__col--2-of-8";
		div.appendChild(labelDiv);

		var label = document.createElement('label');
		label.innerHTML = 'measurements';
		labelDiv.appendChild(label);

		var text = document.createElement('div');
		text.className = "text grid__col grid__col--6-of-8";
		text.innerHTML = data['count'];
		div.appendChild(text);

		var labelDiv = document.createElement('div');
		labelDiv.className = "label grid__col grid__col--2-of-8";
		div.appendChild(labelDiv);

		label = document.createElement('label');
		label.innerHTML = 'process';
		labelDiv.appendChild(label);

		var text = document.createElement('div');
		text.className = "text grid__col grid__col--6-of-8";
		text.innerHTML = data['process']['last'];
		div.appendChild(text);

		var buttonDiv = document.createElement('div');
		buttonDiv.className = 'buttons';

		var labelDiv = document.createElement('div');
		labelDiv.className = "label grid__col grid__col--2-of-8";
		div.appendChild(labelDiv);

		var button = document.createElement('div');
		button.className = "text grid__col grid__col--6-of-8";
		div.appendChild(button);

		var reset = document.createElement("button");
		reset.innerHTML = "Reset";
		reset.type = "button";
		reset.onclick = resetMemoryData.bind(null, divId, callsign);
		button.appendChild(reset);
	});
}

function updateWebKitBrowserInfo(data) {
	if (typeof data.suspended === 'boolean')
		updatePluginStateInfo('WebKitBrowser', data.suspended);

	if (data.url)
		document.getElementById('webkit_current_url').innerHTML = data.url;

	document.getElementById('webkit_fps').innerHTML = data.fps || 0;

	if (typeof data.hidden === 'boolean') {
		document.getElementById('webkit_show').style.display = !data.hidden ? 'none' : '';
		document.getElementById('webkit_hide').style.display = !data.hidden ? '' : 'none';
	}
}

function updateNetflixInfo(data) {
	if (typeof data.suspended === 'boolean')
		updatePluginStateInfo('Netflix', data.suspended);
	if(data.esn)
		document.getElementById('netflix-esn').innerHTML = data.esn;
}

function updateYouTubeInfo(data) {
	if (typeof data.suspended === 'boolean')
		updatePluginStateInfo('YouTube', data.suspended);
}

function updatePluginStateInfo(callsign, suspended){
	var state = suspended ? 'SUSPENDED' : 'RESUMED';
	var container = document.getElementById(callsign + 'StateInfo');
	while (container.hasChildNodes()) container.removeChild(container.firstChild);

	var titleDiv = document.createElement('div');
	titleDiv.className = "title grid__col grid__col--8-of-8";
	titleDiv.innerHTML = "Memory";
	container.appendChild(titleDiv);

	var labelDiv = document.createElement("div");
	labelDiv.className = "label grid__col grid__col--2-of-8";
	container.appendChild(labelDiv);

	var label = document.createElement('label');
	label.innerHTML = 'Current state';
	labelDiv.appendChild(label);

	var div = document.createElement('div');
	div.className = "text grid__col grid__col--6-of-8";
	div.innerHTML = state.toLowerCase();

	var nextState = 'Suspend';
	if (state == 'SUSPENDED') nextState = 'Resume';

	container.appendChild(div);

	var labelDiv2 = document.createElement("div");
	labelDiv2.className = "label grid__col grid__col--2-of-8";
	container.appendChild(labelDiv2);

	var changeState = document.createElement("button");
	changeState.innerHTML = nextState;
	changeState.type = "button";
	changeState.onclick = changePluginState.bind(null, callsign, nextState);

	var div2 = document.createElement("div");
	div2.className = "grid__col grid__col--6-of-8";

	div2.appendChild(changeState);
	container.appendChild(div2);
}

function changePluginState(plugin, state){
	handleRequest('POST', getURLStart('http') + plugin + '/' + state, null, function () {});
}

function resetMemoryData(divId, callsign) {
	handleRequest('PUT', getURLStart('http') +'Monitor/' + callsign, null, function (data) {
		updateMemoryInfoInDiv(divId, callsign);
	});
}

function discover() {
	handleRequest('PUT', getURLStart('http') + "Controller/Discovery", null);

	setTimeout(function() {
		handleRequest('GET', getURLStart('http') + "Controller/Discovery", function(data) {
			var discoveryList = [];
			try { discoveryList = JSON.parse(data); } catch (e) {}

			var container = document.getElementById('discoveryList');
			while (container.hasChildNodes()) container.removeChild(container.firstChild);

			for (var i in discoveryList) {
				var li = document.createElement("li");
				li.innerHTML = i.Locator;
				container.appendChild(li);
			}
		});
	}, 1000);
}

function persist() {
	handleRequest('PUT', getURLStart('http') + "Controller/Persist", null);
}

function harakiri() {
	handleRequest('PUT', getURLStart('http') + "Controller/Harakiri", null);
}

var deviceInfo = null;
var deviceInfoTimeoutId = null;
function updateDeviceInfo() {
	if (deviceInfoTimeoutId) clearTimeout(deviceInfoTimeoutId);
	if (!updateDeviceInfoEnabled) return setTimeout(updateDeviceInfo, 1000);
	var disconnectedContainer = document.getElementById("disconnected");
	var xmlHttp = null;
	var theURL  = "http://" + getHostname() + ":" + getPort();
	theURL += "/Service/DeviceInfo";
	xmlHttp = new XMLHttpRequest();
	function changeDisconnectedContainerStyle(style){
		if(disconnectedContainer && disconnectedContainer.style.display !== style) disconnectedContainer.style.display = style;
	}

	xmlHttp.onload = function() {
		if (xmlHttp.readyState == 4) {
			deviceInfoTimeoutId = setTimeout(updateDeviceInfo, 1000);
			if (xmlHttp.status !== 200)
				changeDisconnectedContainerStyle("block");
			else {
				if(disconnectedContainer && disconnectedContainer.style.display === 'block') {
					pluginInfo[activeCallsign].method();
					toggleMenuItem("Controller");
					reloadMenu();
				}

				changeDisconnectedContainerStyle("none");
				deviceInfo = JSON.parse(xmlHttp.responseText);
				// jsonObj variable now contains the data structure.

				if(document.getElementById("statusBarVersion"))
					document.getElementById("statusBarVersion").innerHTML = deviceInfo.systeminfo.version;

				if(document.getElementById("statusBarSerial"))
					document.getElementById("statusBarSerial").innerHTML = deviceInfo.systeminfo.deviceid;

				if(document.getElementById("statusBarUptime"))
					document.getElementById("statusBarUptime").innerHTML = deviceInfo.systeminfo.uptime;

				if(document.getElementById("statusBarTotalRam"))
					document.getElementById("statusBarTotalRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalram);

				if(document.getElementById("statusBarUsedRam"))
					document.getElementById("statusBarUsedRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalram - deviceInfo.systeminfo.freeram);

				if(document.getElementById("statusBarGpuRamTotal"))
					document.getElementById("statusBarGpuRamTotal").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalgpuram);

				if(document.getElementById("statusBarGpuRamUsed"))
					document.getElementById("statusBarGpuRamUsed").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalgpuram - deviceInfo.systeminfo.freegpuram);

				if(document.getElementById("statusBarCpuLoad"))
					document.getElementById("statusBarCpuLoad").innerHTML = parseFloat(deviceInfo.systeminfo.cpuload).toFixed(1) + " %";
			}
		}
	};
	xmlHttp.open("GET", theURL, true);
	xmlHttp.timeout = 2000;
	xmlHttp.ontimeout = function() {
		updateDeviceInfo();
		changeDisconnectedContainerStyle("block");
	};
	xmlHttp.onerror = function(){
		deviceInfoTimeoutId = setTimeout(updateDeviceInfo, 2000);
	};
	xmlHttp.send();
}

// device information
var deviceMenuTimeoutId = null;
var selectedNetworkInterface;

function networkInterfaceChange(networkInterface) {
	var myData = deviceInfo.addresses.find(function(entry){ return (entry.name === networkInterface);});

	if (myData !== undefined) {
		selectedNetworkInterface = networkInterface;
		document.getElementById("MAC_ID").innerHTML = myData.mac;
		document.getElementById("IpAddress").innerHTML = myData.ip;
	}
}

function serviceRequest () {
	if (activeCallsign != 'DeviceInfo' && deviceInfo) return;
	if (deviceMenuTimeoutId) clearTimeout(deviceMenuTimeoutId);
	deviceMenuTimeoutId = setTimeout(serviceRequest, 5000);

	var names = document.getElementById("NetworkInterface");
	var index = 0;
	var found = false;

	deviceInfo.addresses.forEach(function (entry) {
		index++;
		if ( (selectedNetworkInterface === undefined) || (selectedNetworkInterface === "") ) {
			selectedNetworkInterface = entry.name;
			found = true;
		}
		else if ( (found == false) && (selectedNetworkInterface == entry.name) ) {
			found = true;
		}

		if (index > names.options.length) {

			var newChild = names.appendChild(document.createElement("option"));
			newChild.innerHTML = entry.name;
		} else {
			names.options[index - 1].innerHTML = entry.name;
		}
	})

	if (names.options.length > 0) {
		if (found == false) {
			selectedNetworkInterface = names.options[0].value;
		}
		names.value = selectedNetworkInterface;

		var myData = deviceInfo.addresses.find(function(entry){ return (entry.name === selectedNetworkInterface);});

		if (myData !== undefined) {
			document.getElementById("MAC_ID").innerHTML = myData.mac;
			document.getElementById("IpAddress").innerHTML = myData.ip;
		}
	}

	// Remove all left overs..
	while (index < names.options.length) {
		names.Remove(names.options.length - 1);
	}

	// jsonObj variable now contains the data structure.
	document.getElementById("DeviceName").innerHTML = deviceInfo.systeminfo.devicename;
	document.getElementById("SerialNumber").innerHTML = deviceInfo.systeminfo.serialnumber;
    document.getElementById("DeviceId").innerHTML = deviceInfo.systeminfo.deviceid;
	document.getElementById("Version").innerHTML = deviceInfo.systeminfo.version;
	document.getElementById("Uptime").innerHTML = deviceInfo.systeminfo.uptime;
	document.getElementById("TotalRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalram);
	document.getElementById("UsedRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalram - deviceInfo.systeminfo.freeram);
	document.getElementById("FreeRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.freeram);
	document.getElementById("TotalGpuRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalgpuram);
	document.getElementById("FreeGpuRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.freegpuram);
	document.getElementById("UsedGpuRam").innerHTML = bytesToMbString(deviceInfo.systeminfo.totalgpuram - deviceInfo.systeminfo.freegpuram);
	document.getElementById("CpuLoad").innerHTML = parseFloat(deviceInfo.systeminfo.cpuload).toFixed(1) + " %";
}

// webdriver
function WebDriverStatusInfo(data) {
	var state = data.suspended ? 'DEACTIVATED' : 'ACTIVATED';
	document.getElementById('webdriverstate').innerHTML = state.toLowerCase();
}

// opencdmi
function OpenCDMiStatusInfo(data) {
        var state = data.suspended ? 'DEACTIVATED' : 'ACTIVATED';
        document.getElementById('opencdmistate').innerHTML = state.toLowerCase();
}

// compositor {clients: [djkshgkd, sdfgsdg]}
function CompositorInfo(data) {
	console.log(data);
	try {
		compositor = JSON.parse(data);
	} catch(err) {
		compositor = {};
	}
	var clients = compositor.clients;

	if (clients.length > 0){
		var menu = document.getElementById('compositorClients');

    menu.innerHTML = "";

		var item = document.createElement('option');

		item.value = "";
		item.setAttributeNode(document.createAttribute("disabled"));
		item.setAttributeNode(document.createAttribute("selected"));
		item.innerHTML = "Select a client";
		menu.appendChild(item);

		for (var i = 0; i < clients.length; i++) {
			var item = document.createElement('option');
	    item.value = clients[i];
	    item.innerHTML = clients[i];
	    menu.appendChild(item);
		}
	}
	// document.getElementById('compositorClients').innerHTML = state.toLowerCase();
}

function compositorSetFocus(){
      var menu = document.getElementById('compositorClients');
			var client = menu.options[menu.selectedIndex].value;
			var fullURL = getURLStart('http', true) + client + '/Focus';
			console.log(fullURL);
			handleRequest('POST', fullURL, null, null);
}

function compositorSetTop(){
    var menu = document.getElementById('compositorClients');
    var client = menu.options[menu.selectedIndex].value;
    var fullURL = getURLStart('http', true) + client + '/Top';
    console.log(fullURL);
    handleRequest('POST', fullURL, null, null);
}

function compositorSetInput(){
    var menu = document.getElementById('compositorClients');
    var client = menu.options[menu.selectedIndex].value;
    var fullURL = getURLStart('http', true) + client + '/Input';
    console.log(fullURL);
    handleRequest('POST', fullURL, null, null);
}

function compositorSetOpacity(){
      var menu = document.getElementById('compositorClients');
			var client = menu.options[menu.selectedIndex].value;
			var opacity = document.getElementById('sliderOpacity').value;
			var fullURL = getURLStart('http', true) + client + '/Opacity/' + opacity ;
			console.log(fullURL);
			handleRequest('POST', fullURL, null, null);
}

function updateOpacity(val){
      document.getElementById('newOpacity').value=val;
}

function updateSliderOpacity(val){
      document.getElementById('sliderOpacity').value=val;
}

function compositorVisible(val){
    var menu = document.getElementById('compositorClients');
    var client = menu.options[menu.selectedIndex].value;
    var fullURL = getURLStart('http', true) + client + '/Visible/' + (val ? 'Show' : 'Hide') ;
    console.log(fullURL);
    handleRequest('POST', fullURL, null, null);
}

function compositorSetGeometry(){
    var menu = document.getElementById('compositorClients');
    var client = menu.options[menu.selectedIndex].value;
	var x = document.getElementById('compositorXGeometry').value
	var y = document.getElementById('compositorYGeometry').value
	var w = document.getElementById('compositorWidthGeometry').value
	var h = document.getElementById('compositorHeightGeometry').value
    var fullURL = getURLStart('http', true) + client + '/Geometry/' + x + '/' + y + '/' + w + '/' + h;
    console.log(fullURL);
    handleRequest('POST', fullURL, null, null);
}



// provisioning
function StatusInfo(response) {
	var id = '',
		status = 0,
		tokens = [];
	try{
		var provisioningData = JSON.parse(response);
		status = provisioningData.status;
		id = provisioningData.id;
		tokens = provisioningData.tokens || [];
	} catch(e){
	}

	document.getElementById('device').innerHTML = id;
	document.getElementById('status').innerHTML = (status == 200) ? 'provisioned' : 'not provisioned';
	if (status == 200 && tokens.length > 0) {
		document.getElementById('provisioning-tokens').innerHTML = tokens.join(', ');
	}
	document.getElementById('provisionButton').style.display = (status == 200) ? 'none' : null;
	document.getElementById('provisionLabel').style.display = (status == 200) ? 'none' : null;
}

// remote control
function ActivatePairing() {
	var fullURL = getURLStart('http', true) + '/PairingMode';
	handleRequest('PUT', fullURL, null, null);
}
function RemoteControllerStatusInfo(response) {
	var remotes;
	try {
		remotes = JSON.parse(response);
	} catch(err) {
		remotes = {};
	}
	var names = Object.keys(remotes);
	if (names.length) {
		var form = document.getElementById("remoteControl_form");

		if (form.children.length > 1) {
			form.removeChild(form.lastChild);
		}

		for (var i = 0; i < names.length; i++) {
			var keys = Object.keys(remotes[names[i]]);
			var fieldset = document.createElement("fieldset");
			form.appendChild(fieldset);

			var legend = document.createElement("legend");
			legend.innerHTML = names[i];
			fieldset.appendChild(legend);

			for (var j = 0; j < keys.length; j++) {
				var label = document.createElement("label");
				label.innerHTML = keys[j];
				fieldset.appendChild(label);

				var p = document.createElement("p");
				p.innerHTML = remotes[names[i]][keys[j]];
				fieldset.appendChild(p);
			}

			if (names[i] == "GreenPeakRF4CE") {
				var div = document.createElement("div");
				div.className = "buttons";
				fieldset.appendChild(div);

				var button = document.createElement("button");
				button.innerHTML = "Pair remote";
				button.type = "button";
				button.setAttribute("onclick", "ActivatePairing()");

				div.appendChild(button);
			}
		}
	}
}

var keyMap = {
	13: { code: '0x002B', str: 'enter' },
	37: { code: '0x0003', str: 'left' },
	38: { code: '0x0001', str: 'up' },
	39: { code: '0x0004', str: 'right' },
	40: { code: '0x0002', str: 'down' },
	27: { code: '0x0009', str: 'esc' },
	8: { code: '0x0032', str: 'backspace' },
	48: '0x0020', // 0
	49: '0x0021', // 1
	50: '0x0022', // 2
	51: '0x0023', // 3
	52: '0x0024', // 4
	53: '0x0025', // 5
	54: '0x0026', // 6
	55: '0x0027', // 7
	56: '0x0028', // 8
	57: '0x0029', // 9
	33: '0x0030', // page up
	34: '0x0031', // page down
	65: '0x8004', // a
	66: '0x8005', // b
	67: '0x8006', // c
	68: '0x8007', // d
	69: '0x8008', // e
	70: '0x8009', // f
	71: '0x800A', // g
	72: '0x800B', // h
	73: '0x800C', // i
	74: '0x800D', // j
	75: '0x800E', // k
	76: '0x800F', // l
	77: '0x8010', // m
	78: '0x8011', // n
	79: '0x8012', // o
	80: '0x8013', // p
	81: '0x8014', // q
	82: '0x8015', // r
	83: '0x8016', // s
	84: '0x8017', // t
	85: '0x8018', // u
	86: '0x8019', // v
	87: '0x801A', // w
	88: '0x801B', // x
	89: '0x801C', // y
	90: '0x801D', // z
	46: '0x802A', // delete
	32: { code: '0x802C', str: 'space' },
	189: '0x802D', // minus
	187: '0x802E', // equal
	220: '0x8031', // backslash
	186: '0x8033', // semicolon
	222: '0x8034', // apostrophe
	188: '0x8036', // comma
	190: '0x8037', // dot
	191: '0x8038'  // slash
};

function handleKey(e) {
	if(e.keyCode === 13 && ((activeCallsign === 'WebKitBrowser' && document.activeElement == document.getElementById('webkit_url')) || (activeCallsign === 'WebShell' && document.activeElement == document.getElementById('webShellInput')))) {
		e.preventDefault();
		if (activeCallsign === 'WebKitBrowser')
			SetNewURL(document.getElementById('webkit_url').value);
		else
			sendWebShellCommand();
		return;
	}

	// don't send key's if a textbox is selected.
	if (document.activeElement.nodeName == 'TEXTAREA' || document.activeElement.nodeName == 'INPUT') {
                 return;
        }

	var map = keyMap[e.keyCode] || keyMap[e];
	var code = null,
		preventDefault = false,
		str = null;
	if (typeof map == 'object') {
		code = map.code;
		str = map.str;
		preventDefault = true;
	} else {
		code = map;
		str = String.fromCharCode(e.keyCode);
	}

	if (code && str) {
		document.getElementById('keypress').innerHTML = code;
		handleRequest('PUT', getURLStart('http') + 'RemoteControl/keymap/Code/' + code, null, null);
		document.getElementById('keyPressed').innerHTML = str;
		if (preventDefault && e.keyCode) e.preventDefault();
	}
}

// tracing

var modules = {};
function toggleTrace(module, id) {
	if ((modules[module] !== undefined) && (modules[module].traces !== undefined) && (modules[module].traces[id] !== undefined)) {
		var fullURL = getURLStart('http') + 'Tracing' +  '/' + module + '/' + id + '/';
		if (modules[module].traces[id]) {
			handleRequest('PUT', fullURL + 'off', null, null);
			modules[module].traces[id] = false;
		} else {
			handleRequest('PUT', fullURL + 'on', null, null);
			modules[module].traces[id] = true;
		}
	}
}
function showTraceCategories(module) {
	if ((modules[module] !== undefined) && (modules[module].traces !== undefined)) {
		var tracingDiv = document.getElementById("tracing_div");

		while (tracingDiv.children.length > 0) {
			tracingDiv.removeChild(tracingDiv.children[0]);
		}

		for (var trace in modules[module].traces) {
			var labelDiv = document.createElement("div");
			labelDiv.className = "label grid__col grid__col--2-of-8";
			tracingDiv.appendChild(labelDiv);

			var label =  document.createElement("label");
			label.innerHTML = trace;
			label.setAttribute("for", trace);
			labelDiv.appendChild(label);

			var div = document.createElement("div");
			div.className = "grid__col grid__col--6-of-8";
			tracingDiv.appendChild(div);

			var checkboxDiv = document.createElement("div");
			checkboxDiv.className = "checkbox";
			div.appendChild(checkboxDiv);

			var checkbox = document.createElement("input");
			checkbox.type = "checkbox";
			checkbox.id = trace;
			checkbox.checked = modules[module].traces[trace];
			checkbox.onclick =  toggleTrace.bind(null, module,trace);
			checkboxDiv.appendChild(checkbox);

			var checkboxLabel = document.createElement("label");
			checkboxLabel.setAttribute("for", trace);
			checkboxDiv.appendChild(checkboxLabel);
		}
	}
}
function updateModules(response) {
	var myPage = document.getElementById('modules'),
		entries = myPage.getElementsByTagName('option'),
		index = 0,
		plugins = JSON.parse(response);
	if ((plugins !== undefined) && (plugins.settings !== undefined)) {
		modules = {};
		// Replace and extend the current set
		plugins.settings.forEach(function (entry) {
			if (modules[entry.module] === undefined) {
				modules[entry.module] = {};
				modules[entry.module].traces = {};
				if (index >= entries.length) {
					myPage.appendChild(document.createElement("option"));
				}
				entries[index].innerHTML = entry.module;
				index++;
			}
			modules[entry.module].traces[entry.category] = (entry.state !== undefined) && (entry.state === "enabled");

		});
		// Remove all from the current set that is not needed
		while (index < entries.length) {
			myPage.removeChild(entries[entries.length - 1]);
		}
		showTraceCategories(myPage.value, "tracecategories");
	}
}

// webkitbrowser

var webInfo = [
	{ Name:"Select a preset", URL:""},
	{ Name:"about:blank",       URL:"about:blank"},
	{ Name:"Smashcat",      URL:"http://www.smashcat.org/av/canvas_test/" },
	{ Name:"HTML5",         URL:"http://beta.html5test.com/" },
	{ Name:"PeaceKeeper",   URL:"http://peacekeeper.futuremark.com/run.action" },
	{ Name:"ChipTune",      URL:"http://www.chiptune.com/kaleidoscope/" },
	{ Name:"Poster Circle", URL:"http://www.webkit.org/blog-files/3d-transforms/poster-circle.html" },
	{ Name:"Aquarium",      URL:"http://webglsamples.org/aquarium/aquarium.html" },
	{ Name:"Particles",     URL:"http://oos.moxiecode.com/js_webgl/particles_morph/" },
	{ Name:"MSE 2016",      URL:"http://yt-dash-mse-test.commondatastorage.googleapis.com/unit-tests/2016.html?enablewebm=off" },
	{ Name:"EME 2016",      URL:"http://yt-dash-mse-test.commondatastorage.googleapis.com/unit-tests/2016.html?test_type=encryptedmedia-test&enablewebm=off" },
	{ Name:"Progressive",   URL:"http://yt-dash-mse-test.commondatastorage.googleapis.com/unit-tests/tip.html?test_type=progressive-test&enablewebm=off" },
	{ Name:"YouTube",       URL:"http://youtube.com/tv" },
	{ Name:"HelloRacer",    URL:"http://helloracer.com/webgl" },
	{ Name:"Leaves",        URL:"http://www.webkit.org/blog-files/leaves" },
	{ Name:"Canvas Dots",   URL:"http://themaninblue.com/experiment/AnimationBenchmark/canvas/" },
	{ Name:"Anisotropic",   URL:"http://whiteflashwhitehit.com/content/2011/02/anisotropic_webgl.html" },
	{ Name:"Pasta",         URL:"http://alteredqualia.com/three/examples/webgl_pasta.html" },
	{ Name:"CSS3",          URL:"http://css3test.com" },
	{ Name:"Kraken",        URL:"http://krakenbenchmark.mozilla.org/kraken-1.1/driver.html" },
	{ Name:"KeyPress Test", URL:"http://www.asquare.net/javascript/tests/KeyCode.html" }

];
function SetNewURL(newURLToSet) {
	if (newURLToSet != "") {
		var body = '{"url":"' + newURLToSet + '"}';
		handleRequest('POST', getURLStart('http') + activeCallsign + '/URL', body, null);
	}

	document.getElementById("linkPresets").selectedIndex = 0;
}

function browserState(state) {
	handleRequest('POST', getURLStart('http') + activeCallsign + '/' + (state ? 'Show' : 'Hide'), null, null, null);
}

function loadActiveCallsignData(cb) {
	handleRequest('GET', getURLStart('http', true), null, cb);
}

function bytesToMbString(bytes) {
	return (bytes / 1024 / 1024).toFixed(1) + ' MB';
}

function snapshot() {
	document.getElementById('snapshotOutput').src = '';
	document.getElementById('snapshotOutput').src = getURLStart("http", true) +'Capture?' + new Date().getTime();
}

function tiggerProvisioningRequest() {
	handleRequest('PUT', getURLStart("http", true) , null,function(){
		document.getElementById('provisionButton').style.display = 'none';
		document.getElementById('provisionLabel').style.display = 'none';
		setTimeout(loadActiveCallsignData.bind(null, StatusInfo), 1000);
	}, null);
}

function loadDefaultDataAndMemory(callsign){
	updateMemoryInfoInDiv(callsign + 'Memory', callsign);
	handleRequest('GET', getURLStart('http', true) ,null, function(d){
		try{
			pluginInfo[callsign].socketFn(JSON.parse(d));
		}catch(e){};
	});
}

var pluginInfo = {
	'Controller': {
		infoPrefix: 'controller',
		method: handleRequest.bind(null, 'GET', getURLStart("http", true) + "Plugins", null, loadSwitch)
	},
	'DeviceInfo': {
		infoPrefix: 'deviceinfo',
		method: serviceRequest
	},
	'Provisioning': {
		infoPrefix: 'provisioning',
		method: loadActiveCallsignData.bind(null, StatusInfo)
	},
	'RemoteControl': {
		infoPrefix: 'remotecontrol',
		method: loadActiveCallsignData.bind(null, RemoteControllerStatusInfo)
	},
	'Tracing': {
		infoPrefix: 'tracing',
		method: loadActiveCallsignData.bind(null, updateModules)
	},
	'WebKitBrowser': {
		infoPrefix: 'webkit',
		socketFn: updateWebKitBrowserInfo,
		method: function () {
			var parent = document.getElementById('linkPresets');
			if (parent.children.length == 0) {
				for (var i = 0; i < webInfo.length; i++) {
					var option = document.createElement('option');
					option.text = webInfo[i].Name;
					option.value = webInfo[i].URL;

					parent.appendChild(option);
				}
			}
			loadDefaultDataAndMemory('WebKitBrowser');
		}
	},
	'Netflix': {
		infoPrefix: 'netflix',
		socketFn: updateNetflixInfo,
		method: loadDefaultDataAndMemory.bind(null, 'Netflix')
	},
	'YouTube': {
		infoPrefix: 'youtube',
		socketFn: updateYouTubeInfo,
		method: loadDefaultDataAndMemory.bind(null, 'YouTube')
	},
	'Snapshot': {
		infoPrefix: 'snapshot',
		method: function () {}
	},
	'WebDriver': {
		infoPrefix: 'webdriver',
		method: loadActiveCallsignData.bind(null, WebDriverStatusInfo)
	},
	'WebShell': {
		method: function(){
			if(!webShellSocket) {
				webShellSocket = new WebSocket('ws://' + getHostname() + ':' + getPort() + '/Service/WebShell', 'raw');
				webShellSocket.onmessage = function(e){
					var fileReader = new FileReader();
					fileReader.onload = function() {
						document.getElementById('webShellData').innerHTML = String.fromCharCode.apply(null, new Uint8Array(this.result));
					};
					fileReader.readAsArrayBuffer(e.data);
				};
				webShellSocket.onclose = function(){
					webShellSocket = null;
				}
			}
			document.getElementById("webShellInput").focus();
		}
	},
	'OpenCDMi': {
		infoPrefix: 'opencdmi',
		method: loadActiveCallsignData.bind(null, OpenCDMiStatusInfo)
	},
	'Compositor' : {
		infoPrefix: 'compositor',
		method: loadActiveCallsignData.bind(null, CompositorInfo)
	}
};

function sendWebShellCommand() {
	if(webShellSocket) {
		var str = document.getElementById('webShellInput').value + ' \n';
		var buf = new ArrayBuffer(str.length*2);
		var bufView = new Uint8Array(buf);
		for (var i=0, strLen=str.length; i<strLen; i++) {
			bufView[i] = str.charCodeAt(i);
		}

		document.getElementById('webShellInput').value = '';
		webShellSocket.send(buf);
	}

	setTimeout(function() {
		var shell = document.getElementById("shell");
		var webShellData = document.getElementById("webShellData");

		shell.scrollTop = webShellData.scrollHeight;
	}, 50);
}

function webInspector() {
	var url = "http://" + getHostname() + ":9998";
	var win = window.open(url, '_blank');
	win.focus();
}


function initWebsocket() {
	var socket = new WebSocket(getURLStart('ws', false) + 'Controller', 'notification');
	socket.onmessage = function(evt){
		var json = {};
		try{
			json = JSON.parse(evt.data);
		}catch(e){return;}

		if(json && json.data && json.callsign && pluginInfo[json.callsign] && pluginInfo[json.callsign].socketFn)
			pluginInfo[json.callsign].socketFn(json.data);
	};


	socket.onclose = function() {
		setTimeout(initWebsocket, 2000);
	};
}

function showMenu() {
	var menu = document.getElementById("menu");

	if (menu.style.top === "60px") {
		menu.style.top = "-600px";
	} else {
		menu.style.top = "60px";
	}
}

function showKeyboard() {
	var remote = document.getElementById("remote");

	if (remote.style.bottom === "0px") {
		remote.style.bottom = "-600px";
	} else {
		remote.style.bottom = "0px";
	}
}

function pause() {
	var pause = document.getElementById("pause-button");

	if (updateDeviceInfoEnabled) {
		updateDeviceInfoEnabled = false;
		pause.innerHTML = "resume";
	} else {
		updateDeviceInfoEnabled = true;
		pause.innerHTML = "pause";
	}
}

function isTouchDevice() {
	try {
		document.createEvent("TouchEvent");
		document.getElementsByTagName("BODY")[0].className = "touch";
		return true;
	} catch (e) {
		document.getElementsByTagName("BODY")[0].className = "desktop";
		return false;
	}
}

window.onresize = function() {
	if (!isTouchDevice()) {
		if (window.innerWidth > 960) {
			document.getElementById("menu").style.top = "60px";
		} else {
			document.getElementById("menu").style.top = "-600px";
		}
	}
};

initWebsocket();
updateDeviceInfo();