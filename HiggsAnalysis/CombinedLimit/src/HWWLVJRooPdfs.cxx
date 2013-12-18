/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was generated by  Zijun Xu                           * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "../interface/HWWLVJRooPdfs.h"
//#include "HWWLVJRooPdfs.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include "RooExponential.h" 
#include <math.h> 
#include "TMath.h" 

#include <algorithm>
#include <vector>
#include <string>

#include "RooPlot.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TChain.h"
#include "TString.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TIterator.h"
#include "RooHist.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooCurve.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooExtendPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
using namespace std;

Double_t Exp(Double_t x, Double_t c){
	return TMath::Exp(c*x);
}

Double_t Exp(Double_t x, Double_t x_min, Double_t x_max, Double_t c){
	Double_t integral ;
    if(c==0.){
        integral=x_max-x_min;
    }else{
        integral= ( TMath::Exp(c*x_max)-TMath::Exp(c*x_min) ) / c;
    }
	return TMath::Exp(c*x)/integral ;
}

ClassImp(RooErfExpPdf) 

RooErfExpPdf::RooErfExpPdf(const char *name, const char *title, 
			   RooAbsReal& _x,
			   RooAbsReal& _c,
			   RooAbsReal& _offset,
			   RooAbsReal& _width,
			   Int_t _onOff) :
  RooAbsPdf(name,title), 
  x("x","x",this,_x),
  c("c","c",this,_c),
  offset("offset","offset",this,_offset),
  width("width","width",this,_width),
  onOff(onOff)
{ 
  if (_onOff < 0)
    onOff = -1;
  else
    onOff = 1;
} 


RooErfExpPdf::RooErfExpPdf(const RooErfExpPdf& other, const char* name) :  
  RooAbsPdf(other,name), 
  x("x",this,other.x),
  c("c",this,other.c),
  offset("offset",this,other.offset),
  width("width",this,other.width),
  onOff(other.onOff)
{ 
} 

Double_t RooErfExpPdf::evaluate() const 
{ 
  // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
  // return TMath::Exp(c*x)*(1.+TMath::Erf((x-offset)/width))/2. ; 
  // Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
  return ErfExp(x,c,offset,width,onOff) ; 
} 

Int_t RooErfExpPdf::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /*rangeName*/) const  
{ 
	// LIST HERE OVER WHICH VARIABLES ANALYTICAL INTEGRATION IS SUPPORTED, 
	// ASSIGN A NUMERIC CODE FOR EACH SUPPORTED (SET OF) PARAMETERS 
	// THE EXAMPLE BELOW ASSIGNS CODE 1 TO INTEGRATION OVER VARIABLE X
	// YOU CAN ALSO IMPLEMENT MORE THAN ONE ANALYTICAL INTEGRAL BY REPEATING THE matchArgs 
	// EXPRESSION MULTIPLE TIMES

	if (matchArgs(allVars,analVars,x)) return 1 ; 
	return 0 ; 
} 

Double_t RooErfExpPdf::analyticalIntegral(Int_t code, const char* rangeName) const  
{ 
	// RETURN ANALYTICAL INTEGRAL DEFINED BY RETURN CODE ASSIGNED BY getAnalyticalIntegral
	// THE MEMBER FUNCTION x.min(rangeName) AND x.max(rangeName) WILL RETURN THE INTEGRATION
	// BOUNDARIES FOR EACH OBSERVABLE x

  if (code==1) { 
    Double_t minTerm(ErfExpIndefIntegral(x.min(rangeName), c, offset, width,
					 onOff));
    Double_t maxTerm(ErfExpIndefIntegral(x.max(rangeName), c, offset, width,
					 onOff));
    return (maxTerm-minTerm);
  }
  return 0 ; 
} 

Double_t RooErfExpPdf::ErfExp(Double_t x, Double_t c, Double_t offset, 
			      Double_t width, Int_t onOff) {
  return TMath::Exp(c*x)*(1.+ onOff*TMath::Erf((x-offset)/width))*0.5 ;
}

