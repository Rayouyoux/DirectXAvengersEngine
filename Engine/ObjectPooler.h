#pragma once
#include <vector>
#include <algorithm>

namespace ave {
	namespace ObjectPooling {

#pragma region IPullable

		/// <summary>
		/// Interface to define an element as pullable.
		/// </summary>
		class IPullable {
		protected:
			bool m_bIsActive = false;

		public:
			int UID = 0;

			virtual void OnInstantiation() = 0;
			virtual void OnAcquire() = 0;
			virtual void OnRelease() = 0;

			virtual bool IsActive() const {
				return m_bIsActive;
			}

			virtual void SetActive(bool active) {
				m_bIsActive = active;
			}
		};

#pragma endregion

		class ObjectPool {
		protected:
			int m_iQuantity;
			std::vector<IPullable*> m_lPool;

		public:

		protected:
			ObjectPool();
			ObjectPool(const ObjectPool&) = delete;
			ObjectPool& operator=(const ObjectPool&) = delete;

			~ObjectPool();

			template<typename T>
			void PopulatePool() {
				for (int i = 0; i < m_iQuantity; i++) {
					m_lPool[i] = new T();
					m_lPool[i]->OnInstantiation();
				}
			}

			void ClearPool();

		public:
			static ObjectPool* Create();

			template<typename T>
			typename std::enable_if<std::is_base_of<IPullable, T>::value, void>::type
			Initialize(int iSize) {
				m_iQuantity = iSize;
				m_lPool.resize(iSize);
				PopulatePool<T>();
			}

			/// <summary>
			/// Acquire an inactive object from the pool if possible or the
			/// oldest active object.
			/// </summary>
			/// <typeparam name="T">Specify the expected return type.</typeparam>
			/// <returns>A newly active element from the pool of type specified T.</returns>
			template<typename T>
			T* AcquireObject() {
				IPullable* el = m_lPool[0];
				if (el->IsActive()) {
					el->SetActive(false);
					el->OnRelease();
				}
				el->SetActive(true);
				el->OnAcquire();
				auto it = m_lPool.begin();
				std::rotate(it, it + 1, m_lPool.end());
				return static_cast<T*>(el);
			}

			bool ReleaseObject(IPullable* obj);

			int CountAll();
			int CountActive();
			int CountInactive();

			void Release();
		};
	}
}