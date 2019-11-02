#include "./SceneCRPC.h"

SceneCRPC::SceneCRPC():
	m_title("SceneCRPC"),
	m_active(false),
	m_shader(NULL),
	m_texture(NULL),
	m_player(NULL),
	m_useGravity(false),
	m_gravity(1.0f),
	m_speed(1.0f){
}

SceneCRPC::SceneCRPC(Shader* shader, Texture* texture,const float vertices[]):
	m_title("SceneCRPC"),
	m_active(false),
	m_useGravity(false),
	m_gravity(1.0f),
	m_speed(1.0f),
	m_shader(shader),
	m_texture(texture){
	
	m_objs.push_back(new Object(
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(1.0f,1.0f,1.0f)
	));
	
	m_objs.push_back(new Object(
		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(1.0f,1.0f,1.0f)
	));
	
	m_objs[0]->addVertices(vertices);
	m_objs[0]->createAABB(BBType::Circle);
	m_objs[0]->setAcceleration(1.0f);
	m_objs[0]->setRestitution(0);
	m_objs[0]->setMass(5.0f);
	
	m_objs[1]->addVertices(vertices);
	m_objs[1]->createAABB(BBType::Circle);
	m_objs[1]->setRestitution(0);
	m_objs[1]->setMass(5.0f);
	
	m_player = m_objs[0];
	
	
}

SceneCRPC::~SceneCRPC(){
	std::cout << "Segmentation fault may occur." << std::endl;
	for(int i = 0;i<m_objs.size();i++){
		delete m_objs[i];
	}
}

void SceneCRPC::render(Window* window){
	StaticRenderer::bind();
	
	m_shader->use();
	
	m_texture->bind();
	
	m_shader->setUniformMat4f("u_projection",window->getProjectionMatrix());
	m_shader->setUniformMat4f("u_view",glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0)));
	
	for(int i = 0;i<m_objs.size();i++){
		m_shader->setUniformMat4f("u_model",m_objs[i]->getModelMatrix());
		StaticRenderer::renderObject();
	}
	
	m_texture->unbind();
	
	StaticRenderer::unbind();
}

void SceneCRPC::update(Window* window){
	
	
	m_objs[0]->setVelocity(0.0f,0.0f);
	m_objs[1]->setVelocity(0.0f,0.0f);
	
	input(window);
	
	
	
	Collision obj01Col = AABB::getCollision(m_objs[0]->getBoundingBox(),m_objs[1]->getBoundingBox());
	
	
	
	if(obj01Col.colliding){
		Physics2D::resolveCollision(m_objs[0],m_objs[1],obj01Col);
		Physics2D::positionalCorrection(m_objs[0],m_objs[1],obj01Col);
	}
	

	
	//ignore these, placed to try and fix nan vector bug, didn't work
	//updatePos(m_objs[0]);
	//updatePos(m_objs[1]);
	
	if(m_useGravity){
		for(int i = 0;i<m_objs.size();i++){
			float newY = m_objs[i]->getPos().y - m_gravity;
			
			if(newY > -300.0f + m_objs[i]->getBoundingBox().getHalfExtents().y){
				m_objs[i]->setPos(m_objs[i]->getPos().x,newY,m_objs[i]->getPos().z);
			}else{
				m_objs[i]->setPos(m_objs[i]->getPos().x,-300.0f + m_objs[i]->getBoundingBox().getHalfExtents().y,m_objs[i]->getPos().z);
			}
		}
	}

}

void SceneCRPC::setActive(bool active){
	m_active = active;
}

void SceneCRPC::renderGUI(){
	ImGui::Begin(m_title.c_str());
	ImGui::SliderFloat("speed",&m_speed,0.01f,1000.0f);
	float multiplier = 1.0f;
	m_player->setAcceleration(m_speed*multiplier);
	ImGui::Checkbox("Use Gravity",&m_useGravity);
	ImGui::SliderFloat("Gravity",&m_gravity,0.1f,1.0f);
	ImGui::End();
}

void SceneCRPC::input(Window* window){
	glm::vec3 currentPos = m_player->getPos();
	float dt = ImGui::GetIO().DeltaTime;

	float vel = m_player->getAcceleration()*dt;

	m_player->setVelocity(vel,vel);
	float pos = vel;

	
	if(glfwGetKey(window->getWindow(),GLFW_KEY_W)){
		currentPos.y += pos;
	}else if (glfwGetKey(window->getWindow(),GLFW_KEY_S)){
		currentPos.y -= pos;
	}
	
	if(glfwGetKey(window->getWindow(),GLFW_KEY_D)){
		currentPos.x += pos;
	}else if(glfwGetKey(window->getWindow(),GLFW_KEY_A)){
		currentPos.x -= pos;
	}
	
	m_player->setPos(currentPos.x,currentPos.y,currentPos.z);
}

std::string SceneCRPC::getSceneTitle() const {
	return m_title;
}

bool SceneCRPC::isActive() const {
	return m_active;
}

void SceneCRPC::posCheck(){
	std::cout << m_player->getPos().x << " " << m_player->getPos().y << " " << m_player->getPos().z << std::endl;
	std::cout << m_player->getRot().x << " " << m_player->getRot().y << " " << m_player->getRot().z << std::endl;
	std::cout << m_player->getScl().x << " " << m_player->getScl().y << " " << m_player->getScl().z << std::endl;
}

void SceneCRPC::updatePos(Object* obj){
	glm::vec2 resultingPosition = glm::vec2(obj->getPos().x,obj->getPos().y) + obj->getVelocity();
	obj->setPos(resultingPosition.x,resultingPosition.y,0.0f);
}