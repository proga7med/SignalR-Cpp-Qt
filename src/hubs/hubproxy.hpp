#ifndef HUBPROXY_HPP
#define HUBPROXY_HPP

#include <tuple>
#include <QVector>
#include "infra/Action.hpp"
#include "signalr/hubs/ihubproxy.hpp"
#include "signalr/hubs/ihubconnection.hpp"
#include "signalr/hubs/subscription.hpp"

namespace signalr {
namespace hubs {

class HubProxy : public IHubProxy {
public:
    HubProxy(std::shared_ptr<IHubConnection> pHubConnection,  QString hubName);

    void invokeEvent(const QString& methodname, const QList<QJsonValue> &args);

//    template<typename Result, typename Progress, typename ...Args>
//    std::promise<Result> invoke(const QString& method, std::function<Progress()> onProgress, Args...) {
//        if(method.isNull()) throw QException();
//        if(sizeof...(Args) < 0) throw QException();
//        QVector<QJsonValue> foundTokenArgs = QVector<QJsonValue>(sizeof...(Args));

//        template<int N, typename... Ts> using NthTypeOf =
//                typename std::t<N, std::tuple<Ts...>>::type;

//        for (int i = 0; i < foundTokenArgs.length(); ++i) {
//            foundTokenArgs[i] = Args[i] == nullptr
//                    ? QJsonValue::fromVariant(Args[i])
//                    : QJsonValue::Null;
//        }

//        auto callbackId = m_pHubConnection->
//    }

    virtual ~HubProxy() = default;

private:
  QString m_HubName;
  std::shared_ptr<IHubConnection> m_pHubConnection;
  QHash<QString, QJsonValue> m_State;
  QHash<QString, Subscription> m_Subscriptions;
};


}
}


#endif //HUBPROXY_HPP
