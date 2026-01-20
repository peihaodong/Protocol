#include "tcpclient.h"
#include <QHostAddress>
#include <windows.h>
#include <TlHelp32.h>

TcpClient::TcpClient(QObject *parent /*= nullptr*/)
	: QObject(parent), m_timerConnect(this)
{
	moveToThread(&m_thread);
	m_thread.start();

	connect(this, &TcpClient::signalInit, this, &TcpClient::slotInit);
	connect(this, &TcpClient::signalConnect, this, &TcpClient::slotConnect);
	qRegisterMetaType<protocol::PProtocol>("protocol::PProtocol");
	connect(this, &TcpClient::signalSendMessage, this, &TcpClient::slotSendMessage);
	connect(this, &TcpClient::signalClose, this, &TcpClient::slotClose);

	connect(&m_timerConnect, &QTimer::timeout, [=]() {
		if (m_bConnect)
		{
			m_timerConnect.stop();
		}
		else
		{
			Connect(m_strServerIP, m_nServerPort);
		}
	});
	m_timerConnect.setInterval(2000);

	emit signalInit();
}

TcpClient::~TcpClient()
{
	emit signalClose();

	m_thread.quit();
	m_thread.wait();
}

std::shared_ptr<TcpClient> TcpClient::New()
{
	return std::make_shared<TcpClient>();
}

void TcpClient::Connect(const QString& strServerIP, quint16 nServerPort)
{
	emit signalConnect(strServerIP, nServerPort);
}

void TcpClient::SendMsg(const protocol::PProtocol& protocol)
{
	emit signalSendMessage(protocol);
}

void TcpClient::slotInit()
{
	m_timerConnect.start();
}

void TcpClient::slotConnect(QString strServerIP, quint16 nServerPort)
{
	if (!m_socket)
	{
		m_socket = std::make_shared<QTcpSocket>();
		connect(m_socket.get(), &QTcpSocket::connected, this, &TcpClient::slotConnected);//连接成功
		connect(m_socket.get(), &QTcpSocket::disconnected, this, &TcpClient::slotDisconnected);//断开连接
		connect(m_socket.get(), &QTcpSocket::readyRead, this, &TcpClient::slotReadyRead);//收到数据
		connect(m_socket.get(), static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &TcpClient::slotErrorOccurred);//发生错误

		m_socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, 50 * 1024 * 1024);
		m_socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 50 * 1024 * 1024);
	}

	m_strServerIP = strServerIP;
	m_nServerPort = nServerPort;

	//若已连接，先断开
	if (m_socket->state() == QAbstractSocket::ConnectedState)
		m_socket->disconnectFromHost();

	//发起连接
	m_socket->connectToHost(QHostAddress(m_strServerIP), m_nServerPort);
}

void TcpClient::slotSendMessage(protocol::PProtocol protocol)
{
	if (m_socket->state() != QTcpSocket::ConnectedState)
		return;

	std::shared_ptr<QByteArray> datas = protocol->GetData();
	qint64 butesSent = m_socket->write(*datas.get());

	return;
}

void TcpClient::slotClose()
{
	if (m_socket && m_socket->state() == QTcpSocket::ConnectedState)
		m_socket->disconnectFromHost();
}

void TcpClient::slotConnected()
{
	m_bConnect = true;
	emit signalConnectStatus(m_bConnect);
}

void TcpClient::slotDisconnected()
{
	m_bConnect = false;
	emit signalConnectStatus(m_bConnect);
	m_timerConnect.start();
}

void TcpClient::slotReadyRead()
{
	m_datas.append(m_socket->readAll());
	while (true)
	{
		if (m_datas.size() < sizeof(protocol::SMsgHead))
			return;

		protocol::SMsgHead head;
		memcpy(&head, m_datas.data(), sizeof(protocol::SMsgHead));
		if (m_datas.size() < head.m_len)
			return;

		std::shared_ptr<QByteArray> datas = std::make_shared<QByteArray>(m_datas.mid(0, head.m_len));
		WholePackData(datas);
		m_datas.remove(0, head.m_len);
	}
}

void TcpClient::slotErrorOccurred(QAbstractSocket::SocketError err)
{
	QString error = m_socket->errorString();

	if (err == QAbstractSocket::SocketError::RemoteHostClosedError)
	{
		m_bConnect = false;
		emit signalConnectStatus(m_bConnect);
		m_timerConnect.start();
	}
}

