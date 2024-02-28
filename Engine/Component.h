#pragma once

namespace ave {
	
	class Entity;

	class Component {
	private :
		Entity* m_poEntity;

	public :

		Component();
		~Component();

		void BindEntity(Entity* poEntity);

		virtual void Start();
		virtual void Update();
		virtual void Render();
	};
};
