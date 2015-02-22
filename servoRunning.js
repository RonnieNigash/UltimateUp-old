"use strict";

var Cylon = require("cylon");
var servoPin = 3;
var analogPin = 2;

Cylon.robot({
  connections: {
    edison: { adaptor: "intel-iot"}
  },

  devices: {
    servo: { driver: "servo", pin: servoPin },
    sensor: { driver: "analogSensor", pin: analogPin, lowerLimit: 0, upperLimit:1024}
  },

  work: function(my) {
    var angle = 0,
    prevAngle = angle,
    updateCap = 0,
    beenCalm = 0;

    every((0.30).second(), function() {
      angle = Math.floor(my.sensor.analogRead() / 57) * 10;
      console.log('%d', angle);
      if (angle != prevAngle && updateCap < 10) {
        my.servo.angle(angle);
        prevAngle = angle;
      } else {
	beenCalm++;
	if (beenCalm > 4) {
	  updateCap = 0;
	  beenCalm = 0;
	}
      }
    });
  }
}).start();
