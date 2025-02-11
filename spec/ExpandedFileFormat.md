# Expand file format
I need to expand the file format to make it more scalable. Right now it just contains the position and scales of the level, but I would like to expand it to contain more data like textures, models, etc.
I would also like to store multiple levels in one file.

For this i am planning on implementing a string key system. This will make it easy during the development of the game to add and access levels and assets.

For the data access I might look into implementing a hash map in c, since that could be a fun project. Or if I get lazy, it can always be a long if else chain.

## Current format

- int count: Amount of positions and sizes
- count * Vector2 positions: The position data for the level.
- count * Vector2 sizes: The size data for the level.

## New format

- int levelCount: Amount of levels in the file
- for each level:
    - long key: the level key
    - int levelSize: the size of the data for this level
    - int assetCount: Amount of assets in this level
    - for each asset:
        - string key: the key of the asset
        - long assetSize: the size of the asset
        - the rest will depend on the type of asset
