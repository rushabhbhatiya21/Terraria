#pragma once
#include "raylib.h"
#include <raymath.h>
#include <cmath>
#include <nlohmann/json.hpp>
#include <math/vector2overloads.h>

using Json = nlohmann::json;

// ── Transform2D ──────────────────────────────────────────────────────────────

struct Transform2D
{
	Vector2 pos = {}; // center

	float w = 0; // width
	float h = 0; // height

	Vector2 getCenter()        const { return { pos.x, pos.y }; }
	Vector2 getTop()           const { return { pos.x, pos.y - h * 0.5f}; }
	Vector2 getBottom()        const { return { pos.x, pos.y + h * 0.5f }; }
	Vector2 getLeft()          const { return { pos.x - w * 0.5f, pos.y }; }
	Vector2 getRight()         const { return { pos.x + w * 0.5f, pos.y }; }
	Vector2 getTopLeft()       const { return { pos.x - w * 0.5f, pos.y - h * 0.5f }; }
	Vector2 getTopRight()      const { return { pos.x + w * 0.5f, pos.y - h * 0.5f }; }
	Vector2 getBottomLeft()    const { return { pos.x - w * 0.5f, pos.y + h * 0.5f }; }
	Vector2 getBottomRight()   const { return { pos.x + w * 0.5f, pos.y + h * 0.5f }; }

	// useful for rendering
	Rectangle getAABB()
	{
		return { pos.x - w * 0.5f, pos.y - h * 0.5f, w, h };
	}

	bool intersectPoint(Vector2 point, float delta = 0)
	{
		Rectangle aabb = getAABB();
		aabb.x -= delta;
		aabb.y -= delta;
		aabb.width += 2 * delta;
		aabb.height += 2 * delta;

		return CheckCollisionPointRec(point, aabb);
	}

	bool intersectTransform(Transform2D other, float delta = 0)
	{
		Rectangle a = getAABB();
		Rectangle b = other.getAABB();

		a.x -= delta;
		a.y -= delta;
		a.width += 2 * delta;
		a.height += 2 * delta;

		b.x -= delta;
		b.y -= delta;
		b.width += 2 * delta;
		b.height += 2 * delta;

		return CheckCollisionRecs(a, b);
	}
};

struct GameMap;

// ── PhysicalEntity ───────────────────────────────────────────────────────────

struct PhysicalEntity
{
	Transform2D transform;
	Vector2 lastPosition = {};

	Vector2 velocity = {};
	Vector2 acceleration = {};

	bool upTouch = 0;
	bool downTouch = 0;
	bool leftTouch = 0;
	bool rightTouch = 0;
	bool standingOnPlatform = 0;

	float dropThroughTimer = 0.f;

	// ── Jump state ───────────────────────────────────────────────────────────
	// All tunable constants are here at the top — tweak to taste.

	// Variable-height jump
	static constexpr float JUMP_FORCE = 12.f;  // initial velocity on jump
	static constexpr float JUMP_HOLD_MAX = 0.04f; // max seconds key can extend jump
	static constexpr float JUMP_HOLD_ACCEL = 14.f;  // extra upward accel while holding
	static constexpr float JUMP_CUT_FACTOR = 0.45f; // velocity multiplier on early release

	// Gravity multipliers
	static constexpr float GRAVITY_BASE = 20.f;
	static constexpr float GRAVITY_FALL_MULT = 1.9f;  // faster fall when descending
	static constexpr float GRAVITY_HOLD_MULT = 0.6f;  // lighter gravity while holding jump & rising

	// Terminal velocity (downward only)
	static constexpr float TERMINAL_VELOCITY = 28.f;

	// Coyote time
	static constexpr float COYOTE_TIME = 0.10f; // seconds after leaving ledge you can still jump

	// Jump buffer
	static constexpr float JUMP_BUFFER_TIME = 0.10f; // seconds before landing jump is pre-queued

	// Horizontal movement
	static constexpr float MOVE_SPEED = 9.f;
	static constexpr float GROUND_ACCEL = 70.f;
	static constexpr float GROUND_FRICTION = 70.f;
	static constexpr float AIR_ACCEL = 55.f;
	static constexpr float AIR_FRICTION = 12.f;  // low = preserve momentum in air

	// ── Runtime jump state (managed by updateJump) ───────────────────────────
	bool  jumpHeld = false; // is the jump key currently held
	bool  jumpConsumed = false; // have we used the jump for this airborne period
	float jumpHeldTime = 0.f;   // how long jump has been held this jump
	float coyoteTimer = 0.f;   // counts down after leaving ground
	float jumpBufferTimer = 0.f;   // counts down after pressing jump in air

	// ── Core physics ─────────────────────────────────────────────────────────
	bool isIgnoringPlatforms() const
	{
		return dropThroughTimer > 0.f;
	}

	void teleport(Vector2 pos)
	{
		transform.pos = pos;
		lastPosition = pos;
	}

