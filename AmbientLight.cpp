#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <cstdlib> 

using namespace cv;
using namespace std;

#define LEDS_LEFT 17
#define LEDS_TOP 29
#define LEDS_RIGHT 16
#define LEDS_BOTTOM 28
#define LEDS_TOTAL (LEDS_LEFT + LEDS_TOP + LEDS_RIGHT + LEDS_BOTTOM)

void initArrays(list<unsigned int>& ledsLeft, list<unsigned int>& ledsTop, list<unsigned int>& ledsRight, list<unsigned int>& ledsBottom)
{
    for(int i=0; i < LEDS_LEFT; i++)
        ledsLeft.push_back(0x00000000);
    for(int i=0; i < LEDS_TOP; i++)
        ledsTop.push_back(0x00000000);
    for(int i=0; i < LEDS_RIGHT; i++)
        ledsRight.push_back(0x00000000);
    for(int i=0; i < LEDS_BOTTOM; i++)
        ledsBottom.push_back(0x00000000);
}

void showlist(list<unsigned int> g)
{

    list<unsigned int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << std::hex << *it;
    cout << '\n';
}

unsigned int bgrToHex(Vec3b color) {
    unsigned int hexColor = (color[2] << 16) | (color[1] << 8) | color[0];
    return 0xFF000000 | hexColor;
}


Vec3b dominantColor(Mat& image)
{
    Mat reshapedImage = image.reshape(1, image.rows * image.cols);

    // Convert image to float type for better accuracy
    reshapedImage.convertTo(reshapedImage, CV_32FC3);

    // Perform k-means clustering to find dominant colors
    int K = 1; // number of clusters (dominant colors)
    Mat labels, centers;
    kmeans(reshapedImage, K, labels, TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 10, 1.0), 1, KMEANS_RANDOM_CENTERS, centers);

    // Convert center values back to uchar
    centers.convertTo(centers, CV_8UC1);
    Vec3b dominantColor = centers.at<Vec3b>(0);
    return dominantColor;

    
}

void leftBorder(Mat& image, list<unsigned int>& leds)
{

    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int top = 20;
    int bottom = 320;
    int sectionSizeY = (bottom-top) / leds.size();
    int sectionSizeX = imageWidth * 0.25;


    for( int i=0; i<leds.size(); i++){
        
        int startRow = bottom - ((i+1) * sectionSizeY);
        int endRow = bottom - (i * sectionSizeY);
        int startColumn = 0;
        int endColumn = sectionSizeX;
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b colorBGR = dominantColor(cropped_image);
        
        auto it = leds.begin();
        std::advance(it, i);
        *it = bgrToHex(colorBGR);    
        
    }
}

void rightBorder(Mat& image, list<unsigned int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int top = 20;
    int bottom = 320;
    int sectionSizeY = (bottom-top) / leds.size();
    int sectionSizeX = imageWidth * 0.25;


    for( int i=0; i<leds.size(); i++){
        
        int startRow = top + (i * sectionSizeY);
        int endRow = top + ((i+1) * sectionSizeY);
        int startColumn = imageWidth - sectionSizeX;
        int endColumn = imageWidth;
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b colorBGR = dominantColor(cropped_image);

        auto it = leds.begin();
        std::advance(it, i);
        *it = bgrToHex(colorBGR);  
        
    }
}

void topBorder (Mat& image, list<unsigned int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int left = 20;
    int right = 540;
    int sectionSizeY = imageHeight * 0.25;
    int sectionSizeX = (right-left) / leds.size();


    for( int i=0; i<leds.size(); i++){
        
        int startRow = 0;
        int endRow = sectionSizeY;
        int startColumn = left + (i * sectionSizeX);
        int endColumn = left + ((i+1) * sectionSizeX);
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b colorBGR = dominantColor(cropped_image);

        auto it = leds.begin();
        std::advance(it, i);
        *it = bgrToHex(colorBGR);
          
    }
}

void bottomBorder (Mat& image, list<unsigned int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int left = 20;
    int right = 540;
    int sectionSizeY = imageHeight * 0.25;
    int sectionSizeX = (right-left) / leds.size();


    for( int i=0; i<leds.size(); i++){
        
        int startRow = imageHeight - sectionSizeY;
        int endRow = imageHeight;
        int startColumn = right - ((i+1) * sectionSizeX);
        int endColumn = right - (i * sectionSizeX);
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b colorBGR = dominantColor(cropped_image);

        //cout << "color " << i << " : " << colorBGR << endl;
        
        auto it = leds.begin();
        std::advance(it, i);
        *it = bgrToHex(colorBGR);

        //string img_title = to_string(i);
        //imshow(img_title, cropped_image);       
    }
}

string buildHexString(const list<unsigned int>& list1,
                      const list<unsigned int>& list2,
                      const list<unsigned int>& list3,
                      const list<unsigned int>& list4) {
    stringstream ss;
    for (unsigned int value : list1) {
        ss << setw(8) << setfill('0') << hex << value << " ";
    }
    for (unsigned int value : list2) {
        ss << setw(8) << setfill('0') << hex << value << " ";
    }
    for (unsigned int value : list3) {
        ss << setw(8) << setfill('0') << hex << value << " ";
    }
    for (unsigned int value : list4) {
        ss << setw(8) << setfill('0') << hex << value << " ";
    }
    return ss.str();
}

void saveFrame(const cv::Mat& frame, const std::string& folder, int frameNumber) {
    std::stringstream filename;
    filename << folder << "/frame_" << std::setfill('0') << std::setw(5) << frameNumber << ".jpg";
    cv::imwrite(filename.str(), frame);
}

void intesifyColors(Mat& image)
{
    Mat hsvImage;
    cvtColor(image, hsvImage, COLOR_BGR2HSV);

    // Split the HSV image into individual channels
    vector<Mat> channels;
    split(hsvImage, channels);
    channels[1] *= 2;
    merge(channels, hsvImage);
    
    cvtColor(hsvImage, image, COLOR_HSV2BGR);

}

int main(int argc, char** argv )
{

    VideoCapture cap("/dev/video0");

    double fps = cap.get(cv::CAP_PROP_FPS);
    std::cout << "Frame rate of the video stream: " << fps << std::endl;

    int frameNumber = 0;
    while(true){

        cv::Mat frame;
        cap >> frame;
        resize(frame, frame, Size(580, 340));

        
        list<unsigned int> ledsLeft, ledsTop, ledsRight, ledsBottom;
        initArrays(ledsLeft, ledsTop, ledsRight, ledsBottom);

        if (frame.empty()) {
            cout << "End of video stream" << std::endl;
            break;
        }

        intesifyColors(frame);

        leftBorder(frame, ledsLeft);
        rightBorder(frame, ledsRight);
        topBorder(frame,ledsTop);
        bottomBorder(frame, ledsBottom);

        // Pass to C script
        string hexString = buildHexString(ledsLeft, ledsTop, ledsRight, ledsBottom);
        string command = "sudo ./ledtest " + hexString;
        int result = system(command.c_str());

        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    cap.release();

    waitKey(0);
    return 0;
}