The jppy Python package
=======================

.. image:: https://git.km3net.de/km3py/jppy/badges/master/pipeline.svg
    :target: https://git.km3net.de/km3py/jppy/pipelines

.. image:: https://git.km3net.de/km3py/jppy/badges/master/coverage.svg
    :target: https://km3py.pages.km3net.de/jppy/coverage

.. image:: https://examples.pages.km3net.de/km3badges/docs-latest-brightgreen.svg
    :target: https://km3py.pages.km3net.de/jppy

This software provides access to the PDF functionalities in Jpp. It depends on
some Jpp headers (no Jpp compilation needed) and on the ROOT ``TMath`` (a
compiled ROOT installation is needed).

**Note:** Beware that this package is in the development phase, so the API will
change until version ``1.0.0`` is released!

Installation
============

Run ``make`` to compile the Python modules::

  make

Example
=======

Here is an example how to play around with muon PDFs using the ``jppy`` module::

    In [1]: import jppy

    In [2]: jppy.pdf.JMuonPDF?
    Docstring:      <no docstring>
    Init docstring: __init__(self: jppy.pdf.JMuonPDF, file_descriptor: str, TTS: float, number_of_points: int = 25, epsilon: float = 1e-10) -> None
    File:           ~/dev/pybind/jppy.pdf.cpython-37m-x86_64-linux-gnu.so
    Type:           pybind11_type
    Subclasses:

    In [3]:
    Do you really want to exit ([y]/n)? n

    In [3]: muon_pdf = jppy.pdf.JMuonPDF("/pbs/throng/km3net/src/Jpp/master/data/J%p.dat", 0)
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J1p.dat... OK
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J2p.dat... OK
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J3p.dat... OK
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J4p.dat... OK
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J5p.dat... OK
    loading input from file /pbs/throng/km3net/src/Jpp/master/data/J6p.dat... OK
    adding PDFs... OK

    In [4]: muon_pdf.calculate?
    Docstring: calculate(self: jppy.pdf.JMuonPDF, E: float, R: float, theta: float, phi: float, t1: float) -> JTOOLS::JResultPDF<double>
    Type:      method

    In [5]: result = muon_pdf.calculate(10, 5, 0, 0, 23)

    In [6]: result.f
    Out[6]: 0.0015357848280127355

    In [7]: result.fp
    Out[7]: -2.228096911939611e-05

    In [8]: result.v
    Out[8]: 0.02490276362751662

    In [9]: result.V
    Out[9]: 0.1164929687159807

    In [10]: %timeit muon_pdf.calculate(10, 5, 0, 0, 23)
    2.13 µs ± 242 ns per loop (mean ± std. dev. of 7 runs, 100000 loops each)
