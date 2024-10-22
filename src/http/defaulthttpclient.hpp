#ifndef DEFAULTHTTPCLIENT_HPP
#define DEFAULTHTTPCLIENT_HPP

#include <memory>
#include <QNetworkAccessManager>

#include "signalr/iconnection.hpp"
#include "signalr/http/ihttpclient.hpp"

namespace signalr {
namespace http {

class DefaultHttpClient : public IHttpClient {
public:
    DefaultHttpClient() = default;
    virtual void initialize(std::shared_ptr<IConnection> pConnection) override;
    virtual QtPromise::QPromise<std::shared_ptr<IResponse>> get(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, bool isLongRunning) override;
    virtual QtPromise::QPromise<std::shared_ptr<IResponse>> post(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, const QHash<QString, QString>& postData, bool isLongRunning) override;

protected:
    virtual std::shared_ptr<QNetworkAccessManager> getHttpClient(bool isLongRunning) const;

private:
    std::shared_ptr<QNetworkAccessManager> m_pLongRunningClient;
    std::shared_ptr<QNetworkAccessManager> m_pShortRunningClient;
    std::shared_ptr<IConnection> m_pConnection;
};

}
}

#endif //DEFAULTHTTPCLIENT_HPP