	// Call once per frame BEFORE updateForces.
	// jumpKeyDown  = IsKeyDown(KEY_SPACE)
	// jumpKeyPress = IsKeyPressed(KEY_SPACE)  [true only on the frame of press]
	void updateJump(float dt, bool jumpKeyDown, bool jumpKeyPress)
	{
		// ── Coyote time: count down when in air ───────────────────────────────
		if (downTouch)
		{
			coyoteTimer = COYOTE_TIME;
			jumpConsumed = false; // reset on landing
		}
		else
		{
			coyoteTimer -= dt;
			if (coyoteTimer < 0.f) coyoteTimer = 0.f;
		}

		// ── Jump buffer: remember recent press ────────────────────────────────
		if (jumpKeyPress)
			jumpBufferTimer = JUMP_BUFFER_TIME;
		else
		{
			jumpBufferTimer -= dt;
			if (jumpBufferTimer < 0.f) jumpBufferTimer = 0.f;
		}

		// ── Fire jump: buffer consumed + (on ground OR within coyote window) ──
		bool canJump = (coyoteTimer > 0.f) && !jumpConsumed;
		if (jumpBufferTimer > 0.f && canJump)
		{
			velocity.y = -JUMP_FORCE;
			jumpHeldTime = 0.f;
			jumpConsumed = true;
			jumpBufferTimer = 0.f;
			coyoteTimer = 0.f;
		}

		// ── Variable height: extend jump while held and still rising ──────────
		jumpHeld = jumpKeyDown;
		if (jumpHeld && !downTouch && velocity.y < 0.f && jumpHeldTime < JUMP_HOLD_MAX)
		{
			acceleration.y -= JUMP_HOLD_ACCEL;
			jumpHeldTime += dt;
		}

		// ── Early release: cut velocity immediately ────────────────────────────
		if (!jumpHeld && velocity.y < 0.f)
		{
			velocity.y *= (1.f - JUMP_CUT_FACTOR * dt * 60.f);
			// clamp so we don't overshoot zero
			if (velocity.y > 0.f) velocity.y = 0.f;
		}
	}

	// Gravity with multipliers. Call once per frame BEFORE updateForces.
	void applyGravity()
	{
		float mult = 1.f;

		if (velocity.y > 0.f)
			mult = GRAVITY_FALL_MULT;                           // falling — heavier
		else if (jumpHeld && velocity.y < 0.f)
			mult = GRAVITY_HOLD_MULT;                           // rising + held — lighter

		acceleration.y += GRAVITY_BASE * mult;
	}

	// Horizontal movement with separate ground/air feel.
	// Call once per frame BEFORE updateForces.
	// inputX: -1 = left, 0 = none, +1 = right
	void applyHorizontalMovement(float dt, float inputX)
	{
		float accel = downTouch ? GROUND_ACCEL : AIR_ACCEL;
		float fric = downTouch ? GROUND_FRICTION : AIR_FRICTION;

		if (inputX != 0.f)
		{
			// Accelerate toward desired direction
			acceleration.x += inputX * accel;
		}
		else
		{
			// Friction — decelerate toward zero without overshooting
			float fricForce = fric * (velocity.x > 0.f ? -1.f : 1.f);
			if (std::abs(fricForce * dt) >= std::abs(velocity.x))
				velocity.x = 0.f;
			else
				acceleration.x += fricForce;
		}

		// Clamp horizontal speed
		velocity.x = Clamp(velocity.x, -MOVE_SPEED, MOVE_SPEED);
	}

	void updateForces(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		transform.pos += velocity * deltaTime;

		// Universal drag (air resistance)
		Vector2 dragVector = {
			velocity.x * std::abs(velocity.x),
			velocity.y * std::abs(velocity.y)
		};
		float drag = 0.01f;
		if (Vector2Length(dragVector) * drag * deltaTime > Vector2Length(velocity))
			velocity = {};
		else
			velocity -= dragVector * drag * deltaTime;

		if (Vector2Length(velocity) < 0.01f)
			velocity = {};

		// ── Terminal velocity (downward only) ─────────────────────────────────
		if (velocity.y > TERMINAL_VELOCITY)
			velocity.y = TERMINAL_VELOCITY;

		acceleration = {};
	}

	// called at the end of the frame
	void updateFinal()
	{
		lastPosition = { transform.pos.x, transform.pos.y };
	}

	// Kept for compatibility — prefer updateJump() instead.
	void jump(float force)
	{
		if (downTouch)
			velocity.y = -force;
	}

	Vector2& getPosition()
	{
		return transform.pos;
	}

	Json formatToJson()
	{
		Json j;
		j["posX"] = transform.pos.x;
		j["posY"] = transform.pos.y;
		j["velX"] = velocity.x;
		j["velY"] = velocity.y;

		return j;
	}

	bool loadFromJson(Json& j)
	{
		*this = {};

		if (!j.contains("posX") || !j["posX"].is_number())
			return false;
		transform.pos.x = j["posX"];

		if (!j.contains("posY") || !j["posY"].is_number())
			return false;
		transform.pos.y = j["posY"];

		if (j.contains("velX"))
		{
			if (j["velX"].is_number())
				velocity.x = j["velX"];
		}

		if (j.contains("velY"))
		{
			if (j["velY"].is_number())
				velocity.y = j["velY"];
		}

		lastPosition = transform.pos;
		return true;
	}

	// functions to resolve collisions
	void updatePhysicsTimers(float deltaTime);

	void resolveConstrains(GameMap& mapData);

	void checkCollisionOnce(GameMap& mapData, Vector2& pos);

	Vector2 performCollisionsOnOneAxis(GameMap& mapData, Vector2 pos, Vector2 delta);
};