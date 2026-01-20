#include "mainwindow.h"
#include <QListWidgetItem>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	InitUI();

	quint64 nPort = 10080;
	m_server = TcpServer::New(nPort);
	connect(m_server.get(), &TcpServer::signalMessage, this, &MainWindow::slotMessage);
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

void MainWindow::InitUI()
{
	setWindowTitle(tr("Server"));
}
