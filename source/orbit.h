/*
 * Orbit is a wrapper around the key functionality of STAR.  It supports
 * querying a previously built genome index with single reads and read pairs
 * and obtaining BAM records with their alignments to the index.
 */

#ifdef __cplusplus
extern "C" {
#endif
    // Aligner: a built index to which individual reads can be aligned
    struct Aligner;

    // align_read: align an individual read and get a string of BAM records
    const char* align_read(struct Aligner*, char*, char*, unsigned long long);
    
    // align_read_pair: align a pair of reads and get a string of BAM records
    const char* align_read_pair(struct Aligner*, char*, char*, char*, char*, unsigned long long);

    // init_aligner_clone: create an aligner from the same reference as an
    // existing aligner, sharing key structures with it and saving memory in
    // multi-threaded applications
    struct Aligner* init_aligner_clone(struct Aligner*);

    // init_aligner: initialize an aligner given the array of parameters which
    // would be passed to STAR
    struct Aligner* init_aligner(int, char*[]);

    // destory_aligner: frees the memory occupied by an aligner
    void destroy_aligner(struct Aligner*);

#ifdef __cplusplus
}
#endif

