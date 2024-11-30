#pragma once
#ifndef MONSTER_H
#define MONSTER_H

#include "Base/model.h"
#include "House.h"
#include <iostream>
#include <Windows.h>
#include <glm/glm.hpp>
#include "Scenario.h"

using namespace std;
using namespace glm;

class Monster : public Model {

private:
    float life;
    Terreno* terreno;
    float speed;         // Velocidad del monstruo
    float attackDamage;  // Da�o que inflige el monstruo
    bool reachedHouse;   // Indica si el monstruo lleg� a la casa

public:
    // Constructor del monstruo
    Monster(const string& path, vec3 position, Camera* camera, Terreno* terrain, float speed = 0.4500001f, float attackDamage = 100.0f)
        : Model(path, camera), terreno(terrain), speed(speed), attackDamage(attackDamage), reachedHouse(false) {
        setTranslate(&position); // Establece la posici�n inicial del monstruo
    }

    

    // Mueve el monstruo hacia la casa
    void moveToHouse(const vec3& housePosition, float deltaTime) {
        if (reachedHouse) return; // Si ya lleg� a la casa, no se mueve

        vec3 currentPosition = *getTranslate();

        // Calcula la direcci�n hacia la casa (solo X y Z)
        vec3 direction = normalize(vec3(housePosition.x - currentPosition.x, 0.0f, housePosition.z - currentPosition.z));

        // Movimiento en X y Z
        currentPosition.x += direction.x * speed * deltaTime;
        currentPosition.z += direction.z * speed * deltaTime;

        // Ajustar la altura del monstruo seg�n el terreno
        currentPosition.y = terreno->Superficie(currentPosition.x, currentPosition.z)+3;

        // Actualiza la posici�n
        setTranslate(&currentPosition);

        // Comprueba si el monstruo alcanz� la casa (margen de error peque�o)
        float distanceToHouse = distance(vec3(currentPosition.x, 0.0f, currentPosition.z), vec3(housePosition.x, 0.0f, housePosition.z));
        if (distanceToHouse < 1.0f) {
            reachedHouse = true;
            cout << "El monstruo ha alcanzado la casa." << endl;
        }
    }

    void handleCollisionWithPlayer(vec3 playerPosition, float collisionRadius) {
        vec3 currentPosition = *getTranslate();
        float distanceToPlayer = distance(currentPosition, playerPosition);

        if (distanceToPlayer < collisionRadius) {
            cout << "El monstruo ha colisionado con el jugador." << endl;
            return; // Detener el movimiento si hay colisi�n
        }
    }

    // Maneja el da�o recibido por el monstruo
    void takeDamage(float amount) {
        life -= amount;
        if (life <= 0.0f) {
            std::cout << "�El monstruo ha sido destruido!" << std::endl;
           LOGGER::LOG().exclamation("Has destruido al monstruo. �Has ganado!");
           PostQuitMessage(0);// Aqu� puedes agregar la l�gica para eliminar al monstruo, por ejemplo:
              // Si el monstruo es un objeto din�mico, elim�nalo
        }
        else {
            std::cout << "El monstruo recibe " << amount << " de da�o. Vida restante: " << life << std::endl;
        }
    }

    bool hasReachedHouse() const {
        return reachedHouse;
    }
    float getAttackDamage() {
        return attackDamage;
    }
    float getSpeed() {
        return speed;
    }
    float getLife() {
        return life;
    }

    // Destructor del monstruo
    ~Monster() {
        std::cout << "Monstruo eliminado." << std::endl;
    }
};

#endif

