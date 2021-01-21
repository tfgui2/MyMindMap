#include "finddialog.h"
#include "ui_finddialog.h"
#include "global.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_buttonBox_accepted()
{

}

void FindDialog::on_lineEdit_returnPressed()
{

}

void FindDialog::find()
{
    QString str = ui->lineEdit->text();
    QList<QString> res = global::instance()->find(str);

    ui->listWidget->clear();
    ui->listWidget->addItems(res);
}

void FindDialog::accept()
{
    this->find();
}

void FindDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->text().toInt();
    global::instance()->centerto(id);
}
