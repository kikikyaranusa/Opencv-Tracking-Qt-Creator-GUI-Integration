#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qopencvwidget.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

//class MainWindow : public QMainWindow
class MainWindow : public QWidget

{
    Q_OBJECT
    private:
        QOpenCVWidget *cvwidget;
        CvCapture *camera;

    public:
        MainWindow(CvCapture *cam, QWidget *parent=0);


          //~MainWindow();
    protected:
        void timerEvent(QTimerEvent*);
        void onMouse( int event, int x, int y, int, void* );
        void help();

//public:
 //   explicit MainWindow(QWidget *parent = 0);
  //  ~MainWindow();

private slots:
        void on_startBt_clicked();

        void on_webcamRb_clicked();

private:
    Ui::MainWindow *ui;

/*
    void mouseMoveEvent(QMouseEvent *ev) {
            // vvv That's where the magic happens
            QTransform t;
            t.scale(1, -1);
            t.translate(0, -height()+1);
            QPoint pos = ev->pos() * t;
            // ^^^
            //setText(QString("Pos: %1, %2 \r").arg(pos.x()).arg(pos.y()));
            QString qs =QString("Pos: %1, %2").arg(pos.x()).arg(pos.y());
            std::string str = qs.toStdString();
            const char* p = str.c_str();
            //printf(p);
            qDebug(p);

        }
        */
};


#endif // MAINWINDOW_H
