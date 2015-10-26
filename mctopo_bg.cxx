#ifndef LARLITE_MCTOPO_BG_CXX
#define LARLITE_MCTOPO_BG_CXX

#include "mctopo_bg.h"

namespace larlite {

  bool mctopo_bg::initialize() {
    _showerdep = new TH1D("Enegydeposited_showers","MeV ; MeV;",20,0,800);
    _trackdep = new TH1D("Enegydeposited_track","MeV ; MeV;",20,0,800);
    _totalEdep = new TH1D("TotalEnegydeposited_event","MeV ; MeV;",20,0,800);

    return true;
  }
  
  bool mctopo_bg::analyze(storage_manager* storage) {
	_nEvt++;
	
        // Bring in the info for the event
        auto mctruth = storage->get_data<event_mctruth>("generator");
            if(!mctruth) {
                print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, mctruth!"));
                return false;}// if no mctruth
	// Get all the particles
	// Get the Neutrino
        auto mcpart = mctruth->at(0).GetParticles();
        auto mcnu = mctruth->at(0).GetNeutrino();
        auto ev_mcshower = storage->get_data<event_mcshower>("mcreco");// Get the mcshower info
	
	auto nupart = mcnu.Nu();

	bool background = false;
	//=&==&==&==&==&==&
	// if ccnc or both 
	//=&==&==&==&==&==&


	bool nio = false;
            auto nux = nupart.Trajectory().at(0).X() ;
            auto nuy = nupart.Trajectory().at(0).Y() ;
            auto nuz = nupart.Trajectory().at(0).Z() ;
		if(nux<256.35-_fid && nux>0+_fid && nuy<116.5-_fid && nuy>-116.5+_fid && nuz<1036.8-_fid && nuz>-0+_fid){ 
			_nEvtinvol++;
			nio = true;
		}
	if(!nio) return false;
	//if( mcnu.CCNC()==0){_PassedEvt++; return true;}
	if( mcnu.CCNC()==0){_PassedEvt++; background = true;}


	//=&==&==&==&==&==&
	// What Type of Event 
	//=&==&==&==&==&==&
        int pi0    = 0;
        int cmeson      = 0;
        int ckaon      = 0;
	
        for(auto const& p : mcpart){
            auto x = p.Trajectory().at(0).X() ;
            auto y = p.Trajectory().at(0).Y() ;
            auto z = p.Trajectory().at(0).Z() ;
		// If the particle is contained then log 
		if(x<256.35-_fid && x>0+_fid && y<116.5-_fid && y>-116.5+_fid && z<1036.8-_fid && z>-0+_fid){ 
			if(p.PdgCode()==111 && p.StatusCode()==1) pi0++;
			if(abs(p.PdgCode()==211) && p.StatusCode()==1) cmeson++;
			if(abs(p.PdgCode()==321) && p.StatusCode()==1) ckaon++;
		}// If inside of the fiducial
	}// for loop over mcpart

	// if it is not signal....	
	 if(pi0!=1 || cmeson!=0||ckaon!=0){_PassedEvt++;background = true; } 
	//=&==&==&==&==&==&==&==&==&==&==&
	// If we make it this far
	// Look at the daughter products 
	//=&==&==&==&==&==&==&==&==&==&==&
	// First we need to check if it is a dalitz decay
	event_mcshower showers;
        for (auto const& s : *ev_mcshower) 
		{if(s.MotherPdgCode() == 111){
			if(abs(s.PdgCode()) == 11){ _Lost_Dalitz++; background = true;}
			if(s.Start().E() > _showerenergy && s.DetProfile().E()>_showerdetenergy && s.Start().E()/s.DetProfile().E()>_showercontain && s.MotherProcess()=="primary") showers.push_back(s);}
		}
			
	if(showers.size()!=2 && pi0==1 && cmeson==0  && ckaon==0){ _Lost_Shower++; background = true;}

//_PassedEvt++;


if(background){ 
	        auto ev_mctrack = storage->get_data<event_mctrack>("mcreco");// Get the mcshower info
        double stotaldet=0;
        for (auto const& s : *ev_mcshower){
        auto depe = s.DetProfile();
        stotaldet+=depe.E();
        }// mcshower
        double ttotaldet=0;
//      std::cout<<"Track Summary"<<std::endl;
        for (auto const& t : *ev_mctrack){
        auto start = t.Start();
        auto end = t.End();
        double depe = start.E()- end.E();
//      std::cout<<"Track PDG " << t.PdgCode()<<std::endl;
//      std::cout<<"Track SStart E " << start.E()<<std::endl;
//      std::cout<<"Track End E " << end.E()<<std::endl;
        if(end.E()!=0)ttotaldet+=depe;
        }// mcshower
//        std::cout<<"TotaldetProf Shower" <<stotaldet<<std::endl;
  //      std::cout<<"TotaldetProf Track" <<ttotaldet<<std::endl;
        _showerdep->Fill(stotaldet);
        _trackdep->Fill(ttotaldet);
        double tedep = ttotaldet+stotaldet;
        _totalEdep->Fill(tedep);

	}




    return true;



	
	// This is bad c++ but works
  }

  bool mctopo_bg::finalize() {
	

        if(_fout){
        _fout->cd();
        _showerdep->Write();
        _trackdep->Write();
        _totalEdep->Write();
        }

	


	std::cout<<"Summary of MCTOPO:\n \t Filter mode CCNC "<< _ccnc <<" Was it Both? " <<_bothccnc<<std::endl;
	std::cout<<"\n \t Signal Or Bk "<< _sig <<std::endl;
	std::cout<<"  \t PassedEvt  "<< _PassedEvt<<" Number of evt Lost to Showers"<< _Lost_Shower<<" Number for Dalitz " << _Lost_Dalitz <<" evts in volume " <<_nEvtinvol<< " out of "<<_nEvt<<std::endl;

    return true;
  }

}
#endif
