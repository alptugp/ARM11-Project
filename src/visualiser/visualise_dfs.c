#include "undirected_graph.h"
#include "visualise_dfs.h"
#include <string.h>
#include <stdbool.h>
#include <time.h>

static void print_location(location_t loc) {
    printf("location row, col: %d, %d\n", loc.row, loc.col);
}

static void print_arc_segment(arc_segment_t arc_segment) {
    printf("\nsource location row, col: %d, %d\n", arc_segment.start_pt.row, arc_segment.start_pt.col);
    printf("target location row, col: %d, %d\n", arc_segment.end_pt.row, arc_segment.end_pt.col);
}

void print_grid(grid_t grid) {
    for(int row = 0; row < grid.height; row++) {
        for(int col = 0; col < grid.width; col++) {
            printf("%c", grid.chars[row][col]);
        }
        printf("\n");
    }
}

// Print the list of visited nodes, in order of visiting.
static void print_traversal(undirected_graph_t graph) {
    printf("\n");
    printf("Traversal of nodes so far, in order: ");
    undirected_arc *curr = graph.visited_arcs;
    if(curr) {
        printf("%d ", curr->source.index);
        while(curr) {
            printf("%d ", curr->target.index);
            curr = curr->next_in_list;
        }
    }
    else { // Initial case: have not visited any nodes except start node
        printf("%d", graph.current_node.index);
    }
    printf("\n");
}

static void write_node_to_grid(grid_t *grid, int node_index, int row_index, int col_index) {
    assert(row_index >= 0 && row_index + NODE_CENTRE_TO_RIGHT < grid->height);
    assert(col_index >= 0 && col_index + NODE_CENTRE_TO_BTM < grid->width);
    char buf[2];
    sprintf(buf, "%d", node_index);
    grid->chars[row_index][col_index] = buf[0];
}

static void initialise_nodes(undirected_graph_t graph, grid_t *grid, location_t *node_locations) {
    bool occupied_squares[grid->height][grid->width];
    for(int row_index = 0; row_index < grid->height; row_index++) {
        for(int col_index = 0; col_index < grid->width; col_index++) {
            occupied_squares[row_index][col_index] = false;
        }
    }

    for(int node_index = 0; node_index < graph.num_nodes; node_index++) {
        int row_index, col_index;
        while(true) {
            int lower_bound_row = NODE_CENTRE_TO_BTM + MIN_NODE_GAP;
            int upper_bound_row = grid->height - 1 - lower_bound_row;
            row_index = (rand() % (upper_bound_row - lower_bound_row + 1)) + lower_bound_row;
            int lower_bound_col = NODE_CENTRE_TO_RIGHT + MIN_NODE_GAP;
            int upper_bound_col = grid->width - 1 - lower_bound_col;
            col_index = (rand() % (upper_bound_col - lower_bound_col + 1)) + lower_bound_col;
            if(!occupied_squares[row_index][col_index]) {
                break;
            }
        }

        location_t new_loc = {row_index, col_index};
        node_locations[node_index] = new_loc;
        write_node_to_grid(grid, node_index, row_index, col_index);

        // Mark all squares MIN_NODE_GAP + 2 * a node width and MIN_NODE_GAP + 2 * a node height from (row_index, col_index),
        // so that two nodes will never touch each other, even on the boundary
        //assert(0);
        for(int row_offset = -2 * NODE_CENTRE_TO_BTM - MIN_NODE_GAP; row_offset < 2 * NODE_CENTRE_TO_BTM + MIN_NODE_GAP + 1; row_offset++) {
            for(int col_offset = -2 * NODE_CENTRE_TO_RIGHT - MIN_NODE_GAP; col_offset < 2 * NODE_CENTRE_TO_RIGHT + MIN_NODE_GAP + 1; col_offset++) {
                int row_to_mark = row_index + row_offset;
                int col_to_mark = col_index + col_offset;
                if(row_to_mark >= 0 && row_to_mark < grid->height && col_to_mark >= 0 && col_to_mark < grid->width) {
                    occupied_squares[row_index + row_offset][col_index + col_offset] = true;
                }
            }
        }
    }

    for(int i = 0; i < graph.num_nodes; i++) {
        print_location(node_locations[i]);
    }
}

