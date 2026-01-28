import socket
import threading
import hashlib
import re
import sys
import time
from Crypto.Cipher import AES
from Crypto.Util.number import long_to_bytes, inverse

# --- Configuration ---
HOST = '155.248.210.243'
PORT = 42125
THREADS = 20         # Parallel connections to find the 1/16 case instantly
BITS = 215
P = 26328072917139296674479506920917642091979664886232678979756108269

# The Mask: All 1s XOR 15.
# This flips bits [4..214] and preserves bits [0..3].
# If 'a' ends in 1111, this results in arithmetic negation modulo N.
TARGET_MASK = ((1 << BITS) - 1) ^ 15

# Global control
stop_event = threading.Event()

def get_hash(seed_str):
    h = hashlib.sha256(seed_str.encode()).digest()
    return int.from_bytes(h, 'big') & ((1 << BITS) - 1)

def find_chain(target):
    """Calculates the seed chain using Gaussian Elimination."""
    print(f"[*] Calculating seed chain for mask {hex(target)}...")
    seeds = [str(i) for i in range(1000)]
    hashes = [get_hash(s) for s in seeds]
    basis = {}
    
    for i, h in enumerate(hashes):
        temp_h = h
        temp_indices = {i}
        for bit in range(BITS - 1, -1, -1):
            if (temp_h >> bit) & 1:
                if bit in basis:
                    b_val, b_indices = basis[bit]
                    temp_h ^= b_val; temp_indices ^= b_indices
                else:
                    basis[bit] = (temp_h, temp_indices); break
    
    result = set()
    for bit in range(BITS - 1, -1, -1):
        if (target >> bit) & 1:
            if bit not in basis: return None
            target ^= basis[bit][0]; result ^= basis[bit][1]
            
    final_seeds = [seeds[i] for i in result]
    return final_seeds

def worker(chain, worker_id):
    # Pre-construct the payload to send in one burst (Pipeline)
    # This prevents network latency from causing timeouts
    payload = b""
    for seed in chain:
        payload += b"1\n" + seed.encode() + b"\n"
    payload += b"2\n"

    while not stop_event.is_set():
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            s.settimeout(10)
            s.connect((HOST, PORT))
            
            # 1. Read Initial Data
            buf = b""
            while b"hint_1 =" not in buf:
                chunk = s.recv(4096)
                if not chunk: raise Exception
                buf += chunk
            
            content = buf.decode(errors='ignore')
            flag_match = re.search(r"flag = (\d+)", content)
            h1_match = re.search(r"hint_1 = (\d+)", content)
            
            if not flag_match or not h1_match:
                s.close(); continue
                
            enc_flag = int(flag_match.group(1))
            h1 = int(h1_match.group(1))
            
            # 2. Burst Send Chain
            s.sendall(payload)
            
            # 3. Read Hint 2
            buf = b""
            while b"hint_2 =" not in buf:
                chunk = s.recv(4096)
                if not chunk: break
                buf += chunk
            s.close()
            
            h2_match = re.search(r"hint_2 = (\d+)", buf.decode(errors='ignore'))
            if not h2_match: continue
            h2 = int(h2_match.group(1))
            
            # 4. Attempt Key Recovery: k = (h1 + h2) / 2
            # Valid only if a_new = -a (mod N)
            k_candidate = ((h1 + h2) * inverse(2, P)) % P
            
            # Key must be 128-bit
            if k_candidate >> 128: continue
            
            try:
                # Decrypt
                key_bytes = long_to_bytes(k_candidate).rjust(16, b'\0')
                ct = long_to_bytes(enc_flag)
                if len(ct) % 16 != 0: ct = b'\0'*(16-(len(ct)%16)) + ct
                
                cipher = AES.new(key_bytes, AES.MODE_ECB)
                pt = cipher.decrypt(ct)
                
                if b"ictf{" in pt:
                    print(f"\n\n[!!!] THREAD {worker_id} SUCCESS [!!!]")
                    print(f"[+] KEY: {k_candidate}")
                    print(f"[+] FLAG: {pt.decode(errors='ignore').strip()}")
                    stop_event.set()
                    return
            except: pass
            
        except Exception:
            pass

def main():
    chain = find_chain(TARGET_MASK)
    if not chain:
        print("[-] Failed to generate chain."); return
    
    print(f"[*] Chain length: {len(chain)}")
    print(f"[*] Starting {THREADS} threads (Success Rate: 1/16)...")
    
    threads = []
    for i in range(THREADS):
        t = threading.Thread(target=worker, args=(chain, i))
        t.daemon = True
        t.start()
        threads.append(t)
        
    try:
        while not stop_event.is_set():
            time.sleep(1)
    except KeyboardInterrupt:
        stop_event.set()

if __name__ == "__main__":
    main()
