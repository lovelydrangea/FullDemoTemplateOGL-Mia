#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "Utilities.h"
#include "Animation.h"

class Animator {	
public:
    Animator();
    ~Animator();

    Animator(Animation* Animation);
	
    void UpdateAnimation(float dt);
	
    void PlayAnimation(Animation* pAnimation);
	
    void CalculateBoneTransform(const UTILITIES_OGL::AssimpNodeData* node, const glm::mat4& parentTransform);
	
    std::vector<glm::mat4>* GetFinalBoneMatrices() ;

    void setAnimation(Animation *Animation);

private:
    std::vector<glm::mat4> m_FinalBoneMatrices;
    Animation* m_CurrentAnimation;
    double m_CurrentTime;
    double m_DeltaTime;	
};
#endif