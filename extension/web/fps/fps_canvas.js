/**
 * Silis Floorplan & Power Studio (FPS) - Pure Native 2D CAD Canvas & Physics Engine
 * True Cartesian Space: Origin (0,0) at bottom-left (Y-up, X-right)
 * 100% Behavioral Parity with Qt6 FloorplanView and InteractiveFloorplannerWidget
 */

// Global State
const state = {
  die: { w: 5000, h: 5000 },
  core: { border: 20, w: 4960, h: 4960 },
  siteHeight: 2.720, // Sky130 unithd row height
  gridSnap: 0.005,   // 5 DBU manufacturing grid
  zoom: 1.0,
  panX: 60,
  panY: 60,
  showGrid: true,
  showFlightlines: true,
  showHud: true,
  selectedMacro: null,
  activeEdge: 'bottom',
  isDragging: false,
  isPanning: false,
  dragStart: { x: 0, y: 0 },
  panStart: { x: 0, y: 0 },
  weights: { r2m: 1.0, r2r: 1.0, r2io: 1.0, seed: 42 },
  macros: [
    { inst: 'u_sram0', master: 'sky130_sram_2kbyte_1rw1r_32x512_8', x: 120, y: 81.6, w: 420, h: 280, orient: 'R0', halo: [10, 10, 10, 10], pins: [{ name: 'clk0', x: 0, y: 50 }, { name: 'din0', x: 0, y: 120 }] },
    { inst: 'u_sram1', master: 'sky130_sram_2kbyte_1rw1r_32x512_8', x: 120, y: 489.6, w: 420, h: 280, orient: 'R0', halo: [10, 10, 10, 10], pins: [{ name: 'clk1', x: 0, y: 50 }, { name: 'din1', x: 0, y: 120 }] },
    { inst: 'u_alu_core', master: 'simple_alu', x: 1200, y: 326.4, w: 220, h: 160, orient: 'R0', halo: [5, 5, 5, 5], pins: [{ name: 'clk', x: 0, y: 30 }, { name: 'res_out', x: 220, y: 80 }] }
  ],
  domains: [
    { name: 'PD_TOP', voltage: 1.800, color: 'rgba(137, 180, 250, 0.15)', stroke: '#89b4fa', box: [20, 20, 4980, 4980] },
    { name: 'PD_COMPUTE', voltage: 0.950, color: 'rgba(203, 166, 247, 0.20)', stroke: '#cba6f7', box: [250, 250, 950, 750] }
  ],
  pads: [
    { name: 'clk', edge: 'bottom', pos: 120.0 },
    { name: 'rst_n', edge: 'bottom', pos: 160.0 },
    { name: 'io_tx', edge: 'right', pos: 400.0 },
    { name: 'io_rx', edge: 'left', pos: 350.0 }
  ]
};

// Canvas references
const canvas = document.getElementById('fps-canvas');
const ctx = canvas.getContext('2d');
const wrapper = document.getElementById('canvas-wrapper');

// Setup Canvas Size & High-DPI Scaling
function resizeCanvas() {
  const dpr = window.devicePixelRatio || 1;
  const rect = wrapper.getBoundingClientRect();
  canvas.width = rect.width * dpr;
  canvas.height = rect.height * dpr;
  ctx.scale(dpr, dpr);
  requestAnimationFrame(render);
}
window.addEventListener('resize', resizeCanvas);

// Coordinate Transforms (True Cartesian: Origin (0,0) at bottom-left, Y-up)
function worldToScreen(wx, wy) {
  const rect = wrapper.getBoundingClientRect();
  const sx = state.panX + wx * state.zoom;
  const sy = rect.height - (state.panY + wy * state.zoom); // Invert Y
  return { x: sx, y: sy };
}

function screenToWorld(sx, sy) {
  const rect = wrapper.getBoundingClientRect();
  const wx = (sx - state.panX) / state.zoom;
  const wy = (rect.height - sy - state.panY) / state.zoom;
  return { x: wx, y: wy };
}

// Snapping to Site Rows (2.72 µm) and Manufacturing Grid (0.005 µm)
function snapCoordinate(val, grid) {
  return Math.round(val / grid) * grid;
}

