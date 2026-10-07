// Minimal Terminal Matrix Renderer in JavaScript (Node.js)

function displayMatrixTerminal(matrix) {
  console.log("\n=================================");
  console.log("   Code Mesh — Terminal Matrix   ");
  console.log("=================================\n");

  for (let r = 0; r < matrix.length; r++) {
    let rowStr = "[ ";
    for (let c = 0; c < matrix[r].length; c++) {
      rowStr += String(matrix[r][c]).padStart(5, " ") + " ";
    }
    rowStr += "]";
    console.log(rowStr);
  }

  console.log("\n=================================\n");
}

// Sample 3x3 Matrix
const sampleMatrix = [
  [1, 0, 0],
  [0, 1, 0],
  [0, 0, 1]
];

displayMatrixTerminal(sampleMatrix);
