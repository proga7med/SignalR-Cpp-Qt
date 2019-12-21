#include "httphelper.hpp"

#include <QEventLoop>
#include <QException>

namespace signalr {
namespace http {

QtPromise::QPromise<QNetworkReply*> HttpHelper::getHttpResponseAsync(const std::shared_ptr<QNetworkRequest>& request) {
  try {
     return QtPromise::QPromise<QNetworkReply*>([&](const QtPromise::QPromiseResolve<QNetworkReply*>& resolve, const QtPromise::QPromiseReject<QNetworkReply*>& reject) {
         QNetworkAccessManager manager;
         QNetworkReply* reply = manager.get(*request);
         resolve(reply);
     });
  } catch (...) {
      //Log here
      throw;
  }
}

std::promise<QDataStream> HttpHelper::getHttpRequestStreamAsync(const std::shared_ptr<QNetworkRequest>& request) {
  throw QException();
}

std::promise<QNetworkReply*> HttpHelper::getAsync(const QString& url, std::function<void(std::shared_ptr<QNetworkRequest>)> requestPreparer) {
  throw QException();
}

std::promise<QNetworkReply*> HttpHelper::postAsync(const QString& url, std::function<void(std::shared_ptr<QNetworkRequest>)> requestPreparer, const QHash<QString, QString>& postData) {
  throw QException();
}

QByteArray HttpHelper::processPostData(const QHash<QString, QString>& postData) {
  throw QException();
}

}
}