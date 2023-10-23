#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editdialog.h"

#include <QMainWindow>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonEdit_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *mTable;
};
#endif // MAINWINDOW_H
