#include "Acts/Seeding/New_Seedmaker.hpp"
#include "Acts/Seeding/BinFinder.hpp"
#include "Acts/Seeding/SeedFilter.hpp"
#include "Acts/Seeding/InternalSeed.hpp"
#include "Acts/Seeding/InternalSpacePoint.hpp"

#include "SpacePoint.hpp"
#include "ATLASCuts.hpp"

#include <boost/type_erasure/any_cast.hpp>
 
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>


std::vector<const SpacePoint*> readFile(std::string filename){

  std::string line;
  int   layer;
  std::vector<const SpacePoint*> readSP;

  std::ifstream spFile(filename);
  if (spFile.is_open()){
    while(!spFile.eof())
    {
      std::getline(spFile, line);
      std::stringstream ss(line);
      std::string linetype;
      ss >> linetype;
      float x, y, z, r, covr, covz;
      if (linetype == "lxyz"){
        ss >> layer >> x >> y >> z >> covr >> covz;
        r = std::sqrt(x*x+y*y);
        float f22 = covr;
        float wid = covz;
        float cov = wid*wid*.08333; if(cov < f22) cov = f22;
        if(std::abs(z) > 450.) {covz = 9.*cov; covr = .06;}
        else               {covr = 9.*cov; covz = .06;}
        SpacePoint * sp = new SpacePoint{x,y,z,r,layer,covr,covz};
   //     if(r < 200.){
   //       sp->setClusterList(1,0);
   //     }
        readSP.push_back(sp);
      }
    }
  }
  return readSP;
}

int main(){
  std::vector<const SpacePoint*> spVec = readFile("sp.txt");
  std::cout << "size of read SP: " <<spVec.size() << std::endl;

  Acts::SeedmakerConfig<SpacePoint> config;
// silicon detector max
  config.rMax = 160.;
  config.deltaRMin = 5.;
  config.deltaRMax = 160.;
  config.collisionRegionMin = -250.;
  config.collisionRegionMax = 250.;
  config.zMin = -2800.;
  config.zMax = 2800.;
  config.maxSeedsPerSpM = 5;
  //2.7 eta
  config.cotThetaMax = 7.40627;
  config.sigmaScattering = 1.00000;

  config.minPt = 500.;
  config.bFieldInZ = 0.00199724;

  config.beamPos={-.5,-.5};
  config.impactMax=10.;

  auto bottomBinFinder = std::make_shared<Acts::BinFinder<SpacePoint> >(Acts::BinFinder<SpacePoint>());
  auto topBinFinder = std::make_shared<Acts::BinFinder<SpacePoint> >(Acts::BinFinder<SpacePoint>());
  Acts::SeedFilterConfig sfconf;
  Acts::ATLASCuts<SpacePoint> atlasCuts = Acts::ATLASCuts<SpacePoint>();
  config.seedFilter = std::make_unique<Acts::SeedFilter<SpacePoint> >(Acts::SeedFilter<SpacePoint>(sfconf, &atlasCuts));
  Acts::New_Seedmaker<SpacePoint> a(config);

  // covariance tool, sets covariances per spacepoint as required
  std::function<Acts::Vector2D(const SpacePoint*,float,float,float)> ct = [=]
    (const SpacePoint* sp,float zAlign,float rAlign,float sigma=1)
    -> Acts::Vector2D
    {
      return {sp->covr,sp->covz};
    };

  std::shared_ptr<Acts::SeedmakerState<SpacePoint> > state = a.initState(spVec.begin(), spVec.end(), ct, bottomBinFinder, topBinFinder);
  auto start = std::chrono::system_clock::now();
  for(Acts::SeedingStateIterator<SpacePoint> it = state->begin(); !(it == state->end()); ++it){
    a.createSeedsForSP(it, state);
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout << "time to create seeds: " << elapsed_seconds.count() << std::endl;
  std::cout << "Seeds created: "<<state->outputQueue.size() << std::endl;
  while(!(state->outputQueue.empty())){
    std::unique_ptr<const Acts::InternalSeed<SpacePoint> > seed = std::move(state->outputQueue.front());
    state->outputQueue.pop();
    std::array<const Acts::InternalSpacePoint<SpacePoint> *,3> isp = seed->sp;
    const SpacePoint* sp = isp[0]->sp();
    std::cout << " (" << sp->x() << ", " << sp->y() << ", " << sp->z() << ") ";
    sp = isp[1]->sp();
    std::cout << sp->surface << " (" << sp->x() << ", " << sp->y() << ", " << sp->z() << ") ";
    sp = isp[2]->sp();
    std::cout << sp->surface << " (" << sp->x() << ", " << sp->y() << ", " << sp->z() << ") ";
    std::cout << std::endl;
  }
  return 0;
}
