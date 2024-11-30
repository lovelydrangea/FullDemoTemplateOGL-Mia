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
    float attackDamage;  // Daño que inflige el monstruo
    bool reachedHouse;   // Indica si el monstruo llegó a la casa

public:
    // Constructor del monstruo
    Monster(const std::string& path, glm::vec3 position, Camera* camera, float speed = 1.0f, float attackDamage = 10.0f)
        : Model(path, camera), speed(speed), attackDamage(attackDamage), reachedHouse(false) {
        setTranslate(&position); // Establece la posición inicial del monstruo
    }

    // Mueve el monstruo hacia la casa
    void moveToHouse(const glm::vec3& housePosition, float deltaTime) {
        if (reachedHouse) return; // Si ya llegó a la casa, no se mueve

        glm::vec3 currentPosition = *getTranslate();

        // Calcula la dirección hacia la casa
        glm::vec3 direction = glm::normalize(housePosition - currentPosition);

        // Movimiento en X y Z
        currentPosition.x += direction.x * speed * deltaTime;
        currentPosition.z += direction.z * speed * deltaTime;

        // Actualiza la posición
        setTranslate(&currentPosition);
        setNextTranslate(&currentPosition);

        // Comprueba si el monstruo alcanzó la casa (margen de error pequeño)
        float distance = glm::distance(currentPosition, housePosition);
        if (distance < 1.0f) {
            reachedHouse = true;
            std::cout << "El monstruo ha alcanzado la casa." << std::endl;
        }
    }

    // Inflige daño a la casa
    void attackHouse(House house) {
        if (reachedHouse) {
            std::cout << "El monstruo inflige " << attackDamage << " de daño a la casa." << std::endl;
            house.takeDamage(getAttackDamage()); // Método `takeDamage` de la casa
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