Double_t RooErfExpPdf::ErfExp(Double_t x, Double_t x_min, Double_t x_max, 
			      Double_t c, Double_t offset, Double_t width,
			      Int_t onOff) {
  Double_t minTerm(ErfExpIndefIntegral(x_min, c, offset, width, onOff));
  Double_t maxTerm(ErfExpIndefIntegral(x_max, c, offset, width, onOff));

  return ErfExp(x, c, offset, width, onOff)/(maxTerm - minTerm);
}

Double_t RooErfExpPdf::ErfExpIndefIntegral(Double_t x, Double_t c, 
					   Double_t offset,
					   Double_t width, Int_t onOff) {
  Double_t val;
  if (c==0) 
    val = ErfIndefIntegral(x, offset, width, onOff);
  else
    val = (onOff*TMath::Exp(c*c*width*width/4. + c*offset) *
	   TMath::Erf((c*width*width + 2*offset - 2*x)/width/2) +
	   TMath::Exp(c*x) * 
	   (1 - onOff*TMath::Erf((offset-x)/width)))*0.5/c;
  
  return val;
}

Double_t RooErfExpPdf::ErfIndefIntegral(Double_t x, Double_t offset,
					Double_t width, Int_t onOff) {
  static double const rootpi = TMath::Sqrt(TMath::Pi());
  return (onOff*((x-offset)*TMath::Erf((x-offset)/width) + 
		 width/rootpi*TMath::Exp(-(x-offset)*(x-offset)/width/width)) + 
	  x)*0.5;
}

ClassImp(RooAlpha)

RooAlpha::RooAlpha(){}

RooAlpha::RooAlpha(const char *name, const char *title,
		   RooAbsReal& _x,
		   RooAbsReal& _c,
		   RooAbsReal& _offset,
		   RooAbsReal& _width,
		   RooAbsReal& _ca,
		   RooAbsReal& _offseta,
		   RooAbsReal& _widtha,
           Double_t _xmin,
           Double_t _xmax
	     ) :
  RooAbsPdf(name,title),
  x("x","x",this,_x),
  c("c","c",this,_c),
  offset("offset","offset",this,_offset),
  width("width","width",this,_width),
  ca("ca","ca",this,_ca),
  offseta("offseta","offseta",this,_offseta),
  widtha("widtha","widtha",this,_widtha)
{
        xmin=_xmin;
        xmax=_xmax;
}

RooAlpha::RooAlpha(const RooAlpha& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  c("c",this,other.c),
  offset("offset",this,other.offset),
  width("width",this,other.width),
  ca("ca",this,other.ca),
  offseta("offseta",this,other.offseta),
  widtha("widtha",this,other.widtha)
{
        xmin=other.xmin;
        xmax=other.xmax;
}

double RooAlpha::evaluate() const
{
  Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
  Double_t widtha_tmp=widtha; if(widtha<1e-2){ widtha_tmp=1e-2;}
  return RooErfExpPdf::ErfExp(x,xmin,xmax,c,offset,width_tmp)/RooErfExpPdf::ErfExp(x,xmin,xmax,ca,offseta,widtha_tmp);
}



ClassImp(RooAlpha)

RooAlphaExp::RooAlphaExp(){}

RooAlphaExp::RooAlphaExp(const char *name, const char *title,
		   RooAbsReal& _x,
		   RooAbsReal& _c,
		   RooAbsReal& _ca,
           Double_t _xmin,
           Double_t _xmax
	     ) :
  RooAbsPdf(name,title),
  x("x","x",this,_x),
  c("c","c",this,_c),
  ca("ca","ca",this,_ca)
{
        xmin=_xmin;
        xmax=_xmax;
}

RooAlphaExp::RooAlphaExp(const RooAlphaExp& other, const char* name) :
  RooAbsPdf(other,name),
  x("x",this,other.x),
  c("c",this,other.c),
  ca("ca",this,other.ca)
{
        xmin=other.xmin;
        xmax=other.xmax;
}

double RooAlphaExp::evaluate() const
{
  return Exp(x,xmin,xmax,c)/Exp(x,xmin,xmax,ca);
}


///////////////////////////////////////RooBWRunPdf.cxx
ClassImp(RooBWRunPdf) 

 RooBWRunPdf::RooBWRunPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _mean,
                        RooAbsReal& _width) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   mean("mean","mean",this,_mean),
   width("width","width",this,_width)
 { 
 } 


 RooBWRunPdf::RooBWRunPdf(const RooBWRunPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   mean("mean",this,other.mean),
   width("width",this,other.width)
 { 
 } 



 Double_t RooBWRunPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return (x*x*width/mean) / ( (x*x-mean*mean)*(x*x-mean*mean) + (x*x*width/mean)*(x*x*width/mean) );
 } 

