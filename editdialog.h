#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();

    void setBookId(int id);
    void setAddBook();

public Q_SLOTS:
    void accept() override;

private:
    Ui::EditDialog *ui;
    int mBookId;
    QSqlTableModel *mBookTable;
    QSqlTableModel *mPenerbitTable;

    void updatePenerbitCombo();
};

#endif // EDITDIALOG_H
