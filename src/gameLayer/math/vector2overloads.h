#include <raylib.h>

// ── Vector2 operator overloads ───────────────────────────────────────────────

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

inline Vector2& operator+=(Vector2& a, float scalar)
{
	a.x += scalar;
	a.y += scalar;
	return a;
}

inline Vector2& operator-=(Vector2& a, float scalar)
{
	a.x -= scalar;
	a.y -= scalar;
	return a;
}

inline Vector2& operator*=(Vector2& a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	return a;
}

inline Vector2& operator/=(Vector2& a, float scalar)
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