//////////////////////////////////RooErfPow2Pdf.cxx
ClassImp(RooErfPow2Pdf) 

Double_t  ErfPow2(Double_t x,Double_t c0,Double_t c1, Double_t offset, Double_t width){
        if(width<1e-2)width=1e-2;
   Double_t sqrt_s=2000.;
   return TMath::Power(x/sqrt_s ,-1*(c0+c1*TMath::Log(x/sqrt_s)) )*(1+ TMath::Erf((x-offset)/width)) /2. ; 
 }

 RooErfPow2Pdf::RooErfPow2Pdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c0,
                        RooAbsReal& _c1,
                        RooAbsReal& _offset,
                        RooAbsReal& _width) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c0("c0","c0",this,_c0),
   c1("c1","c1",this,_c1),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width)
 { 
 } 


 RooErfPow2Pdf::RooErfPow2Pdf(const RooErfPow2Pdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c0("c0",this,other.c0),
   c1("c1",this,other.c1),
   offset("offset",this,other.offset),
   width("width",this,other.width)
 { 
 } 



 Double_t RooErfPow2Pdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
   return ErfPow2(x,c0,c1,offset,width_tmp);
 } 


 /////////////////////////////////////////////////////////

 ClassImp(RooAlpha4ErfPow2Pdf) 

 RooAlpha4ErfPow2Pdf::RooAlpha4ErfPow2Pdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c0,
                        RooAbsReal& _c1,
                        RooAbsReal& _offset,
                        RooAbsReal& _width,
                        RooAbsReal& _c0a,
                        RooAbsReal& _c1a,
                        RooAbsReal& _offseta,
                        RooAbsReal& _widtha) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c0("c0","c0",this,_c0),
   c1("c1","c1",this,_c1),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width),
   c0a("c0a","c0a",this,_c0a),
   c1a("c1a","c1a",this,_c1a),
   offseta("offseta","offseta",this,_offseta),
   widtha("widtha","widtha",this,_widtha)
 { 
 } 


 RooAlpha4ErfPow2Pdf::RooAlpha4ErfPow2Pdf(const RooAlpha4ErfPow2Pdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c0("c0",this,other.c0),
   c1("c1",this,other.c1),
   offset("offset",this,other.offset),
   width("width",this,other.width),
   c0a("c0a",this,other.c0a),
   c1a("c1a",this,other.c1a),
   offseta("offseta",this,other.offseta),
   widtha("widtha",this,other.widtha)
 { 
 } 



 Double_t RooAlpha4ErfPow2Pdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
    Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
    Double_t widtha_tmp=widtha; if(widtha<1e-2){ widtha_tmp=1e-2;}
   return ErfPow2(x,c0,c1,offset,width_tmp)/ErfPow2(x,c0a,c1a,offseta,widtha_tmp);
 } 


//////////////////////////////////RooErfPowExpPdf.cxx
ClassImp(RooErfPowExpPdf) 

