#include "hubproxy.hpp"

namespace signalr {
namespace hubs {

HubProxy::HubProxy(std::shared_ptr<IHubConnection> pHubConnection,  QString hubName) {
  m_pHubConnection = std::move(pHubConnection);
  m_HubName = std::move(hubName);
}

QJsonDocument& HubProxy::operator[] (const QString& name) {
  std::lock_guard<std::mutex> lock(m_StateMutex);
  return m_State[name];
}

const QJsonDocument& HubProxy::operator[] (const QString& name) const {
  std::lock_guard<std::mutex> lock(m_StateMutex);
  QJsonDocument value;
  if(m_State.contains(name)) value = m_State[name];
  return value;
}

std::shared_ptr<QJsonSerializer> HubProxy::getJsonSerializer() const {
  return m_pHubConnection->getJsonSerializer();
}

std::shared_ptr<Subscription> HubProxy::subscribe(const QString& eventName) {
  if(eventName.isNull()) throw QException();

  std::shared_ptr<Subscription> subscription;
  if(!m_Subscriptions.contains(eventName)) {
    subscription = std::make_shared<Subscription>();
    m_Subscriptions.insert(eventName, subscription);
  }
  return subscription;
}

void HubProxy::invokeEvent(const QString &methodname, const QJsonArray &args) {
  std::shared_ptr<Subscription> subscription;
  if(m_Subscriptions.contains(methodname)) {
    subscription = m_Subscriptions[methodname];
    subscription->onReceived(args);
  }
}

void HubProxy::on(std::shared_ptr<IHubProxy> proxy, const QString eventName, std::function<void()> onData) {
  if(proxy == nullptr) throw QException();
  if(eventName.isNull() || eventName.isEmpty()) throw QException();
  if(onData == nullptr) throw QException();

  std::shared_ptr<Subscription> subscription = proxy->subscribe(eventName);

  std::function<void(QJsonArray)> handler = [&](QJsonArray args) {
    executeCallback(eventName, args.count(), 0, onData);
  };

  QObject::connect(subscription.get(), &Subscription::received, [=](const QJsonArray &data){ handler(data); });
}

void HubProxy::executeCallback(const QString& eventName, int actualArgs, int expectedArgs, std::function<void()> action) {
  if(expectedArgs > actualArgs) throw QException();

  try {
    action();
  } catch (const QException& ex) {
    qDebug() << ex.what();
  }
}

} //end hubs
} //end signalr
