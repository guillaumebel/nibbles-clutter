/*
 * File: pick_module.h
 * Author: GGZ Dev Team
 * Project: GGZ Text Client 
 * Date: 11/5/2002
 * $Id$
 *
 * Dialog window to pick a module for your game
 *
 * Copyright (C) 2002 GGZ Development Team.
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

int ask_user_to_pick_module(GGZModule **modules, int *modulenumbers, int *preserve);
void cancel_module_picking(void);
