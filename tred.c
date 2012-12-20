#include "common.h"

void compute_tred_refs_mark(int p_top_index, paper_t *p_cur) {
    if (p_cur->tred_visit_index != p_top_index) {
        // haven't visited this paper yet
        p_cur->tred_visit_index = p_top_index;
        //p_cur.tredFollowBackLink = followBackLink;
        // visit all refs
        for (int i = 0; i < p_cur->num_refs; i++) {
            if (p_cur->refs_tred_computed[i] != 0) { // only follow refs that are in the transitively reduced graph
                paper_t *p_ref = p_cur->refs[i];
                if (p_ref->index < p_cur->index) { // allow only past references
                    compute_tred_refs_mark(p_top_index, p_ref);
                }
            }
        }
    }
}

/* Transitively reduce the graph */
void compute_tred(int num_papers, paper_t *papers) {
    // reset the visit id and tred computed number
    for (int i = 0; i < num_papers; i++) {
        paper_t *p = &papers[i];
        p->tred_visit_index = 0;
        // reset the tred_computed value
        for (int j = 0; j < p->num_refs; j++) {
            p->refs_tred_computed[j] = 0;
        }
    }

    // transitively reduce
    for (int i = 0; i < num_papers; i++) {
        paper_t *p = &papers[i];

        // clear the follow back pointer for this paper
        //p->tred_follow_back_link = NULL;

        // iterate all refs, from largest index to smallest index (youngest to oldest)
        for (int j = p->num_refs - 1; j >= 0; j--) {
            paper_t *p_past = p->refs[j];

            // only allow references to the past
            if (p_past->index >= p->index) {
                p->refs_tred_computed[j] = 1;
                continue;
            }

            if (p_past->tred_visit_index == p->index) {
                // have already visited this paper
                // follow this path; increase weight of ref path
                /*
                for (var link = paperPast.tredFollowBackLink; link != null && link.future != null; link = link.future.tredFollowBackLink) {
                    link.tredComputed += 1;
                }
                */
                continue;
            }

            // haven't visited this paper yet
            // mark link as belonging to tred graph and mark past
            p->refs_tred_computed[j] = 1;
            compute_tred_refs_mark(p->index, p_past);
        }
    }
}
