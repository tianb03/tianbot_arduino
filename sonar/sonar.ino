#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

#define ECHO  2
#define TRIG  11

float distance = -1.0;

ros::NodeHandle  nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);

char frameid[] = "/ultrasound";

void setup()
{
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  nh.initNode();
  nh.advertise(pub_range);

  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  frameid;
  range_msg.field_of_view = 0.1;  // fake
  range_msg.min_range = 0.0;
  range_msg.max_range = 5;
}

long range_time;

void loop() {
  if ( millis() >= range_time ){
    int r =0;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    distance = pulseIn(ECHO, HIGH) / 58.00;
    distance = constrain(distance,0,500);
    range_msg.range = distance/100;
    range_msg.header.stamp = nh.now();
    pub_range.publish(&range_msg);
    range_time =  millis() + 50;
  }
  
  nh.spinOnce();
  distance = -1.0;
}
