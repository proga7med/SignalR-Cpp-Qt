#include "signalr/heartbeatmonitor.hpp"

namespace signalr {

HeartBeatMonitor::HeartBeatMonitor(std::shared_ptr<IConnection> pIConnection, std::shared_ptr<std::mutex> connectionStateLock, TimeDelta beatInterval) {
  m_pConnection = std::move(pIConnection);
  m_pConnectionStateLock = std::move(connectionStateLock);
  m_BeatInterval = std::move(beatInterval);
}

void HeartBeatMonitor::start() {
  m_MonitorKeepAlive = m_pConnection->getClientTransport()->isKeepAliveSupported();
  clearFlags();
  m_Timer.setInterval(m_BeatInterval.InMilliseconds());
  QObject::connect(&m_Timer, &QTimer::timeout, [&](){ beat(); });
  m_Timer.start();
}

void HeartBeatMonitor::clearFlags() {
  m_HasBeenWarned = false;
  m_TimedOut = false;
}

void HeartBeatMonitor::beat() {
  auto timeElapsed = QDateTime::currentDateTimeUtc() - m_pConnection->getLastMessageAt();
  beat(timeElapsed);
}

void HeartBeatMonitor::beat(const TimeDelta& timeElasped) {
  if(m_MonitorKeepAlive) checkKeepAlive(timeElasped);
  m_pConnection->markArchive();
}

void HeartBeatMonitor::checkKeepAlive(const TimeDelta& timeElasped) {
  auto connectionSlow = false;
  std::shared_ptr<transports::IClientTransport> pClientTransport = nullptr;
  std::lock_guard<std::mutex> lock (*m_pConnectionStateLock);
  if((m_pConnection->getConnectionState() == ConnectionState::Connected)) {
    if(timeElasped > m_pConnection->getKeepAliveData().getTimeout()) {
      if(!m_TimedOut) {
        //trace.
        m_TimedOut = true;
        pClientTransport = m_pConnection->getClientTransport();
      }
    }
    else if(timeElasped >= m_pConnection->getKeepAliveData().getTimeoutWarning()) {
      if(!m_HasBeenWarned) {
        //trace.
        m_HasBeenWarned = true;
        connectionSlow = true;
      }
    }
    else {
        clearFlags();
    }
  }

  if(pClientTransport != nullptr) /*pClientTransport->*/;
  if(connectionSlow) m_pConnection->onConnectionSlow();
}

void HeartBeatMonitor::reconnected() {
  clearFlags();
}

bool HeartBeatMonitor::hasBeenWarned() {
  return m_HasBeenWarned;
}

bool HeartBeatMonitor::timedOut() {
  return m_TimedOut;
}

}
