#include "Component.h"

void Component::SetOwner(std::shared_ptr<Actor> Owner)
{
	owner = Owner;
}
