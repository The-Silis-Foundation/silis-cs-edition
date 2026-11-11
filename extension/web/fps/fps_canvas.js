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
function render() {
  const rect = wrapper.getBoundingClientRect();
  ctx.clearRect(0, 0, rect.width, rect.height);

  // Background
  ctx.fillStyle = '#0f0f17';
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

  // 2. Outer Die Boundary
  const dieP1 = worldToScreen(0, 0);
  const dieW = state.die.w * state.zoom;
  const dieH = state.die.h * state.zoom;
  ctx.strokeStyle = '#6c7086';
  ctx.lineWidth = 1.5;
  ctx.strokeRect(dieP1.x, dieP1.y - dieH, dieW, dieH);

  // Die Dimension Text
  ctx.fillStyle = '#a6adc8';
  ctx.font = '10px ui-monospace, monospace';
  ctx.fillText(`Die: ${state.die.w} × ${state.die.h} µm`, dieP1.x + 8, dieP1.y - dieH - 6);

  // 3. Inner Core Boundary
  const coreX = state.core.border;
  const coreY = state.core.border;
  const coreW = (state.die.w - state.core.border * 2) * state.zoom;
  const coreH = (state.die.h - state.core.border * 2) * state.zoom;
  const coreP = worldToScreen(coreX, coreY);
  ctx.strokeStyle = '#89b4fa';
  ctx.lineWidth = 1.5;
  ctx.strokeRect(coreP.x, coreP.y - coreH, coreW, coreH);

  // 4. Power Domain Islands (UPF)
  for (const dom of state.domains) {
    if (dom.name === 'PD_TOP') continue; // Skip core top domain for clarity
    const [x1, y1, x2, y2] = dom.box;
    const dp = worldToScreen(x1, y1);
    const dw = (x2 - x1) * state.zoom;
    const dh = (y2 - y1) * state.zoom;
    ctx.fillStyle = dom.color;
    ctx.fillRect(dp.x, dp.y - dh, dw, dh);
    ctx.strokeStyle = dom.stroke;
    ctx.lineWidth = 1.5;
    ctx.setLineDash([4, 4]);
    ctx.strokeRect(dp.x, dp.y - dh, dw, dh);
    ctx.setLineDash([]);
    ctx.fillStyle = dom.stroke;
    ctx.fillText(`${dom.name} (${dom.voltage.toFixed(3)} V)`, dp.x + 6, dp.y - dh + 14);
  }

  // 5. Placed Macros
  for (const m of state.macros) {
    const isSel = state.selectedMacro && state.selectedMacro.inst === m.inst;
    const mp = worldToScreen(m.x, m.y);
    const mw = m.w * state.zoom;
    const mh = m.h * state.zoom;

    // Halo (Dashed Pink keepout)
    const [hl, hr, hb, ht] = m.halo;
    const hp = worldToScreen(m.x - hl, m.y - hb);
    const hw = (m.w + hl + hr) * state.zoom;
    const hh = (m.h + hb + ht) * state.zoom;
    ctx.strokeStyle = '#f38ba8';
    ctx.lineWidth = 1;
    ctx.setLineDash([3, 3]);
    ctx.strokeRect(hp.x, hp.y - hh, hw, hh);
    ctx.setLineDash([]);

    // Macro Block Body
    ctx.fillStyle = isSel ? '#45475a' : '#313244';
    ctx.fillRect(mp.x, mp.y - mh, mw, mh);
    ctx.strokeStyle = isSel ? '#f9e2af' : '#89b4fa';
    ctx.lineWidth = isSel ? 2 : 1.5;
    ctx.strokeRect(mp.x, mp.y - mh, mw, mh);

    // Orientation Badge
    ctx.fillStyle = '#89b4fa';
    ctx.font = '9px monospace';
    ctx.fillText(`[${m.orient}]`, mp.x + 4, mp.y - mh + 12);

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

  // 7. Interconnect Flightlines
  if (state.showFlightlines && state.macros.length >= 2) {
    ctx.strokeStyle = 'rgba(137, 180, 250, 0.45)';
    ctx.lineWidth = 1;
    const m0 = state.macros[0];
    const m1 = state.macros[1];
    const p1 = worldToScreen(m0.x + m0.w, m0.y + m0.h / 2);
    const p2 = worldToScreen(m1.x, m1.y + m1.h / 2);
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    const cx = (p1.x + p2.x) / 2;
    ctx.bezierCurveTo(cx, p1.y, cx, p2.y, p2.x, p2.y);
    ctx.stroke();
  }
}

// Render Peripheral Pad Rings
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
    ctx.fillStyle = isActive ? 'rgba(137, 180, 250, 0.35)' : 'rgba(255, 255, 255, 0.05)';
    ctx.fillRect(rect.x, rect.y, rect.w, rect.h);
    ctx.strokeStyle = isActive ? '#89b4fa' : '#313244';
    ctx.lineWidth = 1;
    ctx.strokeRect(rect.x, rect.y, rect.w, rect.h);
  }

  // Draw Pad markers
  for (const pad of state.pads) {
    let px = 0, py = 0;
    if (pad.edge === 'bottom') { px = pad.pos; py = 0; }
    else if (pad.edge === 'top') { px = pad.pos; py = state.die.h; }
    else if (pad.edge === 'left') { px = 0; py = pad.pos; }
    else if (pad.edge === 'right') { px = state.die.w; py = pad.pos; }

    const sp = worldToScreen(px, py);
    ctx.fillStyle = '#f9e2af';
    ctx.fillRect(sp.x - 3, sp.y - 3, 6, 6);
  }
}

