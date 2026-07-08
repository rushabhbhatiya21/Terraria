#pragma once

struct TextureImpl;
struct AssetManager;

class Texture
{
public:
	int getWidth() const;
	int getHeight() const;

private:
	friend class AssetManager;

	Texture() = default;

	int width = 0;
	int height = 0;

	// Backend-specific implementation comes later.
};