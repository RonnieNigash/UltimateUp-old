var relayr = RELAYR.init({
  appId: ""
});

relayr.devices().getDeviceData({
  deviceId: "",
  token: token,
  incomingData: function(data){
    console.log("sensor",data);
  }
});
