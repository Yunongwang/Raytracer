#include "mainwindow.h"
#include <QPixmap>
#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "RTcontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Ray_Trace_TW)
{
    Raytracingflag = false;
    ui->setupUi(this);
    QPixmap pix(QApplication::applicationDirPath() + "/outPut.ppm");
    ui->label_pic->setPixmap(pix);
    ui->label_pic->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->status_label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    setStatusBar(Raytracingflag);
    ui->label_pic->setStyleSheet("border: 1.25px solid black;");
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusBar(bool rting){
    if(rting) {
        ui->status_label->setStyleSheet("background: red;");
        ui->status_label->setText("RT in progress");
    } else {
        ui->status_label->setText("Waiting");
        ui->status_label->setStyleSheet("background: green;");
    }
}


void MainWindow::threadStarted()
{
    Raytracingflag = true;
    setStatusBar(Raytracingflag);
}

void MainWindow::threadStop()
{
    QPixmap pix(QApplication::applicationDirPath() + "/outPut.ppm");
    int w = ui->label_pic->width();
    int h = ui->label_pic->height();
    ui->status_label->setText("Waiting");
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    Raytracingflag = false;
    setStatusBar(Raytracingflag);
}

void MainWindow::onButtonClicked()
{
    if(Raytracingflag == false){
        QThread *thread = QThread::create([]{ outPutRayTracer(); });
        thread->setObjectName("WorkerThread"); // name to appear in ps, task manager, etc.
        connect(thread, &QThread::started, this, &MainWindow::threadStarted);
        connect(thread, &QThread::finished, this, &MainWindow::threadStop);
        thread->start();
    }
}

