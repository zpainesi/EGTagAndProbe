#include "TROOT.h"
#define BARREL_BEG -0.01
#define BARREL_END  1.305
#define ENDCAP_BEG    1.479
#define ENDCAP_END    2.500

using namespace std;

class resolutionMeasurement
{

    public :
        string name;
       
        TH2D *res_barrel_2d    ;
        TH2D *res_endcap_2d    ;
        TH2D *res_inclusive_2d ;
        TH1D *res_barrel       ;
        TH1D *scale_endcap       ;
        TH1D *scale_inclusive    ;
        TH1D *scale_barrel       ;
        TH1D *res_endcap       ;
        TH1D *res_inclusive    ;
        TH1D *var_barrel       ;
        TH1D *var_endcap       ;
        TH1D *var_inclusive    ;


        resolutionMeasurement()
        {
            res_barrel_2d       = nullptr; 
            res_endcap_2d       = nullptr; 
            res_inclusive_2d    = nullptr; 
            scale_barrel          = nullptr; 
            scale_endcap          = nullptr; 
            scale_inclusive       = nullptr; 
            res_barrel          = nullptr; 
            res_endcap          = nullptr; 
            res_inclusive       = nullptr; 
            var_barrel          = nullptr; 
            var_endcap          = nullptr; 
            var_inclusive       = nullptr; 
        }
        
        resolutionMeasurement(std::string namep,Int_t XBINS,Double_t *xEdges, Int_t dXBINS, Double_t* dXEdges)
        {
            name=namep;
            init(XBINS,xEdges,dXBINS,dXEdges);
        }   

        void init(Int_t BINS,Double_t * Edges, Int_t dXBINS, Double_t* dXEdges)
        {   
           
            freeMem();
 //           std::cout<<" Allocation for "<<name<<"\n";
            res_barrel_2d       = new TH2D((name+"2dResBarrel").c_str(),"2d res barrel",BINS,Edges,dXBINS,dXEdges);
            res_endcap_2d       = new TH2D((name+"2dResECap").c_str(),"2d res endcap",BINS,Edges,dXBINS,dXEdges);
            res_inclusive_2d    = new TH2D((name+"2dResInclusive").c_str(),"2d res inclusive",BINS,Edges,dXBINS,dXEdges);
            scale_barrel        = new TH1D((name+"ScaleBarrel").c_str(),"dependent scale  barrel",BINS,Edges);
            scale_endcap        = new TH1D((name+"ScaleECap").c_str(),"dependent scale endcap",BINS,Edges);
            scale_inclusive     = new TH1D((name+"ScaleInclusive").c_str(),"dependent res inclusive",BINS,Edges);
            res_barrel          = new TH1D((name+"ResBarrel").c_str(),"dependent res barrel",BINS,Edges);
            res_endcap          = new TH1D((name+"ResECap").c_str(),"dependent res endcap",BINS,Edges);
            res_inclusive       = new TH1D((name+"ResInclusive").c_str(),"dependent res inclusive",BINS,Edges);
            var_barrel          = new TH1D((name+"BareResBarrel").c_str(),"res barrel",dXBINS,dXEdges);
            var_endcap          = new TH1D((name+"BareResECap").c_str(),"res endcap",dXBINS,dXEdges);
            var_inclusive       = new TH1D((name+"BareResInclusive").c_str(),"res inclusive",dXBINS,dXEdges);
            
        }
        void Fill(Double_t eta,Double_t x,Double_t y)
        {
             res_inclusive_2d->Fill(x,y);
             var_inclusive->Fill(y);

            if( abs(eta) > BARREL_BEG and abs(eta) < BARREL_END ) {
            
                res_barrel_2d->Fill(x,y);
                var_barrel->Fill(y);
            }
            if( abs(eta) > ENDCAP_BEG and abs(eta) < ENDCAP_END ) 
            { 
                res_endcap_2d->Fill(x,y);
                var_endcap->Fill(y);
            }
        }
        void Write()
        {
            res_barrel_2d    ->Write(); 
            res_endcap_2d    ->Write();
            res_inclusive_2d ->Write();
            scale_barrel       ->Write();
            scale_endcap       ->Write();
            scale_inclusive    ->Write();
            res_barrel       ->Write();
            res_endcap       ->Write();
            res_inclusive    ->Write();
            var_barrel       ->Write();
            var_endcap       ->Write();
            var_inclusive    ->Write();
        }

        void  freeMem()
        {
   //        std::cout<<" De-Allocation for "<<name<<"\n";
          if( not res_barrel_2d    ) delete res_barrel_2d    ;
          if( not res_endcap_2d    ) delete res_endcap_2d    ;
          if( not res_inclusive_2d ) delete res_inclusive_2d ;
          if( not scale_barrel       ) delete scale_barrel       ;
          if( not scale_endcap       ) delete scale_endcap       ;
          if( not scale_inclusive    ) delete scale_inclusive    ;
          if( not res_barrel       ) delete res_barrel       ;
          if( not res_endcap       ) delete res_endcap       ;
          if( not res_inclusive    ) delete res_inclusive    ;
          if( not var_barrel       ) delete var_barrel       ;
          if( not var_endcap       ) delete var_endcap       ;
          if( not var_inclusive    ) delete var_inclusive    ;

        }

