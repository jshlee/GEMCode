#include "GEMCode/GEMValidation/src/TFCand.h"

TFCand::TFCand(const L1MuRegionalCand* t)
{
  l1Cand_ = t;
}

TFCand::TFCand(const TFCand& rhs)
{}

TFCand::~TFCand()
{}

void
TFCand::init(CSCTFPtLUT* ptLUT,
	     edm::ESHandle< L1MuTriggerScales > &muScales,
	     edm::ESHandle< L1MuTriggerPtScale > &muPtScale)
{
  pt_ = muPtScale->getPtScale()->getLowEdge(l1Cand_->pt_packed()) + 1.e-6;
  eta_ = muScales->getRegionalEtaScale(2)->getCenter(l1Cand_->eta_packed());
  phi_ = normalizedPhi(muScales->getPhiScale()->getLowEdge(l1Cand_->phi_packed()));

  /*
    nTFStubs = -1;
// B   
//     //Pt needs some more workaround since it is not in the unpacked data
//     //  PtAddress gives an handle on other parameters
//     ptadd thePtAddress(t->ptLUTAddress());
//     ptdat thePtData  = ptLUT->Pt(thePtAddress);
//     // front or rear bit? 
//     unsigned trPtBit = (thePtData.rear_rank&0x1f);
//     if (thePtAddress.track_fr) trPtBit = (thePtData.front_rank&0x1f);
//     // convert the Pt in human readable values (GeV/c)
//     pt  = muPtScale->getPtScale()->getLowEdge(trPtBit); 
//     
    bool sc_debug = 1;
    if (sc_debug){
        double my_phi = normalizedPhi( t->phi_packed()*0.043633231299858237 + 0.0218 ); // M_PI*2.5/180 = 0.0436332312998582370
        double sign_eta = ( (t->eta_packed() & 0x20) == 0) ? 1.:-1;
        double my_eta = sign_eta*(0.05 * (t->eta_packed() & 0x1F) + 0.925); //  0.9+0.025 = 0.925
        double my_pt = ptscale[t->pt_packed()];
        if (fabs(pt - my_pt)>0.005) std::cout<<"tfcand scales pt diff: my "<<my_pt<<" sc "<<pt<<std::endl;
        if (fabs(eta - my_eta)>0.005) std::cout<<"tfcand scales eta diff: my "<<my_eta<<" sc "<<eta<<std::endl;
        if (fabs(deltaPhi(phi,my_phi))>0.03) std::cout<<"tfcand scales phi diff: my "<<my_phi<<" sc "<<phi<<std::endl;
    }  
*/
}

void 
TFCand::setDR(double dr)
{
  dr_ = dr;
}
