# This is an example test configuration file.
# It is actually a python module got imported by test-controller.py
# and remote-executor.py.
#


### conf_verbose
# Set to 1 to get more info on the progress of the testsuite.
conf_verbose = 1

### conf_build_test
# Before running a test, try to build the test executables on all the test
# machines. Set this variable to 1 to enable, 0 to skip.
# 
conf_build_test = 0

### conf_dump_program_output
# Set to 1 to see all the output from the test programs. Set to 0 to
# keep quiet unless the test programs fail. In that case, the program output
# will be dump to stdout regardless of the setting of this variable.
conf_dump_program_output = 1

### conf_host_list
# This is a sequence of sequence. Each element is a sequence of strings
# that refers to one host and testsuite directory. For each test, the
# controller will pick in order from this list a host-testsuite combo to
# fire up a server or client program. If the controller has to fire up more
# programs than the number of elements in this list, it will wrap around and
# starts from the first element again.
#
# The simplist and default configuration looks something like this
#   conf_host_list = [[ 'localhost', '/mnt/scratch/testsuite']]
#   This means that the controller should run the clients and servers all
#   on the same host using the same ORB. The testsuite is in the directory
#   /mnt/scratch/testsuite.
#
# Other variations are possible. For example, to test one ORB against another
# on the same host:
# conf_host_list = [
#                    [ 'localhost','/mnt/scratch/test_omni' ],
#                    [ 'localhost','/mnt/scratch/test_orbacus' ]
#                  ]
#   In this configuration and in each test case, an omniORB server will be
#   tested against an Orbacus client. This is so because in each of the test
#   cases, a convention has been adopted to start firstly the server and
#   lastly the client. If one reverse the entries then an Orbacus server will
#   be tested with an omniORB client.
#
# One can also run the test server and test clients on different platforms.
# For instance, to test a server running on linux (machine neem) against
# a client on NT (machine feijoa), here is the appropriate configuration:
#   
# conf_host_list = [
#                    [ 'neem','/mnt/scratch/test_omni' ],
#                    [ 'feijoa','D:/temp/test_omni' ]
#                  ]

### conf_executor_port
# The port number on which the remote-executor listen on
# Normally, you don't have to change this except when:
#    1. the port is already in use
# or 2. more than 1 user wants to run their own remote-executor on the same
#       machine.
# The value type is a string!
conf_executor_port = '9000'

### conf_iterations
# Exercise the whole testsuite this many times
conf_iterations = 1

### conf_test_entries
# By default, all the subdirectories containing a TEST_CONFIG file will be
# tested. This can be overridden with a specific subset by naming these
# subdirectories in conf_test_entries
conf_test_entries = []
