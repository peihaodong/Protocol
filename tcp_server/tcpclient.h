#pragma once

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <QColor>
#include "chatprotocol.h"

class TcpClient  : public QObject
{
	Q_OBJECT

public:
	TcpClient(QTcpSocket* socket, const QString& strClientIP, quint16 nClientPort, QObject *parent = nullptr);
	~TcpClient();

	static std::shared_ptr<TcpClient> New(QTcpSocket* socket, const QString& strIP, quint16 nPort);

	void SendMsg(const protocol::PProtocol& protocol);

signals:
	void signalClientDisconnected(QTcpSocket* socket);

	void signalMessage(QString msg, QColor colorText = QColor(0, 0, 0), QColor colorBackground = QColor(255, 255, 255));

	void signalUserChat(protocol::PProtocol protocol);
	void signalSendMessage(protocol::PProtocol protocol);

protected slots:
	//收到数据
	void slotReadyRead();
	//断开连接
	void slotDisconnected();
	//发送错误
	void slotErrorOccurred(QAbstractSocket::SocketError err);

	void slotSendMessage(protocol::PProtocol protocol);

protected:
	void WholePackData(std::shared_ptr<QByteArray> datas);
	void DisposeHeartbeatAsk(protocol::PProtocolHeartbeatAsk ask);
	void DisposeChatAsk(protocol::PProtocolChatAsk ask);

protected:
	QTcpSocket* m_socket = nullptr;
	QString m_strClientIP;
	quint16 m_nClientPort = 0;
	QByteArray m_datas;
};
using PTcpClient = std::shared_ptr<TcpClient>;