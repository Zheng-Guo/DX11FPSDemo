#include "ProgressBar.h"

ProgressBar::ProgressBar(ID3D11Device * d) :UIComponent(d),
percentage(1.f)
{
	background = new Rect(device);
	foreground = new Rect(device);
}

ProgressBar::~ProgressBar()
{
	delete background;
	delete foreground;
}

void ProgressBar::setBackgroundColor(float r, float g, float b, float a)
{
	background->setColor(r, g, b, a);
}

void ProgressBar::setForegroundColor(float r, float g, float b, float a)
{
	foreground->setColor(r, g, b, a);
}

void ProgressBar::setPercentage(float p)
{
	percentage = p;
	foreground->setScale(p, 1.f, 1.f);
	foreground->setPosition(p - 1, 0.f, 0.f);
}

void ProgressBar::setMaterial(Material * m)
{
	background->setMaterial(m);
	foreground->setMaterial(m);
}

void ProgressBar::setPosition(float x, float y, float z)
{
	UIComponent::setPosition(x, y, z);
	background->setParentTransform(transform);
	foreground->setParentTransform(transform);
}

void ProgressBar::setRotation(float x, float y, float z)
{
	UIComponent::setRotation(x, y, z);
	background->setParentTransform(transform);
	foreground->setParentTransform(transform);
}

void ProgressBar::setScaling(float x, float y, float z)
{
	UIComponent::setScaling(x, y, z);
	background->setParentTransform(transform);
	foreground->setParentTransform(transform);
}

void ProgressBar::update(float deltaTime)
{

}

void ProgressBar::draw(ID3D11DeviceContext * deviceContext, CXMMATRIX camView, CXMMATRIX camProjection)
{
	background->draw(deviceContext, camView, camProjection);
	foreground->draw(deviceContext, camView, camProjection);
}
