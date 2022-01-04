#include "Testing/missing.hpp"

namespace Testing {

Default::Default() {}
Default::Default(const Default&) {}
Default::Default(Default&&) {}
Default& Default::operator=(const Default&) {
	return *this;
}
Default& Default::operator=(Default&&) {
	return *this;
}
Default::~Default() {}

Nothing::~Nothing() {}

DefaultOnly::DefaultOnly() {}
DefaultOnly::~DefaultOnly() {}

DefaultCopyOnly::DefaultCopyOnly() {}
DefaultCopyOnly::DefaultCopyOnly(const DefaultCopyOnly&) {}
DefaultCopyOnly::~DefaultCopyOnly() {}

DCM_ONLY::DCM_ONLY() {}
DCM_ONLY::DCM_ONLY(DCM_ONLY&&) {}
DCM_ONLY::DCM_ONLY(const DCM_ONLY&) {}
DCM_ONLY::~DCM_ONLY() {}

}
