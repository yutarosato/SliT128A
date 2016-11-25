// -*- C++ -*-
/*!
 * @file
 * @brief
 * @date
 * @author
 *
 */

#include "SampleMonitor.h"

using DAQMW::FatalType::DATAPATH_DISCONNECTED;
using DAQMW::FatalType::INPORT_ERROR;
using DAQMW::FatalType::HEADER_DATA_MISMATCH;
using DAQMW::FatalType::FOOTER_DATA_MISMATCH;
using DAQMW::FatalType::USER_DEFINED_ERROR1;

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

int cnt_data = 0; // used for judgement of the endpoint of s-curve

int t_event;
std::vector<int> t_chip_v;
std::vector<int> t_unit_v;
std::vector<int> t_bit_v;
std::vector<int> t_time_v;

float t_vref  = -999;
float t_tpchg = -999;
int   t_selch = -999;
int   t_dac   = -999;

// Module specification
// Change following items to suit your component's spec.
static const char* samplemonitor_spec[] =
{
    "implementation_id", "SampleMonitor",
    "type_name",         "SampleMonitor",
    "description",       "SampleMonitor component",
    "version",           "1.0",
    "vendor",            "Kazuo Nakayoshi, KEK",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
};

SampleMonitor::SampleMonitor(RTC::Manager* manager)
    : DAQMW::DaqComponentBase(manager),
      m_InPort("samplemonitor_in",   m_in_data),
      m_in_status(BUF_SUCCESS),
      m_canvas(0),
      m_hist(0),
      m_bin(0),
      m_min(0),
      m_max(0),
      m_monitor_update_rate(30),
      m_event_byte_size(0),
      m_debug(false)
{
    // Registration: InPort/OutPort/Service

    // Set InPort buffers
    registerInPort ("samplemonitor_in",  m_InPort);

    init_command_port();
    init_state_table();
    set_comp_name("SAMPLEMONITOR");
}

SampleMonitor::~SampleMonitor()
{
}

RTC::ReturnCode_t SampleMonitor::onInitialize()
{
    if (m_debug) {
        std::cerr << "SampleMonitor::onInitialize()" << std::endl;
    }

    return RTC::RTC_OK;
}

RTC::ReturnCode_t SampleMonitor::onExecute(RTC::UniqueId ec_id)
{
    daq_do();

    return RTC::RTC_OK;
}

int SampleMonitor::daq_dummy()
{
    if (m_canvas) {
        m_canvas->Update();
        // daq_dummy() will be invoked again after 10 msec.
        // This sleep reduces X servers' load.
        sleep(1);
    }

    return 0;
}

int SampleMonitor::daq_configure()
{
    std::cerr << "*** SampleMonitor::configure" << std::endl;

    ::NVList* paramList;
    paramList = m_daq_service0.getCompParams();
    parse_params(paramList);

    return 0;
}

int SampleMonitor::parse_params(::NVList* list)
{

    std::cerr << "param list length:" << (*list).length() << std::endl;

    int len = (*list).length();
    for (int i = 0; i < len; i+=2) {
        std::string sname  = (std::string)(*list)[i].value;
        std::string svalue = (std::string)(*list)[i+1].value;

        std::cerr << "sname: " << sname << "  ";
        std::cerr << "value: " << svalue << std::endl;
        
        if (sname == "monitorUpdateRate") {
            if (m_debug) {
                std::cerr << "monitor update rate: " << svalue << std::endl;
            }
            char *offset;
            m_monitor_update_rate = (int)strtol(svalue.c_str(), &offset, 10);
        }
        // If you have more param in config.xml, write here
    }

    return 0;
}

int SampleMonitor::daq_unconfigure()
{
    std::cerr << "*** SampleMonitor::unconfigure" << std::endl;
    if( m_canvas ){
      delete m_canvas;
      m_canvas = 0;
    }
    
    if( m_hist ){
      delete m_hist;
      m_hist = 0;
    }
    
    if( m_tree ){
      delete m_tree;
      m_tree = 0;
    }
    return 0;
}

int SampleMonitor::daq_start()
{
    std::cerr << "*** SampleMonitor::start" << std::endl;

    m_in_status  = BUF_SUCCESS;

    //////////////// CANVAS FOR HISTOS ///////////////////
    if( m_canvas ){
      delete m_canvas;
      m_canvas = 0;
    }
    m_canvas = new TCanvas("c1", "histos", 0, 0, 600, 400);

    ////////////////       HISTOS      ///////////////////
    if( m_hist ){
      delete m_hist;
      m_hist = 0;
    }

    int    m_hist_bin = 8192;
    double m_hist_min = 0.0;
    double m_hist_max = 8192;

    gStyle->SetStatW(0.4);
    gStyle->SetStatH(0.2);
    gStyle->SetOptStat("em");

    m_hist = new TH1I("hist", "hist", m_hist_bin, m_hist_min, m_hist_max);
    m_hist->GetXaxis()->SetNdivisions(5);
    m_hist->GetYaxis()->SetNdivisions(4);
    m_hist->GetXaxis()->SetLabelSize(0.07);
    m_hist->GetYaxis()->SetLabelSize(0.06);

    set_tree();
    init_tree();
    set_readbranch_scan();

    return 0;
}

