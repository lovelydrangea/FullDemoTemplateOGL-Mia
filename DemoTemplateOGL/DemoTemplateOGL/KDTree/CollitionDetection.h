#include "KDTree.h"
#include <iostream>
// https://github.com/Meirshomron/MeshCollision
#ifndef COLLITION_DETECTION_H
#define COLLITION_DETECTION_H

extern bool SolveEquision(Node& A, Node& B, glm::vec3& L, glm::mat4& TRTB, glm::mat4& TRTA);
extern bool checkCollision(Node& A, Node& B, glm::mat4& TRTB, glm::mat4& TRTA);
extern void print_queue(std::queue<std::pair<Node*, Node*>> q);
extern bool findCollision(std::pair<Node*, Node*>& collistionNodes, Node& firstShape, glm::mat4 firstTransScale, Node& secondShape, glm::mat4 secondTransScale);

#endif