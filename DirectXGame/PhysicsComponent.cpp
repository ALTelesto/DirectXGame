#include "PhysicsComponent.h"

#include "AComponent.h"
#include "BaseComponentSystem.h"
#include "AGameObject.h"
#include "LogUtils.h"

#include "Vector3D.h"

PhysicsComponent::PhysicsComponent(std::string name, AGameObject* owner) : AComponent(name,AComponent::ComponentType::Physics, owner)
{
	BaseComponentSystem::getInstance()->getPhysicsSystem()->registerComponent(this);
	PhysicsCommon* physicsCommon = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsCommon();
	PhysicsWorld* physicsWorld = BaseComponentSystem::getInstance()->getPhysicsSystem()->getPhysicsWorld();

	Vector3D scale = this->getOwner()->getLocalScale();
	Transform transform; //transform.setFromOpenGL(this->getOwner()->getPhysicsLocalMatrix());
	Vector3D position = this->getOwner()->getLocalPosition();
	transform.setPosition(Vector3(position.x,position.y, position.z));
	Quaternion quat; quat.setToIdentity();
	transform.setOrientation(quat);
	LogUtils::log(this, "Creating box shape");
	BoxShape* boxShape = physicsCommon->createBoxShape(Vector3(scale.x / 2, scale.y / 2, scale.z / 2));

	LogUtils::log(this, "Creating rigidbody");
	this->rigidBody = physicsWorld->createRigidBody(transform);
	LogUtils::log(this, "Adding collider");
	this->rigidBody->addCollider(boxShape, transform);
	this->rigidBody->updateMassPropertiesFromColliders();
	this->rigidBody->setMass(this->mass);
	this->rigidBody->setType(BodyType::DYNAMIC);

	LogUtils::log(this, "Creating getOpenGLMatrix");
	transform = this->rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	LogUtils::log(this, "Second jank call of local matrix");
	this->getOwner()->setPhysicsLocalMatrix(matrix);
}

PhysicsComponent::~PhysicsComponent()
{
	AComponent::~AComponent();
	BaseComponentSystem::getInstance()->getPhysicsSystem()->unregisterComponent(this);
}

void PhysicsComponent::setTransform()
{
	if (rigidBody == nullptr) return;

	Transform transform; //transform.setFromOpenGL(this->getOwner()->getPhysicsLocalMatrix());
	Vector3D position = this->getOwner()->getLocalPosition();
	transform.setPosition(Vector3(position.x, position.y, position.z));
	Quaternion quat; quat.setToIdentity();
	transform.setOrientation(quat);

	rigidBody->setTransform(transform);
}


void PhysicsComponent::perform(float deltaTime)
{
	const Transform transform = this->rigidBody->getTransform();
	float matrix[16];
	transform.getOpenGLMatrix(matrix);

	this->getOwner()->setPhysicsLocalMatrix(matrix);
}

RigidBody* PhysicsComponent::getRigidBody()
{
	return this->rigidBody;
}
