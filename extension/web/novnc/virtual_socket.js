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
                        let arrayBuf = null;
                        if (typeof msg.data === 'string') {
                            try {
                                const binaryStr = atob(msg.data);
                                const len = binaryStr.length;
                                const bytes = new Uint8Array(len);
                                for (let i = 0; i < len; i++) {
                                    bytes[i] = binaryStr.charCodeAt(i);
                                }
                                arrayBuf = bytes.buffer;
                            } catch (e) {
                                console.error('VirtualVncSocket: failed to decode base64 vnc_data', e);
                            }
                        } else if (msg.data instanceof ArrayBuffer) {
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
                        } else if (typeof msg.data === 'object' && msg.data !== null) {
                            const values = Object.values(msg.data);
                            arrayBuf = new Uint8Array(values).buffer;
                        }
                        if (arrayBuf && arrayBuf.byteLength > 0) {
                            this.onmessage({ data: arrayBuf });
                        }
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

        let u8;
        if (data instanceof Uint8Array) {
            u8 = data;
        } else if (data instanceof ArrayBuffer) {
            u8 = new Uint8Array(data);
        } else if (ArrayBuffer.isView(data)) {
            u8 = new Uint8Array(data.buffer, data.byteOffset, data.byteLength);
        } else if (Array.isArray(data)) {
            u8 = new Uint8Array(data);
        } else {
            u8 = new Uint8Array(data);
        }

        let binary = '';
        const len = u8.byteLength;
        for (let i = 0; i < len; i++) {
            binary += String.fromCharCode(u8[i]);
        }
        const b64 = btoa(binary);

        this._vscode.postMessage({ type: 'vnc_data', data: b64, isBase64: true });
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
