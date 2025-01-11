
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h> 
#include <chrono>
#include <thread>


//nekima mozda ne radi primjerASSIMP zbog ponovnih definicija stbi funkcija.
//Jedno od mogucih rjesenja je da se zakomentira linija #define STB_IMAGE_IMPLEMENTATION.
//#define STB_IMAGE_IMPLEMENTATION
//#include "../include/stb/stb_image.h"

#include "../include/headers/shader.h"
#include "../include/headers/renderer.h"
#include "../include/headers/camera.h"
#include "../include/headers/bspline.h"


// Standard Headers
#include <iostream>
#include <cstdlib>

struct TangentVector{
    unsigned int vao;
    unsigned int vbo;
    std::pair<glm::vec3,glm::vec3> vecs;

    TangentVector(glm::vec3 beg, glm::vec3 orientation){
        vecs.first = beg;
        vecs.second = beg + orientation;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);

        glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3),&vecs ,GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
 
        glBindVertexArray(0);
    }
    
    void update(glm::vec3 beg, glm::vec3 orientation){
        vecs.first = beg;
        vecs.second = beg + orientation;

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER,vbo);

        glBufferSubData(GL_ARRAY_BUFFER,0, 2 * sizeof(glm::vec3),&vecs);
        glBindVertexArray(0);

    }
    void draw(){
        glBindVertexArray(this->vao);
        glDrawArrays(GL_LINES,0,2);
        glBindVertexArray(0);
    }

    
};


int width = 1280;
int height = 800;
float fov = 45.f;

void processInput(GLFWwindow *window, Camera& c);
void framebuffer_size_callback(GLFWwindow* window, int w, int h){
	width = w;
	height = h;
	glViewport(0,0,width,height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	fov -= (float)yoffset;
	if(fov < 1.0f)
		fov = 1.0f;
	if(fov > 45.f)
		fov = 45.f;
}

int main(int argc, char * argv[]) {
	GLFWwindow* window;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "title", nullptr, nullptr);
	if(window == nullptr){
		std::cout << "failed to create opengl context" << std::endl;
		exit(1);
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "failed to init glad" << std::endl;
		return -1;
	}
	glClearColor(0.1, 0.1, 0.1, 1);
    glLineWidth(2.0f);
	glEnable(GL_DEPTH_TEST);
    
    std::vector<Shader> shaders;
	Shader shader1("shaders/myshader.vs", "shaders/myshader.fs");
//	Shader shader2("shaders/myshader2.vs", "shaders/myshader2.fs", "shaders/myshader2.gs");
	Shader shader3("shaders/bezier_shader.vs", "shaders/bezier_shader.fs");
	//Shader shader3("shaders/dodatni.vs", "shaders/dodatni.fs", "shaders/dodatni.gs");

   	Camera camera(glm::vec3(-20.f,20.f,75.f), -90.f, 0.f);
    camera.updateRotation(-20.f, 55.f);
	//kocka 1
	Object o("../resources/other/frog.obj");
	o.scale(glm::vec3(4.f));	

//	Object o2("../resources/other/frog.obj");

//	o2.scale(glm::vec3(0.25f));
//	o2.globalMove(glm::vec3(0.8f,0.8f,0.f));
    
    BSpline bspline;

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(60.f),
		       	        	       glm::vec3(1.0f,0.0f,0.0f));
//	o2.rotate(rotationMatrix);
	Renderer r;

    r.addBSpline(&bspline);
	r.addObject(&o);
//	r.addObject(&o2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    std::vector<glm::vec3> control_points;
    Bezier b;
    bool key_pressed = false, animate = false;
    int v_index = 0;
    glm::vec3 oldPos;
    TangentVector tv(glm::vec3(0.f),glm::vec3(0.f));

    //GLuint ssbo;
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
        
        if(!key_pressed && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
            control_points.push_back(camera.getPosition() + camera.getFront() * 3.f);
            std::cout << control_points.back().x << " " << control_points.back().y << " " << control_points.back().z << std::endl;
            b.setupBezier(control_points);
            key_pressed = true;
            r.addCurve(b);
        }
        else if(glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE){
            key_pressed = false;
        }
        


		processInput(window, camera);
		camera.updateZoom(fov);
        
        shader1.use();
		shader1.setMat4("view", camera.getViewMatrix());
		shader1.setMat4("projection", camera.getProjectionMatrix());
        /*shader2.use();
        shader2.setMat4("view", camera.getViewMatrix());
		shader2.setMat4("projection", camera.getProjectionMatrix());*/
        
        shader3.use();
        shader3.setMat4("view", camera.getViewMatrix());
		shader3.setMat4("projection", camera.getProjectionMatrix());

        //animacija
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            o.setPosition(bspline.getVertices()[v_index]);
            animate = true;
        }
        if(animate && v_index<bspline.getVertices().size()-1){
            glm::vec3 front = (bspline.getVertices()[v_index+1] - bspline.getVertices()[v_index]);
            glm::vec3 temp_up = glm::cross(glm::vec3(0,1,0), front); 
            glm::vec3 right = glm::cross(front, temp_up);
            glm::vec3 up = glm::cross(front, right);

            o.globalMove(front);
            o.setOrientation(up,right,-front);
            v_index++;
            
            shader3.setVec3("color",glm::vec3(1,0,0));
            tv.update(o.getPosition(), front * 100.f);
            tv.draw();

            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        else if(v_index>=bspline.getVertices().size()-1){
            v_index = 0;
            animate = false;
        }

        std::vector<Shader> s{shader1,shader3};
		r.renderScene(s);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window, Camera& c)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        c.updatePosition(c.getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        c.updatePosition(-c.getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        c.updatePosition(-glm::normalize(glm::cross(c.getFront(), c.getUp())));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        c.updatePosition(glm::normalize(glm::cross(c.getFront(), c.getUp())));

    if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	    c.updateRotation(2.f, 0.f);
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	    c.updateRotation(-2.f ,0.f);
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	    c.updateRotation(0.f, 2.f);
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	    c.updateRotation(0.f ,-2.f);

}
