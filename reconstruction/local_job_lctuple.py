import os
import subprocess

# Shell script content

for i in range(100,2100,100):
        runi_content = f"""
        #!/bin/bash
          Marlin --global.LCIOInputFiles=Output_REC_bib_1point5tev_hsbib_{i}.slcio  --MyAIDAProcessor.FileName=lctuple_Output_REC_bib_1point5tev_hsbib_{i}   lctuple_steer_jetsk.xml &"""
        # Write the shell script file
        run_script_filename = f'run_local_lctuple_{i}.sh'
        with open(run_script_filename, 'w') as script_file:
              script_file.write(runi_content)
        os.chmod(run_script_filename, 0o755)
        # Make the shell script file executable
        # Execute the shell script
        os.system(f'./{run_script_filename}')
