#include "enemy.hpp"
#include "cylindrical.hpp"

#include "engine.hpp"

#include "loader.hpp"
#include "saver.hpp"
#include "utilities.hpp"

void Entity::Enemy::update(const Core::Engine& engine)
{   
    if (isDead)
    {
        timeLeftTillRespawn -= engine.deltaTime;
        if (timeLeftTillRespawn <= 0.f)
        {
            isDead = false;
            timeLeftTillRespawn = 0.f;

            collider.isEnabled = true;
            physicComp.isEnabled = true;
            mesh.isDrawn = true;
        }
    }
    else 
    {
        move(engine);
    }
}

void Entity::Enemy::move(const Core::Engine& engine)
{
    if(state.enemyState != EnemyState::E_FALLING)
    {
        if(isPlayerInRange())
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

        v.x = cos(angle + engine.deltaTime * speed / patrolRadius);
        v.y = 0;
        v.z = sin(angle + engine.deltaTime * speed / patrolRadius);

        v *= patrolRadius;

        v += patrolTarget;
        
        float f = physicComp.velocity.y;

        physicComp.velocity = v - transform.transform.location;
        physicComp.velocity.y = f;

        angle += engine.deltaTime * speed / patrolRadius;
    }
}

void Entity::Enemy::chase(const Core::Engine& engine)
{
    state.enemyState = EnemyState::E_CHASING;

    const Core::Maths::Vec3 loc = transform.transformMatrixNode->worldData.getTranslationVector();
    Core::Maths::Vec3 direction = (chaseTarget - loc).unitVector();
    Core::Maths::Vec3 velocityXZ { physicComp.velocity.x, 0, physicComp.velocity.z };

    velocityXZ += direction * engine.deltaTime;

    if (velocityXZ.vectorSquareLength() > maxSpeed * maxSpeed && velocityXZ.vectorSquareLength() != 0.f)
    {
        velocityXZ = velocityXZ.unitVector() * maxSpeed;
    }

    physicComp.velocity.x = velocityXZ.x;
    physicComp.velocity.z = velocityXZ.z;
    // Core::Maths::Vec3 direction = (position - chaseTarget).unitVector();
    // mesh->transform.transform.location += direction;
    transform.UpdateLocalTransformMatrix();
    transform.transformMatrixNode->cleanUpdate();
}

bool Entity::Enemy::isPlayerInRange() const
{
    if (!transform.transformMatrixNode.isValid())
        return false;

    return (transform.transformMatrixNode->worldData.getTranslationVector() - chaseTarget).vectorSquareLength() 
        <= detectionRadius * detectionRadius;
}

void Entity::Enemy::kill()
{
    timeLeftTillRespawn = respawnCooldown;
    isDead = true;

    collider.isEnabled = false;
    physicComp.isEnabled = false;

    mesh.isDrawn = false;
}


void Entity::Enemy::save(Save::Saver& saver)     
{
    BasicEntity::save(saver);
    saver.save(angle);

    // Death Data
    saver.save(isDead);
    saver.save(timeLeftTillRespawn);
}

void Entity::Enemy::loadData(Save::Loader& loader) 
{
    BasicEntity::loadData(loader);
    loader.load(angle);

    // Death Data
    loader.load(isDead);
    loader.load(timeLeftTillRespawn);

    loader.tryToDisplayError(__FILE__);
}

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

void Entity::Enemy::takeDamage(int damage)
{
    life = clamp(life - damage, 0, life);
    if(life == 0)
        kill();
}