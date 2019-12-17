#ifndef HUBRESULT_HPP
#define HUBRESULT_HPP

#include <QString>
#include <QObject>
#include <QJsonObject>

namespace signalr {
namespace hubs {

class HubResult : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE HubResult() = default;

    Q_PROPERTY(QString id MEMBER m_Id)
    Q_PROPERTY(QJsonObject result MEMBER m_IsHubException)
    Q_PROPERTY(bool hubException MEMBER m_IsHubException)
    Q_PROPERTY(std::shared_ptr<QObject> errorData MEMBER m_ErrorData)
    Q_PROPERTY(QHash<QString, QJsonObject> state MEMBER m_State)

private:
    QString m_Id;
    QJsonObject m_Result;
    bool m_IsHubException;
    QString m_Error;
    std::shared_ptr<QObject> m_ErrorData;
    QHash<QString, QJsonObject> m_State;
};

}
}

#endif //HUBRESULT_HPP
