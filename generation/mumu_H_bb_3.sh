#!/bin/sh
echo "Setup whizard"
source /data/WHIZARD/bin/whizard-setup.sh
#
# go to output directory
outputdir="$HOME/WHIZARD/output/Hbb3TeV"
mkdir -p ${outputdir}
cd ${outputdir}
echo $(pwd)
#
inputdir="$HOME/WHIZARD/input"
#
${bindir}/whizard ${inputdir}/mumu_H_bb_3.sin -L mumu_H_bb.log
