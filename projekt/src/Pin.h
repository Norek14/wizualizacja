#pragma once
#include "CommonEntity.h"

class Pin : public CommonEntity
{
public:
	Pin();
	std::pair<std::string, std::string> GetModelAndTexturePaths() const override;

};