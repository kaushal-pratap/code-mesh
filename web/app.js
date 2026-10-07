// Code Mesh — Dynamic Frontend Tree & Visualizer Engine (v5.0)

const treeContainer = document.getElementById('tree-container');
const totalNodesEl = document.getElementById('total-nodes');
const totalFoldersEl = document.getElementById('total-folders');
const totalFilesEl = document.getElementById('total-files');
const btnRefresh = document.getElementById('btn-refresh');
const canvasArea = document.getElementById('canvas-area');

const tabGraph = document.getElementById('tab-graph');
const tabMatrix = document.getElementById('tab-matrix');

let globalNodeData = [];
let currentView = 'graph';

/**
 * Fetch mesh_data.json dynamically emitted by C engine
 */
async function loadMeshData() {
  try {
    treeContainer.innerHTML = '<div class="loading-state">Fetching live mesh_data.json...</div>';
    
    const response = await fetch(`mesh_data.json?t=${Date.now()}`);
    if (!response.ok) {
      throw new Error(`Failed to load mesh_data.json (${response.status})`);
    }

    const data = await response.json();
    globalNodeData = data.tree || [];
    renderVisualizer();
  } catch (err) {
    console.error(err);
    treeContainer.innerHTML = `
      <div class="loading-state" style="color: #ff4757;">
        ⚠️ Error loading mesh_data.json.<br>
        Run <code>make run</code> in C engine to export data.
      </div>
    `;
  }
}

/**
 * Main Render Dispatcher
 */
function renderVisualizer() {
  renderHierarchicalTree(globalNodeData);

  if (currentView === 'matrix') {
    renderAdjacencyMatrix(globalNodeData);
  } else {
    renderGraphView(globalNodeData);
  }
}

/**
 * Dynamic Hierarchical Folder Tree Builder
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

  totalNodesEl.textContent = nodes.length;
  totalFoldersEl.textContent = folderCount;
  totalFilesEl.textContent = fileCount;
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
    case 'sh': return '<span class="file-icon icon-c">SH</span>';
    default: return '<span class="file-icon icon-def">📄</span>';
  }
}

function renderAdjacencyMatrix(nodes) {
  const N = nodes.length;
  if (N === 0) return;

  canvasArea.innerHTML = `
    <div class="matrix-container-view">
      <div class="matrix-title">
        <span>⚡ Code Mesh Adjacency Matrix (${N} × ${N})</span>
        <span style="font-size: 0.8rem; color: var(--text-muted);">Nodes: 0 to ${N - 1}</span>
      </div>
      <div class="matrix-grid-view" id="matrix-grid-view"></div>
    </div>
  `;

  const grid = document.getElementById('matrix-grid-view');
  grid.style.gridTemplateColumns = `repeat(${N + 1}, 42px)`;

  grid.appendChild(createCell('', 'matrix-header-cell'));
  for (let c = 0; c < N; c++) {
    grid.appendChild(createCell(`#${c}`, 'matrix-header-cell'));
  }

  for (let r = 0; r < N; r++) {
    grid.appendChild(createCell(`#${r}`, 'matrix-header-cell'));
    for (let c = 0; c < N; c++) {
      const isConnected = (r === c) || (nodes[r].is_dir && nodes[c].path.startsWith(nodes[r].path));
      const cell = createCell(isConnected ? '1' : '0', isConnected ? 'matrix-cell-view active' : 'matrix-cell-view');
      grid.appendChild(cell);
    }
  }
}

function createCell(text, className) {
  const div = document.createElement('div');
  div.className = className;
  div.textContent = text;
  return div;
}

function renderGraphView(nodes) {
  canvasArea.innerHTML = `
    <div class="placeholder-msg">
      <h2>Code Mesh Dependency Engine Active</h2>
      <p style="margin-top: 8px;">Scanned ${nodes.length} workspace nodes dynamically from C core engine.</p>
    </div>
  `;
}

function escapeHtml(str) {
  return String(str)
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}

// Tab Switching Listeners
tabGraph.addEventListener('click', () => {
  tabGraph.classList.add('active');
  tabMatrix.classList.remove('active');
  currentView = 'graph';
  renderVisualizer();
});

tabMatrix.addEventListener('click', () => {
  tabMatrix.classList.add('active');
  tabGraph.classList.remove('active');
  currentView = 'matrix';
  renderVisualizer();
});

btnRefresh.addEventListener('click', loadMeshData);

// Initial Load
loadMeshData();
