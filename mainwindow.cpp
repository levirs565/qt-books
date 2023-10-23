#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtSql>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlRelationalTableModel* model;
    model = new QSqlRelationalTableModel(ui->tableView);
    model->setTable("Buku");

    int penerbitIndex = model->fieldIndex("Kode_Penerbit");
    model->setRelation(penerbitIndex, QSqlRelation("Penerbit", "Kode_Penerbit", "Nama"));

    model->setHeaderData(penerbitIndex, Qt::Horizontal, "Penerbit");
    model->setHeaderData(model->fieldIndex("Jml_Buku"), Qt::Horizontal, "Jumlah Buku");
    model->setHeaderData(model->fieldIndex("Kode_Buku"), Qt::Horizontal, "Kode Buku");

    model->select();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

