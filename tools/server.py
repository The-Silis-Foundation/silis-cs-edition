#!/usr/bin/env python3
"""
Silis CS Edition CAD Web Server Entrypoint Alias
Forwards to cs_server.py
"""
import os
import sys

tools_dir = os.path.dirname(os.path.abspath(__file__))
cs_server_path = os.path.join(tools_dir, "cs_server.py")

if __name__ == "__main__":
    with open(cs_server_path, "rb") as f:
        code = compile(f.read(), cs_server_path, "exec")
        exec(code, {"__name__": "__main__", "__file__": cs_server_path})
