// -*- C++ -*-
/*!
 * @file 
 * @brief
 * @date
 * @author
 *
 */

#ifndef SAMPLEMONITOR_H
#define SAMPLEMONITOR_H

#include "DaqComponentBase.h"

#include <arpa/inet.h> // for ntohl()

////////// ROOT Include files //////////
#include "TH1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TApplication.h"

#include "SampleData.h"

#include <vector>

using namespace RTC;

class SampleMonitor
  : public DAQMW::DaqComponentBase
{
public:
  SampleMonitor(RTC::Manager* manager);
  ~SampleMonitor();
  
  // The initialize action (on CREATED->ALIVE transition)
  // former rtc_init_entry()
  virtual RTC::ReturnCode_t onInitialize();
  
  // The execution action that is invoked periodically
  // former rtc_active_do()
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);
  
private:
  TimedOctetSeq          m_in_data;
  InPort<TimedOctetSeq>  m_InPort;
  
private:
  int daq_dummy();
  int daq_configure();
  int daq_unconfigure();
  int daq_start();
  int daq_run();
  int daq_stop();
  int daq_pause();
  int daq_resume();
  
  int parse_params(::NVList* list);
  int reset_InPort();
  
  unsigned int read_InPort();
  //int online_analyze();

  int set_tree();
  int init_tree();
  int delete_tree();
  int unit_id_mapping( int unit );
  int bit_flip( bool bit );
  int set_readbranch     ();
  int set_readbranch_scan();
  int decode_data(const unsigned char* mydata, int length);
  int fill_data(const unsigned char* mydata, const int size);
  int ch_map( int unit, int bit );
  
  BufferStatus m_in_status;
  
  ////////// ROOT Histogram //////////
  TCanvas *m_canvas;
  TH1I    *m_hist;
  int      m_bin;
  double   m_min;
  double   m_max;
  int      m_monitor_update_rate;
  //const static unsigned int DATA_BUF_SIZE = 1024*1024;
  const static unsigned int DATA_BUF_SIZE = (8192*8+4)*4+1024; // 262160 + 1024; 1024 is spare
  unsigned char m_recv_data[DATA_BUF_SIZE];
  unsigned int  m_event_byte_size;
  struct sampleData m_sampleData;

  TTree* m_tree;
  
  bool m_debug;


  const static int fl_message = 1; // 0(simple message), 1(normal message), 2(detailed message)
  const static int n_chip =     1; // temporal setting
  const static int n_unit =     4;
  const static int n_bit  =    32;
  const static int n_time =  8192; // pow(2,13)

  int nevt_success;
  int nevt_fail;
  
  int cnt_data; // used for judgement of the endpoint of s-curve
  
  int t_event;
  std::vector<int> t_chip_v; // for write
  std::vector<int> t_unit_v;
  std::vector<int> t_bit_v;
  std::vector<int> t_time_v;

  std::vector<int>* t2_chip_v; // for read
  std::vector<int>* t2_unit_v;
  std::vector<int>* t2_bit_v;
  std::vector<int>* t2_time_v;

  int t_chip;
  int t_unit;
  int t_time;
  int t_data[n_bit];
  
};


extern "C"
{
  void SampleMonitorInit(RTC::Manager* manager);
};

#endif // SAMPLEMONITOR_H
