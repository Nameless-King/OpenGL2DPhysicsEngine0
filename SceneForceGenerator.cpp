#include "./SceneForceGenerator.h"

SceneForceGenerator::SceneForceGenerator():
	m_title("SceneForceGenerator"),
	m_active(false),
	m_springConstant(2.0f),
	m_useGravity(false),
	m_shader(NULL),
	m_texture(NULL),
	m_staticObj(NULL),
	m_hangingObj(NULL),
	m_forceSpring(ForceSpring()),
	m_forceGravity(ForceGravity(glm::vec2(0.0f,-10.0f))),
	m_forceDrag(ForceDrag(1.0f,1.0f)),
	m_forceBungee(ForceBungee()){}
	
SceneForceGenerator::SceneForceGenerator(Shader* shader, Texture* texture, const float vertices[]):
	m_title("SceneForceGenerator"),
	m_active(false),
	m_springConstant(2.0f),
	m_useGravity(false),
	m_shader(shader),
	m_texture(texture),
	m_forceGravity(ForceGravity(glm::vec2(0.0f,-100.0f))){
			
	m_staticObj = new Object(
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(1.0f,1.0f,1.0f)
	);
	
	m_staticObj->addVertices(vertices);
	m_staticObj->createAABB(BBType::Circle);
	
	RigidBody2D* rbStatic = new RigidBody2D(5.0f);
	rbStatic->setDamping(1.0f);
	
	m_staticObj->addRigidBody2D(rbStatic);
	
	m_hangingObj = new Object(
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(1.0f,1.0f,1.0f)
	);
	
	m_hangingObj->addVertices(vertices);
	m_hangingObj->createAABB(BBType::Circle);
	
	RigidBody2D* rbHanging = new RigidBody2D(5.0f);
	rbHanging->setDamping(0.5f);
	
	m_hangingObj->addRigidBody2D(rbHanging);

	float kConstant = 0.1f;
	float equilibrium = 2.0f;
	m_forceSpring = ForceSpring(m_staticObj,kConstant,equilibrium);

	float k1 = 0.01f;
	float k2 = 0.0001f;
	m_forceDrag = ForceDrag(k1,k2);

	float bkc = 0.1f;
	float be = 2.0f;
	m_forceBungee = ForceBungee(m_staticObj,bkc,be);
};

SceneForceGenerator::~SceneForceGenerator(){
	delete m_staticObj;
	delete m_hangingObj;
}

std::string SceneForceGenerator::getSceneTitle() const{
	return m_title;
}

bool SceneForceGenerator::isActive() const{
	return m_active;
}

void SceneForceGenerator::render(Window* window){
	StaticRenderer::bind();
	
	m_shader->use();
	m_texture->bind();
	
	m_shader->setUniformMat4f("u_projection",window->getProjectionMatrix());
	m_shader->setUniformMat4f("u_view",glm::mat4(1.0f));
	
	m_shader->setUniformMat4f("u_model",m_staticObj->getModelMatrix());
	StaticRenderer::renderObject();
	
	m_shader->setUniformMat4f("u_model",m_hangingObj->getModelMatrix());
	StaticRenderer::renderObject();
	
	m_texture->unbind();
	
	StaticRenderer::unbind();
}

void SceneForceGenerator::update(Window* window){
	input(window);
	
	m_forceGravity.updateForce(m_hangingObj,ImGui::GetIO().DeltaTime);
	//m_forceSpring.updateForce(m_hangingObj,ImGui::GetIO().DeltaTime);
	m_forceBungee.updateForce(m_hangingObj,ImGui::GetIO().DeltaTime);
	m_forceDrag.updateForce(m_hangingObj,ImGui::GetIO().DeltaTime);

	Physics2D::integrator3(m_hangingObj,ImGui::GetIO().DeltaTime);
}

void SceneForceGenerator::setActive(bool active){
	
}

void SceneForceGenerator::renderGUI(){
	float gravity = m_forceGravity.getGravity().y;
	float k1 = m_forceDrag.getK1();
	float k2 = m_forceDrag.getK2();
	float equilibrium = m_forceSpring.getEquilibrium();
	float springConstant = m_forceSpring.getSpringConstant();
	float bungeeEquilibrium = m_forceBungee.getEquilibrium();
	float bungeeSpringConstant = m_forceBungee.getSpringConstant();

	ImGui::Begin(m_title.c_str());
	if(ImGui::DragFloat("Gravity", &gravity, 0.1f)){
		m_forceGravity.setGravity(0.0f,gravity);
	}
	if(ImGui::DragFloat("Drag constant 1 (k1)",&k1,0.001f)){
		m_forceDrag.setK1(k1);
	}
	if(ImGui::DragFloat("Drag constant 2 (k2)",&k2,0.001f)){
		m_forceDrag.setK2(k2);
	}
	if(ImGui::DragFloat("Equilibrium",&equilibrium,0.1f)){
		m_forceSpring.setEquilibrium(equilibrium);
	}
	if(ImGui::DragFloat("Spring Constant (k)",&springConstant,0.01f)){
		m_forceSpring.setSpringConstant(springConstant);
	}
	if(ImGui::DragFloat("Bungee Equilibrium",&bungeeEquilibrium,0.1f)){
		m_forceBungee.setEquilibrium(bungeeEquilibrium);
	}
	if(ImGui::DragFloat("Bungee Spring Constant",&bungeeSpringConstant,0.01f)){
		m_forceBungee.setSpringConstant(bungeeSpringConstant);
	}
	ImGui::End();
}
	
void SceneForceGenerator::input(Window* window){
	float px = m_staticObj->getPos().x;
	float py = m_staticObj->getPos().y;
	float speed = 5.0f;

	if(glfwGetKey(window->getWindow(),GLFW_KEY_W)){
		py += speed;
	}else if(glfwGetKey(window->getWindow(),GLFW_KEY_S)){
		py -= speed;
	}

	if(glfwGetKey(window->getWindow(),GLFW_KEY_D)){
		px += speed;
	}else if(glfwGetKey(window->getWindow(),GLFW_KEY_A)){
		px -= speed;
	}

	m_staticObj->setPos(px,py);

}