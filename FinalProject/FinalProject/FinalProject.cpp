// FinalProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

Point findCenter(Rect rect);
bool intersect(Point lineP1, Point lineP2, Point rectCenter);

int main(int argc, char* argv[]) {
	const String fileName = "C:\\Users\\Alec\\Documents\\Visual Studio 2015\\Projects\\FinalProject\\Videos\\car_video2.mp4";
	VideoCapture capture(fileName) ;

	if (!capture.isOpened())
	{
		cout << "Error loading the video." << endl;
		int error = -1;
		return 1;
	}

	//Load the car cascades
	String car_cascade_name = "C:\\Users\\Alec\\Documents\\Visual Studio 2015\\Projects\\FinalProject\\haarcascade_cars.xml";
	CascadeClassifier car_cascade;

	if (!car_cascade.load(car_cascade_name))
	{
		printf("--(!)Error loading the cascade.\n"); return -1;
	}

	Mat frame;
	int numCars = 0;
	capture.read(frame);
	
	//Points for video 1
	//Point lineP1(55, 75);
	//Point lineP2(300, 90);

	//Points for video 2
	Point lineP1(80, 120);
	Point lineP2(200, 120);
	
	while (capture.isOpened()) {
		capture.read(frame);
		Mat drawingFrame = frame.clone();
		
		vector<Rect> cars;
		car_cascade.detectMultiScale(frame, cars, 1.1, 2);
		
		line(drawingFrame, lineP1, lineP2, (0, 0, 255), 2, 8, 0);

		for (int c = 0; c < cars.size(); c++) {
			Point rectCenterPt = findCenter(cars[c]);
			Rect centerRect (rectCenterPt.x, rectCenterPt.y, cars[c].width / 12, cars[c].height / 12);
			Mat thisCar = frame(centerRect);
			Scalar average = mean(thisCar);

			circle(drawingFrame, rectCenterPt, 3, (255, 255, 0), -1);
			rectangle(drawingFrame, Point(cars[c].x, cars[c].y), Point(cars[c].x + cars[c].width, cars[c].y + cars[c].height), average, 2);
			

			if (intersect(lineP1, lineP2, rectCenterPt)) {
				numCars++;
				cout << "INTERSECT: " << numCars << endl;
			}
		}

		imshow("Video", drawingFrame);

		char key = waitKey(33);
		if (key == 'q')
		{
			cout << "Final count: " << numCars << endl;
			break;
		}
	}

	destroyAllWindows();
}

Point findCenter(Rect rect) {
	return Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
}

bool intersect(Point lineP1, Point lineP2, Point rectCenter) {
	double slope = (lineP1.y - lineP2.y) / (lineP1.x - lineP2.x);
	double yIntersect = (slope*lineP1.x) + lineP1.y;
	return rectCenter.y == ((slope * rectCenter.x) + yIntersect);
}