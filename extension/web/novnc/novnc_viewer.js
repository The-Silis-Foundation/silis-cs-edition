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
let activeMode = 'fast_peeker';
let awaitingViewerReady = false;
let fallbackHideTimer = null;

const screenWrapper = document.getElementById('screen-wrapper');
const screenEl = document.getElementById('screen');
const statusPill = document.getElementById('status-pill');
const statusText = document.getElementById('status-text');
const overlayPanel = document.getElementById('overlay-panel');
const overlayTitle = document.getElementById('overlay-title');
const overlayDesc = document.getElementById('overlay-desc');
const btnFit = document.getElementById('btn-fit');
const btnFullscreen = document.getElementById('btn-fullscreen');
const btnReconnect = document.getElementById('btn-reconnect');

function showOverlay(title, desc) {
    if (title) overlayTitle.textContent = title;
    if (desc) overlayDesc.textContent = desc;
    overlayPanel.classList.remove('hidden');
}

function hideOverlay() {
    if (fallbackHideTimer) {
        clearTimeout(fallbackHideTimer);
        fallbackHideTimer = null;
    }
    awaitingViewerReady = false;
    overlayPanel.classList.add('hidden');
}

function applyViewportScale() {
    if (rfb) {
        rfb.scaleViewport = scaleViewport;
    }
}

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
    showOverlay(
        'Connecting to Native CAD Display',
        'Establishing portless IPC tunnel to local X11 headless bridge (127.0.0.1:5901)...'
    );

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
            awaitingViewerReady = true;
            // Preserve the currently selected mode on reconnects
            vscode.postMessage({ type: 'rfb_connected', mode: activeMode });
        });

        rfb.addEventListener('firstframe', () => {
            applyViewportScale();
            setStatus('connected', 'Connected (Native 60 FPS)');
            if (!awaitingViewerReady) {
                hideOverlay();
            }
        });

        rfb.addEventListener('disconnect', (e) => {
            awaitingViewerReady = false;
            setStatus('disconnected', 'Disconnected');
            showOverlay(
                'CAD Display Session Idle',
                (e && e.detail && e.detail.clean)
                    ? 'Desktop session closed cleanly. Click Reconnect or choose a layout viewer below.'
                    : 'Connection dropped. Ensure Xvfb and x11vnc are running, then click Reconnect.'
            );
        });

        rfb.addEventListener('credentialsrequired', () => {
            rfb.sendCredentials({ password: '' });
        });

        rfb.addEventListener('securityfailure', (e) => {
            awaitingViewerReady = false;
            setStatus('disconnected', 'Auth Failed');
            showOverlay(
                'Security Handshake Failed',
                e && e.detail && e.detail.reason ? e.detail.reason : 'Authentication failed'
            );
        });

    } catch (err) {
        console.error('Failed to initialize RFB client:', err);
        setStatus('disconnected', 'Init Error');
        showOverlay('Initialization Error', err.message || String(err));
    }
}

// Button actions
if (btnFit) {
    btnFit.addEventListener('click', () => {
        scaleViewport = !scaleViewport;
        applyViewportScale();
        btnFit.classList.toggle('btn-primary', scaleViewport);
    });
}

if (btnFullscreen) {
    btnFullscreen.addEventListener('click', () => {
        if (!document.fullscreenElement) {
            screenWrapper.requestFullscreen().catch(() => {});
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
        hideOverlay();
    });
}

// Launcher button hooks
document.querySelectorAll('.launcher-btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
        e.stopPropagation();
        const mode = btn.getAttribute('data-mode');
        if (mode) {
            activeMode = mode;
            awaitingViewerReady = true;
            const titleEl = btn.querySelector('.launcher-btn-title') || btn.querySelector('span');
            const title = titleEl ? titleEl.textContent.trim() : mode;
            showOverlay('Launching CAD Engine...', `Starting ${title} on DISPLAY=:1...`);
            if (fallbackHideTimer) clearTimeout(fallbackHideTimer);
            fallbackHideTimer = setTimeout(() => {
                if (awaitingViewerReady) hideOverlay();
            }, 12000);
            vscode.postMessage({ type: 'launch_viewer', mode: mode });
        }
    });
});

// Auto-scale whenever the VS Code editor tab or flex wrapper resizes
if (typeof ResizeObserver === 'function' && screenWrapper) {
    const ro = new ResizeObserver(() => {
        requestAnimationFrame(applyViewportScale);
    });
    ro.observe(screenWrapper);
}
window.addEventListener('resize', applyViewportScale);

// Listen for incoming commands from extension host
window.addEventListener('message', (event) => {
    const msg = event.data;
    if (!msg || typeof msg !== 'object') return;

    if (msg.type === 'viewer_ready') {
        hideOverlay();
        setTimeout(applyViewportScale, 50);
    } else if (msg.type === 'viewer_error') {
        awaitingViewerReady = false;
        showOverlay('CAD Engine Launch Failed', msg.error || 'Viewer process exited unexpectedly.');
    } else if (msg.type === 'viewer_launched') {
        if (msg.mode) activeMode = msg.mode;
        overlayTitle.textContent = 'Rendering Layout...';
        overlayDesc.textContent = `Waiting for ${msg.mode || 'CAD engine'} window on DISPLAY=:1...`;
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
