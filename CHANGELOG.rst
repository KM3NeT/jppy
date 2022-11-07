Unreleased changes
------------------

Version 3
---------
3.5.0 / 2022-11-07
~~~~~~~~~~~~~~~~~~
* ``JOscProb`` tools and lots of related functionalities has been added

3.4.1 / 2022-10-17
~~~~~~~~~~~~~~~~~~
* Fixed installation issue on macOS

3.4.0 / 2021-03-31
~~~~~~~~~~~~~~~~~~
* Added access to ``JPDF`` to (``jppy.pdf.JSinglePDF``) which can be used
  to access single, or a subset of PDF files

3.3.0 / 2020-03-31
~~~~~~~~~~~~~~~~~~
* Add the ``jppy.npe`` module which provides access to NPE tables in Jpp.
  The two new classes are ``JMuonNPE`` and ``JShowerNPE``.

3.2.2 / 2020-03-06
~~~~~~~~~~~~~~~~~~
* Binary wheel release on PyPI for macOS 10.15 and Linux

3.2.1 / 2020-03-05
~~~~~~~~~~~~~~~~~~
* c++17 support removed in favour of macOS compatibility

3.2 / 2020-03-02
~~~~~~~~~~~~~~~~
* shower PDF functionality added (``jppy.pdf.JShowerPDF``)

3.1 / 2020-02-28
~~~~~~~~~~~~~~~~
* removed ROOT dependency. ``jppy`` now only depends on a few Jpp headers

3.0 / 2020-02-27
~~~~~~~~~~~~~~~~
* **Major overhaul!** Dropped all the DAQ readout functionality in favour
  of `km3io https://git.km3net.de/km3py/km3io`_
* Introduced the ``jppy.pdf`` module which has a wrapper class ``JMuonPDF``
  to access Jpp PDF methods

Version 2
---------
2.1 / 2019-10-06
~~~~~~~~~~~~~~~
* Automatic discovery of libraries for ROOT 5 and 6

2.0 / 2018-12-20
~~~~~~~~~~~~~~~
* Support for Jpp 9+

Version 1
---------

1.6 / 2017-12-04
~~~~~~~~~~~~~~~
* depend on latest jpp trunk (method renaming grrrrr) (find in THRONG)
* fix build script (was broken in 1.5.1, apparently)

1.5.1 / 2017-11-21
~~~~~~~~~~~~~~~~~
* minor bugfixes. jfitreader now works (and has a km3pipe pump)
* DON'T USE

1.5.0 / 2017-11-21
~~~~~~~~~~~~~~~~~
* Add preliminary jfit reader

1.4.0 / 2017-10-19
~~~~~~~~~~~~~~~~~
* Adds timeslice and summary pump (preliminary)
