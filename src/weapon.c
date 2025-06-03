#include "weapon.h"

void init_weapons() {}

const char *weapon_name(WeaponType type) {
	switch (type) {
	case Weapon_EMPTY:
		return "";
	case Weapon_SCARL:
		return "SCAR-L";
	case Weapon_M16A4:
		return "M16A4";
	case Weapon_GROZA:
		return "Groza";
	case Weapon_AKM:
		return "AKM";
	case Weapon_AUG:
		return "AUG A3";
	case Weapon_M416:
		return "M416";
	case Weapon_QBZ:
		return "QBZ95";
	case Weapon_M762:
		return "Beryl M762";
	case Weapon_MK47:
		return "Mk47 Mutant";
	case Weapon_G36C:
		return "G36C";
	case Weapon_K2:
		return "K2";
	case Weapon_ACE32:
		return "ACE32";
	case Weapon_DEAGLE:
		return "Desert Eagle";
	case Weapon_FLARE:
		return "Flare Gun";
	case Weapon_P18C:
		return "P18C";
	case Weapon_P1911:
		return "P1911";
	case Weapon_P92:
		return "P92";
	case Weapon_R1895:
		return "R1895";
	case Weapon_R45:
		return "R45";
	case Weapon_SKORPION:
		return "Skorpion";
	default:
		return "Unknown Weapon";
	}
}
