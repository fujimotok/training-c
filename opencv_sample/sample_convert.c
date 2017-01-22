#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char **argv)
{
    int i;
    CvCapture *capture;
    IplImage *src;
    IplImage *dst_mono;
    IplImage *dst_edge;

    if (argc != 2)
      {
	printf("usege: sample_convert [device path]\n");
	return -1;
      }

    capture = cvCreateCameraCapture(atoi(argv[1]));
    
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);

    for (i = 0; i < 20; i++) {
        if ((src = cvQueryFrame(capture)) == NULL) {
            printf("camera error\n");
            return -1;
        }
    }

    dst_mono = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
    dst_edge = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

    cvCvtColor(src, dst_mono, CV_BGR2GRAY);
    cvCanny(dst_mono, dst_edge, 50.0, 200.0, 3);

    cvSaveImage("src.bmp", src, NULL);
    cvSaveImage("dst_mono.bmp", dst_mono, NULL);
    cvSaveImage("dst_edge.bmp", dst_edge, NULL);

    cvReleaseCapture(&capture);
    cvReleaseImage(&dst_mono);
    cvReleaseImage(&dst_edge);

    return 0;
}
