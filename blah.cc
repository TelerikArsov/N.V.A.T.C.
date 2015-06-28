#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <cmath>

using namespace cv;
using namespace std;

void rotate(Mat& src, double angle, Mat& dst)
{
    int len = max(src.cols, src.rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(src, dst, r, Size(len, len));
}

Point2f computeIntersect(Vec4i a, Vec4i b)
{
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
    if (float d = ((float)(x1-x2) *(y3-y4)) - ((y1-y2) *(x3-x4)))
    {
        Point2f pt;
        pt.x = ((x1*y2 - y1*x2)  *(x3-x4) - (x1-x2)*  (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2)  *(y3-y4) - (y1-y2)*  (x3*y4 - y3*x4)) / d;
        return pt;
    }
    else
        return Point2f(-1, -1);
}

int main()
{
    cout << "Enter filename: ";
    string file;
    cin >> file;
    const char* imageName = file.c_str();


    Mat image;
    image = imread( imageName, 1 );

    if(!image.data )
    {
        printf( " No image data \n " );
        return -1;
    }

    Mat gray_image;
    Mat gray_image_rep;
    cvtColor( image, gray_image, CV_BGR2GRAY );

    Canny(gray_image, gray_image_rep, 0, 1, 3);
    vector<Vec4i> lines;
    HoughLinesP(gray_image_rep, lines, 1, CV_PI/180, 1, 0, 0);
    
    vector<Point2f> corners;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i+1; j < lines.size(); j++)
        {
            Point2f pt = computeIntersect(lines[i], lines[j]);
            if (pt.x >= 0 && pt.y >= 0)
                corners.push_back(pt);
        }
    }
    
    vector<Vec4i> v;
    v.resize(lines.size());
    int j=0;
    for (int i = 0; i < lines.size(); i++)
    {
       if(abs(lines[i][0]-lines[i][2]>40) || abs(lines[i][1]-lines[i][3]>40))
        {
            //v[j][0]=lines[i][0];v[j][1]=lines[j][1];v[j][2]=lines[i][2];v[j][3]=lines[i][3];
            line(image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), CV_RGB(0,255,0));
            //j++;
        }
    }
    
    //cout<<j;
    //int a = v[0][0]-v[0][2];
    //cout << "a: "<< a <<"v[0][0]: " << v[0][0]<< "v[0][2]: "<< v[0][2]<<endl;
    //int b = v[0][1]-v[0][3];
    //cout << "b: "<< b <<"v[0][1]: " << v[0][1]<< "v[0][3]: "<< v[0][3]<<endl;
    //int c = sqrt(a*a+b*b);
    //float alpha = asin((float)a/c);
    //Mat RotatedImage;
    //rotate(image,alpha,RotatedImage);
    
    int endHeight = 165;
	int endWidth = 240;
	int answerCount = 4;
	int questionCount = 19;
	int counter[questionCount][answerCount];
	char marks [questionCount];
	int y=144,x=219;

	for(int i = 0; i < questionCount;i++)
	{
		for(int j=0; j < answerCount;j++)
		{
			counter[i][j] = 0;
		}
	}

	for(int i = 0; i < questionCount;i++)
	{
		for(int j=0; j < answerCount;j++)
		{
			for(;y<=endHeight;y++)
			{
				//if(i<2)
				//{
				//	cout<<"i: "<<i<<" j: "<<j<<" y: "<<y<<" x: "<<x<<" endWidth: "<<endWidth<<" endHeight: "<<endHeight<<endl;
				//}
				for(;x<=endWidth;x++)
				{
					Scalar intensity = gray_image.at<uchar>(y,x);
					//image.at<Vec3b>(y,x)[0] = 255;
					 //image.at<Vec3b>(y,x)[1] = 123;
					 //image.at<Vec3b>(y,x)[2] = 215;
					 if(intensity.val[0] < 100)
					 {
						  counter[i][j]++;
					 }
				}
				x = 219 + j*45;
			}
			if( counter[i][j] > 60)
			{
				//cout<<"j: "<<j<<endl;
				switch (j)
				{
					case 0:
						marks[i] = 'a';
						break;
					case 1:
						marks[i] = 'b';
						break;
					case 2:
						marks[i] = 'c';
						break;
					case 3:
						marks[i] = 'd';
						break;
				}
			}
			y = 144 + i*37;
			endWidth += 45;
		}
		endWidth = 240;
		x = 219;
		endHeight += 37;
	}

	char answers[questionCount];

	for(int i = 0; i < questionCount; i++)
	{
		cout << (i+1) << ": "<< marks[i]<<endl; 
	}
	for(int i = 0; i < questionCount; i++)
	{
		for(int j = 0; j < answerCount; j++)
		{
			cout << (i+1) << ": "<< counter[i][j]<<endl; 
		}
	}

	Scalar intensitytest = gray_image.at<uchar>(0,0);
	cout << "intensityS"<<intensitytest.val[0];
	
	//cout << "Molq Vuvedete pravilnete otgovori"<<endl;
	//for(int i = 0; i < questionCount; i++)
	//{
	//		cout<<i + 1<<"vpros:"<< ": ";
	//		cin >> answers[i];
	//		cout<<endl;
	//}

	int guessed = 0;
	for(int i = 0; i < questionCount; i++ )
	{
		if(answers[i] == marks[i])
		{
			guessed++;
		}
	}
	cout << guessed;
	namedWindow( imageName, CV_WINDOW_AUTOSIZE );
	namedWindow( "Gray imageEdges", CV_WINDOW_AUTOSIZE );
	//cout << "alpaha : "<<alpha << endl; 
	imshow( imageName, image );
	imshow( "Gray imageEdges", gray_image_rep );

	waitKey(0);

	return 0;
	}
	
			
