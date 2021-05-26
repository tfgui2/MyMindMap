#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QKeyEvent>
#include "global.h"
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include "helpdialog.h"
#include "minimapwidget.h"
#include "finddialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    global::instance()->statusbar = ui->statusbar;
    global::instance()->mainwindow = this;

    QVBoxLayout *layout = new QVBoxLayout;

    scene = new MyGraphicsScene(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    view = new QGraphicsView(scene);

    layout->addWidget(view);
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);


    minimap = new MinimapWidget(widget);
    minimap->setGeometry(10,10, 500,500);
    minimap->hide();
    minimap->scene = scene;

    findDlg = new FindDialog(widget);
    findDlg->hide();

    // 초기 타이틀바
    titlebarName();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this](){autosave();});
    timer->start(1000 * 60);

    this->clearScene();

    // lastfile 자동로드 - 이건 제일 마지막에.
    checkLastFileLoad();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::findstring(QString str)
{
    QList<QString> strlist;

    QList<int> list = scene->findstring(str);
    foreach (int i, list) {
        QString s = QString::number(i);
        strlist.append(s);
    }

    return strlist;
}

void MainWindow::centerto(int id)
{
    scene->centerOn(id, view);
}

void MainWindow::centerto(QPoint scenepos)
{
    view->centerOn(scenepos);
}

void MainWindow::clearScene()
{
    MyGraphicsScene* oldscene = scene;

    scene = new MyGraphicsScene(this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    view->setScene(scene);
    view->centerOn(2300, 0);

    minimap->scene = scene;

    if (oldscene != NULL)
        delete oldscene;

    g_modified = false;
}

void MainWindow::on_actionExit_triggered()
{

    this->close();
}

void MainWindow::on_actionDelete_triggered()
{
    scene->deleteSelectedItem();
}

void MainWindow::on_actionUndo_triggered()
{
    scene->undoStack->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    scene->undoStack->redo();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && event->isAutoRepeat() == false) {
        view->setDragMode(QGraphicsView::ScrollHandDrag);
        g_key_space = true;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space && event->isAutoRepeat() == false) {
        view->setDragMode(QGraphicsView::NoDrag);
        g_key_space = false;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    global::instance()->setLastFileName(currentFileName);

    if (AskSave()) {
        on_actionSave_triggered();
    }

    // TODO ask close
}

void MainWindow::checkLastFileLoad()
{
    QString filename = global::instance()->getLastFileName();
    if (QFile::exists(filename)) {
        currentFileName = filename;
        titlebarName();
        this->openFile(currentFileName);
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (AskSave()) {
        on_actionSave_triggered();
    }

    clearScene();
    currentFileName = "";
    titlebarName();
}

void MainWindow::on_actionOpen_triggered()
{
    if (AskSave()) {
        on_actionSave_triggered();
    }

    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilter("mind map data (*.mmd)");
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QStringList files = dlg.selectedFiles();
    if (files.length() > 0) {
        currentFileName = files[0];

        clearScene();
        titlebarName();
        this->openFile(currentFileName);
    }

}

void MainWindow::on_actionSave_triggered()
{
    if (currentFileName.isEmpty()) {
        QFileDialog dlg;
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        dlg.setNameFilter("mind map data (*.mmd)");
        if (dlg.exec() != QDialog::Accepted) {
            return;
        }

        QStringList files = dlg.selectedFiles();
        if (files.length() > 0) {
            currentFileName = files[0];
            titlebarName();
        }
    }

    global::fileBackup(currentFileName);
    this->saveFile(currentFileName);

    // 파일을 세이브한 경우 모디파이를 초기화
    g_modified = false;
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setNameFilter("mind map data (*.mmd)");
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QStringList files = dlg.selectedFiles();
    if (files.length() > 0) {
        QString newfilename = files[0];
        this->saveFile(newfilename);
    }
}

void MainWindow::openFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    this->scene->load(in);

}

void MainWindow::saveFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    this->scene->save(out);

    global::instance()->statusMessage("file saved! : " + filename + " at " + QDateTime::currentDateTime().toString());
}

void MainWindow::titlebarName()
{
    QString extra = currentFileName;
    if (extra.isEmpty()) {
        extra = "untitled";
    }
    this->setWindowTitle("MyMindMap - " + extra);
}

bool MainWindow::AskSave()
{
    // 변경한게 없으면 return false;
    if (g_modified == false) {
        return false;
    }

    QMessageBox box;
    box.setWindowTitle("Notice");
    box.setText("Do you want to save current document?");
    box.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    box.setDefaultButton(QMessageBox::Ok);

    if (box.exec() == QMessageBox::Ok) {
        return true;
    }

    return false;
}

void MainWindow::on_actionSize_triggered()
{
    this->scene->resizerSelectedItem();
}

void MainWindow::on_actionDetach_triggered()
{
    this->scene->detachSelectedItem();
}

void MainWindow::on_actionColor_triggered()
{
    QColor old = scene->getColorSelectedItem();
    if (old == Qt::black) {
        return;
    }

    QColor c = QColorDialog::getColor(old);
    if (c.isValid()) {
        this->scene->colorChangeSelectedItem(c);
    }
}

void MainWindow::on_actionHelp_triggered()
{
    HelpDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionAdjust_triggered()
{
    this->scene->adjustSelectedItem();
    global::instance()->statusMessage("item adjusted!");
}

void MainWindow::on_actionFont_Color_triggered()
{
    QColor old = scene->getFontColorSelectedItem();
    if (old == Qt::black) {
        return;
    }

    QColor c = QColorDialog::getColor(old);
    if (c.isValid()) {
        this->scene->fontcolorChangeSelectedItem(c);
    }
}

void MainWindow::on_actionMinimap_triggered()
{
    if (minimap->isHidden()) {
        minimap->show();
    }
    else {
        minimap->hide();
    }
}

void MainWindow::on_actionFind_triggered()
{
    findDlg->show();
}

void MainWindow::autosave()
{
    qDebug("auto save");
    if (g_modified == false) {
        return;
    }
    if (currentFileName.isEmpty()) {
        return;
    }

    on_actionSave_triggered();
}
