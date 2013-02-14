# gridtop (very early stages)
https://github.com/flipcoder/gridtop
Copyright (c) 2013, Grady O'Connell

A tiled window managing application that can be run on top of Unity, Gnome,
KDE, Cinnamon, etc.  Basically anything supporting wmctrl commands should work.

It will have vim-like keys for changing focus of windows (win+hjkl), as well as
movement, and resizing.  It will support multiple monitors and workspaces.

The features I intend to add are in this order:
- Directional focusing/alt-tab (win+hjkl)
    - Something that is seriously missing in modern window managers is
      directional focusing.  If i want to move to the window directly on my
      left, I should be able to hit win+h (or equivalent) and it should be
      instant.
    - I don't want to be required to enter a "tiled mode" to have this
      behavior, so it will function even when windows are floating freely.
    - I want more than just two directions of motion:
      We will start with the following vim-like motions for grid switching:
          h, j, k, l, $, ^, G, and GG
- Window movement, resizing, and grid snapping
    - *coming later*

