//
// Created by Alwin Stockinger.
//

#include <iostream>
#include "AlgorithmHandler.h"

enum class MenuOptions{ reach=1, addArc=2, removeArc=3, quit=0} option;

void AlgorithmHandler::run() {
    std::cout << "All Pair Reachability Suite\n";

    option = MenuOptions ::reach;

    while(option != MenuOptions::quit){
        std::cout
                << std::endl
                << "1. Check reachable\n"
                << "2. Add arc\n"
                << "3. Remove arc\n"
                <<"0. Exit\n";

        int intOption;
        std::cin >> intOption;
        option = static_cast<MenuOptions>(intOption);

        switch (option) {
            case MenuOptions ::reach:
                reachabilityCheck();
                break;
            case MenuOptions ::addArc:
                addArc();
                break;
            case MenuOptions ::removeArc:
                removeArc();
                break;
        }
    }
}

void AlgorithmHandler::reachabilityCheck() {

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;

    for( DynamicAPReachAlgorithm* algorithm : algorithms){

        std::cout << algorithm->getName() << ": ";

        if(algorithm->query(graph->getCurrentVertexForId(startId), graph->getCurrentVertexForId(endId))){
            std::cout << "True\n";
        }
        else{
            std::cout << "False\n";
        }
    }
}

void AlgorithmHandler::addArc() {

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;


    graph->addArc(startId, endId, graph->getCurrentTime()+1);
    graph->applyNextDelta();
}

void AlgorithmHandler::removeArc(){

    unsigned long long startId = 0, endId = 0;

    std::cout << "Start Vertex: ";
    std::cin >> startId;

    std::cout << "End Vertex: ";
    std::cin >> endId;


    graph->removeArc(startId, endId, graph->getCurrentTime()+1);
    graph->applyNextDelta();
}