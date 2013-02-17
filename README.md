# gridtop (very early stages)
https://github.com/flipcoder/gridtop

Copyright (c) 2013, Grady O'Connell

A tiled window managing application that can be run on top of Unity, Gnome,
KDE, Cinnamon, etc.  As long as your DE is EWMH compliant.

It will have vim-like keys for changing focus of windows (win+hjkl), as well as
movement, and resizing.  It will support multiple monitors and workspaces.

Right now it's just a command line program that you can bind to keys.  I will
be switching it to being a background daemon soon so I can have persistence,
like window position history, and tags.

I'm very ambitious with my goals usually, but I'd like this to eventually
replace any need for someone to run a tiled window manager.  Developers living
in separate DEs tend to stop focusing on the average user experience.  This is
one of the reasons I think the linux desktop appears weaker than it should.

# Features and Plans

The features I intend to add are in this order:
- Directional focusing/alt-tab (win+hjkl) [Done]
    - Something that is seriously missing in modern window managers is
      directional focusing.  If i want to move to the window directly on my
      left, I should be able to hit win+h (or equivalent) and it should be
      instant.
    - I don't want to be required to enter a "tiled mode" to have this
      behavior, so it will function even when windows are floating freely.
    - I want more than just two directions of motion:
      We will start with the following vim-like motions for grid switching:
          h, j, k, l, $, ^, G, and gg

- Development plans
    - I'll be switching to libwnck (instead of wmctrl+xprop calls) soon.
    - The program itself right now is called for each action.  I'll switch
      to a background daemon that is launched and each command will be done
      through IPC.  (I already have the code for this, I just haven't added
      it to this project.)
    - I'm not sure if I want this program to handle the hotkeys itself or if
      they should be bound by the user.  The more vi-like I make it, the more
      I'm considering having different "modes" that change the effect of
      hotkeys as the program runs.
      
- Window actions, in order of priority
    - *place*
        - the active window is pushed into the area with greatest remaining area, to prevent overlapping another window.
          if the window does not overlap anything, it is simply expanded.
    - *fit*
        - windows that are overlapping are pushed away from eachother and possibly resized if the push hits the edge of the screen.  Variable-sized border "gaps" will be supported.  If the fit fails, the most (in terms of area) overlapped windows will be minimized.
    - *fill*
        - the active window is expanded as far as possible without overlap.
    - *snap*
        - windows are snapped to a user-defined grid making it nicer to resize manually.    
    - *minimized swap*
        - the active window will be swapped with the next minimized window.
    - *and much more, of course*

