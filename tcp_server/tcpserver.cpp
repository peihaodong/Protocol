#include "tcpserver.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

TcpServer::TcpServer(quint16 nServerPort, QObject *parent /*= nullptr*/)
	: QObject(parent), m_nServerPort(nServerPort)
{
	m_server = std::make_shared<QTcpServer>();
	connect(m_server.get(), &QTcpServer::newConnection, this, &TcpServer::slotNewConnection);//新的socket连接

	//开启监听
	m_server->listen(QHostAddress::Any, m_nServerPort);
}

TcpServer::~TcpServer()
{
	//断开所有客户端连接
	m_mapClient.clear();
	//停止监听
	m_server->close();
}

std::shared_ptr<TcpServer> TcpServer::New(quint16 nServerPort)
{
	return std::make_shared<TcpServer>(nServerPort);
}

void TcpServer::slotNewConnection()
{
	QTcpSocket* socket = m_server->nextPendingConnection();
	QString strClientIP = socket->peerAddress().toString();
	quint16 nClientPort = socket->peerPort();

	QString msg = QString("[%1:%2] connect!!!").arg(strClientIP).arg(nClientPort);
	emit signalMessage(msg);

	auto iter = m_mapClient.find(socket);
	if (iter == m_mapClient.end())
	{
		PTcpClient client = TcpClient::New(socket, strClientIP, nClientPort);
		m_mapClient.insert(std::make_pair(socket, client));
		connect(client.get(), &TcpClient::signalClientDisconnected, this, &TcpServer::slotClientDisconnected);
		connect(client.get(), &TcpClient::signalMessage, this, &TcpServer::signalMessage);
		connect(client.get(), &TcpClient::signalUserChat, this, &TcpServer::slotUserChat);
	}
}

void TcpServer::slotClientDisconnected(QTcpSocket* socket)
{
	auto iter = m_mapClient.find(socket);
	if (iter != m_mapClient.end())
	{
		m_mapClient.erase(iter);
	}
}

void TcpServer::slotUserChat(protocol::PProtocol protocol)
{
	for (auto& iter: m_mapClient)
	{
		iter.second->SendMsg(protocol);
	}
}
