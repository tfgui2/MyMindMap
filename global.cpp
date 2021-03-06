#include "global.h"
#include <QFile>
#include <QStatusBar>
#include <QCoreApplication>
#include <QSettings>

bool g_key_space = false;
bool g_modified = false;

global *global::instance()
{
    static global *_instance = nullptr;
    if (_instance == nullptr) {
        _instance = new global;
    }
    return _instance;
}

global::global()
{
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("MySoft.D");
    QCoreApplication::setApplicationName("MyMindMap");
}

bool global::fileBackup(QString filename)
{
    QString newname = filename + ".bak";
    QFile::remove(newname);
    return QFile::copy(filename, newname);
}

void global::statusMessage(QString msg)
{
    if (statusbar)
        statusbar->showMessage(msg, 5000);
}

QList<QString> global::find(QString str)
{
    QList<QString> list = mainwindow->findstring(str);

    return list;
}

void global::centerto(int id)
{
    mainwindow->centerto(id);
}

void global::centerto(QPoint scenepos)
{
    mainwindow->centerto(scenepos);
}

void global::setLastFileName(QString filename)
{
    QSettings settings;
    settings.setValue("lastfilename", filename);
}

QString global::getLastFileName()
{
    QSettings settings;
    QString filename = settings.value("lastfilename").toString();
    return filename;
}

