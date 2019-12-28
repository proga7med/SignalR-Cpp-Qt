#ifndef HUBPROXY_HPP
#define HUBPROXY_HPP

#include <QVector>
#include <QtPromise>
#include <QJsonDocument>
#include <QJsonSerializer>

#include "signalr/hubs/hubinvocation.hpp"
#include "signalr/hubs/ihubproxy.hpp"
#include "signalr/hubs/subscription.hpp"
#include "signalr/hubs/ihubconnection.hpp"


namespace signalr {
namespace hubs {

class HubProxy : public IHubProxy {
public:
  HubProxy(std::shared_ptr<IHubConnection> pHubConnection,  QString hubName);

  QJsonDocument& operator[] (const QString& name);
  const QJsonDocument& operator[] (const QString& name) const;

  std::shared_ptr<QJsonSerializer> getJsonSerializer() const override;
  virtual std::shared_ptr<Subscription> subscribe(const QString& eventName) override;

  template<typename R, typename P>
  QtPromise::QPromise<R> invoke(const QString& methodName, std::function<void(P)> onProgress, const QVariantList args) {
    if(methodName.isNull()) throw QException();
     QJsonArray tokenifiedArguments;
     for(int i = 0; i < args.count(); ++i) {
         auto d = getJsonSerializer()->serialize(args[i]);
         tokenifiedArguments.append((!args[i].isNull())
               ? this->getJsonSerializer()->serialize(args[i])
               : QJsonValue::Null);
     }

     using QtPromise::QPromise;
     using QtPromise::QPromiseResolve;
     using QtPromise::QPromiseReject;

     //auto task = QPromise<R>([&](const QPromiseResolve<R>& resolve, const QPromiseReject<R>& reject){});
     auto callbackId = m_pHubConnection->registerCallback([&](HubResult result) {
         if(!result.getError().isNull()) {
             if(result.isHubException()) {
//                task.fail([&](){
//                    //HubException.const QString& error, const QVariant& errorData
//                });
              }
          }
          else {
              try {
                 if(!result.getState().isEmpty()) {
                     QMap<QString, QJsonDocument>::iterator it;
                     for(it = result.getState().begin(); it != result.getState().end(); ++it) {
                         this->operator[](it.key()) = it.value();
                     }
                 }
                 if(!result.getHubProgressUpdate().getId().isNull()) {
                    onProgress(getJsonSerializer()->deserialize<P>(result.getHubProgressUpdate().getData().object()));
                 }
                 else if(!result.getResult().isNull()) {
//                   task.then([&](const QString& str) {
//                       return getJsonSerializer()->deserialize<R>(result->getResult().object());
//                   });
                 }
                 else {
                   //
                 }
              } catch (const QException& ex) {
//                 task.fail([&](const QException& ex){
//                     qDebug() << ex.what();
//                 });
              }
          }
     });

     auto hubData = HubInvocation();
     hubData.setHub(m_HubName);
     hubData.setMethod(methodName);
     hubData.setArgs(tokenifiedArguments);
     hubData.setCallbackId(callbackId);

     QString value;
     try {
            {
              std::lock_guard<std::mutex> lock(m_StateMutex);
              if(m_State.count() != 0) {
                 hubData.setState(m_State);
                 value = QJsonDocument(m_pHubConnection->getJsonSerializer()->serialize<HubInvocation>(hubData)).toJson();
              }
            }

        value = value.isNull() ? QJsonDocument(m_pHubConnection->getJsonSerializer()->serialize<HubInvocation>(hubData)).toJson() : QString();

     } catch (const QJsonSerializerException& ex) {
        qDebug() << ex.what();
     }

    return m_pHubConnection->send(value);
  }

  void invokeEvent(const QString& methodname, const QJsonArray &args);
  virtual ~HubProxy() = default;

public:
  static void on(std::shared_ptr<IHubProxy> proxy, const QString eventName, std::function<void()> onData);

  template<typename T1>
  static void on(std::shared_ptr<IHubProxy> proxy, const QString eventName, std::function<void(T1)> onData){
    if(proxy == nullptr) throw QException();
    if(eventName.isNull() || eventName.isEmpty()) throw QException();
    if(onData == nullptr) throw QException();

    std::shared_ptr<Subscription> subscription = proxy->subscribe(eventName);

    std::function<void(QJsonArray)> handler = [=](QJsonArray args) {
        executeCallback(eventName, args.count(), 1,  [=]() {
            onData(convert<T1>(args[0].toObject(), proxy->getJsonSerializer()));
        });
    };

    QObject::connect(subscription.get(), &Subscription::received, [=](const QJsonArray &data){ handler(data); });
  }

  template<typename T1, typename T2>
  static void on(std::shared_ptr<IHubProxy> proxy, const QString eventName, std::function<void(T1, T2)> onData){
      if(proxy == nullptr) throw QException();
      if(eventName.isNull() || eventName.isEmpty()) throw QException();
      if(onData == nullptr) throw QException();

      std::shared_ptr<Subscription> subscription = proxy->subscribe(eventName);

      std::function<void(QJsonArray)> handler = [=](QJsonArray args) {
          executeCallback(eventName, args.count(), 2,  [=]() {
              onData(convert<T1>(args[0], proxy->getJsonSerializer()),
                      convert<T2>(args[1], proxy->getJsonSerializer()));
          });
      };

      QObject::connect(subscription.get(), &Subscription::received, [=](const QJsonArray &data){ handler(data); });
  }

  template<typename T1, typename T2, typename T3>
  static void on(std::shared_ptr<IHubProxy> proxy, const QString eventName, std::function<void(T1, T2, T3)> onData){
      if(proxy == nullptr) throw QException();
      if(eventName.isNull() || eventName.isEmpty()) throw QException();
      if(onData == nullptr) throw QException();

      std::shared_ptr<Subscription> subscription = proxy->subscribe(eventName);

      std::function<void(QJsonArray)> handler = [=](QJsonArray args) {
          executeCallback(eventName, args.count(), 2,  [=]() {
              onData(convert<T1>(args[0].toObject(), proxy->getJsonSerializer()),
                      convert<T2>(args[1].toObject(), proxy->getJsonSerializer()),
                      convert<T3>(args[2].toObject(), proxy->getJsonSerializer()));
          });
      };

      QObject::connect(subscription.get(), &Subscription::received, [=](const QJsonArray &data){ handler(data); });
  }

private:
  static void executeCallback(const QString& eventName, int actualArgs, int expectedArgs, std::function<void()> action);

  template<class T>
  static T convert(const QJsonValue& obj, std::shared_ptr<QJsonSerializer> serializer) {
    if(obj.isNull()) return T();
    return serializer->deserialize<T>(obj);
  }

private:
  QString m_HubName;
  std::shared_ptr<IHubConnection> m_pHubConnection;
  QMap<QString, QJsonDocument> m_State;
  QHash<QString, std::shared_ptr<Subscription>> m_Subscriptions;
  mutable std::mutex m_StateMutex;
};


}
}


#endif //HUBPROXY_HPP
