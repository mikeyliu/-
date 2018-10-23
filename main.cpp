#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style/qss.css");
    if( file.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(file.readAll());

        a.setStyleSheet(styleSheet);
        file.close();

    }else
    {

        QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    }
    MainWindow w;
    w.show();

    return a.exec();
}
