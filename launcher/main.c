#include <vitasdk.h>
#include <kubridge.h>
#include <vitashark.h>
#include <vitaGL.h>

#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <wchar.h>
#include <wctype.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_ONLY_PNG
#include "stb_image.h"

#define SCREEN_W 960
#define SCREEN_H 544
#define NUM_GAMES 3

int file_exists(const char *path) {
	SceIoStat stat;
	return sceIoGetstat(path, &stat) >= 0;
}

char *game_checks[NUM_GAMES] = {
	"ux0:data/freespace/root.vp", // Freespace 1
	"ux0:data/freespace2/Root_fs2.vp", // Freespace 2
	"ux0:data/freespace2_demo/root_fs2.vp" // Freespace 2 Demo
};

int get_msg_dialog_result(void) {
	if (sceMsgDialogGetStatus() != SCE_COMMON_DIALOG_STATUS_FINISHED)
		return 0;
	sceMsgDialogTerm();
	return 1;
}

int init_msg_dialog(const char *msg) {
	SceMsgDialogUserMessageParam msg_param;
	memset(&msg_param, 0, sizeof(msg_param));
	msg_param.buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
	msg_param.msg = (SceChar8 *)msg;

	SceMsgDialogParam param;
	sceMsgDialogParamInit(&param);
	_sceCommonDialogSetMagicNumber(&param.commonParam);
	param.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
	param.userMsgParam = &msg_param;

	return sceMsgDialogInit(&param);
}

void fatal_error(const char *fmt, ...) {
	va_list list;
	char string[512];

	va_start(list, fmt);
	vsnprintf(string, sizeof(string), fmt, list);
	va_end(list);

	vglInit(0);
	
	printf(string);
	init_msg_dialog(string);

	while (!get_msg_dialog_result())
		vglSwapBuffers(GL_TRUE);

	sceKernelExitProcess(0);
	while (1);
}

int main(int argc, char *argv[]) {
	scePowerSetArmClockFrequency(444);
	scePowerSetBusClockFrequency(222);
	scePowerSetGpuClockFrequency(222);
	scePowerSetGpuXbarClockFrequency(166);

	if (!file_exists("ur0:/data/libshacccg.suprx") && !file_exists("ur0:/data/external/libshacccg.suprx"))
		fatal_error("Error: libshacccg.suprx is not installed.");
	
	vglInitExtended(0, SCREEN_W, SCREEN_H, 8 * 1024 * 1024, SCE_GXM_MULTISAMPLE_NONE);
	
	// Setting up game selector
	char fname[256];
	GLuint textures[NUM_GAMES];
	glGenTextures(NUM_GAMES, textures);
	GLboolean available_games[NUM_GAMES] = {GL_FALSE, GL_FALSE, GL_FALSE};
	GLboolean has_at_least_one_game = GL_FALSE;
	
	for (int i = 0; i < NUM_GAMES; i++) {
		if (file_exists(game_checks[i])) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			int w, h;
			sprintf(fname, "app0:images/game%d.png", i + 1);
			uint8_t *tex_data = (uint8_t *)stbi_load(fname, &w, &h, NULL, 4);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
			free(tex_data);
			available_games[i] = GL_TRUE;
			has_at_least_one_game = GL_TRUE;
		}
	}
	if (!has_at_least_one_game)
		fatal_error("Error: No games detected.");
	
	// Game Selector main loop
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glViewport(0, 0, SCREEN_W, SCREEN_H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_W, SCREEN_H, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float tex_vertices[] = {
		       0,        0, 0,
		SCREEN_W,        0, 0,
		SCREEN_W, SCREEN_H, 0,
		       0, SCREEN_H, 0
	};
	float tex_texcoords[] = {0, 0, 1, 0, 1, 1, 0, 1};
	int8_t selector_idx = 0;
	while (!available_games[selector_idx]) {
		selector_idx = (selector_idx + 1) % NUM_GAMES;
	}
	uint32_t oldpad = 0;
	for (;;) {
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(0, &pad, 1);
		if ((pad.buttons & SCE_CTRL_LEFT) && !(oldpad & SCE_CTRL_LEFT)) {
			do {
				selector_idx--;
				if (selector_idx < 0)
					selector_idx = NUM_GAMES - 1;
			} while (!available_games[selector_idx]);
		} else if ((pad.buttons & SCE_CTRL_RIGHT) && !(oldpad & SCE_CTRL_RIGHT)) {
			do {
				selector_idx = (selector_idx + 1) % NUM_GAMES;
			} while (!available_games[selector_idx]);
		} else if ((pad.buttons & SCE_CTRL_CROSS) && !(oldpad & SCE_CTRL_CROSS))
			break;
		oldpad = pad.buttons;
		glBindTexture(GL_TEXTURE_2D, textures[selector_idx]);
		glVertexPointer(3, GL_FLOAT, 0, tex_vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, tex_texcoords);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		vglSwapBuffers(GL_FALSE);
	}
	
	switch (selector_idx) {
	case 0:
		sceAppMgrLoadExec("app0:freespace.bin", NULL, NULL);
		break;
	case 1:
		sceAppMgrLoadExec("app0:freespace2.bin", NULL, NULL);
		break;
	default:
		sceAppMgrLoadExec("app0:freespace2_demo.bin", NULL, NULL);
		break;
	}
	return 0;
}