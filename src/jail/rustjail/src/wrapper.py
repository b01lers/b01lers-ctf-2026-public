#!/usr/bin/env python3

import shutil
import subprocess
import os
import tempfile

# wrapper script to put everythin in writable tmp directory so compiling works

def main():
    src_dir = os.path.join(os.getcwd(), 'src')

    # Create a temporary directory
    tmp_parent = tempfile.mkdtemp(prefix='tmp_')
    tmp_dir = os.path.join(tmp_parent, 'src')

    try:
        # Copy the entire src folder into tmp_dir
        shutil.copytree(src_dir, tmp_dir)

        # Path to runner.sh
        run_sh = os.path.join(tmp_dir, 'runner.sh')

        # Run the script
        subprocess.run([run_sh], cwd=tmp_dir, check=True)
    except:
        print('Command failed')

    # Cleanup
    shutil.rmtree(tmp_parent)

if __name__ == '__main__':
    main()
