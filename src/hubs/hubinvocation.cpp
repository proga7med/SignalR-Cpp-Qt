#include "signalr/hubs/hubinvocation.hpp"

namespace signalr {
namespace hubs {

QString HubInvocation::getCallbackId() const {
  return m_CallbackId;
}

void HubInvocation::setCallbackId(QString callbackId) {
  m_CallbackId = std::move(callbackId);
}

QString HubInvocation::getHub() const {
  return m_Hub;
}

void HubInvocation::setHub(QString hub) {
  m_Hub = std::move(hub);
}

QString HubInvocation::getMethod() const {
  return m_Method;
}

void HubInvocation::setMethod(QString method) {
  m_Method = std::move(method);
}

QJsonArray HubInvocation::getArgs() const {
  return m_Args;
}
void HubInvocation::setArgs(QJsonArray args) {
  m_Args = std::move(args);
}

QMap<QString, QJsonDocument> HubInvocation::getState() const {
  return m_State;
}

void HubInvocation::setState(QMap<QString, QJsonDocument> state) {
  m_State = std::move(state);
}

}
}
