#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Base/camera.h"
#include <iostream>
#include "Base/Scene.h"

class Scenario : public Scene {
private:
	SkyDome* sky;
	Terreno* terreno;
	std::vector<Billboard*> billBoard;
	std::vector<Model*> ourModel;
	Model* camara;
	Water* water;
	float angulo;
	int animacion = 0;
	int frameArbol = 1;
public:
	Scenario(Camera *cam) {
		glm::vec3 translate;
		glm::vec3 scale;
		Model* model = new Model("models/Cube.obj", cam);
		translate = glm::vec3(0.0f, 0.0f, 3.0f);
		scale = glm::vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		InitGraph(model);
	}
	Scenario(Model *camIni) {
		InitGraph(camIni);
	}
	void InitGraph(Model *main) {
		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;
		camara = main;
		//creamos el objeto skydome
		sky = new SkyDome(32, 32, 20, (WCHAR*)L"skydome/earth.jpg", main->cameraDetails);
		//creamos el terreno
		terreno = new Terreno((WCHAR*)L"skydome/terreno.jpg", (WCHAR*)L"skydome/texterr2.jpg", 400, 400, main->cameraDetails);
		water = new Water((WCHAR*)L"textures/terreno.bmp", (WCHAR*)L"textures/water.bmp", 20, 20, camara->cameraDetails);
		glm::vec3 translate;
		glm::vec3 scale;
		glm::vec3 rotation;
		translate = glm::vec3(0.0f, 20.0f, 30.0f);
		water->setTranslate(&translate);
		// load models
		// -----------
		ourModel.push_back(main);
		Model* model;
		model = new Model("models/fogata.obj", main->cameraDetails);
		translate = glm::vec3(0.0f, 10.0f, 25.0f);
		model->setTranslate(&translate);
		rotation = glm::vec3(1.0f, 0.0f, 0.0f); //rotation X
		model->setRotX(45); // 45� rotation
		ourModel.push_back(model);
		model= new Model("models/pez.obj", main->cameraDetails);
		translate = glm::vec3(0.0f, 7.0f, 50.0f);
		model->setTranslate(&translate);
		ourModel.push_back(model);
//		model = new Model("models/IronMan.obj", main);
//		translate = glm::vec3(0.0f, 20.0f, 30.0f);
//		scale = glm::vec3(0.025f, 0.025f, 0.025f);	// it's a bit too big for our scene, so scale it down
//		model->setScale(&scale);
//		model->setTranslate(&translate);
//		ourModel.push_back(model);
		model = new Model("models/backpack.obj", main->cameraDetails, false, false);
		translate = glm::vec3(20.0f, 14.0f, 0.0f);
		scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		ourModel.push_back(model);
		inicializaBillboards();
	}

	void inicializaBillboards() {
		float ye = terreno->Superficie(0, 0);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol.png", 6, 6, 0, ye - 1, 0, camara->cameraDetails));

		ye = terreno->Superficie(5, -5);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol2.png", 6, 6, 5, ye - 1, -5, camara->cameraDetails));

		ye = terreno->Superficie(-9, -15);
		billBoard.push_back(new Billboard((WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye - 1, -15, camara->cameraDetails));
	}

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	Scenario* Render() {
		//borramos el biffer de color y el z para el control de profundidad a la 
		//hora del render a nivel pixel.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
//		glClearColor(255.0f, 255.0f, 255.0f, 255.0f);

		// Actualizamos la camara
		camara->cameraDetails->CamaraUpdate(camara->getRotY(), camara->getTranslate());

		if (this->animacion > 25) { // Timer se ejecuta cada 1000/30 = 33.333 ms
			float ye = terreno->Superficie(5, -5);
			Billboard* temporal = billBoard[1];
			wstring textura = L"billboards/Arbol" +
				(this->frameArbol == 1 ? L"" : to_wstring(this->frameArbol)) + L".png";
			billBoard[1] = new Billboard((WCHAR*)textura.c_str(), 6, 6, 5, ye - 1, -5, camara->cameraDetails);
			if (this->frameArbol == 3) {
				this->frameArbol = 1;
			}
			else {
				this->frameArbol++;
			}
			this->animacion = 0;
			delete temporal;
		}
		else {
			animacion++;
		}
		// Decimos que dibuje la media esfera
		sky->Draw();
		// Ahora el terreno
		terreno->Draw();
		water->Draw();
		// Dibujamos cada billboard que este cargado en el arreglo de billboards.
		for (int i = 0; i < billBoard.size(); i++)
			billBoard[i]->Draw();

		// Dibujamos cada modelo que este cargado en nuestro arreglo de modelos
		for (int i = 0; i < ourModel.size(); i++) {
			ourModel[i]->Draw();
		}

		// Le decimos a winapi que haga el update en la ventana
		return this;
	}
	
	std::vector<Model*> *getLoadedModels() {
		return &ourModel;
	}
	std::vector<Billboard*> *getLoadedBillboards() {
		return &billBoard;
	}
	Model* getMainModel() {
		return this->camara;
	}
	float getAngulo() {
		return this->angulo;
	}
	void setAngulo(float angulo) {
		this->angulo = angulo;
	}
	SkyDome* getSky() {
		return sky;
	}
	Terreno* getTerreno() {
		return terreno;
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