#pragma once

#include<forward_list>

class BaseCollider;

class CollisionManager {
public:
	static CollisionManager* GetInstance();

public://�����o�֐�

	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	inline void RemoveCollider(BaseCollider* collide) {
		colliders.remove(collide);
	}

	void CheckAllCollisions();

	
private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator = (const CollisionManager&) = delete;

	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*> colliders;

};