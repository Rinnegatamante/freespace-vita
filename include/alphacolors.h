/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

#ifndef _GLOBAL_ALPHACOLORS_HEADER_FILE
#define _GLOBAL_ALPHACOLORS_HEADER_FILE

// -----------------------------------------------------------------------------------
// ALPHA DEFINES/VARS
//

// Colors for UI
// See Freespace.cpp for usage

// The following colors are for text drawing:
// normal text
#define Color_text_normal			Color_white
// text highlighted while down still down on a line
#define Color_text_subselected	Color_blue
// text highlighted as the selected line
#define Color_text_selected		Color_bright_blue
// text that indicates an error
#define Color_text_error			Color_red
// text that indicates an error, and line is selected or subselected
#define Color_text_error_hi		Color_bright_red
// text that indicates line is active item
#define Color_text_active			Color_bright_white
// text that indicates line is active item, and line is selected or subselected
#define Color_text_active_hi		Color_bright_white
// text drawn as a heading for a section or title, etc
#define Color_text_heading			Color_violet_gray

#define Color_bright Color_bright_blue
#define Color_normal Color_white
extern color Color_black, Color_grey, Color_blue, Color_bright_blue, Color_violet_gray;
extern color Color_green, Color_bright_green, Color_bright_white, Color_white;
extern color Color_red, Color_bright_red, Color_yellow, Color_bright_yellow, Color_dim_red;

extern color Color_ui_light_green, Color_ui_green;
extern color Color_ui_light_pink, Color_ui_pink;

// netplayer colors
extern color *Color_netplayer[12];

// -----------------------------------------------------------------------------------
// ALPHA FUNCTIONS
//

// initialize all alpha colors (call at startup)
void alpha_colors_init();


#endif

