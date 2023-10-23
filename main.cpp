#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("upnvy_perpustakaan");
    db.setUserName("root");
    db.setPassword("root");
    if (!db.open()) {
        qWarning() << "Cannot connect to database";
        a.exit(0);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