// Calculate arc segments to draw from given source location to given target location,
// returning the number of arc segments to draw
static int calc_arc_segs(location_t source_location, location_t target_location, arc_segment_t *arc_segments) {
    int res;
    if(source_location.col == target_location.col || source_location.row == target_location.row) {
        assert(!((source_location.row != target_location.row) 
                && (source_location.col != target_location.col)));
        arc_segments[0] = (arc_segment_t) {source_location, target_location};
        res = 1;
    }
    else {
        bool equalise_rows_first = rand() % 2;
        location_t midpt = equalise_rows_first ? (location_t) {target_location.row, source_location.col} : (location_t) {source_location.row, target_location.col};
        arc_segments[0] = (arc_segment_t) {source_location, midpt};
        arc_segments[1] = (arc_segment_t) {midpt, target_location};
        res = 2;
    }
    return res;
}

static int calc_all_arc_segs(undirected_graph_t graph, location_t *node_locations, arc_segment_t *arc_segments) {
    int num_arc_segments = 0;
    for(int source_index = 0; source_index < graph.num_nodes; source_index++) {
        for(int target_index = source_index; target_index < graph.num_nodes; target_index++) {
            if(!graph.adj_matrix[source_index][target_index]) {
                continue;
            }
            arc_segment_t new_arc_segments[MAX_SEGMENTS_PER_ARC];
            int num_new_arc_segments = calc_arc_segs(node_locations[source_index], node_locations[target_index], new_arc_segments);
            memcpy(arc_segments + num_arc_segments, new_arc_segments, num_new_arc_segments * sizeof(arc_segment_t));
            num_arc_segments += num_new_arc_segments;
        }
    }
    return num_arc_segments;
}

static void draw_arcs(undirected_graph_t graph, grid_t *grid, arc_segment_t *arc_segments, int num_arc_segments, undirected_arc *arcs_to_colour, int num_arcs_to_colour) {
    //  TODO add colours
    for(int i = 0; i < num_arc_segments; i++) {
        arc_segment_t arc_segment = arc_segments[i];
        if(arc_segment.start_pt.row == arc_segment.end_pt.row) {
            assert(arc_segment.start_pt.col != arc_segment.end_pt.col);
            int row_index = arc_segment.start_pt.row;
            bool start_less_than_end = arc_segment.start_pt.col < arc_segment.end_pt.col;
            int start_col = start_less_than_end ? arc_segment.start_pt.col : arc_segment.end_pt.col;
            int end_col = start_less_than_end ? arc_segment.end_pt.col : arc_segment.start_pt.col;
            for(int col_index = start_col + 1; col_index < end_col; col_index++) {
                if(grid->chars[row_index][col_index] == VERTICAL_ARC_CHAR) {
                    grid->chars[row_index][col_index] = HORIZONTAL_AND_VERTICAL_ARC_CHAR;
                }
                else if(grid->chars[row_index][col_index] == EMPTY) {
                    grid->chars[row_index][col_index] = HORIZONTAL_ARC_CHAR;
                }
            }
        }
        else {
            assert(arc_segment.start_pt.col = arc_segment.end_pt.col);
            int col_index = arc_segment.start_pt.col;
            bool start_less_than_end = arc_segment.start_pt.row < arc_segment.end_pt.row;
            int start_row = start_less_than_end ? arc_segment.start_pt.row : arc_segment.end_pt.row;
            int end_row = start_less_than_end ? arc_segment.end_pt.row : arc_segment.start_pt.row;
            for(int row_index = start_row + 1; row_index < end_row; row_index++) {
                if(grid->chars[row_index][col_index] == HORIZONTAL_ARC_CHAR) {
                    grid->chars[row_index][col_index] = HORIZONTAL_AND_VERTICAL_ARC_CHAR;
                }
                else if(grid->chars[row_index][col_index] == EMPTY) {
                    grid->chars[row_index][col_index] = VERTICAL_ARC_CHAR;
                }
            }
        }
    }
}

