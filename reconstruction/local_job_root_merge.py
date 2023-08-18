import os
import subprocess

# Shell script content

run_content = f"""
        #!/bin/bash
        hadd -f lctuple_Output_REC_bib_1point5tev_hsbib_2000all.root  """

for i in range(100,2100,100):
        ith_root_file_name = f"""lctuple_Output_REC_bib_1point5tev_hsbib_{i}.root """
        run_content += ith_root_file_name
        # Write the shell script file

run_script_filename = f'run_local_root_merger.sh'
with open(run_script_filename, 'w') as script_file:
    script_file.write(run_content)
os.chmod(run_script_filename, 0o755)
     # Make the shell script file executable
     # Execute the shell script
os.system(f'./{run_script_filename}')
