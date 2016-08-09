// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// DiscLayer.h, ACTS project
///////////////////////////////////////////////////////////////////

#ifndef ACTS_LAYERS_DISCLAYER_H
#define ACTS_LAYERS_DISCLAYER_H 1

class MsgStream;

#include "ACTS/Layers/Layer.hpp"
#include "ACTS/Surfaces/DiscSurface.hpp"
#include "ACTS/Utilities/Definitions.hpp"
#include <algorithm>

namespace Acts {

class DiscBounds;
class SurfaceMaterial;
class OverlapDescriptor;
class ApproachDescriptor;


/// @class DiscLayer
///
/// Class to describe a disc-like detector layer for tracking,
/// it inhertis from both, Layer base class
/// and DiscSurface class


class DiscLayer : virtual public DiscSurface, public Layer
{
public:
  ///  Factory constructor with DiscSurface components 
  ///
  /// @param transform is the transform to place the layer in the 3D frame
  /// @param dbounds are the disc bounds that describe the layer dimensions
  /// @param surfaceArray is the array of sensitive surfaces
  /// @param thickness is the layer thickness (along the normal vector)
  /// @param od is the overlap descriptor that describes sensitive surface neighbours
  /// @param ad is the approach descriptor that provides the approach surface
  /// @param laytyp is the layer type
  ///
  /// @TODO move OverlapDescriptor and ApproachDescriptor to unqique_ptr
  static LayerPtr
  create(std::shared_ptr<Transform3D>      transform,
         std::shared_ptr<const DiscBounds> dbounds,
         std::unique_ptr<SurfaceArray>     surfaceArray = nullptr,
         double                            thickness    = 0.,
         OverlapDescriptor*                od           = nullptr,
         ApproachDescriptor*               ad           = nullptr,
         LayerType                         laytyp       = Acts::passive)
  {
    return LayerPtr(new DiscLayer(transform,
                                  dbounds,
                                  std::move(surfaceArray),
                                  thickness,
                                  od,
                                  ad,
                                  laytyp));
  }

  /// Factory constructor as copy with shift 
  /// @param dla is the disc laye to be cloned
  /// @param shift is the additional transform to be applied after copying
  static LayerPtr
  create(const DiscLayer& dla, const Transform3D& shift)
  {
    return LayerPtr(new DiscLayer(dla, shift));
  }

  /// Clone with a shift - only cloning that is allowed 
  /// @param shift is the additional transform to be applied after cloning
  LayerPtr
  cloneWithShift(const Transform3D& shift) const override
  {
    return DiscLayer::create(*this, shift);
  }
  
  /// Default Constructor
  DiscLayer() = delete;

  /// Copy constructor of DiscLayer - deleted 
  DiscLayer(const DiscLayer& cla) = delete;

  /// Assignment operator for DiscLayers - deleted
  DiscLayer&
  operator=(const DiscLayer&) = delete;

  /// Destructor
  virtual ~DiscLayer() {}
  
  /// Transforms the layer into a Surface representation for extrapolation 
  /// @return This method returns a surface reference 
  const DiscSurface&
  surfaceRepresentation() const override;

private:
  /// build approach surfaces 
  void
  buildApproachDescriptor() const;

protected:
  // Constructor with DiscSurface components and pointer to SurfaceArray
  ///
  /// @param transform is the transform to place the layer in the 3D frame
  /// @param dbounds are the disc bounds that describe the layer dimensions
  /// @param surfaceArray is the array of sensitive surfaces
  /// @param thickness is the layer thickness (along the normal vector)
  /// @param od is the overlap descriptor that describes sensitive surface neighbours
  /// @param ad is the approach descriptor that provides the approach surface
  /// @param laytyp is the layer taype
  DiscLayer(std::shared_ptr<Transform3D>      transform,
            std::shared_ptr<const DiscBounds> dbounds,
            std::unique_ptr<SurfaceArray>     surfaceArray = nullptr,
            double                            thickness    = 0.,
            OverlapDescriptor*                od           = nullptr,
            ApproachDescriptor*               ad           = nullptr,
            LayerType                         laytyp       = Acts::active);

  /// Copy constructor with shift
  DiscLayer(const DiscLayer& cla, const Transform3D& tr);
};

}  // end of namespace

#endif  // ACTS_LAYERS_DISCLAYER_H