/*
 * Copyright (C) Volition, Inc. 1999.  All rights reserved.
 *
 * All source code herein is the property of Volition, Inc. You may not sell 
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 */

/*
 * $Logfile: /Freespace2/code/UI/LISTBOX.cpp $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * Code to implement a listbox gadget.
 *
 * $Log$
 * Revision 1.5  2004/09/20 01:31:45  theoddone33
 * GCC 3.4 fixes.
 *
 * Revision 1.4  2004/07/04 11:39:06  taylor
 * fix missing debrief text, crash on exit, path separator's, warning fixes, no GR_SOFT
 *
 * Revision 1.3  2002/06/09 04:41:29  relnev
 * added copyright header
 *
 * Revision 1.2  2002/05/07 03:16:53  theoddone33
 * The Great Newline Fix
 *
 * Revision 1.1.1.1  2002/05/03 03:28:11  root
 * Initial import.
 *
 * 
 * 4     12/02/98 5:47p Dave
 * Put in interface xstr code. Converted barracks screen to new format.
 * 
 * 3     10/13/98 9:29a Dave
 * Started neatening up freespace.h. Many variables renamed and
 * reorganized. Added AlphaColors.[h,cpp]
 * 
 * 2     10/07/98 10:54a Dave
 * Initial checkin.
 * 
 * 1     10/07/98 10:51a Dave
 * 
 * 20    3/23/98 5:48p Hoffoss
 * Improved listbox handling.  Most notibly the scrollbar arrows work now.
 * 
 * 19    2/03/98 4:21p Hoffoss
 * Made UI controls draw white text when disabled.
 * 
 * 18    1/20/98 10:36a Hoffoss
 * Fixed optimized warnings.
 * 
 * 17    1/14/98 6:43p Hoffoss
 * Massive changes to UI code.  A lot cleaner and better now.  Did all
 * this to get the new UI_DOT_SLIDER to work properly, which the old code
 * wasn't flexible enough to handle.
 * 
 * 16    9/18/97 10:31p Lawrance
 * allow listbox to change text
 * 
 * 15    9/09/97 4:32p Dave
 * Added sel_changed() function to UI_LISTBOX to check is the selection
 * has changed.
 * 
 * 14    9/09/97 3:39p Sandeep
 * warning level 4 bugs
 * 
 * 13    8/19/97 1:27p Dave
 * Made it possible to create an empty list box and summarily add items.
 * 
 * 12    8/14/97 5:23p Dave
 * Added clear_all_items() to the UI_LISTBOX
 * 
 * 11    6/26/97 5:53p Lawrance
 * change code slightly, so current_item is not forced to 0 when was -1
 * 
 * 10    6/12/97 12:39p John
 * made ui use freespace colors
 * 
 * 9     6/11/97 1:13p John
 * Started fixing all the text colors in the game.
 * 
 * 8     5/26/97 10:26a Lawrance
 * get slider control working 100%
 * 
 * 7     4/22/97 10:11a John
 * Added checkbox lists to listboxes
 * 
 * 6     4/15/97 3:47p Allender
 * moved type selection of list box items into actual UI code.  Made it
 * behave more like windows listboxes do
 * 
 * 5     12/23/96 2:42p Lawrance
 * allowing keys to select list box items in the mission load screen
 * 
 * 4     12/02/96 2:17p John
 * Made right button drag UI gadgets around and
 * Ctrl+Shift+Alt+F12 dumps out where they are.
 * 
 * 3     12/01/96 3:48a Lawrance
 * added function set_current to UI_LISTBOX
 * 
 * 2     11/15/96 11:43a John
 * 
 * 1     11/14/96 6:55p John
 *
 * $NoKeywords: $
 */

#include "uidefs.h"
#include "ui.h"
#include "timer.h"
#include "key.h"
#include "alphacolors.h"

#define KEY_BUFFER_TIMEOUT		1000		// time to clear buffer in milliseconds

#define DEFAULT_LISTBOX_ITEM_LENGTH 40

// --------------------------------------------------------------------
// UI_LISTBOX::link_hotspot
//
//
void UI_LISTBOX::link_hotspot(int up_button_num, int down_button_num)
{
	scrollbar.link_hotspot(up_button_num,down_button_num);
}

