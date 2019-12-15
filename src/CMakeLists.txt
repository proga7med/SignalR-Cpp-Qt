
find_package(Qt5 COMPONENTS Core Network Concurrent Websockets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SIGNALR_SRC connection.cpp 
				http/defaulthttpclient.cpp
				hubs/hubproxy.cpp 
				hubs/hubconnection.cpp 
				hubs/Subscription.cpp
				transports/websockettransport.cpp 
				transports/clienttransportbase)

add_library(SignalRClient SHARED ${SIGNALR_SRC})
target_include_directories(SignalRClient PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(SignalRClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(SignalRClient PUBLIC Qt5::Core Qt5::Network Qt5::Concurrent)