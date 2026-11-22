/**
 * novnc_viewer.js
 * 
 * Main Webview controller for noVNC via Portless Virtual IPC Socket.
 */

import RFB from './core/rfb.js';
import VirtualVncSocket from './virtual_socket.js';

const vscode = acquireVsCodeApi();

let rfb = null;
let virtualSocket = null;
let scaleViewport = true;

const screenEl = document.getElementById('screen');
const statusPill = document.getElementById('status-pill');
const statusText = document.getElementById('status-text');
const overlayPanel = document.getElementById('overlay-panel');
const overlayTitle = document.getElementById('overlay-title');
const overlayDesc = document.getElementById('overlay-desc');
const btnFit = document.getElementById('btn-fit');
const btnFullscreen = document.getElementById('btn-fullscreen');
const btnReconnect = document.getElementById('btn-reconnect');

function setStatus(state, message) {
    statusPill.className = `status-pill status-${state}`;
    statusText.textContent = message;
}

function initVnc() {
    if (rfb) {
        try {
            rfb.disconnect();
        } catch (e) {}
        rfb = null;
    }
    if (virtualSocket) {
        try {
            virtualSocket.dispose();
        } catch (e) {}
        virtualSocket = null;
    }

    screenEl.innerHTML = '';
    setStatus('connecting', 'Connecting...');
    overlayPanel.classList.remove('hidden');
    overlayTitle.textContent = 'Connecting to Native CAD Display';
    overlayDesc.textContent = 'Establishing portless IPC tunnel to local X11 headless bridge (127.0.0.1:5901)...';

    try {
        virtualSocket = new VirtualVncSocket(vscode);
        rfb = new RFB(screenEl, virtualSocket, {
            credentials: { password: '' },
            scaleViewport: scaleViewport,
            resizeSession: false,
            clipViewport: false,
        });

        rfb.viewOnly = false;
        rfb.focusOnClick = true;

        rfb.addEventListener('connect', () => {
            setStatus('connected', 'Connected (Native RFB)');
            console.log('RFB connected to local X11 desktop bridge');
            // Notify extension that RFB handshaking is complete
            vscode.postMessage({ type: 'rfb_connected' });
        });

        rfb.addEventListener('firstframe', () => {
            overlayPanel.classList.add('hidden');
            setStatus('connected', 'Connected (Native 60 FPS)');
        });

        rfb.addEventListener('disconnect', (e) => {
            setStatus('disconnected', 'Disconnected');
            overlayPanel.classList.remove('hidden');
            overlayTitle.textContent = 'CAD Display Session Idle';
            overlayDesc.textContent = (e && e.detail && e.detail.clean)
                ? 'Desktop session closed cleanly. Click Reconnect or choose a layout viewer below.'
                : 'Connection dropped. Ensure Xvfb and x11vnc are running, then click Reconnect.';
        });

        rfb.addEventListener('credentialsrequired', () => {
            rfb.sendCredentials({ password: '' });
        });

        rfb.addEventListener('securityfailure', (e) => {
            setStatus('disconnected', 'Auth Failed');
            overlayPanel.classList.remove('hidden');
            overlayTitle.textContent = 'Security Handshake Failed';
            overlayDesc.textContent = e && e.detail && e.detail.reason ? e.detail.reason : 'Authentication failed';
        });

    } catch (err) {
        console.error('Failed to initialize RFB client:', err);
        setStatus('disconnected', 'Init Error');
        overlayTitle.textContent = 'Initialization Error';
        overlayDesc.textContent = err.message || String(err);
    }
}

// Button actions
if (btnFit) {
    btnFit.addEventListener('click', () => {
        scaleViewport = !scaleViewport;
        if (rfb) {
            rfb.scaleViewport = scaleViewport;
        }
        btnFit.classList.toggle('btn-primary', scaleViewport);
    });
}

if (btnFullscreen) {
    btnFullscreen.addEventListener('click', () => {
        const wrapper = document.getElementById('screen-wrapper');
        if (!document.fullscreenElement) {
            wrapper.requestFullscreen().catch(() => {});
        } else {
            document.exitFullscreen().catch(() => {});
        }
    });
}

if (btnReconnect) {
    btnReconnect.addEventListener('click', () => {
        initVnc();
    });
}

// Dismiss overlay if clicked directly by user
if (overlayPanel) {
    overlayPanel.addEventListener('click', () => {
        overlayPanel.classList.add('hidden');
    });
}

// Launcher button hooks
document.querySelectorAll('.launcher-btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
        e.stopPropagation();
        const mode = btn.getAttribute('data-mode');
        if (mode) {
            vscode.postMessage({ type: 'launch_viewer', mode: mode });
            overlayTitle.textContent = 'Launching CAD Engine...';
            const titleEl = btn.querySelector('.launcher-btn-title') || btn.querySelector('span');
            const title = titleEl ? titleEl.textContent.trim() : mode;
            overlayDesc.textContent = `Starting ${title} on DISPLAY=:1...`;
            // Auto-hide overlay after brief launch window so it never hangs
            setTimeout(() => {
                overlayPanel.classList.add('hidden');
            }, 1200);
        }
    });
});

// Auto-scale on window / tab resize
window.addEventListener('resize', () => {
    if (rfb) {
        rfb.scaleViewport = scaleViewport;
    }
});

// Listen for incoming commands from extension host
window.addEventListener('message', (event) => {
    const msg = event.data;
    if (!msg || typeof msg !== 'object') return;

    if (msg.type === 'viewer_launched') {
        overlayTitle.textContent = 'Rendering Layout...';
        overlayDesc.textContent = `Streaming ${msg.mode || 'CAD engine'} frame...`;
        setTimeout(() => {
            overlayPanel.classList.add('hidden');
        }, 600);
    } else if (msg.type === 'project_info') {
        const tag = document.getElementById('project-tag');
        if (tag && msg.projectName) {
            tag.textContent = msg.projectName;
        }
    } else if (msg.type === 'status_update') {
        if (msg.message) {
            overlayDesc.textContent = msg.message;
        }
    }
});

// Auto-start connection on page load (support both early and deferred module execution)
if (document.readyState === 'loading') {
    window.addEventListener('DOMContentLoaded', () => {
        initVnc();
    });
} else {
    initVnc();
}
