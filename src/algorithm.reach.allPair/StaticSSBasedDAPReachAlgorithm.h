//
// Created by Alwin Stockinger.
//

#ifndef ALLPAIRREACH_STATICSSBASEDDAPREACHALGORITHM_H
#define ALLPAIRREACH_STATICSSBASEDDAPREACHALGORITHM_H


#include "DynamicAPReachAlgorithm.h"
#include "SSBasedDAPReachAlgorithm.h"

class StaticSSBasedDAPReachAlgorithm : public SSBasedDAPReachAlgorithm{

public:
    void run() override;

    [[nodiscard]] std::string getName() const noexcept override;

    [[nodiscard]] std::string getShortName() const noexcept override;

    void onDiGraphSet() override;

    void onDiGraphUnset() override;

    explicit StaticSSBasedDAPReachAlgorithm();

    ~StaticSSBasedDAPReachAlgorithm() override;

    bool query(Algora::Vertex *start, const Algora::Vertex *end) override;

    std::string getBaseName() override;


private:
    Algora::DynamicSSReachAlgorithm* algorithm{};
};


#endif //ALLPAIRREACH_STATICSSBASEDDAPREACHALGORITHM_H
