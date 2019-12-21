#ifndef HTTPREQUESTMESSAGEWRAPPER_HPP
#define HTTPREQUESTMESSAGEWRAPPER_HPP

#include <QObject>
#include <QNetworkRequest>

#include "signalr/http/irequest.hpp"

namespace signalr {
namespace http {

class HttpRequestMessageWrapper : public QObject, public IRequest {
	Q_OBJECT
public:
    HttpRequestMessageWrapper(std::shared_ptr<QNetworkRequest> networkRequest);

	virtual QString getUserAgent() const override;
	virtual void setUserAgent(QString userAgent) override;

	virtual QString getAccept() const override;
	virtual void setAccept(QString accept) override;

	virtual void abort() override;
    virtual void setRequestHeaders(QList<QPair<QString, QString>> &headers) override;

	virtual ~HttpRequestMessageWrapper() = default;

signals:
	void cancel();

private:
    std::shared_ptr<QNetworkRequest> m_pNetworkRequest;
};

}
}

#endif // ! HTTPREQUESTMESSAGEWRAPPER_HPP
