/* Copyright (C) 2014-2014 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------------
 * The following license statement only applies to this libretro API header (libretro-v2-ext.h).
 * ---------------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * This file is for env callbacks we're going to add after libretro v2 has
 * stabilized, but do not want to add yet because it'd make v2 too big of a
 * change. Some of them will deprecate other env callbacks.
 */

#define RETRO_ENVIRONMENT_SET_VARIABLES ($error)
                                           /* const struct retro_variable * --
                                            * Interface to acquire user-defined information from environment
                                            * that cannot feasibly be supported in a multi-system way.
                                            * 
                                            * The first call must be from retro_set_environment or retro_init.
                                            * Additionally, the core may call RETRO_ENVIRONMENT_SET_VARIABLES
                                            * again during retro_load_game, retro_run, and retro_variable::
                                            * ::change_notify, and may have changed some of the entries.
                                            * However, each 'name', 'values' and 'initial' must be the same
                                            * as for the initial call.
                                            */
#define RETRO_ENVIRONMENT_GET_VARIABLE ($error)
                                           /* struct retro_variable_query * --
                                            * Asks the frontend what value a variable has.
                                            */
enum retro_variable_type
{
   /* Tells that the variable list has ended. */
   RETRO_VARIABLE_TYPE_TERMINATOR,

   /* A separator in the list. Use to group similar items together.
    * All other members are ignored for items of this type. */
   RETRO_VARIABLE_TYPE_SEPARATOR,

   /* Enumeration. 'values' is char**, with one entry for each item; terminated
    * by a NULL.
    * 'initial' is an unsigned int * containing the index of the default value. */
   RETRO_VARIABLE_TYPE_ENUM,

   /* Boolean. 'values' is NULL and 'initial' is bool*. */
   RETRO_VARIABLE_TYPE_BOOL,

   /* Integer. 'values' is int* containing two entries: the lowest and
    * highest valid values, inclusive. 'initial' is also int*, with one entry. */
   RETRO_VARIABLE_TYPE_INT,

   /* Floating point. Same as RETRO_VARIABLE_INT, except with 'float' instead of 'int'.
    * The frontend is responsible for calculating a reasonable step size. */
   RETRO_VARIABLE_TYPE_FLOAT,

   /* A resolution, for example output size.
    * 'values' is NULL; valid sizes are between 1x1 and 65535x65535.
    * The frontend should use retro_game_geometry and the monitor size to
    * set reasonable bounds.
    * 'initial' is unsigned int* with two entries (width and height). */
   RETRO_VARIABLE_TYPE_RESOLUTION,

   RETRO_VARIABLE_TYPE_DUMMY = INT_MAX
};

enum retro_variable_change
{
   /* Changes take effect at the next retro_run, or possibly takes a few frames
    * (<= 0.1 seconds, otherwise DELAYED should be used). */
   RETRO_VARIABLE_CHANGE_INSTANT,

   /* Changes take effect during retro_run, but not instantly; for example,
    * it may be delayed until the next level is loaded. */
   RETRO_VARIABLE_CHANGE_DELAYED,

   /* Only used during retro_load_game, or possibly retro_reset. */
   RETRO_VARIABLE_CHANGE_RESET,

   /* This variable is currently ignored; it is only usable if other options are
    * changed first. If these options get changed, RETRO_ENVIRONMENT_SET_VARIABLES
    * must be called again. */
   RETRO_VARIABLE_CHANGE_WRONG_OPTS,

   /* This variable is not applicable for this game. */
   RETRO_VARIABLE_CHANGE_WRONG_GAME,

   RETRO_VARIABLE_CHANGE_DUMMY = INT_MAX
};

struct retro_variable
{
   /* Variable type. See above. */
   enum retro_variable_type type;
   /* When the implementation will acknowledge changes to this variable.
    * Note that the front is allowed to change variables marked
    * currently unusable. */
   enum retro_variable_change change;

   /* Variable name, to be used internally. Suitable for
    * saving to configuration files. Example: gb_colorize */
   const char *name;
   /* Variable name, to show the user. Suitable for GUIs.
    * Example: Game Boy colorization */
   const char *pub_name;
   /* Variable description. Suitable as a second line in GUIs.
    * Example: Emulate fake colors on black&white games. */
   const char *description;
   /* Possible values. See enum retro_variable_type for what type it has.
    * Example: { "Enabled", "Disabled", NULL }
    * (though that one should be a BOOL instead). */
   void *values;
   /* Default value. Example: 1 */
   void *initial;

   /* Called by the frontend every time this variable changes, or NULL to ignore.
    * Can be different for different variables.
    * ID is the index to the array given to RETRO_ENVIRONMENT_SET_VARIABLES.
    * Separators have IDs, but their value must not be set or queried.
    * 'value' has the same type as 'initial'.
    * Can be called during RETRO_ENVIRONMENT_SET_VARIABLES. */
   void (*change_notify)(unsigned int id, void *value, struct retro_core_data *core_handle);
};

struct retro_variable_query
{
   /* Same ID as in change_notify. Core sets this before calling GET_VARIABLE. */
   unsigned int id;
   /* Same type as initial and change_notify. Front sets this. */
   void *value;
};
