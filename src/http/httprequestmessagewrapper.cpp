#include "httprequestmessagewrapper.hpp"

#include <QHash>
#include <QException>

namespace signalr {
namespace http {

HttpRequestMessageWrapper::HttpRequestMessageWrapper(std::shared_ptr<QNetworkRequest> networkRequest) {
   m_pNetworkRequest = std::move(networkRequest);
}

QString HttpRequestMessageWrapper::getUserAgent() const {
	return m_UserAgent;
 }

void HttpRequestMessageWrapper::setUserAgent(QString userAgent) {
	m_UserAgent = std::move(userAgent);
 }

QString HttpRequestMessageWrapper::getAccept() const {
	return m_Accept;
}
	
void HttpRequestMessageWrapper::setAccept(QString accept) {
	m_Accept = std::move(accept);
}

void HttpRequestMessageWrapper::abort() {
   emit cancel();
}

void HttpRequestMessageWrapper::setRequestHeaders(QHash<QString, QString>& headers) {
	if (m_UserAgent.isNull()) throw QException();
	if (m_Accept.isNull()) throw QException();

	QHash<QString, QString>::iterator it;
	for (it = headers.begin(); it != headers.end(); ++it) {
        m_pNetworkRequest->setRawHeader(it.key().toLatin1(), it.value().toLatin1());
	}
}

}
}
