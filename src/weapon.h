#ifndef _TDM_weapon_h
#define _TDM_weapon_h

typedef enum MuzzleType {
	Muzzle_NONE,
	Muzzle_COMPENSATOR,	// 枪口补偿器
	Muzzle_FLASHHIDER,	// 消焰器
	Muzzle_SUPPRESSOR,	// 消音器
	Muzzle_CHOKE,
	Muzzle_DUCKBILL,
} MuzzleType;		// 枪口

typedef enum GripType {
	Grip_NONE,
	Grip_ANGLED,	// 直角前
	Grip_HALF,	// 半截式
	Grip_LASER,	// 激光
	Grip_LIGHT,	// 轻型
	Grip_THUMB,	// 拇指
	Grip_VERTICAL,	// 垂直
} GripType;	// 握把

typedef enum SightType {
	Sight_IRON,	// 机瞄
	Sight_SCOPE2X,	// 2倍
	Sight_SCOPE3X,	// 3倍
	Sight_SCOPE4X,	// 4倍
	Sight_SCOPE6X,	// 6倍
	Sight_SCOPE8X,	// 8倍
	Sight_HOLO,	// 全息
	Sight_REDDOT,	// 红点
} SightType;	// 瞄准镜

typedef enum MagazineType {
	Magazine_EXTEND,	// 扩容
	Magazine_EXTENDQUICK,	// 快速扩容
	Magazine_QUICKDRAW,	// 快速
} MagazineType;		// 弹夹

typedef enum StockType {
	Stock_NONE,
	Stock_BULLETLOOPS,
	Stock_CHEEKPAD,
	Stock_MICROUZI,
	Stock_TACTICAL,
} StockType;

typedef enum WeaponType {
	Weapon_EMPTY,

	// Assault Rifles
	// 突击步枪
	Weapon_ACE32,
	Weapon_AKM,
	Weapon_AUG,
	Weapon_G36C,
	Weapon_GROZA,
	Weapon_K2,
	Weapon_M16A4,
	Weapon_M416,
	Weapon_M762,
	Weapon_MK47,
	Weapon_QBZ,
	Weapon_SCARL,

	// Designated Marksman Rifles
	// 射手步枪
	Weapon_SKS,
	Weapon_MK14,
	Weapon_MINI14,
	Weapon_SLR,
	Weapon_QBU,
	Weapon_VSS,
	Weapon_MK12,
	Weapon_DRAGUNOV,

	// Submachine Guns
	// 冲锋枪
	Weapon_MICROUZI,
	Weapon_UMP45,
	Weapon_TOMMYGUN,
	Weapon_VECTOR,
	Weapon_PP19BIZON,
	Weapon_MP5K,
	Weapon_P90,

	// Sniper Rifles
	// 狙击步枪
	Weapon_AWM,
	Weapon_M24,
	Weapon_KAR98K,
	Weapon_WINM1894,
	Weapon_MOSINNAGANT,
	Weapon_LYNXAMR,

	// Light Machine Guns
	// 轻机枪
	Weapon_DP28,
	Weapon_M249,
	Weapon_MG3,

	// Shotguns
	// 霰弹枪
	Weapon_S1897,
	Weapon_S686,
	Weapon_S12K,
	Weapon_SAWEDOFF,
	Weapon_DBS,
	Weapon_O12,

	// CrossBow
	Weapon_CROSSBOW,

	// Pistols
	// 手枪
	Weapon_DEAGLE,
	Weapon_FLARE,
	Weapon_P18C,
	Weapon_P1911,
	Weapon_P92,
	Weapon_R1895,
	Weapon_R45,
	Weapon_SKORPION,
} WeaponType;

// weapon in player's weapon slot
typedef struct Weapon {
	WeaponType base;
	MuzzleType muzzle;
	GripType grip;
	MagazineType magazine;
	StockType stock;
	SightType sight;

	struct {
		int load;
		int total;
	} bullets;
} Weapon;

void init_weapons();

const char *weapon_name(WeaponType type);

#endif
