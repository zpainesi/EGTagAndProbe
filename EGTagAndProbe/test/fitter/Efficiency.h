

double BetaInverse(double x,double p, double q);

class efficiencyMeasurement
{

    public :
        string name;
        TH1D* passHist;
        TH1D* failHist;
        TH1D* effiHist;
        bool divByTotalForEfficiency;
        double divTotal;
        efficiencyMeasurement(std::string namep)
        {
            name=namep;
            divByTotalForEfficiency=false;
            passHist=nullptr;
            failHist=nullptr;
            effiHist=nullptr;
        }

        
        efficiencyMeasurement(std::string namep,Int_t XBINS,Double_t *xEdges): passHist(nullptr),failHist(nullptr),effiHist(nullptr)
        {
            name=namep;
            divByTotalForEfficiency=false;
            init(XBINS,xEdges);
        }   

        void init(Int_t XBINS,Double_t * xEdges)
        {   
           
            freeMem();
            std::cout<<" Allocation for "<<name<<" bins  : "<<XBINS<<"  [ "<<xEdges[0]<<","<<xEdges[XBINS-1] <<" ] "<<"\n";
            passHist = new TH1D(("hist_"+name+"Pass").c_str(),("hist_"+name+"Pass").c_str(),XBINS,xEdges);
            failHist = new TH1D(("hist_"+name+"Fail").c_str(),("hist_"+name+"Fail").c_str(),XBINS,xEdges);
            effiHist = new TH1D(("hist_"+name+"Efficiency").c_str(),("hist_"+name+"Efficiency").c_str(),XBINS,xEdges);
            
            effiHist->SetDirectory(0); 
            passHist->SetDirectory(0); 
            failHist->SetDirectory(0); 
            
            effiHist->SetLineColor(kBlue);
            effiHist->SetMarkerColor(kBlue);
            effiHist->SetMarkerStyle(22);
        }
        void fill(bool rslt,Double_t x)
        {
            if (rslt == 0 ) failHist->Fill(x);
            if (rslt == 1 ) passHist->Fill(x);

        }
        void Write()
        {
            passHist->Write();
            failHist->Write();
            effiHist->Write();
        }
        
        efficiencyMeasurement * Clone(string namep)
        {

            auto cl= (efficiencyMeasurement *) new efficiencyMeasurement(namep);
            cl->passHist= (TH1D*) passHist->Clone();
            cl->failHist= (TH1D*) failHist->Clone();
            cl->effiHist= (TH1D*) effiHist->Clone();
            
            cl->passHist->SetName( ("hist_"+namep+"Pass" ).c_str() ) ;
            cl->failHist->SetName( ("hist_"+namep+"Fail" ).c_str() ) ;
            cl->effiHist->SetName( ("hist_"+namep+"Efficiency" ).c_str() ) ;
            
            return cl;
        }
        
        void sumForMinCut()
        {
            double pval= 0;
            double fval= 0;
    	    
            for (int k=(passHist->GetNbinsX()+1);k>=0; k--)
    		{
                
                pval += passHist->GetBinContent(k);
                fval += failHist->GetBinContent(k);
                
                passHist->SetBinContent(k,pval);
                failHist->SetBinContent(k,fval);
            }

            divTotal=pval+fval;

        }
        
        void sumForMaxCut()
        {
           double pval= 0;
           double fval= 0;
    	    
            for (int k=0 ; k<= (passHist->GetNbinsX()+1); k++)
    		{
                
                pval+=passHist->GetBinContent(k);
                fval+=failHist->GetBinContent(k);
                
                passHist->SetBinContent(k,pval);
                failHist->SetBinContent(k,fval);
            }

            divTotal=pval+fval;

        }

        void  freeMem()
        {
   //        std::cout<<" De-Allocation for "<<name<<"\n";
          if(passHist) delete passHist;
          if(failHist) delete failHist;
          if(effiHist) delete effiHist;

        }

        ~efficiencyMeasurement()
        {
            freeMem();
        }

        void calculateEfficiency()
        {
    		effiHist->SetBinErrorOption(TH1::kPoisson);

            double a  = 0.3173;
	        double den,num;
            double N2;
    		double N1;
    		double p ;
    		double eU;
          	double eL;
          	double aeff;
          	double n,w;
            
            auto tot= passHist->GetEntries() + failHist->GetEntries();
    	    for (int k=1; k<=(passHist->GetNbinsX()); ++k)
    		{
                num = passHist->GetBinContent(k);
                if(divByTotalForEfficiency)
                  den = divTotal;
                else 
                  den = passHist->GetBinContent(k) + failHist->GetBinContent(k);
    
    		    if(den!=0){
    		    	effiHist->SetBinContent(k,num/den);
    		    }
    		    else{
    	 	    	effiHist->SetBinContent(k,0.);
    		    }
    		    
                N2 = num; N1 = den;
    		    p  = 0;  eU = 0; 	    eL = 0;
          	    aeff = (1-a)/2;
          	    n,w;
          	    	if (N1 > 0)
                	{
                  		p = N2/N1;
                  		n = N1+N2;
                  		w = N2/n;
                		if (N1*p>100 || N1*(1-p)>100){ //cout<< " pass " <<endl;
    		    		eU = sqrt(p*(1-p)/N1);
    		    		eL = sqrt(p*(1-p)/N1);
                        	}
                      		else
                        	{   //cout<<" pass2 "<<endl;
                          		eU = (1-BetaInverse(aeff,N1-N2,N2+1))-p;
                          		eL = p-(1-BetaInverse(1-aeff,N1-N2+1,N2));
                        	}
                	}
    		    effiHist->SetBinError(k,eL);
	        }

        }

};

double BetaInverse(double x,double p, double q)
{
	double result(0.0);
	double dy = 0.001;  
	double eMin = 100;
	for(int i=0;i<1000;i++){
		double y = i*dy;
	        double e = fabs(TMath::BetaIncomplete(y,p,q)-x);
      		if (e<eMin)  
        	{
         		eMin = e;
          		result = y;
        	}
    	}
  	return result;
}


