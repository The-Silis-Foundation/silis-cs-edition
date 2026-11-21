# ADR 016: Portless VNC Webview IPC Tunnel for Zero-Defect Codespaces CAD Viewing

## Status
**ACCEPTED** (2026-11-22)

## Context
In GitHub Codespaces and web-based VS Code environments, users attempting to run native ASIC physical design desktop applications (such as OpenROAD GUI, Silis FastSiliconPeeker, and LayerViewerPanel) face severe browser permission and security restrictions:
1. **Third-Party Cookie Partitioning (CHIPS)**: Iframes attempting to load private forwarded ports (`https://<port>-<name>.githubpreview.dev`) drop GitHub authentication cookies, causing redirects to `github.com/login` with `X-Frame-Options: DENY`.
2. **Anti-Phishing Interstitials**: Public forwarded ports return `HTTP 200 OK` HTML warning interstitial pages, intercepting and breaking the WebSocket Upgrade handshake.
3. **Mock Canvas Loss of Fidelity**: Replacing the genuine C++ OpenGL layout engine with simplified 2D HTML5 canvas mocks (`peeker_canvas.js`) drops OpenROAD OpenDB pin obstruction inspection, physical layer rendering, 60 FPS OpenGL zoom/pan, and dual-mode layout viewing parity.

## Decision
We implement **Option B: Portless Webview IPC Tunnel**:
1. All RFB server traffic from `x11vnc` remains strictly local to the container on `127.0.0.1:5901`.
2. We embed noVNC v1.5.0 directly into the workbench extension (`extension/web/novnc/`).
3. We implement a custom `VirtualVncSocket` satisfying noVNC's `rawChannelProps` (`send`, `close`, `binaryType`, `onerror`, `onmessage`, `onopen`, `protocol`, `readyState`).
4. We bridge RFB binary frames across VS Code's internal `postMessage` pipe to a Node.js `net.connect({ port: 5901, host: '127.0.0.1' })` socket in `extension.js`.
5. We launch the native Qt6 C++ applications (`FastSiliconPeeker`, `LayerViewerPanel`, OpenROAD GUI) on headless display `DISPLAY=:1` (`Xvfb :1`).

## Consequences & Invariants
- **Zero Forwarded Ports**: Port 5901, 6080, and 6081 are never exposed over the network or forwarded by VS Code.
- **Zero Browser Blocks**: No external network requests, zero auth cookies, and zero interstitial gates.
- **100% Desktop Parity**: Native C++ and OpenGL layout inspection runs in Codespaces with zero code deviation from the desktop release.
