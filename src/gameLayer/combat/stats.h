#pragma once

struct Stats
{
	int baseDamage = 0;
	int maxHealth  = 0;
	int critChance = 0;
	int critDamage = 0;
	int armor      = 0;
	int armorPen   = 0;
	int knockback  = 0;
	int knockbackResist = 0;


	//todo add status effects
};

inline Stats makePlayerStats()
{
	Stats playerStats;
	playerStats.baseDamage = 5;
	playerStats.maxHealth = 50;
	playerStats.critChance = 0;
	playerStats.critDamage = 0;
	playerStats.armor = 0;
	playerStats.armorPen = 0;
	playerStats.knockback = 0;
	playerStats.knockbackResist = 0;
	return playerStats;
}

inline Stats makeEvilEyeStats()
{
	Stats EvilEyeStats;
	EvilEyeStats.baseDamage = 15;
	EvilEyeStats.maxHealth = 2800;
	EvilEyeStats.armor = 12;
	EvilEyeStats.knockbackResist = 100;
	return EvilEyeStats;
}


//Armor	   Multiplier	  Damage Taken
//0	       1.0	          100 %
//50	   0.66	          66 %
//100	   0.5	          50 %
//300	   0.25	          25 %