int SampleMonitor::daq_stop()
{
    std::cerr << "*** SampleMonitor::stop" << std::endl;

    m_hist->Draw();
    m_canvas->Update();

    reset_InPort();

    return 0;
}

int SampleMonitor::daq_pause()
{
    std::cerr << "*** SampleMonitor::pause" << std::endl;

    return 0;
}

int SampleMonitor::daq_resume()
{
    std::cerr << "*** SampleMonitor::resume" << std::endl;

    return 0;
}

int SampleMonitor::reset_InPort()
{
    int ret = true;
    while(ret == true) {
        ret = m_InPort.read();
    }

    return 0;
}


int SampleMonitor::set_tree(){
  m_tree = new TTree("slit128A","slit128A");
  m_tree->Branch( "event",  &t_event,  "event/I" );
  m_tree->Branch( "chip",   &t_chip_v );
  m_tree->Branch( "unit",   &t_unit_v );
  m_tree->Branch( "bit",    &t_bit_v  );
  m_tree->Branch( "time",   &t_time_v );
  m_tree->Branch( "vref",   &t_vref,  "vref/F"  );
  m_tree->Branch( "tpchg",  &t_tpchg, "tpchg/F" );
  m_tree->Branch( "selch",  &t_selch, "selch/I" );
  m_tree->Branch( "dac",    &t_dac,   "dac/I"   );

  return 0;
}

int SampleMonitor::init_tree(){
  for( int ibit=0; ibit<n_bit; ibit++ ) t_data[ibit] = 0;
  t_chip_v.clear();
  t_unit_v.clear();
  t_bit_v.clear();
  t_time_v.clear();
  
  return 0;
}

int SampleMonitor::delete_tree(){
  delete m_tree;
  return 0;
}


int SampleMonitor::unit_id_mapping( int unit ){
  int flipped_unit = 0;
  if     ( unit==0 ) flipped_unit = 3;
  else if( unit==1 ) flipped_unit = 2;
  else if( unit==2 ) flipped_unit = 1;
  else if( unit==3 ) flipped_unit = 0;
  else std::cerr << "[ABORT] Wrong unit-ID : " << unit << std::endl;
  return flipped_unit;
}

int SampleMonitor::bit_flip( bool bit ){
  if( bit ) return false;
  else      return true;
}

Int_t SampleMonitor::set_readbranch(){
  m_tree->SetBranchAddress("event", &t_event  );
  m_tree->SetBranchAddress("chip",  &t_chip_v  );
  m_tree->SetBranchAddress("unit",  &t_unit_v  );
  m_tree->SetBranchAddress("bit",   &t_bit_v   );
  m_tree->SetBranchAddress("time",  &t_time_v  );

  return 0;
}

Int_t SampleMonitor::set_readbranch_scan(){
  set_readbranch();
  m_tree->SetBranchAddress("vref",  &t_vref  );
  m_tree->SetBranchAddress("tpchg", &t_tpchg );
  m_tree->SetBranchAddress("selch", &t_selch );
  m_tree->SetBranchAddress("dac",   &t_dac   );

  return 0;
}


int SampleMonitor::decode_data(const unsigned char* mydata, int length)
{
  unsigned short* event_number = (unsigned short*)&mydata[2];
  int ndata = (length-4)/8;
  if( fl_message > 0 ) printf( "       [ Event#=%d : #Data=%d : ", ntohs(*event_number), ndata );
  //t_event = ntohs(*event_number);

  for( int idata=0; idata<ndata; idata++ ){
    unsigned char   chip_id   = mydata[8*idata+4]; chip_id = ( chip_id & 0x7f );
    unsigned char   unit_id   = mydata[8*idata+5];
    unsigned short* time_info = (unsigned short*)&mydata[8*idata+6];
    unsigned long*  data      = (unsigned long* )&mydata[8*idata+8];
    if( idata==0 && (int)unit_id==0 ) t_event = ntohs(*event_number);
    if( idata==0 && fl_message > 0 ) printf( "Unit-ID=%d ] \n", (int)unit_id );
    if( fl_message > 1 ) printf( "%3d : (Chip-ID=%d, Unit-ID=%d) : (time=%d, data=%x)\n", idata, (int)chip_id, (int)unit_id, ntohs(*time_info), ntohl(*data) );

    t_chip  = chip_id;
    t_unit  = unit_id;
    t_time = ntohs(*time_info);

    t_unit = unit_id_mapping( t_unit ); // unit-ID correction

    for( int ibyte=0; ibyte<4; ibyte++ ){
      unsigned char byte_data = mydata[8*idata+8+ibyte];

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

      t_data[0+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x80)) ); // bit-flip correction
      t_data[1+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x40)) ); // bit-flip correction
      t_data[2+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x20)) ); // bit-flip correction
      t_data[3+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x10)) ); // bit-flip correction
      t_data[4+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x08)) ); // bit-flip correction
      t_data[5+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x04)) ); // bit-flip correction
      t_data[6+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x02)) ); // bit-flip correction
      t_data[7+ibyte*8] = bit_flip( (bool)((unsigned char)(byte_data & 0x01)) ); // bit-flip correction
      
      t_chip = 0; // temporal setting for single chip test.
      for( int i=0; i<8; i++ ){
	if( t_data[i+ibyte*8] ){
	  t_chip_v.push_back(t_chip);
	  t_unit_v.push_back(t_unit);
	  t_bit_v.push_back (i+ibyte*8);
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
	  nevt_fail++;
	}else{
	  m_tree->Fill();
	  cnt_data += t_time_v.size();
	  nevt_success++;
	}
	
	init_tree();
	for( int iunit=0; iunit<n_unit; iunit++ ) fl_unit[iunit] = 0;
      }
    }
     
  return 0;
  /* // original codes
    m_sampleData.magic      = mydata[0];
    m_sampleData.format_ver = mydata[1];
    m_sampleData.module_num = mydata[2];
    m_sampleData.reserved   = mydata[3];
    unsigned int netdata    = *(unsigned int*)&mydata[4];
    m_sampleData.data       = ntohl(netdata);

    if (m_debug) {
        std::cerr << "magic: "      << std::hex << (int)m_sampleData.magic      << std::endl;
        std::cerr << "format_ver: " << std::hex << (int)m_sampleData.format_ver << std::endl;
        std::cerr << "module_num: " << std::hex << (int)m_sampleData.module_num << std::endl;
        std::cerr << "reserved: "   << std::hex << (int)m_sampleData.reserved   << std::endl;
        std::cerr << "data: "       << std::dec << (int)m_sampleData.data       << std::endl;
    }
  */
  return 0;
}

