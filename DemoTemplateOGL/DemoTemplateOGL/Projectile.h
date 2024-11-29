#pragma once
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Base/model.h"

class Projectile : public Model {
private:
    glm::vec3 direction;
    float speed;

public:
    Projectile(const std::string& path, glm::vec3 startPosition, glm::vec3 dir, Camera* cam)
        : Model(path, cam), direction(glm::normalize(dir)), speed(10.0f) {
        setTranslate(&startPosition);
    }

    void updatePosition(float deltaTime) {
        glm::vec3 pos = *getTranslate();
        pos += direction * speed * deltaTime;
        setTranslate(&pos);
    }

    bool isOutOfBounds(float maxDistance) {
        return glm::length(*getTranslate()) > maxDistance;
    }
};

#endif
