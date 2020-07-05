/*********************************************************
  detector.h
  --------------------------------------------------------
  generated at Sat Jul 04 10:14:30 2020
  by snns2c ( Bernward Kett 1995 ) 
*********************************************************/

int detector_h(float *in, float *out, int init);

static struct {
  int NoOfInput;    /* Number of Input Units  */
  int NoOfOutput;   /* Number of Output Units */
  int(* propFunc)(float *, float*, int);
} detector_hREC = {1201,3,detector_h};
