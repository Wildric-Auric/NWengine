#pragma once
#include "Geometry.h"

namespace Geo {
class ConstEarClippingTriangulator {
  public:
	Geo::EarClippingTriangulator tlgr;
	void						 Process(bool (*)(Point*, Point*));
};
} // namespace Geo
