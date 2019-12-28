#ifndef HEARTBEATMONITOR_HPP
#define HEARTBEATMONITOR_HPP

#include <mutex>
#include <QTimer>
#include <time_delta.h>

#include "iconnection.hpp"

namespace signalr {

class HeartBeatMonitor {
public:
  HeartBeatMonitor(std::shared_ptr<IConnection> pIConnection, std::shared_ptr<std::mutex> connectionStateLock, TimeDelta beatInterval);
  void start();
  void clearFlags();
  void beat();
  void beat(const TimeDelta& timeElasped);
  void checkKeepAlive(const TimeDelta& timeElasped);
  void reconnected();
  bool hasBeenWarned();
  bool timedOut();

private:
  QTimer m_Timer;
  std::shared_ptr<std::mutex> m_pConnectionStateLock;
  std::shared_ptr<IConnection> m_pConnection;
  TimeDelta m_BeatInterval;
  bool m_MonitorKeepAlive;
  bool m_HasBeenWarned;
  bool m_TimedOut;
};

}

#endif //heartbeatmonitor.cpp