// --------------------------------------------------------------------
// UI_LISTBOX::set_bmaps
//
// Call the UI_SCROLLBAR::set_bmaps() function for the scroll bar gadget.
//
// returns:		-1 ==> error
//					 0 ==> success
//
int UI_LISTBOX::set_bmaps(const char *lbox_fname, const char *b_up_fname, const char *b_down_fname, const char *sb_fname)
{
	if (has_scrollbar) {
		scrollbar.set_bmaps(b_up_fname, b_down_fname, sb_fname);
	}
	
	// set the bitmaps for the list box rectangle
	UI_GADGET::set_bmaps(lbox_fname);
	uses_bmaps = 1;
	return 0;
}

void UI_LISTBOX::create(UI_WINDOW *wnd, int _x, int _y, int _w, int _h, int _numitems, char **_list, char *_check_list, int _max_items)
{
	int tw, th, nrows;
	int real_h;

	gr_set_font(wnd->f_id);
	gr_get_string_size(&tw, &th, "*");

	nrows = _h / th;
	real_h = nrows * th;

	base_create( wnd, UI_KIND_LISTBOX, _x, _y, _w, real_h );

   max_items = _max_items;	
   list = _list;
	num_items = _numitems;
	check_list = _check_list;	
	num_items_displayed = nrows;

	first_item = 0;
	current_item = -1;
	toggled_item = -1;
	last_scrolled = 0;
	textheight = th;
	dragging = 0;
	selected_item = -1;
	key_buffer_count = 0;
	last_typed = timer_get_milliseconds();

	if (_numitems > nrows) {
		scrollbar.create( wnd, _x+_w+3, _y, real_h, 0, _numitems-nrows, 0, nrows );
		scrollbar.set_parent(this);
		has_scrollbar = 1;

	} else {
		has_scrollbar = 0;
	}
};

