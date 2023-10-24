#include "editdialog.h"
#include "ui_editdialog.h"

class BookItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    BookItemDelegate(QObject* parent = nullptr) :
        QStyledItemDelegate(parent) {
    }

    void setPenerbitModel(QSqlTableModel* model) {
        mPenerbitModel = model;
    }

    void setPenerbitCombo(QComboBox* combo) {
        mPenerbitCombo = combo;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const {
        if (editor == mPenerbitCombo) {
            int penerbitFIeldIndex = mPenerbitModel->fieldIndex("Kode_Penerbit");
            int penerbitId = index.data().toInt();

            for (int i = 0; i < mPenerbitModel->rowCount(); i++) {
                int currentPenerbitId = mPenerbitModel->record(i).field(penerbitFIeldIndex).value().toInt();
                if (currentPenerbitId == penerbitId) {
                    mPenerbitCombo->setCurrentIndex(i);
                    break;
                }
            }
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
        if (editor == mPenerbitCombo) {
            int penerbitFieldIndex = mPenerbitModel->fieldIndex("Kode_Penerbit");
            int penerbitId = mPenerbitModel->record(mPenerbitCombo->currentIndex()).field(penerbitFieldIndex).value().toInt();
            model->setData(index, penerbitId);
        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }
private:
    QSqlTableModel *mPenerbitModel;
    QComboBox *mPenerbitCombo;
};

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog),
    mBookModel(new QSqlTableModel()),
    mPenerbitModel(new QSqlTableModel())
{
    ui->setupUi(this);

    mBookModel->setTable("Buku");
    mBookModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mPenerbitModel->setTable("Penerbit");

    mBookMapper.setModel(mBookModel);
    BookItemDelegate *delegate = new BookItemDelegate(this);
    delegate->setPenerbitCombo(ui->comboPenerbit);
    delegate->setPenerbitModel(mPenerbitModel);
    mBookMapper.setItemDelegate(delegate);
    mBookMapper.addMapping(ui->labelKodeBuku, mBookModel->fieldIndex("Kode_Buku"));
    mBookMapper.addMapping(ui->editJudul, mBookModel->fieldIndex("Judul"));
    mBookMapper.addMapping(ui->editPengarang, mBookModel->fieldIndex("Pengarang"));
    mBookMapper.addMapping(ui->spinJumlahBuku, mBookModel->fieldIndex("Jml_Buku"));
    mBookMapper.addMapping(ui->comboPenerbit, mBookModel->fieldIndex("Kode_Penerbit"));
}

EditDialog::~EditDialog()
{
    delete ui;
    delete mBookModel;
    delete mPenerbitModel;
}

void EditDialog::setBookId(int id)
{
    this->mBookId = id;
    mBookModel->setFilter(QString("Kode_Buku = %1").arg(mBookId));

    if (!mBookModel->select())
        qWarning() << "Select failed";

    updatePenerbitCombo();
    mBookMapper.toFirst();
}

void EditDialog::setAddBook()
{
    this->mBookId = -1;
    updatePenerbitCombo();
    if (!this->mBookModel->insertRows(0, 1))
        qInfo() << "Insert row failed";

    mBookMapper.removeMapping(ui->labelKodeBuku);
    this->ui->labelKodeBuku->setText("Buku Baru");
    mBookMapper.toFirst();
}

void EditDialog::accept()
{
    if (this->mBookId == -1) {
        QSqlQuery query;
        query.exec("SELECT MAX(Kode_Buku) + 1 FROM Buku");
        query.first();
        this->mBookModel->setData(this->mBookModel->index(0, mBookModel->fieldIndex("Kode_Buku")),
            query.value(0).toInt());
    }

    if (!this->mBookModel->submitAll()) {

        qWarning() << "insert failed";
        qWarning() << mBookModel->lastError();
    }
    done(Accepted);
}

void EditDialog::updatePenerbitCombo()
{
    if (!mPenerbitModel->select()) qWarning() << "Select penerbit failed";

    this->ui->comboPenerbit->setModel(mPenerbitModel);
    this->ui->comboPenerbit->setModelColumn(mPenerbitModel->fieldIndex("Nama"));
}

#include "editdialog.moc"
