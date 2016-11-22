#include "setting.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Int_t main( Int_t argc, Char_t** argv ){
  gROOT->SetBatch(!true); // tmpppppp
  TApplication app( "app", &argc, argv );
  TStyle* sty = Style();
  sty->SetPadLeftMargin  (0.12);
  sty->SetTitleOffset(0.9,"y");
  sty->SetMarkerColor(2);
  sty->SetLineColor(2);
  sty->SetOptStat(1111);
  sty->SetOptFit(1);
  sty->SetMarkerSize(0.4);

  if( !(app.Argc()==2) )
    std::cerr << "Wrong input" << std::endl
	      << "Usage : " << app.Argv(0) << " (char*)infilename" << std::endl
	      << std::endl, abort();
  Char_t* infilename = app.Argv(1);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TGraphErrors* g_offset = new TGraphErrors( infilename, "%lg %lg  %lg"                    );
  TGraphErrors* g_noise  = new TGraphErrors( infilename, "%lg %*lg %*lg %lg  %lg"          );
  TGraphErrors* g_gain   = new TGraphErrors( infilename, "%lg %*lg %*lg %*lg %*lg %lg %lg" );
  g_offset->Sort();
  g_noise ->Sort();
  g_gain  ->Sort();
  g_noise->SetMinimum(0.0);
  g_gain ->SetMinimum(0.0);

  //std::cout << "[Offset]" << std::endl; g_offset->Print();
  //std::cout << "[Noise]"  << std::endl; g_noise ->Print();
  //std::cout << "[Gain]"   << std::endl; g_gain  ->Print();

  TH1D* h_offset = conv_graphtohist( g_offset, "hist_offset", "Offset; Offset voltage [mV]" );
  TH1D* h_noise  = conv_graphtohist( g_noise,  "hist_noise",  "Noise; Noise [mV]"           );
  TH1D* h_gain   = conv_graphtohist( g_gain,   "hist_gain",   "Gain; Gain [mV/1MIP]"        );
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TCanvas* can1 = new TCanvas( "can1","can1", 1500, 800 );
  can1->Divide( 3,2 );
  can1->Draw();

  can1->cd(1);
  g_offset->SetTitle( "Offset voltage;Channel;Offset voltage [mV];" );
  g_offset->Draw("AP");
  can1->cd(2);
  g_noise->SetTitle( "Noise;Channel;Noise [mV];" );
  g_noise->Draw("AP");
  can1->cd(3);
  g_gain->SetTitle( "Gain;Channel;Gain [mV/1MIP];" );
  g_gain->Draw("AP");

  can1->cd(4);
  h_offset->Draw();
  h_offset->Fit("gaus","L","same");
  //h_offset->Fit("gaus","LR","same",-300, 0); // tmpppp
  can1->cd(5);
  h_noise->Draw();
  h_noise->Fit("gaus","L","same");
  //h_noise->Fit("gaus","LR","same",3,8); // tmpppp
  can1->cd(6);
  gPad->DrawFrame( 0.0, 0.0, h_gain->GetBinCenter(h_gain->GetNbinsX()) + h_gain->GetBinWidth(h_gain->GetNbinsX()), 1.2*h_gain->GetMaximum(),h_gain->GetTitle() );
  h_gain->Draw();
  h_gain->Fit("gaus","L","same");
  //h_gain->Fit("gaus","LR","same",100,260); // tmpppp

  can1->Update();

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  TCanvas* can2 = new TCanvas( "can2","can2", 500, 800 );
  can2->Divide( 1,2 );
  can2->Draw();
  TGraphErrors* g_sn = div_graph( g_gain, g_noise );
  g_sn->Sort();
  g_sn->SetMinimum(0.0);

  TH1D* h_sn  = conv_graphtohist( g_sn, "hist_sn","S/N; S/N" );

  can2->cd(1);
  g_sn->SetTitle("S/N;Channel;S/N;");
  g_sn->Draw("AP");

  can2->cd(2);
  h_sn->Draw();
  h_sn->Fit("gaus","L","same");
  //h_sn->Fit("gaus","LR","same",20,50); // tmpppp
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  std::cout << "finish" << std::endl;
  if( !gROOT->IsBatch() ) app.Run();
  return 0;
}
