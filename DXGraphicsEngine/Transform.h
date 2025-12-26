#pragma once

#include <DirectXMath.h>

class Transform
{
private:
	DirectX::XMVECTOR position{ 0, 0, 0 };
	DirectX::XMVECTOR rotation{ 0, 0, 0 };
	DirectX::XMVECTOR scale{ 1, 1, 1 };
public:
	DirectX::XMMATRIX GetWorldMatrix();

	DirectX::XMVECTOR GetPosition() { return position; }
	DirectX::XMVECTOR GetRotation() { return rotation; }
	DirectX::XMVECTOR GetScale() { return scale; }

	DirectX::XMVECTOR GetMin() { return DirectX::XMVectorAdd(position, DirectX::XMVectorScale(scale, -0.5)); } //subtract half of scale from position
	DirectX::XMVECTOR GetMax() { return DirectX::XMVectorAdd(position, DirectX::XMVectorScale(scale, 0.5)); } //add half of scale to position

	DirectX::XMVECTOR GetForward();
	DirectX::XMVECTOR GetRight();
	DirectX::XMVECTOR GetUp();

	void SetPosition(DirectX::XMVECTOR newPosition);
	void Translate(DirectX::XMVECTOR translation);

	void Rotate(DirectX::XMVECTOR inRotation);

	void SetScale(DirectX::XMVECTOR newScale);
};

