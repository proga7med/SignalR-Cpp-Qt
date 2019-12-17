
find_package(Qt5 COMPONENTS Core Network Concurrent Websockets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SIGNALR_SRC connection.cpp 
                negotiationresponse.cpp
                cancellationtoken.cpp
				http/httprequestmessagewrapper.cpp
                                http/httpresponsemessagewrapper.cpp
				http/defaulthttpclient.cpp
				hubs/hubproxy.cpp 
				hubs/hubconnection.cpp 
				hubs/Subscription.cpp
				transports/websockettransport.cpp 
				transports/transporthelper.cpp
				transports/clienttransportbase)

add_library(SignalRClient SHARED ${SIGNALR_SRC})
target_include_directories(SignalRClient PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(SignalRClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(SignalRClient PRIVATE cxx_std_17)
target_link_libraries(SignalRClient PUBLIC Qt5::Core Qt5::Network Qt5::Concurrent)
