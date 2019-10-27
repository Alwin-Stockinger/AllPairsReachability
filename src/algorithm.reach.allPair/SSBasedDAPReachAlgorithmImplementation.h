//
// Copyright (c) 2019 : Alwin Stockinger
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
#define ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include <algorithm.reach/staticdfsssreachalgorithm.h>
#include <algorithm.reach/staticbfsssreachalgorithm.h>
#include "DynamicSSBasedDAPReachAlgorithm.h"
#include "StaticSSBasedDAPReachAlgorithm.h"


template <class T, bool immediateInit = true>
class SSBasedDAPReachAlgorithmImplementation : public DynamicSSBasedDAPReachAlgorithm<immediateInit> {
    static_assert(std::is_base_of<Algora::DynamicSSReachAlgorithm,T>::value, "Template Parameter has to inherit from Algora::DynamicSSReachAlgorithm");

    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new T();
    }
};

template <>
class SSBasedDAPReachAlgorithmImplementation <Algora::StaticDFSSSReachAlgorithm> : public StaticSSBasedDAPReachAlgorithm{
    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new Algora::StaticDFSSSReachAlgorithm;
    }
};

template <>
class SSBasedDAPReachAlgorithmImplementation <Algora::StaticBFSSSReachAlgorithm> : public StaticSSBasedDAPReachAlgorithm{
    Algora::DynamicSSReachAlgorithm *createSSAlgorithm() override {
        return new Algora::StaticBFSSSReachAlgorithm;
    }
};


#endif //ALLPAIRREACH_SSBASEDDAPREACHALGORITHMIMPLEMENTATION_H
