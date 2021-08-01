#include "KDTree.h"
#include "model.h"
#include <iostream>

#ifndef COLLITION_DETECTION_H
#define COLLITION_DETECTION_H

extern bool SolveEquision(Node* A, Node* B, glm::vec3 L, glm::mat4 TRTB, glm::mat4 TRTA);
extern bool checkCollision(Node* A, Node* B, glm::mat4 TRTB, glm::mat4 TRTA);
extern void print_queue(std::queue<std::pair<Node*, Node*>> q);
extern std::pair<Node*, Node*> findCollision(Model* firstShape, Model* secondShape);

#endif