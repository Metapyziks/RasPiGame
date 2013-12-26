static void placeBigTree(struct map map, int x, int y)
{
	map_setTile(map, x + 0, y + 0, 225, TFLAG_SOLID);
	map_setTile(map, x + 1, y + 0, 226, TFLAG_SOLID);
	map_setTile(map, x + 0, y + 1, 255, TFLAG_SOLID);
	map_setTile(map, x + 1, y + 1, 256, TFLAG_SOLID);
}

void map_genForest(struct map map, int x, int y, int width, int height)
{
	for (int r = 0; r < height; r += 2) {
		int x0 = (width & 1) == 1 && (r & 1) == 1 ? 1 : 0;
		for (int c = x0; c < width; c += 2) {
			placeBigTree(map, x + c, y + r);
		}
	}
}
