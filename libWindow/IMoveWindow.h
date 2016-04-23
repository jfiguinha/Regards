#pragma once
class IMoveWindow
{
public:
	virtual void SetNewPosition() = 0;
	virtual void UpdatePosition() = 0;
	virtual bool OnLButtonDown() = 0;
	virtual void OnLButtonUp() = 0;
};

