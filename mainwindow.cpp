#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mTable = new QSqlRelationalTableModel(ui->tableView);
    mTable->setTable("Buku");

    int penerbitIndex = mTable->fieldIndex("Kode_Penerbit");
    mTable->setRelation(penerbitIndex, QSqlRelation("Penerbit", "Kode_Penerbit", "Nama"));

    mTable->setHeaderData(penerbitIndex, Qt::Horizontal, "Penerbit");
    mTable->setHeaderData(mTable->fieldIndex("Jml_Buku"), Qt::Horizontal, "Jumlah Buku");
    mTable->setHeaderData(mTable->fieldIndex("Kode_Buku"), Qt::Horizontal, "Kode Buku");

    mTable->select();

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(mTable);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buttonEdit_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.size() != 1) {
        return;
    }

    int selectedRow = selected.at(0).row();
    QSqlRecord record = mTable->record(selectedRow);
    int bookId = record.field(mTable->fieldIndex("Kode_Buku")).value().toInt();
    EditDialog edit;
    edit.setModal(true);
    edit.setBookId(bookId);
    edit.exec();
    mTable->selectRow(selectedRow);
}


void MainWindow::on_buttonAdd_clicked()
{
    EditDialog edit;
    edit.setAddBook();
    edit.exec();
    mTable->select();
}

