#include "Fish.h"

Fish::Fish() :
    CommonEntity(GetModelAndTexturePaths().first, GetModelAndTexturePaths().second)
{
}

std::pair<std::string, std::string> Fish::GetModelAndTexturePaths() const {
    return { "resources/models/12265_Fish_v1_L2.obj", "resources/models/fish.jpg" };
}