#include "tcpclient.h"

TcpClient::TcpClient(QTcpSocket* socket, const QString& strClientIP, quint16 nClientPort, QObject *parent /*= nullptr*/)
	: QObject(parent), m_socket(socket), m_strClientIP(strClientIP), m_nClientPort(nClientPort)
{
	connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::slotReadyRead);
	connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::slotDisconnected);
	connect(m_socket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &TcpClient::slotErrorOccurred);
	qRegisterMetaType<protocol::PProtocol>("protocol::PProtocol");
	connect(this, &TcpClient::signalSendMessage, this, &TcpClient::slotSendMessage);

	m_socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 50 * 1024 * 1024);
	m_socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 50 * 1024 * 1024);
}

TcpClient::~TcpClient()
{
	if (m_socket->state() == QTcpSocket::ConnectedState)
		m_socket->disconnectFromHost();

	m_socket->deleteLater();
}

std::shared_ptr<TcpClient> TcpClient::New(QTcpSocket* socket, const QString& strIP, quint16 nPort)
{
	return std::make_shared<TcpClient>(socket, strIP, nPort);
}

void TcpClient::SendMsg(const protocol::PProtocol& protocol)
{
	emit signalSendMessage(protocol);
}

void TcpClient::slotReadyRead()
{
	m_datas.append(m_socket->readAll());

	while (true)
	{
		size_t size = m_datas.size();
		if (size < sizeof(protocol::SMsgHead))
		{
			return;
		}

		protocol::SMsgHead head;
		memcpy(&head, m_datas.data(), sizeof(protocol::SMsgHead));
		if (size < head.m_len)
		{
			return;
		}

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>(m_datas.mid(0, head.m_len));
		WholePackData(datas);
		m_datas.remove(0, head.m_len);
	}
}

void TcpClient::slotDisconnected()
{
	QString error = m_socket->errorString();
	QString msg = QString("[%1:%2] disconnect %3!!!").arg(m_strClientIP).arg(m_nClientPort).arg(error);
	emit signalMessage(msg);
	emit signalClientDisconnected(m_socket);
}

void TcpClient::slotErrorOccurred(QAbstractSocket::SocketError err)
{
	Q_UNUSED(err);

	QString error = m_socket->errorString();
	QString msg = QString("[%1:%2] %3!!!").arg(m_strClientIP).arg(m_nClientPort).arg(error);
	emit signalMessage(msg);
}

void TcpClient::slotSendMessage(protocol::PProtocol protocol)
{
	if (m_socket->state() != QTcpSocket::ConnectedState)
		return;

	std::shared_ptr<QByteArray> datas = protocol->GetData();
	qint64 butesSent = m_socket->write(*datas.get());

	return;
}

void TcpClient::WholePackData(std::shared_ptr<QByteArray> datas)
{
	protocol::SMsgHead head;
	memcpy(&head, datas->data(), sizeof(protocol::SMsgHead));

	//ÊÕµ½Ö¸Áî
	QString msg = QString("[%1:%2] recv cmd:%3, id:%4").arg(m_strClientIP).arg(m_nClientPort).arg(head.m_type).arg(head.m_id);
	emit signalMessage(msg);

	switch (head.m_type)
	{
	case protocol::EMsgType::EMT_Heartbeat_Ask:
	{
		protocol::PProtocolHeartbeatAsk ask = protocol::ProtocolHeartbeatAsk::New();
		ask->ParseData(datas);
		DisposeHeartbeatAsk(ask);
	}
	break;
	case protocol::EMsgType::EMT_Chat_Ask:
	{
		protocol::PProtocolChatAsk ask = protocol::ProtocolChatAsk::New();
		ask->ParseData(datas);
		DisposeChatAsk(ask);
	}
	break;
	}
}

void TcpClient::DisposeHeartbeatAsk(protocol::PProtocolHeartbeatAsk ask)
{
	if (m_socket->state() != QTcpSocket::ConnectedState)
		return;

	protocol::PProtocolHeartbeatReply reply = protocol::ProtocolHeartbeatReply::New();
	reply->m_id = ask->m_id;

	std::shared_ptr<QByteArray> datas = reply->GetData();
	qint64 bytesSent = m_socket->write(*datas.get());

	return;
}

void TcpClient::DisposeChatAsk(protocol::PProtocolChatAsk ask)
{
	protocol::PProtocolChatReply reply = protocol::ProtocolChatReply::New();
	reply->m_string = ask->m_string;
	reply->m_strClientIP = m_strClientIP;
	reply->m_nClientPort = m_nClientPort;
	emit signalUserChat(reply);
}
