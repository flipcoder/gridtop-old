# gridtop (very early stages)
https://github.com/flipcoder/gridtop
Copyright (c) 2013, Grady O'Connell

A grid-based tiled window managing application that can be run on top of any
EMWH-compliant window manager.

Gridtop commands can be specified indivdually on the command line.  They are
then piped to the background daemon (which is spawned if it is not running).
The background daemon deals with managing all the window positions, scheduling
animations and state changes based on the user-specified operators and motions.

I'm very ambitious with my goals usually, but I'd like this to eventually
replace any need for someone to run a tiled window manager.  Developers living
in separate DEs tend to stop focusing on the average user experience.  This is
one of the reasons I think the linux desktop appears weaker than it should.

My motivation for writing this is that I dislike the way window managers
(including tiled) deal with managing and cycling through windows.  This is my
attempt at creating the ideal window manager.

It is grid-based, meaning that windows prefer to snap to a specific grid, as
well as preserving spacing.  Everything should be user configurable.

Windows are manipulated through vim-like commands.

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
    - *operators*
        - *focus* (default)
            - changes which window is active
        - *move/snap*
            - moves the window along the grid
            - if the operator is specified twice, the active window is just
              snapped to the grid
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

