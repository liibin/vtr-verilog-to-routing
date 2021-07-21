/*
 * attraction_groups.h
 *
 *  Created on: Jun. 13, 2021
 *      Author: khalid88
 */

#ifndef VPR_SRC_PACK_ATTRACTION_GROUPS_H_
#define VPR_SRC_PACK_ATTRACTION_GROUPS_H_

#include "vtr_strong_id.h"
#include "vtr_vector.h"
#include "atom_netlist.h"
#include "globals.h"

/**
 * @file
 * @brief This file defines the AttractionInfo class, which is used to store atoms in attraction groups, which are
 *        used during the clustering process.
 *
 * Overview
 * ========
 * Attraction groups are used during the clustering process to tell the clusterer which atoms should be packed
 * in the same cluster by virtue of having the same floorplan constraints. The attraction groups may also be
 * used to cluster similar atoms together in other ways in the future.
 */

/// @brief Type tag for AttractGroupId
struct attraction_id_tag;

/// @brief A unique identifier for a partition
typedef vtr::StrongId<attraction_id_tag> AttractGroupId;

struct AttractionGroup {
    //stores all atoms in the attraction group
    std::vector<AtomBlockId> group_atoms;

    /*
     * Atoms belonging to this attraction group will receive this gain if they
     * are potential candidates to be put in a cluster with the same attraction group.
     */
    float gain = 5;

    /*
     * If the group is made up from a partition of atoms that are confined to a size one spot
     * (i.e. one x, y grid location), the clusterer will immediately put all atoms in the group
     * into the same cluster
     */
    /* TODO: Add the code in the clusterer that will do the above steps. */
    bool region_size_one = false;
};

/// @brief sentinel value for indicating that an attraction group has not been specified
constexpr AttractGroupId NO_ATTRACTION_GROUP(-1);

class AttractionInfo {
  public:
    //Constructor that fills in the attraction groups based on vpr's floorplan constraints.
    //If no constraints were specified, then no attraction groups will be created.
    AttractionInfo();

    //Setters and getters for the class
    AttractGroupId get_atom_attraction_group(const AtomBlockId atom_id);

    AttractionGroup get_attraction_group_info(const AttractGroupId group_id);

    void set_atom_attraction_group(const AtomBlockId atom_id, const AttractGroupId group_id);

    void set_attraction_group_info(AttractGroupId group_id, const AttractionGroup& group_info);

    float get_attraction_group_gain(const AttractGroupId group_id);

    void set_attraction_group_gain(const AttractGroupId group_id, const float new_gain);

    void add_attraction_group(const AttractionGroup& group_info);

    int num_attraction_groups();

  private:
    //Store each atom's attraction group assuming each atom is in at most one attraction group
    vtr::vector<AtomBlockId, AttractGroupId> atom_attraction_group;

    //Store atoms and gain value that belong to each attraction group
    vtr::vector<AttractGroupId, AttractionGroup> attraction_groups;
};

inline AttractGroupId AttractionInfo::get_atom_attraction_group(const AtomBlockId atom_id) {
    return atom_attraction_group[atom_id];
}

inline void AttractionInfo::set_atom_attraction_group(const AtomBlockId atom_id, const AttractGroupId group_id) {
    atom_attraction_group[atom_id] = group_id;
    attraction_groups[group_id].group_atoms.push_back(atom_id);
}

inline int AttractionInfo::num_attraction_groups() {
    return attraction_groups.size();
}

inline float AttractionInfo::get_attraction_group_gain(const AttractGroupId group_id) {
    return attraction_groups[group_id].gain;
}

inline void AttractionInfo::set_attraction_group_gain(const AttractGroupId group_id, const float new_gain) {
    attraction_groups[group_id].gain = new_gain;
}

#endif /* VPR_SRC_PACK_ATTRACTION_GROUPS_H_ */