// Render Engine
// Voltage Tier Palette (Desktop Silis Parity: floorplanner.py:628-640)
function getVoltageColors(v) {
  const volt = Number(v) || 1.8;
  if (volt >= 2.5) return { stroke: '#b58900', fill: 'rgba(181, 137, 0, 0.20)' }; // Amber (High voltage / Padring)
  if (volt >= 1.6) return { stroke: '#268bd2', fill: 'rgba(38, 139, 210, 0.20)' }; // Cyan (Nominal core)
  if (volt >= 0.8) return { stroke: '#6c71c4', fill: 'rgba(108, 113, 196, 0.20)' }; // Slate purple / Indigo (Gated core)
  return { stroke: '#2aa198', fill: 'rgba(42, 161, 152, 0.20)' }; // Muted teal (Ultra-low voltage island)
}

// Render Engine (100% Visual and Behavioral Parity with Silis Desktop)
function render() {
  const rect = wrapper.getBoundingClientRect();
  ctx.clearRect(0, 0, rect.width, rect.height);

  // Background (Strict Desktop Pure Pitch Black #000000)
  ctx.fillStyle = '#000000';
  ctx.fillRect(0, 0, rect.width, rect.height);

  // 1. Site Rows Grid (Sky130 2.72 µm)
  if (state.showGrid && state.zoom > 0.04) {
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

  // 2. Outer Die Boundary (Catppuccin Blue #89b4fa, width 2.0)
  const dieP1 = worldToScreen(0, 0);
  const dieW = state.die.w * state.zoom;
  const dieH = state.die.h * state.zoom;
  ctx.strokeStyle = '#89b4fa';
  ctx.lineWidth = 2.0;
  ctx.strokeRect(dieP1.x, dieP1.y - dieH, dieW, dieH);

  // Die Dimension Text
  ctx.fillStyle = '#a6adc8';
  ctx.font = '10px ui-monospace, monospace';
  ctx.fillText(`Die: ${state.die.w} × ${state.die.h} µm`, dieP1.x + 8, dieP1.y - dieH - 6);

  // 3. Inner Core Boundary (Subtext0 #a6adc8, DashLine, width 1.0)
  const coreX = state.core.border;
  const coreY = state.core.border;
  const coreW = (state.die.w - state.core.border * 2) * state.zoom;
  const coreH = (state.die.h - state.core.border * 2) * state.zoom;
  const coreP = worldToScreen(coreX, coreY);
  ctx.strokeStyle = '#a6adc8';
  ctx.lineWidth = 1.0;
  ctx.setLineDash([6, 4]);
  ctx.strokeRect(coreP.x, coreP.y - coreH, coreW, coreH);
  ctx.setLineDash([]);

  // 4. Power Domain Islands (UPF DashDotLine & Voltage Palette)
  for (const dom of state.domains) {
    if (dom.name === 'PD_TOP') continue;
    const [x1, y1, x2, y2] = dom.box;
    const dp = worldToScreen(x1, y1);
    const dw = (x2 - x1) * state.zoom;
    const dh = (y2 - y1) * state.zoom;
    const vColors = getVoltageColors(dom.voltage);
    ctx.fillStyle = vColors.fill;
    ctx.fillRect(dp.x, dp.y - dh, dw, dh);
    ctx.strokeStyle = vColors.stroke;
    ctx.lineWidth = 1.8;
    ctx.setLineDash([8, 3, 2, 3]); // DashDotLine
    ctx.strokeRect(dp.x, dp.y - dh, dw, dh);
    ctx.setLineDash([]);
    ctx.fillStyle = vColors.stroke;
    const voltStr = (dom.voltage != null && !isNaN(Number(dom.voltage))) ? Number(dom.voltage).toFixed(2) : '1.80';
    ctx.font = '10px ui-monospace, monospace';
    ctx.fillText(`ISLAND: ${dom.name} (${voltStr}V)`, dp.x + 6, dp.y - dh + 14);
  }

  // 5. Placed Macros
  for (const m of state.macros) {
    const isSel = state.selectedMacro && state.selectedMacro.inst === m.inst;
    const mp = worldToScreen(m.x, m.y);
    const mw = m.w * state.zoom;
    const mh = m.h * state.zoom;

    // Halo (Dashed Pink keepout #f38ba8)
    const halo = Array.isArray(m.halo) && m.halo.length >= 4 ? m.halo : [10, 10, 10, 10];
    const [hl, hr, hb, ht] = halo;
    const hp = worldToScreen(m.x - hl, m.y - hb);
    const hw = (m.w + hl + hr) * state.zoom;
    const hh = (m.h + hb + ht) * state.zoom;
    ctx.strokeStyle = '#f38ba8';
    ctx.lineWidth = 1.2;
    ctx.setLineDash([3, 3]);
    ctx.strokeRect(hp.x, hp.y - hh, hw, hh);
    ctx.setLineDash([]);

    // Macro Block Body
    ctx.fillStyle = isSel ? '#45475a' : '#313244';
    ctx.fillRect(mp.x, mp.y - mh, mw, mh);
    ctx.strokeStyle = isSel ? '#00FFFF' : '#89b4fa';
    ctx.lineWidth = isSel ? 2 : 1.5;
    ctx.strokeRect(mp.x, mp.y - mh, mw, mh);

    // Orientation Badge
    ctx.fillStyle = '#89b4fa';
    ctx.font = '9px monospace';
    ctx.fillText(`[${m.orient || 'R0'}]`, mp.x + 4, mp.y - mh + 12);

    // Labels: Instance & Master Name
    if (state.zoom > 0.05) {
      ctx.fillStyle = '#cdd6f4';
      ctx.font = '10px ui-monospace, monospace';
      ctx.fillText(m.inst, mp.x + 8, mp.y - mh / 2);
      ctx.fillStyle = '#a6adc8';
      ctx.font = '9px ui-monospace, monospace';
      ctx.fillText(`(${m.master})`, mp.x + 8, mp.y - mh / 2 + 13);
    }

    // Pins
    if (m.pins) {
      for (const p of m.pins) {
        const pp = worldToScreen(m.x + p.x, m.y + p.y);
        ctx.fillStyle = '#f38ba8';
        ctx.fillRect(pp.x - 2, pp.y - 2, 4, 4);
      }
    }
  }

  // 6. Peripheral I/O Pads on N, S, E, W die perimeters
  renderPadRings();

  // 7. Interconnect Elastic Flylines (Pad to Macro & Inter-Macro)
  if (state.showFlightlines) {
    ctx.lineWidth = 1.2;
    ctx.setLineDash([4, 3]);

    for (const pad of state.pads) {
      let px = 0, py = 0;
      if (pad.edge === 'bottom') { px = pad.pos; py = 0; }
      else if (pad.edge === 'top') { px = pad.pos; py = state.die.h; }
      else if (pad.edge === 'left') { px = 0; py = pad.pos; }
      else if (pad.edge === 'right') { px = state.die.w; py = pad.pos; }
      const p1 = worldToScreen(px, py);

      let targetM = state.macros[0];
      for (const m of state.macros) {
        if (pad.name.toLowerCase().includes(m.inst.toLowerCase().replace('u_', ''))) {
          targetM = m;
          break;
        }
      }
      if (!targetM) continue;

      const p2 = worldToScreen(targetM.x + targetM.w / 2, targetM.y + targetM.h / 2);
      const isClkRst = /clk|rst|reset|clock/i.test(pad.name);
      const isPwr = /vdd|vss|power|gnd/i.test(pad.name);
      ctx.strokeStyle = isClkRst ? 'rgba(249, 226, 175, 0.55)' : (isPwr ? 'rgba(243, 139, 168, 0.55)' : 'rgba(137, 220, 235, 0.47)');

      ctx.beginPath();
      ctx.moveTo(p1.x, p1.y);
      const cx1 = p1.x + (p2.x - p1.x) * 0.25;
      const cy1 = p1.y + (p2.y - p1.y) * 0.05;
      const cx2 = p1.x + (p2.x - p1.x) * 0.75;
      const cy2 = p1.y + (p2.y - p1.y) * 0.95;
      ctx.bezierCurveTo(cx1, cy1, cx2, cy2, p2.x, p2.y);
      ctx.stroke();
    }
    ctx.setLineDash([]);
  }
}

// Render Peripheral Pad Rings (Desktop Silis Parity: ioplacer.py & floorplanner.py)
function renderPadRings() {
  const dieP1 = worldToScreen(0, 0);
  const dieW = state.die.w * state.zoom;
  const dieH = state.die.h * state.zoom;
  const edgeWidth = 14;

  const edges = {
    bottom: { x: dieP1.x, y: dieP1.y, w: dieW, h: edgeWidth },
    top: { x: dieP1.x, y: dieP1.y - dieH - edgeWidth, w: dieW, h: edgeWidth },
    left: { x: dieP1.x - edgeWidth, y: dieP1.y - dieH, w: edgeWidth, h: dieH },
    right: { x: dieP1.x + dieW, y: dieP1.y - dieH, w: edgeWidth, h: dieH }
  };

  for (const [e, rect] of Object.entries(edges)) {
    const isActive = state.activeEdge === e;
    ctx.fillStyle = isActive ? 'rgba(0, 255, 255, 0.12)' : 'rgba(69, 71, 90, 0.06)';
    ctx.fillRect(rect.x, rect.y, rect.w, rect.h);
    ctx.strokeStyle = isActive ? '#00FFFF' : '#45475a';
    ctx.lineWidth = isActive ? 2 : 1;
    if (!isActive) ctx.setLineDash([4, 4]);
    ctx.strokeRect(rect.x, rect.y, rect.w, rect.h);
    if (!isActive) ctx.setLineDash([]);
  }

  // Draw Pad markers with Desktop Silis directional colors
  for (const pad of state.pads) {
    let px = 0, py = 0;
    if (pad.edge === 'bottom') { px = pad.pos; py = 0; }
    else if (pad.edge === 'top') { px = pad.pos; py = state.die.h; }
    else if (pad.edge === 'left') { px = 0; py = pad.pos; }
    else if (pad.edge === 'right') { px = state.die.w; py = pad.pos; }

    const sp = worldToScreen(px, py);
    const pdir = pad.dir || (pad.name.startsWith('out') ? 'output' : (pad.name.startsWith('io') ? 'inout' : (pad.name.includes('v') ? 'power' : 'input')));
    let fill = '#89dceb';
    let stroke = '#11111b';
    let strokeWidth = 1;
    if (pdir === 'input') fill = '#a6e3a1';
    else if (pdir === 'output') fill = '#89b4fa';
    else if (pdir === 'inout') fill = '#cba6f7';
    else if (pdir === 'power') { fill = '#f9e2af'; stroke = '#f38ba8'; strokeWidth = 1.5; }
    else if (pdir === 'ground') { fill = '#f9e2af'; stroke = '#89b4fa'; strokeWidth = 1.5; }

    ctx.fillStyle = fill;
    ctx.fillRect(sp.x - 3.5, sp.y - 3.5, 7, 7);
    ctx.strokeStyle = stroke;
    ctx.lineWidth = strokeWidth;
    ctx.strokeRect(sp.x - 3.5, sp.y - 3.5, 7, 7);

    if (state.zoom > 0.08) {
      ctx.fillStyle = '#cdd6f4';
      ctx.font = '8px ui-monospace, monospace';
      ctx.fillText(pad.name, sp.x + 5, sp.y + 3);
    }
  }
}

// Canvas Mouse Navigation & Manipulation
canvas.addEventListener('wheel', (e) => {
  e.preventDefault();
  const rect = wrapper.getBoundingClientRect();
  const mx = e.clientX - rect.left;
  const my = e.clientY - rect.top;

  const oldWorld = screenToWorld(mx, my);
  const factor = e.deltaY < 0 ? 1.15 : 1 / 1.15;
  state.zoom = Math.max(0.01, Math.min(20.0, state.zoom * factor));

  // Keep mouse point stationary in world coordinates
  const newWorld = screenToWorld(mx, my);
  state.panX += (newWorld.x - oldWorld.x) * state.zoom;
  state.panY += (newWorld.y - oldWorld.y) * state.zoom;

  document.getElementById('status-zoom').innerText = `Zoom: ${Math.round(state.zoom * 100)}%`;
  requestAnimationFrame(render);
});

canvas.addEventListener('mousedown', (e) => {
  const rect = wrapper.getBoundingClientRect();
  const mx = e.clientX - rect.left;
  const my = e.clientY - rect.top;

  // Middle or Right button: Pan
  if (e.button === 1 || e.button === 2) {
    state.isPanning = true;
    state.panStart = { x: mx - state.panX, y: my + state.panY };
    return;
  }

  // Left button: Select or Drag Macro
  if (e.button === 0) {
    const world = screenToWorld(mx, my);
    let hit = null;
    for (let i = state.macros.length - 1; i >= 0; i--) {
      const m = state.macros[i];
      if (world.x >= m.x && world.x <= m.x + m.w && world.y >= m.y && world.y <= m.y + m.h) {
        hit = m;
        break;
      }
    }

    state.selectedMacro = hit;
    if (hit) {
      state.isDragging = true;
      state.dragStart = { x: world.x - hit.x, y: world.y - hit.y };
      syncSelectedMacroCard(hit);
    } else {
      document.getElementById('status-selection').innerText = 'Selected: None';
      document.getElementById('lbl-sel-name').innerText = 'None';
    }
    requestAnimationFrame(render);
  }
});

canvas.addEventListener('mousemove', (e) => {
  const rect = wrapper.getBoundingClientRect();
  const mx = e.clientX - rect.left;
  const my = e.clientY - rect.top;
  const world = screenToWorld(mx, my);

  // Status Bar update
  document.getElementById('status-cursor').innerText = `X: ${world.x.toFixed(3)} µm | Y: ${world.y.toFixed(3)} µm`;

  if (state.isPanning) {
    state.panX = mx - state.panStart.x;
    state.panY = state.panStart.y - my;
    requestAnimationFrame(render);
    return;
  }

  if (state.isDragging && state.selectedMacro) {
    let rawX = world.x - state.dragStart.x;
    let rawY = world.y - state.dragStart.y;

    // Legalize snapping to manufacturing grid & site rows
    const legalX = snapCoordinate(rawX, state.gridSnap);
    const legalY = snapCoordinate(rawY, state.siteHeight);

    // Core boundary clamping
    const minX = state.core.border;
    const minY = state.core.border;
    const maxX = state.die.w - state.core.border - state.selectedMacro.w;
    const maxY = state.die.h - state.core.border - state.selectedMacro.h;

    state.selectedMacro.x = Math.max(minX, Math.min(maxX, legalX));
    state.selectedMacro.y = Math.max(minY, Math.min(maxY, legalY));

    // Update spinboxes
    document.getElementById('sp-sel-x').value = state.selectedMacro.x.toFixed(3);
    document.getElementById('sp-sel-y').value = state.selectedMacro.y.toFixed(3);

    requestAnimationFrame(render);
  }
});

window.addEventListener('mouseup', () => {
  if (state.isDragging) {
    state.isDragging = false;
    regenerateTcl();
  }
  state.isPanning = false;
});

// UI Sync & Tabs
function switchTab(tabId) {
  document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
  document.querySelectorAll('.tab-pane').forEach(p => p.classList.remove('active'));
  document.querySelector(`[onclick="switchTab('${tabId}')"]`).classList.add('active');
  document.getElementById(`tab-${tabId}`).classList.add('active');
}

function syncSelectedMacroCard(m) {
  if (!m) return;
  const halo = Array.isArray(m.halo) && m.halo.length >= 4 ? m.halo : [10, 10, 10, 10];
  document.getElementById('status-selection').innerText = `Selected: ${m.inst || 'None'}`;
  document.getElementById('lbl-sel-name').innerText = m.master || '-';
  document.getElementById('txt-inst-name').value = m.inst || '';
  document.getElementById('cmb-orientation').value = m.orient || 'R0';
  document.getElementById('sp-sel-x').value = m.x != null ? Number(m.x).toFixed(3) : '0.000';
  document.getElementById('sp-sel-y').value = m.y != null ? Number(m.y).toFixed(3) : '0.000';
  document.getElementById('sp-halo-t').value = halo[3];
  document.getElementById('sp-halo-b').value = halo[2];
  document.getElementById('sp-halo-l').value = halo[0];
  document.getElementById('sp-halo-r').value = halo[1];
}

function setActiveEdge(edge) {
  state.activeEdge = edge;
  const labels = { top: 'North (Top)', bottom: 'South (Bottom)', left: 'West (Left)', right: 'East (Right)' };
  document.getElementById('lbl-active-edge').innerText = `Active Edge: ${labels[edge]}`;
  requestAnimationFrame(render);
}

// Live HUD Sync
function syncR2M(v) {
  state.weights.r2m = parseFloat(v);
  document.getElementById('hud-r2m-slider').value = v;
  document.getElementById('hud-r2m-num').value = parseFloat(v).toFixed(2);
}
function syncR2R(v) {
  state.weights.r2r = parseFloat(v);
  document.getElementById('hud-r2r-slider').value = v;
  document.getElementById('hud-r2r-num').value = parseFloat(v).toFixed(2);
}
function syncR2IO(v) {
  state.weights.r2io = parseFloat(v);
  document.getElementById('hud-r2io-slider').value = v;
  document.getElementById('hud-r2io-num').value = parseFloat(v).toFixed(2);
}
function onSeedChanged(v) {
  state.weights.seed = parseInt(v, 10);
}
function rerollSeed() {
  const s = Math.floor(Math.random() * 90000) + 1000;
  document.getElementById('hud-seed-num').value = s;
  onSeedChanged(s);
  action('autoPlaceMacros');
}
function acceptAndLock() {
  action('saveSession');
}
function toggleHud() {
  const hud = document.getElementById('live-tuner-hud');
  state.showHud = !state.showHud;
  hud.style.display = state.showHud ? 'flex' : 'none';
}
function toggleGrid() {
  state.showGrid = !state.showGrid;
  requestAnimationFrame(render);
}
function toggleFlightlines() {
  state.showFlightlines = !state.showFlightlines;
  requestAnimationFrame(render);
}
function canvasZoom(factor) {
  state.zoom *= factor;
  document.getElementById('status-zoom').innerText = `Zoom: ${Math.round(state.zoom * 100)}%`;
  requestAnimationFrame(render);
}
function canvasZoomFit() {
  const rect = wrapper.getBoundingClientRect();
  const dieW = (state.die && state.die.w > 0) ? state.die.w : 4000.0;
  const dieH = (state.die && state.die.h > 0) ? state.die.h : 4000.0;
  const availableW = Math.max(100, rect.width - 100);
  const availableH = Math.max(100, rect.height - 100);
  state.zoom = Math.min(availableW / dieW, availableH / dieH);
  const scaledW = dieW * state.zoom;
  const scaledH = dieH * state.zoom;
  state.panX = (rect.width - scaledW) / 2;
  state.panY = (rect.height - scaledH) / 2;
  document.getElementById('status-zoom').innerText = `Zoom: ${Math.round(state.zoom * 100)}%`;
  requestAnimationFrame(render);
}

// Native Desktop Keyboard Shortcuts Engine (Desktop Parity: floorplanner.py:1900-1920)
function isInputActive() {
  const el = document.activeElement;
  return el && (el.tagName === 'INPUT' || el.tagName === 'TEXTAREA' || el.tagName === 'SELECT');
}

function deselectAll() {
  state.selectedMacro = null;
  const selStatus = document.getElementById('status-selection');
  if (selStatus) selStatus.innerText = 'Selected: None';
  const selLbl = document.getElementById('lbl-sel-name');
  if (selLbl) selLbl.innerText = 'None';
  requestAnimationFrame(render);
}

function selectAllMacros() {
  if (state.macros.length > 0) {
    state.selectedMacro = state.macros[0];
    syncSelectedMacroCard(state.selectedMacro);
    requestAnimationFrame(render);
  }
}

function deleteSelected() {
  if (!state.selectedMacro) return;
  const idx = state.macros.findIndex(m => m.inst === state.selectedMacro.inst);
  if (idx !== -1) {
    state.macros.splice(idx, 1);
    deselectAll();
    regenerateTcl();
    requestAnimationFrame(render);
  }
}

window.addEventListener('keydown', (e) => {
  if (e.ctrlKey || e.metaKey) {
    const k = e.key.toLowerCase();
    if (k === 's') {
      e.preventDefault();
      acceptAndLock();
    } else if (k === 'z') {
      e.preventDefault();
      action('undo');
    } else if (k === 'y') {
      e.preventDefault();
      action('redo');
    } else if (k === 'a') {
      if (!isInputActive()) {
        e.preventDefault();
        selectAllMacros();
      }
    } else if (k === 'e') {
      e.preventDefault();
      action('exportTcl');
    }
    return;
  }

  if (isInputActive()) return;

  if (e.key === 'f' || e.key === 'F') {
    e.preventDefault();
    canvasZoomFit();
  } else if (e.key === 'Delete' || e.key === 'Backspace') {
    e.preventDefault();
    deleteSelected();
  } else if (e.key === 'Escape') {
    e.preventDefault();
    deselectAll();
  } else if (e.key === 'a' || e.key === 'A') {
    e.preventDefault();
    action('autoPlaceMacros');
  }
});

// Live TCL Generator (Strict OpenROAD & Desktop Parity)
function regenerateTcl() {
  let tcl = `# Silis Floorplan & Power Studio (FPS) - Generated Flow Script\n`;
  tcl += `initialize_floorplan -die_area { 0.000 0.000 ${state.die.w.toFixed(3)} ${state.die.h.toFixed(3)} } \\\n`;
  tcl += `                     -core_area { ${state.core.border.toFixed(3)} ${state.core.border.toFixed(3)} ${(state.die.w - state.core.border).toFixed(3)} ${(state.die.h - state.core.border).toFixed(3)} } \\\n`;
  tcl += `                     -site unithd\n\n`;

  tcl += `# Power Domains (IEEE 1801 UPF)\n`;
  for (const dom of state.domains) {
    if (dom.name !== 'PD_TOP') {
      tcl += `set_domain_area ${dom.name} -area { ${dom.box.map(v => v.toFixed(3)).join(' ')} }\n`;
    }
  }
  tcl += `\n# Macro Placements and Keepout Halos\n`;
  for (const m of state.macros) {
    tcl += `place_macro -inst ${m.inst} -origin { ${m.x.toFixed(3)} ${m.y.toFixed(3)} } -orient ${m.orient || 'R0'}\n`;
    const halo = Array.isArray(m.halo) && m.halo.length >= 4 ? m.halo : [10, 10, 10, 10];
    tcl += `set_macro_halo -inst ${m.inst} -halo { ${halo[0].toFixed(3)} ${halo[2].toFixed(3)} ${halo[1].toFixed(3)} ${halo[3].toFixed(3)} }\n`;
  }
  tcl += `\n# PDN Ring & Straps\n`;
  tcl += `add_pdn_ring -grid grid_top -layers { met5 met4 } -widths { 5.0 5.0 } -spacings { 2.0 2.0 }\n`;
  tcl += `add_pdn_stripe -grid grid_top -layer met4 -width 3.2 -pitch 40.0\n`;
  tcl += `add_pdn_stripe -grid grid_top -layer met5 -width 3.2 -pitch 40.0\n`;

  const ed = document.getElementById('live-tcl-editor');
  if (ed) ed.value = tcl;
}

// IPC Action Dispatcher (Dual-Schema Desktop 2.0-FPS Parity: floorplanner.py:1969-2038)
function action(name, data = {}) {
  const ed = document.getElementById('live-tcl-editor');
  const tcl = (ed && ed.value) ? ed.value : '';

  const fullState = {
    version: '2.0-FPS',
    netlist_checksum: state.netlist_checksum || '',
    is_pad_mode: state.pads && state.pads.length > 0,
    die: {
      w: state.die.w,
      h: state.die.h,
      mt: state.core.border,
      mb: state.core.border,
      ml: state.core.border,
      mr: state.core.border
    },
    core: state.core,
    macros: state.macros.map(m => ({
      name: m.master || m.name || m.inst,
      master: m.master || m.name || m.inst,
      inst_name: m.inst || m.inst_name,
      inst: m.inst || m.inst_name,
      x: m.x,
      y: m.y,
      w: m.w,
      h: m.h,
      orientation: m.orient || m.orientation || 'R0',
      orient: m.orient || m.orientation || 'R0',
      halo: Array.isArray(m.halo) ? m.halo : [10, 10, 10, 10],
      pins: m.pins || []
    })),
    power_domains: state.domains.map(d => ({
      domain_name: d.name,
      name: d.name,
      voltage: d.voltage,
      domain_type: d.domain_type || 'Internally Gated',
      box: d.box,
      halo: d.halo || [15, 15, 15, 15],
      x: d.box[0],
      y: d.box[1],
      w: d.box[2] - d.box[0],
      h: d.box[3] - d.box[1]
    })),
    domains: state.domains,
    pins: state.pads.map(p => ({
      name: p.name,
      edge: p.edge,
      pos: p.pos,
      dir: p.dir || 'input'
    })),
    pads: state.pads,
    tap_distance: 14.0,
    pdn_config: {
      enable_ring: true,
      ring_width: 5.0,
      ring_spacing: 2.0,
      ring_offset: 5.0,
      ring_h_layer: 'met5',
      ring_v_layer: 'met4',
      enable_trunks: true,
      trunk_layer: 'met4',
      trunk_width: 3.2,
      targeted_bridges: true
    },
    weights: state.weights
  };

  const payload = {
    command: 'fpsAction',
    action: name,
    tcl,
    ...data,
    state: fullState
  };
  if (window.vscode) {
    window.vscode.postMessage(payload);
  } else if (window.parent && window.parent !== window) {
    window.parent.postMessage({ fromSilisFPS: true, data: payload }, '*');
  }
}

// Session Hydration from Host or VS Code
function loadSessionData(s) {
  if (!s || typeof s !== 'object') return;
  if (s.die) {
    state.die.w = Number(s.die.w) || state.die.w;
    state.die.h = Number(s.die.h) || state.die.h;
    const border = Number(s.die.mt) || Number(s.die.ml) || (s.core && Number(s.core.border)) || state.core.border;
    state.core.border = border;
    state.core.w = state.die.w - border * 2;
    state.core.h = state.die.h - border * 2;
  }
  if (Array.isArray(s.macros)) {
    state.macros = s.macros.map(m => ({
      inst: m.inst_name || m.inst || 'inst',
      master: m.name || m.master || 'master',
      x: Number(m.x) || 0,
      y: Number(m.y) || 0,
      w: Number(m.w) || 420,
      h: Number(m.h) || 280,
      orient: m.orientation || m.orient || 'R0',
      halo: Array.isArray(m.halo) ? m.halo : [10, 10, 10, 10],
      pins: m.pins || []
    }));
  }
  if (Array.isArray(s.power_domains) && s.power_domains.length > 0) {
    state.domains = s.power_domains.map(pd => {
      const x1 = Number(pd.x) || (pd.box ? pd.box[0] : 0);
      const y1 = Number(pd.y) || (pd.box ? pd.box[1] : 0);
      const w = Number(pd.w) || (pd.box ? pd.box[2] - pd.box[0] : 400);
      const h = Number(pd.h) || (pd.box ? pd.box[3] - pd.box[1] : 400);
      const name = pd.domain_name || pd.name || 'PD_UNKNOWN';
      return {
        name,
        voltage: Number(pd.voltage) || 1.8,
        domain_type: pd.domain_type || 'Internally Gated',
        box: [x1, y1, x1 + w, y1 + h]
      };
    });
  } else if (Array.isArray(s.domains)) {
    state.domains = s.domains;
  }
  if (Array.isArray(s.pins) && s.pins.length > 0) {
    state.pads = s.pins.map(p => ({
      name: p.name || 'pin',
      edge: p.edge || 'bottom',
      pos: Number(p.pos) || 0,
      dir: p.dir || 'input'
    }));
  } else if (Array.isArray(s.pads)) {
    state.pads = s.pads;
  }
  if (s.netlist_checksum) state.netlist_checksum = s.netlist_checksum;
  if (s.weights) state.weights = { ...state.weights, ...s.weights };

  canvasZoomFit();
  regenerateTcl();
  requestAnimationFrame(render);
}

window.addEventListener('message', (event) => {
  const msg = event.data;
  if (!msg) return;
  if (msg.command === 'loadSession' && msg.session) {
    loadSessionData(msg.session);
  }
});

// Acquire VS Code API if available
try {
  if (typeof acquireVsCodeApi === 'function') {
    window.vscode = acquireVsCodeApi();
  }
} catch (e) {}

// Startup
resizeCanvas();
canvasZoomFit();
regenerateTcl();
