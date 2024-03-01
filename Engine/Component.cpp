#include "pch.h"
#include "Component.h"
#include "Entity.h"

namespace ave {

	Component::Component() 
	{
		m_poEntity = nullptr;
	}

	
	Component::~Component() 
	{

	}

	void Component::BindEntity(Entity* poEntity) 
	{
		m_poEntity = poEntity;
	}

	void Component::Start() 
	{

	}

	void Component::Update(float deltaTime)
	{

	}

	void Component::LateUpdate(float deltaTime)
	{

	}

	void Component::Render() 
	{

	}
};