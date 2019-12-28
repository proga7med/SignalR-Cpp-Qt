#include "defaulthttpclient.hpp"

#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QCoreApplication>

#include "httphelper.hpp"
#include "defaulthttphandler.hpp"
#include "httprequestmessagewrapper.hpp"
#include "httpresponsemessagewrapper.hpp"

namespace signalr {
namespace http {

void DefaultHttpClient::initialize(std::shared_ptr<IConnection> pConnection) {
  m_pConnection = std::move(pConnection);
  m_pLongRunningClient = std::make_shared<QNetworkAccessManager>();
  m_pShortRunningClient = std::make_shared<QNetworkAccessManager>();
}

QtPromise::QPromise<std::shared_ptr<IResponse>> DefaultHttpClient::get(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, bool isLongRunning) {
  if (prepareRequest == nullptr) throw QException();

  QNetworkRequest requestMessage = QNetworkRequest(QUrl(url));
  std::shared_ptr<IRequest> request = std::make_shared<HttpRequestMessageWrapper>(requestMessage);
  prepareRequest(request);

  return QtPromise::QPromise<QNetworkReply*>([&](const QtPromise::QPromiseResolve<QNetworkReply*>& resolver){
    auto httpClient = getHttpClient(isLongRunning);
    DefaultHttpHandler handler = DefaultHttpHandler(m_pConnection, requestMessage, httpClient);
    auto reply = httpClient->get(requestMessage);
    QObject::connect(reply, &QNetworkReply::finished, [=](){
        resolver(reply);
    });
  })
  .then([&](QNetworkReply* reply){
      auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
      if(statusCode != 200) throw QException();

      return std::dynamic_pointer_cast<IResponse>(std::make_shared<HttpResponseMessageWrapper>(reply));
  }).fail([&](const QException&){
    return QtPromise::QPromise<std::shared_ptr<IResponse>>::reject(QString("the status code isn't 200"));
  });
}

QtPromise::QPromise<std::shared_ptr<IResponse>> DefaultHttpClient::post(const QString& url, std::function<void(std::shared_ptr<IRequest>)> prepareRequest, const QHash<QString, QString>& postData, bool isLongRunning) {
  if (prepareRequest == nullptr) throw QException();

  QNetworkRequest requestMessage = QNetworkRequest(QUrl(url));
  std::shared_ptr<IRequest> request = std::make_shared<HttpRequestMessageWrapper>(requestMessage);
  prepareRequest(request);

  return QtPromise::QPromise<QNetworkReply*>([&](const QtPromise::QPromiseResolve<QNetworkReply*> resolver){
     auto httpClient = getHttpClient(isLongRunning);
     DefaultHttpHandler handler = DefaultHttpHandler(m_pConnection, requestMessage, httpClient);
     auto reply = httpClient->post(requestMessage,  postData.isEmpty() ? QByteArray() : HttpHelper::processPostData(postData));
     QObject::connect(reply, &QNetworkReply::finished, [=](){
         qDebug() << reply->errorString();
         resolver(reply);
     });
  })
  .then([&](QNetworkReply* reply){
      auto statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
      if(statusCode != 200) throw QException();

      return std::dynamic_pointer_cast<IResponse>(std::make_shared<HttpResponseMessageWrapper>(reply));
  });
}

std::shared_ptr<QNetworkAccessManager> DefaultHttpClient::getHttpClient(bool isLongRunning) const {
  return isLongRunning ? m_pLongRunningClient : m_pShortRunningClient;
}

}
}
