#pragma once
#include "UIComponent.h"
#include "Rect.h"

class ProgressBar :public UIComponent
{
protected:
	Rect *background;
	Rect *foreground;
	float percentage;
public:
	ProgressBar(ID3D11Device* d);
	~ProgressBar();
	void setBackgroundColor(float r, float g, float b, float a);
	void setForegroundColor(float r, float g, float b, float a);
	void setPercentage(float p);
	void setMaterial(Material *m);
	virtual void setPosition(float x, float y, float z) override;
	virtual void setRotation(float x, float y, float z) override;
	virtual void setScaling(float x, float y, float z) override;
	virtual void update(float deltaTime) override;
	virtual void draw(ID3D11DeviceContext* deviceContext, CXMMATRIX camView, CXMMATRIX camProjection) override;
};