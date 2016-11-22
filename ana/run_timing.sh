#! /bin/tcsh -f


set CH  = 123
set CH_NAME = `printf "%03d" $CH`

#set FILES = `ls pic/output*.root`
set FILES = "pic/output0[79]*obsch${CH_NAME}_tpch${CH_NAME}.root"
#set FILES = `ls ../store/20160616/timewalk_pic/output1*.root`

#set DAC = -17
#set DAC = -4 # 0.3 MIP
set DAC = -12 # 0.2 MIP
################################################
set BRNAME = "tpchg"
make timing          || exit
make timewalk_width  || exit
mkdir -p "pic"
################################################
./timing ${BRNAME} ${CH} ${CH} ${DAC} ${FILES}
exit
################################################
rm -f tmp.dat
set DAC_LIST  = `seq -31 31`
foreach SEL_DAC( ${DAC_LIST} )
  ./timing ${BRNAME} ${CH} ${CH} ${SEL_DAC} ${FILES} | tee -a tmp.dat
end

grep "HOGETIMEWALK" tmp.dat > tmp_timewalk.dat;
grep "HOGEWIDTH"    tmp.dat > tmp_width.dat;
./timewalk_width tmp_timewalk.dat tmp_width.dat;
