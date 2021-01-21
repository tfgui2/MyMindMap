#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>


namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

private slots:
    void on_buttonBox_accepted();
    void on_lineEdit_returnPressed();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

public:
    void find();

private:
    Ui::FindDialog *ui;


    // QDialog interface
public slots:
    virtual void accept() override;
};

#endif // FINDDIALOG_H
