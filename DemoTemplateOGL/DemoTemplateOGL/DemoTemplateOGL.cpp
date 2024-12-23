// DemoTemplateOGL.cpp : Defines the entry point for the application.
//
#include "WinAPIHeaders/framework.h"
#include "WinAPIHeaders/DemoTemplateOGL.h"
#include "Base/Utilities.h"
#include "InputDevices/KeyboardInput.h"
#include <glad/glad.h>
#include <xstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Base/glext.h"
#include "Base/wglext.h"
#include "Base/model.h"

#include <array>

#ifdef _WIN32 
#include "InputDevices/GamePadRR.h"
#else
#include <GLFW/glfw3.h>
#endif
#include "Base/Scene.h"
#include "Scenario.h"
#include "Player.h"

#define MAX_LOADSTRING 100
#define Timer1 100






bool renderiza;                     // Variable para controlar el render
#ifdef _WIN32 
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HDC dc;
HGLRC rc;
GamePadRR* gamPad;                  // Manejador de gamepad
// Funciones para activar OpenGL version > 2.0
int prepareRenderWindow(HINSTANCE hInstance, int nCmdShow);
bool SetUpPixelFormat(HDC hDC, PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB, PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB);
// Callback principal de la ventana en WINAPI
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
#else
GLFWwindow* window;
void window_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
bool checkInput(GameActions* actions, Scene* scene);
void mouseActions();

// Propiedades de la ventana
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
bool newContext = false; // Bandera para identificar si OpenGL 2.0 > esta activa
struct GameTime gameTime;



// Objecto de escena y render
Scene *OGLobj;

#ifdef _WIN32 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEMOTEMPLATEOGL, szWindowClass, MAX_LOADSTRING);
    // Si no logra activar OpenGL 2 o superior termina el programa
    if (prepareRenderWindow(hInstance, nCmdShow))
        return 1;
    LOGGER::LOGS::getLOGGER().setWindow(hWnd);
    // game loop
    gamPad = new GamePadRR(1); // Obtenemos el primer gamepad conectado
    SetTimer(hWnd, Timer1, 1000/30, (TIMERPROC)WndProc);// Asignamos el timer con un render de 30 fps
    MSG msg = { 0 };
   
