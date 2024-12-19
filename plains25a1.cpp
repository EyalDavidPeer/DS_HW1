// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"
#include "HashSet.h"


Plains::Plains() {}

Plains::~Plains() {

}

//TODO: Update logic for other Exceptions in all Methods
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


    std::weak_ptr <Horse> horse = horses.getValue(horseId);
    if (horse.lock()->getHerdId() != -1) {
        return StatusType::FAILURE;
    }
//    if (herdId == 512080) {
//        auto herd = fullHerds.getValue(herdId);
//        if (herd) {
//            herd->getHorses().printInOrder();
//            printf("The number of horses is: %d\n", herd->getNumberOfHorses());
//        }
//    }


    // Remove from emptyHerds and ensure the herd exists in fullHerds
    emptyHerds.remove(herdId);
    std::shared_ptr <Herd> herd = fullHerds.getValue(herdId);


    if (!herd) {
        herd = std::make_shared<Herd>(herdId);
        if (!herd) {
            return StatusType::ALLOCATION_ERROR;
        }
        fullHerds.insert(herd, herdId);
    }
    herd->insertHorse(horse);
    horse.lock()->setHerdId(herdId);


    // Update other necessary state, e.g., horse's herd membership if required
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
        // Ensure formerLeader and followingHorse are valid

        formerLeader->removeFollower(followingHorse);
    }

    // Set the new leader and update follower list

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
//    if (horse->getHerdId() == 512080 && horseId == 478719) {
//        herd->getHorses().printInOrder();
//        printf("The number of horses is: %d\n", herd->getNumberOfHorses());
//        printf("His followers are:\n");
//        horse->getFollowers().printInOrder();
//        if (horse->getLeadingHorse().lock()) {
//            printf("His leader is: %d\n",
//                   horse->getLeadingHorse().lock()->getId());
//            printf("His leaders are:\n");
//            horse->getLeadingHorse().lock()->getFollowers().printInOrder();
//        }


//    }
    herd->removeHorse(horse);
//    horses.remove(horseId);
    std::shared_ptr <Horse> newhorse = std::make_shared<Horse>(horseId,
                                                               horse->getSpeed());
    auto horseNode = horses.getNode(horseId);
    if (horseNode) {
        horseNode->data.reset();
        horseNode->data = newhorse;
    }
    if (!newhorse) {
        return StatusType::ALLOCATION_ERROR;
    }
//    horses.insert(newhorse, horseId);
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
    return output_t<int>(horse->getSpeed());
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId) {
        return StatusType::INVALID_INPUT;
    }
    HashSet visited;
    auto horse = horses.getValue(horseId);
    auto destHorse = horses.getValue(otherHorseId);

    if (!horse || !destHorse) { return StatusType::FAILURE; }
    if (horse->getHerdId() !=
        destHorse->getHerdId()) {
        return false;
    }
    while (horse) {
        if (visited.exists(horse->getId())) {
            return false;
        }
        if (horse->getId() == otherHorseId) {
            return true;
        }
        visited.insert(horse->getId());
        horse = horse->getLeadingHorse().lock();
    }
    return false;
}

//TODO: replace HaseSet with Hist
//TODO: fixing the Iterator in edge cases
output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto herd = fullHerds.getValue(herdId);
    if (!herd) {
        return StatusType::FAILURE;
    }

    if (herd->getNumberOfHorses() == 1) {
        return true; // A single horse can always run together
    }
    int maxHorses = herd->getNumberOfHorses(); // Use a regular int variable
    HashSet visited;
    HashSet visited_firstCycle;
    int horseCount = 0;
    std::shared_ptr <Horse> rootLeader = nullptr;
    for (auto it = herd->getHorses().begin();
         it != herd->getHorses().end(); ++it) {
        horseCount++;

        auto horse = it.operator*().lock();
        if (horseCount > maxHorses) {
            break;
        }
        if (visited_firstCycle.exists(horse->getId())) {
            break;
        }


        visited_firstCycle.insert(horse->getId());
        auto leadingHorse = horse->getLeadingHorse().lock();

        if (!leadingHorse) {
            if (rootLeader) {
                return false; // More than one horse has no leader
            }

            rootLeader = horse;
        }
    }


    if (!rootLeader) {
        return false;
    }

    if (maxHorses == 2) {
        return true;
    }
    visited.insert(rootLeader->getId());
    int cycle = -1;
    for (auto it = herd->getHorses().begin();
         it != herd->getHorses().end(); ++it) {

        if (visited.getSize() == maxHorses) {
            return true;
        }
        auto horse = it.operator*().lock();
        if (!horse) {
            continue; // Skip if `horse` is null
        }
        if (horse->getId() == rootLeader->getId() || visited.exists(
                horse->getId())) {

        } else if (!visited.exists(
                horse->getId()) &&
                   visited.exists(horse->getLeadingHorse().lock()->getId())) {
            visited.insert(horse->getId());
        } else {
            auto tempHorse = it.operator*().lock();
            while (tempHorse->getId() !=
                   rootLeader->getId()) {
                if (!visited.exists(tempHorse->getId())) {
                    visited.insert(tempHorse->getId());
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

