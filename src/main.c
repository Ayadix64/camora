#include <stdio.h>


#ifndef GLAD_GL_IMPLEMENTATION
	#include "../glad/glad.h"
	#define GLAD_GL_IMPLEMENTATION
#endif

#include <GLFW/glfw3.h>
#include "../ticktackto/include/tick-tack-to.h"

int main(){
        if(!glfwInit()){
		return 1;
	}

	TickInitWindowFlags();
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
	glfwMakeContextCurrent(window);//seting our widow context
	gladLoadGL();//Init our opengl her, this will get all the function adrress of opengl to used;


	TickInit(window); //And the


	char FPS[100];

	char check=0;
	float slid=0.0;
	double tflf = glfwGetTime();



	while(!glfwWindowShouldClose(window) ){
		TickNewFrame();//you have to call this in evry frime; or you will have a bed time!


		DrawText("Hello World!, World Hello! , whozl[fkgopjg]", 300, 20,(Vec4c){255,255,255,255});
		CheckBox("Check Box Bora Broa Broa .", 20, 300, (char*)&check);


		/******************** Render ********************/
		TickRendre(); /*render our Tick data*/
		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clering up our screen
		tflf=glfwGetTime () ;
	}

	glfwTerminate();


}
