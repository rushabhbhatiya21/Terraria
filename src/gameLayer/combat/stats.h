#pragma once

// todo: add boss damage and check in applyDamage if boss then multiply
struct OffensiveStats
{
	int damage = 0;
	int critChance = 0;
	int critDamage = 0;
	int armorPen = 0;
	int knockback = 0;
	int pierceCount = 0;
	int range = 0;

	OffensiveStats& operator+=(const OffensiveStats& other)
	{
		damage += other.damage;
		critChance += other.critChance;
		critDamage += other.critDamage;
		armorPen += other.armorPen;
		knockback += other.knockback;
		pierceCount += other.pierceCount;
		range += other.range;
		return *this;
	}
};

inline OffensiveStats operator+(OffensiveStats lhs, const OffensiveStats& rhs)
{
	lhs += rhs;
	return lhs;
}

struct DefensiveStats
{
	int maxHealth = 0;
	int armor = 0;
	int knockbackResist = 0;

	DefensiveStats& operator+=(const DefensiveStats& other)
	{
		maxHealth += other.maxHealth;
		armor += other.armor;
		knockbackResist += other.knockbackResist;
		return *this;
	}
};

inline DefensiveStats operator+(DefensiveStats lhs, const DefensiveStats& rhs)
{
	lhs += rhs;
	return lhs;
}

struct ToolStats
{
	int miningPower = 0;
	int axePower = 0;
	int hammerPower = 0;
	int range = 0;

	ToolStats& operator+=(const ToolStats& other)
	{
		miningPower += other.miningPower;
		axePower += other.axePower;
		hammerPower += other.hammerPower;
		range += other.range;
		return *this;
	}
};

inline ToolStats operator+(ToolStats lhs, const ToolStats& rhs)
{
	lhs += rhs;
	return lhs;
}

struct EntityStats
{
	OffensiveStats offensive;
	DefensiveStats defensive;
	ToolStats      tool;
	//todo add status effects

	EntityStats& operator+=(const EntityStats& other)
	{
		offensive += other.offensive;
		defensive += other.defensive;
		tool += other.tool;
		return *this;
	}
};

inline EntityStats operator+(EntityStats lhs, const EntityStats& rhs)
{
	lhs += rhs;
	return lhs;
}

inline EntityStats makePlayerStats()
{
	EntityStats playerStats;
	playerStats.defensive.maxHealth = 50;
	return playerStats;
}

inline EntityStats makeEvilEyeStats()
{
	EntityStats EvilEyeStats;
	EvilEyeStats.offensive.damage = 15;

	EvilEyeStats.defensive.armor = 12;
	EvilEyeStats.defensive.maxHealth = 2800;
	EvilEyeStats.defensive.knockbackResist = 100;

	return EvilEyeStats;
}


//Armor	   Multiplier	  Damage Taken
//0	       1.0	          100 %
//50	   0.66	          66 %
//100	   0.5	          50 %
//300	   0.25	          25 %