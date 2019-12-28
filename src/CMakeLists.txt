
find_package(Qt5 COMPONENTS Core Network Concurrent WebSockets JsonSerializer)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SIGNALR_SRC connection.cpp 
                negotiationresponse.cpp
                heartbeatmonitor.cpp
                keepalivedata.cpp
                http/httprequestmessagewrapper.cpp
                http/httpresponsemessagewrapper.cpp
                http/httpwebrequestwrapper.cpp
                http/httpwebresponsewrapper.cpp
                http/defaulthttpclient.cpp
                http/httphelper.cpp
                hubs/hubproxy.cpp
                hubs/hubconnection.cpp
                hubs/subscription.cpp
                hubs/hubresult.cpp
                hubs/hubinvocation.cpp
                hubs/hubprogressupdate.cpp
                hubs/hubregistrationdata.cpp
                transports/websockettransport.cpp
                transports/transporthelper.cpp
                transports/clienttransportbase
                transports/websockets/websocketwrapperrequest.cpp
                transports/websockets/clientwebsockethandler.cpp
                infra/urlbuilder.cpp
                http/defaulthttphandler.cpp
                infra/urlencoder.cpp
                ${PROJECT_SOURCE_DIR}/submodules/time_delta/time_delta.cpp)

set(SIGNALR_HEADERS ${PROJECT_SOURCE_DIR}/include/signalr/hubs/hubconnection.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/hubs/ihubconnection.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/iconnection.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/hubs/subscription.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/hubs/hubinvocation.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/hubs/hubprogressupdate.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/hubs/hubresult.hpp
    ${PROJECT_SOURCE_DIR}/include/signalr/negotiationresponse.hpp)

add_library(SignalRClient STATIC ${SIGNALR_SRC} ${SIGNALR_HEADERS})

target_include_directories(SignalRClient PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(SignalRClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SignalRClient PUBLIC ${PROJECT_SOURCE_DIR}/submodules/time_delta)
target_include_directories(SignalRClient PUBLIC ${PROJECT_SOURCE_DIR}/submodules/qtpromise/include)

target_compile_features(SignalRClient PRIVATE cxx_std_17)
target_link_libraries(SignalRClient PUBLIC Qt5::Core Qt5::Network Qt5::Concurrent Qt5::WebSockets Qt5::JsonSerializer)
