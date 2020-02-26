#!/usr/bin/env python3
# coding=utf-8
# Filename: cherenkov_muon.py
# Author: Tamas Gal <tgal@km3net.de>
"""
Plot PDF of Cherenkov light from muon using Jpp interpolation tables.

Usage:
    cherenkov_muon.py [options]
    cherenkov_muon.py (-h | --help)

Options:
    -f FILE_DESCRIPTOR  Descriptor of the PDFs [default: $JPP_DIR/data/J%p.dat].
    --energy=<GeV>      Muon energy [default: 1e3].
    --distance=<m>      Distance of approach [default: 50].
    --tts=<ns>          PMT time smearing [default: 0.0].
    --xmin=<ns>         X-axis minimum [default: -20].
    --xmax=<ns>         X-axis maximum [default: 500].
    --ymin=<P/t>        Y-axis minimum [default: 1e-7].
    --ymax=<P/t>        Y-axis maximum [default: 1e-2].
    --n-points=<n>      Number of points to calculate [default: 1000].
    -o OUTFILE          Plot file [default: cherenkov_muon.png].
    -h --help           Show this screen.

"""
from docopt import docopt
docopt(__doc__)

import functools
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import jppy


def main():
    from docopt import docopt
    args = docopt(__doc__)

    directions = {
        "north": (0, 0),
        "east": (np.pi / 2, 0),
        "south": (np.pi, 0),
        "west": (np.pi / 2, np.pi)
    }

    muon_pdf = jppy.pdf.JMuonPDF(os.path.expandvars(args['-f']),
                                 TTS=float(args['--tts']))

    xs = np.linspace(float(args['--xmin']), float(args['--xmax']),
                     int(args['--n-points']))

    fig, ax = plt.subplots()

    for direction, (theta, phi) in directions.items():
        pdf = lambda t: muon_pdf.calculate(E=float(args['--energy']),
                                           R=float(args['--distance']),
                                           theta=theta,
                                           phi=phi,
                                           t1=t).f
        ax.plot(xs, list(map(pdf, xs)), label=direction)
        ax.set_yscale("log")

    plt.title(
        "PDF of Cherenkov light from muon with E = {} GeV and R = {} m".format(
            args['--energy'], args['--distance']))
    plt.ylim([float(args['--ymin']), float(args['--ymax'])])
    plt.ylabel(r"$\frac{dP}{dt}$ / (npe/ns)")
    plt.xlabel(r"$\Delta$t / ns")
    plt.legend()
    plt.grid(True)
    plt.savefig(args['-o'])
    print("The plot has been saved as {}".format(args['-o']))


if __name__ == '__main__':
    main()
