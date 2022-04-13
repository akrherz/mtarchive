#!/usr/bin/env python3
"""Do the magic redirection to the Cybox URL"""

# stdlib
import cgi
import os
import re
import sys

FILENAME_RE = re.compile(r'^\d{4}/\d\d/\d\d/.*\.zip$')


def main():
    """Go Main Go."""
    form = cgi.FieldStorage()
    fn = form.getfirst('fn', '')
    if FILENAME_RE.match(fn) is None:
        print("Content-type: text/plain\n")
        print("API Failure: Invalid filename")
        return
    # filename is safe to use
    localfn = f"/isu/mtarchive/data/{fn}"
    if not os.path.isfile(localfn):
        # Return a 404
        print("Status: 404 File Not Found\n")
        print("API Failure: File not found")
        return
    # Get the size of this file
    size = os.path.getsize(localfn)
    if size < 1024:
        with open(localfn, 'r', encoding="utf-8") as f:
            url = f.read()
        print(f"Location: {url}\n\n")
        return

    # Send the file to the client
    sys.stdout.buffer.write(b"Content-type: application/zip\n")
    msg = (
        f"Content-Disposition: attachment; filename={fn.split('/')[-1]}\n"
    )
    sys.stdout.buffer.write(msg.encode("ascii"))
    msg = f"Content-Length: {size}\n\n"
    sys.stdout.buffer.write(msg.encode("ascii"))
    chunksize = 1024 * 1024 * 10
    with open(localfn, 'rb') as f:
        while True:
            data = f.read(chunksize)
            if not data:
                break
            sys.stdout.buffer.write(data)


if __name__ == "__main__":
    main()
