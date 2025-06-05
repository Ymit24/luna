const point = struct {
  x: i32,
  y: i32,
  z: i32,
};

const _start = fn() void {
  let p = point { x: 0, y: 0, z: 0 };
  while (++p.y < 5) {}
  while (true) {}
};

_start();
