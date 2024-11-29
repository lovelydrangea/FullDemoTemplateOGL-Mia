#ifndef GRAPHICSRR_H
#define GRAPHICSRR_H

#ifdef _WIN32 
#include <windows.h>
#include <windowsx.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para seed de random
#include <vector>
#include "Base/camera.h"
#include "Base/Billboard.h"
#include <iostream>
#include "Base/Scene.h"
#include "Texto.h"
#include "Billboard2D.h"
#include <GLFW/glfw3.h>


using namespace std;
using namespace glm;

class Scenario : public Scene {
private:
	SkyDome* sky;
	Terreno* terreno;
	Shader* rainShader;
	vector<Billboard*> billBoard;
	vector<Billboard2D*> billBoard2D;
	vector<Model*> ourModel;
	Model* camara;
	Water* water;
	float angulo;
	int animacion = 0;
	int frameArbol = 1;
	vector<Texto*> ourText;
	wstring wCoordenadas;
	Texto *coordenadas = NULL;
	mat4 viewMatrix;
	mat4 projectionMatrix;
public:
	Scenario(Camera *cam) {
		vec3 translate;
		vec3 scale;
		Model* model = new Model("models/Cube.obj", cam);
		translate = vec3(0.0f, 0.0f, 3.0f);
		scale = vec3(0.25f, 0.25f, 0.25f);	// it's a bit too big for our scene, so scale it down
		model->setScale(&scale);
		model->setTranslate(&translate);
		InitGraph(model);


	}
	Scenario(Model *camIni) {
		InitGraph(camIni);


	}
	void InitGraph(Model* main) {
		glEnable(GL_PROGRAM_POINT_SIZE);

		float matAmbient[] = { 1,1,1,1 };
		float matDiff[] = { 1,1,1,1 };
		angulo = 0;
		camara = main;
		//creamos el objeto skydome
		sky = new SkyDome(20, 17, 12, (WCHAR*)L"skydome/earth.jpg", main->cameraDetails);
		//creamos el terreno
		
		terreno = new Terreno((WCHAR*)L"skydome/terreno.jpg", (WCHAR*)L"skydome/texterr.jpg", 400, 400, main->cameraDetails);
		water = new Water((WCHAR*)L"textures/terreno.bmp", (WCHAR*)L"textures/water.bmp", 80, 50, camara->cameraDetails);
		vec3 translate;
		vec3 scale;
		vec3 rotation;
		translate = vec3(-97.0f, terreno->Superficie(-97.0f, -5.0f) + 2.9f, -5.0f);
		rotation = vec3(0.0f, 0.0f, 1.0f);
		water->setRotZ(0);
		water->setTranslate(&translate);
		// load models
		// -----------
		ourModel.emplace_back(main);
		Model* model;

		vec3 PosicionJugador = glm::vec3(5.0f, terreno->Superficie(5.0f, -13.0f), -13.0f);
		vec3 PosicionCasa = glm::vec3(38.0f, terreno->Superficie(18.0f, 27.0f) + 10, 27.0f);

		CrearBosque(ourModel, terreno, main->cameraDetails, 18, PosicionJugador, PosicionCasa, 12.0f);
		// Creamos un pequeñito bosque utilizando las librerias, randomizando como tal el escenario
		// Modelos mios jajsjasjasd
		//Pinos
		/*

		*/
		//Fogata para complementar el bosque!!
		model = new Model("models/fogata.obj", main->cameraDetails);
		translate = vec3(0.0f, 10.0f, 25.0f);
		model->setTranslate(&translate);
		rotation = vec3(1.0f, 0.0f, 0.0f); //rotation X
		model->setRotX(0); // 45� rotation
		ourModel.emplace_back(model);
		model->AABB = NULL;


		//Casita
		model = new Model("models/model.dae", main->cameraDetails);
		translate = vec3(38.0f, terreno->Superficie(18.0f, 27.0f) + 10, 27.0f);
		scale = vec3(15.0f, 15.0f, 15.0f);
		model->setScale(&scale);
		model->setTranslate(&translate);
		ourModel.emplace_back(model);
		//Monstrous se ven oscuros

		model = new Model("models/base.obj", main->cameraDetails,true,true);
		translate = vec3(-54.0f, terreno->Superficie(-54.0f,23.0f)+3, 23.0f);
		model->setTranslate(&translate);
		scale = vec3(3.0f, 3.0f, 3.0f);
		rotation = vec3(1.0f, 1.0f, 0.0f); //rotation X
		model->setScale(&scale);
		model->setRotY(0); // 45� rotation
		model->setRotY(90);
		ourModel.emplace_back(model);



		// señal
		model = new Model("models/sign.fbx", main->cameraDetails, false, false);
		translate = vec3(6.0f, terreno->Superficie(6.0f,35.0f), 35.0f);
		scale = vec3(.0025f, .0025f, .0025f);
		model->setScale(&scale);
		model->setTranslate(&translate);
		ourModel.emplace_back(model);
		delete model->AABB;
		model->AABB = NULL;



		model = new Model("models/monster.fbx", main->cameraDetails);
		translate = vec3(-53.0f, terreno->Superficie(-53.0f, 52.0f)+2 , 52.0f);
		scale = vec3(.04f, .04f, .04f);
		rotation = vec3(1.0f, 0.0f, 1.0f); //rotation X
		model->setRotX(-90);
		model->setRotZ(90);
		model->setScale(&scale);
		model->setTranslate(&translate);
		ourModel.emplace_back(model);

		Camera* camrita = new Camera();
		camrita

		

		/*
		model= new Model("models/pez.obj", main->cameraDetails);
		translate = vec3(0.0f, 7.0f, 50.0f);
		model->setTranslate(&translate);
		ourModel.emplace_back(model);

		model = new Model("models/dancing_vampire.dae", main->cameraDetails);
		translate = vec3(0.0f, terreno->Superficie(0.0f, 60.0f) , 60.0f);
		scale = vec3(0.1f, 0.1f, 0.1f);	// it's a bit too big for our scene, so scale it down
		model->setTranslate(&translate);
		model->setScale(&scale);
		model->setRotY(90);
		ourModel.emplace_back(model);
		try {
			Animation *ani = new Animation("models/dancing_vampire.dae", model->GetBoneInfoMap(), model->GetBoneCount());
		    model->setAnimator(new Animator(ani));
		}catch(...){
			cout << "Could not load animation!\n";
		}


		*/;

		InitRainShader();
		InitRainGeometry();

		inicializaBillboards();
		
		
		wstring prueba(L"");
		ourText.emplace_back(new Texto(prueba, 20, 0, 0, SCR_HEIGHT, 0, camara));
		// Inicializa un Billboard2D antes de intentar usarlo
		billBoard2D.emplace_back(new Billboard2D((WCHAR*)L"", 6, 6, 100, 100, 0, camara->cameraDetails));
		if (!billBoard2D.empty()) {
			vec3 scale = vec3(100.0f, 100.0f, 0.0f); // Asegura que la escala está definida aquí
			billBoard2D.back()->setScale(&scale);
		}
		else {
			std::cerr << "Error: El vector billBoard2D está vacío al intentar establecer la escala.\n";
		}

		wCoordenadas = L"X: " + to_wstring(getMainModel()->getTranslate()->x) +
			L" Y: " + to_wstring(getMainModel()->getTranslate()->y) +
			L" Z: " + to_wstring(getMainModel()->getTranslate()->z);
		coordenadas = new Texto(wCoordenadas, 15, 0, 0, 0, 0, camara);

	}
	

