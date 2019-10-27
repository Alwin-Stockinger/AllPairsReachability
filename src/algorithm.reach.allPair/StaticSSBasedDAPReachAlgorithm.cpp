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

#include "StaticSSBasedDAPReachAlgorithm.h"

void StaticSSBasedDAPReachAlgorithm::run() {
    algorithm->run();
}

std::string StaticSSBasedDAPReachAlgorithm::getName() const noexcept {
    return std::__cxx11::string("Single Source based All Pair Reachability based on " + algorithm->getName());
}

std::string StaticSSBasedDAPReachAlgorithm::getShortName() const noexcept {
    return std::__cxx11::string("SS AP based on " + algorithm->getShortName());
}

void StaticSSBasedDAPReachAlgorithm::onDiGraphSet() {
    DynamicDiGraphAlgorithm::onDiGraphSet();
    if(!algorithm){
        algorithm = createSSAlgorithm();
    }
    algorithm->setGraph(diGraph);
}

void StaticSSBasedDAPReachAlgorithm::onDiGraphUnset() {
    if(algorithm) algorithm->unsetGraph();
    DynamicDiGraphAlgorithm::onDiGraphUnset();
}

StaticSSBasedDAPReachAlgorithm::StaticSSBasedDAPReachAlgorithm() : SSBasedDAPReachAlgorithm() {}

StaticSSBasedDAPReachAlgorithm::~StaticSSBasedDAPReachAlgorithm() {
    delete algorithm;
}

bool StaticSSBasedDAPReachAlgorithm::query(Algora::Vertex *start, const Algora::Vertex *end) {
    if(start == end){
        return true;
    }
    else if(diGraph->isIsolated(end)){
        return false;
    }
    algorithm->setSource(start);
    return algorithm->query(end);
}

std::string StaticSSBasedDAPReachAlgorithm::getBaseName() {
    return algorithm->getShortName();
}
