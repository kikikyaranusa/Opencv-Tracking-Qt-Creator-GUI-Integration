#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>


using namespace std;
using namespace cv;

static Mat image;
static Rect2d boundingBox;
static bool paused;
static bool selectObject = false;
static bool startSelection = false;

string tracker_algorithm;
bool initialized;
Ptr<Tracker> tracker;

//sprintf(string, "#%d,Posterior %.2f; fps: %.2f, #numwindows:%d, %s", imAcq->currentFrame - 1, tld->currConf, fps, tld->detectorCascade->numWindows, learningString);
CvScalar yellow = CV_RGB(255, 255, 0);
CvScalar blue = CV_RGB(0, 0, 255);
CvScalar black = CV_RGB(0, 0, 0);
CvScalar white = CV_RGB(255, 255, 255);
CvFont font;


static const char* keys =
{ "{@tracker_algorithm | | Tracker algorithm }"
    "{@video_name      | | video name        }"
    "{@start_frame     |0| Start frame       }"
    "{@bounding_frame  |0,0,0,0| Initial bounding frame}"};

void MainWindow::onMouse( int event, int x, int y, int, void* )
{
  if( !selectObject )
  {
    switch ( event )
    {
      case EVENT_LBUTTONDOWN:
        //set origin of the bounding box
        startSelection = true;
        boundingBox.x = x;
        boundingBox.y = y;
        break;
      case EVENT_LBUTTONUP:
        //sei with and height of the bounding box
        boundingBox.width = std::abs( x - boundingBox.x );
        boundingBox.height = std::abs( y - boundingBox.y );
        paused = false;
        selectObject = true;
        break;
      case EVENT_MOUSEMOVE:

        if( startSelection && !selectObject )
        {
          //draw the bounding box
          Mat currentFrame;
          image.copyTo( currentFrame );
          rectangle( currentFrame, Point((int) boundingBox.x, (int)boundingBox.y ), Point( x, y ), Scalar( 255, 0, 0 ), 2, 1 );
          //imshow( "Tracking API", currentFrame );
          //qcvwidget->putImage(image);
        }
        break;
    }
  }
}

void MainWindow::help()
{
  cout << "\nThis example shows the functionality of \"Long-term optical tracking API\""
       "-- pause video [p] and draw a bounding box around the target to start the tracker\n"
       "Example of <video_name> is in opencv_extra/testdata/cv/tracking/\n"
       "Call:\n"
       "./tracker <tracker_algorithm> <video_name> <start_frame> [<bounding_frame>]\n"
       "tracker_algorithm can be: MIL, BOOSTING, MEDIANFLOW, TLD\n"
       << endl;

  cout << "\n\nHot keys: \n"
       "\tq - quit the program\n"
       "\tp - pause video\n";
}

MainWindow::MainWindow(CvCapture *cam, QWidget *parent) : QWidget(parent) , ui(new Ui::MainWindow){
    //camera = cam;
    QVBoxLayout *layout = new QVBoxLayout;
    cvwidget = new QOpenCVWidget(this);

    //setMouseTracking(true);

     //camera = cvCaptureFromCAM( 0 );
    //camera = cvCaptureFromFile("http://192.168.0.100/image1");
     //camera = cvCaptureFromFile("http://192.168.1.99/video.mjpg");
   //camera = cvCreateCameraCapture(0);
   //assert(camera);
   //IplImage *image=cvQueryFrame(camera);
   boundingBox.width =100;
   boundingBox.height=100;

    //cvwidget->imagelabel->resize(image->width, image->height);
    //formLayout->addWidget(cvwidget);
    //Ui::MainWindow::formLayout->addWidget(cvwidget);
    //layout->addWidget(cvwidget);
    //setLayout(layout);
    //resize(500, 400);

    //startTimer(100);  // 0.1-second timer
     ui->setupUi(this);
     ui->formLayout->addWidget(cvwidget);
 }

