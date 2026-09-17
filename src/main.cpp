#include <stdio.h>


#ifndef GLAD_GL_IMPLEMENTATION
	#include "../glad/glad.h"
	#define GLAD_GL_IMPLEMENTATION
#endif

#include <GLFW/glfw3.h>

#include "../ticktackto/include/tick-tack-to.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>


/*#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>*/
#define min(x,y) x>y?y:x
#define max(x,y) x>y?x:y



int main(){
        if(!glfwInit()){
		return 1;
	}

	TickInitWindowFlags();
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	glfwMakeContextCurrent(window);//seting our widow context
	gladLoadGL();//Init our opengl her, this will get all the function adrress of opengl to used;


	TickInit(window); //And the


	char check=0;
	float slid=0.0;
	double tflf = glfwGetTime();



	int camera = 0;
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open(camera);
	if (!cap.isOpened()) {
	        return -1;
	}
	// wait for a new frame from camera and store it into 'frame'

	cap.read(frame);

	cv::cvtColor(frame, frame,cv::COLOR_BGR2RGB);

	// check if we succeeded
	if (frame.empty()) {
	        printf("ERROR! blank frame grabbed\n");
	}

	TickTexture2D videotexture = LoadTexture(frame.data, frame.cols, frame.rows, 24);


	while(!glfwWindowShouldClose(window) ){
		TickNewFrame();//you have to call this in evry frime; or you will have a bed time!


		DrawText("Hello World!, World Hello! , whozl[fkgopjg]", 300, 20,(Vec4c){255,255,255,255});
		CheckBox("Check Box Bora Broa Broa .", 20, 300, (char*)&check);

		cap.read(frame);
			// check if we succeeded
		if (frame.empty()) {
		        printf("ERROR! blank frame grabbed\n");
		}
		//cv::cvtColor(frame, frame,cv::COLOR_BGR2RGB);
		ReloadTexture(&videotexture, frame.data, frame.cols, frame.rows, 3);
		float frameHdem = (float)frame.cols/(float)frame.rows;
		DrawTexture(videotexture, (float)0,
	                                  (float)0,
					min(GetWindowW()*frameHdem,GetWindowH())*frameHdem,
					min(GetWindowW(),GetWindowH()/frameHdem)
		);
		/******************** Render ********************/
		DrawCircle(GetWindowW() / 2,GetWindowH() - 40, 30, 360, {245,245,240,255});

		TickRendre(); /*render our Tick data*/
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clering up our screen
		tflf=glfwGetTime () ;
	}

	glfwTerminate();
}
