CORBA Open Source Testing
=========================

This source tree contains several test suites for CORBA implementations in
C++ and Java. The code originally came from the OMG's CORBA Open Source
Testing project, which lived at cost.omg.org. That site no longer exists,
but some of it can be seen at the Internet Archive:

  http://web.archive.org/web/20030613010832/http://cost.omg.org/


All the contributing companies agreed to release their tests under the terms
of the GNU Lesser General Public License, available in the file COPYING.LIB.

There are two top-level directories:

 - tests/

     Contains tests that work with omniORB -- omniORB's own testsuite and
     C++ tests from Iona's ORBacus.

 - legacy/

     Contains C++ and Java test code from other vendors who joined the COST
     project, but that was not adapted for other ORBs including omniORB.
     This is kept for historical interest.
