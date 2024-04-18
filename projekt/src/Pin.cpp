#include "Pin.h"

Pin::Pin() :
    CommonEntity(GetModelAndTexturePaths().first, GetModelAndTexturePaths().second)

{
}
std::pair<std::string, std::string> Pin::GetModelAndTexturePaths() const {
    return { "resources/models/10492_Bowling Pin_v1_max2011_iteration-2.obj", "resources/models/10492_Bowling-Pin_v1_diffuse.jpg" };
}