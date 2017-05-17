#include "mainwindow.h"
#include <QApplication>
#include "qopencvwidget.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>



int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
   // MainWindow w;
    //w.show();

   // return a.exec();


   // CvCapture * camera = cvCreateCameraCapture(0);
     CvCapture * camera;
    //CvCapture * camera = cvCaptureFromCAM( 0 );
    //CvCapture * camera = cvCaptureFromFile("http://192.168.0.100/image1");
    //assert(camera);
   // IplImage * image=cvQueryFrame(camera);
  //  assert(image);

   // printf("Image depth=%i\n", image->depth);
   // printf("Image nChannels=%i\n", image->nChannels);

    QApplication app(argc, argv);
    MainWindow *mainWin = new MainWindow(camera);
    //mainWin->setWindowTitle("OpenCV --> QtImage");
    mainWin->show();
    int retval = app.exec();

    cvReleaseCapture(&camera);

    return retval;

}
