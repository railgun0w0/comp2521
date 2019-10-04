// mkrand ... generate sequence of distinct random values in 1..max

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

int main (int argc, char *argv[])
{
	if (argc != 4)
		errx (EX_USAGE, "usage: %s <max> <#values> <seed>", argv[0]);

	int max = atoi (argv[1]);
	int nvals = atoi (argv[2]);
	srand ((unsigned int) atoi (argv[3]));

	if (nvals < 0)
		errx (EX_USAGE, "#nvals (%d) < 0", nvals);
	if (max < 0)
		errx (EX_USAGE, "max (%d) < 0", max);
	if (nvals > max / 3)
		errx (EX_USAGE, "#values (%d) << max (%d)", nvals, max);
	if (nvals > 1000)
		errx (EX_USAGE, "nvals (%d) > 1000", nvals);

	bool *already = calloc ((size_t) max, sizeof (bool));
	if (already == NULL)
		err (EX_OSERR, "couldn't allocate already-seen array");

	// BEWARE: loop body may not increment `sofar',
	//         and `sofar' is what leads to loop completion!
	// potential for infinite loop if rand() no good;
	// guaranteed infinite loop if nvals > max.
	// can become very s-l-o-w if nvals close to max.
	int sofar = 0;
	while (sofar < nvals) {
		int val = 1 + (rand () % max);
		if (! already[val]) {
			printf ("%d\n", val);
			already[val] = true;
			sofar++;
		}
	}

	free (already);

	return EXIT_SUCCESS;
}
