/**
 * Silis Native 2D Layout Peeker - High-Performance Canvas Renderer
 * True Cartesian Space: Origin (0,0) at bottom-left (Y-up, X-right)
 */

// Global State
const state = {
  die: { w: 5000, h: 5000 },
  core: { border: 20, w: 4960, h: 4960 },
  siteHeight: 2.720,
  zoom: 1.0,
  panX: 80,
  panY: 80,
  showGrid: true,
  selectedMacro: null,
  searchQuery: '',
  isPanning: false,
  panStart: { x: 0, y: 0 },
  macros: [
    { inst: 'u_sram0', master: 'sky130_sram_2kbyte_1rw1r_32x512_8', x: 120, y: 81.6, w: 420, h: 280, orient: 'R0', pins: [{ name: 'clk0', x: 0, y: 50 }, { name: 'din0', x: 0, y: 120 }, { name: 'dout0', x: 420, y: 120 }] },
    { inst: 'u_sram1', master: 'sky130_sram_2kbyte_1rw1r_32x512_8', x: 120, y: 489.6, w: 420, h: 280, orient: 'R0', pins: [{ name: 'clk1', x: 0, y: 50 }, { name: 'din1', x: 0, y: 120 }, { name: 'dout1', x: 420, y: 120 }] },
    { inst: 'u_alu_core', master: 'simple_alu', x: 1200, y: 326.4, w: 220, h: 160, orient: 'R0', pins: [{ name: 'clk', x: 0, y: 30 }, { name: 'res_out', x: 220, y: 80 }] },
    { inst: 'u_ctrl_unit', master: 'control_logic', x: 2000, y: 1200, w: 300, h: 200, orient: 'R90', pins: [{ name: 'clk', x: 0, y: 100 }, { name: 'en', x: 150, y: 200 }] }
  ],
  pins: [
    { name: 'clk', edge: 'bottom', x: 120.0, y: 0.0, layer: 'met3' },
    { name: 'rst_n', edge: 'bottom', x: 160.0, y: 0.0, layer: 'met3' },
    { name: 'io_tx', edge: 'right', x: 5000.0, y: 2500.0, layer: 'met3' },
    { name: 'io_rx', edge: 'left', x: 0.0, y: 2500.0, layer: 'met3' }
  ]
};

// Canvas references
const canvas = document.getElementById('peeker-canvas');
const ctx = canvas.getContext('2d');
const wrapper = document.getElementById('canvas-wrapper');

// Initialize and setup resize and listeners
function initPeeker() {
  resizeCanvas();
  window.addEventListener('resize', resizeCanvas);
  
  canvas.addEventListener('mousedown', onMouseDown);
  canvas.addEventListener('mousemove', onMouseMove);
  canvas.addEventListener('mouseup', onMouseUp);
  canvas.addEventListener('wheel', onWheel, { passive: false });

  parseUrlAndLoadData();
}

function resizeCanvas() {
  const dpr = window.devicePixelRatio || 1;
  const rect = wrapper.getBoundingClientRect();
  canvas.width = rect.width * dpr;
  canvas.height = rect.height * dpr;
  ctx.scale(dpr, dpr);
  requestAnimationFrame(render);
}

// URL Parameter Handling & Fetching DEF Geometry
async function parseUrlAndLoadData() {
  const params = new URLSearchParams(window.location.search);
  const defPath = params.get('def');
  const project = params.get('project');

  if (defPath || project) {
    try {
      const endpoint = defPath ? `/api/def/parse?def=${encodeURIComponent(defPath)}` : `/api/def/parse?project=${encodeURIComponent(project)}`;
      const response = await fetch(endpoint);
      if (response.ok) {
        const data = await response.json();
        if (data.die) state.die = data.die;
        if (data.core) state.core = data.core;
        if (data.components) state.macros = data.components;
        if (data.macros) state.macros = data.macros;
        if (data.pins) state.pins = data.pins;
      }
    } catch (err) {
      console.warn('Backend server offline or unreachable. Using robust built-in layout mock data.');
    }
  }

  updateComponentCount();
  canvasZoomFit();
}

function updateComponentCount() {
  const total = state.macros.length + state.pins.length;
  document.getElementById('hud-comp-count').innerText = `Components: ${total} (Macros: ${state.macros.length}, Pins: ${state.pins.length})`;
}

// Coordinate Transforms (True Cartesian: Origin bottom-left, Y-up)
function worldToScreen(wx, wy) {
  const rect = wrapper.getBoundingClientRect();
  const sx = state.panX + wx * state.zoom;
  const sy = rect.height - (state.panY + wy * state.zoom);
  return { x: sx, y: sy };
}

function screenToWorld(sx, sy) {
  const rect = wrapper.getBoundingClientRect();
  const wx = (sx - state.panX) / state.zoom;
  const wy = (rect.height - sy - state.panY) / state.zoom;
  return { x: wx, y: wy };
}

// Zoom & Pan controls
function canvasZoomIn() {
  state.zoom *= 1.25;
  requestAnimationFrame(render);
  updateZoomHud();
}

function canvasZoomOut() {
  state.zoom /= 1.25;
  requestAnimationFrame(render);
  updateZoomHud();
}