        ~resolutionMeasurement()
        {
            freeMem();
        }
        
        void scaleBareVarHists()
        {
            var_barrel->Scale(1.0/var_barrel->Integral());
            var_endcap->Scale(1.0/var_endcap->Integral());
            var_inclusive->Scale(1.0/var_inclusive->Integral());
        }


        void calculateResolutions()
        {
       		
        	int bin1,bin2,bin3;
        	double fwhm,max;
        	double rms,mean;
        
        	for(int i=1; i<=res_inclusive->GetNbinsX(); ++i){
        	    
                // Inclusive
                 
                    TH1D *projectionY_inclusive = res_inclusive_2d->ProjectionY("proj_inclusive",i,i);
        		    //RMS/Mean
        		    rms = projectionY_inclusive->GetRMS();
        		    mean = projectionY_inclusive->GetMean();
        		    //FWHM/max
        		    bin1 = projectionY_inclusive->FindFirstBinAbove(projectionY_inclusive->GetMaximum()/2);
        		    bin2 = projectionY_inclusive->FindLastBinAbove(projectionY_inclusive->GetMaximum()/2);
        		    fwhm = projectionY_inclusive->GetBinCenter(bin2) - projectionY_inclusive->GetBinCenter(bin1);
        		    max  = projectionY_inclusive->GetXaxis()->GetBinCenter(projectionY_inclusive->GetMaximumBin());
        
        		    res_inclusive->SetBinContent(i,fwhm / max);	
        	        res_inclusive->SetBinError(i,fwhm / (sqrt(projectionY_inclusive->Integral())));
        		    
                    scale_inclusive->SetBinContent(i,mean);	
        		    
                    //res_inclusive->SetBinContent(i,rms / mean);
        	        //res_inclusive->SetBinError(i,rms / (sqrt(projectionY_inclusive->Integral())));
        	
        
        		// Barrel
        
        		    TH1D *projectionY_barrel = res_barrel_2d->ProjectionY("proj_barrel",i,i);
        		    //RMS/Mean	
        		    rms = projectionY_barrel->GetRMS();
        		    mean = projectionY_barrel->GetMean();
        		    //FWHM/max
        		    bin1 = projectionY_barrel->FindFirstBinAbove(projectionY_barrel->GetMaximum()/2);
        		    bin2 = projectionY_barrel->FindLastBinAbove(projectionY_barrel->GetMaximum()/2);
        		    fwhm = projectionY_barrel->GetBinCenter(bin2) - projectionY_barrel->GetBinCenter(bin1);
        		    max  = projectionY_barrel->GetXaxis()->GetBinCenter(projectionY_barrel->GetMaximumBin());
        
        		    res_barrel->SetBinContent(i,fwhm / max);		
        	        res_barrel->SetBinError(i,fwhm / (sqrt(projectionY_barrel->Integral())));
                    scale_barrel->SetBinContent(i,mean);	
                    scale_barrel->SetBinError(i,mean/sqrt(projectionY_barrel->Integral()));	
        	        //res_barrel->SetBinContent(i,rms / mean);
        	        //res_barrel->SetBinError(i,rms / (sqrt(projectionY_barrel->Integral())));
        
        
        		// Endcap
        
        		    TH1D *projectionY_endcap = res_endcap_2d->ProjectionY("proj_endcap",i,i);
        		    //RMS/Mean	
        		    rms = projectionY_endcap->GetRMS();
        		    mean = projectionY_endcap->GetMean();
        		    //FWHM/max
        		    bin1 = projectionY_endcap->FindFirstBinAbove(projectionY_endcap->GetMaximum()/2);
        		    bin2 = projectionY_endcap->FindLastBinAbove(projectionY_endcap->GetMaximum()/2);
        		    fwhm = projectionY_endcap->GetBinCenter(bin2) - projectionY_endcap->GetBinCenter(bin1);
        		    max  = projectionY_endcap->GetXaxis()->GetBinCenter(projectionY_endcap->GetMaximumBin());
        
        		    res_endcap->SetBinContent(i,fwhm / max);	
        	        res_endcap->SetBinError(i,fwhm / (sqrt(projectionY_endcap->Integral())));
                    scale_endcap->SetBinContent(i,mean);	
                    scale_endcap->SetBinError(i,mean/sqrt(projectionY_endcap->Integral()));	
                    //res_endcap->SetBinContent(i,rms / mean);
        	        //res_endcap->SetBinError(i,rms / (sqrt(projectionY_endcap->Integral())));
        
        	} 
        }

};

