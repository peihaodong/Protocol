#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

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

	//发送按钮槽函数
	void slotSendPB();

protected:
	void InitUI();
	void InitTCP();

private:
	Ui::MainWindowClass *ui;
	int m_nRowsLimit = 100;
};
