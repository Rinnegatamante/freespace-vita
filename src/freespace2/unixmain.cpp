#include <sys/stat.h>

#include "pstypes.h"
#include "osregistry.h"
#include "osapi.h"

#undef malloc
#undef free

#ifdef __vita__
#include <vitasdk.h>
#include <vitaGL.h>
#include <dirent.h>
int _newlib_heap_size_user = 256 * 1024 * 1024;

#ifdef FS1_DEMO
#define ROOT_PATH "ux0:data/freespace_demo"
#elif defined(FS2_DEMO)
#define ROOT_PATH "ux0:data/freespace2_demo"
#elif defined(MAKE_FS1)
#define ROOT_PATH "ux0:data/freespace"
#else
#define ROOT_PATH "ux0:data/freespace2"
#endif

char patched_fname[256];
void patch_fname(char *fname) {
	if (!strncmp(fname, "a:\\", 3)) {
		sprintf(patched_fname, "%s/%s", ROOT_PATH, &fname[3]);
	} else if (!strncmp(fname, "(null)/", 7)) {
		sprintf(patched_fname, "%s/%s", ROOT_PATH, &fname[7]);
	} else {
		sprintf(patched_fname, "%s/%s", ROOT_PATH, fname);
	}
}

#define X_RATIO (1280.0f / 1450.0f)
#define Y_RATIO (960.0f / 1088.0f)

extern "C" int __real_sceTouchPeek(SceUInt32 port, SceTouchData *pData, SceUInt32 nBufs);
extern "C" int __wrap_sceTouchPeek(SceUInt32 port, SceTouchData *pData, SceUInt32 nBufs) {
	int num = __real_sceTouchPeek(port, pData, nBufs);
	if (pData->reportNum) {
		pData->report[0].x -= 235;
		pData->report[0].x = (int)((float)pData->report[0].x * X_RATIO);
		pData->report[0].y = (int)((float)pData->report[0].y * Y_RATIO);
	}
	if (pData->report[0].x < 0)
		pData->report[0].x = 0;
	return num;
}

extern "C" int __real_remove(const char *fname);
extern "C" int __wrap_remove(const char *fname) {
	//printf("remove %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_remove(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_remove(patched_fname);
}

extern "C" FILE *__real_fopen(char *fname, char *mode);
extern "C" FILE *__wrap_fopen(char *fname, char *mode) {
	//printf("fopen %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_fopen(fname, mode);
	} else {
		patch_fname(fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_fopen(patched_fname, mode);
}

extern "C" int __real_stat(const char *fname, struct stat *buf);
extern "C" int __wrap_stat(const char *fname, struct stat *buf) {
	//printf("stat %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_stat(fname, buf);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_stat(patched_fname, buf);
}

extern "C" DIR *__real_opendir(const char *fname);
extern "C" DIR *__wrap_opendir(const char *fname) {
	//printf("opendir %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_opendir(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_opendir(patched_fname);
}
#endif

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow);

#if defined(__APPLE__) && !defined(MACOSX)
char full_path[1024];
#endif

void vm_dump();

int main(int argc, char **argv)
{
	char userdir[MAX_PATH] = { 0 };
#ifdef __vita__
	argc = 0;
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	int ret = sceNetShowNetstat();
	SceNetInitParam initparam;
	if (ret == SCE_NET_ERROR_ENOTINIT) {
		initparam.memory = malloc(141 * 1024);
		initparam.size = 141 * 1024;
		initparam.flags = 0;
		sceNetInit(&initparam);
	}
	vglInitExtended(0, 960, 544, 4 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
	scePowerSetArmClockFrequency(444);
	scePowerSetBusClockFrequency(222);
	scePowerSetGpuClockFrequency(222);
	scePowerSetGpuXbarClockFrequency(166);

#ifdef FS1_DEMO
	
#elif defined(FS2_DEMO)
#include "../vita/freespace2_demo.h"
	SceIoStat st;
	if (sceIoGetstat("ux0:data/freespace2_demo/.freespace2_demo/Data/Players/Single/Player.plr", &st) < 0) {
		sceIoMkdir("ux0:data/freespace2_demo/.freespace2_demo", 0777);
		sceIoMkdir("ux0:data/freespace2_demo/.freespace2_demo/Data", 0777);
		sceIoMkdir("ux0:data/freespace2_demo/.freespace2_demo/Data/Players", 0777);
		sceIoMkdir("ux0:data/freespace2_demo/.freespace2_demo/Data/Players/Single", 0777);
		FILE *f = fopen("ux0:data/freespace2_demo/.freespace2_demo/Data/Players/Single/Player.plr", "wb");
		fwrite(def_save, 1, size_def_save, f);
		fclose(f);
	}
#elif defined(MAKE_FS1)
#include "../vita/freespace1.h"
	SceIoStat st;
	if (sceIoGetstat("ux0:data/freespace/.freespace/Data/Players/Single/Player.plr", &st) < 0) {
		sceIoMkdir("ux0:data/freespace/.freespace", 0777);
		sceIoMkdir("ux0:data/freespace/.freespace/Data", 0777);
		sceIoMkdir("ux0:data/freespace/.freespace/Data/Players", 0777);
		sceIoMkdir("ux0:data/freespace/.freespace/Data/Players/Single", 0777);
		FILE *f = fopen("ux0:data/freespace/.freespace/Data/Players/Single/Player.plr", "wb");
		fwrite(def_save, 1, size_def_save, f);
		fclose(f);
	}
#else
#include "../vita/freespace2.h"
	SceIoStat st;
	if (sceIoGetstat("ux0:data/freespace2/.freespace2/data/players/single/Player.plr", &st) < 0) {
		sceIoMkdir("ux0:data/freespace2/.freespace2", 0777);
		sceIoMkdir("ux0:data/freespace2/.freespace2/data", 0777);
		sceIoMkdir("ux0:data/freespace2/.freespace2/data/players", 0777);
		sceIoMkdir("ux0:data/freespace2/.freespace2/data/players/single", 0777);
		FILE *f = fopen("ux0:data/freespace2/.freespace2/data/players/single/Player.plr", "wb");
		fwrite(def_save, 1, size_def_save, f);
		fclose(f);
	}
#endif
#endif


#if defined(__APPLE__) && !defined(MACOSX)
	strcpy( full_path, *argv );
#endif

	// create user game directory
	snprintf(userdir, MAX_PATH, "%s/%s/", detect_home(), Osreg_user_dir);
	_mkdir(userdir);	
	
	char *argptr = NULL;
	int i;
	int len = 0;

	for (i = 1; i < argc; i++) {
		len += strlen(argv[i]) + 1;
	}

	argptr = (char *)malloc(len+5);

	if (argptr == NULL) {
		fprintf(stderr, "ERROR: out of memory in main!\n");
		exit(1);
	}

	memset(argptr, 0, len+5);

	for (i = 1; i < argc; i++) {
		strcat(argptr, argv[i]);
		strcat(argptr, " ");
	}

	int retr = WinMain(1, 0, argptr, 0);

	free(argptr);
		
	vm_dump();
	
	return retr;	
}
