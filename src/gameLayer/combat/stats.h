#pragma once

struct Stats
{
	int baseDamage = 5;
	int critChance = 0;
	int critDamage = 0;
	int armorPen   = 0;

	int armor      = 0;

	//todo add status effects
};

//Armor	   Multiplier	  Damage Taken
//0	       1.0	          100 %
//50	   0.66	          66 %
//100	   0.5	          50 %
//300	   0.25	          25 %