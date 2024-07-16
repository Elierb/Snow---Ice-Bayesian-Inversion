
#include <stdio.h>

#include "delaunay2d.h"

int main(int argc, char *argv[])
{
  delaunay2d_t *tri;
  bbox2d_t bound;

  int i, j;
  double x, y, z;
  int t0, t;

  FILE *fp;

  int width = 1024;
  int height = 1024;

  int nweights;
  int indices[DELAUNAY2D_MAXTRIANGLEEDGES];
  double weights[DELAUNAY2D_MAXTRIANGLEEDGES];
  
  tri = delaunay2d_create(10, -1.0, 1.0, -1.0, 1.0);
  if (tri == NULL) {
    fprintf(stderr, "error: failed to create delaunay\n");
    return -1;
  }

  if (delaunay2d_add(tri,
		     0.0, 0.0, 1.0,
		     &bound) < 0) {
    fprintf(stderr, "error: failed to add point\n");
    return -1;
  }
    
  for (i = 0; i < 4; i ++) {
    if (delaunay2d_set_value_of_index(tri, i, 0.0) < 0) {
      fprintf(stderr, "error: failed to set edge point values\n");
      return -1;
    }
  }

  if (delaunay2d_ct_update(tri) < 0) {
    fprintf(stderr, "error: failed to do linear update\n");
    return -1;
  }

  t0 = 0;
  fp = fopen("ct_delaunay.txt", "w");
  if (fp == NULL) {
    fprintf(stderr, "error: failed to create output file\n");
    return -1;
  }

  
  for (j = 0; j < height; j ++) {
    y = -1.0 + 2.0*((double)j + 0.5)/(double)height;

    for (i = 0; i < width; i ++) {

      x = -1.0 + 2.0*((double)i + 0.5)/(double)width;

      t = delaunay2d_ct_value_at_gradient(tri, t0, x, y, &z, &nweights, indices, weights);
      if (t < 0) {
	fprintf(stderr, "error: failed to get linear value\n");
	return -1;
      }

      t0 = t;
      fprintf(fp, "%15.9f ", z);
    }
    fprintf(fp, "\n");
  }

  fclose(fp);

  delaunay2d_destroy(tri);

  return 0;
}
