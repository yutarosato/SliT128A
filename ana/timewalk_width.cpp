#include "setting.h"

const Bool_t fl_batch = !true;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Int_t main( Int_t argc, Char_t** argv ){
  gROOT->SetBatch(fl_batch);
  TApplication app( "app", &argc, argv );
  TStyle* sty = Style();
  sty->SetPadLeftMargin  (0.12);
  sty->SetLabelSize(0.03,"xyz");
  sty->SetTitleOffset(1.4,"y");
  sty->SetTitleSize(0.04,"xyz");
  sty->SetMarkerColor(2);
  sty->SetLineColor(2);
  sty->SetOptStat(1111);
  sty->SetOptFit(1);
  sty->SetMarkerSize(0.4);

  if( !(app.Argc()==3) )
    std::cerr << "Wrong input" << std::endl
	      << "Usage : " << app.Argv(0) << " (char*)infilename_timewalk (char*)infilename_width" << std::endl
	      << std::endl, abort();
  Char_t* infilename_timewalk = app.Argv(1);
  Char_t* infilename_width    = app.Argv(2);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TGraphErrors* g_timewalk = new TGraphErrors( infilename_timewalk, "%lg %*lg %lg %lg" );
  g_timewalk->SetTitle("Time-walk;DAC [bit]; Time-walk [ns]");
  g_timewalk->Sort();
  g_timewalk->SetMarkerSize(0.7);
  g_timewalk->SetMarkerColor(1);
  g_timewalk->SetLineColor  (1);
  g_timewalk->SetMinimum(0.0);

  TGraphErrors* g_width = new TGraphErrors( infilename_width, "%lg %*lg %lg %lg" );
  g_width->SetTitle("Signal Width;DAC [bit]; Signal Width [ns]");
  g_width->Sort();
  g_width->SetMarkerSize(0.7);
  g_width->SetMarkerColor(1);
  g_width->SetLineColor  (1);
  g_width->SetMinimum(0.0);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TCanvas* can1 = new TCanvas( "can1","can1", 1200, 500 );
  can1->Divide(2,1);
  can1->Draw();

  can1->cd(1);
  g_timewalk->Draw("APC");

  can1->cd(2);
  g_width->Draw("APC");

  can1->Update();

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  std::cout << "finish" << std::endl;
  if( !gROOT->IsBatch() ) app.Run();
  return 0;
}
