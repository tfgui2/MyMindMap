#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "mygraphicsscene.h"
#include "minimapwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void clearScene();

private slots:
    void on_actionExit_triggered();
    void on_actionDelete_triggered();
    void on_actionSize_triggered();

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    // doc 관리
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    void on_actionDetach_triggered();

    void on_actionColor_triggered();

    void on_actionHelp_triggered();

    void on_actionAdjust_triggered();

    void on_actionFont_Color_triggered();

    void on_actionMinimap_triggered();

protected:
    void openFile(QString filename);
    void saveFile(QString filename);
    QString currentFileName = "";
    void titlebarName();

    bool AskSave();

private:
    Ui::MainWindow *ui;

    QGraphicsView *view;
    MyGraphicsScene *scene;

    MinimapWidget *minimap;


    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
    void checkLastFileLoad();
};
#endif // MAINWINDOW_H
