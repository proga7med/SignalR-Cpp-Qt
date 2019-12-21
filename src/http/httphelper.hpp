#ifndef HTTPHELPER_HPP
#define HTTPHELPER_HPP

#include <future>
#include <memory>

#include <QDataStream>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QtPromise>

namespace signalr {
namespace http {

class HttpHelper {
public:
  static QtPromise::QPromise<QNetworkReply*> getHttpResponseAsync(const std::shared_ptr<QNetworkRequest>& request);
  static std::promise<QDataStream> getHttpRequestStreamAsync(const std::shared_ptr<QNetworkRequest>& request);
  static std::promise<QNetworkReply*> getAsync(const QString& url, std::function<void(std::shared_ptr<QNetworkRequest>)> requestPreparer);
  static std::promise<QNetworkReply*> postAsync(const QString& url, std::function<void(std::shared_ptr<QNetworkRequest>)> requestPreparer, const QHash<QString, QString>& postData);
  static QByteArray processPostData(const QHash<QString, QString>& postData);
};

}
}

#endif //HTTPHELPER_HPP
