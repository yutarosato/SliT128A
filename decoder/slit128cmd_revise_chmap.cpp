#include <cstdio>
#include <fstream>
#include <iostream>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include <arpa/inet.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TRandom.h>
#include <TGraph.h>



const int fl_message = 0; // 0(simple message), 1(normal message), 2(detailed message)
const int n_chip =     1; // temporal setting
const int n_unit =     4;
const int n_bit  =    32;
const int n_time =  8192; // pow(2,13)

int nevt_success = 0;
int nevt_fail    = 0;
int fl_unit[n_unit] = {0};

int t_chip;
int t_unit;
int t_time;
int t_data[n_bit];
int t_chip_miss;
int t_unit_miss;
int t_time_miss;

int cnt_data = 0; // used for judgement of the endpoint of s-curve
int cnt_bitfall =0;
int cnt_bitfall_total =0;
int cnt_bitfall_array[4] = {0};
int fl_bitfall =-999;

TTree* tree;
int t_event;
std::vector<int> t_chip_v;
std::vector<int> t_unit_v;
std::vector<int> t_bit_v;
std::vector<int> t_time_v;

std::vector<int> t_chip_miss_v;
std::vector<int> t_unit_miss_v;
std::vector<int> t_time_miss_v;

float t_vref  = -999;
float t_tpchg = -999;
int   t_selch = -999;
int   t_dac   = -999;

int read_4_bytes( FILE *fp, unsigned char *buf ){ // -1(fread err), 0(end file), +n(correctly read n-byte)
  int n;
  n = fread( buf, 1, 4, fp );
  if( n==0 ){
    if( ferror(fp) ){
      warn("fread error");
      return -1;
    }
    if( feof(fp) ) return 0;
  }
  return n;
}

int fill_event_buf( FILE *fp, unsigned char *event_buf ){ // 0(correctly read one event), -1(end event (or fread err))
  int n;
  static bool first_read = true;
  static unsigned char tmpbuf[4];
  int event_data_len = 0;
  
  if( first_read ){
    n = read_4_bytes( fp, tmpbuf );
    if( n <= 0 ) return -1;
    memcpy( &event_buf[event_data_len], tmpbuf, 4 );
    first_read = false;
  }else{
    memcpy( &event_buf[event_data_len], tmpbuf, 4 );
  }
  event_data_len += 4; /* first time: read from file. after second time stored in tmpbuf */
  
  n = read_4_bytes( fp, tmpbuf );
  if( n <= 0 ) return -1;
  memcpy( &event_buf[event_data_len], tmpbuf, 4 );
  event_data_len += 4;
  
  n = read_4_bytes( fp, tmpbuf );
  if( n <= 0 ) return -1;
  memcpy( &event_buf[event_data_len], tmpbuf, 4 );
  event_data_len += 4;
  
  while( 1 ){
    n = read_4_bytes( fp, tmpbuf );
    if( n <= 0 ) return -1;

    if( (tmpbuf[0] & 0x80) == 0x00 ){
      if( fl_message > 0 ) printf( "    fill_event_buf done %d bytes\n", event_data_len);
      return event_data_len;
    }else{
      memcpy( &event_buf[event_data_len], tmpbuf, 4 );
      event_data_len += 4; /* increment by above (***) read_4_bytes() */
      
      n = read_4_bytes( fp, tmpbuf );
      memcpy( &event_buf[event_data_len], tmpbuf, 4 );
      event_data_len += 4;
      if( n <= 0 ) return -1;
    }
  }
  
  return 0;
}


int set_tree(){
  tree = new TTree("slit128A","slit128A");
  tree->Branch( "event",  &t_event,  "event/I" );
  tree->Branch( "chip",   &t_chip_v );
  tree->Branch( "unit",   &t_unit_v );
  tree->Branch( "bit",    &t_bit_v  );
  tree->Branch( "time",   &t_time_v );
  tree->Branch( "vref",   &t_vref,  "vref/F"  );
  tree->Branch( "tpchg",  &t_tpchg, "tpchg/F" );
  tree->Branch( "selch",  &t_selch, "selch/I" );
  tree->Branch( "dac",    &t_dac,   "dac/I"   );
  tree->Branch( "chip_miss",   &t_chip_miss_v );
  tree->Branch( "unit_miss",   &t_unit_miss_v );
  tree->Branch( "time_miss",   &t_time_miss_v );

  return 0;
}

