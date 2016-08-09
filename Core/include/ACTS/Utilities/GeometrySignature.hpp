// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// GeometrySignature.h, ACTS project
///////////////////////////////////////////////////////////////////

#ifndef ACTS_GEOMETRYUTILS_GEOMETRYSIGNATURE_H
#define ACTS_GEOMETRYUTILS_GEOMETRYSIGNATURE_H 1

namespace Acts {

/** @class GeometrySignature

   An enumeration object that puts the signature
   of a GeometryBuilder to all subvolumes

  @TODO will be in the future be replace by GeometryID mechanism


  */

enum GeometrySignature {
  Global             = 0,
  ID                 = 1,
  BeamPipe           = 2,
  Calo               = 3,
  MS                 = 4,
  Cavern             = 5,
  NumberOfSignatures = 6,
  Unsigned           = 99
};

enum GeometryType {
  Static                = 0,
  Dense                 = 1,
  DenseWithLayers       = 1,
  Detached              = 2,
  Master                = 3,
  NumberOfGeometryTypes = 3
};

}  // end of namespace

#endif  // ACTS_GEOMETRYUTILS_GEOMETRYSIGNATURE_H