#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "DemoTemplateOGL.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    HWND hWnd;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
        //        if (Up.y > 0.0f)
        //            Up.y *= -1.0f;
    }
public:
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 CamaraUpdate() {
        return glm::lookAt(Position,
            Position + Front,
            glm::vec3(0, 1, 0));
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void CamaraAvanza(float vel)
    {
        Position.x = Position.x + Front.x * vel;
        Position.y = Position.y + Front.y * vel;
        Position.z = Position.z + Front.z * vel;

        setPosition(Position);
        /*gluLookAt(posc.X, posc.Y, posc.Z,
            posc.X + dirc.X, posc.Y, posc.Z + dirc.Z,
            0, 1, 0);*/
        glm::vec3 tmp(Front.x,0,Front.z);
        glm::lookAt(Position, Position + tmp, Up);
    }

    void CamaraGiraY(float grados) {
        glm::vec3 v = Transforma(Front, grados, Ejes::EjeY);
        setFront(v);
    }

    //esto es de Graficas Computacionales no ocupan aprenderselo para la revision
    glm::vec3 Transforma(glm::vec3 &v, float grados2, int eje = 0)
    {
        float Matriz[4][4];
        float grados = grados2 * 3.141516 / 180.0;

        Identidad(Matriz);

        if (eje == Ejes::EjeX)
        {
            Matriz[1][1] = cos(grados);
            Matriz[2][2] = Matriz[1][1];
            Matriz[1][2] = -sin(grados);
            Matriz[2][1] = sin(grados);
        }

        if (eje == Ejes::EjeY)
        {
            Matriz[0][0] = cos(grados);
            Matriz[2][2] = Matriz[0][0];
            Matriz[0][2] = -sin(grados);
            Matriz[2][0] = sin(grados);
        }

        if (eje == Ejes::EjeZ)
        {
            Matriz[0][0] = cos(grados);
            Matriz[1][1] = Matriz[0][0];
            Matriz[0][1] = -sin(grados);
            Matriz[1][0] = sin(grados);
        }

        glm::vec3 aux;

        aux.x = v.x * Matriz[0][0] + v.y * Matriz[0][1] + v.z * Matriz[0][2];
        aux.y = v.x * Matriz[1][0] + v.y * Matriz[1][1] + v.z * Matriz[1][2];
        aux.z = v.x * Matriz[2][0] + v.y * Matriz[2][1] + v.z * Matriz[2][2];

        v = aux;
        return v;
    }

    //esto es de Graficas Computacionales no ocupan aprenderselo para la revision
    void Identidad(float Mat[4][4])
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Mat[i][j] = 0;
                if (i == j)
                    Mat[i][j] = 1;
            }
        }
    }
    glm::vec3 &getPosition() { return Position; };
    void setPosition(glm::vec3 &Position) { this->Position = Position; cameraDetails.Position = &this->Position; };
    glm::vec3 &getFront() { return Front; };
    void setFront(glm::vec3 &Front) { this->Front = Front; cameraDetails.Front = &this->Front; };
    glm::vec3 &getUp() { return Up; };
    void setUp(glm::vec3 &Up) { this->Up = Up; cameraDetails.Up = &this->Up; };
    glm::vec3 &getRight() { return Right; };
    void setRight(glm::vec3 &Right) { this->Right = Right; cameraDetails.Right = &this->Right; };
    glm::vec3 &getWorldUp() { return WorldUp; };
    void setWorldUp(glm::vec3 &WorldUp) { this->WorldUp = WorldUp; cameraDetails.WorldUp = &this->WorldUp; };
    // euler Angles
    float getYaw() { return Yaw; };
    void setYaw(float Yaw) { this->Yaw = Yaw; cameraDetails.Yaw = &this->Yaw; };
    float getPitch() { return Pitch; };
    void setPitch(float Pitch) { this->Pitch = Pitch; cameraDetails.Pitch = &this->Pitch; };
    // camera options
    float getMovementSpeed() { return MovementSpeed; };
    void setMovementSpeed(float MovementSpeed) { this->MovementSpeed = MovementSpeed; cameraDetails.MovementSpeed = &this->MovementSpeed; };
    float getMouseSensitivity() { return MouseSensitivity; };
    void setMouseSensitivity(float MouseSensitivity) { this->MouseSensitivity = MouseSensitivity; cameraDetails.MouseSensitivity = &this->MouseSensitivity; };
    float getZoom() { return Zoom; };
    void setZoom(float Zoom) { this->Zoom = Zoom; cameraDetails.Zoom = &this->Zoom; };
    HWND getHWND() { return this->hWnd; };
    void setHWND(HWND hWnd) { this->hWnd = hWnd; }
};
#endif