int init_tree(){
  for( int ibit=0; ibit<n_bit; ibit++ ) t_data[ibit] = 0;
  t_chip_v.clear();
  t_unit_v.clear();
  t_bit_v.clear();
  t_time_v.clear();
  t_chip_miss_v.clear();
  t_unit_miss_v.clear();
  t_time_miss_v.clear();
  
  return 0;
}

int delete_tree(){
  tree;
  return 0;
}

/*
int unit_id_mapping( int unit ){
  int flipped_unit = 0;
  if     ( unit==0 ) flipped_unit = 3;
  else if( unit==1 ) flipped_unit = 2;
  else if( unit==2 ) flipped_unit = 1;
  else if( unit==3 ) flipped_unit = 0;
  else std::cerr << "[ABORT] Wrong unit-ID : " << unit << std::endl;
  return flipped_unit;
}
*/

int bit_flip( bool bit ){
  if( bit ) return false;
  else      return true;
}

int decode( unsigned char *buf, int length ){
  unsigned short* event_number = (unsigned short*)&buf[2];
  int ndata = (length-4)/8;
  if( fl_message > 0 ) printf( "       [ Event#=%d : #Data=%d : ", ntohs(*event_number), ndata );
  //t_event = ntohs(*event_number);

  for( int idata=0; idata<ndata; idata++ ){
    unsigned char   chip_id   = buf[8*idata+4]; chip_id = ( chip_id & 0x7f );
    unsigned char   unit_id   = buf[8*idata+5];
    unsigned short* time_info = (unsigned short*)&buf[8*idata+6];
    unsigned long*  data      = (unsigned long* )&buf[8*idata+8];
    if( idata==0 && (int)unit_id==0 ) t_event = ntohs(*event_number);
    if( idata==0 && fl_message > 0 ) printf( "Unit-ID=%d ] \n", (int)unit_id );
    if( fl_message > 1 ) printf( "%3d : (Chip-ID=%d, Unit-ID=%d) : (time=%d, data=%x)\n", idata, (int)chip_id, (int)unit_id, ntohs(*time_info), ntohl(*data) );

    t_chip  = chip_id;
    t_unit  = unit_id;
    t_time = ntohs(*time_info);

    t_chip_miss = t_chip; // bit-fall study
    t_time_miss = t_time; // bit-fall study

    cnt_bitfall_total++;
    if( fl_bitfall<0 ) fl_bitfall = t_time;
    if( fl_bitfall==t_time ){
      cnt_bitfall++;
      //printf("CONTINUE : t = %d : %d\n",t_time,cnt_bitfall);
    }else{
      //printf("DET NEXT TIME : t = %d : %d\n",t_time,cnt_bitfall);
      if( cnt_bitfall!=4 ){
	//printf("     => BITFALL : t = %d=>%d : %d : unit_id = ",fl_bitfall,t_time,cnt_bitfall );
	int fl_bitfall_det[4] = {0};
	for( int ifall=0; ifall<cnt_bitfall; ifall++ ) fl_bitfall_det[cnt_bitfall_array[ifall]]++;
	for( int ifall=0; ifall<4;           ifall++ ){
	  if(fl_bitfall_det[ifall]==0){
	    //printf("%d,",ifall);
	    t_unit_miss = ifall;
	    t_chip_miss_v.push_back(t_chip_miss);
	    t_unit_miss_v.push_back(t_unit_miss);
	    t_time_miss_v.push_back(t_time_miss);
	  }
	}
	//printf("\n");
      }
      cnt_bitfall = 1;
      fl_bitfall = t_time;
    }
    cnt_bitfall_array[cnt_bitfall-1] = t_unit;

    //t_unit = unit_id_mapping( t_unit ); // unit-ID correction // removed for ch-map correction@20161004

    for( int ibyte=0; ibyte<4; ibyte++ ){ // for-loop from large ch number to small ch number
      unsigned char byte_data = buf[8*idata+8+ibyte];

      if( fl_message > 1 ) std::cout << "("
				     << (int )((unsigned char)(byte_data)) << " : "
				     << (bool)((unsigned char)(byte_data & 0x80)) << " "
				     << (bool)((unsigned char)(byte_data & 0x40)) << " "
				     << (bool)((unsigned char)(byte_data & 0x20)) << " "
				     << (bool)((unsigned char)(byte_data & 0x10)) << "  "
				     << (bool)((unsigned char)(byte_data & 0x08)) << " "
				     << (bool)((unsigned char)(byte_data & 0x04)) << " "
				     << (bool)((unsigned char)(byte_data & 0x02)) << " "
				     << (bool)((unsigned char)(byte_data & 0x01)) << ") ";
      if( fl_message > 1 && ibyte==3 ) std::cout << std::endl;

      t_data[7+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x80)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[6+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x40)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[5+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x20)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[4+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x10)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[3+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x08)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[2+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x04)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[1+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x02)) ); // bit-flip correction // modified for ch-map correction @20161004
      t_data[0+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x01)) ); // bit-flip correction // modified for ch-map correction @20161004
      
      t_chip = 0; // temporal setting for single chip test.
      for( int i=0; i<8; i++ ){
	if( t_data[i+(3-ibyte)*8] ){ // modified for ch-map correction @20161004
	  t_chip_v.push_back(t_chip);
	  t_unit_v.push_back(t_unit);
	  t_bit_v.push_back (i+(3-ibyte)*8); // modified for ch-map correction @20161004
	  t_time_v.push_back(t_time);
	}
      }
    }

    //if( idata%5000==0 ) std::cout << idata << std::endl;
    //if( t_unit!=3 ) continue;
    fl_unit[t_unit]++;
  }
    {
      int tmp_fl = 1;
      for( int iunit=0; iunit<n_unit; iunit++ ) tmp_fl *= (int)((bool)fl_unit[iunit]);
      if( tmp_fl==1 ){
        if( fl_message   ) printf( "=>[ Event#=%d : #Data=%d+3 ]\n", t_event, ndata );
	if( ndata!=32765 ){
	  printf( "=>[ Event#=%d : #Data=%d+3 ] * lost data event : not saved int tree\n", t_event, ndata );
	  tree->Fill(); // tmpppp
	  cnt_data += t_time_v.size();// tmpppp
	  nevt_success++; // tmpppp
	  //nevt_fail++;
	}else{
	  tree->Fill();
	  cnt_data += t_time_v.size();
	  nevt_success++;
	}
	
	init_tree();
	for( int iunit=0; iunit<n_unit; iunit++ ) fl_unit[iunit] = 0;
      }
    }
     
  return 0;
}