Double_t  ErfPowExp(Double_t x,Double_t c0,Double_t c1, Double_t offset, Double_t width){
        if(width<1e-2)width=1e-2;
   Double_t sqrt_s=2000.;
   return TMath::Power(x/sqrt_s ,-1*(c1*TMath::Log(x/sqrt_s)) )*TMath::Exp(-1*x/sqrt_s*c0)*(1+ TMath::Erf((x-offset)/width)) /2. ; 
 }

 RooErfPowExpPdf::RooErfPowExpPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c0,
                        RooAbsReal& _c1,
                        RooAbsReal& _offset,
                        RooAbsReal& _width) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c0("c0","c0",this,_c0),
   c1("c1","c1",this,_c1),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width)
 { 
 } 


 RooErfPowExpPdf::RooErfPowExpPdf(const RooErfPowExpPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c0("c0",this,other.c0),
   c1("c1",this,other.c1),
   offset("offset",this,other.offset),
   width("width",this,other.width)
 { 
 } 



 Double_t RooErfPowExpPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
   return ErfPowExp(x,c0,c1,offset,width_tmp);
 } 


 /////////////////////////////////////////////////////////

 ClassImp(RooAlpha4ErfPowExpPdf) 

 RooAlpha4ErfPowExpPdf::RooAlpha4ErfPowExpPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c0,
                        RooAbsReal& _c1,
                        RooAbsReal& _offset,
                        RooAbsReal& _width,
                        RooAbsReal& _c0a,
                        RooAbsReal& _c1a,
                        RooAbsReal& _offseta,
                        RooAbsReal& _widtha) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c0("c0","c0",this,_c0),
   c1("c1","c1",this,_c1),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width),
   c0a("c0a","c0a",this,_c0a),
   c1a("c1a","c1a",this,_c1a),
   offseta("offseta","offseta",this,_offseta),
   widtha("widtha","widtha",this,_widtha)
 { 
 } 


 RooAlpha4ErfPowExpPdf::RooAlpha4ErfPowExpPdf(const RooAlpha4ErfPowExpPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c0("c0",this,other.c0),
   c1("c1",this,other.c1),
   offset("offset",this,other.offset),
   width("width",this,other.width),
   c0a("c0a",this,other.c0a),
   c1a("c1a",this,other.c1a),
   offseta("offseta",this,other.offseta),
   widtha("widtha",this,other.widtha)
 { 
 } 



 Double_t RooAlpha4ErfPowExpPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
    Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
    Double_t widtha_tmp=widtha; if(widtha<1e-2){ widtha_tmp=1e-2;}
   return ErfPowExp(x,c0,c1,offset,width_tmp)/ErfPowExp(x,c0a,c1a,offseta,widtha_tmp);
 } 

//////////////////////////////////////////RooErfPowPdf.cxx
ClassImp(RooErfPowPdf) 

Double_t  ErfPow(Double_t x,Double_t c, Double_t offset, Double_t width){
   if(width<1e-2)width=1e-2;
   Double_t sqrt_s=2000.;
   return TMath::Power(x/sqrt_s ,c)*(1+ TMath::Erf((x-offset)/width)) /2. ; 
 }

 RooErfPowPdf::RooErfPowPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c,
                        RooAbsReal& _offset,
                        RooAbsReal& _width) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c("c","c",this,_c),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width)
 { 
 } 


 RooErfPowPdf::RooErfPowPdf(const RooErfPowPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c("c",this,other.c),
   offset("offset",this,other.offset),
   width("width",this,other.width)
 { 
 } 



 Double_t RooErfPowPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
   return ErfPow(x,c,offset,width_tmp);
 } 


 /////////////////////////////////////////////////////////

 ClassImp(RooAlpha4ErfPowPdf) 

 RooAlpha4ErfPowPdf::RooAlpha4ErfPowPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _c,
                        RooAbsReal& _offset,
                        RooAbsReal& _width,
                        RooAbsReal& _ca,
                        RooAbsReal& _offseta,
                        RooAbsReal& _widtha) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   c("c","c",this,_c),
   offset("offset","offset",this,_offset),
   width("width","width",this,_width),
   ca("ca","ca",this,_ca),
   offseta("offseta","offseta",this,_offseta),
   widtha("widtha","widtha",this,_widtha)
 { 
 } 


 RooAlpha4ErfPowPdf::RooAlpha4ErfPowPdf(const RooAlpha4ErfPowPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   c("c",this,other.c),
   offset("offset",this,other.offset),
   width("width",this,other.width),
   ca("ca",this,other.ca),
   offseta("offseta",this,other.offseta),
   widtha("widtha",this,other.widtha)
 { 
 } 



 Double_t RooAlpha4ErfPowPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t width_tmp=width; if(width<1e-2){ width_tmp=1e-2;}
   Double_t widtha_tmp=widtha; if(widtha<1e-2){ widtha_tmp=1e-2;}
   return ErfPow(x,c,offset,width_tmp)/ErfPow(x,ca,offseta,widtha_tmp);
 } 