void TcpClient::WholePackData(std::shared_ptr<QByteArray> datas)
{
	protocol::SMsgHead head;
	memcpy(&head, datas->data(), sizeof(protocol::SMsgHead));

	switch (head.m_type)
	{
	case protocol::EMsgType::EMT_Heartbeat_Reply:
	{
		protocol::PProtocolHeartbeatReply reply = protocol::ProtocolHeartbeatReply::New();
		reply->ParseData(datas);
		DisposeHeartbeatReply(reply);
	}
	break;
	case protocol::EMsgType::EMT_Chat_Reply:
	{
		protocol::PProtocolChatReply reply = protocol::ProtocolChatReply::New();
		reply->ParseData(datas);
		DisposeChatReply(reply);
	}
	break;
	}
}

void TcpClient::DisposeHeartbeatReply(protocol::PProtocolHeartbeatReply reply)
{

}

void TcpClient::DisposeChatReply(protocol::PProtocolChatReply reply)
{

}

bool TcpClient::IsProcessRunning(const QString& processName)
{
	//创建进程快照
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	//遍历进程
	if (::Process32First(hSnapshot, &pe32))
	{
		do 
		{
			//得到进程名
			QString exeName = QString::fromWCharArray(pe32.szExeFile);
			//比较进程名（忽略大小）
			if (exeName.compare(processName, Qt::CaseInsensitive) == 0)
			{
				::CloseHandle(hSnapshot);
				return true;
			}
		} while (::Process32Next(hSnapshot, &pe32));
	}

	::CloseHandle(hSnapshot);
	return false;
}

bool TcpClient::RunProcess(const QString& pathProcess)
{
	STARTUPINFOW si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = {0};

	BOOL success = ::CreateProcess(pathProcess.toStdWString().c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!success)
		return false;

	::CloseHandle(pi.hThread);
	::CloseHandle(pi.hProcess);

	return true;
}

bool TcpClient::StopProcess(const QString& processName)
{
	//创建进程快照
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	DWORD pid = 0;

	//遍历进程
	if (::Process32First(hSnapshot, &pe32))
	{
		do
		{
			//得到进程名
			QString exeName = QString::fromWCharArray(pe32.szExeFile);
			//比较进程名（忽略大小）
			if (exeName.compare(processName, Qt::CaseInsensitive) == 0)
			{
				pid = pe32.th32ProcessID;
				break;
			}
		} while (::Process32Next(hSnapshot, &pe32));
	}

	::CloseHandle(hSnapshot);
	
	if (pid == 0)
		return false;

	//打开进程，获取句柄（需要终止权限）
	HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProcess == NULL)
		return false;

	//终止进程
	BOOL success = ::TerminateProcess(hProcess, 0);
	if (!success)
	{
		::CloseHandle(hProcess);
		return false;
	}

	::CloseHandle(hProcess);
	return true;
}

TcpClientManager* TcpClientManager::GetInstance()
{
	static TcpClientManager inst;
	return &inst;
}

TcpClientManager::~TcpClientManager()
{
	m_thread.quit();
	m_thread.wait();
}

bool TcpClientManager::IsConnect() const
{
	return m_bConnect;
}

void TcpClientManager::SendMsg(const protocol::PProtocol& protocol)
{
	emit signalSendMsg(protocol);
}

void TcpClientManager::slotInit()
{
	if (!m_clientTCP)
		return;

	m_clientTCP = TcpClient::New();
	connect(m_clientTCP.get(), &TcpClient::signalConnectStatus, this, &TcpClientManager::slotConnectStatus);
	qRegisterMetaType<protocol::PProtocolHeartbeatReply>("protocol::PProtocolHeartbeatReply");
}

void TcpClientManager::slotSendMsg(const protocol::PProtocol& protocol)
{
	if (m_clientTCP)
	{
		m_clientTCP->SendMsg(protocol);
	}
}

void TcpClientManager::slotConnectStatus(bool bConnect)
{
	m_bConnect = bConnect;
	emit signalTcpClientConnectStatus(bConnect);
}

TcpClientManager::TcpClientManager(QObject* parent /*= nullptr*/)
{
	moveToThread(&m_thread);
	m_thread.start();

	connect(this, &TcpClientManager::signalInit, this, &TcpClientManager::slotInit);
	qRegisterMetaType<protocol::PProtocol>("protocol::PProtocol");
	connect(this, &TcpClientManager::signalSendMsg, this, &TcpClientManager::slotSendMsg);

	emit signalInit();
}
