# Masstransit_cpp

AMQP message publishing and consuming library. Like and a little compatible with Masstransit for .Net.

### Quick start

#### Create message struct

It is necessary to implement `ctor` from `nlohmann::json`, static method `message_type()->std::string` and serialize method `to_json()->nlohmann::json`.

```cpp
#include <masstransit_cpp/json.hpp>

struct my_message {
	int data_1;
	std::string data_2;

	my_message(nlohmann::json const& obj);
	
	static std::string message_type() { return "MyMessage"; }

	nlohmann::json to_json() { return { {"data_1", data_1}, {"data_2", data_2} }; }
};
```

#### Create message consumer

Consumer have to extend `masstrasit_cpp::message_consumer<message_t>` and override `void consume(masstrasit_cpp::consume_context<message_t> const&)`.

```cpp
#include <my_message.hpp>
#include <masstransit_cpp/message_consumer.hpp>
#include <iostream>

class my_message_consumer : public masstrasit_cpp::message_consumer<my_message> {

public:
	~my_message_consumer() override {}
	
	void consume(masstrasit_cpp::consume_context<my_message> const& context) override {
	    std::cout << "Consume " << context.message.data_1 << ":" << context.message.data_2 << std::endl;
	}
};
```

#### Configure and start bus

```cpp
#include <my_consumer.hpp>
#include <masstransit_cpp/bus.hpp>

using namespace masstransit_cpp;

void main() {

auto one_consumer = std::make_shared<my_consumer>();

auto bus = masstransit_cpp::bus_factory::create_using_rabbit_mq([=](masstransit_cpp::rabbit_mq_configurator & bus_configurator) {
    		
    			auto host = bus_configurator.host(masstransit_cpp::amqp_uri("127.0.0.1"), [](auto & host_configurator) {
    				host_configurator.username("user");
    				host_configurator.password("password");
    			});
    
    			bus_configurator.receive_endpoint(host, "QueueName", [=](masstransit_cpp::rabbit_mq::receive_endpoint_configurator & endpoint_configurator) {
    				endpoint_configurator.consumer<my_message>(one_consumer);   // set consumer for message
    				
    				endpoint_configurator.handler<my_message>(                  // OR set handler for message
    				                [=](mtc::consume_context<my_message> const& context) {
			                            std::cout << "Handle " << context.message.data_1 << ":" << context.message.data_2 << std::endl;
		                            });
    			});
    		});

    b->start();
//  ... wait ...
    b->stop();
}
```

#### Publish message

Bus implement interface `masstransit_cpp::i_bus` and has template method `void publish<message_t>(message_t const&)`.
You need pass your configured `bus` as dependence where you want to publish message.

```cpp
void publish_foo(std::shared_ptr<i_publish_endpoint> const& bus) {
    my_message m;
    
    m.data_1 = 42;
    m.data_2 = "qwerty";
    
    std::future<bool> task = bus->publish(m);
}
```

#### Configure consumers using Boost.DI

If you use dependency injection, your consumers may depends on bus (handle some message and publish new one). You can configure bus use Boost.DI.

```cpp
#include <my_message.hpp>
#include <masstransit_cpp/bus.hpp>
#include <masstransit_cpp/message_consumer.hpp>
#include <boost/di.hpp>

struct next_message {
	int data;

    next_message(int data) : data(data) {}
	next_message(nlohmann::json const& obj);
	
	static std::string message_type() { return "NextMessage"; }

	nlohmann::json to_json() { return { {"data", data} }; }
};

class dependent_consumer : public masstrasit_cpp::message_consumer<my_message> {

    std::shared_ptr<masstrasit_cpp::i_publish_endpoint> bus_;
    
public:
    dependent_consumer(std::shared_ptr<masstrasit_cpp::i_publish_endpoint> const& bus)
        : bus_(bus) {}
	~my_message_consumer() override {}
	
	void consume(masstrasit_cpp::consume_context<my_message> const& context) override {
	    bus_->publish(next_message());
	}
};


template<class injector_t>
std::shared_ptr<masstrasit_cpp::bus> create_bus(injector_t const& injector) {   // method to create bus using injector

	return masstrasit_cpp::bus_factory::create_using_rabbit_mq([&injector](masstrasit_cpp::rabbit_mq_configurator & bus_configurator) {
		auto host = bus_configurator.host(masstrasit_cpp::amqp_uri("localhost"), [](masstrasit_cpp::amqp_host_configurator & host_configurator) {
			host_configurator.username("user");
			host_configurator.password("password");
		});

		bus_configurator.receive_endpoint(host, "QueueName", 
		        [&injector](masstrasit_cpp::rabbit_mq::receive_endpoint_configurator & endpoint_configurator) {
		                                                                        // load consumer for my_message from injector,
		            endpoint_configurator.load_from<my_message>(injector);      // i.e. impl of masstrasit_cpp::message_consumer<my_message>
        		});                                                             
	});
}

void main() {
    auto container = boost::di::make_injector(
			boost::di::bind<masstrasit_cpp::message_consumer<my_message>, dependent_consumer>().to<dependent_consumer>(),
			boost::di::bind<mtc::bus>().to([](auto const& injector) { return create_bus(injector); })
		);

		auto bus = container.create<std::shared_ptr<mtc::bus>>();
			
		bus->start();
		bus->publish(my_message()).get();
		bus->stop();
}
```

