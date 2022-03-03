
// # Tile: 

const tilesIndex = [
  5, 4, 3, 2, 1, 0,
  6, 7, 8, 9, 10, 11,
  12, 13, 14, 15, 16, 17,
  18, 19, 20, 21, 22, 23
]

const i = [
  0,   1,   2,   3,
  7,   6,   5,   4,
  8,   9,   10,  11,
  15,  14,  13,  12,
]

const xy = [
  [0,0],  [1,0],  [2,0],  [3,0],
  [0,1],  [1,1],  [2,1],  [3,1],
  [0,2],  [1,2],  [2,2],  [3,2],
  [0,3],  [1,3],  [2,3],  [3,3],
]

const mapping = ([x, y]) => y * 4 + (y % 2 ? 3 - x : x)

const cmapping = ([x, y]) => {
  // tile
  // const tile_y = y / 4;
  // const tile_x = x / 4;

  // let tile;
  let i;

  // if (tile_y % 2) { // odd
  //   tile = tile_y + 3 - tile_x;
  // } else { // even
  //   tile = tile_y * 4 + tile_x;
  // }
  i = 0 //tile * 16;

  const _x = x % 4;
  const _y = y % 4;
  if (_y % 2) {
    i += _y * 4 + 3 - _x;
  } else {
    i += _y * 4 + _x;
  }

  return i
};

const TILE_WIDTH = 4
const TILE_HEIGHT = 4
const GRID_WIDTH = 6
const GRID_HEIGHT = 4

function drawHorizontalLine(y) {
  for (let x = 0; x < 24; x++) {
    // tile
    let tile_y = y / TILE_HEIGHT |0;
    let tile_x = x / TILE_WIDTH |0;

    let tile;
    let i;

    if (tile_y % 2) { // odd
      tile_x = tile_x
    } else { // even
      tile_x = GRID_WIDTH - tile_x - 1
    }
    tile = tile_y * GRID_WIDTH + tile_x;

    i = tile * 16;

    let _x = x % 4;
    let _y = y % 4;
    if (_y % 2) {
      i += _y * 4 + 3 - _x;
    } else {
      i += _y * 4 + _x;
    }

    console.log('p', [x, y],'->', i, 'on', tile, [tile_x, tile_y], [_x, _y], )
  }
};

[1].map(y => 
  drawHorizontalLine(y)
)

// xy.map((value, index) => {
//   console.log('test', index, value, mapping(value), i[index], cmapping(value))
// })

// # Grid: 

// # 5  4  3  2  1  0
