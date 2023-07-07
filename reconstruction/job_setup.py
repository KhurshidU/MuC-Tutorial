import os
import subprocess

# Shell script content

for i in range(0,10):
	runi_content = f"""
		#!/bin/bash
                 Marlin --global.LCIOInputFiles=/work/khurriccan/MuC-Tutorial/simulation/mumu_H_bb.slcio  --Output_REC.LCIOOutputFile=output_file_REC_{1}.slcio  --Output_DST.LCIOOutputFile=output_file_DST_{1}.slcio  --MyAIDAProcessor.FileName=histograms_{1}  --LCIOInputFiles.MaxRecordNumber=1  --LCIOInputFiles.SkipNEvents={i}  --Config.Overlay=Test steer_reco_kh_bib_sk_0.2g.xml > log_file_{i}.log 2>&1 
	 """
	# Write the shell script file
	run_script_filename = f'run{i}.sh'
	with open(run_script_filename, 'w') as script_file:
    		script_file.write(runi_content)

	os.chmod(run_script_filename, 0o755)
	# Make the shell script file executable
	# Execute the shell script
	os.system(f'./{run_script_filename}')

for i in range(0,10):
        marlini_content = f"""
              Universe = Vanilla
	      Executable     = run{i}.sh
              +SingularityImage = "docker://ubuntu"
              Requirements = HasSingularity              

	      Error   = output.err{i}.$(Cluster)-$(Process)
              Output  = output.out{i}.$(Cluster)-$(Process)
              Log     = output.log{i}.$(Cluster)

              should_transfer_files = YES
              WhenToTransferOutput = ON_EXIT

              request_cpus = 1
              request_memory = 1 GB
              +ProjectName="snowmass21.futurecolliders"

              queue 1
        """
        # Write the shell script file
        condor_submit_filename= f'marlin{i}.submit'
        with open(condor_submit_filename, 'w') as submit_file:
                submit_file.write(marlini_content)

        # Execute the shell script
       # os.system(f'condor_submit {condor_submit_filename}')
        subprocess.call(f'condor_submit marlin{i}.submit', shell=True)


# Remove the shell script file (optional)
