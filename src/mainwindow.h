#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Ray_Trace_TW; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onButtonClicked();
    void threadStarted();
    void threadStop();


private:
    Ui::Ray_Trace_TW *ui;
    bool Raytracingflag;
    void setStatusBar(bool rting);
};
#endif // MAINWINDOW_H
