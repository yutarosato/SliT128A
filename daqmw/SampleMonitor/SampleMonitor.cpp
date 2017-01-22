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
      m_hist_1ch_1evt  (0),
      m_hist_1ch_int   (0),
      m_hist_allch_1evt(0),
      m_hist_allch_int (0),
      m_graph_nbit     (0),
      m_graph_nhit     (0),
      m_monitor_update_rate(100),
      m_obs_ch(0),
      m_event_byte_size(0),
      m_nevt_success(0),
      m_nevt_fail(0),
      m_debug(false)
{

  // Registration: InPort/OutPort/Service
  
  // Set InPort buffers
  registerInPort ("samplemonitor_in",  m_InPort);
  
  init_command_port();
  init_state_table();
  set_comp_name("SAMPLEMONITOR");

  m_tree = new MTree();
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
    
    if( sname == "monitorUpdateRate" ){
      if( m_debug ){
	std::cerr << "monitor update rate: " << svalue << std::endl;
      }
      char* offset;
      m_monitor_update_rate = (int)strtol(svalue.c_str(), &offset, 10);
    }
    // If you have more param in config.xml, write here
    if( sname == "sel1ch" ){
      if( m_debug ){
	std::cerr << "monitor update rate: " << svalue << std::endl;
      }
      char *offset;
      m_obs_ch = (int)strtol(svalue.c_str(), &offset, 10);
    }
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
  
  if( m_hist_1ch_1evt ){
    delete m_hist_1ch_1evt;
    m_hist_1ch_1evt = 0;
  }

  if( m_hist_1ch_int ){
    delete m_hist_1ch_int;
    m_hist_1ch_int = 0;
  }

  if( m_hist_allch_1evt ){
    delete m_hist_allch_1evt;
    m_hist_allch_1evt = 0;
  }

  if( m_hist_allch_int ){
    delete m_hist_allch_int;
    m_hist_allch_int = 0;
  }

  if( m_graph_nbit ){
    delete m_graph_nbit;
    m_graph_nbit = 0;
  }

  if( m_graph_nhit ){
    delete m_graph_nhit;
    m_graph_nhit = 0;
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
  m_canvas = new TCanvas("c1", "histos", 0, 0, 1200, 1000);
  m_canvas->Divide(2,3);
  
  ////////////////       HISTOS      ///////////////////
  if( m_hist_1ch_1evt ){
    delete m_hist_1ch_1evt;
    m_hist_1ch_1evt = 0;
  }

  if( m_hist_1ch_int ){
    delete m_hist_1ch_int;
    m_hist_1ch_int = 0;
  }

  if( m_hist_allch_1evt ){
    delete m_hist_allch_1evt;
    m_hist_allch_1evt = 0;
  }

  if( m_hist_allch_int ){
    delete m_hist_allch_int;
    m_hist_allch_int = 0;
  }

  if( m_graph_nbit ){
    delete m_graph_nbit;
    m_graph_nbit = 0;
  }

  if( m_graph_nhit ){
    delete m_graph_nhit;
    m_graph_nhit = 0;
  }
  
  int    m_hist_xbin = 8192;
  double m_hist_xmin = 0.0;
  double m_hist_xmax = 8192.0;

  int    m_hist_ybin = n_unit*n_bit;
  double m_hist_ymin = 0.0;
  double m_hist_ymax = n_unit*n_bit;
  
  gStyle->SetStatW(0.4);
  gStyle->SetStatH(0.2);
  gStyle->SetOptStat("em");
  
  m_hist_1ch_1evt = new TH1I("hist_1ch_1evt", "hist_1ch_1evt", m_hist_xbin, m_hist_xmin, m_hist_xmax);
  m_hist_1ch_1evt->GetXaxis()->SetNdivisions(5);
  m_hist_1ch_1evt->GetYaxis()->SetNdivisions(4);
  m_hist_1ch_1evt->GetXaxis()->SetLabelSize(0.07);
  m_hist_1ch_1evt->GetYaxis()->SetLabelSize(0.06);

  m_hist_1ch_int = new TH1I("hist_1ch_int", "hist_1ch_int", m_hist_xbin, m_hist_xmin, m_hist_xmax);
  m_hist_1ch_int->GetXaxis()->SetNdivisions(5);
  m_hist_1ch_int->GetYaxis()->SetNdivisions(4);
  m_hist_1ch_int->GetXaxis()->SetLabelSize(0.07);
  m_hist_1ch_int->GetYaxis()->SetLabelSize(0.06);

  m_hist_allch_1evt = new TH2I("hist_allch_1evt", "hist_allch_1evt", m_hist_xbin, m_hist_xmin, m_hist_xmax, m_hist_ybin, m_hist_ymin, m_hist_ymax);
  m_hist_allch_1evt->GetXaxis()->SetNdivisions(5);
  m_hist_allch_1evt->GetYaxis()->SetNdivisions(4);
  m_hist_allch_1evt->GetXaxis()->SetLabelSize(0.07);
  m_hist_allch_1evt->GetYaxis()->SetLabelSize(0.06);

  m_hist_allch_int = new TH2I("hist_allch_int", "hist_allch_int", m_hist_xbin, m_hist_xmin, m_hist_xmax, m_hist_ybin, m_hist_ymin, m_hist_ymax);
  m_hist_allch_int->GetXaxis()->SetNdivisions(5);
  m_hist_allch_int->GetYaxis()->SetNdivisions(4);
  m_hist_allch_int->GetXaxis()->SetLabelSize(0.07);
  m_hist_allch_int->GetYaxis()->SetLabelSize(0.06);

  m_graph_nbit = new TGraph();
  m_graph_nhit = new TGraph();
  
  m_tree->set_writebranch();
  m_tree->init_tree();
  
  return 0;
}

int SampleMonitor::daq_stop()
{
  std::cerr << "*** SampleMonitor::stop" << std::endl;

  m_canvas->cd(1); m_hist_1ch_1evt  ->Draw();
  m_canvas->cd(2); m_hist_1ch_int   ->Draw();
  m_canvas->cd(3); m_hist_allch_1evt->Draw();
  m_canvas->cd(4); m_hist_allch_int ->Draw();
  m_canvas->cd(5); m_graph_nbit     ->Draw();
  m_canvas->cd(6); m_graph_nhit     ->Draw();
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


int SampleMonitor::fill_data(const unsigned char* mydata, const int size)
{
  if( m_tree->decode_data(mydata, size) ) m_nevt_fail++;    // false
  else                                    m_nevt_success++; // success

  for( Int_t ivec=0; ivec<m_tree->getnhit(); ivec++ ){
    int obs_ch = m_tree->ch_map(m_tree->get_unit().at(ivec),m_tree->get_bit().at(ivec));
    int time   = m_tree->get_time().at(ivec);

    unsigned long sequence_num = get_sequence_num();
    if( (sequence_num % m_monitor_update_rate) == 0 ){
      m_hist_allch_1evt->Fill( obs_ch, time );
    if( m_obs_ch == obs_ch ) m_hist_1ch_1evt->Fill( time );
    }

    m_hist_allch_int ->Fill( obs_ch, time );
    if( m_obs_ch == obs_ch ) m_hist_1ch_int->Fill( time );
  }
  
  m_graph_nbit->SetPoint( m_graph_nbit->GetN(), sequence_num, m_hist_allch_int->Entries() );
  m_graph_nhit->SetPoint( m_graph_nbit->GetN(), sequence_num, m_hist_allch_int->Entries() );

  return 0;
}

int SampleMonitor::detect_signal( TH1I* hist ){
  int  bin_start        = 0;
  int  bin_end          = 0;
  int  bin_start_before = 0;
  bool fl_bin           = false;
  bool fl_start         = true;
  int  cnt_nsig         = 0;
  int  cnt_nring        = 0;

  for( int ibin=0; ibin<hist->GetNbinsX(); ibin++ ){
    if( hist->GetBinContent(ibin+1) && !fl_bin ){ // edge(up)
      bin_start = ibin;
      fl_bin = true;
    }else if( !hist->GetBinContent(ibin+1) && fl_bin){ // edge(down)
      bin_end = ibin;
      fl_bin = false;

      int width = bin_end - bin_start;
      int span  = bin_start - bin_start_before;

      double entry_eff_before = ( bin_start>th_window          ? hist->Integral( bin_start-th_window, bin_start-1           )/((double)th_window) : hist->Integral(1,         bin_start-1 )/(double)(bin_start-1       ) );
      double entry_eff_after  = ( bin_start<n_time-th_window+2 ? hist->Integral( bin_start,           bin_start+th_window-1 )/((double)th_window) : hist->Integral(bin_start, n_time      )/(double)(n_time-bin_start+1) );

      if( entry_eff_before <= th_eff_before && entry_eff_after >= th_eff_after && ( span > th_span || bin_start_before == 0) && width > th_width ){ // true signal
	//hist_width  ->Fill( width     );
	//hist_1ch_int->Fill( bin_start );
	//hist_wid_tim->Fill( bin_start, width );
	if( !fl_start ){
	  //hist_nring->Fill( cnt_nring );
	  //hist_span ->Fill( span );
	}
	bin_start_before = bin_start;
	cnt_nsig++;
	cnt_nring = 0;
	fl_start = false;
      }else{ // ringing
	cnt_nring++;
      }
    }
  }
  //hist_nring->Fill( cnt_nring );
  //hist_nsig ->Fill( cnt_nsig );

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
    if( m_monitor_update_rate == 0 ) m_monitor_update_rate = 100;

    fill_data(&m_recv_data[0], m_event_byte_size);

    unsigned long sequence_num = get_sequence_num();
    if ((sequence_num % m_monitor_update_rate) == 0) {
      m_canvas->cd(1); m_hist_1ch_1evt  ->Draw();
      m_canvas->cd(2); m_hist_1ch_int   ->Draw();
      m_canvas->cd(3); m_hist_allch_1evt->Draw();
      m_canvas->cd(4); m_hist_allch_int ->Draw();
      m_canvas->cd(5); m_graph_nbit     ->Draw();
      m_canvas->cd(6); m_graph_nhit     ->Draw();
      m_canvas->Update();
      m_hist_1ch_1evt  ->Reset(); // tmppppp
      m_hist_allch_1evt->Reset(); // tmppppp
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
