//
//  main.cpp
//  HumanProjection
//
//  Created by over-hrk on 2017/03/26.
//  Copyright © 2017年 over-hrk. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[]) {
    
    cv::VideoCapture cap(0);
    
    if( !cap.isOpened() )
    {
        std::cerr << "Cannot open the camera." << std::endl;
        return 1;
    }
    
    double w = 640, h = 480;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, h);
    
    int B = 255;
    int G = 0;
    int R = 0;
    
    const int ksize = 3;
    
    cv::Mat frame;
    cv::Mat filtered;
    cv::Mat merged;
    cv::vector<cv::Mat> color_shuffle;
    color_shuffle.resize(3);
    
    //背景画像を取得 スペースで背景画像取得
    cv::Mat bg;
    while (cv::waitKey(1) != 32) {
        cap >> frame;
        frame.copyTo(bg);
        imshow("bg", bg);
    }
    
    cv::Mat diff, grey, mask;
    const int th = 20;
    
    while(1)//無限ループ
    {
        cap >> frame; // get a new frame from camera
        cv::Laplacian(frame, filtered, frame.depth(), ksize);
        
        cv::absdiff(frame,bg,diff);//背景画像との差分を取得
        cvtColor(diff,grey,CV_BGR2GRAY);//差分画像をグレイスケールに
        cv::threshold(grey, mask, th, 255, cv::THRESH_BINARY);//二値化画像化
        
        cv::vector<cv::Mat> planes;
        
        // 3つのチャネルB, G, Rに分離 (OpenCVではデフォルトでB, G, Rの順)
        cv::split(filtered, planes);
        
        cv::threshold( planes[0], color_shuffle[0], 10, B, cv::THRESH_BINARY_INV);
        cv::threshold( planes[1], color_shuffle[1], 10, G, cv::THRESH_BINARY_INV);
        cv::threshold( planes[2], color_shuffle[2], 10, R, cv::THRESH_BINARY_INV);
        
        cv::merge(color_shuffle,  merged);
        cv::imshow("window", merged);//画像を表示．
        cv::imshow("mask", mask);//画像を表示．
        
        int key = cv::waitKey(1);
        if(key == 113) // "q"
        {
            break;
        }
        else if(key == 115) // "s"
        {
            B = rand() % 255;
            G = rand() % 255;
            R = rand() % 255;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
