#ifndef HTTPRESPONSEMESSAGEWRAPPER_HPP
#define HTTPRESPONSEMESSAGEWRAPPER_HPP

#include <QObject>
#include <QNetworkReply>

#include "iresponse.hpp"

namespace signalr {
namespace http {

class HttpResponseMessageWrapper : public QObject, public IResponse {
    Q_OBJECT
public:
    HttpResponseMessageWrapper(QNetworkReply* pNetworkReply);
    QString readAsString();
    QDataStream getStream() const override;
    ~HttpResponseMessageWrapper() = default;

private:
    QNetworkReply* m_pNetworkReply;
};

}
}

#endif //HTTPRESPONSEMESSAGEWRAPPER_HPP