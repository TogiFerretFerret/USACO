import subprocess
while True:
    msg=input("> ")
    if msg=="exit":
        break
    # run picord and put msg as stdin
    process = subprocess.Popen(['picord'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # send it stdin and then whatever ctrl-d is for end of input
    _, _ = process.communicate(input=msg.encode())
    # wait for it to finish
    process.wait()

