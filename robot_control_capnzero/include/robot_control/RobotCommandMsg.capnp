# ReceiverID 0 is reserved as wildcard for all robots. If it is > 0, it represents the ID of the robot.
id_ros/ID receiverId

struct RobotCommandMsg {
    cmd @0 :UInt8;
}

# The following commands are defined
const start :UInt8 = 0;
const stop :UInt8 = 1;