int main( int argc, char *argv[] ){
  unsigned char event_buf[1024*1024];
  int n;
  
  FILE *fp;
  char *data_filename;
  char *out_filename;

  /*
  if( argc == 1 ) fp = stdin;
  else{
    data_filename = argv[1];
    fp = fopen( data_filename, "r" );
    if( fp == NULL ){
      err( EXIT_FAILURE, "fopen" );
    }
  }
  */
  if( argc<2 ){
    std::cerr << "Usage : "
	      << argv[0] << "  "
	      << "output_rootfile  input_binary_file  (VREF) (Test Pulse charge) (selected channel-ID)  (DAC value)" << std::endl;
    abort();    
  }else{
    out_filename = argv[1];

    if( argc==2 ) fp = stdin;
    else if( argc>2 ){
      data_filename = argv[2];
      fp = fopen( data_filename, "r" );
      if( fp == NULL ) err( EXIT_FAILURE, "fopen" );
    }

    if( argc>3 ) t_vref  = atof(argv[3]);
    if( argc>4 ) t_tpchg = atof(argv[4]);
    if( argc>5 ) t_selch = atoi(argv[5]);
    if( argc>6 ) t_dac   = atoi(argv[6]);
  }

  // open output file and define tree's branches
  TFile outfile( out_filename, "RECREATE" );
  set_tree();
  init_tree();
  
  while(1){
    n = fill_event_buf( fp, event_buf ); // read one event
    if( n < 0 ) break;
    else decode( event_buf, n ); // save it in tree
  }

  std::cout << "#event (success/fail) = "
	    << nevt_success << "/"
	    << nevt_fail    << ", "
	    << "#hit = "    << cnt_data
	    << std::endl;
  std::ofstream outlog("tmp.log");
  outlog << cnt_data << std::endl;
  
  tree->Write();
  outfile.Close();

  delete_tree();

  if( cnt_data ) return 0;
  else           return 1; // no hit !
  //return 0;
}
