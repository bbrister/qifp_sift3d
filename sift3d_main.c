/* -----------------------------------------------------------------------------
 * sift3d_main.c
 * -----------------------------------------------------------------------------
 * This file contains the CLI to extract SIFT3D descriptors for the Quantitaive
 * Imaging Feature Platform (QIFP). It takes in a DICOM series and a DSO and
 * outputs a single SIFT3D descriptor.
 * -----------------------------------------------------------------------------
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "imutil.h"
#include "immacros.h"
#include "sift.h"
#include "miniball.h"

#define DIM 3

const double scale_factor = 1.0; // Ratio of keypoint scale to lesion radius

/* Command:
 *       sift3d_main [dso] [output] args...
 */
int main(int argc, char *argv[]) {

        double center[DIM];
        double **points;
        double *buf;
        const char *dso_path, *im_path, *output_name;
        SIFT3D sift3d;
        Image im, mask;
        Keypoint_store kp;
        SIFT3D_Descriptor_store desc;
        double rad;
        int i, x, y, z, num_points;

        const int num_args = 2;
        const size_t point_size = DIM * sizeof(double);

        // Get the arguments
        if (argc < num_args) {
                fprintf(stderr, "Not enough arguments: %d need %d \n", argc,
                        num_args);
                return 1; 
        }
        dso_path = argv[1];
        output_name = argv[2];
        im_path = im_get_parent_dir(dso_path);

        // Initialize intermediates
        init_SIFT3D_Descriptor_store(&desc);
        init_im(&im);
        init_im(&mask);
        init_Keypoint_store(&kp);

        // Read the DSO
        if (im_read(dso_path, &mask)) {
                fprintf(stderr, "Failed to read DSO file %s \n", dso_path);
                return 1;
        }

        // Count the number of points in the DSO
        num_points = 0;
        assert(mask.nc == 1);
        SIFT3D_IM_LOOP_START(&mask, x, y, z)
                if (SIFT3D_IM_GET_VOX(&mask, x, y, z, 0))
                        num_points++;
        SIFT3D_IM_LOOP_END

        // Allocate a nested array of points
        if ((points = malloc(num_points * sizeof(double *))) == NULL ||
                (buf = malloc(num_points * point_size)) == NULL) {
                fprintf(stderr, "Out of memory! \n");
                return 1;
        }
        for (i = 0; i < num_points; i++) {
                points[i] = buf + point_size * i;
        }

        // Convert the DSO to a nested array of points
        i = 0;
        SIFT3D_IM_LOOP_START(&mask, x, y, z)
                if (!SIFT3D_IM_GET_VOX(&mask, x, y, z, 0))
                        continue;
                points[i][0] = x;
                points[i][1] = y;
                points[i][2] = z;
                i++;
        SIFT3D_IM_LOOP_END

        // Compute the minimum bounding sphere
        if (miniball(points, num_points, DIM, center, &rad)) {
                fputs("Failed to compute the miniball \n", stderr);
                return 1;
        }

        // Generate a keypoint from the sphere
        if (resize_Keypoint_store(&kp, 1))
                return 1;
        kp.buf->xd = center[0];
        kp.buf->yd = center[1];
        kp.buf->zd = center[2];
        kp.buf->sd = scale_factor * rad;
        //TODO detect rotation?

        // Read the image
        if (im_read(im_path, &im)) {
                fprintf(stderr, "Failed to read the input image: %s \n",
                        im_path);
                return 1;
        }

	// Initialize the SIFT data 
	if (init_SIFT3D(&sift3d)) {
		fprintf(stderr, "Failed to initialize SIFT data.");
                return 1;
        }

        // Parse the SIFT3D options and increment the argument list
        if ((argc = parse_args_SIFT3D(&sift3d, argc, argv, SIFT3D_FALSE)) < 0)
                return 1;

        // Extract the keypoint
        if (SIFT3D_extract_raw_descriptors(&sift3d, &im, &kp, &desc)) {
                fputs("Failed to extract descriptors \n", stderr);
                return 1;
        }

        // Write the descriptors to a file
        //TODO

        return 0;
}
