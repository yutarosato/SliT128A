#include "setting.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Int_t main( Int_t argc, Char_t** argv ){
  gROOT->SetBatch(!true); // tmpppppp
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

  if( !(app.Argc()==4) )
    std::cerr << "Wrong input" << std::endl
	      << "Usage : " << app.Argv(0) << " (char*)infilename (double)xmin(0fc) (double)xmax(9fc)" << std::endl
	      << std::endl, abort();
  Char_t*  infilename = app.Argv(1);
  Double_t xmin       = atof(app.Argv(2));
  Double_t xmax       = atof(app.Argv(3));
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TGraphErrors* g_gain = new TGraphErrors( infilename, "%lg %lg %lg" );
  g_gain->Sort();
  g_gain->SetMarkerSize(0.7);
  g_gain->SetMarkerColor(1);
  g_gain->SetLineColor  (1);
  g_gain->SetMinimum(0.0);
  std::cout << "[Gain]"   << std::endl; g_gain->Print();

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TCanvas* can1 = new TCanvas( "can1","can1", 600, 800 );
  can1->Draw();

  TPad* padU = new TPad( "padU", "padU", 0.0,0.0,1.0,1.0 );
  padU->SetBottomMargin( 0.40 );
  padU->SetFillColor(0);
  padU->SetFillStyle(0);
  padU->Draw();
  padU->cd(0);

  g_gain->SetTitle( "Linearity;;Pulse Height [mV]" );
  g_gain->GetXaxis()->SetLabelSize(0.0);
  g_gain->Draw("AP");
  //g_gain->Fit("pol1");
  g_gain->Fit("pol1","R","",xmin,xmax);
  TF1* func = g_gain->GetFunction("pol1");
  func->SetLineStyle(1);
  func->SetLineWidth(2);
  func->Print();

  TF1* func_ex = new TF1("func_ex", "[1]*x+[0]", 0, 20 );
  func_ex->SetParameter(0, func->GetParameter(0) );
  func_ex->SetParameter(1, func->GetParameter(1) );
  func_ex->SetLineColor(2);
  func_ex->Draw("same");

  func->Draw("Lsame");
  g_gain->Draw("Psame");

  padU->Update();
  TPaveStats* stat = (TPaveStats*)g_gain->FindObject("stats"); // [note] to get "stats", canvas update is need in advance.
  stat->SetX1NDC(0.60);
  stat->SetX2NDC(0.85);
  stat->SetY1NDC(0.45);
  stat->SetY2NDC(0.60);
  stat->Draw();
  
  TPad* padD = new TPad( "padD", "padD", 0.0,0.0,1.0,1.0 );
  padD->SetTopMargin( 0.60 );
  padD->SetFillColor(0);
  padD->SetFillStyle(0);
  padD->Draw();
  padD->cd(0);

  TGraphErrors* g_residual = new TGraphErrors();
  g_residual->SetTitle(";TP charge [fC];Residual [%]");
  g_residual->SetMarkerSize(0.7);
  g_residual->SetMarkerColor(1);
  g_residual->SetLineColor  (2);
  for( Int_t ip=0; ip<g_gain->GetN(); ip++ ){
    g_residual->SetPoint( g_residual->GetN(),
			  g_gain->GetX()[ip],
			  100*(
			       func->Eval(g_gain->GetX()[ip]) - g_gain->GetY()[ip]
			       )/func->Eval( g_gain->GetX()[ip]) );
    g_residual->SetPointError( g_residual->GetN()-1, 0.0, 100*g_gain->GetEY()[ip]/func->Eval(g_gain->GetX()[ip]) );
  }
  g_residual->Draw("APC");
  can1->Update();
  can1->Print("pic/linearity.eps");
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  std::cout << "finish" << std::endl;
  if( !gROOT->IsBatch() ) app.Run();
  return 0;
}
