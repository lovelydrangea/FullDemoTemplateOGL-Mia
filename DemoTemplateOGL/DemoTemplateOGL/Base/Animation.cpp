#include "Animation.h"

Animation_Exception::Animation_Exception(){ }
const char* Animation_Exception::what(){
    return "Failed at load the animation sequence";
}

Animation::Animation() {};

Animation::Animation(std::string const& animationPath, std::unordered_map<std::string, BoneInfo>& boneInfoMap, int& boneCount) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (scene && scene->mRootNode && scene->mNumAnimations > 0) {
        aiAnimation* animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        ReadHeirarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBones(animation, boneInfoMap, boneCount);
    } else throw Animation_Exception();
}

Animation::~Animation() { }

Bone* Animation::FindBone(std::string const& name) {
    auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
        [&](Bone& bone) {
             return bone.GetBoneName().compare(name) == 0;
        }
    );
    if (iter == m_Bones.end()) return nullptr;
    else return &(*iter);
}

	
double Animation::GetTicksPerSecond() { return m_TicksPerSecond; }

double Animation::GetDuration() { return m_Duration;}

UTILITIES_OGL::AssimpNodeData& Animation::GetRootNode() { return m_RootNode; }

std::unordered_map<std::string,BoneInfo>& Animation::GetBoneIDMap() { 
    return m_BoneInfoMap;
}

void Animation::ReadMissingBones(const aiAnimation* animation, std::unordered_map<std::string, BoneInfo>& boneInfoMap, int& boneCount) {
    int size = animation->mNumChannels;

//    std::map<std::string, BoneInfo>& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
//    int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++) {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(UTILITIES_OGL::AssimpNodeData& dest, const aiNode* src) {
    assert(src);

    dest.name = src->mName.data;
    aiMatrix4x4 mat = src->mTransformation;
    dest.transformation = UTILITIES_OGL::aiMatrix4x4ToGlm(mat);
    dest.childrenCount = src->mNumChildren;
    dest.children.reserve(src->mNumChildren);
    for (unsigned int i = 0; i < src->mNumChildren; i++) {
        UTILITIES_OGL::AssimpNodeData newData;
        ReadHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}
