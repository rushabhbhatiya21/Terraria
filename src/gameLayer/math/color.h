#pragma once

namespace Engine
{
	struct Color4f
	{
		unsigned char r;        // Color red value
		unsigned char g;        // Color green value
		unsigned char b;        // Color blue value
		unsigned char a;        // Color alpha value
	};

	// Get color with alpha applied, alpha goes from 0.0f to 1.0f
	inline Color4f colorAlpha(Color4f color, float alpha)
	{
		if (alpha < 0.0f) alpha = 0.0f;
		else if (alpha > 1.0f) alpha = 1.0f;

		return Color4f { color.r, color.g, color.b, (unsigned char)(255.0f * alpha) };
	}

	// Some Basic Colors
	// NOTE: Custom raylib color palette for amazing visuals on WHITE background
	inline constexpr Color4f LightGray  { 200, 200, 200, 255 };   // Light Gray
	inline constexpr Color4f Gray       { 130, 130, 130, 255 };   // Gray
	inline constexpr Color4f DarkGray   { 80, 80, 80, 255 };      // Dark Gray
	inline constexpr Color4f Yellow     { 253, 249, 0, 255 };     // Yellow
	inline constexpr Color4f Gold       { 255, 203, 0, 255 };     // Gold
	inline constexpr Color4f Orange     { 255, 161, 0, 255 };     // Orange
	inline constexpr Color4f Pink       { 255, 109, 194, 255 };   // Pink
	inline constexpr Color4f Red        { 230, 41, 55, 255 };     // Red
	inline constexpr Color4f Maroon     { 190, 33, 55, 255 };     // Maroon
	inline constexpr Color4f Green      { 0, 228, 48, 255 };      // Green
	inline constexpr Color4f Lime       { 0, 158, 47, 255 };      // Lime
	inline constexpr Color4f DarkGreen  { 0, 117, 44, 255 };      // Dark Green
	inline constexpr Color4f SkyBlue    { 102, 191, 255, 255 };   // Sky Blue
	inline constexpr Color4f Blue       { 0, 121, 241, 255 };     // Blue
	inline constexpr Color4f DarkBlue   { 0, 82, 172, 255 };      // Dark Blue
	inline constexpr Color4f Purple     { 200, 122, 255, 255 };   // Purple
	inline constexpr Color4f Violet     { 135, 60, 190, 255 };    // Violet
	inline constexpr Color4f DarkPurple { 112, 31, 126, 255 };    // Dark Purple
	inline constexpr Color4f Beige      { 211, 176, 131, 255 };   // Beige
	inline constexpr Color4f Brown      { 127, 106, 79, 255 };    // Brown
	inline constexpr Color4f DarkBrown  { 76, 63, 47, 255 };      // Dark Brown

	inline constexpr Color4f White      { 255, 255, 255, 255 };   // White
	inline constexpr Color4f Black      { 0, 0, 0, 255 };         // Black
	inline constexpr Color4f Blank      { 0, 0, 0, 0 };           // Blank (Transparent)
	inline constexpr Color4f Magenta    { 255, 0, 255, 255 };     // Magenta
	inline constexpr Color4f RayWhite   { 245, 245, 245, 255 };   // My own White (raylib logo)
}