#include "WinAPIHeaders/framework.h"
#include "WinAPIHeaders/DemoTemplateOGL.h"
#include "Base/Utilities.h"
#include "InputDevices/KeyboardInput.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Base/glext.h"
#include "Base/wglext.h"
#include "Base/model.h"

#ifdef _WIN32 
#include "InputDevices/GamePadRR.h"
#else
#include <GLFW/glfw3.h>
#endif
#include "Base/Scene.h"
#include "Scenario.h"

using namespace std;


#define MAX_LOADSTRING 100
#define Timer1 100
bool renderiza;                     // Variable para controlar el render

// Funciones para activar OpenGL version > 2.0
int prepareRenderWindow(HINSTANCE hInstance, int nCmdShow);
bool checkInput(GameActions* actions, Scene *scene);
void mouseActions();

// Propiedades de la ventana
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
bool newContext = false; // Bandera para identificar si OpenGL 2.0 > esta activa
struct GameTime gameTime;

// Objecto de escena y render
Scene *OGLobj;

int main(int, char**){
    GLFWwindow* window;

    if (!glfwInit()){
        return -1;
    }
    window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "DemoTemplateOGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "No opengl load"<< endl;
        glfwTerminate();
        return -1;
    }
    // Main character with it's camera
    glm::vec3 translate, scale, v(0, 0, -1);
    translate = glm::vec3(5.0f, 10.0f, -5.0f);
    //5, ye - 1,-5
    //MainModel *model = new MainModel(hWnd, "models/Cube.obj", translate);
    Camera* camera = new Camera();
    Model* model = new Model("models/BaseSpiderman.obj", translate, camera);
    model->setTranslate(&translate);
    camera->setFront(v);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);	// it's a bit too big for our scene, so scale it down
    model->setScale(&scale);
    model->setTranslate(&translate);
    
    OGLobj = new Scenario(model); // Creamos nuestra escena con esa posicion de inicio
//    SetTimer(hWnd, Timer1, 1000/30, (TIMERPROC)WndProc);// Asignamos el timer con un render de 30 fps
    renderiza = true;
//    MSG msg = { 0 };

    int running = 1;
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        gameTime.deltaTime = get_nanos() - gameTime.lastTick;
        gameTime.lastTick = get_nanos();
        if (renderiza) {
            GameActions actions;
            // render
            // ------
            bool checkCollition = false;//checkInput(&actions, OGLobj);
            if (checkCollition) { // Bandera para buscar colisiones sobre Camara/Modelo
                // Posicionamos la camara/modelo pixeles arriba de su posicion en el terreno
                model->getNextTranslate()->y = OGLobj->getTerreno()->Superficie(model->getNextTranslate()->x, model->getNextTranslate()->z) + 1.7;
                if (OGLobj->lookForCollition(true) != NULL) { // Llamamos a la funcion de colision 
                    model->setNextTranslate(model->getTranslate());
                    model->setNextRotX(model->getRotX());
                    model->setNextRotY(model->getRotY());
                    model->setNextRotZ(model->getRotZ());
                    model->setNextRotationVector(model->getRotationVector());
                } else {
                    //model->CamaraAvanza();
                    model->setTranslate(model->getNextTranslate());
                    model->setRotX(model->getNextRotX());
                    model->setRotY(model->getNextRotY());
                    model->setRotZ(model->getNextRotZ());
                    model->setRotationVector(model->getNextRotationVector());
                }
            }
            Scene *escena = OGLobj->Render();
            glfwSwapBuffers(window);
            if (escena != OGLobj) {
                delete OGLobj;
                OGLobj = escena;
            }
//            renderiza = false;
        }
    }
    glfwTerminate();
    if (OGLobj != NULL) delete OGLobj;
    if (camera != NULL) delete camera;
    if (model != NULL) delete model;
    return 0;
}
