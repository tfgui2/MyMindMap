#ifndef GLOBAL_H
#define GLOBAL_H

#include <QObject>
#include <QStatusBar>
#include "mainwindow.h"

extern bool g_key_space;
extern bool g_modified;

class global
{
public:
    static global *instance();
protected:
    global();

    // file
public:
    static bool fileBackup(QString filename); // filename을 받으면 여기에 수정을 해서 백업파일로 카피한다.

public: // statusbar
    QStatusBar *statusbar;
    void statusMessage(QString msg);

public: // MainWindow;
    MainWindow *mainwindow;
    QList<QString> find(QString str);
    void centerto(int id);

public: // setting
    void setLastFileName(QString filename);
    QString getLastFileName();
};

#endif // GLOBAL_H
