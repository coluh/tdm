#ifndef _TDM_config_h
#define _TDM_config_h

typedef struct Config {
	struct {
		float normal;
	} sensitivity;
} Config;

void config_load(const char *path);
const Config *config_get();

#endif
