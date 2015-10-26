/**
 * \file mctopo_bg_Sig.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class mctopo_bg_Sig
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef LARLITE_MCTOPO_BG_H
#define LARLITE_MCTOPO_BG_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcpart.h"

#include "TH1D.h"

#include <string>


namespace larlite {
  /**
     \class mctopo_bg_Sig
     User custom analysis class made by SHELL_USER_NAME
   */
  class mctopo_bg : public ana_base{
  
  public:

    /// Default constructor
    mctopo_bg(){ _name="mctopo_bg"; _fout=0;_bothccnc=false; _ccnc=1; _sig = true;  _topo=2; _fid=0.0; _showerenergy=0; _showerdetenergy=0; _showercontain=0;}

    /// Default destructor
    virtual ~mctopo_bg(){}

    /** IMPLEMENT in mctopo_bg_Sig.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in mctopo_bg_Sig.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in mctopo_bg_Sig.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

//--------
//Setters for various types of options
//--------
	// set for both ccnc or just a specific type 
	// if not set for both nc will be assumed
    void SetBothCCNC(bool bothccnc) { _bothccnc = bothccnc ; }
        // CC = 0 
        // NC = 1
    void SetCCNC(int ccnc) { _ccnc = ccnc ; }
	// if we are looking for signal
    void SetSignal(bool sig) { _sig = sig ; }
        // change this to string or something as time goes on... or keep a list
        // 0 = inclusive ==> any >1pi0 that is a final state particle (aka Jonathan's Definition)
        // 1 = inclusive ==> any  1 pi0 that is a final state particle
        // 2 = exclusive no cmeson ==> any  1pi0 that is a final state particle and has no final state pi+/-
    void SetTopology(int Topo) { _topo = Topo ; }
	// Set a fiducial cut volume
    void SetFiducial(double fid) { _fid = fid ; }
	// Set the MC Truth shower energy threshold
    void SetShowerEnergy(double shenergy) { _showerenergy = shenergy ; }
	// Set the Deposit shower energy threshold
    void SetDetShowerEnergy(double dshenergy) { _showerdetenergy = dshenergy ; }
	// Set the shower containment 
    void SetShowerContainment(double shcontain) { _showercontain = shcontain ; }
//--------
//--------
//--------


  private: 
        TH1D* _showerdep;
        TH1D* _trackdep;
        TH1D* _totalEdep;


  protected:

        int _PassedEvt = 0;
        int _nEvt = 0;	
        int _nEvtinvol = 0;	
        int _Lost_Shower = 0;	
        int _Lost_Dalitz = 0;	

	bool _bothccnc;
	int _ccnc;
	bool _sig;
	int _topo;
	double _fid;
	double _showerenergy;
	double _showerdetenergy;
	double _showercontain;
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