#else
int main(int argc, char** argv){
    if (!glfwInit()){
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT, "DemoTemplateOGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "No opengl load"<< endl;
        glfwTerminate();
        return -1;
    }
#endif
    // Main character with it's camera
    glm::vec3 translate, scale, v,rotate(0, 0, -1);


    translate = glm::vec3(5.0f, 10.0f, -5.0f);

    //5, ye - 1,-5
    //MainModel *model = new MainModel(hWnd, "models/Cube.obj", translate);
    // Esta es la camara prinicpal
    Camera* camera = new Camera();
    //Personaje principal
    Player * model = new Player("models/catidle.fbx", translate, camera);
    model->setTranslate(&translate);
    camera->setFront(v);
    rotate = glm::vec3(0.0f, 0.0f, 0.0f);
    camera->setCharacterHeight(3.5);

    scale = glm::vec3(1.0f, 1.0f, 1.0f);// it's a bit too big for our scene, so scale it down
    model->setScale(&scale);
    model->setRotX(0);
    model->setNextRotX(0);
    model->setTranslate(&translate);
    //Se soluciono?d
    try {
        Animation* ani = new Animation("models/catwalk.fbx", model->GetBoneInfoMap(), model->GetBoneCount());
        model->setAnimator(new Animator(ani));
    }
    catch (...) {
        cout << "Could not load animation!\n";
    }
    

    OGLobj = new Scenario(model); // Creamos nuestra escena con esa posicion de inicio
    translate = glm::vec3(5.0f, OGLobj->getTerreno()->Superficie(model->getNextTranslate()->x, model->getNextTranslate()->z), -5.0f);
    OGLobj->getTerreno()->getNextRotX();
    model->setTranslate(&translate);
    model->setNextTranslate(&translate);
    renderiza = true;

    int running = 1;
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    gameTime.lastTick = get_nanos() / 1000 /1000;
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#ifndef _WIN32
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    while(!glfwWindowShouldClose(window)){
        gameTime.deltaTime = get_nanos() / 1000 / 1000 - gameTime.lastTick;
        gameTime.lastTick = get_nanos() / 1000 / 1000;
        glfwPollEvents();
        OGLobj->setAngulo(OGLobj->getAngulo() + 1.5 * gameTime.deltaTime);
#else
    while (running) {
        gameTime.deltaTime = get_nanos() / 1000 / 1000 - gameTime.lastTick;
        gameTime.lastTick = get_nanos() / 1000 / 1000;
#endif
        if (renderiza) {
            GameActions actions;
            // render
            // ------
            bool checkCollition = checkInput(&actions, OGLobj);
            if (checkCollition) { // Bandera para buscar colisiones sobre Camara/Modelo
                if (OGLobj->lookForCollition(true) != NULL) { // Llamamos a la funcion de colision 
                    model->setNextTranslate(model->getTranslate());
                    model->setNextRotX(model->getRotX());
                    model->setNextRotY(model->getRotY());
                    model->setNextRotZ(model->getRotZ());
                } else {
                    //model->CamaraAvanza();
                    model->setTranslate(model->getNextTranslate());
                    model->setRotX(model->getNextRotX());
                    model->setRotY(model->getNextRotY());
                    model->setRotZ(model->getNextRotZ());
                }
            }
            Scene *escena = OGLobj->Render();
            if (escena != OGLobj) {
                delete OGLobj;
                OGLobj = escena;
            }
     //      renderiza = false;
#ifdef _WIN32
            SwapBuffers(dc);
#else
            glfwSwapBuffers(window);
#endif
        }
#ifdef _WIN32
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT){
                running = 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
#endif
    }
    if (OGLobj != NULL) delete OGLobj;
    if (camera != NULL) delete camera;
    if (model != NULL) delete model;
    font_atlas::clearInstance();
#ifndef _WIN32
	glfwTerminate();
#endif
#ifdef _WIN32
    return (int) msg.wParam;
#else
	return 0;
#endif
}

void mouseActions() {
#ifdef _WIN32 
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd, &p);
    cDelta.setPosition(p.x, p.y, cDelta.getLbtn() || cDelta.getRbtn());
#else
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cDelta.setPosition(x, y, cDelta.getLbtn() || cDelta.getRbtn());
#endif
}

bool checkInput(GameActions* actions, Scene* scene) {
    bool checkCollition = false;

#ifdef _WIN32 
    if (gamPad->IsConnected()) {
        // Conversión de valores del gamepad
        double grados = (float)gamPad->GetState().Gamepad.sThumbLX / 32767.0;
        if (grados > 0.19 || grados < -0.19) {
            actions->setAngle(grados * 3.0);
        }
        float velocidad = (float)gamPad->GetState().Gamepad.sThumbLY / 32767;
        if (velocidad > 0.19 || velocidad < -0.19) {
            actions->advance = velocidad;
            checkCollition = true;
        }
    }
#else
    if (false) {
        cout << "This should be the gamepad code \n";
    }
#endif
    else {
        mouseActions();
        checkCollition = KeysEvents(actions);
    }

    Model* mainModel = scene->getMainModel(); //Modelo principal
    if (actions->action) { // Si se presiona la tecla F

        Player* player = dynamic_cast<Player*>(mainModel);
        if (player) {
            // Radio del ataque del jugador
            float attackRadius = 5.0f; // Radio dentro del cual se afecta a los monstruos
            float attackDamage = 20.0f; // Daño del ataque

            // El jugador ataca los modelos cercanos (posiblemente monstruos)
            player->attack(*scene->getLoadedModels(), attackDamage, attackRadius);
        }

        actions->action = false; // Resetea la acción
    }
    if (actions->firstPerson) {
        // Alternar entre primera y tercera persona presionando la tecla P
        bool isFirstPerson = mainModel->cameraDetails->getFirstPerson();
        mainModel->cameraDetails->setFirstPerson(!isFirstPerson);

        // Mensaje opcional de depuración
        std::cout << "Modo de cámara cambiado a: "
            << (isFirstPerson ? "Tercera Persona" : "Primera Persona")
            << std::endl;
    }

    if (actions->sideAdvance != 0) {
        // Obtenemos la rotación actual únicamente en el eje Y
        float currentRotY = mainModel->getRotY();

        // Calculamos la nueva rotación solo en el eje Y (sin inclinar)
        float newRotY = currentRotY + (8.0f * actions->sideAdvance); // Ajusta el "5.0f" para controlar la sensibilidad de giro

        // Aplicamos la rotación únicamente en el eje Y
        mainModel->setRotY(newRotY);
        mainModel->setNextRotY(newRotY);

        checkCollition = true; // Detectar colisiones si es necesario
    }


    if (actions->advance != 0) {
        //Movimiento del personaje aqui nos basaremos para hacer el movimiento del monstrou
        glm::vec3 pos = *mainModel->getTranslate();
        pos.x += actions->advance * 1.20f * glm::sin(glm::radians(mainModel->getRotY()));
        pos.z += actions->advance * 1.20f * glm::cos(glm::radians(mainModel->getRotY()));

        // Ajustar la altura con base en el terreno
        pos.y = scene->getTerreno()->Superficie(pos.x, pos.z);
        mainModel->setNextTranslate(&pos);
        checkCollition = true;
    }


    // Otros ángulos de cámara
    if (actions->getAngle() != nullptr) {
        mainModel->cameraDetails->calculateAngleAroundPlayer((*actions->getAngle()) * 3.0f);
    }
    if (actions->getPitch() != nullptr) {
        mainModel->cameraDetails->setPitch(mainModel->cameraDetails->getPitch() + (*actions->getPitch()) * 0.50f * 3.0f);
    }
    if (actions->getZoom() != nullptr) {
        mainModel->cameraDetails->setZoom(mainModel->cameraDetails->getZoom() + *actions->getZoom() * 0.50f);
    }
    if (actions->getPlayerZoom() != nullptr) {
        mainModel->cameraDetails->calculateZoomPlayer(*actions->getPlayerZoom() * 0.50f);
    }

    return checkCollition;
}