function canvasZoomFit() {
  const rect = wrapper.getBoundingClientRect();
  const margin = 60;
  const availableW = Math.max(100, rect.width - margin * 2);
  const availableH = Math.max(100, rect.height - margin * 2);
  const scaleX = availableW / state.die.w;
  const scaleY = availableH / state.die.h;
  state.zoom = Math.min(scaleX, scaleY);
  
  const scaledW = state.die.w * state.zoom;
  const scaledH = state.die.h * state.zoom;
  state.panX = (rect.width - scaledW) / 2;
  state.panY = (rect.height - scaledH) / 2;
  
  requestAnimationFrame(render);
  updateZoomHud();
}

function updateZoomHud() {
  document.getElementById('status-zoom').innerText = `Zoom: ${Math.round(state.zoom * 100)}%`;
}

function toggleLayers() {
  state.showGrid = !state.showGrid;
  const btn = document.getElementById('btn-toggle-layers');
  btn.style.borderColor = state.showGrid ? 'var(--accent)' : 'var(--border)';
  requestAnimationFrame(render);
}

function onSearchInput(query) {
  state.searchQuery = query.trim().toLowerCase();
  requestAnimationFrame(render);
}

// Mouse Event Handlers for Panning & Selection
let isMouseDown = false;
let mouseStart = { x: 0, y: 0 };

function onMouseDown(e) {
  isMouseDown = true;
  mouseStart = { x: e.clientX, y: e.clientY };
  if (e.button === 1 || e.shiftKey) {
    state.isPanning = true;
  }
}

function onMouseMove(e) {
  const rect = canvas.getBoundingClientRect();
  const mouseX = e.clientX - rect.left;
  const mouseY = e.clientY - rect.top;
  const worldPos = screenToWorld(mouseX, mouseY);

  document.getElementById('status-cursor').innerText = `X: ${worldPos.x.toFixed(3)} um | Y: ${worldPos.y.toFixed(3)} um`;

  if (isMouseDown) {
    const dx = e.clientX - mouseStart.x;
    const dy = e.clientY - mouseStart.y;
    mouseStart = { x: e.clientX, y: e.clientY };

    if (state.isPanning || e.buttons === 4) {
      state.panX += dx;
      state.panY -= dy; // Invert for Y-up Cartesian
    }
    requestAnimationFrame(render);
  }
}

function onMouseUp(e) {
  if (!isMouseDown) return;
  const dx = Math.abs(e.clientX - mouseStart.x);
  const dy = Math.abs(e.clientY - mouseStart.y);

  // If click without drag, perform selection test
  if (dx < 3 && dy < 3 && !state.isPanning) {
    const rect = canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    const mouseY = e.clientY - rect.top;
    const worldPos = screenToWorld(mouseX, mouseY);
    performSelection(worldPos);
  }

  isMouseDown = false;
  state.isPanning = false;
}

function onWheel(e) {
  e.preventDefault();
  const rect = canvas.getBoundingClientRect();
  const mouseX = e.clientX - rect.left;
  const mouseY = e.clientY - rect.top;

  const worldBefore = screenToWorld(mouseX, mouseY);
  const zoomFactor = e.deltaY < 0 ? 1.15 : 1 / 1.15;
  state.zoom *= zoomFactor;

  const worldAfter = screenToWorld(mouseX, mouseY);
  state.panX += (worldAfter.x - worldBefore.x) * state.zoom;
  state.panY += (worldAfter.y - worldBefore.y) * state.zoom;

  requestAnimationFrame(render);
  updateZoomHud();
}

// Selection & Inspector Update
function performSelection(pos) {
  let found = null;
  for (const m of state.macros) {
    if (pos.x >= m.x && pos.x <= m.x + m.w && pos.y >= m.y && pos.y <= m.y + m.h) {
      found = m;
      break;
    }
  }

  state.selectedMacro = found;
  updateInspector(found);
  requestAnimationFrame(render);
}

function updateInspector(m) {
  if (!m) {
    document.getElementById('val-inst').innerText = 'None';
    document.getElementById('val-master').innerText = '-';
    document.getElementById('val-x').innerText = '-';
    document.getElementById('val-y').innerText = '-';
    document.getElementById('val-w').innerText = '-';
    document.getElementById('val-h').innerText = '-';
    document.getElementById('val-orient').innerText = '-';
    document.getElementById('status-selection').innerText = 'Selected: None';
    document.getElementById('pins-list-container').innerHTML = '<div class="no-pins">No component selected</div>';
    return;
  }

  document.getElementById('val-inst').innerText = m.inst;
  document.getElementById('val-master').innerText = m.master;
  document.getElementById('val-x').innerText = m.x.toFixed(3) + ' um';
  document.getElementById('val-y').innerText = m.y.toFixed(3) + ' um';
  document.getElementById('val-w').innerText = m.w.toFixed(3) + ' um';
  document.getElementById('val-h').innerText = m.h.toFixed(3) + ' um';
  document.getElementById('val-orient').innerText = m.orient;
  document.getElementById('status-selection').innerText = `Selected: ${m.inst} (${m.master})`;

  const pinsContainer = document.getElementById('pins-list-container');
  if (m.pins && m.pins.length > 0) {
    let html = '';
    for (const p of m.pins) {
      html += `<div class="pin-item"><span>${p.name}</span><span>X: ${p.x}, Y: ${p.y}</span></div>`;
    }
    pinsContainer.innerHTML = html;
  } else {
    pinsContainer.innerHTML = '<div class="no-pins">No pin terminals defined</div>';
  }
}

