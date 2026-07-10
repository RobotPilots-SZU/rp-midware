#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <etl/message_bus.h>
#include <etl/message_router.h>


LOG_MODULE_REGISTER(breeze_middleware, LOG_LEVEL_INF);

// 消息 ID
enum class MessageType : uint8_t {
    TYPE_TEST_A = 0x01,
    TYPE_TEST_B = 0x02,
};

// 路由器 ID
constexpr etl::message_router_id_t ROUTER_TEST = 1U;

// 消息结构
struct TestMessage : public etl::message<static_cast<etl::message_id_t>(MessageType::TYPE_TEST_A)> {
    int value;

    explicit TestMessage(int value_)
      : value(value_)
    {
    }
};

// 消息总线实例
etl::message_bus<2> messageBus;
class TestRouter : public etl::message_router<TestRouter, TestMessage>
{
public:
    using base_t = etl::message_router<TestRouter, TestMessage>;

    TestRouter()
      : base_t(ROUTER_TEST)
    {
    }

    // 每支持一种消息类型，就写一个同名重载
    void on_receive(const TestMessage& msg)
    {
        LOG_INF("Received TestMessage with value: %d", msg.value);
    }

    // 收到未在模板参数列表中声明的消息类型时调用
    void on_receive_unknown(const etl::imessage& msg)
    {
        LOG_WRN("Unknown message id: %u",
                static_cast<unsigned>(msg.get_message_id()));
    }
};



int main(void) {
    LOG_INF("Breeze Middleware Sample Started");

    TestRouter router;
    messageBus.subscribe(router);    // 订阅到 messageBus

    TestMessage msg{42};
    messageBus.receive(msg);         // 会路由到 TestRouter::on_receive

    while (true) {
        k_sleep(K_SECONDS(1));
    }
    return 0;
}