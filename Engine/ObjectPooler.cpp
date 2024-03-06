#include <algorithm>
#include "pch.h"
#include "ObjectPooler.h"

namespace ave {
	namespace ObjectPooling {
		ObjectPool::ObjectPool() {
			m_iQuantity = 0;
			m_lPool.clear();
		}

		ObjectPool::~ObjectPool() {
			ClearPool();
		}

		ObjectPool* ObjectPool::Create() {
			return new ObjectPool();
		}

		void ObjectPool::ClearPool() {
			for (int i = 0; i < m_lPool.size(); i++) {
				delete m_lPool[i];
			}
		}

		/// <summary>
		/// Release the given object, placing it at the begining of the pool
		/// and ready to be acquired once more.
		/// </summary>
		bool ObjectPool::ReleaseObject(IPullable* obj) {
			auto it = std::find(m_lPool.begin(), m_lPool.end(), obj);

			if (it != m_lPool.end()) {
				(*it)->SetActive(false);
				(*it)->OnRelease();

				std::rotate(m_lPool.begin(), it, it + 1);
				return true;
			}

			return false;
		}

		int ObjectPool::CountAll() {
			return m_lPool.size();
		}

		int ObjectPool::CountActive() {
			auto isActive = [](IPullable* p) { return p->IsActive(); };
			return std::count_if(m_lPool.begin(), m_lPool.end(), isActive);
		}

		int ObjectPool::CountInactive() {
			auto isInactive = [](IPullable* p) { return !p->IsActive(); };
			int count = std::count_if(m_lPool.begin(), m_lPool.end(), isInactive);
			return std::count_if(m_lPool.begin(), m_lPool.end(), isInactive);
		}

		void ObjectPool::Release() {
			delete this;
		}
	}
}
