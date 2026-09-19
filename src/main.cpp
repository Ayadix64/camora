#include <atomic>
#include <cstring>
#include <pthread.h>
#include <stdio.h>
#include <thread>


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
#define pos_or_zeor(x) x<0?0:x


void *imagebuffer = NULL;

void captureThread( void* buff,
                    pthread_mutex_t* buffmut,
                    cv::VideoCapture* cap,
                    pthread_mutex_t* capmutex,
                    std::atomic_bool* sholdclose) //it runs on sepret thread to smooth the main window
{
        cv::Mat frame;
        while(!*sholdclose){
                pthread_mutex_lock(capmutex);
                cap->read(frame);
                cv::cvtColor(frame, frame,cv::COLOR_BGR2RGB);

        	if (frame.empty()) {
        	        printf("ERROR! blank frame grabbed\n");
        	}
                pthread_mutex_unlock(capmutex);

                //pthread_mutex_lock(buffmut);
                memcpy(buff, frame.data, frame.rows*frame.cols*3);
                //pthread_mutex_unlock(buffmut);
        }

}



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
	pthread_mutex_t capmutex;
	std::atomic_bool sholdclose=false;
	pthread_mutex_init(&capmutex, NULL);

	cap.open(camera);
	if (!cap.isOpened()) {
	        return -1;
	}
	// wait for a new frame from camera and store it into 'frame'

	cap.read(frame);

	cv::cvtColor(frame, frame,cv::COLOR_BGR2RGB);

	void* buff = malloc(frame.rows*frame.cols*3);
	pthread_mutex_t buffmut;
	pthread_mutex_init(&buffmut, NULL);

	// check if we succeeded
	if (frame.empty()) {
	        printf("ERROR! blank frame grabbed\n");
	}

	TickTexture2D videotexture = LoadTexture(frame.data, frame.cols, frame.rows, 3);


	std::thread(captureThread,buff,&buffmut ,&cap, &capmutex, &sholdclose).detach();

	while(!glfwWindowShouldClose(window) ){
		TickNewFrame();//you have to call this in evry frime; or you will have a bed time!
		//pthread_mutex_lock(&buffmut);
		ReloadTexture(&videotexture, buff, frame.cols, frame.rows, 3);
		//pthread_mutex_unlock(&buffmut);
		float frameHdem = (float)frame.cols/(float)frame.rows;
		float w = min((float)GetWindowW(),(float)GetWindowH()*frameHdem);
		float h = min(w/frameHdem,GetWindowW());
		DrawTexture(videotexture, (float)pos_or_zeor((GetWindowW()-w)/2.0),
	                                  (float)pos_or_zeor((GetWindowH()-h)/2.0),
					w,
					h
		);
		/******************** Render ********************/
		DrawCircle(GetWindowW() / 2,GetWindowH() - 40, 30, 360, {245,245,240,255});
		TickRendre(); /*render our Tick data*/

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clering up our screen
		tflf=glfwGetTime () ;
	}
	//sholdclose=true;
	free(buff);
	glfwTerminate();
}
