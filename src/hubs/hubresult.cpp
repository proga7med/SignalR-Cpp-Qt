#include "signalr/hubs/hubresult.hpp"

namespace signalr {
namespace hubs {

QString HubResult::getId() const {
  return m_Id;
}

void HubResult::setId(QString id) {
  m_Id = std::move(id);
}

HubProgressUpdate HubResult::getHubProgressUpdate() const {
  return m_HubProgressUpdate;
}

void HubResult::setHubProgressUpdate(HubProgressUpdate hubProgressUpdate) {
  m_HubProgressUpdate = std::move(hubProgressUpdate);
}

QJsonDocument HubResult::getResult() const {
  return m_Result;
}

void HubResult::setResult(QJsonDocument result) {
  m_Result = std::move(result);
}

bool HubResult::isHubException() const {
  return m_IsHubException;
}

void HubResult::setHubException(bool hubException) {
  m_IsHubException = hubException;
}

QString HubResult::getError() const {
  return m_Error;
}

void HubResult::setError(QString error) {
  m_Error = std::move(error);
}

QVariant HubResult::getErrorData() const {
  return m_ErrorData;
}

void HubResult::setErrorData(QVariant errorData) {
  m_ErrorData = std::move(errorData);
}

QMap<QString, QJsonDocument> HubResult::getState() const {
  return m_State;
}

void HubResult::setState(QMap<QString, QJsonDocument> state) {
  m_State = std::move(state);
}


}
}
