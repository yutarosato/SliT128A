#include "setting.h"

const Bool_t fl_batch  = true;
const Int_t  fl_th_evt = 50; // only use data more than fl_th_evt events;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TH1I* GetHistogram( Char_t* infilename, Char_t* histname ){
  TFile* rootfile = new TFile( infilename );
  if( rootfile->IsZombie() ) std::cerr << "[ABORT] can not find rootfile for " << infilename << std::endl, abort();
  TH1I* hist_1ch_int = (TH1I*)rootfile->Get( histname );
  if( hist_1ch_int==NULL ) std::cerr << "[ABORT] can not find histogram for " << infilename << " : " << histname << std::endl, abort();
  return hist_1ch_int;
}
Double_t GetVar( Char_t* infilename, Char_t* treename, Char_t* brname ){
  TFile* rootfile = new TFile( infilename );
  if( rootfile->IsZombie() ) std::cerr << "[ABORT] can not find rootfile for " << infilename << std::endl, abort();
  TTree* tree = (TTree*)rootfile->Get( treename );
  if( tree==NULL ) std::cerr << "[ABORT] can not find tree for " << infilename << std::endl, abort();
  if( !tree->GetEntries() ) std::cerr << "[ABORT] can not find entry in tree for " << infilename << std::endl, abort();
  tree->GetEntry(0);
  Double_t var = (Double_t)(tree->GetLeaf( brname )->GetValue());
  return var;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Int_t main( Int_t argc, Char_t** argv ){
  gROOT->SetBatch(fl_batch);
  TApplication app( "app", &argc, argv );
  TStyle* sty = Style();
  sty->SetPadLeftMargin(0.12);
  sty->SetPadTopMargin (0.20);
  sty->SetTitleOffset(0.9,"y");
  sty->SetOptStat(1111);
  sty->SetOptFit(1);

  if( app.Argc()<6 ) std::cerr << "Wrong input" << std::endl
			       << "Usage : " << app.Argv(0) << " (char*)brach-name (int)obs_ch (int)tp_ch (int)dac (char*)infilenames1 (char*)infilename2 ..." << std::endl
			       << std::endl, abort();
  Char_t* brname = app.Argv(1);
  Int_t   obs_ch = atoi( app.Argv(2) );
  Int_t   tp_ch  = atoi( app.Argv(3) );
  Int_t   dac    = atoi( app.Argv(4) );
  Int_t   nfile  = app.Argc() - 5;

  TLatex* tex1 = new TLatex();
  tex1->SetTextColor(2);
  //tex1->SetTextSize(0.25);
  tex1->SetTextSize(0.03*nfile);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Make Histograms>
  THStack* mh = new THStack( "mh", "Timing;Time [bit]" );
  TH1I** hist_1ch_int  = new TH1I*[nfile];
  TH1I** hist_width    = new TH1I*[nfile];
  TLegend* leg         = new TLegend( 0.12,0.80,0.75,0.95 );
  Double_t* var        = new Double_t[nfile];
  Double_t* val_width  = new Double_t[nfile];
  Double_t* val_widthE = new Double_t[nfile];

  for( Int_t ifile=0; ifile<nfile; ifile++ ){
    Char_t* infilename = app.Argv(ifile+5);
    // hist_1ch_int
    hist_1ch_int[ifile] = GetHistogram( infilename, Form("hist_1ch_int_obsch%03d_tpch%03d_dac%d",obs_ch,tp_ch,dac) );
    hist_1ch_int[ifile]->SetLineColor(ifile+1);
    hist_1ch_int[ifile]->SetTitle( infilename );
    hist_1ch_int[ifile]->SetLineWidth(2);
    mh->Add( hist_1ch_int[ifile] );
    leg->AddEntry( hist_1ch_int[ifile], infilename, "L" );
    var[ifile] = GetVar( infilename, Form("meta_obsch%03d_tpch%03d_dac%d",obs_ch,tp_ch,dac), brname );
    // hist_width
    hist_width[ifile] = GetHistogram( infilename, Form("hist_width_obsch%03d_tpch%03d_dac%d",obs_ch,tp_ch,dac) );
    val_width [ifile] = hist_width[ifile]->GetMean();
    val_widthE[ifile] = hist_width[ifile]->GetMeanError();
    //val_widthE[ifile] = hist_width[ifile]->GetRMS(); // for study of multi-comparaotr @20160802

    std::cout << std::setw(3) << std::right << ifile      << " : "
	      << std::setw(8) << std::right << var[ifile] << " : "
	      << infilename << std::endl;
  }


  TH1I* hist_total = new TH1I(*hist_1ch_int[0]); // used for peak search
  hist_total->Reset();
  hist_total->SetName ("hist_total");
  hist_total->SetTitle("hist_total");
  for( Int_t ifile=0; ifile<nfile; ifile++ ) hist_total->Add( hist_1ch_int[ifile] );

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  TGraphErrors* g_timing = new TGraphErrors();
  g_timing->SetName( "g_timing" );
  g_timing->SetTitle( Form("Timing(DAC=%d);%s;#Deltat [ns]",dac,brname) );
  g_timing->SetMarkerColor(2);
  g_timing->SetLineColor(2);
  
  TGraphErrors* g_timflc = new TGraphErrors(); // for multi-comparator
  g_timflc->SetName( "g_timflc" );
  g_timflc->SetTitle( Form("Fluctuation of Timing(DAC=%d);%s;#sigma_{#Deltat} [ns]",dac,brname) );
  g_timflc->SetMarkerColor(2);
  g_timflc->SetLineColor(2);
  g_timflc->SetMinimum(0.0);

  TGraphErrors* g_width = new TGraphErrors();
  g_width->SetName( "g_width" );
  g_width->SetTitle( Form("Width(DAC=%d);%s;Width [ns]",dac,brname) );
  g_width->SetMarkerColor(2);
  g_width->SetLineColor(2);
  
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Draw>
  TCanvas* can = new TCanvas( "can","can", 1800, 1000 );
  can->Draw();
  
  TPad* pad_up1 = new TPad("pad_up","pad_up",0.02, 0.61, 0.49, 0.98 );
  pad_up1->SetFillColor(0);
  pad_up1->Draw();

  TPad* pad_up2 = new TPad("pad_up","pad_up",0.51, 0.61, 0.74, 0.98 );
  pad_up2->SetFillColor(0);
  pad_up2->Draw();

  TPad* pad_up3 = new TPad("pad_up","pad_up",0.76, 0.61, 0.99, 0.98 );
  pad_up3->SetFillColor(0);
  pad_up3->Draw();

  const Int_t npad = 4;
  TPad*** pad_down = new TPad**[npad];
  for( Int_t ipad=0; ipad<npad; ipad++ ){
    pad_down[ipad] = new TPad*[nfile];
    for( Int_t ifile=0; ifile<nfile; ifile++ ){
      if( ifile==0 ){
	pad_down[ipad][ifile] = new TPad( Form("pad_down%d_%d",ipad,ifile),Form("pad_down%d_%d",ipad,ifile), ipad*(1.0/npad), 0.02+ifile*(0.57/(nfile+1)), (ipad+1)*(1.0/npad), 0.02+(ifile+2)*(0.57/(nfile+1)) );
	pad_down[ipad][ifile]->SetTopMargin   (0.0);
	pad_down[ipad][ifile]->SetBottomMargin(0.5);
      }else{
	//pad_down[ipad][ifile] = new TPad( Form("pad_down%d_%d",ipad,ifile),Form("pad_down%d_%d",ipad,ifile), ipad*(1.0/npad), 0.02+ifile*(0.57/(nfile+1)), (ipad+1)*(1.0/npad), 0.02+(ifile+1)*(0.57/(nfile+1)) ); // wrong ???
	pad_down[ipad][ifile] = new TPad( Form("pad_down%d_%d",ipad,ifile),Form("pad_down%d_%d",ipad,ifile), ipad*(1.0/npad), 0.02+(ifile+1)*(0.57/(nfile+1)), (ipad+1)*(1.0/npad), 0.02+(ifile+2)*(0.57/(nfile+1)) );
	pad_down[ipad][ifile]->SetTopMargin   (0.0);
	pad_down[ipad][ifile]->SetBottomMargin(0.0);
      }
      pad_down[ipad][ifile]->SetFillColor(0);
      pad_down[ipad][ifile]->Draw();
    }
  }

  pad_up1->cd();
  mh->DrawClone("nostack");
  leg->DrawClone();
  for( Int_t ifile=0; ifile<nfile; ifile++ ) hist_1ch_int[ifile]->SetLineColor(3);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Plot Timing Distribtuion>
  Int_t cnt_pad = 0;
  Double_t* val_timing   = new Double_t[nfile];
  Double_t* val_timingE2 = new Double_t[nfile];
  Double_t* val_timflc   = new Double_t[nfile];
  Double_t* val_timflcE2 = new Double_t[nfile];
  for( Int_t ifile=0; ifile<nfile; ifile++ ){
    val_timing  [ifile] = 0.0;
    val_timingE2[ifile] = 0.0;
    val_timflc  [ifile] = 0.0;
    val_timflcE2[ifile] = 0.0;
  }

  Int_t* cnt_nsig = new Int_t[nfile];
  for( Int_t ifile=0; ifile<nfile; ifile++ ) cnt_nsig[ifile] = 0;  

  for( Int_t ibin=0; ibin<hist_total->GetNbinsX(); ibin++ ){
    //if( hist_total->GetBinContent(ibin+1) > 0.3*hist_total->GetMaximum() ){
    if( hist_total->GetBinContent(ibin+1) > 0.3*hist_total->GetMaximum() && ibin>50 ){ // tmpppppp
      for( Int_t ifile=0; ifile<nfile; ifile++ ){
	pad_down[cnt_pad][ifile]->cd();
	TH1* frame = pad_down[cnt_pad][ifile]->DrawFrame( hist_total->GetBinCenter(ibin+1)-20, 0.0,
							  hist_total->GetBinCenter(ibin+1)+25, 1.1*mh->GetMaximum("nostack"), Form("Timing%d;Time [bit];",ifile) );
	frame->SetTitleSize(0.03*nfile,"XY");
	frame->SetLabelSize(0.03*nfile,"XY");
	hist_1ch_int[ifile]->Draw( "same" );
	if( hist_1ch_int[ifile]->Integral(ibin-20,ibin+25) < fl_th_evt ) continue;
	TFitResultPtr fit_result = hist_1ch_int[ifile]->Fit("gaus",(fl_batch ? "RQS" : "RS"),"",hist_total->GetBinCenter(ibin+1)-12, hist_total->GetBinCenter(ibin+1)+15 );
	tex1->DrawLatexNDC( 0.20, 0.70, hist_1ch_int[ifile]->GetTitle() );
	if( fit_result->Status()==0 ){ // success
	  hist_1ch_int[ifile]->GetFunction("gaus")->SetLineWidth(2);
	  val_timing  [ifile] +=     hist_1ch_int[ifile]->GetFunction("gaus")->GetParameter(1); // 0(constant), 1(mean), 2(sigma)
	  val_timingE2[ifile] += pow(hist_1ch_int[ifile]->GetFunction("gaus")->GetParError (1),2);
	  val_timflc  [ifile] +=     hist_1ch_int[ifile]->GetFunction("gaus")->GetParameter(2)/pow(hist_1ch_int[ifile]->GetFunction("gaus")->GetParError (2),2);
	  val_timflcE2[ifile] += 1/pow(hist_1ch_int[ifile]->GetFunction("gaus")->GetParError (2),2);
	  cnt_nsig[ifile]++;
	} else{
	  std::cerr << "[WARNING] Falsely Fitting : " << "ifile = " << ifile << ", time = " << hist_total->GetBinCenter(ibin+1) << std::endl;
	}
      }
      cnt_pad++;

      ibin += 500;
      if( cnt_pad == npad ){
	cnt_pad = 0;
	can->Update();
	//can->WaitPrimitive();
      }
    }
  }


  Int_t fl_base = 0;
  for( Int_t ifile=0; ifile<nfile; ifile++ ){
    if( cnt_nsig[ifile] ){
      fl_base = ifile;
      break;
    }
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Input Timing and Width>  
  for( Int_t ifile=0; ifile<nfile; ifile++ ){
    std::cout << ifile << " : Nsig = " << cnt_nsig[ifile] << std::endl;
    if( !cnt_nsig[ifile] ) continue;
    g_timing->SetPoint     ( g_timing->GetN(),   var[ifile], 5.0*    ( val_timing  [ifile] - val_timing  [fl_base] )/cnt_nsig[ifile] );
    g_timing->SetPointError( g_timing->GetN()-1,        0.0, 5.0*sqrt( val_timingE2[ifile] + val_timingE2[fl_base] )/cnt_nsig[ifile] );
    g_timflc->SetPoint     ( g_timflc->GetN(),   var[ifile], 5.0*    ( val_timflc  [ifile] )/val_timflcE2[ifile] );
    g_timflc->SetPointError( g_timflc->GetN()-1,        0.0, 5.0*sqrt( 1/val_timflcE2[ifile] )                   );
    g_width ->SetPoint     ( g_width ->GetN(),   var[ifile], 5.0*val_width [ifile] );
    g_width ->SetPointError( g_width ->GetN()-1,        0.0, 5.0*val_widthE[ifile] );
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Draw>
  pad_up2->cd();
  g_timing->Sort();
  g_timing->Draw("APC");
  g_timing->Print("all");

  pad_up3->cd();
  g_width->Sort();
  g_width->Print("all");
  g_width->Draw("APC");
  
  can->Update();
  can->Print( Form("pic/timing_obsch%03d_tpch%03d_dac%d.eps",obs_ch,tp_ch,dac) );
  TFile outfile( Form("pic/timing_obsch%03d_tpch%03d_dac%d.root",obs_ch,tp_ch,dac), "RECREATE" );
  g_timing->Write();
  g_width ->Write();
  outfile.Close();


  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // <Make Log>
  if( g_timing->GetN() && g_timing->GetX()[0] <= 3.84*0.5 && g_timing->GetX()[g_timing->GetN()-1] >= 3.84*3.0 ){
    std::cout << std::setw(13) << std::right << dac
	      << std::setw(13) << std::right << obs_ch
	      << std::setw(13) << std::right << (g_timing->Eval(3.84*0.5) - g_timing->Eval(3.84*3.0)); // time-walk [ns]
    Double_t error   [2] = {0.0, 0.0 };
    Bool_t   fl_error[2] = {true,true};
    for( Int_t ip=0; ip<g_timing->GetN(); ip++ ){
      if( g_timing->GetX()[ip] >= 3.84*0.5 && fl_error[0] ){
	fl_error[0] = false;
	error   [0] = g_timing->GetEY()[ip];
      }
      if( g_timing->GetX()[ip] >= 3.84*3.0 && fl_error[1] ){
	fl_error[1] = false;
	error   [1] = g_timing->GetEY()[ip];
      }
    }
    std::cout << std::setw(13) << std::right << sqrt( pow(error[0],2) + pow(error[1],2) );
    std::cout << " HOGETIMEWALK" << std::endl;
  }
  if( g_width->GetN() && g_width->GetX()[0] <= 3.84*1.0 && g_width->GetX()[g_width->GetN()-1] >= 3.84*1.0 ){
    std::cout << std::setw(13) << std::right << dac
	      << std::setw(13) << std::right << obs_ch
	      << std::setw(13) << std::right << g_width ->Eval(3.84*1.0); // signal width [ns]
    Double_t error    = 0.0;
    Bool_t   fl_error = true;
    for( Int_t ip=0; ip<g_width->GetN(); ip++ ){
      if( g_width->GetX()[ip] >= 3.84*1.0 && fl_error ){
	fl_error = false;
	error    = g_width->GetEY()[ip];
	break;
      }
    }
    std::cout << std::setw(13) << std::right << error;
    std::cout << " HOGEWIDTH"    << std::endl;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  /* // for presentation plot
  TCanvas* can1 = new TCanvas( "can1","can1", 1200, 600 );
  can1->Divide(2,1);
  can1->Draw();
  can1->cd(1);
  g_timing->Draw("APC");
  can1->cd(2);
  g_width->Draw("APC");
  */
  /* // for multi-comparaotr study
  TCanvas* can1 = new TCanvas( "can1","can1", 1800, 400 );
  can1->Draw();
  can1->Divide(2,1);
  can1->cd(1);
  g_timing->Draw("APC");
  can1->cd(2);
  g_timflc->Sort();
  g_timflc->Draw("APC");
  TFile outfile_multi_comparaotor( "pic/multi_comparator_timing.root", "RECREATE" );
  g_timing->Write();
  g_timflc->Write();
  outfile_multi_comparaotor.Close();
  */
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  std::cout << "finish" << std::endl;
  if( !gROOT->IsBatch() ) app.Run();
  return 0;
}
