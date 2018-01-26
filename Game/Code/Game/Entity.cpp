#include "Game/Entity.hpp"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Update(float deltaSeconds)
{
	deltaSeconds++;
}

void Entity::Render() const
{
}

void Entity::WrapAround()
{
}

void Entity::SetCenterForPhysicalAndLiberalRadii()
{
}

void Entity::SetRadiiForPhysicalAndLiberalRadii()
{
}
