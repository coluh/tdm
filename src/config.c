#include "config.h"
#include <stddef.h>

static Config current;

void config_load(const char *path) {
	if (path == NULL) { // default configs
		current.sensitivity.normal = 0.1f;
	}
}

const Config *config_get() {
	return &current;
}
