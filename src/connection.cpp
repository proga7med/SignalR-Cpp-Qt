#include "connection.hpp"

#include <utility>

namespace signalr {

Connection::Connection(QString url) {
    m_Url = std::move(url);
}

std::promise<void> Connection::start() {
    throw QException();
}

std::promise<void> Connection::start(std::shared_ptr<transports::IClientTransport> pClientTransport) {
   throw QException();
}

void Connection::stop() {

}

void Connection::stop(int timeout) {

}

void Connection::setClientCertificate(QSslConfiguration configuration) {
    m_Configuration = std::move(configuration);
}

}
