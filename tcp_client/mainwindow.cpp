#include "mainwindow.h"
#include <QListWidgetItem>
#include <QDateTime>
#include "tcpclient.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	InitUI();

	InitTCP();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slotMessage(QString msg, QColor colorText /*= QColor(0, 0, 0)*/, QColor colorBackground /*= QColor(255, 255, 255)*/)
{
	QListWidgetItem* pItem = new QListWidgetItem();
	QString strInfo = QString("%1 | %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss zzz")).arg(msg);
	pItem->setText(strInfo);
	pItem->setTextColor(colorText);
	pItem->setBackgroundColor(colorBackground);
	ui->listWidget->addItem(pItem);
	if (ui->listWidget->count() > m_nRowsLimit)
	{
		auto p = ui->listWidget->takeItem(0);
		delete p;
	}
	ui->listWidget->scrollToBottom();
}

void MainWindow::slotSendPB()
{
	QString strText = ui->textEdit->toPlainText();
	if (strText.isEmpty())
		return;

	TcpClientManager* pTcpClientManager = TcpClientManager::GetInstance();
	if (!pTcpClientManager->IsConnect())
		return;

	std::shared_ptr<protocol::ProtocolChatAsk> ask = protocol::ProtocolChatAsk::New();
	ask->m_string = strText;
	pTcpClientManager->SendMsg(ask);

	ui->textEdit->setText("");
}

void MainWindow::InitUI()
{
	setWindowTitle(tr("Client"));

	connect(ui->pb_Send, &QPushButton::clicked, this, &MainWindow::slotSendPB);
}

void MainWindow::InitTCP()
{
	if (!TcpClient::IsProcessRunning("TcpServer.exe"))
	{
		QString strPath = QCoreApplication::applicationDirPath() + "/TcpServer.exe";
		TcpClient::RunProcess(strPath);
	}

	TcpClientManager* pTcpClientManager = TcpClientManager::GetInstance();

	connect(pTcpClientManager, &TcpClientManager::signalMessage, this, &MainWindow::slotMessage);
}
