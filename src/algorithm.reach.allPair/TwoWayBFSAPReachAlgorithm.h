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
#ifndef ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H
#define ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"

class TwoWayBFSAPReachAlgorithm : public DynamicAPReachAlgorithm{
public:
    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;

    void setStepSize(unsigned long long newStep){
        stepSize = newStep;
    }

private:
public:
    void run() override;

    std::string getName() const noexcept override;

    std::string getShortName() const noexcept override;

private:
    unsigned long long stepSize = 1ULL;

};



#endif //ALLPAIRREACH_TWOWAYBFSAPREACHALGORITHM_H
