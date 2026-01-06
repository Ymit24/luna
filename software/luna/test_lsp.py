#!/usr/bin/env python3
import subprocess
import sys

def send_message(proc, message):
    content = message.encode('utf-8')
    header = f"Content-Length: {len(content)}\r\n\r\n"
    proc.stdin.write(header.encode('utf-8'))
    proc.stdin.write(content)
    proc.stdin.flush()

def read_message(proc):
    headers = {}
    while True:
        line = proc.stdout.readline().decode('utf-8')
        if line == '\r\n' or line == '\n' or line == '':
            break
        if ':' in line:
            key, value = line.split(':', 1)
            headers[key.strip()] = value.strip()

    content_length = int(headers.get('Content-Length', 0))
    if content_length > 0:
        content = proc.stdout.read(content_length).decode('utf-8')
        return content
    return ''

def main():
    proc = subprocess.Popen(
        ['./bin/lsp'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=False
    )

    initialize_request = '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}'
    send_message(proc, initialize_request)
    response = read_message(proc)
    print(f"Initialize response: {response[:200]}...")

    invalid_luna_code = "this is not valid luna code !!!"
    did_change = f'''{{
        "jsonrpc": "2.0",
        "method": "textDocument/didChange",
        "params": {{
            "textDocument": {{
                "uri": "file:///tmp/test.luna"
            }},
            "contentChanges": [
                {{
                    "text": "{invalid_luna_code}"
                }}
            ]
        }}
    }}'''
    send_message(proc, did_change)

    stderr_output = proc.stderr.read().decode('utf-8')
    print(f"STDERR output: {stderr_output}")

    import time
    import select

    if select.select([proc.stdout], [], [], 2.0)[0]:
        diag_response = read_message(proc)
        print(f"Diagnostics: {diag_response[:500]}")
    else:
        print("No diagnostics received!")

    proc.stdin.close()
    proc.wait()

if __name__ == '__main__':
    main()