#ifdef _WIN32
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
        } break;
        case WM_COMMAND: {
        } break;
        case WM_MOUSEMOVE: {
            int value = lParam;
        }break;
        case WM_TIMER: {
            if (OGLobj != NULL)
                OGLobj->setAngulo(OGLobj->getAngulo() + 1.5);
//            if (!renderiza)
//                renderiza = true;
        } break;
        case WM_PAINT: {
        }break;
        case WM_DESTROY: {
            if (newContext) {
                ReleaseDC(hWnd, dc);
                wglDeleteContext(rc);
                PostQuitMessage(0);
            }
        } break;
        case WM_SIZE: {
            if (newContext) {
                //esta opcion del switch se ejecuta una sola vez al arrancar y si se
                //afecta el tama�o de la misma se dispara de nuevo
                int height = HIWORD(lParam),
                    width = LOWORD(lParam);
                if (height == 0)
                    width = 1;
                SCR_HEIGHT = height;
                SCR_WIDTH = width;
                glViewport(0, 0, width, height);
            }
        } break;
        case WM_LBUTTONDOWN: {
            cDelta.setLbtn(true);
        }break;
        case WM_LBUTTONUP: {
            cDelta.setLbtn(false);
        }break;
        case WM_RBUTTONDOWN: {
            cDelta.setRbtn(true);
        }break;
        case WM_RBUTTONUP: {
            cDelta.setRbtn(false);
        }break;
        case WM_MOUSEWHEEL: {
            char delta = HIWORD(wParam);
            cDelta.setMouseWheel(delta);
        }break;
        case WM_KEYDOWN: {
            KEYS[wParam] = true;
        } break;
        case WM_KEYUP: {
            if (wParam == KEYB_CAMERA || wParam == KEYB_HMOVEMENT)
                KEYS[wParam] = false;
        } break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetUpPixelFormat(HDC hDC, PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB, PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB) {
    const int pixelAttribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
//        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
//        WGL_SAMPLES_ARB, 4,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_ALPHA_BITS_ARB, 8,
        0 
    };
    int pixelFormatID;
    UINT numFormats;
    bool status = wglChoosePixelFormatARB(hDC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
    if (status == false || numFormats == 0)
        return true;
    PIXELFORMATDESCRIPTOR PFD;
    DescribePixelFormat(hDC, pixelFormatID, sizeof(PFD), &PFD);
    SetPixelFormat(hDC, pixelFormatID, &PFD);
    return false;
}

