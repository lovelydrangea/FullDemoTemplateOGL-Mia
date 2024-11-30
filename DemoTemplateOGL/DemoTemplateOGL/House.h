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
         // Establece la posici�n inicial de la casa
    }

    // Maneja el da�o recibido por la casa
    void takeDamage(float amount) {
         life-= amount;
        if (life <= 0.0f) {
            std::cout << "�La casa ha sido destruida!" << std::endl;
            // L�gica adicional para la destrucci�n de la casa
        }
        else {
            std::cout << "La casa recibe " << amount << " de da�o. Vida restante: " << health << std::endl;
        }
    }

    // Obtiene la vida actual de la casa
    float getHealth() const {
        return health;
    }

};

#endif
