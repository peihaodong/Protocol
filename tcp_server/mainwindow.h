#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected slots:
	void slotMessage(QString msg, QColor colorText = QColor(0, 0, 0), QColor colorBackground = QColor(255, 255, 255));

protected:
	void InitUI();

private:
	Ui::MainWindowClass *ui;

	int m_nRowsLimit = 100;
	PTcpServer m_server;
};
