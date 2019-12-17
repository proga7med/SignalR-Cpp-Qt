#include "hubproxy.hpp"

namespace signalr {
namespace hubs {

HubProxy::HubProxy(std::shared_ptr<IHubConnection> pHubConnection,  QString hubName) {
  m_pHubConnection = std::move(pHubConnection);
  m_HubName = std::move(hubName);
}

void HubProxy::invokeEvent(const QString &methodname, const QList<QJsonValue>& args) {
    Subscription subscription;
    if(m_Subscriptions.contains(methodname)) {
        subscription = m_Subscriptions[methodname];
        subscription.onReceived(args);
    }
}

} //end hubs
} //end signalr
