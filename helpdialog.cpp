#include "helpdialog.h"
#include "ui_helpdialog.h"

#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>


HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QFile file("help.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString text = in.readAll();
        ui->textEdit->setText(text);
    }
    else {
        ui->textEdit->setText(file.errorString());
        QFileInfo info(file);
        ui->textEdit->append(info.absoluteFilePath());
    }

}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_buttonBox_accepted()
{

}
