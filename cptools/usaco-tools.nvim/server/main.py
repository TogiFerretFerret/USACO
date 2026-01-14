import http.server
import socketserver
import os
import io
import zipfile
import shutil
import urllib.parse

PORT = 27182
TEMPLATE_PATH = "template/USACO.cpp"
TESTCASES_DIR = "testcases"

class Handler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/submit':
            try:
                # 1. Get Problem Name from Header
                # Headers are case-insensitive in get(), but let's be precise.
                problem_name_header = self.headers.get('X-Problem-Name')
                
                if not problem_name_header:
                    self.send_error(400, "Missing X-Problem-Name header")
                    return
                
                # Decode URL-encoded name (just in case)
                problem_name = urllib.parse.unquote(problem_name_header)

                # 2. Read Raw Body (Zip Data)
                try:
                    content_length = int(self.headers['Content-Length'])
                    zip_data = self.rfile.read(content_length)
                except (ValueError, TypeError):
                    self.send_error(400, "Missing or invalid Content-Length")
                    return

                if zip_data:
                    print(f"Received {len(zip_data)} bytes for '{problem_name}'")
                    self.process_submission(problem_name, zip_data)
                    self.send_response(200)
                    self.send_header('Access-Control-Allow-Origin', '*')
                    self.end_headers()
                    self.wfile.write(b"OK")
                else:
                    self.send_error(400, "Empty body")

            except Exception as e:
                print(f"Error: {e}")
                import traceback
                traceback.print_exc()
                self.send_error(500, str(e))
        else:
            self.send_error(404)

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        # Allow our custom header
        self.send_header('Access-Control-Allow-Headers', 'Content-Type, X-Problem-Name')
        self.end_headers()

    def log_message(self, format, *args):
        # Redirect access logs to stdout so they aren't caught as "Server Error" by the plugin
        import sys
        sys.stdout.write("%s - - [%s] %s\n" %
                         (self.client_address[0],
                          self.log_date_time_string(),
                          format % args))

    def process_submission(self, name, zip_bytes):
        print(f"Processing: {name}")
        
        # 1. Create Source File
        safe_name = name.replace("/", "_")
        cpp_filename = f"{safe_name}.cpp"
        
        if not os.path.exists(cpp_filename):
            if os.path.exists(TEMPLATE_PATH):
                shutil.copy(TEMPLATE_PATH, cpp_filename)
                print(f"Created {cpp_filename}")
            else:
                print(f"Template not found at {TEMPLATE_PATH}")
                with open(cpp_filename, 'w') as f:
                    f.write("// USACO " + name + "\n")

        # 2. Handle Testcases
        if not os.path.exists(TESTCASES_DIR):
            os.makedirs(TESTCASES_DIR)

        with zipfile.ZipFile(io.BytesIO(zip_bytes)) as z:
            file_list = z.namelist()
            print(f"Zip contents: {file_list}")
            
            # Filter for .in files
            input_files = [f for f in file_list if f.endswith('.in')]
            
            # Helper to extract number for sorting
            def get_num(s):
                try:
                    # Extract last sequence of digits before extension
                    base = os.path.basename(s)
                    num = ''.join(filter(str.isdigit, base))
                    return int(num) if num else 999
                except:
                    return 999
            
            input_files.sort(key=get_num)
            
            count = 0
            for in_file in input_files:
                # USACO zips usually have matching .out files
                out_file = in_file[:-3] + '.out'
                if out_file not in file_list:
                    # Sometimes they are .sol
                    out_file = in_file[:-3] + '.sol'
                    if out_file not in file_list:
                        continue

                in_data = z.read(in_file)
                out_data = z.read(out_file)
                
                target_in = os.path.join(TESTCASES_DIR, f"{safe_name}_{count}.in")
                target_out = os.path.join(TESTCASES_DIR, f"{safe_name}_{count}.out")
                
                with open(target_in, 'wb') as f:
                    f.write(in_data)
                with open(target_out, 'wb') as f:
                    f.write(out_data)
                
                print(f"Extracted testcase {count} to {target_in}")
                count += 1

with socketserver.TCPServer(('', PORT), Handler) as httpd:
    print(f"Serving at port {PORT}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass