#include "enemy.hpp"
#include "cylindrical.hpp"

#include "engine.hpp"

#include "player.hpp"

#include "loader.hpp"
#include "saver.hpp"
#include "utilities.hpp"

#include "layersEnum.hpp"

constexpr Core::Maths::Vec4 Entity::Enemy::colorTakenOnDamage;
constexpr Core::Maths::Vec4 Entity::Enemy::defaultColor;

Entity::Enemy::Enemy() 
    : Physics::CollisionComponentInterface<Physics::Shapes::Box>(&transform),
        Physics::PhysicComponentInterface(&transform),
        Renderer::RenderableInterface(&transform)
{
    collider.isOverlap = true;
    physicComp.collider.worldCollider.radius = defaultPhysicCompRadius;
    collider.layers = defaultLayers;
}

void Entity::Enemy::update(const Core::Engine& engine, float playTime)
{   
    if (state.enemyState == EnemyState::E_DEAD)
    {
        timeLeftTillRespawn -= engine.deltaTime;
        tryToRespawn();
    }
    else 
    {
        lerpColorBackToNormal(playTime);

        if (target != nullptr)
        {
            chaseTarget = target->transform.transformMatrixNode->getWorldMatrix().getTranslationVector();

            // look at target
            const Core::Maths::Vec3 delta = chaseTarget - transform.transformMatrixNode->getWorldMatrix().getTranslationVector();
            if (delta.x != 0.f)
            {              
                transform.transform.rotation.y = std::atan2(delta.x, delta.z);

                transform.UpdateLocalTransformMatrix();
            }
        }

        move(engine);
        tryToAttack(playTime);
    }
}

void Entity::Enemy::setResources(const DemoResourceManager& resourceManager)
{
    if (type.enemyType == EnemyType::E_NORMAL)
    {
        mesh.model   = &resourceManager.get(defaultModel);
        mesh.texture = &resourceManager.get(defaultTexture);
        attackSound.setAudio(resourceManager.get(defaultAttackSound));
    }
    else if (type.enemyType == EnemyType::E_BOSS)
    {
        mesh.model   = &resourceManager.get(bossDefaultModel);
        mesh.texture = &resourceManager.get(bossDefaultTexture);
        attackSound.setAudio(resourceManager.get(bossDefaultAttackSound));
    }
    mesh.shader  = &resourceManager.get(defaultShader); 
    mesh.linkShaderWithModel();
}

void Entity::Enemy::move(const Core::Engine& engine)
{
    if(state.enemyState != EnemyState::E_FALLING)
    {
        if(isTargetInChaseRange())
            chase(engine);
        else
        {
            patrol(engine);
        }
    }
}

void Entity::Enemy::patrol(const Core::Engine& engine)
{
    if (!transform.transformMatrixNode.isValid())
        return;

    Core::Maths::Vec3 patrolDir = patrolTarget - transform.transformMatrixNode->getWorldMatrix().getTranslationVector();
    patrolDir.y = 0.f;

    if (state.enemyState == EnemyState::E_CHASING)
    {
        Core::Maths::Vec3 firstPointOfCircle = patrolTarget - transform.transformMatrixNode->getWorldMatrix().getTranslationVector();
        firstPointOfCircle.x += patrolRadius;
        firstPointOfCircle.y = 0;

        if(firstPointOfCircle.vectorSquareLength() > epsilonReturnPatrolDistanceToPoint * epsilonReturnPatrolDistanceToPoint)
        {
            // clamps
            if (firstPointOfCircle.vectorSquareLength() > maxSpeed * maxSpeed && firstPointOfCircle.vectorSquareLength() != 0.f)
            {
                firstPointOfCircle = firstPointOfCircle.unitVector() * maxSpeed;
            }

            physicComp.velocity.x = firstPointOfCircle.x;
            physicComp.velocity.z = firstPointOfCircle.z;        
        }
        else
        {
            state.enemyState = EnemyState::E_PATROLLING;
            angle = 0.f;
        }
    }
    else if (state.enemyState == EnemyState::E_PATROLLING)
    {
        Core::Maths::Vec3 v;

        v.x = cos(angle);
        v.y = 0;
        v.z = sin(angle);

        v *= patrolRadius;

        v += patrolTarget;
        
        float f = physicComp.velocity.y;

        physicComp.velocity = v - transform.transform.location;
        physicComp.velocity.y = f;

        angle += speed / patrolRadius * engine.deltaTime;
    }
}

