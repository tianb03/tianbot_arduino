#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include <Servo.h>

#define ECHO  2
#define TRIG  11

Servo myservo;
float distance = -1.0;

ros::NodeHandle  nh;

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;
char base_link[] = "/base_link";

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/ultrasound", &range_msg);

char ultrasound[] = "/ultrasound";

float deg = 0;
float yaw = 0.0;

void setup()
{
  myservo.attach(9);
  
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  nh.initNode();
  nh.advertise(pub_range);
  broadcaster.init(nh);
  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id =  ultrasound;
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

  t.header.frame_id = base_link;
  t.child_frame_id = ultrasound;
  t.transform.translation.x = 0.0; 
  yaw = (deg-90)/180*3.14159;
  //Serial.println(yaw);
  t.transform.rotation = tf::createQuaternionFromYaw(yaw);
  t.header.stamp = nh.now();
  broadcaster.sendTransform(t);
  nh.spinOnce();


  myservo.write(deg); 
  delay(50);  
  deg= deg + 5;

  if(deg==180)
  {
    deg = 0;
    myservo.write(deg); 
    delay(400);
  }
  
  
  distance = -1.0;
}
