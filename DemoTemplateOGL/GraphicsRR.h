#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#include <windows.h>
#include <windowsx.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "DemoTemplateOGL.h"
#include "camera.h"
#include <iostream>
#include "model.h"
#include "SkyDome.h"
#include "Terreno.h"
#include "Billboard.h"
#include "CollitionDetection.h"

class GraphRR : public Camera {
public:
	SkyDome* sky;
	Terreno* terreno;
	std::vector<Billboard *> billBoard;
	std::vector<Model *> ourModel;
	Model* camara;
	bool frontCollition;
	bool backCollition;

	float angulo;

	GraphRR(HWND hWnd) {
		InitGraph(hWnd, glm::vec3(0.0f, 0.0f, 3.0f));
	}
	GraphRR(HWND hWnd, glm::vec3 camIni) {
		InitGraph(hWnd, camIni);
	}
	void InitGraph(HWND hWnd, glm::vec3 camIni) {
		setHWND(hWnd);
		setPosition(camIni);
		//Camera(this->Position);
		glm::vec3 v(0, 0, -1);
		setFront(v);
		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;

		//creamos el objeto skydome
		sky = new SkyDome(hWnd, 32, 32, 20, (WCHAR*)L"skydome/earth.jpg");
		//creamos el terreno
		terreno = new Terreno(hWnd, (WCHAR*)L"skydome/terreno.jpg", (WCHAR*)L"skydome/texterr2.jpg", 400, 400);
		// load models
		// -----------
		Model* model;
		glm::vec3 translate;
		glm::vec3 scale;

		model = new Model(this->getHWND(), "models/Cube.obj");
		translate = camIni;// glm::vec3(0.0f, 10.0f, 20.0f);
		scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		camara = model;
		ourModel.push_back(model);
		model = new Model(this->getHWND(), "models/fogata.obj");
		translate = glm::vec3(0.0f, 10.0f, 25.0f);
		model->setTranslate(&translate);
		ourModel.push_back(model);
		model= new Model(this->getHWND(), "models/pez.obj");
		translate = glm::vec3(0.0f, 7.0f, 50.0f);
		model->setTranslate(&translate);
		ourModel.push_back(model);
//		model = new Model(this->getHWND(), "models/IronMan.obj");
//		translate = glm::vec3(0.0f, 10.0f, 20.0f);
//		scale = glm::vec3(0.025f, 0.025f, 0.025f);	// it's a bit too big for our scene, so scale it down
//		model->setScale(&scale);
//		model->setTranslate(&translate);
//		ourModel.push_back(model);
		model = new Model(this->getHWND(), "models/backpack.obj", false, false);
		translate = glm::vec3(20.0f, 14.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		ourModel.push_back(model);
		inicializaBillboards(hWnd);
	}

	void inicializaBillboards(HWND hWnd)
	{
		float ye = terreno->Superficie(0, 0);
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol.png", 6, 6, 0, ye - 1, 0));

		ye = terreno->Superficie(5, -5);
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol2.png", 6, 6, 5, ye - 1, -5));

		ye = terreno->Superficie(-9, -15);
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye - 1, -15));
	}

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	void Render(HDC hDC)
	{
		//borramos el biffer de color y el z para el control de profundidad a la 
		//hora del render a nivel pixel.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//		glClearColor(255.0f, 255.0f, 255.0f, 255.0f);
		// Posicionamos la camara pixeles arriba de su posicion en el terreno
		getPosition().y = terreno->Superficie(getPosition().x, getPosition().z) + 1.7;
		// Actualizamos la camara
		CamaraUpdate();
		// Obtenemos la proyeccion en base a la ventana
		glm::mat4 projection = glm::perspective(glm::radians(getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		// Obtenemos la vista
		glm::mat4 view = GetViewMatrix();
		// Actualizamos la estructura global con los parametros de proyeccion y vista
		cameraDetails.currentProjection = projection;
		cameraDetails.currentView = view;

		// Decimos que dibuje la media esfera
		sky->Draw();
		// Ahora el terreno
		terreno->Draw();

		// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
		for (int i = 0; i < ourModel.size(); i++) {
			ourModel[i]->Draw();
		}

		// Dibujamos cada billboard que este cargado en el arreglo de billboards.
		for (int i = 0; i < billBoard.size(); i++)
			billBoard[i]->Draw();
		// Le decimos a winapi que haga el update en la ventana
		SwapBuffers(hDC);
	}

	bool lookForCollition() {
		std::pair<Node*, Node*> innerCollisionNodes;
		for (int i = 0; i < ourModel.size(); i++) {
			if (ourModel[i] != camara) {
				innerCollisionNodes = findCollision(camara, ourModel[i]);
				if (innerCollisionNodes.first) {
					return true;
				}
			}
		}
		return false;
	}

	void CamaraGiraY(float grados) {
		Camera::CamaraGiraY(grados);
		glm::vec3 main = getPosition();
		main = Transforma(getPosition(), grados, Ejes::EjeY);
		main.x = getPosition().x + 5;
//		main.y = terreno->Superficie(main.x, main.z) + 0.6;
//		camara->setTranslate(&main);
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(getPosition() + glm::vec3(sin(0) * 2, 0, cos(0) * 2), 
						   getPosition(),
						   getUp());
	}

	glm::mat4 CamaraUpdate() {
		return glm::lookAt(getPosition() + glm::vec3(sin(0) * 2, 0, cos(0) * 2),
			getPosition(),// + getFront(),
			glm::vec3(0, 1, 0));
	}

	void CamaraAvanza(float vel) {
		Camera::CamaraAvanza(vel);
		glm::vec3 main = getPosition();
		main.x = getPosition().x + 5;
//		main.z = getPosition().z + 30;
//		main.y = terreno->Superficie(main.x, main.z) + 0.6;
		camara->setTranslate(&main);
	}

	~GraphRR() {
		if (this->sky != NULL) {
			delete this->sky;
			this->sky = NULL;
		}
		if (this->terreno != NULL) {
			delete this->terreno;
			this->terreno = NULL;
		}
		if (billBoard.size() > 0)
			for (int i = 0; i < billBoard.size(); i++)
				delete billBoard[i];
		this->billBoard.clear();
		if (ourModel.size() > 0)
			for (int i = 0; i < ourModel.size(); i++)
				delete ourModel[i];
		this->ourModel.clear();
	}
};

#endif