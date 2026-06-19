#include <FastNoiseSIMD.h>

#include "worldGenerator.h"
#include <asserts.h>
#include "randomStuff.h"
#include <structure.h>
#include <saveMap.h>

//Block initBlock(ItemId type)
//{
//    Block b;
//    b.type = type;
//
//    auto* def = getItem(type);
//
//    permaAssertComment(
//        def,
//        "Missing item definition in initBlock()"
//    );
//
//    b.hp = def->block.hp;
//    b.light = 0;
//
//    return b;
//}

void generateWorld(GameMap& gameMap, const int w, const int h, int seed)
{
    std::cout << "PASS 1\n";

    gameMap.create(w, h);

    std::ranlux24_base rng(seed++);

    // ── Desert region ────────────────────────────────────────────────────────
    int desertStart = getRandomInt(rng, 10, w - 210);
    int desertEnd = desertStart + 100 + getRandomInt(rng, 0, 100);
    if (desertEnd > w) desertEnd = w;

    gameMap.desertStart = desertStart;
    gameMap.desertEnd = desertEnd;

    // ── Load tree structure ──────────────────────────────────────────────────
    Structure treeStructure;
    loadBlockDataFromFile(
        treeStructure.mapData,
        treeStructure.w,
        treeStructure.h,
        RESOURCES_PATH "structures/tree.bin"
    );

    // ── Noise generators ─────────────────────────────────────────────────────
    std::unique_ptr<FastNoiseSIMD> dirtNoisegen(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> cavesNoisegen(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> oreNoisegen(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> ore2Noisegen(FastNoiseSIMD::NewFastNoiseSIMD());
    std::unique_ptr<FastNoiseSIMD> detailNoisegen(FastNoiseSIMD::NewFastNoiseSIMD());

    dirtNoisegen->SetSeed(seed++);
    cavesNoisegen->SetSeed(seed++);
    oreNoisegen->SetSeed(seed++);
    ore2Noisegen->SetSeed(seed++);
    detailNoisegen->SetSeed(seed++);

    // Dirt surface height
    dirtNoisegen->SetFractalType(FastNoiseSIMD::FractalType::FBM);
    dirtNoisegen->SetFractalOctaves(6);
    dirtNoisegen->SetFractalGain(0.4f);
    dirtNoisegen->SetFrequency(0.01f);

    // Caves
    cavesNoisegen->SetFractalType(FastNoiseSIMD::FractalType::FBM);
    cavesNoisegen->SetFractalOctaves(3);
    cavesNoisegen->SetFrequency(0.02f);

    // Ore blobs (low freq = big blobs, high threshold = rare)
    oreNoisegen->SetFractalType(FastNoiseSIMD::FractalType::FBM);
    oreNoisegen->SetFractalOctaves(2);
    oreNoisegen->SetFrequency(0.06f);

    // Second ore pass (for iron/gold, different pattern)
    ore2Noisegen->SetFractalType(FastNoiseSIMD::FractalType::FBM);
    ore2Noisegen->SetFractalOctaves(2);
    ore2Noisegen->SetFrequency(0.07f);

    // Small detail noise (clay pocket variance)
    detailNoisegen->SetFractalType(FastNoiseSIMD::FractalType::FBM);
    detailNoisegen->SetFractalOctaves(2);
    detailNoisegen->SetFrequency(0.15f);

    // ── Fill noise arrays ────────────────────────────────────────────────────
    float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);
    float* cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);
    float* oreNoise = FastNoiseSIMD::GetEmptySet(w * h);
    float* ore2Noise = FastNoiseSIMD::GetEmptySet(w * h);
    float* detailNoise = FastNoiseSIMD::GetEmptySet(w * h);

    dirtNoisegen->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
    cavesNoisegen->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1);
    oreNoisegen->FillNoiseSet(oreNoise, 0, 0, 0, h, w, 1);
    ore2Noisegen->FillNoiseSet(ore2Noise, 0, 0, 0, h, w, 1);
    detailNoisegen->FillNoiseSet(detailNoise, 0, 0, 0, h, w, 1);

    // Remap all from [-1,1] to [0,1]
    for (int i = 0; i < w; i++) dirtNoise[i] = (dirtNoise[i] + 1) / 2.f;
    for (int i = 0; i < w * h; i++) cavesNoise[i] = (cavesNoise[i] + 1) / 2.f;
    for (int i = 0; i < w * h; i++) oreNoise[i] = (oreNoise[i] + 1) / 2.f;
    for (int i = 0; i < w * h; i++) ore2Noise[i] = (ore2Noise[i] + 1) / 2.f;
    for (int i = 0; i < w * h; i++) detailNoise[i] = (detailNoise[i] + 1) / 2.f;

    // Helpers
    auto idx = [&](int x, int y) { return x + y * w; };
    auto getCaveNoise = [&](int x, int y) { return cavesNoise[idx(x, y)]; };
    auto getOreNoise = [&](int x, int y) { return oreNoise[idx(x, y)]; };
    auto getOre2Noise = [&](int x, int y) { return ore2Noise[idx(x, y)]; };
    auto getDetailNoise = [&](int x, int y) { return detailNoise[idx(x, y)]; };

    // ── Depth thresholds (y=0 is surface, y grows downward) ─────────────────
    // stoneHeight ~ 80-130, so:
    //   shallow  = stoneHeight + 10   (just below dirt/stone boundary)
    //   mid      = stoneHeight + 60
    //   deep     = stoneHeight + 120
    //   verydeep = stoneHeight + 180
    // These are relative; we compare against absolute y below.

    //// ── Stone height random walk ─────────────────────────────────────────────
    //int keepDirectionTimeStone = getRandomInt(rng, 30, 100);
    //int directionStone = getRandomInt(rng, -2, 2);
    //int stoneHeight = 90;

    // ── Dirt offset: thick layer like Terraria ───────────────────────────────
    // Was [-5, 35]. Now [-8, 55] → ~20–55 block thick dirt layer.
    const int dirtOffsetStart = -8;
    const int dirtOffsetEnd = 55;

    // ── Per-column surface heights (needed for wall pass) ────────────────────
    std::vector<int> surfaceY(w, 0); // y of the grassBlock/sand surface

    // ═══════════════════════════════════════════════════════════════════════════
    //  PASS 1 — Terrain + ores
    // ═══════════════════════════════════════════════════════════════════════════
    int previousStoneHeight = 90;

    for (int x = 0; x < w; x++)
    {
        bool inDesert = (x >= desertStart && x <= desertEnd);

        // Smooth noise-based terrain
        int targetStoneHeight = 90 + int((dirtNoise[x] - 0.5f) * 10.f);

        // Prevent sudden cliffs
        int stoneHeight =
            std::clamp(
                targetStoneHeight,
                previousStoneHeight - 1,
                previousStoneHeight + 1
            );

        previousStoneHeight = stoneHeight;

        //// Stone height random walk
        //keepDirectionTimeStone--;
        //if (keepDirectionTimeStone <= 0)
        //{
        //    keepDirectionTimeStone = getRandomInt(rng, 5, 40);
        //    directionStone = getRandomInt(rng, -2, 2);
        //}

        //auto nudge = [&](int dir, int& height)
        //    {
        //        if (dir == -1 && getRandomChance(rng, 0.1f))                         height--;
        //        if (dir == -2 && getRandomChance(rng, 0.1f))                         height--;
        //        if (dir == -2 && getRandomChance(rng, 0.1f))                         height--;
        //        if (dir ==  1 && getRandomChance(rng, 0.1f))                         height++;
        //        if (dir ==  2 && getRandomChance(rng, 0.1f))                         height++;
        //        if (dir ==  2 && getRandomChance(rng, 0.1f))                         height++;
        //    };
        //nudge(directionStone, stoneHeight);
        //stoneHeight = std::clamp(stoneHeight, 60, 120);

        int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];
        dirtHeight = stoneHeight - dirtHeight;

        int surfaceRow = dirtHeight; // grassBlock / sand row
        surfaceY[x] = surfaceRow;

        int dirtType = inDesert ? Items::sand : Items::dirt;
        int grassType = inDesert ? Items::sand : Items::grassBlock;
        int stoneType = inDesert ? Items::sandStone : Items::stone;

        for (int y = 0; y < h; y++)
        {
            ItemId blockType;
            blockType = Items::air;

            // ── Base terrain ─────────────────────────────────────────────────
            if (y > surfaceRow)    blockType = dirtType;
            if (y == surfaceRow)   blockType = grassType;
            if (y >= stoneHeight)  blockType = stoneType;

            // ── Desert sandstone triangle ────────────────────────────────────
            if (inDesert)
            {
                int   desertMid = (desertEnd + desertStart) / 2;
                int   desertHalfWidth = (desertEnd - desertStart) / 2;
                int   distFromMid = std::abs(x - desertMid);
                float desertDist = 1.f - distFromMid / float(desertHalfWidth);
                int   triStoneY = (10 + stoneHeight) + int(desertDist * (20 + stoneHeight));
                if (y > triStoneY) blockType = Items::stone;
            }

            // ── Caves (noise threshold) ──────────────────────────────────────
            if (getCaveNoise(x, y) < 0.30f)
                blockType = Items::air;

            // ── Ore veins (only in solid stone or sandStone) ─────────────────
            if (blockType == Items::stone || blockType == Items::sandStone)
            {
                int depth = y - stoneHeight; // 0 at stone surface, grows down

                // Copper — shallow (depth 0–80), oreNoise blob
                if (depth >= 0 && depth < 80)
                {
                    float thresh = 0.72f - depth * 0.0008f; // slightly rarer deeper
                    if (getOreNoise(x, y) > thresh)
                        blockType = Items::copper;
                }

                // Iron — mid depth (depth 40–160), ore2Noise blob
                if (depth >= 40 && depth < 160)
                {
                    float thresh = 0.74f;
                    if (getOre2Noise(x, y) > thresh)
                        blockType = Items::iron;
                }

                // Gold — deep (depth 100–250), oreNoise (different range)
                if (depth >= 100 && depth < 250)
                {
                    float thresh = 0.77f;
                    if (getOreNoise(x, y) > thresh)
                        blockType = Items::gold;
                }

                // Ruby — very deep (depth 190+), ore2Noise
                if (depth >= 190)
                {
                    float thresh = 0.79f;
                    if (getOre2Noise(x, y) > thresh)
                        blockType = inDesert ? Items::sandRuby : Items::rubyBlock;
                }

                // BlueRuby — deepest (depth 230+), oreNoise
                if (depth >= 230)
                {
                    float thresh = 0.81f;
                    if (getOreNoise(x, y) > thresh)
                        blockType = inDesert ? Items::snowBlueRuby : Items::blueRubyBlock;
                }
            }

            // ── Clay pockets — near dirt/stone boundary ──────────────────────
            // Appears in the dirt layer within ~15 blocks above stone
            if (blockType == Items::dirt)
            {
                int distToStone = stoneHeight - y;
                if (distToStone >= 0 && distToStone < 15)
                {
                    float thresh = 0.68f + distToStone * 0.01f; // rarer further from stone
                    if (getDetailNoise(x, y) > thresh)
                        blockType = Items::clay;
                }
            }

            gameMap.getBlockUnsafe(x, y) = initBlock(blockType);
        }
    }

    FastNoiseSIMD::FreeNoiseSet(dirtNoise);
    FastNoiseSIMD::FreeNoiseSet(cavesNoise);
    FastNoiseSIMD::FreeNoiseSet(oreNoise);
    FastNoiseSIMD::FreeNoiseSet(ore2Noise);
    FastNoiseSIMD::FreeNoiseSet(detailNoise);

    std::cout << "PASS 1 DONE\n";
    std::cout << "PASS 2\n";

    // ═══════════════════════════════════════════════════════════════════════════
    //  PASS 2 — Perlin worms (large caves)
    // ═══════════════════════════════════════════════════════════════════════════
    for (int i = 0; i < 20; i++)
    {
        float x = getRandomFloat(rng, 10.f, float(w - 10));
        float y = getRandomFloat(rng, 51.f, float(h - 10));

        float dirX = getRandomFloat(rng, -1.f, 1.f);
        float dirY = getRandomFloat(rng, -1.f, 1.f);
        float length = float(getRandomInt(rng, 200, 700));
        float radius = 2.5f;

        int changeTime = getRandomInt(rng, 5, 20);

        for (int j = 0; j < int(length); j++)
        {
            int intR = int(std::ceil(radius));
            for (int ox = -intR; ox <= intR; ox++)
            {
                for (int oy = -intR; oy <= intR; oy++)
                {
                    if (float(ox * ox + oy * oy) <= radius * radius)
                    {
                        auto b = gameMap.getBlockSafe(int(x) + ox, int(y) + oy);
                        if (b) b->type = Items::air;
                    }
                }
            }

            changeTime--;
            if (changeTime <= 0)
            {
                changeTime = getRandomInt(rng, 5, 20);
                float keep = getRandomChance(rng, 0.7f) ? 0.8f : 0.2f;
                dirX = dirX * keep + getRandomFloat(rng, -1.f, 1.f) * (1.f - keep);
                dirY = dirY * keep + getRandomFloat(rng, -1.f, 1.f) * (1.f - keep);
            }

            x += dirX * 1.5f;
            y += dirY * 1.5f;
            radius += getRandomFloat(rng, -0.2f, 0.2f);
            radius = std::clamp(radius, 2.2f, 8.5f);
        }
    }

    std::cout << "PASS 2 DONE\n";
    std::cout << "PASS 3\n";

    // ═══════════════════════════════════════════════════════════════════════════
    //  PASS 3 — Background walls (dirtWall + stoneWall inside caves)
    // ═══════════════════════════════════════════════════════════════════════════
    // Strategy: for every air block below surface, look at surrounding solid
    // blocks to decide the wall type — dirtWall near surface, stoneWall deeper.
    // We use a separate wall layer if your GameMap supports it, otherwise we
    // place wall blocks into a wall grid. Adjust getWallUnsafe to your API.
    // If GameMap has no wall layer yet, comment this section out.
    for (int x = 1; x < w - 1; x++)
    {
        for (int y = 1; y < h - 1; y++)
        {
            auto& b = gameMap.getBlockUnsafe(x, y);
            if (b.type != Items::air) continue;
            if (y < surfaceY[x] - 1)  continue; // above ground — no wall

            // Check if any neighbour is solid (cave interior check)
            bool hasSolidNeighbour =
                gameMap.getBlockUnsafe(x - 1, y).type != Items::air ||
                gameMap.getBlockUnsafe(x + 1, y).type != Items::air ||
                gameMap.getBlockUnsafe(x, y - 1).type != Items::air ||
                gameMap.getBlockUnsafe(x, y + 1).type != Items::air;

            if (!hasSolidNeighbour) continue;

            // Decide wall type by depth
            int depthBelowSurface = y - surfaceY[x];
            int wallType = (depthBelowSurface < 30) ? Items::dirtWall : Items::stoneWall;

            // Place into wall layer — adjust to your GameMap API:
            // gameMap.getWallUnsafe(x, y).type = wallType;
            // If you store walls as a second Block array, replace the line above.
            // For now we leave a clearly-labelled call so you can wire it in.
            (void)wallType; // remove when wired
        }
    }

    std::cout << "PASS 3 DONE\n";
    std::cout << "PASS 4\n";

    // ═══════════════════════════════════════════════════════════════════════════
    //  PASS 4 — Trees
    // ═══════════════════════════════════════════════════════════════════════════
    for (int x = 0; x < w; x++)
    {
        if (!getRandomChance(rng, 0.04f)) continue;

        for (int y = 0; y < h; y++)
        {
            auto type = gameMap.getBlockUnsafe(x, y).type;
            if (type == Items::air) continue;

            if (type == Items::grassBlock)
            {
                Vector2 spawnPos{ float(x) - treeStructure.w / 2.f, float(y) - treeStructure.h };
                treeStructure.pasteIntoMap(gameMap, spawnPos);
                x += 3;
            }
            break;
        }
    }

    std::cout << "PASS 4 DONE\n";
    std::cout << "PASS 5\n";

    // ═══════════════════════════════════════════════════════════════════════════
    //  PASS 5 — Surface scatter
    //    Grass blocks get: grass plants, saplings, mushrooms, stone rocks
    //    Probabilities are mutually exclusive and checked in priority order.
    //    Rocks are 1–3 wide stone/stoneBricks clusters sitting on the surface.
    // ═══════════════════════════════════════════════════════════════════════════
    for (int x = 0; x < w; x++)
    {
        int sy = surfaceY[x];
        if (sy < 1 || sy >= h - 1) continue;

        auto& surface = gameMap.getBlockUnsafe(x, sy);
        auto& above = gameMap.getBlockUnsafe(x, sy - 1);

        if (surface.type != Items::grassBlock) continue;
        if (above.type != Items::air)        continue;

        float roll = getRandomFloat(rng, 0.f, 1.f);

        if (roll < 0.18f)
        {
            // ── Grass plant ───────────────────────────────────────────────────
            above = initBlock(Items::grass);
        }
        else if (roll < 0.18f + 0.04f)
        {
            // ── Sapling ───────────────────────────────────────────────────────
            above = initBlock(Items::sappling);
        }
        else if (roll < 0.18f + 0.04f + 0.03f)
        {
            // ── Mushroom (jar block repurposed as mushroom cap, 1 tall) ───────
            // Using Items::jar as a small mushroom — swap to a dedicated block
            // once you have one.  Two variants: stone = brown, glass = red cap.
            above = initBlock(
                getRandomChance(rng, 0.6f) ? Items::jar : Items::glass
            );
        }
        else if (roll < 0.18f + 0.04f + 0.03f + 0.025f)
        {
            // ── Rock cluster (1–3 blocks wide, 1–2 tall) ─────────────────────
            // Rocks = small stone or stoneBricks lumps on the surface.
            int rockWidth = getRandomInt(rng, 1, 3);
            int rockHeight = getRandomInt(rng, 1, 2);
            ItemId rockType = getRandomChance(rng, 0.7f) ? Items::stone : Items::stoneBricks;

            int startX = x - rockWidth / 2;
            for (int rx = 0; rx < rockWidth; rx++)
            {
                for (int ry = 0; ry < rockHeight; ry++)
                {
                    auto b = gameMap.getBlockSafe(startX + rx, sy - 1 - ry);
                    if (b && b->type == Items::air)
                    {
                        *b = initBlock(rockType);
                    }
                }
            }
            x += rockWidth; // skip ahead so next scatter doesn't overlap the rock
        }
    }

    std::cout << "PASS 5 DONE\n";
}
