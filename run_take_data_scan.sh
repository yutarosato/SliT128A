#! /bin/tcsh -f

set HEADNAME = "output"
set VREF     =  0.0 # VREF value [mV]
set TPCHG    =  11.52 # Test pulse charge [fC] : 3.84 fC = 38.4 mV * 100fF (@1MIP)

#set CH_LIST = 123 
#set CH_LIST = `seq 122 124`
set CH_LIST = `seq 0 127`

set CTRL_DAC_LIST = `seq -31 31`
#set CTRL_DAC_LIST = 31

###########################################
(cd decoder; make;)

foreach CH( ${CH_LIST})
   echo "Channel : $CH"
   set CNT=0
   foreach CTRL_DAC( ${CTRL_DAC_LIST} )
      set TMP = `echo "obase=2; ibase=10; ${CTRL_DAC}" | bc | sed 's|-||'`
      if( ${CTRL_DAC} < 1 ) then
         set CTRL_DAC_BIT = `printf "L%05d\n" ${TMP} | sed 's|0|L|g' | sed 's|1|H|g'`
      else
         set CTRL_DAC_BIT = `printf "H%05d\n" ${TMP} | sed 's|0|L|g' | sed 's|1|H|g'`
      endif
      echo "   DAC : ${CTRL_DAC} => ${CTRL_DAC_BIT}"

      # <Slow Control>
      pwd
      cd slow_control;
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHHH LLLLL${CTRL_DAC_BIT}LLLLL
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLL${CTRL_DAC_BIT}LLLLL
      ./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLLLLLLLLLLLLL # other DAC = 0 # default
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLLHHHHHHLLLLL # other DAC = +31
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLLLHHHHHLLLLL # other DAC = -31
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLLHLHHHHLLLLL # other DAC = +15
      #./make_control.sh 1111111 $CH LLLLL${CTRL_DAC_BIT}LLHLH LLLLLLLHHHHLLLLL # other DAC = -15

      while (1)
         ./slit128sc control.dat 192.168.10.16;
         if( $? == 0 ) then
         break
         endif
      end
      #./slit128sc -d control.dat 192.168.10.16;
      #exit

      set OUTNAME = "${HEADNAME}_${VREF}_${TPCHG}_${CH}_${CTRL_DAC}"

      # <Take Data>
      cd ../;
      mkdir -p binary_data
      nc -d 192.168.10.16 24 > binary_data/${OUTNAME}.dat &
      sleep 3
      #sleep 4
      #sleep 5
      #sleep 6
      #sleep 16
      #sleep 32
      #sleep 64
      kill -9 $!
      usleep 100000 # 0.1 sec

      # <Decode>
      mkdir -p root_data
      mkdir -p root_data/tmp

      cd decoder;
      ./slit128cmd_revise_chmap ../root_data/${OUTNAME}.root ../binary_data/${OUTNAME}.dat ${VREF} ${TPCHG} ${CH} ${CTRL_DAC}
      set NOHIT = $?
      rm -f ../binary_data/${OUTNAME}.dat
      #./slit128cmd ../root_data/${OUTNAME}.root ../binary_data/${OUTNAME}.dat ${VREF} ${TPCHG} ${CH} ${CTRL_DAC} && rm -f ../binary_data/${OUTNAME}.dat
      
      if( ${NOHIT} == 0 )then
         @ CNT = 0
      else
         @ CNT += 1
      endif
      if( ${CNT} == 2 )then
         echo "FINISH"
         cd ../
         break;
      endif
      cd ../

   end # DAC LOOP
   ./run_offset_noise.sh 123 0
end # CHANNEL LOOP

hadd -f root_data/${HEADNAME}_${VREF}_${TPCHG}.root root_data/${HEADNAME}_${VREF}_${TPCHG}_*.root && mv root_data/${HEADNAME}_${VREF}_${TPCHG}_*.root root_data/tmp/.

