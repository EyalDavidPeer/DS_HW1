// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

Plains::Plains() : m_leadsCycle(0) {}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (emptyHerds.search(herdId) || fullHerds.search(herdId)) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Herd> herd;
    try {
        herd = std::make_shared<Herd>(herdId);
        emptyHerds.insert(herd, herdId);
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}


StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!emptyHerds.search(herdId)) {
        return StatusType::FAILURE;
    }
    try {
        emptyHerds.remove(herdId);
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;

}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (horses.search(horseId)) {
        return StatusType::FAILURE;
    }

    try {
        auto horse = std::make_shared<Horse>(horseId, speed);
        horses.insert(horse, horseId);
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }

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
    std::shared_ptr<Herd> herd = fullHerds.getValue(herdId);

    if (!herd) {
        try {
            emptyHerds.remove(herdId);
            herd = std::make_shared<Herd>(herdId);
            fullHerds.insert(herd, herdId);

        } catch (const std::bad_alloc &) {
            return StatusType::ALLOCATION_ERROR;
        }
    }

    try {
        horse.lock()->setHerdId(herdId);
        herd->insertHorse(horse);
    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }

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

    followingHorse->setLeaderHorse(leaderHorse);

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


    std::shared_ptr<Horse> newhorse;
    try {
        herd->removeHorse(horse);
        newhorse = std::make_shared<Horse>(horseId, horse->getSpeed());
    } catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
    auto horseNode = horses.getNode(horseId);
    if (horseNode) {
        horseNode->data.reset();
        horseNode->data = newhorse;
    }

    if (herd->isEmpty()) {
        try {
            emptyHerds.insert(herd, herd->getId());
            fullHerds.remove(herd->getId());
        }
        catch (const std::bad_alloc &) {
            return StatusType::ALLOCATION_ERROR;
        }
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

    try {
        Histogram visited(maxHorses);
        Histogram visitedPath(maxHorses);
        std::shared_ptr<Horse> supremeLeader = nullptr;

        int i = 0;
        // checks if there is a single supremeLeader in the herd and updating the Histograms
        for (auto it = herd->getHorses().begin();
             it != herd->getHorses().end(); ++it) {

            auto horse = it.operator*();
            if (!horse) {
                return false;
            }
            visited[i] = 0;
            visitedPath[i] = 0;
            horse->setHerdPosition(i);
            i++;
            auto leadingHorse = horse->getLeadingHorse().lock();
            if (!leadingHorse) {
                if (supremeLeader) {
                    return false;
                }
                supremeLeader = horse;
            }
        }

        if (!supremeLeader) {
            return false;
        }

        visited[supremeLeader->getHerdPosition()]++;
        int path = 0;
        for (auto it = herd->getHorses().begin();
             it != herd->getHorses().end(); ++it) {
            auto horse = it.operator*();
            if (!horse) {
                continue;
            }
            // if the current horse is the supremeLeader or already visited
            if (horse->getId() == supremeLeader->getId() ||
                visited[horse->getHerdPosition()] != 0) {
                continue;
            } else {
                auto tempHorse = it.operator*();
                path++;
                while (tempHorse->getId() !=
                       supremeLeader->getId()) {
                    // if the horse in the while loop already visited
                    if (visited[tempHorse->getHerdPosition()] != 0) {
                        // if the horse is visited in the current path - cycle detected
                        if (visitedPath[tempHorse->getHerdPosition()] == path) {
                            return false;
                        }
                            //else - this horse has a valid path to the leader(from older loop)
                        else {
                            break;
                        }
                        // else - updating the horse state to visited and saving the path of the visit
                    } else {
                        visited[tempHorse->getHerdPosition()]++;
                        visitedPath[tempHorse->getHerdPosition()] = path;
                    }
                    // moving forward to next leading horse
                    tempHorse = tempHorse->getLeadingHorse().lock();
                }
            }
        }
        return true;
    }
    catch (const std::bad_alloc &) {
        return StatusType::ALLOCATION_ERROR;
    }
}

