#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QTranslator>
#include <QString>
#include <QDir>
#include <QDateTime>
#include <QSharedMemory>
#include <QTimer>

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

	//多开检测
	QString uniqueKey = "PHD-TCP-SERVER-1761642818";
	QSharedMemory sharedMemory(uniqueKey);
	QTimer::singleShot(100, [&]() {
		if (sharedMemory.attach())
		{
			//退出程序
			qApp->quit();
			return;
		}
		else
		{
			sharedMemory.create(1);
			//启动程序
			InitApp();
		}
	});

    return a.exec();
}
