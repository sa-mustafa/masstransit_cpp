cmake_minimum_required(VERSION 3.6)

project(masstransit)


# Versions

file(STRINGS include/global.hpp _VERSION_MAJOR REGEX "^#define MASSTRANSIT_VERSION_MAJOR [0-9]+$")
file(STRINGS include/global.hpp _VERSION_MINOR REGEX "^#define MASSTRANSIT_VERSION_MINOR [0-9]+$")
file(STRINGS include/global.hpp _VERSION_PATCH REGEX "^#define MASSTRANSIT_VERSION_PATCH [0-9]+$")

string(REGEX MATCH "[0-9]+" _VERSION_MAJOR ${_VERSION_MAJOR})
string(REGEX MATCH "[0-9]+" _VERSION_MINOR ${_VERSION_MINOR})
string(REGEX MATCH "[0-9]+" _VERSION_PATCH ${_VERSION_PATCH})

set(MTC_VERSION ${_VERSION_MAJOR}.${_VERSION_MINOR}.${_VERSION_PATCH})
set(MTC_SOVERSION ${_VERSION_MAJOR})


# Dependencies

find_package(Boost REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_NO_LIB -DBOOST_LOG_DYN_LINK")
#set(AdditionalLibraries ${AdditionalLibraries} ${Boost_LIBRARIES})

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SimpleAmqpClient REQUIRED)
find_package(Rabbitmqc REQUIRED)
include_directories(SYSTEM ${SimpleAmqpClient_INCLUDE_DIRS})
set(AdditionalLibraries ${AdditionalLibraries} ${SimpleAmqpClient_LIBRARIES} ${Rabbitmqc_LIBRARIES})


# Sources

set(SOURCE_FILES 
	include/bus.hpp
	include/bus_control.hpp
	include/bus_factory.hpp
	include/consume_context.hpp
	include/consume_context_info.hpp
	#include/datetime.hpp
	include/global.hpp
	include/host_info.hpp
	include/i_message_consumer.hpp
	include/i_publish_endpoint.hpp
	include/json.hpp
	include/json_adapters.hpp
	include/message_consumer.hpp
	include/message_handler.hpp
	include/i_receive_endpoint.hpp
	include/i_receive_endpoint_configurator.hpp
	include/uuid.hpp

	include/rabbit_mq/exchange_manager.hpp
	include/rabbit_mq/message_publisher.hpp
	include/rabbit_mq/rabbit_mq_bus.hpp
	include/rabbit_mq/rabbit_mq_configurator.hpp
	include/rabbit_mq/receive_endpoint.hpp
	include/rabbit_mq/receive_endpoint_configurator.hpp
	include/rabbit_mq/amqp_host.hpp
	include/rabbit_mq/amqp_host_configurator.hpp
	include/in_memory/in_memory_bus.hpp
	include/in_memory/in_memory_configurator.hpp
	include/in_memory/receive_endpoint.hpp
	include/in_memory/receive_endpoint_configurator.hpp
	include/threads/tasks_queue.hpp
	include/threads/thread_pool.hpp
	include/threads/worker.hpp
	include/threads/worker_thread.hpp
	include/threads/task_repeat.hpp
	
	src/bus_factory.cpp
	src/consume_context_info.cpp
	src/host_info.cpp
	src/json_adapters.cpp
	src/i_receive_endpoint.cpp
	src/i_receive_endpoint_configurator.cpp

	src/in_memory/receive_endpoint.cpp
	src/in_memory/receive_endpoint_configurator.cpp
	src/in_memory/in_memory_bus.cpp
	src/in_memory/in_memory_configurator.cpp
	src/rabbit_mq/amqp_host.cpp
	src/rabbit_mq/amqp_host_configurator.cpp
	src/rabbit_mq/exchange_manager.cpp
	src/rabbit_mq/message_publisher.cpp
	src/rabbit_mq/rabbit_mq_bus.cpp
	src/rabbit_mq/rabbit_mq_configurator.cpp
	src/rabbit_mq/receive_endpoint.cpp
	src/rabbit_mq/receive_endpoint_configurator.cpp
	src/threads/thread_pool.cpp
	src/threads/worker.cpp
	src/threads/worker_thread.cpp
	)

if(${CMAKE_VERSION} GREATER_EQUAL 3.8.2)
	source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})
endif()

# Build

option(BUILD_SHARED_LIBS "Build MassTransit as a shared library" OFF)
		
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++1z")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMODULE_SETUP_DLL_EXPORTS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMASSTRANSIT_DLL_EXPORTS")
if(BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCONTROLS_DLL -DMASSTRANSIT_DLL")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")
add_library(masstransit ${SOURCE_FILES})
target_link_libraries(masstransit ${AdditionalLibraries})

if (WIN32)
  set_target_properties(masstransit PROPERTIES VERSION ${MTC_VERSION} OUTPUT_NAME masstransit.${MTC_SOVERSION})
else ()
  set_target_properties(masstransit PROPERTIES VERSION ${MTC_VERSION} SOVERSION ${MTC_SOVERSION})
endif ()

set_property(GLOBAL PROPERTY USE_FOLDERS 1)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

# Tests

option(BUILD_TESTS "Enable tests" OFF)

if (BUILD_TESTS)
    enable_testing()
    set(BUILD_SHARED_LIBS OFF)
    add_subdirectory(tests)
    add_test(tests tests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif (BUILD_TESTS)


# Install

install(TARGETS masstransit
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
	
install(FILES
	include/bus.hpp
	include/bus_control.hpp
	include/bus_factory.hpp
	include/consume_context.hpp
	include/global.hpp
	include/host_info.hpp
	include/i_message_consumer.hpp
	include/i_publish_endpoint.hpp
	include/json.hpp
	include/json_adapters.hpp
	include/message_consumer.hpp
	include/message_handler.hpp
	include/i_receive_endpoint.hpp
	include/i_receive_endpoint_configurator.hpp
	include/uuid.hpp

	include/rabbit_mq/exchange_manager.hpp
	include/rabbit_mq/rabbit_mq_bus.hpp
	include/rabbit_mq/rabbit_mq_configurator.hpp
	include/rabbit_mq/receive_endpoint.hpp
	include/rabbit_mq/receive_endpoint_configurator.hpp
	include/rabbit_mq/amqp_host.hpp
	include/rabbit_mq/amqp_host_configurator.hpp
	include/in_memory/in_memory_bus.hpp
	include/in_memory/in_memory_configurator.hpp
	include/in_memory/receive_endpoint.hpp
	include/in_memory/receive_endpoint_configurator.hpp
	include/threads/tasks_queue.hpp
	include/threads/thread_pool.hpp
	include/threads/worker.hpp
	include/threads/worker_thread.hpp
	include/threads/task_repeat.hpp
    
	DESTINATION include/masstransit
)