	void InitRainShader() {
		glEnable(GL_PROGRAM_POINT_SIZE);

		// Inicializar el shader de la lluvia
		rainShader = new Shader("shaders/shader.vert", "shaders/shader.frag", nullptr);

		// Activar el shader
		rainShader->use();

		// Establecer variables uniform necesarias (ejemplo)
		rainShader->setMat4("viewMatrix", viewMatrix);
		rainShader->setMat4("projectionMatrix", projectionMatrix);

		// Verificar errores de compilación y enlace
	}


	unsigned int rainVAO, rainVBO;

	void InitRainGeometry() {
		std::vector<glm::vec3> rainPositions;

		// Genera posiciones aleatorias para las gotas de lluvia
		for (int i = 0; i < 10000; i++) {
			float x = ((rand() % 200) - 100) / 100.0f; // Posición X
			float y = ((rand() % 200) - 100) / 100.0f; // Posición Y
			float z = ((rand() % 200) - 100) / 100.0f; // Posición Z
			rainPositions.push_back(glm::vec3(x, y, z));
		}

		glGenVertexArrays(1, &rainVAO);
		glGenBuffers(1, &rainVBO);

		glBindVertexArray(rainVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rainVBO);
		glBufferData(GL_ARRAY_BUFFER, rainPositions.size() * sizeof(glm::vec3), &rainPositions[0], GL_STATIC_DRAW);

		// Configura atributos de posición
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void RenderRain() {
		glEnable(GL_PROGRAM_POINT_SIZE);

		// Asegúrate de que el shader de la lluvia esté cargado y listo
		if (rainShader) {
			rainShader->use(); // Activa el shader de lluvia

			// Configura las propiedades de la luz
			rainShader->setVec3("lightPosition", glm::vec3(10.0f, 50.0f, 20.0f)); // Posición de la luz
			rainShader->setVec3("lightColor", glm::vec3(10.0f, 10.0f, 10.0f));       // Color de la luz

			// Calcula la matriz MVP (Model-View-Projection)
			glm::mat4 model = glm::mat4(1.0f); // Matriz modelo (identidad si no necesitas transformaciones)
			glm::mat4 MVP = projectionMatrix * viewMatrix * model;

			// Envía la matriz MVP al shader
			rainShader->setMat4("MVP", MVP);

			// Envía el tiempo transcurrido como una variable uniforme para animar las partículas
			float timeValue = static_cast<float>(glfwGetTime());
			rainShader->setFloat("time", timeValue);

			// Vincula el VAO que contiene las partículas de lluvia (configuración de vértices)
			glBindVertexArray(rainVAO);

			// Dibuja las partículas como puntos (1000 partículas, ajusta según sea necesario)
			glDrawArrays(GL_POINTS, 0, 100000);

			// Desvincula el VAO (buen hábito para evitar efectos no deseados)
			glBindVertexArray(1);

			

			// Desactiva el shader actual
			rainShader->desuse();

		}
		
	}





	void inicializaBillboards() {

		float ye = terreno->Superficie(0, 0);
		billBoard.emplace_back(new Billboard((WCHAR*)L"billboards/Arbol.png", 6, 6, 0, ye - 1, 0, camara->cameraDetails));

		ye = terreno->Superficie(5, -5);
		billBoard.emplace_back(new Billboard((WCHAR*)L"billboards/Arbol2.png", 6, 6, 5, ye - 1, -5, camara->cameraDetails));

		ye = terreno->Superficie(-9, -15);
		billBoard.emplace_back(new Billboard((WCHAR*)L"billboards/Arbol3.png", 8, 8, -9, ye - 1, -15, camara->cameraDetails));


	}

	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	Scene* Render() {
		glEnable(GL_PROGRAM_POINT_SIZE);

		// Validación inicial de los recursos clave
		if (!sky) {
			std::cerr << "Error: SkyDome no está inicializado.\n";
			return nullptr;
		}
		if (!terreno) {
			std::cerr << "Error: Terreno no está inicializado.\n";
			return nullptr;
		}
		if (!water) {
			std::cerr << "Error: Water no está inicializado.\n";
			return nullptr;
		}
		if (billBoard.empty()) {
			std::cerr << "Error: No hay Billboards cargados.\n";
			return nullptr;
		}
		if (billBoard2D.empty()) {
			std::cerr << "Error: No hay Billboards 2D cargados.\n";
			return nullptr;
		}
		if (ourModel.empty()) {
			std::cerr << "Error: No hay modelos cargados.\n";
			return nullptr;
		}
		if (!camara || !camara->cameraDetails) {
			std::cerr << "Error: Cámara o detalles de cámara no están inicializados.\n";
			return nullptr;
		}

		try {
			// Renderizado principal
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

			// Actualización de la cámara
			camara->cameraDetails->CamaraUpdate(camara->getRotY(), camara->getTranslate());
			
			// Animación y actualización de billboards
			if (this->animacion > 25) {
				if (billBoard.size() > 1) {
					float ye = terreno->Superficie(5, -5);
					Billboard* temporal = billBoard[1];
					wstring textura = L"billboards/Arbol" +
						(this->frameArbol == 1 ? L"" : to_wstring(this->frameArbol)) + L".png";
					try {
						billBoard[1] = new Billboard((WCHAR*)textura.c_str(), 6, 6, 5, ye - 1, -5, camara->cameraDetails);
						delete temporal;
					}
					catch (const std::exception& e) {
						std::cerr << "Error al actualizar Billboard: " << e.what() << "\n";
					}
					this->frameArbol = (this->frameArbol == 3) ? 1 : this->frameArbol + 1;
				}
				this->animacion = 0;
			}
			else {
				animacion++;
			}
			

			// Calcula las matrices de vista y proyección
			viewMatrix = camara->cameraDetails->GetViewMatrix();
			projectionMatrix = camara->cameraDetails->GetProjectionMatrix();


			// Dibujar el cielo
			sky->Draw();

			// Dibujar el terreno
			terreno->Draw();

			// Dibujar el agua
			water->Draw();
			//espera y jala la lluvia
			RenderRain();

			// Dibujar billboards 3D
			for (auto& billboard : billBoard) {
				if (billboard) billboard->Draw();
			}

			// Dibujar modelos
			for (auto& model : ourModel) {
				if (model) model->Draw();
			}

			// Dibujar textos
			for (auto& text : ourText) {
				if (text) text->Draw();
			}
		

			// Actualizar y dibujar coordenadas
			if (!ourModel.empty() && getMainModel()) {
				wCoordenadas = L"X:" + to_wstring(getMainModel()->getTranslate()->x) +
					L" Y:" + to_wstring(getMainModel()->getTranslate()->y) +
					L" Z:" + to_wstring(getMainModel()->getTranslate()->z);
				coordenadas->initTexto(wCoordenadas);
				coordenadas->Draw();
			}
			else {
				std::cerr << "Error: El modelo principal no está disponible.\n";
			}

		}
		catch (const std::exception& e) {
			std::cerr << "Excepción capturada durante el renderizado: " << e.what() << "\n";
			return nullptr;
		}
		catch (...) {
			std::cerr << "Excepción desconocida capturada durante el renderizado.\n";
			return nullptr;
		}

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
		if (billBoard2D.size() > 0)
			for (int i = 0; i < billBoard2D.size(); i++)
				delete billBoard2D[i];
		this->billBoard.clear();
		if (ourText.size() > 0)
			for (int i = 0; i < ourText.size(); i++)
				delete ourText[i];
		this->ourText.clear();
		if (ourModel.size() > 0)
			for (int i = 0; i < ourModel.size(); i++)
				if (ourModel[i] != camara)
					delete ourModel[i];
		this->ourModel.clear();
	}
	void CrearBosque(std::vector<Model*>& ourModel, Terreno* terreno, Camera* camaraDetails, int cantidadArboles, glm::vec3 PosicionJugador, glm::vec3 PosicionCasa, float distanciaMinima) {
		// Semilla para números aleatorios
		srand(static_cast<unsigned>(time(0)));

		for (int i = 0; i < cantidadArboles; ++i) {
			bool posicionValida = false;
			vec3 translate;

			// Intentar generar una posición válida
			int intentos = 0; // Para evitar bucles infinitos
			while (!posicionValida && intentos < 100) {
				// Generar coordenadas X y Z aleatorias dentro de un rango
				float x = static_cast<float>((rand() % 100) - 50); // Rango: -50 a 50
				float z = static_cast<float>((rand() % 100) - 50); // Rango: -50 a 50

				// Obtener la altura del terreno en esas coordenadas
				float y = terreno->Superficie(x, z);

				translate = vec3(x, y, z);

				// Verificar que las distancias sean mayores o iguales a la distancia mínima en X, Y y Z
				bool lejosDeJugador = abs(translate.x - PosicionJugador.x) >= distanciaMinima &&
					abs(translate.z - PosicionJugador.z) >= distanciaMinima;

				bool lejosDeCasa = abs(translate.x - PosicionCasa.x) >= distanciaMinima &&
					abs(translate.z - PosicionCasa.z) >= distanciaMinima;

				if (lejosDeJugador && lejosDeCasa) {
					posicionValida = true;
				}

				intentos++;
			}

			if (!posicionValida) {
				std::cerr << "Advertencia: No se pudo encontrar una posición válida para un árbol tras 100 intentos.\n";
				continue; // Saltar este árbol si no se encuentra una posición válida
			}

			// Crear y posicionar el modelo del árbol
			Model* model = new Model("models/Low Poly Pine.obj", camaraDetails);
			model->setTranslate(&translate);

			// Escalar el árbol para variar su tamaño, si lo deseas (opcional)
			float scaleValue = static_cast<float>((rand() % 5 + 10)) / 10.0f; // Rango: 1.0f - 1.5f
			vec3 scale = vec3(scaleValue, scaleValue, scaleValue);
			model->setScale(&scale);

			// Agregar el modelo a la lista
			ourModel.emplace_back(model);
		}
	}

};

#endif