//////////////////////////////////////////RooPow2Pdf.cxx
ClassImp(RooPow2Pdf) 

 RooPow2Pdf::RooPow2Pdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _p0,
                        RooAbsReal& _p1) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1)
 { 
 } 


 RooPow2Pdf::RooPow2Pdf(const RooPow2Pdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1)
 { 
 } 



 Double_t RooPow2Pdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t sqrt_s=2000.;
   return TMath::Power( x/sqrt_s,-1*( p0+p1*TMath::Log(x/sqrt_s) ) )  ; 
 } 

////////////////////////////RooPowPdf.cxx
ClassImp(RooPowPdf) 

 RooPowPdf::RooPowPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _p0) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   p0("p0","p0",this,_p0)
 { 
 } 


 RooPowPdf::RooPowPdf(const RooPowPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   p0("p0",this,other.p0)
 { 
 } 



 Double_t RooPowPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t sqrt_s=2000.;
   return TMath::Power( x/sqrt_s, p0 )  ; 
 } 

/////////////////////////////  RooQCDPdf.cxx
ClassImp(RooQCDPdf) 

 RooQCDPdf::RooQCDPdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _p0,
                        RooAbsReal& _p1,
                        RooAbsReal& _p2) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1),
   p2("p2","p2",this,_p2)
 { 
 } 


 RooQCDPdf::RooQCDPdf(const RooQCDPdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1),
   p2("p2",this,other.p2)
 { 
 } 



 Double_t RooQCDPdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   //Double_t sqrt_s=8000.;
   //return TMath::Power(1-x/sqrt_s ,p0)/TMath::Power(x/sqrt_s, p1+p2*TMath::Log(x/sqrt_s))  ; 

   Double_t sqrt_s=2000.;
   return TMath::Power(1-x/sqrt_s ,p0)/TMath::Power(x/sqrt_s, p1+p2*TMath::Log(x/sqrt_s))  ; 
   //return TMath::Power(x/sqrt_s ,p0)* (1+ TMath::Power(x/sqrt_s, p1) )*TMath::Power(1-x/sqrt_s ,p2) ; 
 } 

//////////////////////////////////////////RooUser1Pdf.cxx
ClassImp(RooUser1Pdf) 

 RooUser1Pdf::RooUser1Pdf(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _p0,
                        RooAbsReal& _p1
                        ) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   p0("p0","p0",this,_p0),
   p1("p1","p1",this,_p1)
 { 
 } 


 RooUser1Pdf::RooUser1Pdf(const RooUser1Pdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   p0("p0",this,other.p0),
   p1("p1",this,other.p1)
 { 
 } 



 Double_t RooUser1Pdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   Double_t sqrt_s=500.;
   return TMath::Power(1-x/sqrt_s ,p0)/TMath::Power(x/sqrt_s, p1)  ; 
   //return TMath::Power(1-x/p2 ,p0)/TMath::Power(x/p2, p1)  ; 
 } 



