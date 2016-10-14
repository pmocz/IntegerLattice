#ifndef DEFINES_H
#define DEFINES_H

/* Method -- choose whether to save memory */
#define METHOD_MEMSAVE


/* Resolution */
#define NX           1024
#define NVX          1024
#define NT           30
#define TMAX         3.0
#define OUTPUTEVERY  10


/* Domain -L:L -V:V */
#define L            0.5
#define V            1.0
                    
#define DX           ((2.0*L)/NX)
#define DVX          ((2.0*V)/NVX)

#define DIM          1

#endif
