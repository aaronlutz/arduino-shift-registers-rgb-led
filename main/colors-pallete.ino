

void getGradient(int _gradiantNo, int _gradiant[16][3]) {
  if (_gradiantNo == 1) {
    // 16 step color gradiant, http://www.perbang.dk/rgbgradient/
    // Color 1 = 35ACE9, Color 2 = BF001A
    int pallete[16][3] = {
        {53, 172, 233},
        {45, 138, 230},
        {44, 104, 227},
        {40, 69, 224},
        {39, 36, 221},
        {66, 33, 219},
        {94, 29, 216},
        {122, 25, 213},
        {151, 22, 210},
        {180, 18, 207},
        {205, 15, 200},
        {202, 12, 166},
        {199, 9, 131},
        {196, 5, 96},
        {193, 2, 61},
        {191, 0, 25}};

      for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 3; y++) {
          _gradiant[x][y] = pallete[x][y];
        }
      }
  }
}
