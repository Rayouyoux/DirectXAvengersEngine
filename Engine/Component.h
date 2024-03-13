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

		Entity* GetEntity();

		// Main components methods
		virtual void	Start() {};
		virtual void	Update(float deltaTime) {};
		virtual void	LateUpdate(float deltaTime) {};
		virtual void	Render() {};

		virtual void    OnCollisionEnter(Entity* collider) {};
		virtual void    OnCollisionStay(Entity* collider) {};
		virtual void    OnCollisionExit(Entity* collider) {};

	};
};
