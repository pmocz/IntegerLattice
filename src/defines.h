#ifndef DEFINES_H
#define DEFINES_H

/* Method -- choose whether to save memory */
#define METHOD_MEMSAVE


/* Resolution */
const int NX              = 1024;
const int NVX             = 1024;
const int NT              = 30;
const int OUTPUTEVERY     = 10;
const double TMAX         = 3.0;

/* Domain -L:L -V:V */
const double L            = 0.5;
const double V            = 1.0;
  
                    
const double DX           = ((2.0*L)/NX);
const double DVX          = ((2.0*V)/NVX);
const double DT           = TMAX / NT;

const int DIM             = 1;

#endif
