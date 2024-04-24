#pragma once

class BaseScene
{
public:
	BaseScene(){}
	virtual ~BaseScene(){}

	virtual void Init();
	virtual void Draw();
	virtual void Update();
	virtual void Release();

private:

};
