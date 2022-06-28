
let x = read();
let y = read();

while (x != y) {
  if (x > y) {
      x = x - y;
  }
  
  if (x < y) {
      y = y - x;
  }
}

alert(y);