// Canvas Mouse Navigation & Manipulation
canvas.addEventListener('wheel', (e) => {
  e.preventDefault();
  const rect = wrapper.getBoundingClientRect();
  const mx = e.clientX - rect.left;
  const my = e.clientY - rect.top;

  const oldWorld = screenToWorld(mx, my);
  const factor = e.deltaY < 0 ? 1.15 : 0.85;
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
  document.getElementById('status-selection').innerText = `Selected: ${m.inst}`;
  document.getElementById('lbl-sel-name').innerText = m.master;
  document.getElementById('txt-inst-name').value = m.inst;
  document.getElementById('cmb-orientation').value = m.orient;
  document.getElementById('sp-sel-x').value = m.x.toFixed(3);
  document.getElementById('sp-sel-y').value = m.y.toFixed(3);
  document.getElementById('sp-halo-t').value = m.halo[3];
  document.getElementById('sp-halo-b').value = m.halo[2];
  document.getElementById('sp-halo-l').value = m.halo[0];
  document.getElementById('sp-halo-r').value = m.halo[1];
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
  state.zoom = Math.min((rect.width - 100) / state.die.w, (rect.height - 100) / state.die.h);
  state.panX = 50;
  state.panY = 50;
  document.getElementById('status-zoom').innerText = `Zoom: ${Math.round(state.zoom * 100)}%`;
  requestAnimationFrame(render);
}

// Live TCL Generator
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
  tcl += `\n# Macro Placements\n`;
  for (const m of state.macros) {
    tcl += `place_macro -inst ${m.inst} -origin { ${m.x.toFixed(3)} ${m.y.toFixed(3)} } -orient ${m.orient}\n`;
  }
  tcl += `\n# PDN Ring & Straps\n`;
  tcl += `add_pdn_ring -grid grid_top -layers { met5 met4 } -widths { 5.0 5.0 } -spacings { 2.0 2.0 }\n`;
  tcl += `add_pdn_stripe -grid grid_top -layer met4 -width 3.2 -pitch 40.0\n`;
  tcl += `add_pdn_stripe -grid grid_top -layer met5 -width 3.2 -pitch 40.0\n`;

  const ed = document.getElementById('live-tcl-editor');
  if (ed) ed.value = tcl;
}

// IPC Action Dispatcher
function action(name, data = {}) {
  const payload = { command: 'fpsAction', action: name, ...data, state: { die: state.die, core: state.core, macros: state.macros, weights: state.weights } };
  if (window.vscode) {
    window.vscode.postMessage(payload);
  } else if (window.parent && window.parent !== window) {
    window.parent.postMessage({ fromSilisFPS: true, data: payload }, '*');
  }
}

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
