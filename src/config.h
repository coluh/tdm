#ifndef _TDM_config_h
#define _TDM_config_h

#include <raylib.h>
typedef struct Config {
	struct {
		float normal;
	} sensitivity;
	struct {
		Texture2D fog;
	} textures;
} Config;

void config_load(const char *path);
const Config *config_get();

#endif