// Draw the graph and then animate the traversal, by colouring each visited arc in turn.
static void animate_traversal(undirected_graph_t graph, long animation_delay) {
    // Initialise grid
    int height = graph.num_nodes * (NODE_HEIGHT + MIN_NODE_GAP * 2 + 2);
    int width = graph.num_nodes * (NODE_WIDTH + MIN_NODE_GAP * 2 + 2);
    char **chars = malloc(sizeof(char *) * height);
    grid_t grid = {chars, height, width};
    for(int row_index = 0; row_index < grid.height; row_index++) {
        grid.chars[row_index] = malloc(sizeof(char) * grid.width);
        for(int col_index = 0; col_index < grid.width; col_index++) {
            grid.chars[row_index][col_index] = EMPTY;
        }
    }
    // Put nodes on grid and calculate how to draw arcs between them
    location_t node_locations[graph.num_nodes]; // array mapping node indices to their location on grid (as given by centre)
    arc_segment_t arc_segments[graph.num_nodes * graph.num_nodes * MAX_SEGMENTS_PER_ARC];

    // Draw initial graph (no colouring)
    // Allows user to request redraws in case graph is unclear
    undirected_arc arcs_to_colour[graph.num_visited_arcs];
    int arcs_to_colour_index = 0;
    char *redraw_input;
    int num_arc_segments;
    do {
        for(int row_index = 0; row_index < grid.height; row_index++) {
            for(int col_index = 0; col_index < grid.width; col_index++) {
                grid.chars[row_index][col_index] = EMPTY;
            }
        }
        initialise_nodes(graph, &grid, node_locations);
        num_arc_segments = calc_all_arc_segs(graph, node_locations, arc_segments);
        draw_arcs(graph, &grid, arc_segments, num_arc_segments, arcs_to_colour, arcs_to_colour_index);
        print_grid(grid);
        printf("Redraw graph with nodes, arcs in different position? (Y or N) ");
        str_stdin(&redraw_input);
    } while(strcmp(redraw_input, "Y") == 0);


    undirected_arc *curr = graph.visited_arcs;
    while(curr) {
        system("clear");
        arcs_to_colour[arcs_to_colour_index] = *curr;
        draw_arcs(graph, &grid, arc_segments, num_arc_segments, arcs_to_colour, arcs_to_colour_index);
        print_grid(grid);
        clock_t finish = clock() + animation_delay;
        while(clock() < finish) {}
        arcs_to_colour_index++;
    }

    for(int row_index = 0; row_index < grid.height; row_index++) {
        free(grid.chars[row_index]);
    }
    free(grid.chars);
}

bool dfs_visualise(graph_union_t graph_union) {
    undirected_graph_t graph = graph_union.undirected_graph;
    bool exit = false;
    while(!exit) {
        printf("\n");
        printf("SELECT AN OPTION TO ANIMATE.\n");
        printf("Options are: 'PRINT', 'ANIMATE', 'NEXT', and 'EXIT'\n");
        printf("'PRINT' prints a comma-separated ordered list of visited nodes.\n");
        printf("'ANIMATE' displays the graph in text-graphical form, and then colours each visited arc in turn.\n");
        printf("'NEXT' proceeds to the next step of the algorithm, or terminates if we are finished.\n");
        printf("'EXIT' quits immediately.\n");
        char *user_choice;
        str_stdin(&user_choice);
        if(strcmp(user_choice, "PRINT") == 0) {
            print_traversal(graph);
        }
        else if(strcmp(user_choice, "ANIMATE") == 0) {
            printf("Enter delay between animation frames, in integer seconds:");
            char *animation_delay;
            str_stdin(&animation_delay);
            char *dummy_ptr;
            animate_traversal(graph, strtol(animation_delay, &dummy_ptr, 10));
        }
        else if(strcmp(user_choice, "NEXT") == 0) {
            printf("Proceeding to next step of algorithm.\n");
            exit = true;
        }
        else if(strcmp(user_choice, "EXIT") == 0) {
            return true;
        }
        else {
            printf("Not a valid option!\n");
        }
    }
    return false;
}
