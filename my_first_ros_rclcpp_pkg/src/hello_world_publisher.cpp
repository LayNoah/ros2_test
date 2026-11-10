#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/*
# 클래스 정의
class MyPublisher : public rclcpp::Node
{

}
*/

/*
class MyPublisher : public rclcpp::Node
{
public:
    # 생성자
    MyPublisher() :
    Node("MyPublisher"), count_(0)
    {
    }

}
*/

/*
class MyPublisher : public rclcpp::Node
{
public:
    # 생성자
    MyPublisher()
    : Node("MyPublisher"), count_(0)
    {
        auto qos = rclcpp::QoS(rclcpp::KeepLast(10));

        ma_publisher = this->create_publisher<std_msgs::msg::String>(
            "hello", qos
        );

        timer_ = this->create_wall_timer(
            1s, std::bind(&MyPublisher::publish_ma_msg, this)
        );
    }
}
*/

/*
class MaPublisher : public rclcpp::Node
{
public:
    MaPublisher()
    : Node("MaPublisher"), count_(0)
    {
        qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));

        ma_publisher = this->create_publisher<std_msgs::msg::String>(
            "hello", qos_profile
        );

        timer = this->create_wall_timer(
            1s, std::bind(&MaPublisher::pub_ma_msg, this)
        );
    }
}
*/

class HelloworldPublisher : public rclcpp::Node
{
    public:
        HelloworldPublisher()
        : Node("HelloworldPublisher"), count_(0)
        {
            auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
            helloworld_publisher = this->create_publisher<std_msgs::msg::String>(
                "Hello World!", qos_profile
            );
            timer_ = this->create_wall_timer(
                1s, std::bind(&HelloworldPublisher::publish_helloworld_msg, this)
            );
        }

    private:
        void publish_helloworld_msg()
        {
            auto msg = std_msgs::msg::String();
            msg.data = "Hello World: " + std::to_string(count++);
            RCLCPP_INFO(this->get_logger(), "Published message : '%s'", msg.data.c_str());
            helloworld_publisher_->publish(msg)
        }

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr helloworld_publisher;
        size_t count_;
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<HelloWorldPublisher>;
    rclcpp::spin(Node);
    rclcpp::shutdown();
    return 0;
}
