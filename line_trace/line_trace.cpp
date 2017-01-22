#include "opencv2/opencv.hpp"
#include <fstream>
#include <stdlib.h>

int g_is_inv = 0;

void stop_car (void) 
{
  std::ofstream l_front("/sys/class/gpio/gpio67/value");
  l_front << "0" << std::endl;
  std::ofstream l_back("/sys/class/gpio/gpio69/value");
  l_back << "0"<< std::endl;
  std::ofstream r_front("/sys/class/gpio/gpio71/value");
  r_front << "0" << std::endl;
  std::ofstream r_back("/sys/class/gpio/gpio73/value");
  r_back << "0"<< std::endl;
}

void main_loop (cv::VideoCapture &cap)
{
  cv::Mat frame;
  cap >> frame; // get a new frame from camera

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

  if (center_px > bin_img.size().width/2)
    {
      std::ofstream l_front("/sys/class/gpio/gpio67/value");
      l_front << "1" << std::endl;
      std::ofstream l_back("/sys/class/gpio/gpio69/value");
      l_back << "0"<< std::endl;
      std::ofstream r_front("/sys/class/gpio/gpio71/value");
      r_front << "0" << std::endl;
      std::ofstream r_back("/sys/class/gpio/gpio73/value");
      r_back << "0"<< std::endl;
    }
  else
    {
      std::ofstream l_front("/sys/class/gpio/gpio67/value");
      l_front << "0" << std::endl;
      std::ofstream l_back("/sys/class/gpio/gpio69/value");
      l_back << "0"<< std::endl;
      std::ofstream r_front("/sys/class/gpio/gpio71/value");
      r_front << "1" << std::endl;
      std::ofstream r_back("/sys/class/gpio/gpio73/value");
      r_back << "0"<< std::endl;
    }

  cv::circle(frame, cv::Point(center_px ,bin_img.size().height/4), 20, cv::Scalar(0,0,255), 3, 4);

  cv::imwrite("/var/tmp/img.jpg", frame);
}

int main(int argc, char* argv[])
{
  atexit(stop_car);

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
