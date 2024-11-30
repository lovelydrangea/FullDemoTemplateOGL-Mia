#pragma once
#ifndef MONSTER_H
#define MONSTER_H

#include "Base/model.h"
#include "House.h"
#include <iostream>

#include <glm/glm.hpp>

class Monster : public Model {

private:
    
    float speed;         // Velocidad del monstruo
    float attackDamage;  // Da�o que inflige el monstruo
    bool reachedHouse;   // Indica si el monstruo lleg� a la casa

public:
    // Constructor del monstruo
    Monster(const std::string& path, glm::vec3 position, Camera* camera, float speed = 1.0f, float attackDamage = 10.0f)
        : Model(path, camera), speed(speed), attackDamage(attackDamage), reachedHouse(false) {
        setTranslate(&position); // Establece la posici�n inicial del monstruo
    }

    // Mueve el monstruo hacia la casa
    void moveToHouse(const glm::vec3& housePosition, float deltaTime) {
        if (reachedHouse) return; // Si ya lleg� a la casa, no se mueve

        glm::vec3 currentPosition = *getTranslate();

        // Calcula la direcci�n hacia la casa
        glm::vec3 direction = glm::normalize(housePosition - currentPosition);

        // Movimiento en X y Z
        currentPosition.x += direction.x * speed * deltaTime;
        currentPosition.z += direction.z * speed * deltaTime;

        // Actualiza la posici�n
        setTranslate(&currentPosition);
        setNextTranslate(&currentPosition);

        // Comprueba si el monstruo alcanz� la casa (margen de error peque�o)
        float distance = glm::distance(currentPosition, housePosition);
        if (distance < 1.0f) {
            reachedHouse = true;
            std::cout << "El monstruo ha alcanzado la casa." << std::endl;
        }
    }

    // Inflige da�o a la casa
    void attackHouse(House house) {
        if (reachedHouse) {
            std::cout << "El monstruo inflige " << attackDamage << " de da�o a la casa." << std::endl;
            house.takeDamage(getAttackDamage()); // M�todo `takeDamage` de la casa
        }
    } 
    bool hasReachedHouse() const {
        return reachedHouse;
    }
    float getAttackDamage() {
        return attackDamage;
    }



    // Destructor del monstruo
    ~Monster() {
        std::cout << "Monstruo eliminado." << std::endl;
    }
};

#endif

