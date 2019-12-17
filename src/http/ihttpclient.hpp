#ifndef IHTTPCLIENT_HPP
#define IHTTPCLIENT_HPP

#include <memory>
#include <future>

#include <QHash>
#include <QString>

#include "irequest.hpp"
#include "iresponse.hpp"
#include "signalr/iconnection.hpp"

namespace signalr {
namespace http {

class IHttpClient {
public:
   virtual void initialize(std::shared_ptr<IConnection> pConnection) = 0;
   virtual std::promise<std::shared_ptr<IResponse>> get(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, bool isLongRunning) = 0;
   virtual std::promise<std::shared_ptr<IResponse>> post(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, QHash<QString, QString> postData, bool isLongRunning) = 0;

   virtual std::promise<std::shared_ptr<IResponse>> post(std::shared_ptr<IHttpClient> pHttpClient, QString url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, bool isLongRunning) {
     if(pHttpClient == nullptr) throw QException();
     if(url.isNull()) throw QException();
     if(prepareRequest == nullptr) throw QException();
     return post(url, prepareRequest, QHash<QString, QString>(), isLongRunning);
   }

   virtual ~IHttpClient() = default;
};

}
}

#endif //IHTTPCLIENT_HPP
