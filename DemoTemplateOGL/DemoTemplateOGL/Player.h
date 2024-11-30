#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Base/model.h"
#include "Monster.h"
#include <iostream>

class Player : public Model {
private:
    float health; // Vida del jugador
    float attackDamage; // Daño que el jugador inflige al atacar

public:
    // Constructor del jugador
    Player(const std::string& path, glm::vec3 position, Camera* camera, float attackDamage = 10.0f)
        : Model(path, camera), health(100.0f), attackDamage(attackDamage) {
        setTranslate(&position); // Establece la posición inicial del jugador
    }

    // Actualiza el estado del jugador
    void update(float deltaTime) {
        // Aquí puedes agregar cualquier lógica de actualización necesaria (animaciones, etc.)
    }

    // Verifica si el jugador está colisionando con otro objeto
    bool checkCollision(Model& other) {
        if (this->colisionaCon(other)) {
            std::cout << "Colisión detectada con otro objeto. Atacando..." << std::endl;
            return true;
        }
        return false;
    }

    // Aplica daño al jugador
    void takeDamage(float amount) {
        health -= amount;
        if (health <= 0.0f) {
            std::cout << "¡El jugador ha sido derrotado!" << std::endl;
            // Lógica de derrota del jugador
        }
    }

    // Aplica daño a otro modelo (ejemplo, un monstruo)
    void attack(std::vector<Model*>& models, float attackDamage, float attackRadius) {
        glm::vec3 playerPosition = *getTranslate();

        for (auto it = models.begin(); it != models.end(); ) {
            Monster* monster = dynamic_cast<Monster*>(*it);
            if (monster) {
                glm::vec3 monsterPosition = *monster->getTranslate();
                float distanceToMonster = glm::distance(playerPosition, monsterPosition);

                if (distanceToMonster <= attackRadius) {
                    // Aplica daño al monstruo
                    std::cout << "El jugador inflige " << attackDamage << " de daño al monstruo." << std::endl;
                    monster->takeDamage(attackDamage);

                    // Elimina el monstruo si su vida es 0
                    if (monster->getLife() <= 0.0f) {
                        std::cout << "¡El monstruo ha sido eliminado!" << std::endl;
                        delete* it;  // Elimina el objeto del heap
                        it = models.erase(it);  // Borra el puntero del vector
                        continue;  // Pasa al siguiente modelo
                    }
                }
            }
            ++it;  // Avanza el iterador
        }
    }


    // Obtiene la vida actual del jugador
    float getHealth() const {
        return health;
    }
};

#endif
