#pragma once
#include "blocks.h"

// Tool type IDs
constexpr int TOOL_PICKAXE = 6001;
constexpr int TOOL_AXE = 6002;
constexpr int TOOL_HAMMER = 6003;

struct BlockData
{
    int hp;
    int bestTool; // 6001=pickaxe, 6002=axe, 6003=hammer, 0=any/hand
};

// Indexed by Block::type
// Usage:  auto& data = BLOCK_DATA[block.type];
//         block.hp       = data.hp;
//         block.bestTool = data.bestTool;
static const BlockData BLOCK_DATA[Block::BLOCKS_COUNT] =
{
    //                         hp    bestTool
    /* air             */  {    0,          0 },

    // ── Soil & terrain ──────────────────────
    /* dirt            */  {   20, TOOL_PICKAXE },
    /* grassBlock      */  {   20, TOOL_PICKAXE },
    /* stone           */  {   60, TOOL_PICKAXE },
    /* grass           */  {    5, TOOL_AXE     },
    /* sand            */  {   15, TOOL_PICKAXE },
    /* sandRuby        */  {   80, TOOL_PICKAXE },
    /* sandStone       */  {   55, TOOL_PICKAXE },

    // ── Wood & planks ───────────────────────
    /* woodPlank       */  {   35, TOOL_AXE     },

    // ── Stone & brick ───────────────────────
    /* stoneBricks     */  {   70, TOOL_HAMMER  },

    // ── Soil (cont.) ────────────────────────
    /* clay            */  {   25, TOOL_PICKAXE },

    // ── Wood (cont.) ────────────────────────
    /* woodLog         */  {   40, TOOL_AXE     },
    /* leaves          */  {    5, TOOL_AXE     },

    // ── Ores ────────────────────────────────
    /* copper          */  {   60, TOOL_PICKAXE },
    /* iron            */  {   80, TOOL_PICKAXE },
    /* gold            */  {  100, TOOL_PICKAXE },

    // ── Ore blocks ──────────────────────────
    /* copperBlock     */  {   70, TOOL_PICKAXE },
    /* ironBlock       */  {   90, TOOL_PICKAXE },
    /* goldBlock       */  {  110, TOOL_PICKAXE },

    // ── Brick ───────────────────────────────
    /* bricks          */  {   65, TOOL_HAMMER  },

    // ── Snow & ice ──────────────────────────
    /* snow            */  {   15, TOOL_PICKAXE },
    /* ice             */  {   20, TOOL_PICKAXE },

    // ── Ore blocks (cont.) ──────────────────
    /* rubyBlock       */  {  120, TOOL_PICKAXE },

    // ── Furniture / platforms ───────────────
    /* platform        */  {   20, TOOL_AXE     },
    /* workBench       */  {   30, TOOL_AXE     },

    // ── Special ─────────────────────────────
    /* glass           */  {   10, TOOL_HAMMER  },
    /* furnace         */  {   50, TOOL_PICKAXE },
    /* painting        */  {   10, TOOL_HAMMER  },

    // ── Plants ──────────────────────────────
    /* sappling        */  {    5, TOOL_AXE     },

    // ── Ore blocks (cont.) ──────────────────
    /* snowBlueRuby    */  {   80, TOOL_PICKAXE },
    /* blueRubyBlock   */  {  130, TOOL_PICKAXE },

    // ── Furniture ───────────────────────────
    /* door            */  {   30, TOOL_AXE     },
    /* jar             */  {   10, TOOL_HAMMER  },
    /* table           */  {   30, TOOL_AXE     },
    /* wordrobe        */  {   35, TOOL_AXE     },
    /* bookShelf       */  {   30, TOOL_AXE     },

    // ── Snow brick & furniture ──────────────
    /* snowBricks      */  {   70, TOOL_HAMMER  },
    /* iceTable        */  {   30, TOOL_AXE     },
    /* iceWordrobe     */  {   35, TOOL_AXE     },
    /* iceBookShelf    */  {   30, TOOL_AXE     },
    /* icePlatform     */  {   20, TOOL_AXE     },

    // ── Sand furniture ──────────────────────
    /* sandTable       */  {   30, TOOL_AXE     },
    /* sandWordrobe    */  {   35, TOOL_AXE     },
    /* sandBookShelf   */  {   30, TOOL_AXE     },
    /* sandPlatform    */  {   20, TOOL_AXE     },

    // ── Chests ──────────────────────────────
    /* woodenChest     */  {   40, TOOL_AXE     },
    /* iceChest        */  {   40, TOOL_AXE     },
    /* sandChest       */  {   40, TOOL_AXE     },
    /* boneChest       */  {   40, TOOL_AXE     },

    // ── Bone set ────────────────────────────
    /* boneBricks      */  {   75, TOOL_HAMMER  },
    /* boneBench       */  {   30, TOOL_AXE     },
    /* boneWordrobe    */  {   35, TOOL_AXE     },
    /* boneBookShelf   */  {   30, TOOL_AXE     },
    /* bonePlatform    */  {   20, TOOL_AXE     },

    // ── Walls (all broken by hammer) ────────
    /* dirtWall        */  {   12, TOOL_HAMMER  },
    /* stoneWall       */  {   30, TOOL_HAMMER  },
    /* woodWall        */  {   20, TOOL_HAMMER  },
    /* sandStoneWall   */  {   30, TOOL_HAMMER  },
    /* brickWall       */  {   35, TOOL_HAMMER  },
    /* glassWall       */  {    8, TOOL_HAMMER  },
    /* copperBlockWall */  {   40, TOOL_HAMMER  },
    /* silverBlockWall */  {   45, TOOL_HAMMER  },
    /* goldBlockWall   */  {   55, TOOL_HAMMER  },
    /* snowWall        */  {   20, TOOL_HAMMER  },
    /* sandWall        */  {   25, TOOL_HAMMER  },
    /* stoneBricksWall */  {   35, TOOL_HAMMER  },
    /* rubyBlockWall   */  {   55, TOOL_HAMMER  },
    /* heroglyphWall   */  {   60, TOOL_HAMMER  },
    /* blueRubyWall    */  {   60, TOOL_HAMMER  },
    /* plankedWall     */  {   20, TOOL_HAMMER  },
    /* snowBrickWall   */  {   30, TOOL_HAMMER  },
    /* boneBrickWall   */  {   40, TOOL_HAMMER  },
};

// ─── Helpers ────────────────────────────────────────────────────────────────

// Call once after constructing a Block to stamp hp and bestTool from the table.
inline void initBlock(Block& b)
{
    if (b.type < Block::BLOCKS_COUNT)
    {
        b.hp = BLOCK_DATA[b.type].hp;
        b.bestTool = BLOCK_DATA[b.type].bestTool;
    }
}