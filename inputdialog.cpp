#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::setData(QString data)
{
    ui->textEdit->setPlainText(data);
}

QString InputDialog::getData() const
{
    QString data = ui->textEdit->toPlainText();
    return data;
}
