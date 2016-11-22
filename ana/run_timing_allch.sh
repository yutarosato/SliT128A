#! /bin/tcsh -f

set CH_LIST   = `seq 0 127`
set CALIB     = "threshold_calib.dat"
set THRESHOLD = "0.768" # [fC] (e.g. : 0.2 MIP = 0.768 fC)

################################################
set BRNAME = "tpchg"
make timing                || exit
make timewalk_width_allch  || exit
mkdir -p "pic"
rm -f tmp.dat

################################################
foreach CH( ${CH_LIST} )
    set CH_NAME = `printf "%03d" $CH`
    set FILES = "pic/output0[79]*obsch${CH_NAME}_tpch${CH_NAME}.root"
    set DAC = `awk -v "ch=${CH}" -v "th=${THRESHOLD}" '{ if($1==ch){ dac=$7*th+$5; if(dac>0){ dac=int(dac+0.5);}else{ dac=int(dac-0.5);} print dac}}' ${CALIB}`
    ./timing ${BRNAME} ${CH} ${CH} ${DAC} ${FILES} | tee -a tmp.dat
end

grep "HOGETIMEWALK" tmp.dat > tmp_timewalk.dat;
grep "HOGEWIDTH"    tmp.dat > tmp_width.dat;
./timewalk_width_allch tmp_timewalk.dat tmp_width.dat;
