// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

Plains::Plains() : m_leadsCycle(0) {}

Plains::~Plains() {

}

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (emptyHerds.search(herdId) || fullHerds.search(herdId)) {
        return StatusType::FAILURE;
    }
    auto herd = std::make_shared<Herd>(herdId);
    emptyHerds.insert(herd, herdId);

    return StatusType::SUCCESS;
}


StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!emptyHerds.search(herdId)) {
        return StatusType::FAILURE;
    }
    emptyHerds.remove(herdId);
    return StatusType::SUCCESS;

}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (horses.search(horseId)) {
        return StatusType::FAILURE;
    }
    auto horse = std::make_shared<Horse>(horseId, speed);
    if (!horse) {
        return StatusType::ALLOCATION_ERROR;
    }

    horses.insert(horse, horseId);
    return StatusType::SUCCESS;

}

StatusType Plains::join_herd(int horseId, int herdId) {

    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!horses.search(horseId)) {
        return StatusType::FAILURE;
    }

    if (!emptyHerds.search(herdId) && !fullHerds.search(herdId)) {
        return StatusType::FAILURE;
    }

    std::weak_ptr<Horse> horse = horses.getValue(horseId);
    if (horse.lock()->getHerdId() != -1) {
        return StatusType::FAILURE;
    }
    emptyHerds.remove(herdId);
    std::shared_ptr<Herd> herd = fullHerds.getValue(herdId);


    if (!herd) {
        herd = std::make_shared<Herd>(herdId);
        if (!herd) {
            return StatusType::ALLOCATION_ERROR;
        }
        fullHerds.insert(herd, herdId);
    }
    herd->insertHorse(horse);
    horse.lock()->setHerdId(herdId);

    return StatusType::SUCCESS;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }

    auto followingHorse = horses.getValue(horseId);
    auto leaderHorse = horses.getValue(horseToFollowId);
    if (!followingHorse || !leaderHorse || !followingHorse->isInHerd() ||
        followingHorse->getHerdId() != leaderHorse->getHerdId()) {
        return StatusType::FAILURE;
    }

    auto formerLeader = followingHorse->getLeadingHorse().lock();
    followingHorse->setLeaderHorse(leaderHorse);
    if (leaderHorse) {
        leaderHorse->addFollower(followingHorse);
    }
    if (formerLeader) {
        formerLeader->removeFollower(followingHorse);
    }

    return StatusType::SUCCESS;
}

StatusType Plains::leave_herd(int horseId) {

    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto horse = horses.getValue(horseId);
    if (!horse || !horse->isInHerd()) {
        return StatusType::FAILURE;
    }

    auto herd = fullHerds.getValue(horse->getHerdId());

    herd->removeHorse(horse);
    std::shared_ptr<Horse> newhorse = std::make_shared<Horse>(horseId,
                                                              horse->getSpeed());
    if (!newhorse) {
        return StatusType::ALLOCATION_ERROR;
    }
    auto horseNode = horses.getNode(horseId);
    if (horseNode) {
        horseNode->data.reset();
        horseNode->data = newhorse;
    }

    if (herd->isEmpty()) {
        emptyHerds.insert(herd, herd->getId());
        fullHerds.remove(herd->getId());
    }

    return StatusType::SUCCESS;
}

output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto horse = horses.getValue(horseId);
    if (!horse) {
        return StatusType::FAILURE;
    }
    return horse->getSpeed();
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId) {
        return StatusType::INVALID_INPUT;
    }
    auto horse = horses.getValue(horseId);
    auto destHorse = horses.getValue(otherHorseId);

    if (!horse || !destHorse) { return StatusType::FAILURE; }
    if (horse->getHerdId() !=
        destHorse->getHerdId()) {
        return false;
    }
    m_leadsCycle++;
    while (horse) {
        if (horse->getFollowCycle() == m_leadsCycle) {
            return false;
        }
        if (horse->getId() == otherHorseId) {
            return true;
        }
        horse->setFollowCycle(m_leadsCycle);
        horse = horse->getLeadingHorse().lock();
    }
    return false;
}

output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto herd = fullHerds.getValue(herdId);
    if (!herd) {
        return StatusType::FAILURE;
    }
    int maxHorses = herd->getNumberOfHorses();
    if (maxHorses == 1) {
        return true;
    }

    Histogram visited(maxHorses);
    Histogram visited_firstCycle(maxHorses);

    int horseCount = 0;
    int i = 0;
    std::shared_ptr<Horse> rootLeader = nullptr;
    for (auto it = herd->getHorses().begin();
         it != herd->getHorses().end(); ++it) {
        auto horse = it.operator*();
        horse->setHerdPosition(i);
        visited[i] = 0;
        visited_firstCycle[i] = 0;
        i++;
    }

    for (auto it = herd->getHorses().begin();
         it != herd->getHorses().end(); ++it) {
        horseCount++;

        auto horse = it.operator*();
        if (horseCount > maxHorses) {
            break;
        }
        if (visited_firstCycle[horse->getHerdPosition()] > 0) {
            break;
        }
        visited_firstCycle[horse->getHerdPosition()]++;
        auto leadingHorse = horse->getLeadingHorse().lock();
        if (!leadingHorse) {
            if (rootLeader) {
                return false;
            }
            rootLeader = horse;
        }
    }

    if (!rootLeader) {

        return false;
    }

    visited[rootLeader->getHerdPosition()]++;
    int cycle = -1;
    for (auto it = herd->getHorses().begin();
         it != herd->getHorses().end(); ++it) {

        auto horse = it.operator*();
        if (!horse) {
            continue;
        }
        if (horse->getId() == rootLeader->getId() ||
            visited[horse->getHerdPosition()] > 0) {

        } else if (!(visited[horse->getHerdPosition()] > 0) &&
                   visited[horse->getLeadingHorse().lock()->getHerdPosition()] >
                   0) {
            visited[horse->getHerdPosition()]++;
        } else {
            auto tempHorse = it.operator*();
            while (tempHorse->getId() !=
                   rootLeader->getId()) {
                if (!(visited[tempHorse->getHerdPosition()] > 0)) {
                    visited[tempHorse->getHerdPosition()]++;
                }
                tempHorse = tempHorse->getLeadingHorse().lock();
                if (cycle > maxHorses * 2) {
                    return false;
                }
                cycle++;
            }
        }

    }
    return true;
}

