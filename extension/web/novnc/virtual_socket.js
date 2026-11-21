/**
 * virtual_socket.js
 * 
 * Portless Webview IPC Socket for noVNC.
 * Implements the rawChannel interface required by noVNC's Websock wrapper:
 * [send, close, binaryType, onerror, onmessage, onopen, protocol, readyState].
 * 
 * Instead of establishing an external TCP/WebSocket network connection (which gets
 * blocked by GitHub Codespaces X-Frame-Options: DENY and interstitial gates),
 * this virtual socket proxies all RFB binary frames over VS Code's internal
 * postMessage IPC pipe directly to the extension host's local TCP bridge (127.0.0.1:5901).
 */

export default class VirtualVncSocket {
    constructor(vscodeApi) {
        this._vscode = vscodeApi;
        this.binaryType = "arraybuffer";
        this.protocol = "binary";
        this.readyState = 0; // WebSocket.CONNECTING

        this.onerror = () => {};
        this.onmessage = () => {};
        this.onopen = () => {};
        this.onclose = () => {};

        this._messageListener = (event) => {
            const msg = event.data;
            if (!msg || typeof msg !== 'object') return;

            switch (msg.type) {
                case 'vnc_open':
                    this.readyState = 1; // WebSocket.OPEN
                    if (typeof this.onopen === 'function') {
                        this.onopen();
                    }
                    break;

                case 'vnc_data':
                    if (this.readyState === 1 && typeof this.onmessage === 'function') {
                        let arrayBuf;
                        if (msg.data instanceof ArrayBuffer) {
                            arrayBuf = msg.data;
                        } else if (ArrayBuffer.isView(msg.data)) {
                            arrayBuf = msg.data.buffer.slice(
                                msg.data.byteOffset,
                                msg.data.byteOffset + msg.data.byteLength
                            );
                        } else if (Array.isArray(msg.data)) {
                            arrayBuf = new Uint8Array(msg.data).buffer;
                        } else if (msg.data && msg.data.type === 'Buffer' && Array.isArray(msg.data.data)) {
                            arrayBuf = new Uint8Array(msg.data.data).buffer;
                        } else {
                            try {
                                arrayBuf = new Uint8Array(msg.data).buffer;
                            } catch (e) {
                                console.error('VirtualVncSocket: failed to convert data to ArrayBuffer', e);
                                return;
                            }
                        }
                        this.onmessage({ data: arrayBuf });
                    }
                    break;

                case 'vnc_close':
                    this.readyState = 3; // WebSocket.CLOSED
                    if (typeof this.onclose === 'function') {
                        this.onclose({ code: 1000, reason: msg.reason || 'Normal closure', wasClean: true });
                    }
                    break;

                case 'vnc_error':
                    if (typeof this.onerror === 'function') {
                        this.onerror(new Error(msg.error || 'VNC bridge connection error'));
                    }
                    break;
            }
        };

        window.addEventListener('message', this._messageListener);
        this.connect();
    }

    connect() {
        this.readyState = 0;
        this._vscode.postMessage({ type: 'vnc_connect' });
    }

    send(data) {
        if (this.readyState !== 1) {
            console.warn('VirtualVncSocket.send called when readyState is not OPEN:', this.readyState);
            return;
        }

        let bytes;
        if (data instanceof Uint8Array) {
            bytes = Array.from(data);
        } else if (data instanceof ArrayBuffer) {
            bytes = Array.from(new Uint8Array(data));
        } else if (ArrayBuffer.isView(data)) {
            bytes = Array.from(new Uint8Array(data.buffer, data.byteOffset, data.byteLength));
        } else if (Array.isArray(data)) {
            bytes = data;
        } else {
            bytes = Array.from(new Uint8Array(data));
        }

        this._vscode.postMessage({ type: 'vnc_data', data: bytes });
    }

    close() {
        this.readyState = 3;
        this._vscode.postMessage({ type: 'vnc_close' });
        if (typeof this.onclose === 'function') {
            this.onclose({ code: 1000, reason: 'Client initiated close', wasClean: true });
        }
    }

    dispose() {
        window.removeEventListener('message', this._messageListener);
        this.close();
    }
}
