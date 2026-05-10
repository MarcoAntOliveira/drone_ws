/**
 * @file vel_contro.hpp
 * @author Vyshnav Achuthan (vyachu07@umd.edu)
 * @brief 
 * @version 0.1
 * @date 2024-04-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/bool.hpp"
#include "px4_msgs/msg/offboard_control_mode.hpp"
#include "px4_msgs/msg/trajectory_setpoint.hpp"
#include "px4_msgs/msg/vehicle_status.hpp"
#include "px4_msgs/msg/vehicle_attitude.hpp"
#include "px4_msgs/msg/vehicle_command.hpp"
#include "px4_msgs/srv/detect_aruco.hpp"
#include "math.h"
#include <rclcpp/clock.hpp> 

using namespace std::chrono_literals;
using namespace std::placeholders;

enum class State {
    IDLE,
    ARMING,
    TAKEOFF,
    SEARCH_GABARITO, // Novo
    SEARCH_BASE,     // Novo
    LANDING,         // Novo
    FINISHED,
    LOITER,
    OFFBOARD
};
class OffboardControl : public rclcpp::Node {
public:
    OffboardControl();
private:
    rclcpp::Subscription<px4_msgs::msg::VehicleStatus>::SharedPtr status_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr offboard_velocity_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleAttitude>::SharedPtr attitude_sub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr my_bool_sub_;
    rclcpp::Publisher<px4_msgs::msg::OffboardControlMode>::SharedPtr publisher_offboard_mode_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_velocity_;
    rclcpp::Publisher<px4_msgs::msg::TrajectorySetpoint>::SharedPtr publisher_trajectory_;
    rclcpp::Publisher<px4_msgs::msg::VehicleCommand>::SharedPtr vehicle_command_publisher_;
    rclcpp::TimerBase::SharedPtr arm_timer_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Client<px4_msgs::srv::DetectAruco>::SharedPtr client_;

    // Make sure these match what you have in your .cpp
    void state_loiter();   // Add this back if you still use it in the cpp
  
    void land();           // This was missing from your header
    void arm();
    void take_off();

    void arm_message_callback(const std_msgs::msg::Bool::SharedPtr msg);
    void arm_timer_callback();
    void state_init();
    void state_arming();
    void state_takeoff();
    void state_search_gabarito();
    void state_search_base();
    void state_landing();
    void state_finished();
    void state_offboard();
    

    void callbackCallbackGabarito(const px4_msgs::srv::DetectAruco::Response::SharedPtr msg);

    void publish_vehicle_command(uint16_t command, float param1 = 0.0, float param2 = 0.0, float param7 = 0.0);
    void vehicle_status_callback(const px4_msgs::msg::VehicleStatus::SharedPtr msg);
    void offboard_velocity_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
    void attitude_callback(const px4_msgs::msg::VehicleAttitude::SharedPtr msg);
    void cmdloop_callback();
    State current_state;
    State last_state;
    int myCnt;
    bool arm_message;
    bool flightCheck;
    bool offboardMode;
    bool failsafe;
    float yaw;
    float trueYaw;
    float target_x;
    float target_y;
    geometry_msgs::msg::Vector3 velocity;
    // px4_msgs::msg::VehicleStatus_<std::allocator<void>>::NAVIGATION_STATE_MAX nav_state;
    // px4_msgs::msg::VehicleStatus_<std::allocator<void>>::ARMING_STATE_ARMED arm_state;
    uint8_t nav_state;
    uint8_t arm_state;


};
