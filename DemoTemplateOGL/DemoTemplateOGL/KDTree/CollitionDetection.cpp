#include "KDTree.h"
#include "CollitionDetection.h"
#include <iostream>
//https://github.com/Meirshomron/MeshCollision
struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    // Checks if two AABBs intersect
    bool intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
};


AABB computeTransformedAABB(const Node& node, const glm::mat4& transform) {
    glm::vec4 corners[8];
    glm::vec3 halfExtents(node.m_halfWidth, node.m_halfHeight, node.m_halfDepth);

    // Compute the 8 corners of the bounding box
    corners[0] = transform * glm::vec4(-halfExtents.x, -halfExtents.y, -halfExtents.z, 1.0f);
    corners[1] = transform * glm::vec4(-halfExtents.x, -halfExtents.y,  halfExtents.z, 1.0f);
    corners[2] = transform * glm::vec4(-halfExtents.x,  halfExtents.y, -halfExtents.z, 1.0f);
    corners[3] = transform * glm::vec4(-halfExtents.x,  halfExtents.y,  halfExtents.z, 1.0f);
    corners[4] = transform * glm::vec4( halfExtents.x, -halfExtents.y, -halfExtents.z, 1.0f);
    corners[5] = transform * glm::vec4( halfExtents.x, -halfExtents.y,  halfExtents.z, 1.0f);
    corners[6] = transform * glm::vec4( halfExtents.x,  halfExtents.y, -halfExtents.z, 1.0f);
    corners[7] = transform * glm::vec4( halfExtents.x,  halfExtents.y,  halfExtents.z, 1.0f);

    glm::vec3 min = glm::vec3(corners[0]);
    glm::vec3 max = min;

    for (int i = 1; i < 8; ++i) {
        glm::vec3 corner = glm::vec3(corners[i]);
        min = glm::min(min, corner);
        max = glm::max(max, corner);
    }

    return AABB(min, max);
}

bool checkAABBCollision(const Node& nodeA, const Node& nodeB, const glm::mat4& transformB, const glm::mat4& transformA) {
    AABB aabbA = computeTransformedAABB(nodeA, transformA);
    AABB aabbB = computeTransformedAABB(nodeB, transformB);
    return aabbA.intersects(aabbB);
}

bool SolveEquision(Node& A, Node& B, glm::vec3& L, glm::mat4& TRTB, glm::mat4& TRTA) {
	glm::vec4 centerB = TRTB * B.m_center;
	glm::vec4 centerA = TRTA * A.m_center;

	glm::vec3 T = glm::vec3(centerB.x - centerA.x, centerB.y - centerA.y, centerB.z - centerA.z);

	float p1 = glm::abs(glm::dot(A.m_halfWidth * A.m_axisX, L));
	float p2 = glm::abs(glm::dot(A.m_halfHeight * A.m_axisY, L));
	float p3 = glm::abs(glm::dot(A.m_halfDepth * A.m_axisZ, L));
	float p4 = glm::abs(glm::dot(B.m_halfWidth * B.m_axisX, L));
	float p5 = glm::abs(glm::dot(B.m_halfHeight * B.m_axisY, L));
	float p6 = glm::abs(glm::dot(B.m_halfDepth * B.m_axisZ, L));

	float leftSide = glm::abs(glm::dot(T, L));
	float rightSide = p1 + p2 + p3 + p4 + p5 + p6;

	if (leftSide > rightSide)
		return true;
	return false;
}

bool checkCollision(Node& A, Node& B, glm::mat4& TRTB, glm::mat4& TRTA) {
	bool collison = true;
	glm::vec3 L;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B.m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B.m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTB * glm::vec4(B.m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisX, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisY, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisX, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisY, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	L = glm::vec3(TRTA * glm::vec4(A.m_axisZ, 1)) * glm::vec3(TRTB * glm::vec4(B.m_axisZ, 1));
	if (SolveEquision(A, B, L, TRTB, TRTA))
		collison = false;

	return collison;
}

void print_queue(std::queue<std::pair<Node*, Node*>> q) {
	while (!q.empty())
	{
		std::cout << q.front().first->m_center.x << ", " << q.front().first->m_center.y << ", " << q.front().first->m_center.z << std::endl;
		std::cout << q.front().second->m_center.x << ", " << q.front().second->m_center.y << ", " << q.front().second->m_center.z << std::endl;

		q.pop();
	}
	std::cout << std::endl;
}

bool findCollision(std::pair<Node*, Node*>& collistionNodes, Node& firstShape, glm::mat4 firstTransScale, Node& secondShape, glm::mat4 secondTransScale) {
	std::queue<std::pair<Node*, Node*>> queue;
	bool ret = false;
	queue.push(std::pair<Node*, Node*>(&firstShape, &secondShape));

	while (!queue.empty()) {
		std::pair<Node*, Node*>& curPair = queue.front();
		queue.pop();
//		if (checkAABBCollision(*curPair.first, *curPair.second, secondTransScale, firstTransScale)) {
			if (checkCollision(*curPair.first, *curPair.second, secondTransScale, firstTransScale)) {
				if (curPair.first->isLastInnerNode() && curPair.second->isLastInnerNode()) {
					collistionNodes = curPair;
					ret = true;
					break;
				}
				else if (curPair.second->isLastInnerNode() && curPair.first != nullptr) {
					queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second));
					queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second));
				}
				else if (curPair.first->isLastInnerNode() && curPair.second != nullptr) {
					queue.push(std::pair<Node*, Node*>(curPair.first, curPair.second->m_left));
					queue.push(std::pair<Node*, Node*>(curPair.first, curPair.second->m_right));
				}
				else {
					queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second->m_left));
					queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second->m_left));
					queue.push(std::pair<Node*, Node*>(curPair.first->m_left, curPair.second->m_right));
					queue.push(std::pair<Node*, Node*>(curPair.first->m_right, curPair.second->m_right));
				}
			}
//		}
	}
	return ret;
}