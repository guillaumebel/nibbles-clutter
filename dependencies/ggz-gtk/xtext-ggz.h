/*
 * File: xtext-ggz.h
 * Author: Jason Short
 * Project: GGZ GTK Client
 * $Id$
 *
 * This file is included from xtext.c to add additional ggz-specific
 * directives.
 *
 * Copyright (C) 2005 GGZ Development Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */


/*
 * xtext.c and xtext.h was taken from xchat-2.6.0.  I then made the following
 * modifications to xtext.c:
 *
 * - Remove the following code from xtext.c
 *     #ifdef XCHAT
 *     #include "../../config.h"
 *     #else
 *     #define USE_XLIB
 *     #endif
 *   and add #include "xtext-ggz.h" in its place.
 * - Add #include "xtext-ggz.h" at the *bottom* of xtext.h.
 *
 * This file therefore contains extra code that xtext.c and xtext.h will
 * need.  It's contained here so that large changes to xtext.c aren't
 * necessary.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#undef XCHAT
#ifndef WIN32
#define USE_XLIB
#endif

#undef XTEXT_MARK_FG
#undef XTEXT_MARK_BG
#undef XTEXT_FG
#undef XTEXT_BG
#undef XTEXT_MARKER

#define XTEXT_MARK_FG 18       /* for marking text */
#define XTEXT_MARK_BG 14
#define XTEXT_FG 18
#define XTEXT_BG 19
#define XTEXT_MARKER 5         /* for marker line */

#include <strings.h> /* For strcasecmp */
