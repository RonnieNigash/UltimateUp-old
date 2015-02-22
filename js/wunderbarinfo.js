var relayr = RELAYR.init({
  appId: "c5eac84a-1511-4cda-8136-9af661257cf7"
});

relayr.devices().getDeviceData({
  deviceId: "c4e8751d-f9ff-4509-ba0c-98b78afb0576",
  token: "_C4efSMSUjtisJQybuiC5PN_pkVW.P-S" ,
  incomingData: function(data){
    console.log("sensor",data);
  }
});
