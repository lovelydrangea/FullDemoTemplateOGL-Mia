#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#include <windows.h>
#include <windowsx.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "camera.h"
#include <iostream>
#include "Model.h"
#include "MainModel.h"
#include "SkyDome.h"
#include "Terreno.h"
#include "Billboard.h"
#include "CollitionDetection.h"

class Scenario {
public:
	SkyDome* sky;
	Terreno* terreno;
	std::vector<Billboard *> billBoard;
	std::vector<Model *> ourModel;
	MainModel* camara;
	HWND hwnd;
	float angulo;

	Scenario(HWND hWnd) {
		glm::vec3 translate;
		glm::vec3 scale;
		MainModel* model = new MainModel(hWnd, "models/Cube.obj");
		translate = glm::vec3(0.0f, 0.0f, 3.0f);
		scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		InitGraph(hWnd, model);
	}
	Scenario(HWND hWnd, MainModel *camIni) {
		InitGraph(hWnd, camIni);
	}
	void InitGraph(HWND hWnd, MainModel *main) {
		hwnd = hWnd;
		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;
		camara = main;
		//creamos el objeto skydome
		sky = new SkyDome(hWnd, 32, 32, 20, (WCHAR*)L"skydome/earth.jpg", main);
		//creamos el terreno
		terreno = new Terreno(hWnd, (WCHAR*)L"skydome/terreno.jpg", (WCHAR*)L"skydome/texterr2.jpg", 400, 400, main);
		// load models
		// -----------
		ourModel.push_back(main);
		Model* model;
		glm::vec3 translate;
		glm::vec3 scale;
		model = new Model(this->hwnd, "models/fogata.obj", main);
		translate = glm::vec3(0.0f, 10.0f, 25.0f);
		model->setTranslate(&translate);
		ourModel.push_back(model);
		model= new Model(this->hwnd, "models/pez.obj", main);
		translate = glm::vec3(0.0f, 7.0f, 50.0f);
		model->setTranslate(&translate);
		ourModel.push_back(model);
//		model = new Model(this->hwnd, "models/IronMan.obj", main);
//		translate = glm::vec3(0.0f, 20.0f, 30.0f);
//		scale = glm::vec3(0.025f, 0.025f, 0.025f);	// it's a bit too big for our scene, so scale it down
//		model->setScale(&scale);
//		model->setTranslate(&translate);
//		ourModel.push_back(model);
		model = new Model(this->hwnd, "models/backpack.obj", main, false, false);
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
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol.png", 6, 6, 0, ye - 1, 0, camara));

		ye = terreno->Superficie(5, -5);
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol2.png", 6, 6, 5, ye - 1, -5, camara));

		ye = terreno->Superficie(-9, -15);
		billBoard.push_back(new Billboard(hWnd, (WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye - 1, -15, camara));
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
		camara->getPosition().y = terreno->Superficie(camara->getPosition().x, camara->getPosition().z) + 1.7;
		// Actualizamos la camara
		camara->CamaraUpdate();

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
				innerCollisionNodes = findCollision(camara->kdTree.getRoot(), camara->makeTransScale(glm::mat4(1)), ourModel[i]->kdTree.getRoot(), ourModel[i]->makeTransScale(glm::mat4(1)));
				if (innerCollisionNodes.first) {
					return true;
				}
			}
		}
		return false;
	}

	~Scenario() {
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
				if (ourModel[i] != camara)
					delete ourModel[i];
		this->ourModel.clear();
	}
};

#endif