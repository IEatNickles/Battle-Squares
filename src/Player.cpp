#include "Player.hpp"
#include "Input.hpp"

#include "math/math.hpp"

#include "PhysicsBody.hpp"

#include "ParticleSystem.hpp"

#include <SDL.h>
#include <cmath>

Player::~Player() {
    delete m_WalkParticles;
    m_JumpSound.dispose();
    m_MoveSound.dispose();
    m_CollideSound.dispose();
}

Player::Player(ParticleSystem& ps) {
    m_JumpSound = Sound("assets/sounds/jump.wav");
    m_MoveSound = Sound("assets/sounds/move.wav");
    m_CollideSound = Sound("assets/sounds/collide.wav");

    m_WalkParticles = new ParticleEmitter(Vec2::zero(), 0.2f, 1.0f, 30);
    ps.addEmitter(m_WalkParticles);

    m_Body = new PhysicsBody(Vec2::zero(), Vec2::one(), false, PL_PLAYER);

    m_Speed = 7.0f;
    m_MaxJumps = 3;
    m_CurrentJumps = m_MaxJumps;
    m_JumpHeight = 2.0f;
}

void Player::update(Timestep ts, const Input& input, const Physics& physics) {
    RaycastHit hit;
    m_Grounded = physics.raycast(m_Body->getPosition(), Vec2(0.0f, -0.6f), hit, PL_PLAYER);
    if (m_Grounded && m_Body->getVelocity().y < 0.0f) {
        m_CurrentJumps = m_MaxJumps;
        m_CollideSound.play();
    }

    if (m_CurrentJumps > 0 && SDL_GetTicks() - m_LastJumpTime > 200 && input.isKeyDown(SDLK_w)) {
        m_JumpSound.play();
        m_Body->setVelocity(Vec2(m_Body->getVelocity().x, std::sqrt(2.0f * m_JumpHeight * 30.0f)));
        m_CurrentJumps -= 1;
        m_LastJumpTime = SDL_GetTicks();
        m_Grounded = false;
    }

    if (input.isKeyDown(SDLK_d) && !input.isKeyDown(SDLK_a)) {
        m_MoveDir = 1.0f;
    } else if (input.isKeyDown(SDLK_a) && !input.isKeyDown(SDLK_d)) {
        m_MoveDir = -1.0f;
    } else {
        m_MoveDir = 0.0f;
    }

    if (m_Grounded && std::abs(m_Body->getVelocity().x) > 0.5f) {
        if (SDL_GetTicks() - m_LastStepTime > 320) {
            m_MoveSound.play();
            m_LastStepTime = SDL_GetTicks();
        }
        m_WalkParticles->play();
    } else {
        m_WalkParticles->stop();
    }

    float x_velocity = math::lerp(m_Body->getVelocity().x, m_MoveDir * 7.0f, 10.0f * ts.delta);
    m_Body->setVelocity(Vec2(x_velocity, m_Body->getVelocity().y));

    float t = std::max(-1.0f, std::min(1.0f, -m_Body->getVelocity().y / 20.0f));
    Vec2 target_squish = Vec2(math::lerp(1.0f, 0.75f, t), math::lerp(1.0f, 1.5f, t));
    m_Squish.x = math::lerp(m_Squish.x, target_squish.x, ts.delta * 20.0f);
    m_Squish.y = math::lerp(m_Squish.y, target_squish.y, ts.delta * 20.0f);

    Transform& trns = m_Body->getTransform();
    trns.scale = m_Squish;

    float shear_factor = 1.0f;
    if (m_Body->getVelocity().y != 0.0f) {
        shear_factor = m_Body->getVelocity().y * 0.1f;
    }

    m_Shear = math::lerp(0.0f, 0.25f, shear_factor * (m_Body->getVelocity().x / 7.0f));
    trns.shear = Vec2(m_Shear, 0.0f);

    m_WalkParticles->setPosition(trns.position.add(Vec2(0.0f, -0.5f)));
}

PhysicsBody* Player::getBody() const {
    return m_Body;
}

// TEMP
void Player::setPos(const Vec2& pos) {
    m_Body->setPosition(pos);
    m_Body->setVelocity(Vec2::zero());
}

Mat4 Player::getMatrix() const {
    return m_Body->getTransform().getMatrix();
}
