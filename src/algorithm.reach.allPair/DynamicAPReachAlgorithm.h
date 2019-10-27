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

#ifndef ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
#define ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H


#include <algorithm.reach/dynamicssreachalgorithm.h>
#include <algorithm/dynamicdigraphalgorithm.h>

class DynamicAPReachAlgorithm : public Algora::DynamicDiGraphAlgorithm{
public:

    explicit DynamicAPReachAlgorithm() : DynamicDiGraphAlgorithm(){ }
    ~DynamicAPReachAlgorithm() override = default;

    virtual bool query(Algora::Vertex *start, const Algora::Vertex *end) = 0;

    virtual std::string getBaseName() = 0;

};


#endif //ALLPAIRREACH_DYNAMICAPREACHALGORITHM_H
