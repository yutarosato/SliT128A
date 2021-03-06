#include "MTree.h"

MTree::MTree(){
  m_tree = new TTree();
}

MTree::MTree( const Char_t* name ){
  m_tree  = new TTree(name,name);
}

MTree::~MTree(){
  delete m_tree;
  init_tree();
  t2_chip_v = 0;
  t2_unit_v = 0;
  t2_bit_v  = 0;
  t2_time_v = 0;
}

int MTree::ch_map( int unit, int bit ){
  return n_bit*unit + bit;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
int MTree::unit_id_mapping( int unit ){
  int flipped_unit = 0;
  if     ( unit==0 ) flipped_unit = 3;
  else if( unit==1 ) flipped_unit = 2;
  else if( unit==2 ) flipped_unit = 1;
  else if( unit==3 ) flipped_unit = 0;
  else std::cerr << "[ABORT] Wrong unit-ID : " << unit << std::endl;
  return flipped_unit;
}
*/

int MTree::bit_flip( bool bit ){
  if( bit ) return false;
  else      return true;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int MTree::set_readbranch(){
  m_tree->SetBranchAddress("event", &t_event    );
  m_tree->SetBranchAddress("chip",  &t2_chip_v  );
  m_tree->SetBranchAddress("unit",  &t2_unit_v  );
  m_tree->SetBranchAddress("bit",   &t2_bit_v   );
  m_tree->SetBranchAddress("time",  &t2_time_v  );

  t2_chip_v->clear();
  t2_unit_v->clear();
  t2_bit_v ->clear();
  t2_time_v->clear();

  return 0;
}

int MTree::set_writebranch(){
  m_tree->Branch( "event",  &t_event,  "event/I" );
  m_tree->Branch( "chip",   &t_chip_v );
  m_tree->Branch( "unit",   &t_unit_v );
  m_tree->Branch( "bit",    &t_bit_v  );
  m_tree->Branch( "time",   &t_time_v );
  
  return 0;
}

int MTree::init_tree(){
  t_chip_v.clear();
  t_unit_v.clear();
  t_bit_v.clear();
  t_time_v.clear();
  m_tree->Reset();
  
  return 0;
}


int MTree::decode_data(const unsigned char* mydata, int length)
{
  init_tree();
  unsigned short* event_number = (unsigned short*)&mydata[2];

  int ndata = (length-16)/8;
  if( fl_message > 1 ) printf( "       [ Event#=%d : #Data=%d]\n", ntohs(*event_number), ndata );
  //t_event = ntohs(*event_number);
  int adjust = 0;
  for( int idata=0; idata<ndata; idata++ ){
    unsigned char   chip_id   = mydata[8*idata+adjust+4]; chip_id = ( chip_id & 0x7f );
    unsigned char   unit_id   = mydata[8*idata+adjust+5];
    unsigned short* time_info = (unsigned short*)&mydata[8*idata+adjust+6];
    unsigned long*  data      = (unsigned long* )&mydata[8*idata+adjust+8];
    if( idata<4  && (int)unit_id==0 ) t_event = ntohs(*event_number);
    if( fl_message > 1 ) printf( "%3d : (Chip-ID=%d, Unit-ID=%d) : (time=%d, data=%x)\n", idata, (int)chip_id, (int)unit_id, ntohs(*time_info), ntohl(*data) );

    int tmp_chip  = chip_id;
    int tmp_unit  = unit_id;
    int tmp_time = ntohs(*time_info);
    int tmp_data[n_bit];

    //tmp_unit = unit_id_mapping( tmp_unit ); // unit-ID correction // unit-ID correction // removed for ch-map correction@20161004

    for( int ibyte=0; ibyte<4; ibyte++ ){
      unsigned char byte_data = mydata[8*idata+adjust+8+ibyte];
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

      tmp_data[7+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x80)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[6+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x40)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[5+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x20)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[4+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x10)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[3+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x08)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[2+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x04)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[1+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x02)) ); // bit-flip correction // modified for ch-map correction @20161004
      tmp_data[0+(3-ibyte)*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x01)) ); // bit-flip correction // modified for ch-map correction @20161004
      
      tmp_chip = 0; // temporal setting for single chip test.
      for( int i=0; i<8; i++ ){
	if( tmp_data[i+(3-ibyte)*8] ){ // modified for ch-map correction @20161004
	  t_chip_v.push_back(tmp_chip);
	  t_unit_v.push_back(tmp_unit);
	  t_bit_v.push_back (i+(3-ibyte)*8); // modified for ch-map correction @20161004
	  t_time_v.push_back(tmp_time);
	}
      }
    } // End of ibyte

    if( idata < 3) adjust += 4;
  } // End of idata
  
  if( fl_message   ) printf( "=>[ Event#=%d : #Data=%d+3 ]\n", t_event, ndata );
  if( ndata!=32768 ){
    printf( "=>[ Event#=%d : #Data=%d+3 ] * lost data event : not saved int tree\n", t_event, ndata );
    return 1; // fail
  }else{
    m_tree->Fill();
    m_tree->GetEntry(m_tree->GetEntries()-1);
    return 0; // success
  }
}
