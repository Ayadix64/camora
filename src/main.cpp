#include "conf.hpp"
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <stdio.h>
#include <string>
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

#define min(x,y) x>y?y:x
#define max(x,y) x>y?x:y
#define pos_or_zeor(x) x<0?0:x
#define cube(x) (x)*(x) // yeah..
#define CONFIG_FILE "camora.conf"

struct {
        u32 defCamera = 0;
        float dim = 1.0;
        float animationSpeed = 1.0;
} parameters;


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

                pthread_mutex_lock(buffmut);
                memcpy(buff, frame.data, frame.rows*frame.cols*3);
                pthread_mutex_unlock(buffmut);
        }
        return;
}

bool snapButton(void)
{

        static unsigned int raduis = 30;
        static bool isHover = false;
        static double snaptimer = 0.0;
        const float animationduration = 0.1*(1.0/parameters.animationSpeed);
        bool isClicked = false;
        float x  = GetWindowW()/2.0, y = GetWindowH() -40;

        if( (cube(GetMousePos().x - x) +  cube(GetMousePos().y -y)) < 30* 30){
                if(!isHover)snaptimer = glfwGetTime();
                isHover=true;
                isClicked= GetMouseClickes()&1;
        }else if(isHover){
                snaptimer = glfwGetTime() + animationduration;
                isHover = false;
        }
        if(!isHover && glfwGetTime() >   snaptimer){

                snaptimer = glfwGetTime();
        }
        if(abs(glfwGetTime() - snaptimer) <= animationduration && glfwGetTime() != snaptimer  )
        {
                raduis = 30+abs(glfwGetTime() - snaptimer)*10.0 * (1.0/animationduration);
        }
        if(isClicked){
                snaptimer = glfwGetTime() + animationduration;
                printf("clicked\n");
        }

        DrawCircle(x,y, raduis, 360, {245,245,240,255});
        DrawEmptyCircle(x,  y, 40,5, 360, {245,245,240,255});
        return isClicked;
}

void Menu(void){
        static bool showMenu = false;
        const  double animationDur = 0.2f* (1.0f/parameters.animationSpeed);
        static double animationEnd = 0.0;
        static int goUp= 0.0; //up ? down?
        static float y = GetWindowH();

        if(showMenu || animationEnd > glfwGetTime() ){
                DrawRoundedRectangel(0 ,(float)GetWindowH()- y*(4*(float)GetWindowH()/5), GetWindowW(), GetWindowH() , 30, 36, {25,25,30,255});
        }
        if(glfwGetTime() >= animationEnd && animationEnd > 0.0){
                animationEnd=0.0;
        }else if(animationEnd>0.0){
                y = (1.0f/animationDur)*(goUp?animationDur-(animationEnd-glfwGetTime()) : (animationEnd-glfwGetTime()));
        }

        if(Button("Menu", 20 ,GetWindowH() - 50, 0, 0)&1){
               showMenu = !showMenu;
               goUp= showMenu?1.0:0.0;
               animationEnd = glfwGetTime() + animationDur;
        }
        //animation , privded by trust me bro fondation
}


void getParameters(void)
{
        std::string conf;
        if(readConf(CONFIG_FILE, "camera", conf) ){
                parameters.defCamera = atoi(conf.c_str());
        }
        if(readConf(CONFIG_FILE, "dim", conf) ){
                parameters.dim = atof(conf.c_str());
        }
        if(readConf(CONFIG_FILE, "animation", conf) ){
                parameters.animationSpeed = atof(conf.c_str());
        }

}
void SetParameters(void)
{

        std::string conf;
        writeConf(CONFIG_FILE, "camera", std::to_string(parameters.defCamera));

        writeConf(CONFIG_FILE, "dim", std::to_string(parameters.dim));

        writeConf(CONFIG_FILE, "animation", std::to_string(parameters.animationSpeed));


}

int main(){
        if(!glfwInit()){
		return 1;
	}

	TickInitWindowFlags();
	GLFWwindow* window = glfwCreateWindow(800, 600, "camora", NULL, NULL);
	glfwMakeContextCurrent(window);//seting our widow context
	gladLoadGL();//Init our opengl her, this will get all the function adrress of opengl to used;


	TickInit(window); //And the
	getParameters();

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


	std::thread capthr= std::thread(captureThread,buff,&buffmut ,&cap, &capmutex, &sholdclose);

	while(!glfwWindowShouldClose(window) ){
		TickNewFrame();//you have to call this in evry frime; or you will have a bed time!
		pthread_mutex_lock(&buffmut);
		ReloadTexture(&videotexture, buff, frame.cols, frame.rows, 3);
		pthread_mutex_unlock(&buffmut);
		float frameHdem = (float)frame.cols/(float)frame.rows;
		float w = min((float)GetWindowW(),(float)GetWindowH()*frameHdem);
		float h = min(w/frameHdem,GetWindowW());
		DrawTexture(videotexture, (float)pos_or_zeor((GetWindowW()-w)/2.0),
	                                  (float)pos_or_zeor((GetWindowH()-h)/2.0),
					w,
					h
		);
		snapButton();
		Menu();

		TickRendre();
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clering up our screen
		tflf=glfwGetTime () ;
	}
	sholdclose=true;
	if(capthr.joinable()){
	        capthr.join();
	}
	free(buff);
	glfwTerminate();
	SetParameters();
	return 0;
}