// Render Engine
function render() {
  const rect = wrapper.getBoundingClientRect();
  ctx.clearRect(0, 0, rect.width, rect.height);

  // Background
  ctx.fillStyle = '#0f0f17';
  ctx.fillRect(0, 0, rect.width, rect.height);

  // 1. Grid & Site Rows
  if (state.showGrid && state.zoom > 0.03) {
    ctx.strokeStyle = 'rgba(255, 255, 255, 0.035)';
    ctx.lineWidth = 1;
    const coreY1 = state.core.border;
    const coreY2 = state.die.h - state.core.border;
    const coreX1 = state.core.border;
    const coreX2 = state.die.w - state.core.border;

    for (let y = coreY1; y <= coreY2; y += state.siteHeight) {
      const p1 = worldToScreen(coreX1, y);
      const p2 = worldToScreen(coreX2, y);
      ctx.beginPath();
      ctx.moveTo(p1.x, p1.y);
      ctx.lineTo(p2.x, p2.y);
      ctx.stroke();
    }
  }

  // 2. Die Bounding Box
  const dieP1 = worldToScreen(0, 0);
  const dieW = state.die.w * state.zoom;
  const dieH = state.die.h * state.zoom;
  ctx.strokeStyle = '#6c7086';
  ctx.lineWidth = 1.5;
  ctx.strokeRect(dieP1.x, dieP1.y - dieH, dieW, dieH);

  ctx.fillStyle = '#a6adc8';
  ctx.font = '10px ui-monospace, monospace';
  ctx.fillText(`Die: ${state.die.w} x ${state.die.h} um`, dieP1.x + 8, dieP1.y - dieH - 6);

  // 3. Core Area
  const coreX = state.core.border;
  const coreY = state.core.border;
  const coreW = (state.die.w - state.core.border * 2) * state.zoom;
  const coreH = (state.die.h - state.core.border * 2) * state.zoom;
  const coreP = worldToScreen(coreX, coreY);
  ctx.strokeStyle = '#89b4fa';
  ctx.lineWidth = 1.5;
  ctx.strokeRect(coreP.x, coreP.y - coreH, coreW, coreH);

  // 4. Placed Components / Macros
  for (const m of state.macros) {
    const isSel = state.selectedMacro && state.selectedMacro.inst === m.inst;
    const isMatched = state.searchQuery && m.inst.toLowerCase().includes(state.searchQuery);

    const p = worldToScreen(m.x, m.y);
    const mw = m.w * state.zoom;
    const mh = m.h * state.zoom;

    // Macro Fill
    ctx.fillStyle = isSel ? 'rgba(137, 180, 250, 0.35)' : (isMatched ? 'rgba(249, 226, 175, 0.30)' : 'rgba(45, 45, 68, 0.85)');
    ctx.fillRect(p.x, p.y - mh, mw, mh);

    // Macro Border
    ctx.strokeStyle = isSel ? '#89b4fa' : (isMatched ? '#f9e2af' : '#45475a');
    ctx.lineWidth = isSel ? 2 : 1;
    ctx.strokeRect(p.x, p.y - mh, mw, mh);

    // Macro Label
    if (state.zoom > 0.08) {
      ctx.fillStyle = isSel ? '#89b4fa' : '#cdd6f4';
      ctx.font = '11px ui-monospace, monospace';
      ctx.fillText(m.inst, p.x + 6, p.y - mh + 16);
      ctx.fillStyle = '#a6adc8';
      ctx.font = '9px ui-monospace, monospace';
      ctx.fillText(m.master, p.x + 6, p.y - mh + 28);
    }

    // Pin Terminals
    if (m.pins && state.zoom > 0.15) {
      ctx.fillStyle = '#fab387';
      for (const pin of m.pins) {
        const pp = worldToScreen(m.x + pin.x, m.y + pin.y);
        ctx.fillRect(pp.x - 2, pp.y - 2, 4, 4);
      }
    }
  }

  // 5. I/O Pins / Pads
  for (const pin of state.pins) {
    const pp = worldToScreen(pin.x, pin.y);
    ctx.fillStyle = '#a6e3a1';
    ctx.beginPath();
    ctx.arc(pp.x, pp.y, 4, 0, Math.PI * 2);
    ctx.fill();
    ctx.strokeStyle = '#181825';
    ctx.lineWidth = 1;
    ctx.stroke();

    if (state.zoom > 0.1) {
      ctx.fillStyle = '#a6e3a1';
      ctx.font = '10px ui-monospace, monospace';
      ctx.fillText(pin.name, pp.x + 6, pp.y + 3);
    }
  }
}

// Start on load
window.addEventListener('load', initPeeker);
