# Teck
A 2D side scroller hack&slash (at least eventually).

This is my first real project in c, so there might be a lot of things not up to spec for all of you experienced c devs out there, butI really don't care for now. This isn't a super serious project, it's just a hobby project I can have some fun with, and get experience with some lower level stuff.

## Technologies

The project contains of 3 parts:
- core: library written in c with all the common stuff.
- game: the game itself, also written in c.
- editor: the level editor, written in c++.

The entire project uses raylib, because I couldn't be bothered dealing with OpenGl at the same time as learning c. I'll leave that for another project.

## TODO

- [ ] Ditch CPM and link dependencies like a real man.
- [ ] Expand on level data format to make it scalable, without redoing everything.
- [ ] Fix collision detection. Right now it has a bunch of issues.
- [ ] Model rendering. Will probably wait until level data format is done.
- [ ] Textures.
- [ ] Combat + enemies.
