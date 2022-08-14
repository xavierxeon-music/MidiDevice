#include <Midi/MidiTunnelClient.h>

#include <QTimer>

Midi::Tunnel::Client::Client(QObject* parent, const QString& serverName)
   : Socket(parent)
   , serverName(serverName)
{
   QTimer* reconnectTimer = new QTimer(this);
   connect(reconnectTimer, &QTimer::timeout, this, &Client::slotTryToConnectToHost);
   reconnectTimer->start(10 * 1000);
}

void Midi::Tunnel::Client::slotTryToConnectToHost()
{
   if (QAbstractSocket::UnconnectedState != getState())
      return;

   static QTcpSocket* newSocket = nullptr;
   if (!newSocket)
      newSocket = new QTcpSocket(this);

   newSocket->connectToHost(serverName, port);
   if (newSocket->waitForConnected(1000))
   {
      setSocket(newSocket);
      newSocket = nullptr;
   }
}
