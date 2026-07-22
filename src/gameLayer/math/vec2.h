#pragma once
#include <cstddef>      // std::size_t
#include <functional>   // std::hash
#include <random>

namespace Engine
{
	inline constexpr double Pi = 3.14159265358979323846264338327950288;
	inline constexpr double Deg2Rad = Pi / 180.0;
	inline constexpr double Rad2Deg = 180.0 / Pi;

	struct Vec2
	{
		float x;
		float y;
	};

	struct Vec2i
	{
		int x;
		int y;

		bool operator==(const Vec2i& other) const
		{
			return x == other.x && y == other.y;
		}

		Vec2i operator+(const Vec2i& other) const
		{
			return { x + other.x, y + other.y };
		}

		Vec2 toVec2() const
		{
			return Vec2{ (float)x, (float)y };
		}

		Vec2 center() const
		{
			return { x + 0.5f, y + 0.5f };
		}
	};

	struct Vec2iHash
	{
		std::size_t operator()(const Vec2i& v) const
		{
			std::size_t seed = 0;

			seed ^= std::hash<int>{}(v.x)
				+ 0x9e3779b9
				+ (seed << 6)
				+ (seed >> 2);

			seed ^= std::hash<int>{}(v.y)
				+ 0x9e3779b9
				+ (seed << 6)
				+ (seed >> 2);

			return seed;
		}
	};

	// ── Vec2 operator overloads ───────────────────────────────────────────────

	inline Vec2 operator+(const Vec2& a, const Vec2& b)
	{
		return Vec2{ a.x + b.x, a.y + b.y };
	}

	inline Vec2 operator-(const Vec2& a, const Vec2& b)
	{
		return Vec2{ a.x - b.x, a.y - b.y };
	}

	inline Vec2 operator*(const Vec2& a, float scalar)
	{
		return Vec2{ a.x * scalar, a.y * scalar };
	}

	inline Vec2 operator/(const Vec2& a, float scalar)
	{
		return Vec2{ a.x / scalar, a.y / scalar };
	}

	inline Vec2 operator+(const Vec2& a, float scalar)
	{
		return Vec2{ a.x + scalar, a.y + scalar };
	}

	inline Vec2& operator+=(Vec2& a, float scalar)
	{
		a.x += scalar;
		a.y += scalar;
		return a;
	}

	inline Vec2& operator-=(Vec2& a, float scalar)
	{
		a.x -= scalar;
		a.y -= scalar;
		return a;
	}

	inline Vec2& operator*=(Vec2& a, float scalar)
	{
		a.x *= scalar;
		a.y *= scalar;
		return a;
	}

	inline Vec2& operator/=(Vec2& a, float scalar)
	{
		a.x /= scalar;
		a.y /= scalar;
		return a;
	}

	inline bool operator==(const Vec2& a, const Vec2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline bool operator!=(const Vec2& a, const Vec2& b)
	{
		return !(a == b);
	}

	inline Vec2& operator+=(Vec2& a, const Vec2& b)
	{
		a.x += b.x;
		a.y += b.y;
		return a;
	}

	inline Vec2& operator-=(Vec2& a, const Vec2& b)
	{
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}

	inline Vec2& operator*=(Vec2& a, const Vec2& b)
	{
		a.x *= b.x;
		a.y *= b.y;
		return a;
	}

	inline Vec2& operator/=(Vec2& a, const Vec2& b)
	{
		a.x /= b.x;
		a.y /= b.y;
		return a;
	}

	inline Vec2& operator/(Vec2& a, const Vec2& b)
	{
		return Vec2
		{
			a.x / b.x,
			a.y / b.y
		};
	}

	inline Vec2 Vec2Normalize(Vec2 v)
	{
		Vec2 result = { 0 };
		float length = sqrtf((v.x * v.x) + (v.y * v.y));

		if (length > 0)
		{
			float ilength = 1.0f / length;
			result.x = v.x * ilength;
			result.y = v.y * ilength;
		}

		return result;
	}

	inline float Vec2Length(Vec2 v)
	{
		return sqrtf((v.x * v.x) + (v.y * v.y));
	}

	// Calculate distance between two vectors
	inline float Vec2Distance(Vec2 v1, Vec2 v2)
	{
		float result = sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

		return result;
	}

	// Calculate vector square length
	inline float Vec2LengthSqr(Vec2 v)
	{
		float result = (v.x * v.x) + (v.y * v.y);

		return result;
	}

	inline Vec2 Vec2Zero(void)
	{
		Vec2 result = { 0.0f, 0.0f };

		return result;
	}

	// Calculate linear interpolation between two vectors
	inline Vec2 Vec2Lerp(Vec2 v1, Vec2 v2, float amount)
	{
		Vec2 result = { 0 };

		result.x = v1.x + amount * (v2.x - v1.x);
		result.y = v1.y + amount * (v2.y - v1.y);

		return result;
	}

	// Scale vector (multiply by value)
	inline Vec2 Vec2Scale(Vec2 v, float scale)
	{
		Vec2 result = { v.x * scale, v.y * scale };

		return result;
	}

	// Calculate linear interpolation between two floats
	inline float Lerp(float start, float end, float amount)
	{
		float result = start + amount * (end - start);

		return result;
	}
}