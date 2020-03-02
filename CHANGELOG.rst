Unreleased changes
------------------

Version 3
---------
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