void Entity::Enemy::chase(const Core::Engine& engine)
{
    if(type.enemyType == EnemyType::E_NORMAL)
    {
        state.enemyState = EnemyState::E_CHASING;

        const Core::Maths::Vec3& loc = transform.transformMatrixNode->getWorldMatrix().getTranslationVector();
        Core::Maths::Vec3 direction = (chaseTarget - loc).unitVector();
        Core::Maths::Vec3 velocityXZ { physicComp.velocity.x, 0, physicComp.velocity.z };
        velocityXZ += direction * speed;

        if (velocityXZ.vectorSquareLength() > maxSpeed * maxSpeed && velocityXZ.vectorSquareLength() != 0.f)
        {
            velocityXZ = velocityXZ.unitVector() * maxSpeed;
        }

        physicComp.setVelocityOnAxis<0>(velocityXZ.x, engine);
        physicComp.setVelocityOnAxis<2>(velocityXZ.z, engine);
        transform.UpdateLocalTransformMatrix();
    }
    else if(type.enemyType == EnemyType::E_BOSS)
    {
        if(timeForHit < timeBetweenHitBoss)
        {
            timeForHit += engine.deltaTime;
        }
        else
        {   
            attackSound.play();
            timeForHit = 0.0f;
            target->dealDamages(bossDamages);
        }
    }
}

bool Entity::Enemy::isTargetInChaseRange() const
{
    if (!transform.transformMatrixNode.isValid())
        return false;

    return (transform.transformMatrixNode->getWorldMatrix().getTranslationVector() - chaseTarget).vectorSquareLength() 
        <= detectionRadius * detectionRadius;
}

bool Entity::Enemy::isTargetInAttackRange() const
{
    if (!transform.transformMatrixNode.isValid())
        return false;

    return (transform.transformMatrixNode->getWorldMatrix().getTranslationVector() - chaseTarget).vectorSquareLength() 
        <= attackRadius * attackRadius;
}

void Entity::Enemy::kill()
{
    timeLeftTillRespawn = respawnCooldown;
    state.enemyState = EnemyState::E_DEAD;

    collider.isEnabled = false;
    physicComp.isEnabled = false;

    mesh.isDrawn = false;
}

void Entity::Enemy::takeDamage(int damage, float playTime)
{
    lastReceivedHitTime = playTime;
    mesh.color = colorTakenOnDamage;
    life = clamp(life - damage, 0, life);

    if(life == 0)
        kill();
}

void Entity::Enemy::tryToAttack(float playTime)
{
    // attack player
    if (playTime > lastAttackTime + attackCooldown && target != nullptr)
    {
        if (isTargetInAttackRange())
        {
            attackSound.play();
            lastAttackTime = playTime;
            target->dealDamages(damages);
        }
    }
}

void Entity::Enemy::tryToRespawn()
{
    if (timeLeftTillRespawn <= 0.f)
    {
        state.enemyState = EnemyState::E_PATROLLING; 
        timeLeftTillRespawn = 0.f;

        collider.isEnabled = true;
        physicComp.isEnabled = true;
        mesh.isDrawn = true;
    }
}

void Entity::Enemy::lerpColorBackToNormal(float playTime)
{
    // Lerp color when hit
    if ((playTime - lastReceivedHitTime) < 1)
    {
        mesh.color = mesh.color + (defaultColor - mesh.color) * (playTime - lastReceivedHitTime);
    }
    else 
        mesh.color = defaultColor;
}

 void Entity::Enemy::physicCompOnOverlapEnter(const Physics::PhysicsSystem::CollisionsCallbacksSentData& data)
{
    if (data.encounteredEntityID == this)
        return;

    //kill when player jump over the enemy
    if (data.hit.normal.y < - jumpKilledHeight)
        kill();
}

void Entity::Enemy::colliderOnOverlapEnter(const Physics::PhysicsSystem::CollisionsCallbacksSentData& data)
{
    if (data.movingEntityID == this)
        return;

    if (data.hit.normal.y > jumpKilledHeight)
    {
        kill();
    }
}