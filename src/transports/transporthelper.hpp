#ifndef TRANSPORTHELPER_HPP
#define TRANSPORTHELPER_HPP

#include <QtPromise>

#include "signalr/iconnection.hpp"

namespace signalr::transports {

class TransportHelper {
public:
  QtPromise::QPromise<NegotiationResponse> getNegotiationResponse(std::shared_ptr<http::IHttpClient> pHttpClient, std::shared_ptr<IConnection> pIConnection, const QString& connectionData);
  static QtPromise::QPromise<QString> getStartResponse(std::shared_ptr<http::IHttpClient> pHttpClient, std::shared_ptr<IConnection> pIConnection, const QString& connectionData, const QString& transport);
  static bool verifyLastActive(std::shared_ptr<IConnection> pIConnection);
};

}



#endif //TRANSPORTHELPER_HPP
