#ifndef WEBSOCKETTRANSPORT_HPP
#define WEBSOCKETTRANSPORT_HPP

#include "iclienttransport.hpp"
#include <QtWebSockets/QWebSocket>

namespace signalr {
namespace transports {

class WebSocketTransport : public IClientTransport {
public:
    WebSocketTransport() = default;


    virtual ~WebSocketTransport() = default;
private:
    QWebSocket m_WebSocket;
};

}
}

#endif //WEBSOCKETTRANSPORT_HPP
