#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./Dependencies/glm/glm.hpp"
#include "./Dependencies/glm/gtc/matrix_transform.hpp"
#include "./Dependencies/glm/gtc/type_ptr.hpp"

#include "./Dependencies/imgui/imgui.h"
#include "./Dependencies/imgui/imgui_impl_glfw.h"
#include "./Dependencies/imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObj.h"
#include "VertexBufferObj.h"
#include "IndexBufferObj.h"
#include "Object.h"
#include "Renderer.h"
#include "StaticRenderer.h"
#include "GUIControlPanel.h"
#include "AABB.h"
#include "Physics2D.h"

#include "SceneExample.h"
#include "SceneCRPC.h"
#include "SceneIntegrator.h"
#include "SceneForces.h"
#include "SceneParticle.h"
#include "SceneForceGenerator.h"
#include "SceneBuoyantForce.h"
#include "SceneCollisions.h"

void input(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void updatePos(Object* obj);
void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam);

const int WIDTH = 800;
const int HEIGHT = 600;
int zoom = 1.0f;
bool shouldShowDemoWindow = true;

Window windowObj(WIDTH, HEIGHT,"Hello,World");

int main(){
	
	if(windowObj.getWindow() == NULL){
		return EXIT_FAILURE;
	}
	windowObj.setFramebufferSizeCallback(framebuffer_size_callback);
	windowObj.setClearColor(glm::vec4(0.9f,0.7f,0.7f,1.0f));

	if(glewInit() != GLEW_OK){
		std::cout << "Error glewInit" <<std::endl;
		return EXIT_FAILURE	;
	}
	
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback,0);
	
	GUIControlPanel::init(windowObj.getWindow(),true);	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	Shader shader0("./shaders/shader0.vs","./shaders/shader0.fs");	
	Shader shaderPoint("./shaders/shaderPoint.vs","./shaders/shaderPoint.fs");
	
	Texture texture0("./textures/circle.png");
		
	StaticRenderer::init();
	
	//SceneCRPC scene0(&shader0,&texture0,StaticRenderer::getVertices());
	//SceneExample scene1(&shader0,&texture0,StaticRenderer::getVertices());
	SceneIntegrator scene2(&shader0,&texture0,StaticRenderer::getVertices());
	SceneForces scene3(&shader0, &texture0, StaticRenderer::getVertices());
	SceneParticle scene4(&shaderPoint);
	SceneForceGenerator scene5(&shader0, &texture0, StaticRenderer::getVertices());
	SceneBuoyantForce scene6(&shader0, &texture0 , StaticRenderer::getVertices());
	SceneCollisions scene7(&shader0, &texture0, StaticRenderer::getVertices());

	//GUIControlPanel::registerScene(&scene1);
	//GUIControlPanel::registerScene(&scene0);
	GUIControlPanel::registerScene(&scene2);
	GUIControlPanel::registerScene(&scene3);
	GUIControlPanel::registerScene(&scene4);
	GUIControlPanel::registerScene(&scene5);
	GUIControlPanel::registerScene(&scene6);
	GUIControlPanel::registerScene(&scene7);

	std::cout << "Retrieved Error Code: " << glGetError() << std::endl;
	
	while(!windowObj.windowShouldClose()){
		windowObj.pollEvents();
		input(windowObj.getWindow());
	
		GUIControlPanel::updateCurrentScene(&windowObj);
		GUIControlPanel::start();
		GUIControlPanel::renderMenu();
	
		windowObj.displayWindowStats();

		GUIControlPanel::renderCurrentSceneGUI();
		
		windowObj.clearColor();
		glClear(GL_COLOR_BUFFER_BIT);	

		GUIControlPanel::renderCurrentScene(&windowObj);
		GUIControlPanel::finalize();

		windowObj.swapBuffers();
	}
	
	StaticRenderer::destroy();
	std::cout << "Retrieved Error Code: " << glGetError() << std::endl;
		
	return 0;
	
}	

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0,width,height);
	windowObj.resize(width,height);
}

void input(GLFWwindow* window){
	if(glfwGetKey(window,GLFW_KEY_ESCAPE)){
		windowObj.closeWindow();
	}
}

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam){
	
	 fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

