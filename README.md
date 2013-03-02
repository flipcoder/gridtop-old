# gridtop (very close to being usable)
https://github.com/flipcoder/gridtop
Copyright (c) 2013, Grady O'Connell

A grid-based tiled window managing application that can be run on top of any
EMWH-compliant window manager.

# Features and Plans

The features I intend to add are in this order:
- Directional focusing/alt-tab (win+hjkl) [Almost done]
    - Something that is seriously missing in non-tiled window managers is
      directional focusing.  If i want to move to the window directly on my
      left, I should be able to hit win+h (or equivalent) and it should be
      instant.
    - I don't want to be required to enter a "tiled mode" to have this
      behavior, so it will function even when windows are floating freely.
    - I want more than just two directions of motion.
- Toggling window decorations
- Window animations/easing [Done]
    - Not just because it looks awesome, but beacuse it is more clear what
      is going on.
- Development plans
    - Switch to libwnck [Done]
    - Switch to daemon+IPC [Done]
    - Eventually, i'd like to explore using mouse+touch commands to make using
      this more user-friendly. (A user-friendly tiled window manager!?)
      I'll codename this feature "What-Windows-8-Should've-Been"

- Window actions, in order of priority
    - *modes*
        - *visual* (coming later)
            - multi-select windows or grid positions, to apply commands to more than one window
    - *context*
        - *window*
        - *workspace*
        - *display*
    - *operators*
        - *focus* (default)
            - changes which window is active
        - *move*
            - moves the window along the grid
            - if the operator is specified twice or with motion *self*, the
              active window is just snapped to the grid
        - *split*
            - halves the window and spawns a new window depending on the
              parameters
        - *swap*
            - swap active window with the window matching the given motion
        - *extend*
            - spawns a new window in the direction depending on the motion
        - *place*
            - the active window is pushed into the area with greatest remaining area, to prevent overlapping another window.
              if the window does not overlap anything, it is simply expanded.
        - *push*
            - moves the active window in the motion's direction until it collides with another window or display edge
        - *fill*
            - the active window is expanded as far as possible without overlap in the given direction
        - *stick*
            - sticks the window to the window in the target direction, so that movement
              of the parent will reposition this window.
        - *conceal*
            - moves active window behind target so that it can be cycled
        - *cycle*
            - moves through conceal windows behind active window
    - *motions*
        - By default, motions refer to windows in the specific direction, but
          I'll add a way to extend these to allow for targeting workspaces and
          displays similarly.
        - *self*
            - operator-specific, much like 'dd' is to 'd' in vim, it usually
              applies only the current window, and can also be specified by
              triggering the operator twice.
        - *left*
        - *right*
        - *up*
        - *down*
        - *left-edge*
        - *right-edge*
        - *top*
        - *bottom*
        - *tag* (eventually)

# Usage

The features discussed below are for future use, since the program is not yet
in a usable state.  It's missing just a few essential components before it will
be functional.  Check back often :)

Use xbindkeys (recommended) or another hotkey application to set up your keys.

Set hotkeys to call the program with the following arguments for each motion:

For vim style keys, it is recommended you bind win+h,j,k,l for motions
For a more arrow-key-like layout, it is recommend to bind win+i,j,k,l in the
shape of the arrow keys if you want a more conventional feel.

Each motion is listed here:

    gridtop left
    gridtop right
    gridtop up
    gridtop down
    gridtop left-edge
    gridtop right-edge
    gridtop top-edge
    gridtop bottom-edge
    gridtop self (*not necessary to bind*)

By default, motions trigger window focus changes.

Now, bind your other operator keys

    gridtop focus (*default*)
    gridtop move

*many more to come, read features list*

There is a slight delay between key evaluation after you press an operator,
before you are switched back into focus mode, this allows us to use our
hotkeys together, much like a language.

Change modes by specifying their names:

    gridtop window *(default)*
    gridtop workspace
    gridtop display

If you wish to bind multiple commands to one key, such as, switching window
focus to the furthest window on the right, use:

    gridtop window focus right-edge

In the above example, *window* changes the mode, *focus* is the operator/verb,
and *right-edge* is the motion.  The commands together focuses the window on
the farthest right edge of the workspace.

It is recommended you keep the hotkeys to a single motion, operator, or hint
to make efficient use out of each bind.

# How does it work?

Gridtop commands are specified as parameters to the gridtop application.
The new instance then sends the parameters to the background daemon (which is
spawned if it is not running). The background daemon deals with managing all
the window positions, scheduling animations and state changes based on the
user-specified operators and motions.

The most important parts of the code are inside WindowManager and
CommandResolver(.cpp/h), which sets up a factory for creating commands, pushing
them onto the "pending" stack before calling the operators' execute().

# Progress

I'd like this to eventually replace any need for someone to run a tiled window
manager.  Developers living in separate DEs tend to stop focusing on the
average user experience.  This is one of the reasons I think the linux desktop
appears weaker than it should, at least to the average person.

My motivation for writing this is that I dislike the way window managers
(including tiled) deal with managing and cycling through windows.  This is my
attempt at creating the ideal window manager.

Progress-wise, I hack on it only every once in a while, and it is quickly
approaching a usable state.  Check back frequently for more updates.

