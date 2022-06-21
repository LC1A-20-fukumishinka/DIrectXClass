#pragma once
#include "Object3D.h"
class TouchableObject :public Object3D
{
public:
	static TouchableObject *create(Model *model = nullptr);

	bool Initialize(Model *model);
};