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
}
MainModel::MainModel(HWND hWnd, string const& path, glm::vec3 actualPosition, bool rotationX, bool rotationY, bool gamma) {
	cameraDetails = this;
	this->setPosition(actualPosition);
    Camera::updateCameraVectors();
    this->gammaCorrection = gamma;
    Model::loadModel(hWnd, path, rotationX, rotationY);
    this->defaultShader = false;
    this->buildKDtree();
}
glm::mat4 MainModel::CamaraUpdate() {
	this->setTranslate(&this->getPosition());
//	this->setScale();
	return Camera::CamaraUpdate();
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