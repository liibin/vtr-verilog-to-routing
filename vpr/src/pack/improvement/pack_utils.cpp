//
// Created by elgammal on 2022-07-27.
//

#include "pack_utils.h"
#include "re_cluster.h"
#include "re_cluster_util.h"
#include "globals.h"
#include "clustered_netlist_fwd.h"
#include "move_utils.h"
#include "cluster_placement.h"
#include "packing_move_generator.h"
#include "pack_move_utils.h"
#include "string.h"


void iteratively_improve_packing(const t_packer_opts& packer_opts, t_clustering_data& clustering_data, int verbosity) {
    /*
    auto& cluster_ctx = g_vpr_ctx.clustering();
    auto& atom_ctx = g_vpr_ctx.atom();
    */
    int proposed = 0;
    int succeeded = 0;
    std::unique_ptr<packingMoveGenerator> move_generator;
    if(strcmp(packer_opts.pack_move_type.c_str(), "random") == 0 )
        move_generator = std::make_unique<randomPackingMove>();
    else if(strcmp(packer_opts.pack_move_type.c_str(), "semiDirected") == 0)
        move_generator = std::make_unique<quasiDirectedPackingMove>();
    else{
        VTR_LOG("Packing move type (%s) is not correct!\n", packer_opts.pack_move_type.c_str());
        VTR_LOG("Packing iterative improvement is aborted\n");
        return;
    }

    bool is_proposed, is_valid, is_successful;
    std::vector<molMoveDescription> new_locs;

    for(int i = 0; i < packer_opts.pack_num_moves; i++) {
        new_locs.clear();
        is_proposed = move_generator->propose_move(new_locs);
        if (!is_proposed) {
            VTR_LOGV(verbosity > 2, "Move failed! Can't propose.\n");
            continue;
        }

        is_valid = move_generator->evaluate_move(new_locs);
        if (!is_valid) {
            VTR_LOGV(verbosity > 2, "Move failed! Proposed move is bad.\n");
            continue;
        }

        proposed++;
        is_successful = move_generator->apply_move(new_locs, clustering_data);

        if (!is_successful) {
            VTR_LOGV(verbosity > 2, "Move failed! Proposed move isn't legal.\n");
            continue;
        }
        succeeded++;
        VTR_LOGV(verbosity > 2, "Packing move succeeded!\n");
    }
    VTR_LOG("\n### Iterative packing stats: \n\tpack move type = %s\n\ttotal pack moves = %zu\n\tgood pack moves = %zu\n\tlegal pack moves = %zu\n\n", packer_opts.pack_move_type.c_str(), packer_opts.pack_num_moves, proposed, succeeded);
}