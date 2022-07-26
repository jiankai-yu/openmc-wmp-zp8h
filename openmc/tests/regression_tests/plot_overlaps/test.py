import glob
import hashlib
import os

import h5py
import openmc

from tests.testing_harness import TestHarness
from tests.regression_tests import config


class PlotTestHarness(TestHarness):
    """Specialized TestHarness for running OpenMC plotting tests."""
    def __init__(self, plot_names):
        super().__init__(None)
        self._plot_names = plot_names

    def _run_openmc(self):
        openmc.plot_geometry(openmc_exec=config['exe'])

    def _test_output_created(self):
        """Make sure *.png has been created."""
        for fname in self._plot_names:
            assert os.path.exists(fname), 'Plot output file does not exist.'

    def _cleanup(self):
        super()._cleanup()
        for fname in self._plot_names:
            if os.path.exists(fname):
                os.remove(fname)

    def _get_results(self):
        """Return a string hash of the plot files."""
        outstr = bytes()

        for fname in self._plot_names:
            if fname.endswith('.png'):
                # Add PNG output to results
                with open(fname, 'rb') as fh:
                    outstr += fh.read()
            elif fname.endswith('.h5'):
                # Add voxel data to results
                with h5py.File(fname, 'r') as fh:
                    outstr += fh.attrs['filetype']
                    outstr += fh.attrs['num_voxels'].tobytes()
                    outstr += fh.attrs['lower_left'].tobytes()
                    outstr += fh.attrs['voxel_width'].tobytes()
                    outstr += fh['data'][()].tobytes()

        # Hash the information and return.
        sha512 = hashlib.sha512()
        sha512.update(outstr)
        outstr = sha512.hexdigest()

        return outstr


def test_plot_overlap():
    harness = PlotTestHarness(('plot_1.png', 'plot_2.png', 'plot_3.png',
                               'plot_4.h5'))
    harness.main()
