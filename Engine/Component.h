#pragma once

namespace ave {
	
	class Entity;

	class Component 
	{

	private :

		Entity*			m_poEntity;

	public :

		// Constructor
		Component();

		// Deconstructor
		~Component();

		// Set Method
		void			BindEntity(Entity* poEntity);

		// Main components methods
		virtual void	Start() = 0;
		virtual void	Update(__int64 deltaTime) = 0;
		virtual void	LateUpdate(__int64 deltaTime) = 0;
		virtual void	Render() = 0;

	};
};
