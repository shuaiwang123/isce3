#!/usr/bin/env python3
#
# Author: Liang Yu
# Copyright 2019-

import numpy as np
from nisar.workflows import resampSlc

class resamp_opts:
    '''
    class to emulate argparse terminal input
    member values set to test basic functionality
    values can be adjusted to meet test requirements
    '''
    product = '../../../lib/isce/data/envisat.h5'
    frequency = 'A'
    polarization = 'HH'
    offsetdir = '../../../lib/isce/data/offsets'
    outdir = ''
    linesPerTile = 249


def test_resamp():
    '''
    run resample SLC without flattening and compare output against golden data
    '''
    # init inputs
    opts = resamp_opts()

    # run resamp
    resampSlc.main(opts)

    # load generated resampled SLC
    test_slc = np.fromfile('resamp_{}_{}.slc'.format(opts.frequency, opts.polarization), dtype=np.complex64).reshape(500,500)

    # load reference SLC generated by C++ resamp when initialized with same inputs
    ref_slc = np.fromfile('../../../lib/isce/data/warped_envisat.slc', dtype=np.complex64).reshape(500,500)

    # avoid image edges and get magnitude of mean difference
    mean_error = np.abs(np.mean(test_slc[20:-20,20:-20] - ref_slc[20:-20,20:-20]))

    # check if mean difference within bounds
    assert(mean_error < 1e-6)


if __name__ == '__main__':
    test_resamp()

# end of file
