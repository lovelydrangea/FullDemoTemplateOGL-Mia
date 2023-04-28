#pragma once
#include "Model.h"
#include "SkyDome.h"
#include "Terreno.h"
#include "Billboard.h"

class Scene {
	public:
		virtual float getAngulo() = 0;
		virtual void setAngulo(float angulo) = 0;
		virtual Model* getMainModel() = 0;
		virtual Scene* Render(HDC hDC) = 0;
		virtual SkyDome* getSky() = 0;
		virtual Terreno* getTerreno() = 0;
		virtual std::vector<Model*> *getLoadedModels() = 0;
		virtual std::vector<Billboard*> *getLoadedBillboards() = 0;

		Model* lookForCollition(bool collitionMovement = false) {
			std::vector<Model*> *ourModel = getLoadedModels();
			Model *camara = getMainModel();
			for (int i = 0; i < ourModel->size(); i++) {
				if ((*ourModel)[i] != camara) {
					if (camara->colisionaCon((*ourModel)[i], collitionMovement)) {
						return (*ourModel)[i];
					}
				}
			}
			return NULL;
		}
};