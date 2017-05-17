
#ifndef QOPENCVWIDGET_H
#define QOPENCVWIDGET_H

#include <opencv/cv.h>
#include <QPixmap>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QImage>
#include <QMouseEvent>
#include "qopencvlabel.h"

class QOpenCVWidget : public QWidget {
    private:
        //QLabel *imagelabel;
        //QOpenCVLabel *imagelabel;
        QVBoxLayout *layout;

        QImage image;

    public:
        QOpenCVWidget(QWidget *parent = 0);
        ~QOpenCVWidget(void);
        void putImage(IplImage *);
        QOpenCVLabel *imagelabel;

};

#endif
