#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog),
    mBookTable(new QSqlTableModel()),
    mPenerbitTable(new QSqlTableModel())
{
    ui->setupUi(this);

    mBookTable->setTable("Buku");
    mPenerbitTable->setTable("Penerbit");
}

EditDialog::~EditDialog()
{
    delete ui;
    delete mBookTable;
}

void EditDialog::setBookId(int id)
{
    this->mBookId = id;
    mBookTable->setFilter(QString("Kode_Buku = %1").arg(mBookId));

    if (!mBookTable->select())
        qWarning() << "Select failed";

    QSqlRecord record = mBookTable->record(0);
    this->ui->labelKodeBuku->setNum(this->mBookId);
    this->ui->editJudul->setText(record.field(mBookTable->fieldIndex("Judul")).value().toString());
    this->ui->editPengarang->setText(record.field(mBookTable->fieldIndex("Pengarang")).value().toString());
    this->ui->spinJumlahBuku->setValue(record.field(mBookTable->fieldIndex("Jml_Buku")).value().toInt());

    if (!mPenerbitTable->select()) qWarning() << "Select penerbit failed";

    this->ui->comboPenerbit->setModel(mPenerbitTable);
    this->ui->comboPenerbit->setModelColumn(mPenerbitTable->fieldIndex("Nama"));

    int penerbitId = record.field(mBookTable->fieldIndex("Kode_Penerbit")).value().toInt();
    int idColumnIndexInPenerbit = mPenerbitTable->fieldIndex("Kode_Penerbit");
    for (int i = 0; i < mPenerbitTable->rowCount(); i++) {
        int currentPenerbitId = mPenerbitTable->record(i).field(idColumnIndexInPenerbit).value().toInt();
        if (currentPenerbitId == penerbitId) {
            this->ui->comboPenerbit->setCurrentIndex(i);
            break;
        }
    }
}

void EditDialog::accept()
{
    this->mBookTable->setData(
        this->mBookTable->index(0, mBookTable->fieldIndex("Judul")),
        this->ui->editJudul->text());
    this->mBookTable->setData(
        this->mBookTable->index(0, mBookTable->fieldIndex("Pengarang")),
        this->ui->editPengarang->text());
    this->mBookTable->setData(
        this->mBookTable->index(0, mBookTable->fieldIndex("Jml_Buku")),
        this->ui->spinJumlahBuku->value());
    this->mBookTable->setData(
                        this->mBookTable->index(0, mBookTable->fieldIndex("Kode_Penerbit")),
                        this->mPenerbitTable->record(
                            this->ui->comboPenerbit->currentIndex()
                                ).field(mPenerbitTable->fieldIndex("Kode_Penerbit")).value().toInt());
    this->mBookTable->submitAll();
    done(Accepted);
}
