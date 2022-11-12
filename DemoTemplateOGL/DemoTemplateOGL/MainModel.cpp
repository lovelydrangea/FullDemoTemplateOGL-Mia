#include "MainModel.h"

MainModel::MainModel() {
    this->setHWND(NULL);
    this->gammaCorrection = false;
	cameraDetails = this;
}
MainModel::MainModel(HWND hWnd, string const& path, bool rotationX, bool rotationY, bool gamma) {
	cameraDetails = this;
	this->gammaCorrection = gamma;
    Model::loadModel(hWnd, path, rotationX, rotationY);
    this->defaultShader = false;
    this->buildKDtree();
	this->rotacionAngle = 0;
	// Creamos el cubo AABB apartir del arbol de puntos del modelo cargado
	vector<Vertex> cuboAABB = init_cube(this->kdTree.getRoot()->m_center.x, this->kdTree.getRoot()->m_center.y, this->kdTree.getRoot()->m_center.z, this->kdTree.getRoot()->m_halfWidth, this->kdTree.getRoot()->m_halfHeight, this->kdTree.getRoot()->m_halfDepth);
	vector<unsigned int> cuboIndex = getCubeIndex();
	this->AABB = new Model(hWnd, cuboAABB, cuboAABB.size(), cuboIndex, cuboIndex.size());
}
MainModel::MainModel(HWND hWnd, string const& path, glm::vec3 actualPosition, bool rotationX, bool rotationY, bool gamma) {
	cameraDetails = this;
	this->setPosition(actualPosition);
    Camera::updateCameraVectors();
    this->gammaCorrection = gamma;
    Model::loadModel(hWnd, path, rotationX, rotationY);
    this->defaultShader = false;
    this->buildKDtree();
	this->rotacionAngle = 0;
	// Creamos el cubo AABB apartir del arbol de puntos del modelo cargado
	vector<Vertex> cuboAABB = init_cube(this->kdTree.getRoot()->m_center.x, this->kdTree.getRoot()->m_center.y, this->kdTree.getRoot()->m_center.z, this->kdTree.getRoot()->m_halfWidth, this->kdTree.getRoot()->m_halfHeight, this->kdTree.getRoot()->m_halfDepth);
	vector<unsigned int> cuboIndex = getCubeIndex();
	this->AABB = new Model(hWnd, cuboAABB, cuboAABB.size(), cuboIndex, cuboIndex.size());
}
glm::mat4 MainModel::CamaraUpdate() {
	this->setTranslate(&this->getPosition());
//	this->setScale();
	return Camera::CamaraUpdate();
}
glm::mat4 MainModel::makeTransScaleNextPosition(const glm::mat4& prevTransformations) {
	glm::mat4 model;
	if (this->rotacionAngle != 0)
		model = prevTransformations * glm::rotate(makeTransNextPosition(), glm::radians(this->rotacionAngle), this->rotation);
	else
		model = prevTransformations * makeTransNextPosition();
	if (hasScale)
		model = glm::scale(model, scale);
	return model;
}
glm::mat4 MainModel::makeTransNextPosition() {
	glm::vec3 pos = this->getNextPosition();
	return  glm::translate(glm::mat4(1), pos);//glm::mat4(1) *glm::mat4(1)* glm::mat4(1);
}
bool MainModel::colisionaCon(Model* objeto) {
	std::pair<Node*, Node*> innerCollisionNodes = this->nodoColisionCon(objeto);
	if (innerCollisionNodes.first)
		return true;
	return false;
}
std::pair<Node*, Node*> MainModel::nodoColisionCon(Model* objeto) {
	return findCollision(this->kdTree.getRoot(), this->makeTransScaleNextPosition(glm::mat4(1)), objeto->kdTree.getRoot(), objeto->makeTransScale(glm::mat4(1)));
}
/*void MainModel::CamaraGiraY(float grados) {
		Camera::CamaraGiraY(grados);
		glm::vec3 main = getPosition();
		main = Transforma(getPosition(), grados, Ejes::EjeY);
		main.x = getPosition().x + 5;
		setTranslate(&main);
	}

glm::mat4 MainModel::GetViewMatrix() {
	return glm::lookAt(getPosition() + glm::vec3(sin(0) * 2, 0, cos(0) * 2), 
						   getPosition(),
						   getUp());
}

glm::mat4 MainModel::CamaraUpdate() {
	return glm::lookAt(getPosition() + glm::vec3(sin(0) * 2, 0, cos(0) * 2),
			getPosition() + getFront(),
			glm::vec3(0, 1, 0));
}

void MainModel::CamaraAvanza(float vel) {
	Camera::CamaraAvanza(vel);
	glm::vec3 main = getPosition();
	main.x = getPosition().x + 5;
	main.z = getPosition().z + 30;
	setTranslate(&main);
}
*/