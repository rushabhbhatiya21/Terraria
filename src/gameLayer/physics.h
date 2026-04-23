#pragma once
#include "raylib.h"
#include <raymath.h>
#include <cmath>

// Vector2 operator overloads
inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return Vector2{ a.x + b.x, a.y + b.y };
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return Vector2{ a.x - b.x, a.y - b.y };
}

inline Vector2 operator*(const Vector2& a, float scalar)
{
	return Vector2{ a.x * scalar, a.y * scalar };
}

inline Vector2 operator/(const Vector2& a, float scalar)
{
	return Vector2{ a.x / scalar, a.y / scalar };
}

inline Vector2 &operator+=(Vector2& a, float scalar)
{
	a.x += scalar;
	a.y += scalar;
	return a;
}

inline Vector2 &operator-=(Vector2& a, float scalar)
{
	a.x -= scalar;
	a.y -= scalar;
	return a;
}

inline Vector2 &operator*=(Vector2& a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	return a;
}

inline Vector2 &operator/=(Vector2& a, float scalar)
{
	a.x /= scalar;
	a.y /= scalar;
	return a;
}

inline bool operator==(const Vector2& a, const Vector2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vector2& a, const Vector2& b)
{
	return !(a == b);
}

inline Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vector2& operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Vector2& operator*=(Vector2& a, const Vector2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

inline Vector2& operator/=(Vector2& a, const Vector2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

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

	void teleport(Vector2 pos)
	{
		transform.pos = pos;
		lastPosition = pos;
	}

	void updateForces(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		transform.pos += velocity * deltaTime;

		// Universal drag (air resistance / friction)
		Vector2 dragVector = Vector2{
			velocity.x * std::abs(velocity.x),
			velocity.y * std::abs(velocity.y)
		};

		float drag = 0.01f; // tweak

		if (Vector2Length(dragVector) * drag * deltaTime > Vector2Length(velocity))
		{
			velocity = {};
		}
		else
		{
			velocity -= dragVector * drag * deltaTime;
		}

		if (Vector2Length(velocity) < 0.01f)
		{
			velocity = {};
		}

		acceleration = {};
	}

	// called at the end of the frame
	void updateFinal()
	{
		lastPosition = { transform.pos.x, transform.pos.y };
	}

	void applyGravity()
	{
		acceleration += {0, 20.f};
	}

	void jump(float force)
	{
		// dont use acceleration because it is affected by deltaTime
		// directly change velocity because we want jump to happen in one frame
		if (downTouch)
		{
			velocity.y = -force;
		}
	}

	Vector2& getPosition()
	{
		return transform.pos;
	}

	// functions to resolve collisions
	void resolveConstrains(GameMap& mapData);

	void checkCollisionOnce(GameMap& mapData, Vector2& pos);

	Vector2 performCollisionsOnOneAxis(GameMap& mapData, Vector2 pos, Vector2 delta);
};