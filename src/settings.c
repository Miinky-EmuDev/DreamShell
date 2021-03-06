/****************************
 * DreamShell ##version##   *
 * settings.c               *
 * DreamShell settings      *
 * Created by SWAT          *
 * http://www.dc-swat.ru    *
 ***************************/

#include <ds.h>

static const char settings_file[] = "/vmu/a1/DS_CORE4.CFG";
static Settings_t current_set;
static int loaded = 0;

const static unsigned short DS_pal[16]= {
	0xF000,0xF655,0xF300,0xFECC,0xFCBB,0xF443,0xFA99,0xF222,
	0xFA88,0xF500,0xF988,0xF100,0xF877,0xF844,0xF700,0xF500
};

const static unsigned char DS_data[32*32/2]= {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x22,0xB0,0x00,0x00,0x00,0x55,0x5B,0x00,0x00,0xB1,0x17,0x00,0x00,0x00,0x00,0x00,
	0xB2,0x22,0x22,0xB0,0x05,0x38,0x63,0x10,0x05,0x3A,0xC4,0x50,0x00,0x00,0x00,0x00,
	0x00,0x00,0xB2,0x29,0x25,0x40,0x07,0x37,0x04,0x10,0x00,0x00,0x00,0x00,0x00,0x00,
	0x22,0x22,0x22,0x22,0xFD,0x4E,0x92,0x88,0x04,0xCB,0xBB,0xBB,0xB0,0x00,0x00,0x00,
	0x00,0xBB,0xBB,0xBB,0xB5,0x42,0x9F,0x14,0x2D,0x34,0xC9,0x22,0x99,0x92,0x22,0x22,
	0x00,0x00,0x00,0x00,0x05,0x40,0x00,0x14,0xBB,0x91,0x43,0x52,0x22,0x99,0x92,0x22,
	0x00,0x00,0x00,0x00,0x05,0x40,0x00,0xC6,0x00,0x00,0xBA,0x42,0x22,0x22,0x22,0x22,
	0x00,0x00,0x00,0x00,0x05,0x40,0x00,0x35,0x00,0x00,0x01,0x40,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x05,0x31,0x14,0xA0,0x06,0x15,0x13,0x50,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x0B,0x1C,0xC5,0x00,0x05,0xC6,0xA7,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


Settings_t *GetSettings() {
	
	if(!loaded && !LoadSettings()) {
		ResetSettings();
	}
	
	return &current_set;
}


void SetSettings(Settings_t *settings) {
	
	if(settings != (Settings_t *)&current_set) {
		memcpy(&current_set, settings, sizeof(current_set));
	}
	
	loaded = 1;
}


void ResetSettings() {
	
	Settings_t *cur = &current_set;
	VideoSettings_t *vid = &current_set.video;
	memset(&current_set, 0, sizeof(current_set));
	
	vid->bpp = 16;
	
	vid->tex_width = 1024;
	vid->tex_height = 512;
	vid->tex_filter = -1;

	vid->virt_width = 640;
	vid->virt_height = 480;
	
	strncpy(cur->app, "Main", 4);
	cur->app[4] = '\0';
	
	strncpy(cur->startup, "/lua/startup.lua", 16);
	cur->startup[16] = '\0';
	
	cur->version = GetVersion();
	
	loaded = 1;
}


int LoadSettings() {
	
	Settings_t *sets;
	uint8 *data;
	vmu_pkg_t pkg;
	size_t size;
	file_t fd;
	
	fd = fs_open(settings_file, O_RDONLY);

	if(fd == FILEHND_INVALID) {
		return 0;
	}
	
	size = fs_total(fd);
	data = calloc(1, size);
	
	if(!data) {
		fs_close(fd);
		return 0;
	}
	
	memset(&pkg, 0, sizeof(pkg));
	fs_read(fd, data, size);
	
	if(vmu_pkg_parse(data, &pkg) < 0) {
		free(data);
		return 0;
	}
	
	sets = (Settings_t *)pkg.data;
	
	if(sets->version != GetVersion()) {
		dbglog(DBG_DEBUG, "%s: Settings file version is different from current version\n", __func__);
	}
	
	size = sizeof(current_set);
	memcpy(&current_set, pkg.data, pkg.data_len > size ? size : pkg.data_len);
	
	return 1;
}


int SaveSettings() {
	
	if(!loaded) {
		return 0;
	}
	
	uint8 *pkg_out;
	vmu_pkg_t pkg;
	int pkg_size;
	file_t fd;
	
	if(FileExists(settings_file)) {
		fs_unlink(settings_file);
	}
	
	fd = fs_open(settings_file, O_CREAT | O_WRONLY);

	if(fd == FILEHND_INVALID) {
		dbglog(DBG_DEBUG, "%s: Can't open for write %s\n", __func__, settings_file);
		return 0;
	}
	
	if(current_set.version != GetVersion()) {
		current_set.version = GetVersion();
	}
	
	memset(&pkg, 0, sizeof(pkg));

	strcpy(pkg.desc_short, "DreamShell Settings");
	strcpy(pkg.desc_long, getenv("VERSION"));
	strcpy(pkg.app_id, "DreamShell");

	pkg.icon_cnt = 1;
	pkg.icon_anim_speed = 0;
	memcpy(pkg.icon_pal, DS_pal, 32);
	pkg.icon_data = DS_data;
	pkg.eyecatch_type = VMUPKG_EC_16BIT;
	pkg.data_len = sizeof(current_set);
	pkg.data = (void *)&current_set;

	vmu_pkg_build(&pkg, &pkg_out, &pkg_size);
	fs_write(fd, pkg_out, pkg_size);
	fs_close(fd);
	free(pkg_out);

	return 1;
}

