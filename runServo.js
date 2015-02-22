// This code will move the servo in several directions as proof of concept
var Cylon = require('cylon');

Cylon
  .robot()
  .connection('edison', { adaptor: 'intel-iot' })
  .device('servo', { driver: 'servo', pin: 3, connection: 'edison' })
  .on('ready', function(my) {
    var angle = 0;
    my.servo.angle(angle);
    setInterval(function(){
      angle = angle + 30;
      if(angle == 180){
          angle = 30; //reset position if servo angle is greater than 135 (i.e. 180)
      }
      my.servo.angle(angle);
      console.log("Servo Angle: "+angle);
    },500);
  });

Cylon.start();
