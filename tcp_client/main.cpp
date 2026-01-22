#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QSharedMemory>
#include <QTimer>

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.22
   > GitHub: https://github.com/peihaodong/Protocol.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

void InitApp()
{
	//创建主窗口
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//启动程序
	InitApp();

    return a.exec();
}
