#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Base/model.h"
#include "Projectile.h"
#include <vector>
#include "Base/Billboard.h"

class Player : public Model {
private:
    std::vector<Projectile*> projectiles; // Lista de proyectiles disparados por el jugador
    float shootCooldown;                 // Tiempo restante para poder disparar nuevamente
    const float maxCooldown = 0.5f;      // Tiempo mínimo entre disparos (en segundos)
    float health;                        // Vida del jugador

public:
    Player(const std::string& path, glm::vec3 position, Camera* camera)
        : Model(path, camera), shootCooldown(0.0f), health(100.0f) {
        setTranslate(&position); // Establece la posición inicial del jugador
    }

    // Actualiza el estado del jugador
    void update(float deltaTime) {
        if (shootCooldown > 0.0f) {
            shootCooldown -= deltaTime; // Reduce el cooldown de disparo con el tiempo
        }

        // Actualizar posición de los proyectiles
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            (*it)->updatePosition(deltaTime);

            // Si el proyectil está fuera de los límites, elimínalo
            if ((*it)->isOutOfBounds(100.0f)) {
                delete* it;
                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // Dibuja el jugador y sus proyectiles
    void draw(Shader& shader) {
        this->Draw(shader); // Dibuja el modelo del jugador

        for (auto& projectile : projectiles) {
            projectile->Draw(shader); // Dibuja cada proyectil
        }
    }

    // El jugador dispara un proyectil
    void shoot(glm::vec3 direction) {
        if (shootCooldown <= 0.0f) {
            glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f);
            glm::vec3 startPosition = *getTranslate() + glm::vec3(0.0f, 1.5f, 0.0f); // Justo frente al jugador
            Projectile* newProjectile = new Projectile("models/dragon_ball.gld", startPosition, direction, cameraDetails);
            newProjectile->setScale(&scale);
            projectiles.push_back(newProjectile);

            shootCooldown = maxCooldown; // Resetea el cooldown de disparo
        }
    }

    // Verifica si el jugador está colisionando con otro objeto
    bool checkCollision(Model& other) {
        return this->colisionaCon(other); // Usa el método heredado para detectar colisiones
    }

    // Maneja el daño recibido por el jugador
    void takeDamage(float amount) {
        health -= amount;
        if (health <= 0.0f) {
            std::cout << "¡El jugador ha sido derrotado!" << std::endl;
            // Manejar lógica de derrota
        }
    }

    // Obtiene la vida actual del jugador
    float getHealth() const {
        return health;
    }

    // Limpia los proyectiles cuando se destruye el jugador
    ~Player() {
        for (auto& projectile : projectiles) {
            delete projectile;
        }
        projectiles.clear();
    }
};

#endif
