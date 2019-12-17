#include "defaulthttpclient.hpp"

#include <QNetworkReply>
#include <QCoreApplication>

#include "httprequestmessagewrapper.hpp"
#include "httpresponsemessagewrapper.hpp"

namespace signalr {
namespace http {

void DefaultHttpClient::initialize(std::shared_ptr<IConnection> pConnection) {
  m_pConnection = std::move(pConnection);
  m_pLongRunningClient = std::make_shared<QNetworkAccessManager>();
  m_pShortRunningClient = std::make_shared<QNetworkAccessManager>();
}

std::promise<std::shared_ptr<IResponse>> DefaultHttpClient::get(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, bool isLongRunning) {
	if (prepareRequest == nullptr) throw QException();

    std::shared_ptr<QNetworkRequest> requestMessage = std::make_shared<QNetworkRequest>(QUrl(url));
	std::shared_ptr<IRequest> request = std::make_shared<HttpRequestMessageWrapper>(requestMessage);
	prepareRequest(request);

	auto httpClient = getHttpClient(isLongRunning);
    auto reply = httpClient->get(*requestMessage.get());
    while(!reply->isFinished()) qApp->processEvents();

    auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != 200) throw QException();

    reply->deleteLater();
    std::promise<std::shared_ptr<IResponse>> task =
            std::promise<std::shared_ptr<IResponse>>();

    task.set_value(std::dynamic_pointer_cast<IResponse>(std::make_shared<HttpResponseMessageWrapper>(reply)));
    return task;
}

std::promise<std::shared_ptr<IResponse>> DefaultHttpClient::post(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, QHash<QString, QString> postData, bool isLongRunning) {
  if (prepareRequest == nullptr) throw QException();

  std::shared_ptr<QNetworkRequest> requestMessage = std::make_shared<QNetworkRequest>(QUrl(url));
  std::shared_ptr<IRequest> request = std::make_shared<HttpRequestMessageWrapper>(requestMessage);

  prepareRequest(request);
  auto httpClient = getHttpClient(isLongRunning);
  auto reply = httpClient->post(*requestMessage.get(), QByteArray()/*TODO: postData.isEmpty() ? QString() : */);
  while(!reply->isFinished()) qApp->processEvents();

  reply->deleteLater();
  auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  if(statusCode != 200) throw QException();

  std::promise<std::shared_ptr<IResponse>> task =
          std::promise<std::shared_ptr<IResponse>>();

  task.set_value(std::dynamic_pointer_cast<IResponse>(std::make_shared<HttpResponseMessageWrapper>(reply)));
  return task;
}


}
}
