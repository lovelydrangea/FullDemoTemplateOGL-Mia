#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Base/model.h"
#include "Monster.h"
#include <iostream>

class Player : public Model {
private:
    float health; // Vida del jugador
    float attackDamage; // Da�o que el jugador inflige al atacar

public:
    // Constructor del jugador
    Player(const std::string& path, glm::vec3 position, Camera* camera, float attackDamage = 10.0f)
        : Model(path, camera), health(100.0f), attackDamage(attackDamage) {
        setTranslate(&position); // Establece la posici�n inicial del jugador
    }

    // Actualiza el estado del jugador
    void update(float deltaTime) {
        // Aqu� puedes agregar cualquier l�gica de actualizaci�n necesaria (animaciones, etc.)
    }

    // Verifica si el jugador est� colisionando con otro objeto
    bool checkCollision(Model& other) {
        if (this->colisionaCon(other)) {
            std::cout << "Colisi�n detectada con otro objeto. Atacando..." << std::endl;
            return true;
        }
        return false;
    }

    // Aplica da�o al jugador
    void takeDamage(float amount) {
        health -= amount;
        if (health <= 0.0f) {
            std::cout << "�El jugador ha sido derrotado!" << std::endl;
            // L�gica de derrota del jugador
        }
    }

    // Aplica da�o a otro modelo (ejemplo, un monstruo)
    void attack(Monster monster) {
        // Aqu� podr�as implementar c�mo el ataque afecta al objetivo
        std::cout << "El jugador inflige " << attackDamage << " de da�o al objetivo." << std::endl;
    }

    // Obtiene la vida actual del jugador
    float getHealth() const {
        return health;
    }
};

#endif