void MainWindow::timerEvent(QTimerEvent*) {
    IplImage *image=cvQueryFrame(camera);
    cv::Mat frame = cv::cvarrToMat(image);
    char scstring[128];
    char mstring[128];


    //--calculate fps
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, 8);
    double fps=cvGetCaptureProperty(camera, CV_CAP_PROP_FPS);

   sprintf(scstring, "fps: %.2f,position %d,%d", fps, cvwidget->imagelabel->m_pos.x(), 480-cvwidget->imagelabel->m_pos.y());
   sprintf(mstring, "pos press: %d,%d screen: %d %d",cvwidget->imagelabel->m_pCursor.x(), cvwidget->imagelabel->m_pCursor.y(), image->width, image->height);

    //std::cout << "FPS: " << fps << std::endl;
   /*
    double toc = (cvGetTickCount() - tic) / cvGetTickFrequency();
    toc = toc / 1000000;
    float fps = 1 / toc;*/
    //cvRectangle(image, cvPoint(100,100), cvPoint(50,50),yellow, 2,8,0);
    cvLine(image, cvPoint((int)image->width/2-50,(int)image->height/2), cvPoint((int)image->width/2+50,(int)image->height/2),yellow, 2,8,0);
    cvLine(image, cvPoint((int)image->width/2,(int)image->height/2-50), cvPoint((int)image->width/2,(int)image->height/2+50),yellow, 2,8,0);

    cvPutText(image, scstring, cvPoint(25, 25), &font, white);
    cvPutText(image, mstring, cvPoint(25, 40), &font, white);
   if (cvwidget->imagelabel->m_startSelection)
       startSelection = true;

    //--draw box tracking --------------------
    if( startSelection )
    {
      //---draw the bounding box---
        boundingBox.width = ui->bbwidthSb->value();
        boundingBox.height= ui->bbheightSb->value();
        startSelection = false;
        cvwidget->imagelabel->m_startSelection =false;

        boundingBox.x = cvwidget->imagelabel->m_pos.x();
        boundingBox.y = 480-cvwidget->imagelabel->m_pos.y();
     // rectangle(frame, Point((int) boundingBox.x, (int)boundingBox.y ), Point( x, y ), Scalar( 255, 0, 0 ), 2, 1 );
        cvRectangle(image,Point((int) (boundingBox.x-boundingBox.width/2), (int)(boundingBox.y-boundingBox.height/2)),Point((int) (boundingBox.x+boundingBox.width/2),(int) (boundingBox.y+boundingBox.height/2)), CV_RGB(255,0,0),5,8);
        selectObject = true;
    }

    if (selectObject){
        cvRectangle(image,Point((int) (boundingBox.x-boundingBox.width/2), (int)(boundingBox.y-boundingBox.height/2)),Point((int) (boundingBox.x+boundingBox.width/2),(int) (boundingBox.y+boundingBox.height/2)), CV_RGB(255,0,0),5,8);

        tracker_algorithm=ui->algolCb->currentText().toStdString();
        tracker = Tracker::create( tracker_algorithm );
        if( tracker == NULL )
        {
          cout << "***Error in the instantiation of the tracker...***\n";
          //return -1;
        }
         tracker->init(frame, boundingBox) ;
         initialized = true;

    }
    //----tracking here --------------
    if(initialized)
    {
      //----updates the tracker--------------------------
      if( tracker->update(frame, boundingBox ) )
      {
          cvRectangle(image,Point((int) (boundingBox.x-boundingBox.width/2), (int)(boundingBox.y-boundingBox.height/2)),Point((int) (boundingBox.x+boundingBox.width/2),(int) (boundingBox.y+boundingBox.height/2)), CV_RGB(255,0,0),5,8);
          selectObject=false;
      }

    }
    //--end tracking ------------------

    cvwidget->putImage(image);
    //quit on ESC button


}


void MainWindow::on_startBt_clicked()
{
    initialized = false;
    tracker_algorithm=ui->algolCb->currentText().toStdString();
    tracker = Tracker::create( tracker_algorithm );
    if( tracker == NULL )
    {
      cout << "***Error in the instantiation of the tracker...***\n";
      //return -1;
    }
    //--start button -------------------
    //--update setting ------------
    boundingBox.width = ui->bbwidthSb->value();
    boundingBox.height= ui->bbheightSb->value();
    tracker_algorithm = ui->algolCb->currentText().toStdString();
    if (ui->webcamRb->isChecked())
    {
        camera = cvCreateCameraCapture(0);
        assert(camera);
        startTimer(100);  // 0.1-second timer
        //ui->httpEd->setText("webcam-walah");
    }
    else if (ui->httpRb->isChecked())
    {

        QByteArray ba =  ui->inputEd->text().toLatin1();
        const char *c_str2 = ba.data();
        camera = cvCaptureFromFile(c_str2);
        //camera = cvCaptureFromFile("rtsp://192.168.1.99:554/live.sdp");
        //camera = cvCaptureFromFile("http://192.168.1.99/video.mjpg");
        assert(camera);
        startTimer(100); // 0.1-second timer
        //ui->httpEd->setText("http-walah");

    }
    else if (ui->rtspRb ->isChecked())
    {

        QByteArray ba =  ui->inputEd->text().toLatin1();
        const char *c_str2 = ba.data();
        camera = cvCaptureFromFile(c_str2);
        assert(camera);
        startTimer(100);
    }
}

void MainWindow::on_webcamRb_clicked()
{
    //--radio button webcam ----------------
}
