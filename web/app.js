// Code Mesh — Dynamic Frontend AST Matrix Visualizer Engine (v6.1)

const treeContainer = document.getElementById('tree-container');
const totalMatricesEl = document.getElementById('total-matrices');
const totalFilesEl = document.getElementById('total-files');
const totalFoldersEl = document.getElementById('total-folders');
const matrixCountBadge = document.getElementById('matrix-count-badge');

const btnRefresh = document.getElementById('btn-refresh');
const canvasArea = document.getElementById('canvas-area');

const tabMatrices = document.getElementById('tab-matrices');
const tabTreeSummary = document.getElementById('tab-tree-summary');

let globalNodeData = [];
let globalMatrixData = [];
let currentView = 'matrices';

/**
 * Main Data Fetcher — Loads both mesh_data.json and matrix_data.json
 */
async function loadAllEngineData() {
  try {
    canvasArea.innerHTML = '<div class="loading-state">Syncing live data from C Core Engine...</div>';

    // 1. Fetch File Tree Data
    const treeRes = await fetch(`mesh_data.json?t=${Date.now()}`);
    if (treeRes.ok) {
      const treeJson = await treeRes.json();
      globalNodeData = treeJson.tree || [];
      renderHierarchicalTree(globalNodeData);
    }

    // 2. Fetch Matrix Collection Data
    const matrixRes = await fetch(`matrix_data.json?t=${Date.now()}`);
    if (matrixRes.ok) {
      const matrixJson = await matrixRes.json();
      globalMatrixData = matrixJson.matrices || [];
    } else {
      globalMatrixData = [];
    }

    // Update Header Counts
    totalMatricesEl.textContent = globalMatrixData.length;
    matrixCountBadge.textContent = globalMatrixData.length;

    // Render Current View
    renderView();

  } catch (err) {
    console.error('Error fetching engine data:', err);
    canvasArea.innerHTML = `
      <div class="empty-state" style="color: #ff4757;">
        ⚠️ Unable to load engine data JSON.<br>
        Run <code>make run-test</code> in C engine to export live matrix data.
      </div>
    `;
  }
}

/**
 * Render View Dispatcher
 */
function renderView() {
  if (currentView === 'matrices') {
    renderDiscoveredMatrices(globalMatrixData);
  } else {
    renderWorkspaceTreeSummary(globalNodeData);
  }
}

/**
 * Renders detected C 2D Matrices with high-tech bracket UI & Instant Tooltips
 */
function renderDiscoveredMatrices(matrices) {
  if (!matrices || matrices.length === 0) {
    canvasArea.innerHTML = `
      <div class="empty-state">
        <h3>No 2D Matrices Detected</h3>
        <p>Run <code>make run-test</code> on C source files containing 2D arrays (e.g. <code>double m[3][4]</code>).</p>
      </div>
    `;
    return;
  }

  let html = `
    <div class="section-title">
      <h2>⚡ AST Discovered 2D Matrices</h2>
      <span class="badge-info">${matrices.length} Active Structure${matrices.length > 1 ? 's' : ''}</span>
    </div>
    <div class="matrices-grid">
  `;

  matrices.forEach((mat, idx) => {
    const rows = mat.rows || (mat.data ? mat.data.length : 0);
    const cols = mat.columns || mat.cols || (mat.data && mat.data[0] ? mat.data[0].length : 0);
    const totalCells = rows * cols;

    html += `
      <div class="matrix-card-view">
        <div class="card-top">
          <div class="matrix-name-group">
            <span class="matrix-name">
              <span>📐</span> ${escapeHtml(mat.name || `matrix_${idx + 1}`)}
            </span>
            <span class="type-pill">C 2D Array</span>
          </div>
          <span class="matrix-dim-badge">${rows} × ${cols}</span>
        </div>

        <div class="math-matrix-wrapper">
          <div class="bracket-left"></div>
          
          <div class="matrix-cells-grid" style="grid-template-columns: repeat(${cols}, minmax(52px, 1fr));">
    `;

    // Render 2D Grid Cells with Instant Hover Tooltip Badge
    if (mat.data && Array.isArray(mat.data)) {
      for (let r = 0; r < rows; r++) {
        for (let c = 0; c < cols; c++) {
          const val = (mat.data[r] && mat.data[r][c] !== undefined) ? mat.data[r][c] : 0;
          html += `
            <div class="matrix-cell-node" data-tooltip="[Row ${r}, Col ${c}]">
              ${val}
            </div>
          `;
        }
      }
    }

    html += `
          </div>

          <div class="bracket-right"></div>
        </div>

        <div class="matrix-footer">
          <span>Row Major Contiguous Memory</span>
          <span>${totalCells} Elements</span>
        </div>
      </div>
    `;
  });

  html += `</div>`;
  canvasArea.innerHTML = html;
}

/**
 * Renders Workspace Tree Overview
 */
