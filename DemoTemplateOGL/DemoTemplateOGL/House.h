#pragma once
#ifndef HOUSE_H
#define HOUSE_H

#include "Base/model.h"
#include <iostream>

class House : public Model {
private:
    float health; // Vida de la casa

public:
    float life = getHealth();
    // Constructor de la casa
    House(const std::string& path, Camera* camera, float health = 500.0f)
        : Model(path, camera), health(health) {
         // Establece la posición inicial de la casa
    }

    // Maneja el daño recibido por la casa
    void takeDamage(float amount) {
         life-= amount;
        if (life <= 0.0f) {
            std::cout << "¡La casa ha sido destruida!" << std::endl;
            // Lógica adicional para la destrucción de la casa
        }
        else {
            std::cout << "La casa recibe " << amount << " de daño. Vida restante: " << health << std::endl;
        }
    }

    // Obtiene la vida actual de la casa
    float getHealth() const {
        return health;
    }

};

#endif
