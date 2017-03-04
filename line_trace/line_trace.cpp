#include "opencv2/opencv.hpp"
#include <fstream>
#include <stdlib.h>
#include <signal.h>

int g_is_inv = 0;
int (*g_logic)(cv::Mat&);

/*
 * 7 8 9     ^
 * 4 5 6 = < N >
 * 1 2 3     v
 */
enum MOVE_DIRECTION {
  MOVE_BACK   = 2,
  MOVE_LEFT   = 4,
  MOVE_STOP   = 5,
  MOVE_RIGHT  = 6,
  MOVE_FOWARD = 8,
};

void move_car (int direction)
{
  int is_l_forard = 0;
  int is_l_back   = 0;
  int is_r_forard = 0;
  int is_r_back   = 0;

  switch (direction)
    {
    case MOVE_BACK:
      is_l_back   = 1;
      is_r_back   = 1;
      break;
    case MOVE_LEFT:
      is_l_forard = 1;
      //is_r_back   = 1;
      break;
    case MOVE_RIGHT:
      is_r_forard = 1;
      //is_l_back   = 1;
      break;
    case MOVE_FOWARD:
      is_l_forard = 1;
      is_r_forard = 1;
      break;
    case MOVE_STOP:
    default:
      break;
    }

  std::ofstream l_forard ("/sys/class/gpio/gpio67/value");
  std::ofstream l_back   ("/sys/class/gpio/gpio69/value");
  std::ofstream r_forard ("/sys/class/gpio/gpio71/value");
  std::ofstream r_back   ("/sys/class/gpio/gpio73/value");

  l_forard << is_l_forard << std::endl;
  l_back   << is_l_back   << std::endl;
  r_forard << is_r_forard << std::endl;
  r_back   << is_r_back   << std::endl;
}

int raster_scan (cv::Mat &frame)
{
  cv::Mat gray_img;
  cv::Mat bin_img; 
  cv::cvtColor(frame, gray_img, CV_BGR2GRAY);
  cv::threshold(gray_img,bin_img,0,255,
		(g_is_inv ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY) | cv::THRESH_OTSU);
  cv::GaussianBlur(bin_img, bin_img, cv::Size(11,11), 10, 10);

  int cnt = 0, cnt_max = 0, current_px = 0, start_px = 0, flag = 0;
  for (int i = 0; i < bin_img.size().width; i++)
    {
      char px = bin_img.at<unsigned char>(bin_img.size().height/4, i);
      if ((px == 0) && (flag == 0))
	{
	  flag = 1;
	  cnt = 1;
	  current_px = i;
	}
      if ((px == 0) && (flag == 1))
	{
	  cnt++;
	}
      if ((px == 255) || i == bin_img.size().width - 1)
	{
	  flag = 0;
	  if (cnt_max < cnt)
	    {
	      cnt_max = cnt;
	      start_px = current_px;
	    }
	  cnt = 0;
	}
    }
  int center_px = start_px + cnt_max / 2;

  //frame = bin_img;
  cv::circle(frame, cv::Point(center_px ,bin_img.size().height/4), 20, cv::Scalar(0,0,255), 3, 4);

  if (center_px > 0 && center_px < bin_img.size().width * 3 / 8)
    {
      return MOVE_RIGHT;
    }
  else if (center_px > bin_img.size().width * 5 / 8)
    {
      return MOVE_LEFT;
    }
  else
    {
      return MOVE_FOWARD;
    }
}

void main_loop (cv::VideoCapture &cap)
{
  cv::Mat frame;
  cap >> frame; // get a new frame from camera

  move_car(g_logic(frame));

  cv::imwrite("/var/tmp/img.jpg", frame);
}

int main(int argc, char* argv[])
{
  signal(SIGTERM,
	 [](int signum){
	   move_car(MOVE_STOP);
	   exit(signum);
	 });

  g_logic = raster_scan;

  if ((argc == 2) && (argv[1][0] = '1'))
    {
      g_is_inv = 1;
    }

  cv::VideoCapture cap(1); // /dev/video*

  if(!cap.isOpened())
    {
      return -1;
    }

  while(1)
    {
      main_loop(cap);
    }
  return 0;
}
