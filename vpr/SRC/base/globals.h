/* 
 Global variables 

 Key global variables that are used everywhere in VPR: 
 block, and g_atom_nl

 These variables represent the user netlist in various stages of the CAD flow:
  g_atom_nl for the unclustered user-supplied netlist
  g_clbs_nlist and block for the clustered netlist post packing
 */

#ifndef GLOBALS_H
#define GLOBALS_H
#include <unordered_map>
#include <memory>

#include "vtr_matrix.h"
#include "netlist.h"
#include "atom_netlist_fwd.h"
#include "rr_node.h"
#include "tatum/TimingGraph.hpp"
#include "tatum/TimingConstraints.hpp"

/********************************************************************
 Atom Netlist Globals
 ********************************************************************/
/* Atom netlist */
extern AtomNetlist g_atom_nl;

/* Mappins to/from the Atom Netlist */
extern AtomLookup g_atom_lookup;

/* Atom net power info */
extern std::unordered_map<AtomNetId,t_net_power> g_atom_net_power;


/********************************************************************
 Timing Globals
 ********************************************************************/
extern std::shared_ptr<tatum::TimingGraph> g_timing_graph;
extern std::shared_ptr<tatum::TimingConstraints> g_timing_constraints;


struct timing_analysis_profile_info {
    double timing_analysis_wallclock_time() {
        return sta_wallclock_time + slack_wallclock_time;
    }

    double old_timing_analysis_wallclock_time() {
        return old_sta_wallclock_time + old_delay_annotation_wallclock_time;
    }

    double sta_wallclock_time = 0.;
    double slack_wallclock_time = 0.;
    size_t num_full_updates = 0;

    double old_sta_wallclock_time = 0.;
    double old_delay_annotation_wallclock_time = 0.;
    size_t num_old_sta_full_updates = 0;
};
extern timing_analysis_profile_info g_timing_analysis_profile_stats;


/********************************************************************
 CLB Netlist Globals
 ********************************************************************/
/* blocks in the user netlist */
extern int num_blocks;
extern struct s_block *block;

/* New external-to-complex block, post-packed netlist*/
extern t_netlist g_clbs_nlist;

extern std::string g_placement_id; //SHA256 digest of .place file

/********************************************************************
 Physical FPGA architecture globals 
 *********************************************************************/

/* x and y dimensions of the FPGA itself, the core of the FPGA is from [1..nx][1..ny], the I/Os form a perimeter surrounding the core */
extern int nx, ny;
extern struct s_grid_tile **grid; /* FPGA complex blocks grid [0..nx+1][0..ny+1] */

/* Special pointers to identify special blocks on an FPGA: I/Os, unused, and default */
extern t_type_ptr IO_TYPE;
extern t_type_ptr EMPTY_TYPE;
extern t_type_ptr FILL_TYPE;

/* type_descriptors are blocks that can be moved by the placer
 such as: I/Os, CLBs, memories, multipliers, etc
 Different types of physical block are contained in type descriptors
 */
extern int num_types;
extern struct s_type_descriptor *type_descriptors;

/* default output name */
extern char *default_output_name;

/* Default area of a 1x1 logic tile (excludes routing) on the FPGA */
extern float grid_logic_tile_area;

/*******************************************************************
 Routing related globals
 ********************************************************************/

/* chan_width is for x|y-directed channels; i.e. between rows */
extern t_chan_width chan_width;

/* [0..num_nets-1] of linked list start pointers.  Defines the routing.  */
extern struct s_trace **trace_head, **trace_tail;

extern std::string g_routing_id; //SHA256 digest of .route file

/* Structures to define the routing architecture of the FPGA.           */
extern int num_rr_nodes;
extern t_rr_node *rr_node; /* [0..num_rr_nodes-1]          */
extern int num_rr_indexed_data;
extern t_rr_indexed_data *rr_indexed_data; /* [0 .. num_rr_indexed_data-1] */
extern vtr::t_ivec ***rr_node_indices;
extern int **net_rr_terminals; /* [0..num_nets-1][0..num_pins-1] */
extern int g_num_arch_switches;
extern s_arch_switch_inf *g_arch_switch_inf; /* [0..(g_num_arch_switches-1)] */
extern int g_num_rr_switches;
extern s_rr_switch_inf *g_rr_switch_inf; /* autogenerated in build_rr_graph based on switch fan-in. [0..(g_num_rr_switches-1)] */
/*
 * g_switch_fanin_remap is only used for printing out switch fanin stats (the -switch_stats option)
 * array index: [0..(g_num_arch_switches-1)]; 
 * map key: num of all possible fanin of that type of switch on chip
 * map value: remapped switch index (index in g_rr_switch_inf)
 */
extern map<int, int> *g_switch_fanin_remap; 
extern int **rr_blk_source; /* [0..num_blocks-1][0..num_class-1] */

/* the head pointers of structures that are "freed" and used constantly */
/*struct s_heap *g_heap_free_head;
struct s_trace *g_trace_free_head;
struct s_linked_f_pointer *g_linked_f_pointer_free_head;*/

/*******************************************************************
 Timing related globals
 ********************************************************************/

extern float pb_max_internal_delay; /* biggest internal delay of block */
extern const t_pb_type *pbtype_max_internal_delay; /* block type with highest internal delay */

/*******************************************************************
 Clock Network
 ********************************************************************/
extern t_clock_arch * g_clock_arch;

#endif