// Funciones de inicializacion para ventana compatible con OpenGL
int prepareRenderWindow(HINSTANCE hInstance, int nCmdShow) {
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = szWindowClass;
    RegisterClassEx(&wc);
    hInst = hInstance;
    HWND fakeWND = CreateWindow(
        szWindowClass, L"Fake Window",      // window class, title
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // style
        0, 0,                       // position x, y
        1, 1,                       // width, height
        NULL, NULL,                 // parent window, menu
        hInstance, NULL);           // instance, param
    HDC fakeDC = GetDC(fakeWND);        // Device Context
    PIXELFORMATDESCRIPTOR fakePFD;
    ZeroMemory(&fakePFD, sizeof(fakePFD));
    fakePFD.nSize = sizeof(fakePFD);
    fakePFD.nVersion = 1;
    fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    fakePFD.iPixelType = PFD_TYPE_RGBA;
    fakePFD.cColorBits = 32;
    fakePFD.cAlphaBits = 8;
    fakePFD.cDepthBits = 24;
    int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
    if (fakePFDID == 0) {
        MessageBox(fakeWND, L"ChoosePixelFormat() failed.", L"", 0);
        return 1;
    }
    if (SetPixelFormat(fakeDC, fakePFDID, &fakePFD) == false) {
        MessageBox(fakeWND, L"SetPixelFormat() failed.", L"", 0);
        return 1;
    }
    HGLRC fakeRC = wglCreateContext(fakeDC);    // Rendering Contex
    if (fakeRC == 0) {
        MessageBox(fakeWND, L"wglCreateContext() failed.", L"", 0);
        return 1;
    }
    if (wglMakeCurrent(fakeDC, fakeRC) == false) {
        MessageBox(fakeWND, L"wglMakeCurrent() failed.", L"", 0);
        return 1;
    }
    wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
    if (wglChoosePixelFormatARB == nullptr) {
        MessageBox(fakeWND, L"wglGetProcAddress() failed.", L"", 0);
        return 1;
    }
    wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
    if (wglCreateContextAttribsARB == nullptr) {
        MessageBox(fakeWND, L"wglGetProcAddress() failed.", L"", 0);
        return 1;
    }
    gladLoadGL();
    // create window
    RECT wr = { 0, 0, SCR_WIDTH, SCR_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100,
        wr.right - wr.left, //SCR_WIDTH, 
        wr.bottom - wr.top, //SCR_HEIGHT,
        NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        return 1;
    }
    dc = NULL;
    dc = GetDC(hWnd);
    if (SetUpPixelFormat(dc, wglChoosePixelFormatARB, wglCreateContextAttribsARB)) {
        MessageBox(hWnd, L"wglChoosePixelFormatARB() failed.", L"", 0);
        return 1;
    }
    const int major_min = 3, minor_min = 2;
    int  contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    rc = NULL;
//    rc = wglCreateContext(dc);
    rc = wglCreateContextAttribsARB(dc, 0, contextAttribs);
    if (rc == NULL) {
        MessageBox(hWnd, L"wglCreateContextAttribsARB() failed.", L"", 0);
        return 1;
    }
//    wglMakeCurrent(dc, rc);
    wglDeleteContext(fakeRC);
    ReleaseDC(fakeWND, fakeDC);
    DestroyWindow(fakeWND);
    if (!wglMakeCurrent(dc, rc)) {
        MessageBox(hWnd, L"wglMakeCurrent() failed.", L"", 0);
        return 1;
    }
    gladLoadGL();
    newContext = true;
    return 0;
}
#else
void window_size_callback(GLFWwindow* window, int width, int height){
    if (height == 0)
        width = 1;
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    char k = (key == GLFW_KEY_LEFT_SHIFT)? input.Shift : key;
    if (action == GLFW_PRESS || action == GLFW_REPEAT || action == GLFW_RELEASE)
        KEYS[k] = GLFW_RELEASE == action ? false : true;
    else
        if (k == KEYB_CAMERA || k == KEYB_HMOVEMENT)
            KEYS[k] = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        cDelta.setRbtn(action == GLFW_PRESS);
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
        cDelta.setLbtn(action == GLFW_PRESS);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    char delta = yoffset;
    cDelta.setMouseWheel(delta);
}
#endif
