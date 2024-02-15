#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace cv;
using namespace std;

void initArrays(list<int>& ledsLeft, list<int>& ledsTop, list<int>& ledsRight, list<int>& ledsBottom)
{
    for(int i=0; i < 17; i++)
        ledsLeft.push_back(i);
    for(int i=17; i < 46; i++)
        ledsTop.push_back(i);
    for(int i=46; i < 62; i++)
        ledsRight.push_back(i);
    for(int i=62; i < 91; i++)
        ledsBottom.push_back(i);
}

void showlist(list<int> g)
{

    list<int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';
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

void leftBorder(Mat& image, list<int>& leds)
{

    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int top = 100;
    int bottom = 500;
    int sectionSizeY = (bottom-top) / leds.size();
    int sectionSizeX = imageWidth * 0.25;


    for( int i=0; i<leds.size(); i++){
        
        int startRow = top + (i * sectionSizeY);
        int endRow = top + ((i+1) * sectionSizeY);
        int startColumn = 0;
        int endColumn = sectionSizeX;
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b color = dominantColor(cropped_image);

        cout << "color " << i << " : " << color << endl;
        
        string img_title = to_string(i);
        //imshow(img_title, cropped_image);       
        
    }
}

void rightBorder(Mat& image, list<int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int top = 100;
    int bottom = 500;
    int sectionSizeY = (bottom-top) / leds.size();
    int sectionSizeX = imageWidth * 0.25;


    for( int i=0; i<leds.size(); i++){
        
        int startRow = top + (i * sectionSizeY);
        int endRow = top + ((i+1) * sectionSizeY);
        int startColumn = imageWidth - sectionSizeX;
        int endColumn = imageWidth;
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b color = dominantColor(cropped_image);

        cout << "color " << i << " : " << color << endl;
        
        string img_title = to_string(i);
        //imshow(img_title, cropped_image);       
        
    }
}

void topBorder (Mat& image, list<int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int left = 100;
    int right = 900;
    int sectionSizeY = imageHeight * 0.25;
    int sectionSizeX = (right-left) / leds.size();


    for( int i=0; i<leds.size(); i++){
        
        int startRow = 0;
        int endRow = sectionSizeY;
        int startColumn = left + (i * sectionSizeX);
        int endColumn = left + ((i+1) * sectionSizeX);
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b color = dominantColor(cropped_image);

        cout << "color " << i << " : " << color << endl;
        
        string img_title = to_string(i);
        //imshow(img_title, cropped_image);       
    }
}

void bottomBorder (Mat& image, list<int>& leds)
{
    int imageWidth = image.size().width;
    int imageHeight = image.size().height;

    int left = 100;
    int right = 900;
    int sectionSizeY = imageHeight * 0.25;
    int sectionSizeX = (right-left) / leds.size();


    for( int i=0; i<leds.size(); i++){
        
        int startRow = imageHeight - sectionSizeY;
        int endRow = imageHeight;
        int startColumn = left + (i * sectionSizeX);
        int endColumn = left + ((i+1) * sectionSizeX);
        Mat cropped_image = image(Range(startRow, endRow), Range(startColumn, endColumn)).clone();
        Vec3b color = dominantColor(cropped_image);

        cout << "color " << i << " : " << color << endl;
        
        string img_title = to_string(i);
        //imshow(img_title, cropped_image);       
    }
}
 

int main(int argc, char** argv )
{

    auto start_time = std::chrono::high_resolution_clock::now();

    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    image = imread( argv[1], IMREAD_COLOR );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    resize(image, image, Size(), 0.5, 0.5);

    list<int> ledsLeft, ledsTop, ledsRight, ledsBottom;
    initArrays(ledsLeft, ledsTop, ledsRight, ledsBottom);


    cout << "Width : " << image.size().width << endl;
    cout << "Height: " << image.size().height << endl;
    cout<<"Channels: :"<< image.channels() << endl;

    leftBorder(image, ledsLeft);
    rightBorder(image, ledsRight);
    topBorder(image,ledsTop);
    bottomBorder(image, ledsBottom);

    auto current_time = std::chrono::high_resolution_clock::now();
    std::cout << "Program has been running for " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << "mili seconds" << std::endl;

 
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}