function renderWorkspaceTreeSummary(nodes) {
  let fileCount = 0;
  let dirCount = 0;

  nodes.forEach(n => {
    if (n.is_dir) dirCount++;
    else fileCount++;
  });

  canvasArea.innerHTML = `
    <div class="section-title">
      <h2>📁 Workspace File Tree Summary</h2>
      <span class="badge-info">${nodes.length} Scanned Nodes</span>
    </div>

    <div class="matrices-grid">
      <div class="matrix-card-view">
        <div class="card-top">
          <span class="matrix-name">📂 Directory Structure</span>
        </div>
        <div style="font-family: var(--font-mono); font-size: 0.9rem; line-height: 1.8; color: var(--text-secondary);">
          <p>• Total Nodes: <strong style="color:var(--accent-cyan);">${nodes.length}</strong></p>
          <p>• Total Folders: <strong style="color:var(--accent-green);">${dirCount}</strong></p>
          <p>• Total Files: <strong style="color:var(--accent-purple);">${fileCount}</strong></p>
        </div>
      </div>
    </div>
  `;
}

/**
 * Dynamic Sidebar Tree Builder
 */
function renderHierarchicalTree(nodes) {
  treeContainer.innerHTML = '';

  let folderCount = 0;
  let fileCount = 0;

  const root = { name: 'root', is_dir: true, children: {}, id: -1 };

  nodes.forEach(node => {
    if (node.is_dir) folderCount++;
    else fileCount++;

    const cleanPath = node.path.replace(/^\.\//, '').replace(/^\//, '');
    const parts = cleanPath.split('/').filter(Boolean);

    let current = root;
    parts.forEach((part, index) => {
      const isLast = index === parts.length - 1;
      if (!current.children[part]) {
        current.children[part] = {
          name: part,
          is_dir: isLast ? node.is_dir : true,
          id: isLast ? node.id : -1,
          children: {}
        };
      } else if (isLast) {
        current.children[part].id = node.id;
        current.children[part].is_dir = node.is_dir;
      }
      current = current.children[part];
    });
  });

  function createTreeDOM(objContainer) {
    const fragment = document.createDocumentFragment();

    const keys = Object.keys(objContainer.children).sort((a, b) => {
      const isDirA = objContainer.children[a].is_dir;
      const isDirB = objContainer.children[b].is_dir;
      if (isDirA === isDirB) return a.localeCompare(b);
      return isDirA ? -1 : 1;
    });

    keys.forEach(key => {
      const item = objContainer.children[key];

      if (item.is_dir) {
        const folderDiv = document.createElement('div');
        folderDiv.className = 'tree-folder';

        const titleDiv = document.createElement('div');
        titleDiv.className = 'tree-folder-title';
        titleDiv.innerHTML = `
          <span class="caret open">▶</span>
          <span>📁</span>
          <span>${escapeHtml(item.name)}</span>
          ${item.id !== -1 ? `<span class="node-id">#${item.id}</span>` : ''}
        `;

        const branchContainer = document.createElement('div');
        branchContainer.className = 'tree-branch';
        branchContainer.appendChild(createTreeDOM(item));

        titleDiv.addEventListener('click', (e) => {
          e.stopPropagation();
          const caret = titleDiv.querySelector('.caret');
          caret.classList.toggle('open');
          branchContainer.style.display = branchContainer.style.display === 'none' ? 'block' : 'none';
        });

        folderDiv.appendChild(titleDiv);
        folderDiv.appendChild(branchContainer);
        fragment.appendChild(folderDiv);
      } else {
        const fileDiv = document.createElement('div');
        fileDiv.className = 'tree-file';

        const extIcon = getFileBadge(item.name);
        const idBadge = item.id !== -1 ? `<span class="node-id">#${item.id}</span>` : '';

        fileDiv.innerHTML = `
          ${extIcon}
          <span class="node-name">${escapeHtml(item.name)}</span>
          ${idBadge}
        `;

        fragment.appendChild(fileDiv);
      }
    });

    return fragment;
  }

  treeContainer.appendChild(createTreeDOM(root));

  totalFilesEl.textContent = fileCount;
  totalFoldersEl.textContent = folderCount;
}

function getFileBadge(filename) {
  const ext = filename.split('.').pop().toLowerCase();
  switch (ext) {
    case 'c': return '<span class="file-icon icon-c">C</span>';
    case 'h': return '<span class="file-icon icon-h">H</span>';
    case 'json': return '<span class="file-icon icon-json">{}</span>';
    case 'js': return '<span class="file-icon icon-js">JS</span>';
    case 'html': return '<span class="file-icon icon-html">&lt;&gt;</span>';
    case 'css': return '<span class="file-icon icon-css">#</span>';
    default: return '<span class="file-icon icon-def">📄</span>';
  }
}

function escapeHtml(str) {
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}

// Tab Listeners
tabMatrices.addEventListener('click', () => {
  tabMatrices.classList.add('active');
  tabTreeSummary.classList.remove('active');
  currentView = 'matrices';
  renderView();
});

tabTreeSummary.addEventListener('click', () => {
  tabTreeSummary.classList.add('active');
  tabMatrices.classList.remove('active');
  currentView = 'tree-summary';
  renderView();
});

btnRefresh.addEventListener('click', loadAllEngineData);

// Initial Load
loadAllEngineData();
