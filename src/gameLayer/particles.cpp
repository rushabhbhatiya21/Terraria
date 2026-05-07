#include "particles.h"

std::unordered_map<int, Color> blockColors = {
    { Block::air,              {   0,   0,   0,   0 } }, // transparent
    { Block::dirt,             { 200, 118,  83, 255 } },
    { Block::grassBlock,       {  86, 152,  23, 255 } },
    { Block::stone,            { 136, 136, 136, 255 } },
    { Block::grass,            { 100, 180,  40, 255 } }, // flat grass / foliage
    { Block::sand,             { 237, 201, 122, 255 } },
    { Block::sandRuby,         { 220, 140,  80, 255 } }, // sandy with red tint
    { Block::sandStone,        { 210, 180, 120, 255 } },
    { Block::woodPlank,        { 160, 110,  60, 255 } },
    { Block::stoneBricks,      { 120, 120, 120, 255 } },
    { Block::clay,             { 180, 140, 120, 255 } },
    { Block::woodLog,          { 101,  79,  46, 255 } },
    { Block::leaves,           {  34, 139,  34, 255 } },
    { Block::copper,           { 184, 115,  51, 255 } },
    { Block::iron,             { 188, 160, 135, 255 } },
    { Block::gold,             { 255, 215,   0, 255 } },
    { Block::copperBlock,      { 200, 130,  60, 255 } },
    { Block::ironBlock,        { 210, 210, 210, 255 } },
    { Block::goldBlock,        { 255, 200,  30, 255 } },
    { Block::bricks,           { 178,  76,  54, 255 } },
    { Block::snow,             { 240, 245, 255, 255 } },
    { Block::ice,              { 180, 220, 255, 210 } },
    { Block::rubyBlock,        { 200,  30,  60, 255 } },
    { Block::platform,         { 160, 110,  60, 200 } }, // semi-transparent wood
    { Block::workBench,        { 130,  85,  40, 255 } },
    { Block::glass,            { 200, 230, 255, 120 } },
    { Block::furnace,          { 100,  90,  80, 255 } },
    { Block::painting,         { 180, 120,  60, 255 } }, // wooden frame
    { Block::sappling,         {  60, 180,  60, 255 } },
    { Block::snowBlueRuby,     { 180, 210, 255, 255 } }, // icy blue-white
    { Block::blueRubyBlock,    {  60, 120, 220, 255 } },
    { Block::door,             { 140,  90,  50, 255 } },
    { Block::jar,              { 180, 210, 200, 180 } }, // glassy greenish
    { Block::table,            { 150, 100,  55, 255 } },
    { Block::wordrobe,         { 120,  80,  45, 255 } },
    { Block::bookShelf,        { 160, 100,  40, 255 } },
    { Block::snowBricks,       { 220, 230, 245, 255 } },
    { Block::iceTable,         { 160, 210, 240, 220 } },
    { Block::iceWordrobe,      { 150, 205, 235, 220 } },
    { Block::iceBookShelf,     { 155, 208, 238, 220 } },
    { Block::icePlatform,      { 170, 215, 245, 180 } },
    { Block::sandTable,        { 220, 185, 105, 255 } },
    { Block::sandWordrobe,     { 215, 180, 100, 255 } },
    { Block::sandBookShelf,    { 218, 183, 103, 255 } },
    { Block::sandPlatform,     { 225, 190, 110, 200 } },
    { Block::woodenChest,      { 140,  95,  45, 255 } },
    { Block::iceChest,         { 160, 215, 245, 220 } },
    { Block::sandChest,        { 220, 185, 105, 255 } },
    { Block::boneChest,        { 230, 220, 195, 255 } },
    { Block::boneBricks,       { 220, 210, 185, 255 } },
    { Block::boneBench,        { 225, 215, 190, 255 } },
    { Block::boneWordrobe,     { 222, 212, 188, 255 } },
    { Block::boneBookShelf,    { 224, 214, 190, 255 } },
    { Block::bonePlatform,     { 228, 218, 193, 200 } },

    // --- walls (slightly darker / more muted than their block counterpart) ---
    { Block::dirtWall,         { 120,  88,  58, 255 } },
    { Block::stoneWall,        { 100, 100, 100, 255 } },
    { Block::woodWall,         { 120,  85,  40, 255 } },
    { Block::sandStoneWall,    { 185, 158, 100, 255 } },
    { Block::brickWall,        { 148,  58,  38, 255 } },
    { Block::glassWall,        { 180, 215, 245,  80 } },
    { Block::copperBlockWall,  { 160, 100,  40, 255 } },
    { Block::silverBlockWall,  { 170, 175, 180, 255 } },
    { Block::goldBlockWall,    { 210, 165,  20, 255 } },
    { Block::snowWall,         { 210, 220, 235, 255 } },
    { Block::sandWall,         { 205, 170,  95, 255 } },
    { Block::stoneBricksWall,  {  95,  95,  95, 255 } },
    { Block::rubyBlockWall,    { 160,  20,  45, 255 } },
    { Block::heroglyphWall,    { 180, 155,  80, 255 } }, // sandy gold tone
    { Block::blueRubyWall,     {  40,  90, 180, 255 } },
    { Block::plankedWall,      { 130,  88,  42, 255 } },
    { Block::snowBrickWall,    { 195, 210, 228, 255 } },
    { Block::boneBrickWall,    { 195, 185, 160, 255 } },
};

void Particle::render() const
{
    // fade slowly at start
    DrawRectangleV(positon, { .1f,.1f }, Fade(color, 1.f - (1.f - life) * (1.f - life))); // ease-out
}

Color getBlockColor(int type)
{
    return blockColors[type];
}

std::vector<Particle> spawnParticles(Vector2 pos, std::ranlux24_base& rng, int blockType, int numberOfParticles, float maxOffset, int angleOffset)
{
    std::vector<Particle> particles;
    for (int i = 0; i < numberOfParticles; i++)
    {
        Particle p;
        float offset = getRandomFloat(rng, 0, maxOffset);
        p.positon.x = pos.x + offset;
        p.positon.y = pos.y;

        float angle = getRandomFloat(rng, 220 + angleOffset, 320 + angleOffset) * DEG2RAD;
        float speed = getRandomFloat(rng, 50, 150) / 30.f;

        p.velocity = { cosf(angle) * speed, sinf(angle) * speed };
        p.life = getRandomFloat(rng, .3f, .7f);
        p.color = getBlockColor(blockType);
        particles.push_back(p);
    }
    return particles;
}

void updateParticles(std::vector<Particle>& particles, float dt)
{
    if (particles.empty()) return;

    for (auto& p : particles)
    {
        p.velocity.y += 9.8f * dt;
        p.positon.x += p.velocity.x * dt;
        p.positon.y += p.velocity.y * dt;
        p.life -= dt;
    }

    // remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0.f; }),
        particles.end()
    );
}

void renderParticles(std::vector<Particle>& particles)
{
    for (auto& p : particles)
        p.render();
}