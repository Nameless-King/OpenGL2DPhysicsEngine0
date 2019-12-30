#include "./ScenePhysicsSystem.h"

ScenePhysicsSystem::ScenePhysicsSystem():
	m_title("ScenePhysicsSystem"),
	m_shader(NULL),
	m_texture(NULL){
}

ScenePhysicsSystem::ScenePhysicsSystem(Shader* shader, Texture* texture, const float vertices[]):
	m_title("ScenePhysicsSystem"),
	m_shader(shader),
	m_texture(texture){
}

ScenePhysicsSystem::~ScenePhysicsSystem(){

}

void ScenePhysicsSystem::render(Window* window){
	StaticRenderer::bind();
	m_shader->use();
	m_texture->bind();
	m_shader->setUniformMat4f("u_projection",window->getProjectionMatrix());
	m_shader->setUniformMat4f("u_view",window->getCameraController()->getViewMatrix());
	
	
	m_texture->unbind();
	StaticRenderer::unbind();
}

void ScenePhysicsSystem::update(Window* window){

	
	input(window);
}

void ScenePhysicsSystem::renderGUI(){
	ImGui::Begin(m_title.c_str());
	ImGui::End();
}

void ScenePhysicsSystem::input(Window* window){
	
	
	if(glfwGetKey(window->getWindow(),GLFW_KEY_W)){
		
	}else if(glfwGetKey(window->getWindow(),GLFW_KEY_S)){
		
	}
	
	if(glfwGetKey(window->getWindow(),GLFW_KEY_D)){
		
	}else if(glfwGetKey(window->getWindow(),GLFW_KEY_A)){
		
	}
	
	
}

std::string ScenePhysicsSystem::getSceneTitle() const {
	return m_title;
}