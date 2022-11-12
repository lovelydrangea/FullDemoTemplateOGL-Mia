#include "KeyboardInput.h"

struct Input input;
bool KEYS[256];

void Init() {
	for (int i = 0; i < 256; i++) {
		KEYS[i] = false;
	}
}//Initizalizes keys

bool KeysEvents(Scene *scene){
	MainModel* OGLobj = scene->getMainModel();
	bool checkCollition = false;
	if (KEYS[input.A]) {
//		OGLobj->ProcessKeyboard(LEFT, 0.50 * 3.0);
		OGLobj->CamaraGiraY(-0.50 * 3.0);
		KEYS[input.A] = false;
//		OGLobj->backCollition = false;
//		OGLobj->frontCollition = false;
	}
	if (KEYS[input.D]) {
//		OGLobj->ProcessKeyboard(RIGHT, 0.50 * 3.0);
		OGLobj->CamaraGiraY(0.50 * 3.0);
//		OGLobj->backCollition = false;
//		OGLobj->frontCollition = false;
		KEYS[input.D] = false;
	}
	if (KEYS[input.W]) {
//		OGLobj->ProcessKeyboard(FORWARD, 0.50);
		KEYS[input.W] = false;
		OGLobj->movePosition(0.50);
//		OGLobj->frontCollition = true;
		checkCollition = true;
	}
	if (KEYS[input.S]) {
//		OGLobj->ProcessKeyboard(BACKWARD, 0.50);
		KEYS[input.S] = false;
		OGLobj->movePosition(-0.50);
//		OGLobj->backCollition = true;
		checkCollition = true;
	}
	return checkCollision;
}
