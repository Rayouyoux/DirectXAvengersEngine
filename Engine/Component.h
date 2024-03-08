#pragma once

namespace ave {
	
	class Entity;

	class Component 
	{

	public :

		Entity*			m_poEntity;

	public :

		// Constructor
		Component();

		// Deconstructor
		virtual ~Component();

		// Set Method
		void			BindEntity(Entity* poEntity);

		// Main components methods
		virtual void	Start() = 0;
		virtual void	Update(float deltaTime) = 0;
		virtual void	LateUpdate(float deltaTime) = 0;
		virtual void	Render() = 0;

	};
};
