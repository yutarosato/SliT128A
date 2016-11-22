#include "setting.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Int_t main( Int_t argc, Char_t** argv ){
  gROOT->SetBatch(!true); // tmpppppp
  TApplication app( "app", &argc, argv );
  TStyle* sty = Style();
  sty->SetPadLeftMargin  (0.12);
  sty->SetTitleOffset(1.1,"y");
  sty->SetPadLeftMargin(0.15);
  sty->SetLabelSize(0.04,"xyz");
  sty->SetMarkerColor(2);
  sty->SetLineColor(2);
  sty->SetOptStat(1111);
  sty->SetOptFit(1);
  sty->SetMarkerSize(0.4);

  if( !(app.Argc()==3) )
    std::cerr << "Wrong input" << std::endl
	      << "Usage : " << app.Argv(0) << " (char*)infilename1 (char*)infilename2" << std::endl
	      << std::endl, abort();
  Char_t* infilename_p = app.Argv(1);
  Char_t* infilename_m = app.Argv(2);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TMultiGraph* mg = new TMultiGraph();
  TGraph* g_p = new TGraph( infilename_p );
  TGraph* g_m = new TGraph( infilename_m );
  TGraph* g   = new TGraph();
  mg->SetTitle(";IDAC[k#Omega];V_{threshold} [mV];");

  if( g_p->GetN() != g_m->GetN() ) std::cerr << "[ABORT] Wrong #points : " << g_p->GetN() << " : " << g_m->GetN() << std::endl;
  g_p->Sort();
  g_m->Sort();
  mg->Add( g_p );
  mg->Add( g_m );

  for( Int_t ip=0; ip<g_p->GetN(); ip++ ){
    if( fabs(g_p->GetX()[ip] - g_m->GetX()[ip]) > 0.01 ) std::cerr << "[ABORT] Wrong X value : " << g_p->GetX()[ip] << " : " << g_m->GetX()[ip] << std::endl;
    g->SetPoint( g->GetN(), g_p->GetX()[ip], g_p->GetY()[ip] - g_m->GetY()[ip] );
  }
  g->SetTitle(";IDAC[k#Omega];#Delta V_{threshold} [mV];");
  g->Sort();

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TCanvas* can1 = new TCanvas( "can1","can1", 1000, 500 );
  can1->Divide( 2,1 );
  can1->Draw();

  can1->cd(1);
  mg->Draw("APC");
  can1->cd(2);
  g->Draw("APC");

  can1->Update();

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  std::cout << "finish" << std::endl;
  if( !gROOT->IsBatch() ) app.Run();
  return 0;
}
