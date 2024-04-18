#pragma once
#include "CommonEntity.h"

class Fish : public CommonEntity
{
public:
	Fish();
	std::pair<std::string, std::string> GetModelAndTexturePaths() const override;
};