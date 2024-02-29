#include "pch.h"

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

	void Component::Update(__int64 deltaTime) 
	{

	}

	void Component::LateUpdate(__int64 deltaTime)
	{

	}

	void Component::Render() 
	{

	}
};