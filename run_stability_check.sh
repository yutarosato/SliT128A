#! /bin/tcsh -f

set DIR = tmp_stability
mkdir -p ${DIR}

foreach CNT(`seq 1 2`)
#foreach CNT(`seq 4 5`)
#foreach CNT(`seq 6 7`)
#foreach CNT(`seq 1 10`)
#foreach CNT(`seq 11 30`)
#foreach CNT(`seq 31 35`)
#foreach CNT(`seq 36 45`)
#foreach CNT(`seq 46 50`)
#foreach CNT(`seq 51 55`)
#foreach CNT(`seq 56 65`)
#foreach CNT(`seq 66 68`)
#foreach CNT(`seq 69 70`)
#foreach CNT(`seq 71 72`)
./run_take_data_scan_boost.sh
set FILE = `ls root_data/*.root`
set FILE_BASE = `basename ${FILE} .root`
mv ${FILE} ${DIR}/${FILE_BASE}_`printf "%02d" ${CNT}`.root
rm -rf root_data/tmp
end