void UI_LISTBOX::draw()
{
	int i, x1, y1, stop;
	int w1, h1;

	UI_GADGET::draw();
	gr_set_font(my_wnd->f_id);

	if (uses_bmaps) {
		if (disabled_flag) {
			if ( bmap_ids[LBOX_DISABLED] >= 0 ) {
				gr_set_bitmap(bmap_ids[LBOX_DISABLED], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(x, y);
			}

		} else {
			if ( bmap_ids[LBOX_NORMAL] >= 0 ) {
				gr_set_bitmap(bmap_ids[LBOX_NORMAL], GR_ALPHABLEND_NONE, GR_BITBLT_MODE_NORMAL, 1.0f, -1, -1);
				gr_bitmap(x, y);
			}
		}

	} else {
		gr_set_color_fast(&CBLACK);
		gr_set_clip( x, y, w, h );
		ui_rect( 0, 0, w-1, h-1 );
		gr_reset_clip();		
		if (has_scrollbar) {
			ui_draw_sunken_border( x-2, y-2, x+w+scrollbar.w+4, y+h+1 );

		} else {
			ui_draw_sunken_border( x-2, y-2, x+w+4, y+h+1 );
		}
	}

	stop = first_item+num_items_displayed;
	if (stop>num_items) stop = num_items;

	x1 = y1 = 0;
	gr_set_clip( x, y, w, h );

	for ( i=first_item; i<stop; i++ ) {
		gr_get_string_size( &w1, &h1,list[i] );

		if (check_list)
			w1 += 18;

		if (i !=current_item) {
/*
			if ((current_item == -1) && (my_wnd->selected_gadget == this ) && (i == first_item)  )	{
				if ( !uses_bmaps ) {
					gr_set_color_fast( &CBLACK );
					gr_rect( x1, y1, w1+2, h1 );
				}
				current_item = first_item;
				gr_set_color_fast( &CBRIGHT_GREEN );
			} else {
				if ( !uses_bmaps ) {
					gr_set_color_fast( &CBLACK );
					gr_rect( x1, y1, w1+2, h1 );
				}
				gr_set_color_fast( &CWHITE );
			}
*/
			if (!uses_bmaps) {
				gr_set_color_fast( &CBLACK );
				gr_rect( x1, y1, w1+2, h1 );
			}

			gr_set_color_fast(&CWHITE);

		} else {
			if (my_wnd->selected_gadget == this) {
				gr_set_color_fast( &CGRAY );
				gr_rect( x1, y1, w1+2, h1 );
				gr_set_color_fast( &CBRIGHT_GREEN );

			} else {
				gr_set_color_fast( &CGRAY );
				gr_rect( x1, y1, w1+2, h1 );
				gr_set_color_fast( &CBLACK );
			}
		}

		if ( check_list )	{
			if ( check_list[i] )	{
				gr_string( x1+2, y1, "X" );
			}

			gr_string( x1+16, y1, list[i] );

		} else
			gr_string( x1+2, y1, list[i] );

		if (i==current_item)
			gr_set_color_fast( &CGRAY );
		else
			gr_set_color_fast( &CBLACK );

		if ( !uses_bmaps ) {
			ui_rect( x1+w1+2, y1, w-1, y1+h1-1 );
			ui_rect( x1, y1, x1+1, y1+h1-1 );
		}

		y1 += h1;
	}

	if (stop < num_items_displayed-1 && !uses_bmaps) {
		gr_set_color_fast(&CBLACK);
		ui_rect( x1, y1, w-1, h-1 );
	}
}

void UI_LISTBOX::process(int focus)
{
	int OnMe, mitem, oldbarpos, kf = 0;
	int i, j;

	selected_item = -1;
	toggled_item = -1;

	if (disabled_flag)
		return;

	if (my_wnd->selected_gadget == this)
		focus = 1;

	if (has_scrollbar) {
		scrollbar.process(0);
		if (my_wnd->selected_gadget == &scrollbar) {
			set_focus();
			focus = 1;
		}
	}

	if (num_items < 1) {
		current_item = -1;
		first_item = 0;
		old_current_item = current_item;
		old_first_item = first_item;
		
//		if (my_wnd->selected_gadget == this) {
//			my_wnd->selected_gadget == get_next();
//		}

		return;
	}

	old_current_item = current_item;
	old_first_item = first_item;

	OnMe = is_mouse_on();

	if (has_scrollbar) {
		if (scrollbar.moved) {
			first_item = scrollbar.position;
			Assert(first_item >= 0);

			if (current_item<first_item)
				current_item = first_item;

			if (current_item > first_item + num_items_displayed - 1)
				current_item = first_item + num_items_displayed - 1;
		}
	}

	if (!B1_PRESSED)
		dragging = 0;

	if (B1_PRESSED && OnMe) {
		set_focus();
		dragging = 1;
	}

	if ( key_buffer_count && (timer_get_milliseconds() > last_typed + KEY_BUFFER_TIMEOUT) )
		key_buffer_count = 0;

	if (focus) {
		if (my_wnd->keypress) {
			kf = 0;

			switch (my_wnd->keypress) {
				case KEY_ENTER:
					selected_item = current_item;
					break;

				case KEY_SPACEBAR:
					toggled_item = current_item;
					break;

				case KEY_UP:
					current_item--;
					kf = 1;
					break;

				case KEY_DOWN:
					current_item++;
					kf = 1;
					break;

				case KEY_HOME:
					current_item = 0;
					kf = 1;
					break;

				case KEY_END:
					current_item=num_items - 1;
					kf = 1;
					break;

				case KEY_PAGEUP:
					current_item -= num_items_displayed;
					kf = 1;
					break;

				case KEY_PAGEDOWN:
					current_item += num_items_displayed;
					kf = 1;
					break;

				default:		// enter the key in the key buffer
					if (my_wnd->keypress == KEY_BACKSP) {
						key_buffer_count = 0;

					} else if (key_buffer_count < MAX_KEY_BUFFER) {
						key_buffer[key_buffer_count++] = (char) my_wnd->keypress;
						last_typed = timer_get_milliseconds();
					}

					if (!key_buffer_count)
						break;

					for (i=0; i<num_items; i++) {
						char *current_text;
						
						current_text = get_string(i);
						for (j=0; j<key_buffer_count; j++)
							if ( (current_text[j] != ascii_table[(int)key_buffer[j]]) && (current_text[j] != shifted_ascii_table[(int)key_buffer[j]]) )
								break;

						if (j == key_buffer_count) {
							set_first_item(i - num_items_displayed / 2);
							set_current(i);
							break;
						}
					}
			}
		}

		if (kf == 1) {
			if (current_item < 0)
				current_item = 0;

			if (current_item >= num_items)
				current_item = num_items - 1;

			if (current_item < first_item)
				first_item = current_item;

			if (current_item >= first_item + num_items_displayed)
				first_item = current_item - num_items_displayed + 1;

			if (num_items <= num_items_displayed ) {
				first_item = 0;

			} else {
				if (has_scrollbar) {
					oldbarpos = scrollbar.position;
					scrollbar.position = first_item;

					scrollbar.bar_position = scrollbar.position - scrollbar.start;
					scrollbar.bar_position *= scrollbar.h - scrollbar.bar_size;
					scrollbar.bar_position /= scrollbar.stop - scrollbar.start;

					if (scrollbar.bar_position < 0) {
						scrollbar.bar_position = 0;
					}
		
					if (scrollbar.bar_position > scrollbar.h - scrollbar.bar_size) {
						scrollbar.bar_position = scrollbar.h - scrollbar.bar_size;
					}
				}	
	
			}
		}
	}

	if (focus) {
		if (B1_PRESSED && dragging) {
			if (ui_mouse.y < y )
				mitem = -1;
			else
				mitem = (ui_mouse.y - y)/textheight;

			if ( (mitem < 0) && (timer_get_milliseconds() > last_scrolled + 1000 / 18) ) {
				current_item--;
				last_scrolled = timer_get_milliseconds();
			}

			if ( (mitem >= num_items_displayed) && (timer_get_milliseconds() > last_scrolled + 1000 / 18) ) {
				current_item++;
				last_scrolled = timer_get_milliseconds();
			}

			if ((mitem >= 0) && (mitem<num_items_displayed)) {
				current_item = mitem + first_item;
			}

			if (current_item < 0)
				current_item = 0;

			if (current_item >= num_items)
				current_item = num_items - 1;

			if (current_item < first_item)
				first_item = current_item;

			if (current_item >= first_item + num_items_displayed)
				first_item = current_item - num_items_displayed + 1;

			if (num_items <= num_items_displayed) {
				first_item = 0;

			} else if (has_scrollbar) {
				oldbarpos = scrollbar.position;
				scrollbar.position = first_item;

				scrollbar.bar_position = scrollbar.position - scrollbar.start;
				scrollbar.bar_position *= scrollbar.h - scrollbar.bar_size;
				scrollbar.bar_position /= scrollbar.stop - scrollbar.start;

				if (scrollbar.bar_position < 0) {
					scrollbar.bar_position = 0;
				}

				if (scrollbar.bar_position > scrollbar.h - scrollbar.bar_size) {
					scrollbar.bar_position = scrollbar.h - scrollbar.bar_size;
				}
			}
		}

		if (check_list) {
			if (B1_JUST_RELEASED)
				toggled_item = current_item;
		}

		if (B1_DOUBLE_CLICKED) {
			selected_item = current_item;
		}
	}
}

int UI_LISTBOX::toggled()
{
	if (check_list) {
		return toggled_item;
	} else {
		return -1;
	}
}

int UI_LISTBOX::selected()
{
	if (check_list) {
		return -1;
	} else {
		return selected_item;
	}
}

int UI_LISTBOX::current()
{
	return current_item;
}

void UI_LISTBOX::set_current(int _index)
{
	current_item = _index;
}

void UI_LISTBOX::set_first_item(int _index)
{
	if (_index < 0)
		_index = 0;
	else if (_index > num_items)
		_index = num_items;

	first_item = _index;
}

char *UI_LISTBOX::get_string(int _index)
{
	return list[_index];
}

void UI_LISTBOX::clear_all_items()
{
   int idx;

	for ( idx=0; idx<num_items; idx++ )
		list[idx][0] = 0;

	num_items = 0;
	first_item = 0;
}

void UI_LISTBOX::set_new_list(int _numitems, char **_list)
{
 	num_items = _numitems;
	list = _list;
	current_item = 0;
}

int UI_LISTBOX::add_string(char *str)
{
   if (max_items < 0)  // only if we created an "empty" listbox can we add items
		return 0;

	else {
		if ( (num_items == max_items - 1) || (strlen(str) > DEFAULT_LISTBOX_ITEM_LENGTH) )
			return 0;                     // we've reached our limit

		else {
			list[num_items] = strdup(str);
			num_items++;
			return 1;
		}
	}
}

int UI_LISTBOX::sel_changed()
{
	return old_current_item == current_item ? 0 : 1;
}