int SampleMonitor::fill_data(const unsigned char* mydata, const int size)
{
  m_tree->GetEntry(m_tree->GetEntries()-1);
  for( Int_t ivec=0; ivec<(int)t_unit_v.size(); ivec++ ){
    const Int_t obs_ch = 123; // tmppppp
    if( obs_ch != ch_map(t_unit_v.at(ivec),t_bit_v.at(ivec)) ) continue; // observed channel
    m_hist->Fill( t_time_v.at(ivec) );
  }
  /* // original code
  for (int i = 0; i < size/(int)ONE_EVENT_SIZE; i++) {
    decode_data(mydata, size);
    float fdata = m_sampleData.data/1000.0; // 1000 times value is received
    m_hist->Fill(fdata);
    
    mydata+=ONE_EVENT_SIZE;
  }
  */
  return 0;
}

unsigned int SampleMonitor::read_InPort()
{
    /////////////// read data from InPort Buffer ///////////////
    unsigned int recv_byte_size = 0;
    bool ret = m_InPort.read();

    //////////////////// check read status /////////////////////
    if (ret == false) { // false: TIMEOUT or FATAL
        m_in_status = check_inPort_status(m_InPort);
        if (m_in_status == BUF_TIMEOUT) { // Buffer empty.
            if (check_trans_lock()) {     // Check if stop command has come.
                set_trans_unlock();       // Transit to CONFIGURE state.
            }
        }
        else if (m_in_status == BUF_FATAL) { // Fatal error
            fatal_error_report(INPORT_ERROR);
        }
    }
    else {
        recv_byte_size = m_in_data.data.length();
    }

    if (m_debug) {
        std::cerr << "m_in_data.data.length():" << recv_byte_size
                  << std::endl;
    }

    return recv_byte_size;
}

int SampleMonitor::daq_run()
{
    if (m_debug) {
        std::cerr << "*** SampleMonitor::run" << std::endl;
    }

    unsigned int recv_byte_size = read_InPort();
    if (recv_byte_size == 0) { // Timeout
        return 0;
    }

    check_header_footer(m_in_data, recv_byte_size); // check header and footer
    m_event_byte_size = get_event_size(recv_byte_size);
    if (m_event_byte_size > DATA_BUF_SIZE) {
        fatal_error_report(USER_DEFINED_ERROR1, "DATA BUF TOO SHORT");
    }

    /////////////  Write component main logic here. /////////////
    memcpy(&m_recv_data[0], &m_in_data.data[HEADER_BYTE_SIZE], m_event_byte_size);

    fill_data(&m_recv_data[0], m_event_byte_size);

    if (m_monitor_update_rate == 0) {
        m_monitor_update_rate = 1000;
    }

    unsigned long sequence_num = get_sequence_num();
    if ((sequence_num % m_monitor_update_rate) == 0) {
        m_hist->Draw();
        m_canvas->Update();
    }
    /////////////////////////////////////////////////////////////
    inc_sequence_num();                      // increase sequence num.
    inc_total_data_size(m_event_byte_size);  // increase total data byte size

    return 0;
}

extern "C"
{
    void SampleMonitorInit(RTC::Manager* manager)
    {
        RTC::Properties profile(samplemonitor_spec);
        manager->registerFactory(profile,
                    RTC::Create<SampleMonitor>,
                    RTC::Delete<SampleMonitor>);
    }
};

int SampleMonitor::ch_map( int unit, int bit ){
  return n_bit*unit + bit;
}
