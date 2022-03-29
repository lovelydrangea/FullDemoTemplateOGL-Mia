#include "KeyboardInput.h"

struct Input input;
bool KEYS[256];

void Init() {
	for (int i = 0; i < 256; i++) {
		KEYS[i] = false;
	}
}//Initizalizes keys

void KeysEvents(Scenario *scene){
	MainModel* OGLobj = scene->camara;
	if (KEYS[input.A]) {
//		OGLobj->ProcessKeyboard(LEFT, 0.50 * 3.0);
		OGLobj->CamaraGiraY(-0.50 * 3.0);
		OGLobj->backCollition = false;
		OGLobj->frontCollition = false;
		KEYS[input.A] = false;
	}
	if (KEYS[input.D]) {
//		OGLobj->ProcessKeyboard(RIGHT, 0.50 * 3.0);
		OGLobj->CamaraGiraY(0.50 * 3.0);
		OGLobj->backCollition = false;
		OGLobj->frontCollition = false;
		KEYS[input.D] = false;
	}
	if (KEYS[input.W]) {
//		OGLobj->ProcessKeyboard(FORWARD, 0.50);
		if (!OGLobj->frontCollition) {
			OGLobj->CamaraAvanza(0.50);
			if (scene->lookForCollition()) {
				OGLobj->CamaraAvanza(-0.50);
				OGLobj->frontCollition = true;
			}
			OGLobj->backCollition = false;
		}
		KEYS[input.W] = false;
	}
	if (KEYS[input.S]) {
//		OGLobj->ProcessKeyboard(BACKWARD, 0.50);
		if (!OGLobj->backCollition) {
			OGLobj->CamaraAvanza(-0.50);
			if (scene->lookForCollition()) {
				OGLobj->CamaraAvanza(0.50);
				OGLobj->backCollition = true;
			}
			OGLobj->frontCollition = false;
		}
		KEYS[input.S] = false;
	}

}
