/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */


#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <QtConcurrent/QtConcurrent>
#include <QJsonObject>
#include "../../Common/common.h"

class Client : public QObject
{
  Q_OBJECT
public:
  explicit Client(QObject *parent = 0);
  ~Client();

private:
  QLocalSocket *m_socket;
  void serverMessageLoop();
  bool            m_running;
  QFuture<void>   m_clientLoopThread;

signals:
  void signalFromClient(signalType, QVariantMap);

public slots:
   void messageFromUI(signalType, QVariantMap);
};

#endif // CLIENT_H
