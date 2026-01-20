#pragma once

#include <QObject>
#include <memory>
#include <QTcpServer>
#include <QTcpSocket>
#include <map>
#include "tcpclient.h"

class TcpServer  : public QObject
{
	Q_OBJECT

public:
	TcpServer(quint16 nServerPort, QObject *parent = nullptr);
	~TcpServer();

	static std::shared_ptr<TcpServer> New(quint16 nServerPort);

signals:
	void signalMessage(QString msg, QColor colorText = QColor(0, 0, 0), QColor colorBackground = QColor(255, 255, 255));

protected slots:
	void slotNewConnection();
	void slotClientDisconnected(QTcpSocket* socket);

	void slotUserChat(protocol::PProtocol protocol);

protected:
	quint16 m_nServerPort = 10080;
	std::shared_ptr<QTcpServer> m_server;
	std::map<QTcpSocket*, PTcpClient> m_mapClient;
};
using PTcpServer = std::shared_ptr<TcpServer>;