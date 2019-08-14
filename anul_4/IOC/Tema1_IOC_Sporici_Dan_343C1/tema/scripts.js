var lightsStatus = 0;

var currentLocation = "";
var currentCountry = "";
var currentChannel = 0;
var maxChannels = 3;

var _incOpacity = 0;

function switchLights()
{
    if (lightsStatus == 0)
    {
        document.getElementById("ls1").style.display = "initial";
        document.getElementById("ls2").style.display = "initial";
        lightsStatus = 1;
    }
    else
    {
        document.getElementById("ls1").style.display = "none";
        document.getElementById("ls2").style.display = "none";
        
        lightsStatus = 0;
    }
}


function _updateLocationCallback(jsonResponse)
{
    var response = JSON.parse(jsonResponse);
    
    currentLocation = response.city;
    countryName = response.country;
    
    document.getElementById("ip-address").textContent = response.query;
    
    getCurrentWeather();
    getAQCoefficient();
}

function _updateAQCallback(jsonResponse)
{
    var response = JSON.parse(jsonResponse);
    var nOfElems = response.results.length;
    
    var i = Math.floor(Math.random() * 100 - 1);
    
    document.getElementById("aqValue").textContent = response.results[i].city + " : " + response.results[i].value + response.results[i].unit;
}

function _updateWeatherCallback(jsonResponse)
{
    var response = JSON.parse(jsonResponse);
    
    document.getElementById("regionName").textContent = currentLocation + "/" + countryName;
    document.getElementById("weatherInfo").textContent = response.query.results.channel.item.condition.temp + "\u2109 (" +response.query.results.channel.item.condition.text + ")";
}

function getHttpResponse(url, callback)
{
    var xmlHttpRequest = new XMLHttpRequest();
    
    xmlHttpRequest.onreadystatechange = function() 
    {
        if (xmlHttpRequest.readyState == 4 && xmlHttpRequest.status == 200)
            callback(xmlHttpRequest.responseText);
    }
    
    xmlHttpRequest.open("GET", url, true);
    xmlHttpRequest.send(null);
}

function getCurrentWeather()
{
    getHttpResponse("https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22" + currentLocation + "%22)&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys", _updateWeatherCallback);
}

function getAQCoefficient()
{
    getHttpResponse("https://api.openaq.org/v1/measurements", _updateAQCallback);
}

function fadeAQInterface()
{
    var currentOpacity = parseFloat(document.getElementById("aq-info").style.opacity, 10);
    
    if (isNaN(currentOpacity))
        currentOpacity = 1;
    
    
    if (_incOpacity == 1)
    {
        if (currentOpacity + parseFloat('0.05', 10) <= parseFloat('0.7', 10))
            currentOpacity += parseFloat('0.05', 10);
        else
            _incOpacity = 0;
    }
    else
    {
        if (currentOpacity - parseFloat('0.05', 10) >= parseFloat('0.4', 10))
            currentOpacity -= parseFloat('0.05', 10);
        else
            _incOpacity = 1;
        
        
            
    }
   
    document.getElementById("aq-info").style.opacity = currentOpacity;
}

function updateLocation()
{
    getHttpResponse("http://ip-api.com/json", _updateLocationCallback);
    window.setInterval(fadeAQInterface, 120);
}

function setNextChannel()
{
    document.getElementById("tvchannel" + currentChannel).style.display = "none";
    currentChannel = (currentChannel + 1) % maxChannels;
    document.getElementById("tvchannel" + currentChannel).style.display = "initial";
}

function setPrevChannel()
{
    document.getElementById("tvchannel" + currentChannel).style.display = "none";
    currentChannel--;
    
    if (currentChannel < 0)
        currentChannel = maxChannels-1;
    else
        currentChannel %= maxChannels;
    
    document.getElementById("tvchannel" + currentChannel).style.display = "initial";
}

