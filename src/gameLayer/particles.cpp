#include "particles.h"

std::vector<Particle> particles;

std::unordered_map<int, Engine::Color4f> blockColors = {
    { Items::air,              {   0,   0,   0,   0 } }, // transparent
    { Items::dirt,             { 200, 118,  83, 255 } },
    { Items::grassBlock,       {  86, 152,  23, 255 } },
    { Items::stone,            { 136, 136, 136, 255 } },
    { Items::grass,            { 100, 180,  40, 255 } }, // flat grass / foliage
    { Items::sand,             { 237, 201, 122, 255 } },
    { Items::sandRuby,         { 220, 140,  80, 255 } }, // sandy with red tint
    { Items::sandStone,        { 210, 180, 120, 255 } },
    { Items::woodPlank,        { 160, 110,  60, 255 } },
    { Items::stoneBricks,      { 120, 120, 120, 255 } },
    { Items::clay,             { 180, 140, 120, 255 } },
    { Items::woodLog,          { 101,  79,  46, 255 } },
    { Items::leaves,           {  34, 139,  34, 255 } },
    { Items::copper,           { 184, 115,  51, 255 } },
    { Items::iron,             { 188, 160, 135, 255 } },
    { Items::gold,             { 255, 215,   0, 255 } },
    { Items::copperBlock,      { 200, 130,  60, 255 } },
    { Items::ironBlock,        { 210, 210, 210, 255 } },
    { Items::goldBlock,        { 255, 200,  30, 255 } },
    { Items::bricks,           { 178,  76,  54, 255 } },
    { Items::snow,             { 240, 245, 255, 255 } },
    { Items::ice,              { 180, 220, 255, 210 } },
    { Items::rubyBlock,        { 200,  30,  60, 255 } },
    { Items::platform,         { 160, 110,  60, 200 } }, // semi-transparent wood
    { Items::workBench,        { 130,  85,  40, 255 } },
    { Items::glass,            { 200, 230, 255, 120 } },
    { Items::furnace,          { 100,  90,  80, 255 } },
    { Items::painting,         { 180, 120,  60, 255 } }, // wooden frame
    { Items::sappling,         {  60, 180,  60, 255 } },
    { Items::snowBlueRuby,     { 180, 210, 255, 255 } }, // icy blue-white
    { Items::blueRubyBlock,    {  60, 120, 220, 255 } },
    { Items::door,             { 140,  90,  50, 255 } },
    { Items::jar,              { 180, 210, 200, 180 } }, // glassy greenish
    { Items::table,            { 150, 100,  55, 255 } },
    { Items::wordrobe,         { 120,  80,  45, 255 } },
    { Items::bookShelf,        { 160, 100,  40, 255 } },
    { Items::snowBricks,       { 220, 230, 245, 255 } },
    { Items::iceTable,         { 160, 210, 240, 220 } },
    { Items::iceWordrobe,      { 150, 205, 235, 220 } },
    { Items::iceBookShelf,     { 155, 208, 238, 220 } },
    { Items::icePlatform,      { 170, 215, 245, 180 } },
    { Items::sandTable,        { 220, 185, 105, 255 } },
    { Items::sandWordrobe,     { 215, 180, 100, 255 } },
    { Items::sandBookShelf,    { 218, 183, 103, 255 } },
    { Items::sandPlatform,     { 225, 190, 110, 200 } },
    { Items::woodenChest,      { 140,  95,  45, 255 } },
    { Items::iceChest,         { 160, 215, 245, 220 } },
    { Items::sandChest,        { 220, 185, 105, 255 } },
    { Items::boneChest,        { 230, 220, 195, 255 } },
    { Items::boneBricks,       { 220, 210, 185, 255 } },
    { Items::boneBench,        { 225, 215, 190, 255 } },
    { Items::boneWordrobe,     { 222, 212, 188, 255 } },
    { Items::boneBookShelf,    { 224, 214, 190, 255 } },
    { Items::bonePlatform,     { 228, 218, 193, 200 } },

    // --- walls (slightly darker / more muted than their block counterpart) ---
    { Items::dirtWall,         { 120,  88,  58, 255 } },
    { Items::stoneWall,        { 100, 100, 100, 255 } },
    { Items::woodWall,         { 120,  85,  40, 255 } },
    { Items::sandStoneWall,    { 185, 158, 100, 255 } },
    { Items::brickWall,        { 148,  58,  38, 255 } },
    { Items::glassWall,        { 180, 215, 245,  80 } },
    { Items::copperBlockWall,  { 160, 100,  40, 255 } },
    { Items::silverBlockWall,  { 170, 175, 180, 255 } },
    { Items::goldBlockWall,    { 210, 165,  20, 255 } },
    { Items::snowWall,         { 210, 220, 235, 255 } },
    { Items::sandWall,         { 205, 170,  95, 255 } },
    { Items::stoneBricksWall,  {  95,  95,  95, 255 } },
    { Items::rubyBlockWall,    { 160,  20,  45, 255 } },
    { Items::heroglyphWall,    { 180, 155,  80, 255 } }, // sandy gold tone
    { Items::blueRubyWall,     {  40,  90, 180, 255 } },
    { Items::plankedWall,      { 130,  88,  42, 255 } },
    { Items::snowBrickWall,    { 195, 210, 228, 255 } },
    { Items::boneBrickWall,    { 195, 185, 160, 255 } },

    { Items::shuriken,         Engine::LightGray },
    { Items::woodenArrow,      Engine::Brown }
};

void Particle::render() const
{
    // fade slowly at start
    //DrawRectangleV(positon, { .1f,.1f }, Fade(color, 1.f - (1.f - life) * (1.f - life))); // ease-out
}

Engine::Color4f getBlockColor(int type)
{
    return blockColors[type];
}

void spawnParticles(Engine::Vec2 pos, std::ranlux24_base& rng, int blockType, int numberOfParticles, float maxOffset, int angleOffset, bool fullCircle)
{
    for (int i = 0; i < numberOfParticles; i++)
    {
        Particle p;
        float angle = 0;
        float offset = getRandomFloat(rng, 0, maxOffset);
        p.positon.x = pos.x + offset;
        p.positon.y = pos.y;

        if (fullCircle)
            angle = getRandomFloat(rng, 0.0f, 360.0f) * Engine::Deg2Rad;
        else
            angle = getRandomFloat(rng, 220 + angleOffset, 320 + angleOffset) * Engine::Deg2Rad;
        float speed = getRandomFloat(rng, 50, 150) / 30.f;

        p.velocity = { cosf(angle) * speed, sinf(angle) * speed };
        p.life = getRandomFloat(rng, .3f, .7f);
        p.color = getBlockColor(blockType);
        particles.push_back(p);
    }
}

void updateParticles(float dt)
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

void renderParticles()
{
    for (auto& p : particles)
        p.render();
}