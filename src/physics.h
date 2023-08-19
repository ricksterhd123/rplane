// *Simple* abstraction of bullet physics
// phew
#include "bullet/btBulletDynamicsCommon.h"

class World {
    private:
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *dispatcher;
        btBroadphaseInterface *overlappingPairCache;
        btSequentialImpulseConstraintSolver *solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
        btAlignedObjectArray<btCollisionShape *> collisionShapes;
        btAlignedObjectArray<btRigidBody *> rigidBodies;
    public:
        World();
        ~World();

        btDiscreteDynamicsWorld* getDynamicsWorld();
        btRigidBody* addBox(btVector3 origin, btVector3 size, btScalar mass, btScalar restitution = 0);
        btRigidBody* addSphere(btVector3 origin, btScalar radius, btScalar mass, btScalar restitution = 0);
        void step(btScalar FPS);
};

World::World() {
    /// collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    this->collisionConfiguration = new btDefaultCollisionConfiguration();

    /// use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);

    /// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    this->overlappingPairCache = new btDbvtBroadphase();

    /// the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    this->solver = new btSequentialImpulseConstraintSolver;

    this->dynamicsWorld = new btDiscreteDynamicsWorld(
        this->dispatcher, 
        this->overlappingPairCache, 
        this->solver, 
        this->collisionConfiguration
    );

    this->dynamicsWorld->setGravity(btVector3(0, -10, 0));

    this->collisionShapes = btAlignedObjectArray<btCollisionShape *>();
    this->rigidBodies = btAlignedObjectArray<btRigidBody *>();
}

World::~World() {

}

btRigidBody* World::addBox(btVector3 origin, btVector3 size, btScalar mass, btScalar restitution)
{
    btCollisionShape *boxShape = new btBoxShape(size);

    this->collisionShapes.push_back(boxShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(origin);

    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        boxShape->calculateLocalInertia(mass, localInertia);

    // using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);

    rbInfo.m_restitution = restitution;

    btRigidBody* box = new btRigidBody(rbInfo);
    this->rigidBodies.push_back(box);
    this->dynamicsWorld->addRigidBody(box);

    return box;
}

btRigidBody* World::addSphere(btVector3 origin, btScalar radius, btScalar mass, btScalar restitution)
{
    btCollisionShape *sphereShape = new btSphereShape(radius);
    this->collisionShapes.push_back(sphereShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        sphereShape->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(origin);

    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, sphereShape, localInertia);

    rbInfo.m_restitution = restitution;

    btRigidBody* sphere = new btRigidBody(rbInfo);
    this->rigidBodies.push_back(sphere);
    this->dynamicsWorld->addRigidBody(sphere);

    return sphere;
}

void World::step(btScalar FPS) {
    this->dynamicsWorld->stepSimulation(1.f / FPS, 10);
}
