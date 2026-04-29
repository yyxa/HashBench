// cli/main.cpp
#include <memory>

#include "core/Registry.hpp"
#include "core/ScoreFunction.hpp"

#include "hash/StdHash.hpp"
#include "hash/CRC32.hpp"
#include "hash/MurmurHash3.hpp"
#include "hash/XXHash.hpp"
#include "hash/CityHash.hpp"
#include "hash/SHA256.hpp"

#include "dataset/CSVDataset.hpp"

#include "metric/Speed.hpp"
#include "metric/Throughput.hpp"
#include "metric/CollisionRate.hpp"
#include "metric/Entropy.hpp"
#include "metric/Memory.hpp"
#include "metric/Attack.hpp"
#include "metric/ProbeLength.hpp"
#include "metric/LoadFactor.hpp"

int main() {

    auto& reg = Registry::instance();

    // DATASETS
    reg.registerDataset([] { 
        return std::make_shared<CSVDataset>("../datasets/spam.csv", 1); 
    });

    reg.registerDataset([] { 
        return std::make_shared<CSVDataset>("../datasets/transaction_data.csv", 0); 
    });

    reg.registerDataset([] { 
        return std::make_shared<CSVDataset>("../datasets/URL_Classification.csv", 1); 
    });

    reg.registerDataset([] { 
        return std::make_shared<CSVDataset>("../datasets/words_pos.csv", 1); 
    });

    reg.registerDataset([] { 
        return std::make_shared<CSVDataset>("../datasets/users.csv", 0); 
    });


    // HASHES
    reg.registerHash([] { return std::make_shared<StdHash>(); });
    reg.registerHash([] { return std::make_shared<CRC32>(); });
    reg.registerHash([] { return std::make_shared<MurmurHash3>(); });
    reg.registerHash([] { return std::make_shared<XXHash>(); });
    reg.registerHash([] { return std::make_shared<CityHash>(); });
    reg.registerHash([] { return std::make_shared<SHA256>(); });


    // METRICS
    reg.registerMetric([] { return std::make_shared<Speed>(); });
    reg.registerMetric([] { return std::make_shared<Throughput>(); });
    reg.registerMetric([] { return std::make_shared<CollisionRate>(); });
    reg.registerMetric([] { return std::make_shared<Entropy>(); });
    reg.registerMetric([] { return std::make_shared<Memory>(); });
    reg.registerMetric([] { return std::make_shared<Attack>(); });
    reg.registerMetric([] { return std::make_shared<ProbeLength>(); });
    reg.registerMetric([] { return std::make_shared<LoadFactor>(); });


    ScoreFunction bench;
    bench.run(2048);

    return 0;
}