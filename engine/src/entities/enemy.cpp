#include "enemy.hpp"
#include "cylindrical.hpp"

#include "engine.hpp"

#include "player.hpp"

#include "loader.hpp"
#include "saver.hpp"
#include "utilities.hpp"

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
            chaseTarget = target->transform.transformMatrixNode->worldData.getTranslationVector();

            // look at target
            const Core::Maths::Vec3 delta = chaseTarget - transform.transformMatrixNode->worldData.getTranslationVector();
            if (delta.x != 0.f)
            {              
                transform.transform.rotation.y = std::atan2(delta.x, delta.z);

                transform.UpdateLocalTransformMatrix();
                transform.transformMatrixNode->setDirtySelfAndChildren();
            }
        }

        move(engine);
        tryToAttack(playTime);
    }
}

void Entity::Enemy::setResources(const DemoResourceManager& resourceManager)
{
    mesh.model   = &resourceManager.get(E_Model::E_DOG);
    mesh.shader  = &resourceManager.get(E_Shader::E_LIGHTED);
    mesh.texture = &resourceManager.get(E_Texture::E_DOG_TEXTURE);
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

    Core::Maths::Vec3 patrolDir = patrolTarget - transform.transformMatrixNode->worldData.getTranslationVector();
    patrolDir.y = 0.f;

    if (state.enemyState == EnemyState::E_CHASING)
    {
        Core::Maths::Vec3 firstPointOfCircle = patrolTarget - transform.transformMatrixNode->worldData.getTranslationVector();
        firstPointOfCircle.x += patrolRadius;
        firstPointOfCircle.y = 0;

        if(firstPointOfCircle.vectorSquareLength() > epsilonReturnPatrolDistanceToPoint * epsilonReturnPatrolDistanceToPoint)
        {
            // clamps
            if (firstPointOfCircle.vectorSquareLength() > maxSpeed * maxSpeed && firstPointOfCircle.vectorSquareLength() != 0.f)
            {
                firstPointOfCircle = firstPointOfCircle.unitVector() * maxSpeed;
            }
            // Core::Maths::Vec3 temp = firstPointOfCircle.unitVector();
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
        // Core::Maths::Cylindrical cylindricCoord = Core::Maths::Cylindrical::convertToCylindrical(position - patrolTarget);
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
    state.enemyState = EnemyState::E_CHASING;

    const Core::Maths::Vec3& loc = transform.transformMatrixNode->worldData.getTranslationVector();
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
    transform.transformMatrixNode->cleanUpdate();
}

bool Entity::Enemy::isTargetInChaseRange() const
{
    if (!transform.transformMatrixNode.isValid())
        return false;

    return (transform.transformMatrixNode->worldData.getTranslationVector() - chaseTarget).vectorSquareLength() 
        <= detectionRadius * detectionRadius;
}

bool Entity::Enemy::isTargetInAttackRange() const
{
    if (!transform.transformMatrixNode.isValid())
        return false;

    return (transform.transformMatrixNode->worldData.getTranslationVector() - chaseTarget).vectorSquareLength() 
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


// void Entity::Enemy::save(Save::Saver& saver)     
// {
//     saver.save(angle);

//     // Death Data
//     saver.save(isDead);
//     saver.save(timeLeftTillRespawn);
// }

// void Entity::Enemy::loadData(Save::Loader& loader) 
// {
//     loader.load(angle);

//     // Death Data
//     loader.load(isDead);
//     loader.load(timeLeftTillRespawn);

//     loader.tryToDisplayError(__FILE__);
// }

// void Entity::Enemy::onCollisionEnter(const SegmentHit& hit) 
// {
    
// }

// void Entity::Enemy::onCollisionExit() 
// {

// }

// void Entity::Enemy::onOverlapEnterSelfHit(const SegmentHit& hit) 
// {
//     if (hit.normal.y < -0.5)
//     {
//         kill();
//     }
// }

// void Entity::Enemy::onOverlapEnterAnotherHit(const SegmentHit& hit) 
// {    
//     if (hit.normal.y > 0.5)
//     {
//         kill();
//     }
// }

void Entity::Enemy::takeDamage(int damage, float playTime)
{
    lastReceivedHitTime = playTime;
    mesh.color = {1,0,0,1};
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
            lastAttackTime = playTime;
            target->dealDamages(1.f);
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
        mesh.color = mesh.color + (Core::Maths::Vec4{1,1,1,1} - mesh.color) * (playTime - lastReceivedHitTime);
    }
    else 
        mesh.color = {1,1,1,1};
}