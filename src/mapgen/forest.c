static void placeTree(struct map map, int x, int y)
{
	map_setTile(map, x, y, 0x0088, TFLAG_SOLID);
}

static void placeStump(struct map map, int x, int y)
{
	map_setTile(map, x, y, 0x0089, TFLAG_SOLID);
}

static void placeBigTree(struct map map, int x, int y)
{
	map_setTile(map, x + 0, y + 0, 0x00e1, TFLAG_SOLID);
	map_setTile(map, x + 1, y + 0, 0x00e2, TFLAG_SOLID);
	map_setTile(map, x + 0, y + 1, 0x00ff, TFLAG_SOLID);
	map_setTile(map, x + 1, y + 1, 0x0100, TFLAG_SOLID);
}

static void placeGrass(struct map map, int x, int y, int width, int height)
{
	for (int r = y; r < y + height; ++r)
	for (int c = x; c < x + width; ++c) {
		switch (rand() & 0x3) {
			case 0x0:
			case 0x1:
				map_setTile(map, c, r, 0x000f, TFLAG_NONE); break;
			case 0x2:
				map_setTile(map, c, r, 0x0069, TFLAG_NONE); break;
			case 0x3:
				map_setTile(map, c, r, 0x006a, TFLAG_NONE); break;
		}
	}
}

void map_genForest(struct map map, int x, int y, int width, int height)
{
	placeGrass(map, 5, 4, width - 7, 4);

	for (int r = y; r < y + height - 1; ++r)
	for (int c = x; c < x + width - 1; ++c) {
		if (map_hasTile(map, c + 0, r + 0) ||
			map_hasTile(map, c + 1, r + 0) ||
		    map_hasTile(map, c + 1, r + 1) ||
			map_hasTile(map, c + 0, r + 1)) {
			continue;
		}

		int i = 1;

		int down = r < y + height - 2 &&
			!map_hasTile(map, c + 0, r + 2) &&
			!map_hasTile(map, c + 1, r + 2) ? i++ : 0;

		int right = c < x + width - 2 &&
			!map_hasTile(map, c + 2, r + 0) &&
			!map_hasTile(map, c + 2, r + 1) ? i++ : 0;

		int downRight = r < y + height - 2 && c < x + width - 2 &&
			!map_hasTile(map, c + 1, r + 1) &&
			!map_hasTile(map, c + 2, r + 1) &&
			!map_hasTile(map, c + 2, r + 2) &&
			!map_hasTile(map, c + 1, r + 2) ? i++ : 0;

		int select = rand() % i;

		if (select == 0) {
			placeBigTree(map, c + 0, r + 0);
		} else if (select == down) {
			placeBigTree(map, c + 0, r + 1);
		} else if (select == right) {
			placeBigTree(map, c + 1, r + 0);
		} else if (select == downRight) {
			placeBigTree(map, c + 1, r + 1);
		}
	}

	for (int r = y; r < y + height; ++r)
	for (int c = x; c < x + width; ++c) {
		if (!map_hasTile(map, c, r)) {
			if ((rand() & 0xff) < 0xe7) {
				placeTree(map, c, r);
			} else {
				placeStump(map, c, r);
			}
		}
	}
}
