# Expand file format
I need to expand the file format to make it more scalable. Right now it just contains the position and scales of the format, but I would like to expand it to contain more data like textures, models, etc. and for each assets to have a key.

I would also like to store multiple levels in one file.

## Current format

- int count: Amount of positions and sizes
- count * Vector2 positions: The position data for the level.
- count * Vector2 sizes: The size data for the level.

## New format

- int levelCount: Amount of levels in the file
for each level:
- long key: the level key
- int levelSize: the size of the data for this level
- int assetCount: Amount of assets in this level
for each asset:
- string key: the key of the asset
- long assetSize: the size of the asset
- the rest will depend on the type of asset
