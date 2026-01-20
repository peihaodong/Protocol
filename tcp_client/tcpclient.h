#pragma once

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <atomic>
#include <QColor>
#include <QThread>
#include <QTimer>
#include "chatprotocol.h"

class TcpClient  : public QObject
{
	Q_OBJECT

public:
	TcpClient(QObject *parent = nullptr);
	~TcpClient();

	static std::shared_ptr<TcpClient> New();

	void Connect(const QString& strServerIP, quint16 nServerPort);

	void SendMsg(const protocol::PProtocol& protocol);

signals:
	void signalInit();
	void signalConnect(QString strServerIP, quint16 nServerPort);
	void signalSendMessage(protocol::PProtocol protocol);
	void signalClose();
	void signalConnectStatus(bool bConnect);

protected slots:
	void slotInit();
	void slotConnect(QString strServerIP, quint16 nServerPort);
	void slotSendMessage(protocol::PProtocol protocol);
	void slotClose();

	//连接成功
	void slotConnected();
	//断开连接
	void slotDisconnected();
	//收到数据
	void slotReadyRead();
	//发送错误
	void slotErrorOccurred(QAbstractSocket::SocketError err);

protected:
	void WholePackData(std::shared_ptr<QByteArray> datas);
	void DisposeHeartbeatReply(protocol::PProtocolHeartbeatReply reply);
	void DisposeChatReply(protocol::PProtocolChatReply reply);

public:
	static bool IsProcessRunning(const QString& processName);
	static bool RunProcess(const QString& pathProcess);
	static bool StopProcess(const QString& processName);

protected:
	QThread m_thread;
	std::shared_ptr<QTcpSocket> m_socket;
	QTimer m_timerConnect;
	QString m_strServerIP = "127.0.0.1";
	quint16 m_nServerPort = 10080;
	QByteArray m_datas;
	std::atomic_bool m_bConnect = false;
};
using PTcpClient = std::shared_ptr<TcpClient>;

class TcpClientManager : public QObject
{
	Q_OBJECT

public:
	static TcpClientManager* GetInstance();
	~TcpClientManager();

	bool IsConnect() const;
	void SendMsg(const protocol::PProtocol& protocol);

signals:
	void signalInit();
	void signalSendMsg(const protocol::PProtocol& protocol);
	void signalTcpClientConnectStatus(bool bConnect);

protected slots:
	void slotInit();
	void slotSendMsg(const protocol::PProtocol& protocol);
	void slotConnectStatus(bool bConnect);

protected:
	TcpClientManager(QObject* parent = nullptr);

protected:
	QThread m_thread;
	PTcpClient m_clientTCP;
	std::atomic_bool m_bConnect = false;
};