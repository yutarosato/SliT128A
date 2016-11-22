#! /bin/tcsh -f

set INFILE = `ls pic/scurve*.root`

################################################

make threshold_calib || exit
./threshold_calib ${INFILE}
