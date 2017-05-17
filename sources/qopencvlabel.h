#ifndef QOPENCVLABEL
#define QOPENCVLABEL

#endif // QOPENCVLABEL

#include <QMouseEvent>

class QOpenCVLabel : public QLabel {
public:
    QOpenCVLabel(){
        this->setMouseTracking(true);


    }
public:
    QPoint m_pos;
    QPoint m_mCursor;
    QPoint m_pCursor;
    bool m_startSelection;
private:


    void mouseMoveEvent(QMouseEvent *ev) {
            // vvv That's where the magic happens
            QTransform t;
            t.scale(1, -1);
            t.translate(0, -height()+1);
            m_pos = ev->pos() * t;
             m_mCursor = ev->globalPos() - geometry().topLeft();

      }

    void mousePressEvent(QMouseEvent *event)
    {
        QTransform t;
        t.scale(1, -1);
        t.translate(0, -height()+1);
        //QPoint m_pos = ev->pos() * t;
        //m_pos = ev->pos() * t;
        if(event->button() == Qt::LeftButton)
        {
           // m_pCursor = event->globalPos() - geometry().topLeft();
            m_pCursor = event->pos() * t;
            event->accept();
            m_startSelection =true;
        }
    }

};


